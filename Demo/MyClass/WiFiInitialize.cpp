#include "../StdAfx.h"
#include "WiFiInitialize.h"

#define LOGIN_TIMEOUT "Connection timed out"

CWiFiInitialize::CWiFiInitialize(void)
{
}

CWiFiInitialize::~CWiFiInitialize(void)
{

}

BOOL CWiFiInitialize::Clean()
{
	zeroIt(m_ScriptParam);
	zeroIt(m_RequiredParam);
	return TRUE;
}
BOOL CWiFiInitialize::SetParameter(LPSTR ParameterName,LPSTR ParameterValue)
{	
	if(strcmp(ParameterName,_T("IP_ADDRESS")) == NULL){
		strCopy(m_ScriptParam.IP_ADDRESS, ParameterValue);
		m_RequiredParam.IP_ADDRESS = TRUE;
	}else if(strcmp(ParameterName,_T("USERNAME")) == NULL){
		strCopy(m_ScriptParam.USERNAME, ParameterValue);
	}else if(strcmp(ParameterName,_T("PASSWORD")) == NULL){
		strCopy(m_ScriptParam.PASSWORD, ParameterValue);
	}else if(strcmp(ParameterName,_T("KEYWORD")) == NULL){
		strCopy(m_ScriptParam.KEYWORD, ParameterValue);
		m_RequiredParam.KEYWORD = TRUE;
	}else if(strcmp(ParameterName,_T("TIMEOUT")) == NULL){
		m_ScriptParam.TIMEOUT = atoi(ParameterValue);
		m_RequiredParam.TIMEOUT = TRUE;
	}
	return TRUE;
}
BOOL CWiFiInitialize::PreTest(LPSTR lpszRunInfo)
{
	return CheckScriptParameterIsFull(lpszRunInfo);
}
BOOL CWiFiInitialize::Test(BOOL bRetry, LPSTR lpszRunInfo)
{
	TCHAR szMessage[DLL_INFO_SIZE];
	TCHAR   szCmd[MAX_PATH]={0};
	TCHAR   szData[32]={0};
	TCHAR	szLocalPath[MAX_PATH]={0};
	CStringArray strAryCmd;	

	RegDeleteKey(HKEY_CURRENT_USER, "Software\\SimonTatham\\PuTTY\\SshHostKeys");
	PipeLineCleanData(g_GlobalInfo.PipeID, szMessage);
	GetLocalDirectory(szLocalPath,szMessage);
	while(ProcessFind("plink.exe", szMessage)){ProcessKill("plink.exe", szMessage);};
	sprintf_s(szCmd,MAX_PATH,"%splink.exe -ssh %s -l %s -pw %s", szLocalPath, m_ScriptParam.IP_ADDRESS, m_ScriptParam.USERNAME, m_ScriptParam.PASSWORD);

	strAryCmd.Add("y/n");
	strAryCmd.Add(LOGIN_TIMEOUT);
	strAryCmd.Add(m_ScriptParam.KEYWORD);
	
	if(!CCommFunc::PipeLineQueryMultiData(g_GlobalInfo.PipeID, szCmd, strAryCmd, m_ScriptParam.TIMEOUT, szMessage, DLL_INFO_SIZE)) 
		goto FAILED;
	if(strstr(szMessage, "y/n"))
	{
		if(!CCommFunc::PipeLineQueryData(g_GlobalInfo.PipeID, "n", m_ScriptParam.KEYWORD, m_ScriptParam.TIMEOUT, szMessage))
			goto FAILED;
	}else if(strstr(szMessage, LOGIN_TIMEOUT))
		goto FAILED;
	
	sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "%s", "DUT initialized successfully.");
	return TRUE;
FAILED:
	sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "%s", szMessage);
	return FALSE;
}
BOOL CWiFiInitialize::CheckScriptParameterIsFull(LPSTR lpszRunInfo)
{
	char szMessage[DLL_INFO_SIZE]={0};

	if(!m_RequiredParam.IP_ADDRESS)		strcat_s(szMessage,"IP_ADDRESS");
	if(!m_RequiredParam.TIMEOUT)		strcat_s(szMessage,";TIMEOUT");
	if(!m_RequiredParam.KEYWORD)		strcat_s(szMessage,";KEYWORD");

	if(strcmp(szMessage,"") == 0)
		return TRUE;
	else{
		strcpy_s(lpszRunInfo,DLL_INFO_SIZE,szMessage);
		return FALSE;
	}
}


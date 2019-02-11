#include "../StdAfx.h"
#include "LoadDutRequired.h"
#include <io.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#define STR_SSID_5G "$SSID_5G"
#define STR_SSID_2G "$SSID_2G"

CLoadDutRequired::CLoadDutRequired(void)
{
}

CLoadDutRequired::~CLoadDutRequired(void)
{
	
}

BOOL CLoadDutRequired::Clean()
{
	zeroIt(m_ScriptParam);
	zeroIt(m_RequiredParam);

	strCopy(m_ScriptParam.ERRORCODE, _T("T089"));
	strCopy(m_ScriptParam.RESPONSE, _T("#"));
	strCopy(m_ScriptParam.TYPE, _T("SSH"));
	m_ScriptParam.TIMEOUT = 3;
	return TRUE;
}
BOOL CLoadDutRequired::SetParameter(LPSTR ParameterName,LPSTR ParameterValue)
{	
	if(strcmp(ParameterName,_T("MODE")) == NULL){
		strCopy(m_ScriptParam.MODE, ParameterValue);
		m_ScriptParam.SettingMode = TRUE;
	}else if(strcmp(ParameterName,_T("TYPE")) == NULL){
		strCopy(m_ScriptParam.TYPE, ParameterValue);
	}else if(strcmp(ParameterName,_T("RESPONSE")) == NULL){
		strCopy(m_ScriptParam.RESPONSE, ParameterValue);
	}else if(strcmp(ParameterName,_T("ERRORCODE")) == NULL){
		strCopy(m_ScriptParam.ERRORCODE, ParameterValue);
		m_RequiredParam.ERRORCODE = TRUE;
	}else if(strcmp(ParameterName,_T("SEND")) == NULL){
		strCopy(m_ScriptParam.SEND, ParameterValue);
		m_RequiredParam.SEND = TRUE;
	}else if(strcmp(ParameterName,_T("TIMEOUT")) == NULL){
		m_ScriptParam.TIMEOUT = atoi(ParameterValue);
	}
	return TRUE;
}
BOOL CLoadDutRequired::PreTest(LPSTR lpszRunInfo)
{
	if(!SyncTestMode(lpszRunInfo)) return FALSE;
	if(!SyncTestType(lpszRunInfo)) return FALSE;
	if(!ReadEnvString(lpszRunInfo)) return FALSE;
	if(m_ScriptParam.SettingMode) return TRUE;
	return CheckScriptParameterIsFull(lpszRunInfo);
}
BOOL CLoadDutRequired::Test(BOOL bRetry, LPSTR lpszRunInfo, PERRORREPORT pErrorReport)
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szOutput[DLL_INFO_SIZE]={0};
	SearchFileContant("C:\\Users\\Administrator\\Downloads\\Rawdata.txt", "0F010001 = ", "\n", szOutput, szMessage);
	if(m_ScriptParam.SettingMode)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Set Mode:[%s], Type:[%s]", m_ScriptParam.MODE, m_ScriptParam.TYPE);
		return TRUE;
	}

	switch(m_TestParam.TestMode){
	case MODE_COMMAND:
		if(!CommandTest(szMessage))
			goto FAILED;
	}

	sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "%s", szMessage);
	return TRUE;

FAILED:
	strcpy_s(pErrorReport->ErrorCode, ERRORCODE_STRING, m_ScriptParam.ERRORCODE);
	pErrorReport->TestResult = FALSE;
	strcpy_s(pErrorReport->FailedTest, FAILTEST_STRING, "WIFI_LOAD_DUT_REQUIRED");
	pErrorReport->DisableSfcsLink = FALSE;
	strcpy_s(pErrorReport->ErrorMsg, ERRORMSG_STRING, szMessage);		
	return FALSE;

//ERRORED:
//	strcpy_s(pErrorReport->ErrorCode, ERRORCODE_STRING, _T("T089"));
//	pErrorReport->TestResult = FALSE;
//	strcpy_s(pErrorReport->FailedTest, FAILTEST_STRING, "WIFI_LOAD_DUT_REQUIRED");
//	pErrorReport->DisableSfcsLink = TRUE;
//	strcpy_s(pErrorReport->ErrorMsg, ERRORMSG_STRING, szMessage);		
//	return FALSE;
}
BOOL CLoadDutRequired::SyncTestMode(LPSTR lpszRunInfo)
{
	if(strcmp(m_ScriptParam.MODE, "COMMAND") == NULL)
		m_TestParam.TestMode = MODE_COMMAND;
	return TRUE;
}
BOOL CLoadDutRequired::SyncTestType(LPSTR lpszRunInfo)
{
	if(strcmp(m_ScriptParam.TYPE, "SSH") == NULL)
		m_TestParam.TestType = TYPE_SSH;
	return TRUE;
}
BOOL CLoadDutRequired::SyncCmdSend(LPSTR lpszRunInfo)
{
	CString strCmd, strKey;
	int addr;

	strCmd.Format("%s", m_ScriptParam.SEND);
	if( (addr = strCmd.Find(STR_SSID_5G)) > 0)
	{
		strCmd = strCmd.Left(addr) + m_SSID_5G + strCmd.Mid(addr+ strlen(STR_SSID_5G));
		strCopy(m_ScriptParam.SEND, strCmd.GetBuffer());							
	}else if( (addr = strCmd.Find(STR_SSID_2G)) > 0){
		strCmd = strCmd.Left(addr) + m_SSID_2G + strCmd.Mid(addr+ strlen(STR_SSID_5G));
		strCopy(m_ScriptParam.SEND, strCmd.GetBuffer());							
	}
	return TRUE;
}
BOOL CLoadDutRequired::CheckScriptParameterIsFull(LPSTR lpszRunInfo)
{
	char szMessage[DLL_INFO_SIZE]={0};

	if(m_TestParam.TestMode == MODE_COMMAND)
	{
		if(!m_RequiredParam.ERRORCODE)	strcat_s(szMessage,";ERRORCODE");
		if(!m_RequiredParam.SEND)		strcat_s(szMessage,";SEND");
	}

	if(strcmp(szMessage,"") == 0)
		return TRUE;
	else{
		strcpy_s(lpszRunInfo,DLL_INFO_SIZE,szMessage);
		return FALSE;
	}
}
BOOL CLoadDutRequired::ReadEnvString(LPSTR lpszRunInfo)
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};	
	TCHAR szLocalPath[MAX_PATH];
	TCHAR szFilePath[MAX_PATH];
	void *hndl;
	static BOOL bEnable=TRUE;

	if(!bEnable) return TRUE;
	GetLocalDirectory(szLocalPath,szMessage);
	sprintf_s(szFilePath, sizeof(szFilePath), "%s%s", szLocalPath, CONFIG_STATION_FILE);
	if(!CreateIni(&hndl, szFilePath, szMessage)) return FALSE;
	if(!GetIniKeyNameResult(hndl, "GLOBAL_FILE_PATH", "ENV_CONFIG_PATH", INI_TYPE_STRING, &szFilePath[0], szMessage)) return FALSE;
	if(!DestroyIni(hndl,szMessage)) return FALSE;

	if(!CheckFileExist(szFilePath,szMessage))
		sprintf_s(szFilePath, sizeof(szFilePath), "%s%s", szLocalPath, _T("TestProgramEnvSet\\EnvConfig.ini"));
	if(!CreateIni(&hndl, szFilePath, szMessage)) return FALSE;
	if(!GetIniKeyNameResult(hndl, "CONNECTION", STR_SSID_5G, INI_TYPE_STRING, &m_SSID_5G[0], lpszRunInfo)) return FALSE;
	if(!GetIniKeyNameResult(hndl, "CONNECTION", STR_SSID_2G, INI_TYPE_STRING, &m_SSID_2G[0], lpszRunInfo)) return FALSE;
	if(!DestroyIni(hndl,szMessage)) return FALSE;	

	bEnable = FALSE;
	return TRUE;
}
BOOL CLoadDutRequired::CommandTest(LPSTR lpszRunInfo)
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	BOOL bRtn = TRUE;

	if(!SyncCmdSend(lpszRunInfo)) return FALSE;

	switch(m_TestParam.TestType){
	case TYPE_SSH:
		bRtn = TestSSH(lpszRunInfo);
		break;
	}

	return bRtn;
}
BOOL CLoadDutRequired::TestSSH(LPSTR lpszRunInfo)
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};

	PipeLineCleanData(g_GlobalInfo.PipeID, szMessage);
	if(!CCommFunc::PipeLineQueryData(g_GlobalInfo.PipeID, m_ScriptParam.SEND, m_ScriptParam.RESPONSE, m_ScriptParam.TIMEOUT, szMessage))
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Query command is fail, [Command:%s, Response:%s]", szMessage, m_ScriptParam.RESPONSE);		
		return FALSE;
	}

	sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Query command is successful, [Command:%s, Response:%s]", szMessage, m_ScriptParam.RESPONSE);
	return TRUE;
}

BOOL CLoadDutRequired::SearchFileContant(LPSTR input, LPSTR start, LPSTR stop, LPSTR output, LPSTR lpszRunInfo)
{
	FILE *stream;
	int FileNo;
	long streamLength, ActuallyRead;
	TCHAR *Content = NULL;
	char *p1, *p2;
	TCHAR szRawData[DLL_INFO_SIZE]={0};


	if((stream = fopen(input, _T("rb"))) != NULL)
	{
		FileNo = _fileno(stream);
		streamLength = _filelength(FileNo);

		Content = (TCHAR*)malloc((size_t)(streamLength+1));
		memset(Content, 0x00, (streamLength+1));
		fseek(stream, SEEK_SET, 0);
		ActuallyRead = (long)fread(Content, sizeof(TCHAR), streamLength, stream);
		fclose(stream) ;

		Content = strstr(Content,start);
		Content = strtok(Content,stop);
		memcpy(output,p1,p2-p1);
//		p1 = strstr(Content,start) + strlen(start);
//		p2 = strstr(p1,stop);
		memcpy(output,p1,p2-p1);
		if(Content)	free(Content);
		return TRUE;
	}else{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Can't found file : %s",input);
		return FALSE;
	}
}

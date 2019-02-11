#include "../StdAfx.h"
#include "WifiConnectTest.h"

CWifiConnectTest::CWifiConnectTest(void)
{

}

CWifiConnectTest::~CWifiConnectTest(void){

}
BOOL CWifiConnectTest::Clean()
{
	zeroIt(m_ScriptParam);
	zeroIt(m_RequiredParam);
	zeroIt(m_TestParam);

	strCopy(m_ScriptParam.RESPONSE, _T("#"));
	m_ScriptParam.TIMEOUT = 3;
	return TRUE;
}
BOOL CWifiConnectTest::SetParameter(LPSTR ParameterName,LPSTR ParameterValue)
{	
	if(strcmp(ParameterName,_T("BAND")) == NULL){
		strCopy(m_ScriptParam.BAND, ParameterValue);
		m_RequiredParam.BAND = TRUE;
	}else if(strcmp(ParameterName,_T("ERRORCODE")) == NULL){
		strCopy(m_ScriptParam.ERRORCODE, ParameterValue);
		m_RequiredParam.ERRORCODE = TRUE;
	}else if(strcmp(ParameterName,_T("RESPONSE")) == NULL){
		strCopy(m_ScriptParam.RESPONSE, ParameterValue);
	}else if(strcmp(ParameterName,_T("TIMEOUT")) == NULL){
		m_ScriptParam.TIMEOUT = atoi(ParameterValue);
	}
	return TRUE;
}
BOOL CWifiConnectTest::PreTest(LPSTR lpszRunInfo)
{
	SyncTestBand(lpszRunInfo);
	return CheckScriptParameterIsFull(lpszRunInfo);
}
TM_RETURN CWifiConnectTest::Test(funcGetMessage cbListMessage, funcGetMessage cbXmlMessage, funcGetMessage cbSfcsSpecMessage, funcGetMessage cbSfcsResultMessage, ERRORREPORT &ErrorReport,int nRetryCnt,BOOL bRetry, LPSTR lpszDestIP, LPSTR lpszSrcIP)
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szCmd[MAX_PATH]={0};
	double dbTestTimeCost = GetTickCount();

	PipeLineCleanData(g_GlobalInfo.PipeID, szMessage);
	for(int items=0; items<2; items++)
	{
		if(items == 0)
			strCopy(szCmd, (m_TestParam.TestBand == BAND_2G)? _T("ifconfig ath1 down;ifconfig ath0 up"): _T("ifconfig ath0 down;ifconfig ath1 up"));
		else if(items == 1)
			strCopy(szCmd, (m_TestParam.TestBand == BAND_2G)? _T("iwlist ath0 scan > /dev/null 2>&1"): _T("iwlist ath1 scan > /dev/null 2>&1"));

		if(!CCommFunc::PipeLineQueryData(g_GlobalInfo.PipeID, szCmd, m_ScriptParam.RESPONSE, m_ScriptParam.TIMEOUT, szMessage))
		{
			sprintf_s(szMessage, DLL_INFO_SIZE, "Query command is fail, [Command:%s, Response:%s]", szMessage, m_ScriptParam.RESPONSE);		
			goto FAILED;
		}
	}

	if(!PingTest(lpszDestIP, m_ScriptParam.TIMEOUT, szMessage))
	{
		sprintf_s(szMessage,"Connect golden is fail,[IP:%s, TimeOut:%d]", lpszDestIP, m_ScriptParam.TIMEOUT);
		goto FAILED;
	}
	sprintf_s(szMessage,"Connect golden is successful,[IP:%s]\tTimeCost:%.3lf", lpszDestIP, (float)((GetTickCount() - dbTestTimeCost)/1000));

	CCommFunc::OutputMsgToBoth(cbListMessage, szMessage);

	//Collect test result to XML
	CCommFunc::ExportTestMessage(cbXmlMessage, "<TestCase name=\"%s\">","WIFI_CONNECT_TEST");
		CCommFunc::ExportTestMessage(cbXmlMessage, "<Data>");
			CCommFunc::ExportTestMessage(cbXmlMessage, "<SOURCE>%s</SOURCE>", lpszSrcIP);
			CCommFunc::ExportTestMessage(cbXmlMessage, "<DESTATION>%s</DESTATION>", lpszDestIP);
			CCommFunc::ExportTestMessage(cbXmlMessage,"<TEST_TIME>%.3f</TEST_TIME>", (float)((GetTickCount() - dbTestTimeCost)/1000));
		CCommFunc::ExportTestMessage(cbXmlMessage, "</Data>");
	CCommFunc::ExportTestMessage(cbXmlMessage, "</TestCase>");

	return TM_RETURN_PASS;

FAILED:
	CCommFunc::OutputMsgToBoth(cbListMessage, szMessage);
	ErrorReport.DisableSfcsLink = FALSE;
	ErrorReport.TestResult=FALSE;
	strCopy(ErrorReport.ErrorCode, m_ScriptParam.ERRORCODE);
	strCopy(ErrorReport.ErrorDetail,_T("NULL"));	
	strCopy(ErrorReport.FailedTest,_T("WIFI_CONNECT_TEST"));
	strCopy(ErrorReport.ErrorMsg,szMessage);
	return TM_RETURN_FAIL;

//ERRORD:
//	ErrorReport.DisableSfcsLink = TRUE;
//	ErrorReport.TestResult=FALSE;
//	strCopy(ErrorReport.ErrorCode,"T089");
//	strCopy(ErrorReport.ErrorDetail,_T("NULL"));	
//	strCopy(ErrorReport.FailedTest,_T("WIFI_CONNECT_TEST"));
//	strCopy(ErrorReport.ErrorMsg,szMessage);
//	return TM_RETURN_ERROR;
}
BOOL CWifiConnectTest::CheckScriptParameterIsFull(LPSTR lpszRunInfo)
{
	char szMessage[DLL_INFO_SIZE]={0};
	if(!m_RequiredParam.BAND)		strcat_s(szMessage,"BAND");
	if(!m_RequiredParam.ERRORCODE)	strcat_s(szMessage,";ERRORCODE");


	if(strcmp(szMessage,"") == 0)
		return TRUE;
	else{
		strcpy_s(lpszRunInfo,DLL_INFO_SIZE,szMessage);
		return FALSE;
	}
}
BOOL CWifiConnectTest::SyncTestBand(LPSTR lpszRunInfo)
{
	if(strcmp(m_ScriptParam.BAND, "2G") == NULL)
		m_TestParam.TestBand = BAND_2G;
	else if(strcmp(m_ScriptParam.BAND, "5G") == NULL)
		m_TestParam.TestBand = BAND_5G;

	return TRUE;
}

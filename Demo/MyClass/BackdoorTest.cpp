#include "../StdAfx.h"
#include "BackdoorTest.h"


CBackdoorTest::CBackdoorTest(void)
{

}

CBackdoorTest::~CBackdoorTest(void)
{

}
BOOL CBackdoorTest::Clean()
{
	zeroIt(m_ScriptParam);
	zeroIt(m_RequiredParam);
	return TRUE;
}
BOOL CBackdoorTest::SetParameter(LPSTR ParameterName,LPSTR ParameterValue)
{	
	return TRUE;
}
BOOL CBackdoorTest::PreTest(LPSTR lpszRunInfo)
{
	return CheckScriptParameterIsFull(lpszRunInfo);
}

BOOL CBackdoorTest::SendData(LPSTR szCmd, LPSTR szData, DWORD &dwBytesRead, LPSTR lpszRunInfo)
{
	TCHAR szResponds[DLL_INFO_SIZE]={0};
	if(strcmp(szCmd, "BackdoorPacketLoadSerialNum_Req") == NULL)
	{
		sprintf_s(szData, DLL_INFO_SIZE, "%s%s", szCmd, _T("AAA$BBB"));
		dwBytesRead = strlen(szData);
	}
	return TRUE;
}


BOOL CBackdoorTest::Response(LPSTR szCmd, LPSTR szData, DWORD dwBytesRead, LPSTR lpszRunInfo)
{
	TCHAR szBuffer[DLL_INFO_SIZE];
	if(strcmp(szCmd, "BackdoorPacketGetEepromInfo_Req") == NULL)
	{
		memcpy(szBuffer, szData, dwBytesRead);
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Confirm the EEPROM format is successful.");
	}

	return TRUE;
}


BOOL CBackdoorTest::CheckScriptParameterIsFull(LPSTR lpszRunInfo)
{
	char szMessage[DLL_INFO_SIZE]={0};

	if(strcmp(szMessage,"") == 0)
		return TRUE;
	else{
		strcpy_s(lpszRunInfo,DLL_INFO_SIZE,szMessage);
		return FALSE;
	}
}
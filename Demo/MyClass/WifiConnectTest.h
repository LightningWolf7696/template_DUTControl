#pragma once

#include "../CommFunc.h"

class CWifiConnectTest
{
public:
	CWifiConnectTest(void);
public:
	~CWifiConnectTest(void);	

private:
	enum TEST_BAND
	{
		BAND_NULL = 0,
		BAND_2G,
		BAND_5G,
	};
	typedef struct _SCRIPT_PARAMETER
	{
		TCHAR BAND[MAX_PATH];
		TCHAR ERRORCODE[MAX_PATH];
		TCHAR RESPONSE[DLL_INFO_SIZE];
		int	  TIMEOUT;
	}SCRIPT_PARAMETER;
	typedef struct _REQUIRED_SCRIPT_PARAMETER
	{
		BOOL	BAND;
		BOOL	ERRORCODE;
	}REQUIRED_SCRIPT_PARAMETER;

	typedef struct _TEST_PARAMETER
	{
		TEST_BAND TestBand;
	}TEST_PARAMETER;


	SCRIPT_PARAMETER				m_ScriptParam;
	REQUIRED_SCRIPT_PARAMETER		m_RequiredParam;
	TEST_PARAMETER					m_TestParam;
public:
	BOOL Clean();
	BOOL SetParameter(LPSTR ParameterName,LPSTR ParameterValue);
	BOOL PreTest(LPSTR lpszRunInfo);
	TM_RETURN Test(funcGetMessage cbListMessage, funcGetMessage cbXmlMessage, funcGetMessage cbSfcsSpecMessage, funcGetMessage cbSfcsResultMessage, ERRORREPORT &ErrorReport,int nRetryCnt,BOOL bRetry, LPSTR lpszDestIP, LPSTR lpszSrcIP);
	BOOL CheckScriptParameterIsFull(LPSTR lpszRunInfo);
	BOOL SyncTestBand(LPSTR lpszRunInfo);
};

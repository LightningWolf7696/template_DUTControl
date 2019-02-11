#pragma once
#include "../CommFunc.h"

class CWiFiInitialize
{
public:
	CWiFiInitialize(void);
public:
	~CWiFiInitialize(void);

private:
	typedef struct _SCRIPT_PARAMETER
	{
		TCHAR IP_ADDRESS[MAX_PATH];
		TCHAR USERNAME[MAX_PATH];
		TCHAR PASSWORD[MAX_PATH];
		TCHAR KEYWORD[MAX_PATH];
		int	  TIMEOUT;
	}SCRIPT_PARAMETER;
	typedef struct _REQUIRED_SCRIPT_PARAMETER
	{
		BOOL	IP_ADDRESS;
		BOOL	TIMEOUT;
		BOOL	KEYWORD;
	}REQUIRED_SCRIPT_PARAMETER;

	SCRIPT_PARAMETER				m_ScriptParam;
	REQUIRED_SCRIPT_PARAMETER		m_RequiredParam;


public:
	BOOL Clean();
	BOOL SetParameter(LPSTR ParameterName,LPSTR ParameterValue);
	BOOL PreTest(LPSTR lpszRunInfo);
	BOOL Test(BOOL bRetry, LPSTR lpszRunInfo);

private:
	BOOL CheckScriptParameterIsFull(LPSTR lpszRunInfo);

};

#pragma once

#include "../CommFunc.h"

class CBackdoorTest
{
public:
	CBackdoorTest(void);
public:
	~CBackdoorTest(void);

private:
	typedef struct _SCRIPT_PARAMETER
	{
	}SCRIPT_PARAMETER;
	typedef struct _REQUIRED_SCRIPT_PARAMETER
	{
	}REQUIRED_SCRIPT_PARAMETER;

	SCRIPT_PARAMETER				m_ScriptParam;
	REQUIRED_SCRIPT_PARAMETER		m_RequiredParam;


public:
	BOOL Clean();
	BOOL SetParameter(LPSTR ParameterName,LPSTR ParameterValue);
	BOOL PreTest(LPSTR lpszRunInfo);
	BOOL SendData(LPSTR szCmd, LPSTR szData, DWORD &dwBytesRead, LPSTR lpszRunInfo);
	BOOL Response(LPSTR szCmd, LPSTR szData, DWORD dwBytesRead, LPSTR lpszRunInfo);

private:
	BOOL CheckScriptParameterIsFull(LPSTR lpszRunInfo);



};

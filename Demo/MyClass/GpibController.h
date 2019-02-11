#pragma once
#include "../CommFunc.h"

class CGpibController
{
public:
	CGpibController(void);
public:
	~CGpibController(void);

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
	BOOL Test();

private:
	BOOL CheckScriptParameterIsFull(LPSTR lpszRunInfo);

};

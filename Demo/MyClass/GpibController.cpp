#include "../StdAfx.h"
#include "GpibController.h"

CGpibController::CGpibController(void)
{
}

CGpibController::~CGpibController(void)
{

}

BOOL CGpibController::Clean()
{
	zeroIt(m_ScriptParam);
	zeroIt(m_RequiredParam);
	return TRUE;
}
BOOL CGpibController::SetParameter(LPSTR ParameterName,LPSTR ParameterValue)
{	
	return TRUE;
}
BOOL CGpibController::PreTest(LPSTR lpszRunInfo)
{
	return CheckScriptParameterIsFull(lpszRunInfo);
}
BOOL CGpibController::Test()
{
	TCHAR szMessage[DLL_INFO_SIZE];
	TCHAR   szCmd[MAX_PATH]={0};
	BOOL	bResult;


	//Write
	strCopy(szCmd, "*IDN?");
	bResult = g_GlobalInfo.fWriteGpib(g_GlobalInfo.GpibID, szCmd, szMessage);

	//Read
	zeroIt(szMessage);
	bResult = g_GlobalInfo.fReadGpib(g_GlobalInfo.GpibID, szMessage);

	//Query
	zeroIt(szMessage);
	bResult = g_GlobalInfo.fQueryGpib(g_GlobalInfo.GpibID, szCmd, szMessage);


	return TRUE;
}
BOOL CGpibController::CheckScriptParameterIsFull(LPSTR lpszRunInfo)
{
	return TRUE;
}


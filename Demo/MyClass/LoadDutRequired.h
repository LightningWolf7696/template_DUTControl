#pragma once

#include "../CommFunc.h"

class CLoadDutRequired
{
public:
	CLoadDutRequired(void);
public:
	~CLoadDutRequired(void);

private:
	enum TEST_MODE
	{
		MODE_NULL = 0,
		MODE_COMMAND
	};
	enum TEST_TYPE
	{
		TYPE_NULL = 0,
		TYPE_SSH
	};

	typedef struct _SCRIPT_PARAMETER
	{
		TCHAR MODE[MAX_PATH];
		TCHAR TYPE[MAX_PATH];
		TCHAR RESPONSE[DLL_INFO_SIZE];
		TCHAR ERRORCODE[MAX_PATH];
		TCHAR SEND[MAX_PATH];
		int	  TIMEOUT;
		BOOL  SettingMode;
	}SCRIPT_PARAMETER;
	typedef struct _REQUIRED_SCRIPT_PARAMETER
	{
		BOOL	SEND;
		BOOL	ERRORCODE;
	}REQUIRED_SCRIPT_PARAMETER;

	typedef struct _TEST_PARAMETER
	{
		TEST_MODE	TestMode;
		TEST_TYPE	TestType;
	}TEST_PARAMETER;


	SCRIPT_PARAMETER				m_ScriptParam;
	REQUIRED_SCRIPT_PARAMETER		m_RequiredParam;
	TEST_PARAMETER					m_TestParam;
	TCHAR							m_SSID_5G[MAX_PATH];
	TCHAR							m_SSID_2G[MAX_PATH];

public:
	BOOL Clean();
	BOOL SetParameter(LPSTR ParameterName,LPSTR ParameterValue);
	BOOL PreTest(LPSTR lpszRunInfo);
	BOOL Test(BOOL bRetry, LPSTR lpszRunInfo, PERRORREPORT pErrorReport);
	BOOL ReadEnvString(LPSTR lpszRunInfo);

private:
	BOOL CheckScriptParameterIsFull(LPSTR lpszRunInfo);
	BOOL SyncTestMode(LPSTR lpszRunInfo);
	BOOL SyncTestType(LPSTR lpszRunInfo);
	BOOL SyncCmdSend(LPSTR lpszRunInfo);
	BOOL CommandTest(LPSTR lpszRunInfo);
	BOOL TestSSH(LPSTR lpszRunInfo);
	BOOL SearchFileContant(LPSTR input, LPSTR start, LPSTR stop, LPSTR output, LPSTR lpszRunInfo);
//
//public:
//	enum _ITEM{
//		LOAD_ENV_INI=0,
//		PLINK_INIT,
//		PROCESS_KILL,
//		REG_DELETE
//	};
//	typedef  struct FLAG{
//		BOOL	LOAD_ENV_INI;
//		BOOL	PLINK_INIT;
//		BOOL	REG_DELETE;
//		BOOL	PROCESS_KILL;
//	}FLAG;
//public:
//	FLAG	Flag;
//	BOOL	res;
//	CString szWorkName;
//	TCHAR	szMessage[DLL_INFO_SIZE];
//	TCHAR   UsserName[32];
//	TCHAR	PassWord[32];
//public:
////	void Clean();
//	BOOL START(FULL_CONNECT_PARAM &m_Full_Connect_Param,LPSTR lpszRunInfo);
//	BOOL OpenEnvConfigFile(FULL_CONNECT_PARAM &m_Full_Connect_Param,LPSTR lpszRunInfo);
//	BOOL PipePlinkInit(int &Pipeid,LPSTR szCmd,LPSTR UserName,LPSTR PassWord,LPSTR szKeyWord,UINT TimeOut);
//	BOOL RegDelete(LPSTR Key);
};

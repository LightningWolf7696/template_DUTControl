#pragma once

#include "resource.h"		// main symbols
#include "../../Common/MVTDef.h"
#include "DemoDef.h"
#include "../../Common/UtilityDllDef.h"
#include "MyDef.h"

#define CONSOLE_MAX_LENGTH 10000
#define PIPE_MAX_LENGTH  10000
#define TELNET_MAX_LENGTH 10000
#define SOCKET_MAX_LENGTH 10000
#define szSeparateKey  "\n"
#define UART_MESSAGE _T("UART_Message.txt")
#define UART_COMMAND _T("UART_Command.txt")
#define PIPE_MESSAGE _T("PIPE_Message.txt")
#define PIPE_COMMAND _T("PIPE_Command.txt")
#define TELNET_MESSAGE _T("TELNET_Message.txt")
#define TELNET_COMMAND _T("TELNET_Command.txt")
#define SOCKET_MESSAGE _T("SOCKET_Message.txt")
#define SOCKET_COMMAND _T("SOCKET_Command.txt")

static enum INI_TYPE
{
	INI_TYPE_STRING = 0,
	INI_TYPE_INTEGER = 1,
	INI_TYPE_LONG = 2,
	INI_TYPE_FLOAT =3,
	INI_TYPE_DOUBLE =4,
	INI_TYPE_BOOL = 5
};

typedef enum _RELAY_OUT{
	OUT_PORT1 =1,
	OUT_PORT2,
	OUT_PORT3,
	OUT_PORT4
}RELAY_OUT;

typedef enum _RELAY_WORKING{
	RELAY_OFF = 0,
	RELAY_ON
}RELAY_WORKING;

#define RealyParameter(Array, Port1, Port2, Port3, Port4) \
{                     \
	Array[0] = Port1; \
	Array[1] = Port2; \
	Array[2] = Port3; \
	Array[3] = Port4; \
}

extern HWND						g_hParentWnd;

class CCommFunc
{
public:
	CCommFunc(void);
public:
	~CCommFunc(void);

public:
	static BOOL SerialPortQueryDataEx(int nCom, LPSTR szInput, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo,  LPSTR szTestKeyWord="", LPSTR szTestKeySuccess="");
	static BOOL SerialPortGetKeyWordEx(int nCom, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, int ReturnSize,  LPSTR szTestKeyWord="", LPSTR szTestKeySuccess="");
	static BOOL SerialPortQueryDataHex(int nCom, UCHAR* szInput, DWORD nInputSize, UCHAR* szKeyWord, DWORD nKeyWordSize, UINT nTimeOutSec,LPSTR lpszRunInfo);
	static BOOL SerialPortGetKeyWordHex(int nCom, UCHAR* szKeyWord, DWORD nKeyWordSize, UINT nTimeOutSec,LPSTR lpszRunInfo);
	static BOOL TelnetGetKeyWord(int nID, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, LPSTR szRecvMsg);
	static BOOL PipeLineQueryData(int nID, LPSTR szInput, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo);
	static BOOL PipeLineQueryMultiData(int nID, LPSTR szInput, const CStringArray& strAryKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, int nRtnSize);
	static BOOL PipeLineGetKeyWord(int nID, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo);
	static BOOL SocketGetKeyWord(int nID, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, LPSTR szRecvMsg);
	static BOOL ExportTestMessage(funcGetMessage cbMessage, const TCHAR *format, ...);
	static BOOL ExtractString(LPSTR szSrc, LPSTR szDest, int nDestLens, LPSTR szStart, LPSTR szEnd);
	static BOOL CombineMessage(LPSTR lpszSrcData, LPSTR lpszAvailData, int nAvailLens, LPSTR szKey);
	static BOOL DelFolderAllFile(LPSTR lpszDelPath, LPSTR lpszCmd, LPSTR lpszRunInfo);
	static BOOL OutputCommand(LPSTR szFileName, const TCHAR *format, ...);
	static BOOL OutputMessage(LPSTR szFileName, const TCHAR *format, ...);
	static CString TraceStringMid(LPSTR szMessage, LPSTR szStart, LPSTR szEnd);
	static BOOL CloseRelayAll(UINT nComPort, LPSTR lpszRunInfo);
	static BOOL WriteUsbRelayOnePort(UINT nComPort, RELAY_OUT nOutNum, RELAY_WORKING bMode, LPSTR lpszRunInfo);
	static BOOL WriteUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo);
	static BOOL ReadUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo);
	static BOOL QueryUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo);
	static BOOL ExportTestLable(const COLORREF *Color,const TCHAR *format, ...);
	static BOOL OutputMsgToBoth(funcGetMessage cbMessage, const TCHAR *format, ...);
	static LABELDATA   g_LabelData;

};

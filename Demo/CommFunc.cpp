#include "StdAfx.h"
#include "CommFunc.h"

LABELDATA  CCommFunc::g_LabelData;

CCommFunc::CCommFunc(void)
{	
}

CCommFunc::~CCommFunc(void)
{
}

BOOL CCommFunc::SerialPortQueryDataEx(int nCom, LPSTR szInput, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, LPSTR szTestKeyWord, LPSTR szTestKeySuccess)
{
	char szMessage[CONSOLE_MAX_LENGTH]={0},szTemp[CONSOLE_MAX_LENGTH];
	BOOL bRtn = TRUE;
	double dbStartTime = GetTickCount();
	double dbTestTime = GetTickCount();
	int nBootTestKeyTime = 5;
	DWORD dwBytesRead;	
	TCHAR szCmd[1024]={0};
	int   nFailCnt=0;
	TCHAR szTimeMsg[DLL_INFO_SIZE]={0};
	CString strMessage="";
	int	  nLens;

	OutputCommand(UART_COMMAND, szInput);
	sprintf_s(szCmd,sizeof(szCmd),"%s\r",szInput);
	if(!SerialPortWriteDataEx(nCom, szCmd, strlen(szCmd), lpszRunInfo)) 
		return FALSE;
	int n=0;
	do
	{
		n++;
		zeroIt(szTemp);
		if(!SerialPortReadDataEx(nCom, szTemp, sizeof(szTemp), dwBytesRead, lpszRunInfo))
			return FALSE;
		strMessage += szTemp;

		Sleep(10);
		if(((GetTickCount() - dbStartTime)/1000) > nTimeOutSec)
		{				
			bRtn = FALSE;
			break;
		}else if((strlen(szTestKeyWord) > 0) && (strMessage == "")) {//Check DUT is ready.
			if(((GetTickCount() - dbTestTime)/1000) > nBootTestKeyTime)
			{
				if(SerialPortWriteDataEx(nCom, szTestKeyWord, strlen(szTestKeyWord), lpszRunInfo))
				{
					Sleep(100);
					if(!SerialPortReadDataEx(nCom, szTemp, sizeof(szTemp), dwBytesRead, lpszRunInfo))
						return FALSE;
					if(strstr(szTemp, szTestKeySuccess))
						return TRUE;
				}
				dbTestTime = GetTickCount();
			}
		}
		if(strMessage.Find(szKeyWord) != SEARCH_NOT_FOUND)
			break;
		Sleep(100);
	} while(TRUE);

	memset(lpszRunInfo,0x00,DLL_INFO_SIZE);
	zeroIt(szMessage);
	strMessage.Replace("%","p");
	nLens = strMessage.GetLength();
	if(nLens > 500)
		sprintf_s(szMessage,DLL_INFO_SIZE,strMessage.Right(500));
	else
		sprintf_s(szMessage,DLL_INFO_SIZE,"%s",strMessage.GetBuffer());

	if(bRtn){
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"%s",szMessage);	
	}else{
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Get serial port keyword is timeout, [KeyWord = %s, TimeOut = %d]",szKeyWord,nTimeOutSec);
		if(nLens >= LOG_MAX_SIZE)
			ExportTestLog(strMessage.Right(LOG_MAX_SIZE-1));
		else
			ExportTestLog(strMessage);
	}


	OutputMessage(UART_MESSAGE, strMessage.GetBuffer());
	return bRtn;
}

BOOL CCommFunc::SerialPortGetKeyWordEx(int nCom, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, int ReturnSize, LPSTR szTestKeyWord, LPSTR szTestKeySuccess)
{
	char szMessage[CONSOLE_MAX_LENGTH]={0},szTemp[CONSOLE_MAX_LENGTH];
	BOOL bRtn = TRUE;
	double dbStartTime = GetTickCount();
	double dbTestTime = GetTickCount();
	int nBootTestKeyTime = 5;
	DWORD dwBytesRead;
	CString strMessage="";
	int	  nLens;
	double dwTestTime = GetTickCount();
	double dwCostTime;
	CString strCurr="";
	BOOL  bTrigger=FALSE;

	do
	{
		zeroIt(szTemp);
		if(!SerialPortReadDataEx(nCom, szTemp, sizeof(szTemp), dwBytesRead, lpszRunInfo))
			return FALSE;
		strMessage += szTemp;
		Sleep(10);
		dwCostTime = (GetTickCount() - dwTestTime)/1000;
		if(dwCostTime > nBootTestKeyTime)
		{
			dwTestTime = GetTickCount();
			bTrigger = (strCurr == strMessage)? TRUE : FALSE;
			strCurr = strMessage;
		}
		dwTestTime = dwCostTime - dbStartTime;

		if( ((GetTickCount() - dbStartTime)/1000) > nTimeOutSec)
		{
			bRtn = FALSE;
			break;
		}else if((strlen(szTestKeyWord) > 0) && ((strMessage == "") || bTrigger)) {//Check DUT is ready.
			if(((GetTickCount() - dbTestTime)/1000) > nBootTestKeyTime)
			{
				if(SerialPortWriteDataEx(nCom, szTestKeyWord, strlen(szTestKeyWord), lpszRunInfo))
				{
					Sleep(100);
					if(!SerialPortReadDataEx(nCom, szTemp, sizeof(szTemp), dwBytesRead, lpszRunInfo))
						return FALSE;
					if(strstr(szTemp, szTestKeySuccess))
						return TRUE;
				}
				dbTestTime = GetTickCount();
			}
		}		
		if(strMessage.Find(szKeyWord) != SEARCH_NOT_FOUND)
			break;
		Sleep(10);
	} while (TRUE);
	memset(lpszRunInfo,0x00,DLL_INFO_SIZE);
	zeroIt(szMessage);
	strMessage.Replace("%","p");
	nLens = strMessage.GetLength();
	if(nLens >= ReturnSize)
		sprintf_s(szMessage,ReturnSize,strMessage.Right(ReturnSize-1));
	else
		sprintf_s(szMessage,ReturnSize,"%s",strMessage.GetBuffer());

	if(bRtn){
		sprintf_s(lpszRunInfo,ReturnSize,"%s",szMessage);	
	}else{
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Get serial port keyword is timeout, [KeyWord = %s, TimeOut = %d]",szKeyWord,nTimeOutSec);
		if(nLens >= LOG_MAX_SIZE)
			ExportTestLog(strMessage.Right(LOG_MAX_SIZE-1));
		else
			ExportTestLog(strMessage);
	}	


	OutputMessage(UART_MESSAGE, strMessage.GetBuffer());
	return bRtn;
}

BOOL CCommFunc::SerialPortQueryDataHex(int nCom, UCHAR* szInput, DWORD nInputSize, UCHAR* szKeyWord, DWORD nKeyWordSize, UINT nTimeOutSec,LPSTR lpszRunInfo)
{
	TCHAR szMessage[CONSOLE_MAX_LENGTH]={0},szTemp[CONSOLE_MAX_LENGTH];
	BOOL bRtn = TRUE;
	double dbStartTime = GetTickCount();
	double dbTestTime = GetTickCount();
	//	int nBootTestKeyTime = 5;
	DWORD dwBytesRead;	
	//	TCHAR szCmd[1024]={0};
	//	int   nFailCnt=0;
	//	TCHAR szTimeMsg[DLL_INFO_SIZE]={0};
	CString strMessage="";
	int	  nLens;
	TCHAR  szExpect[CONSOLE_MAX_LENGTH]={0};
	TCHAR szByte[3]={0};
	unsigned char * pustr;

	for(DWORD i=0; i<nKeyWordSize; i++)
	{
		sprintf_s(szByte,sizeof(szByte),"%02X",szKeyWord[i]);
		strcat_s(szExpect, szByte);
	}
	for(DWORD i=0; i<nInputSize; i++)
	{
		sprintf_s(szByte,sizeof(szByte),"%02X",szInput[i]);
		strcat_s(szMessage, szByte);
	}

	OutputMessage(UART_MESSAGE, "[COM%d, WRITE] %s", nCom, szMessage);
	OutputCommand(UART_COMMAND,szMessage);
	if(!SerialPortWriteDataEx(nCom, (LPSTR)szInput, nInputSize,lpszRunInfo)) return FALSE;

	nLens = 0;
	do
	{
		zeroIt(szTemp);
		if(!SerialPortReadDataEx(nCom, szTemp, sizeof(szTemp), dwBytesRead, lpszRunInfo))
			//		if(!SerialPortReadDataEx(nCom, szTemp, 5, dwBytesRead, lpszRunInfo))
			return FALSE;
		pustr=(unsigned char*)szTemp; 
		for(DWORD i=0; i<dwBytesRead; i++)
		{
			sprintf_s(szByte,sizeof(szByte),"%02X",pustr[i]);
			strMessage += szByte;
		}
		nLens += dwBytesRead;


		Sleep(10);
		if(((GetTickCount() - dbStartTime)/1000) > nTimeOutSec)
		{				
			bRtn = FALSE;
			break;
		}
		if(strMessage.Find(szExpect) != SEARCH_NOT_FOUND)
			break;
	} while(TRUE);

	memset(lpszRunInfo,0x00,DLL_INFO_SIZE);
	zeroIt(szMessage);
	nLens = strMessage.GetLength();
	if(nLens > 500)
		sprintf_s(szMessage,DLL_INFO_SIZE,strMessage.Right(500));
	else
		sprintf_s(szMessage,DLL_INFO_SIZE,"%s",strMessage.GetBuffer());

	if(bRtn){
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"%s",szMessage);	
	}else{
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Get serial port keyword is timeout, [KeyWord = %s, TimeOut = %d]",szKeyWord,nTimeOutSec);
		if(nLens >= LOG_MAX_SIZE)
			ExportTestLog(strMessage.Right(LOG_MAX_SIZE-1));
		else
			ExportTestLog(strMessage);
	}


	OutputMessage(UART_MESSAGE, "[COM%d, READ] %s", nCom, strMessage.GetBuffer());
	return bRtn;
}

BOOL CCommFunc::SerialPortGetKeyWordHex(int nCom, UCHAR* szKeyWord, DWORD nKeyWordSize, UINT nTimeOutSec,LPSTR lpszRunInfo)
{
	TCHAR szMessage[CONSOLE_MAX_LENGTH]={0},szTemp[CONSOLE_MAX_LENGTH];
	BOOL bRtn = TRUE;
	double dbStartTime = GetTickCount();
	double dbTestTime = GetTickCount();
	DWORD dwBytesRead;	
	CString strMessage="";
	int	  nLens;
	TCHAR  szExpect[CONSOLE_MAX_LENGTH]={0};
	TCHAR szByte[3]={0};
	unsigned char * pustr;

	for(DWORD i=0; i<nKeyWordSize; i++)
	{
		sprintf_s(szByte,sizeof(szByte),"%02X",szKeyWord[i]);
		strcat_s(szExpect, szByte);
	}

	nLens = 0;
	do
	{
		zeroIt(szTemp);
		if(!SerialPortReadDataEx(nCom, szTemp, sizeof(szTemp), dwBytesRead, lpszRunInfo))
			//		if(!SerialPortReadDataEx(nCom, szTemp, 5, dwBytesRead, lpszRunInfo))
			return FALSE;
		pustr=(unsigned char*)szTemp; 
		for(DWORD i=0; i<dwBytesRead; i++)
		{
			sprintf_s(szByte,sizeof(szByte),"%02X",pustr[i]);
			strMessage += szByte;
		}
		nLens += dwBytesRead;


		Sleep(10);
		if(((GetTickCount() - dbStartTime)/1000) > nTimeOutSec)
		{				
			bRtn = FALSE;
			break;
		}
		if(strMessage.Find(szExpect) != SEARCH_NOT_FOUND)
			break;
	} while(TRUE);

	memset(lpszRunInfo,0x00,DLL_INFO_SIZE);
	zeroIt(szMessage);
	nLens = strMessage.GetLength();
	if(nLens > 500)
		sprintf_s(szMessage,DLL_INFO_SIZE,strMessage.Right(500));
	else
		sprintf_s(szMessage,DLL_INFO_SIZE,"%s",strMessage.GetBuffer());

	if(bRtn){
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"%s",szMessage);	
	}else{
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Get serial port keyword is timeout, [KeyWord = %s, TimeOut = %d]",szKeyWord,nTimeOutSec);
		if(nLens >= LOG_MAX_SIZE)
			ExportTestLog(strMessage.Right(LOG_MAX_SIZE-1));
		else
			ExportTestLog(strMessage);
	}


	OutputMessage(UART_MESSAGE, "[COM%d, READ] %s", nCom, strMessage.GetBuffer());
	return bRtn;
}

BOOL CCommFunc::TelnetGetKeyWord(int nID, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, LPSTR szRecvMsg)
{
	char szMessage[TELNET_MAX_LENGTH]={0},szTemp[TELNET_MAX_LENGTH];
	BOOL bRtn = TRUE;
	double dbStartTime = GetTickCount();
	double dbTestTime = GetTickCount();	

	do
	{
		zeroIt(szTemp);
		if(!TelnetReadData(nID, szTemp, lpszRunInfo))
			return FALSE;
		if((strlen(szMessage)+strlen(szTemp)) >= sizeof(szMessage))
			strCopy(szMessage,&szMessage[strlen(szMessage)-100]);
		strcat_s(szMessage,strlen(szMessage)+strlen(szTemp)+1,szTemp);
		Sleep(10);
		if(((GetTickCount() - dbStartTime)/1000) > nTimeOutSec)
		{
			sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Get serial port keyword is timeout, [KeyWord = %s, TimeOut = %d]",szKeyWord,nTimeOutSec);
			bRtn = FALSE;
			break;
		}
	} while (!strstr(szMessage, szKeyWord));

	strcpy_s(szRecvMsg,TELNET_MAX_LENGTH,szMessage);
	return bRtn;
}

BOOL CCommFunc::PipeLineQueryData(int nID, LPSTR szInput, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo)
{
	TCHAR szMessage[PIPE_MAX_LENGTH],szTemp[PIPE_MAX_LENGTH];
	BOOL bRtn = TRUE;
	DWORD dbStartTime = GetTickCount();
	DWORD dbTestTime = GetTickCount();
	TCHAR szCmd[1024]={0};
	CString strMessage="";
	int	  nLens;

	OutputCommand(PIPE_COMMAND, szInput);
	sprintf_s(szCmd,sizeof(szCmd),"%s\n",szInput);
	if(!PipeLineWriteData(nID, szCmd,lpszRunInfo)) return FALSE;
	do
	{
		zeroIt(szTemp);
		if(!PipeLineReadData(nID, szTemp, sizeof(szTemp), lpszRunInfo))
			return FALSE;
		strMessage += szTemp;

		Sleep(10);
		if(((GetTickCount() - dbStartTime)/1000) > nTimeOutSec)
		{
			bRtn = FALSE;
			break;
		}
		if(strMessage.Find(szKeyWord) != SEARCH_NOT_FOUND)
			break;
	} while(TRUE);

	memset(lpszRunInfo,0x00,DLL_INFO_SIZE);
	zeroIt(szMessage);
	nLens = strMessage.GetLength();
	if(nLens > (DLL_INFO_SIZE-1))
		sprintf_s(szMessage,DLL_INFO_SIZE,strMessage.Right(DLL_INFO_SIZE-1));
	else
		sprintf_s(szMessage,DLL_INFO_SIZE,"%s",strMessage.GetBuffer());

	if(bRtn){
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"%s",szMessage);
	}else{
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Get pipe keyword is timeout, [KeyWord = %s, TimeOut = %d]",szKeyWord,nTimeOutSec);
		if(nLens >= LOG_MAX_SIZE)
			ExportTestLog(strMessage.Right(LOG_MAX_SIZE-1));
		else
			ExportTestLog(strMessage);
	}


	OutputMessage(PIPE_MESSAGE, strMessage.GetBuffer());
	return bRtn;
}

BOOL CCommFunc::PipeLineGetKeyWord(int nID, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo)
{
	char szMessage[PIPE_MAX_LENGTH]={0},szTemp[PIPE_MAX_LENGTH];
	BOOL bRtn = TRUE;
	double dbStartTime = GetTickCount();
	double dbTestTime = GetTickCount();
	CString strMessage="";
	int nLens;


	do
	{
		zeroIt(szTemp);
		if(!PipeLineReadData(nID, szTemp, sizeof(szTemp), lpszRunInfo))
			return FALSE;
		strMessage += szTemp;
		Sleep(10);
		if(((GetTickCount() - dbStartTime)/1000) > nTimeOutSec)
		{
			bRtn = FALSE;
			break;
		}
		if(strMessage.Find(szKeyWord) != SEARCH_NOT_FOUND)
			break;
	} while (TRUE);

	memset(lpszRunInfo,0x00,DLL_INFO_SIZE);
	zeroIt(szMessage);
	nLens = strMessage.GetLength();
	if(nLens > 500)
		sprintf_s(szMessage,DLL_INFO_SIZE,strMessage.Right(500));
	else
		sprintf_s(szMessage,DLL_INFO_SIZE,"%s",strMessage.GetBuffer());

	if(bRtn){
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"%s",szMessage);	
	}else{
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Get pipe keyword is timeout, [KeyWord = %s, TimeOut = %d]",szKeyWord,nTimeOutSec);
		if(nLens >= LOG_MAX_SIZE)
			ExportTestLog(strMessage.Right(LOG_MAX_SIZE-1));
		else
			ExportTestLog(strMessage);
	}	

	OutputMessage(PIPE_MESSAGE, strMessage.GetBuffer());
	return bRtn;
}

BOOL CCommFunc::SocketGetKeyWord(int nID, LPSTR szKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, LPSTR szRecvMsg)
{
	char szMessage[SOCKET_MAX_LENGTH]={0},szTemp[SOCKET_MAX_LENGTH];
	BOOL bRtn = TRUE;
	double dbStartTime = GetTickCount();
	double dbTestTime = GetTickCount();	
	DWORD dwBytesRead;
	do
	{
		zeroIt(szTemp);
		if(!SocketReceive(szTemp, sizeof(szTemp)-1, dwBytesRead, lpszRunInfo))
			return FALSE;
		if((strlen(szMessage)+strlen(szTemp)) >= sizeof(szMessage))
			strCopy(szMessage,&szMessage[strlen(szMessage)-100]);
		strcat_s(szMessage,strlen(szMessage)+strlen(szTemp)+1, szTemp);
		Sleep(10);
		if(((GetTickCount() - dbStartTime)/1000) > nTimeOutSec)
		{
			sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Get socket keyword is timeout, [KeyWord = %s, TimeOut = %d]",szKeyWord,nTimeOutSec);
			bRtn = FALSE;
			break;
		}
	} while (!strstr(szMessage, szKeyWord));

	strcpy_s(szRecvMsg,SOCKET_MAX_LENGTH,szMessage);
	return bRtn;
}

BOOL CCommFunc::ExportTestMessage(funcGetMessage cbMessage, const TCHAR *format, ...)
{
	TCHAR szBuffer[DLL_INFO_SIZE];
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szBuffer, sizeof(szBuffer), format, marker);
	va_end(marker);
	cbMessage(szBuffer);
	return TRUE;
}

BOOL CCommFunc::ExtractString(LPSTR szSrc, LPSTR szDest, int nDestLens, LPSTR szStart, LPSTR szEnd)
{
	CString strSrc,strTemp,strDest;
	int nAddr;
	strSrc.Format(_T("%s"),szSrc);
	nAddr = strSrc.Find(szStart);
	strTemp = strSrc.Mid(nAddr+strlen(szStart));
	nAddr = strTemp.Find(szEnd);
	if(nAddr != -1)
		strDest = strTemp.Left(strTemp.Find(szEnd));	
	strDest = strDest.Trim();
	sprintf_s(szDest,nDestLens,"%s",strDest.GetBuffer());
	return TRUE;
}

BOOL CCommFunc::CombineMessage(LPSTR lpszSrcData, LPSTR lpszAvailData, int nAvailLens, LPSTR szKey)
{
	TCHAR *pszToken;
	CString strKey,strSrcData;
	int nAddr;
	pszToken = strstr(lpszSrcData, szKey);
	if(pszToken)
	{
		nAddr = pszToken-lpszSrcData;
		strSrcData.Format(_T("%s"),lpszSrcData);
		//create available data
		strKey = strSrcData.Left(nAddr-1);//Skip chr(13)
		strcpy_s(lpszAvailData,nAvailLens,strKey);

		//separate unused data
		strKey = strSrcData.Mid(nAddr+1);
		strcpy_s(lpszSrcData, SOCKET_MAX_LENGTH, strKey);
		return TRUE;
	}

	return FALSE;
}

BOOL CCommFunc::DelFolderAllFile(LPSTR lpszDelPath, LPSTR lpszCmd, LPSTR lpszRunInfo)
{
	char szMessage[DLL_INFO_SIZE]={0};
	char szDestFileName[MAX_PATH]={0};
	FILE *stream = NULL;
	errno_t err;

	//Check delete folder is exist or not.
	if(!CheckFileExist(lpszDelPath,lpszRunInfo))
		return FALSE;

	sprintf_s(szDestFileName,sizeof(szDestFileName),"%s\\%s",lpszDelPath,"MyDelete.bat");	
	if((err = fopen_s(&stream, szDestFileName, "w")) != NULL)
	{
		sprintf_s(lpszRunInfo,DLL_INFO_SIZE,"Can't create  cableLoss from DUT.[Name:%s]",szDestFileName);
		return FALSE;
	}

	fprintf(stream,"cd %s\n",lpszDelPath);
	fprintf(stream,"%c:\n",lpszDelPath[0]);
	fprintf(stream,"%s\n",lpszCmd);
	fclose(stream);

	WinExec(szDestFileName,SC_MINIMIZE);

	//Delete bat file
	//	if(CheckFileExist(szDestFileName,lpszRunInfo)) DeleteFile(szDestFileName);

	return TRUE;
}

BOOL CCommFunc::OutputCommand(LPSTR szFileName, const TCHAR *format, ...)
{
	DWORD dwWriteByte = 0;

	//#ifdef _DEBUG
	TCHAR szBuffer[65535];
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szLocalPath[MAX_PATH]={0};
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szBuffer, sizeof(szBuffer), format, marker);
	va_end(marker);

	CString strLogPath;

	if(GetLocalDirectory(szLocalPath,szMessage) != ERROR_SUCCESS)
		strLogPath = _T("c:\\");
	strLogPath.Format("%s",szLocalPath);
	strLogPath += szFileName;

	CString strOutput;// = cTime.Format(_T("%Y-%m-%d %H:%M:%S "));

	strOutput += szBuffer;
	strOutput += _T("\r\n");

	HANDLE hFile = ::CreateFile(	strLogPath, 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE,
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwRC = ::SetFilePointer(hFile,
			0,
			NULL,
			FILE_END);

		//always append
		if(dwRC != INVALID_SET_FILE_POINTER)
			::WriteFile(hFile, strOutput, (DWORD)strOutput.GetLength()*sizeof(TCHAR), &dwWriteByte, NULL);

		::CloseHandle(hFile);
	}
	//#endif

	return dwWriteByte;
}

BOOL CCommFunc::OutputMessage(LPSTR szFileName, const TCHAR *format, ...)
{
	DWORD dwWriteByte = 0;

	//#ifdef _DEBUG
	TCHAR szBuffer[65535];
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	TCHAR szLocalPath[MAX_PATH]={0};
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szBuffer, sizeof(szBuffer), format, marker);
	va_end(marker);

	CString strLogPath;

	if(GetLocalDirectory(szLocalPath,szMessage) != ERROR_SUCCESS)
		strLogPath = _T("c:\\");
	strLogPath.Format("%s",szLocalPath);
	strLogPath += szFileName;

	CString strOutput;// = cTime.Format(_T("%Y-%m-%d %H:%M:%S "));

	strOutput += szBuffer;
	strOutput += _T("\r\n");

	HANDLE hFile = ::CreateFile(	strLogPath, 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE,
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwRC = ::SetFilePointer(hFile,
			0,
			NULL,
			FILE_END);

		//always append
		if(dwRC != INVALID_SET_FILE_POINTER)
			::WriteFile(hFile, strOutput, (DWORD)strOutput.GetLength()*sizeof(TCHAR), &dwWriteByte, NULL);

		::CloseHandle(hFile);
	}
	//#endif

	return dwWriteByte;
}

CString CCommFunc::TraceStringMid(LPSTR szMessage, LPSTR szStart, LPSTR szEnd)
{
	CString strMsg="";
	int addr;

	strMsg.Format("%s",szMessage);
	addr = strMsg.Find(szStart);
	if(addr < 0 ) return _T("");
	strMsg = strMsg.Mid(addr+strlen(szStart));
	addr = strMsg.Find(szEnd);
	if(addr < 0) return _T("");
	return strMsg.Left(addr).Trim();
}



BOOL CCommFunc::QueryUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo)
{
	DWORD dwCount=0, dwBytesRead=0;
	TCHAR szExpectData[8]={0x55,0x01,0x01,0x00,0x00,0x00,0x00,0x00};
	TCHAR szActualData[8]={0};

	for(int nAddr=0; nAddr<4; nAddr++)
		szExpectData[nAddr+3] = (PortCtrl[nAddr])? 0x02:0x01;

	for(int nAddr=0; nAddr<7; nAddr++)
		szExpectData[7] += szExpectData[nAddr];

	if(!SerialPortWriteDataEx(nComPort, szExpectData, sizeof(szExpectData), lpszRunInfo))
		return FALSE;

	if(!SerialPortReadDataEx(nComPort,szActualData,sizeof(szActualData),dwBytesRead, lpszRunInfo))
		return FALSE;

	if(szActualData[0] != 0x22)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay return failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
			nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7]);
		return FALSE;
	}
	for(int nAddr=3; nAddr<7; nAddr++)
	{
		if(szExpectData[nAddr] != szActualData[nAddr])
		{
			sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay write failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x;Expect:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
				nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7],
				szExpectData[0],szExpectData[1],szExpectData[2],szExpectData[3],szExpectData[4],szExpectData[5],szExpectData[6],szExpectData[7]);
			return FALSE;
		}
	}

	for(int nAddr=0; nAddr<4; nAddr++)
		PortCtrl[nAddr] = (szActualData[nAddr+3] == 0x02)? TRUE:FALSE;

	return TRUE;
}

BOOL CCommFunc::WriteUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo)
{
	DWORD dwCount=0, dwBytesRead=0;
	TCHAR szData[8]={0x55,0x01,0x01,0x00,0x00,0x00,0x00,0x00};

	for(int nAddr=0 ; nAddr<4 ; nAddr++)
		szData[nAddr+3] = (PortCtrl[nAddr])? 0x02:0x01;

	for(int nAddr=0 ; nAddr<7 ; nAddr++)
		szData[7] += szData[nAddr];

	if(!SerialPortWriteDataEx(nComPort, szData, sizeof(szData), lpszRunInfo))
		return FALSE;

	return TRUE;
}

BOOL CCommFunc::ReadUsbRelay(UINT nComPort, BOOL *PortCtrl, LPSTR lpszRunInfo)
{
	DWORD dwCount=0, dwBytesRead=0;
	TCHAR szData[8]={0x55,0x01,0x0,0x00,0x00,0x00,0x00,0x56};
	TCHAR szMessage[DLL_INFO_SIZE]={0};

	if(!SerialPortWriteDataEx(nComPort, szData, sizeof(szData), lpszRunInfo))
		return FALSE;
	if(!SerialPortReadDataEx(nComPort,szData,sizeof(szData),dwBytesRead, lpszRunInfo))
		return FALSE;

	if(szData[0] != 0x22)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay return failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
			nComPort,szData[0],szData[1],szData[2],szData[3],szData[4],szData[5],szData[6],szData[7]);
		return FALSE;
	}
	for(int nAddr=0 ; nAddr<4 ; nAddr++)
		PortCtrl[nAddr] = (szData[nAddr+3] == 0x02)? TRUE:FALSE;

	return TRUE;
}

BOOL CCommFunc::CloseRelayAll(UINT nComPort, LPSTR lpszRunInfo)
{
	DWORD dwCount=0, dwBytesRead=0;
	TCHAR szExpectData[8]={0x55,0x01,0x01,0x01,0x01,0x01,0x01,0x5B};
	TCHAR szActualData[8]={0};

	if(!SerialPortWriteDataEx(nComPort, szExpectData, sizeof(szExpectData), lpszRunInfo))
		return FALSE;

	Sleep(100);
	if(!SerialPortReadDataEx(nComPort,szActualData,sizeof(szActualData),dwBytesRead, lpszRunInfo))
		return FALSE;

	if(szActualData[0] != 0x22)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay return failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
			nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7]);
		return FALSE;
	}
	for(int nAddr=3; nAddr<7; nAddr++)
	{
		if(szActualData[nAddr] == 0x02)
		{
			sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay write failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x;Expect:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
				nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7],
				szExpectData[0],szExpectData[1],szExpectData[2],szExpectData[3],szExpectData[4],szExpectData[5],szExpectData[6],szExpectData[7]);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CCommFunc::WriteUsbRelayOnePort(UINT nComPort, RELAY_OUT nOutNum, RELAY_WORKING bMode, LPSTR lpszRunInfo)
{
	DWORD dwCount=0, dwBytesRead=0;
	TCHAR szExpectData[8]={0x55,0x01,0x12,0x00,0x00,0x00,0x00,0x00};
	TCHAR szActualData[8]={0};

	szExpectData[2] = (bMode == RELAY_ON)? 0x12 : 0x11;

	szExpectData[6] = nOutNum;

	szExpectData[7]=0;
	for(int nAddr=0; nAddr<7; nAddr++)
		szExpectData[7] += szExpectData[nAddr];

	if(!SerialPortWriteDataEx(nComPort, szExpectData, sizeof(szExpectData), lpszRunInfo))
		return FALSE;

	Sleep(100);
	if(!SerialPortReadDataEx(nComPort,szActualData,sizeof(szActualData),dwBytesRead, lpszRunInfo))
		return FALSE;

	if(szActualData[0] != 0x22)
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay return failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
			nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7]);
		return FALSE;
	}

	if( (szActualData[6] & (1 << (nOutNum-1))) != (bMode << (nOutNum-1)))
	{
		sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Relay write failed,[COM:%d; Actual:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x;Expect:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x]",
			nComPort,szActualData[0],szActualData[1],szActualData[2],szActualData[3],szActualData[4],szActualData[5],szActualData[6],szActualData[7],
			szExpectData[0],szExpectData[1],szExpectData[2],szExpectData[3],szExpectData[4],szExpectData[5],szExpectData[6],szExpectData[7]);
		return FALSE;
	}

	return TRUE;
}

BOOL CCommFunc::ExportTestLable(const COLORREF *Color,const TCHAR *format, ...)
{
	TCHAR szBuffer[MAX_PATH];
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szBuffer, sizeof(szBuffer), format, marker);
	va_end(marker);

	if(!Color || (strcmp(szBuffer, _T("")) == 0)) return FALSE;

	g_LabelData.Color = *Color;
	g_LabelData.Keep = FALSE;
	strcpy_s(g_LabelData.LabelName, sizeof(g_LabelData.LabelName), szBuffer);

	::SendMessage(g_hParentWnd, WM_ERASELABEL, (WPARAM)NULL, (LPARAM)&g_LabelData);
	::SendMessage(g_hParentWnd, WM_CREATELABEL, (WPARAM)NULL, (LPARAM)&g_LabelData);

	return TRUE;
}

BOOL CCommFunc::OutputMsgToBoth(funcGetMessage cbMessage, const TCHAR *format, ...)
{
	TCHAR szBuffer[DLL_INFO_SIZE];
	va_list marker;
	va_start(marker, format);
	vsprintf_s(szBuffer, sizeof(szBuffer), format, marker);
	va_end(marker);
	cbMessage(szBuffer);
	ExportTestLog("%s",szBuffer);
	return TRUE;
}

BOOL CCommFunc::PipeLineQueryMultiData(int nID, LPSTR szInput, const CStringArray& strAryKeyWord, UINT nTimeOutSec,LPSTR lpszRunInfo, int nRtnSize)
{
	TCHAR szMessage[PIPE_MAX_LENGTH],szTemp[PIPE_MAX_LENGTH];
	BOOL bRtn = FALSE;
	DWORD dbStartTime = GetTickCount();
	DWORD dbTestTime = GetTickCount();
	TCHAR szCmd[DLL_INFO_SIZE]={0};
	CString strMessage="";
	int	  nLens;
	CString strKey;

	OutputCommand(PIPE_COMMAND, szInput);
	sprintf_s(szCmd,sizeof(szCmd),"%s\n",szInput);
	if(!PipeLineWriteData(nID, szCmd,lpszRunInfo)) return FALSE;
	do
	{
		do 
		{
			zeroIt(szTemp);
			if(!PipeLineReadData(nID, szTemp, sizeof(szTemp), lpszRunInfo))
				return FALSE;
			strMessage += szTemp;

			Sleep(10);
		} while (strlen(szTemp)>0);
		if(((GetTickCount() - dbStartTime)/1000) > nTimeOutSec)
		{
			bRtn = FALSE;
			break;
		}
		for(int i=0 ; i<strAryKeyWord.GetSize() ; i++)
		{
			strKey = strAryKeyWord.GetAt(i);
			if(strMessage.Find(strKey) != SEARCH_NOT_FOUND)
			{
				bRtn=TRUE;
				break;
			}
		}
	} while(!bRtn);

	memset(lpszRunInfo,0x00,nRtnSize);
	zeroIt(szMessage);
	strMessage.Replace("%","p");
	nLens = strMessage.GetLength();
	if(nLens > (nRtnSize-1))
		sprintf_s(szMessage,nRtnSize,strMessage.Right(nRtnSize-1));
	else
		sprintf_s(szMessage,nRtnSize,"%s",strMessage.GetBuffer());

	if(bRtn){
		sprintf_s(lpszRunInfo,nRtnSize,"%s",szMessage);	
	}else{
		zeroIt(szMessage);
		for(int i=0 ; i<strAryKeyWord.GetSize() ; i++){
			strcat_s(szMessage,strAryKeyWord.GetAt(i));
			strcat_s(szMessage,";");
		}
		sprintf_s(lpszRunInfo,nRtnSize,"Get pipe keyword is timeout, [KeyWord = %s, TimeOut = %d]",szMessage,nTimeOutSec);
		if(nLens >= LOG_MAX_SIZE)
			ExportTestLog(strMessage.Right(LOG_MAX_SIZE-1));
		else
			ExportTestLog(strMessage);
	}


	OutputMessage(PIPE_MESSAGE, "%s", strMessage.GetBuffer());
	return bRtn;
}

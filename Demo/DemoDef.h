#ifndef _DEMO_DEFINE_H__
#define _DEMO_DEFINE_H__
#define	DLLEXPORT	__declspec(dllexport)
typedef BOOL (CALLBACK *funcGetMessage)(LPSTR lpszRunInfo);
typedef ERRORREPORT (CALLBACK *funcGetActualReport)();
typedef BOOL (CALLBACK *funcGetGpibID)(int &ud, int nBoardNum, int nPAddr, int nSAddr, LPSTR lpszRunInfo);
typedef BOOL (CALLBACK *funcQueryGpib)(int ud, LPSTR lpszCmd, LPSTR lpszRunInfo);
typedef BOOL (CALLBACK *funcReadGpib)(int ud, LPSTR lpszRunInfo);
typedef BOOL (CALLBACK *funcWriteGpib)(int ud, LPSTR lpszCmd, LPSTR lpszRunInfo);
typedef BOOL (CALLBACK *funcAssignVariable)(BOOL IsNew, LPSTR ParameterName, LPSTR ParameterValue, int size);

extern "C" BOOL PASCAL EXPORT LOAD_GUI_INFORMATION(LPSTR lpszOP, LPSTR lpszBarcode, LPSTR lpszPN, SFCS_MODE eSfcsMode, BOOL bStopOnFailed, HWND hParentWnd, LPSTR lpszBarcode2, funcGetActualReport fGetActualReport, LPSTR lpszBarcode3, LPSTR lpszBarcode4, funcAssignVariable fAssignVariable, CString *strScanSrc, ...);

extern "C" BOOL PASCAL EXPORT SetToDutParameter(LPSTR ActionName, LPSTR ParameterName,LPSTR ParameterValue);
extern "C" BOOL PASCAL EXPORT CleanToDutParameter(LPSTR ActionName);
extern "C" BOOL PASCAL EXPORT PreShootToDutParameter(LPSTR ActionName, LPSTR lpszRunInfo);

extern "C" BOOL PASCAL EXPORT RestartDutTest();
extern "C" BOOL PASCAL EXPORT WiFiInitialize(BOOL bRetry, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT LoadDutRequired(BOOL bRetry, LPSTR lpszRunInfo, PERRORREPORT pErrorReport);
extern "C" TM_RETURN PASCAL EXPORT WifiConnectTest(funcGetMessage cbListMessage, funcGetMessage cbXmlMessage, funcGetMessage cbSfcsSpecMessage, funcGetMessage cbSfcsResultMessage, ERRORREPORT &ErrorReport,int nRetryCnt,BOOL bRetry, LPSTR lpszDestIP, LPSTR lpszSrcIP);
extern "C" BOOL PASCAL EXPORT BackdoorSendData(LPSTR szCmd, LPSTR szData, DWORD &dwBytesRead, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT BackdoorResponse(LPSTR szCmd, LPSTR szData, DWORD dwBytesRead, LPSTR lpszRunInfo);

extern "C" BOOL PASCAL EXPORT GpibController(funcQueryGpib cbQueryGpib, funcReadGpib cbReadGpib, funcWriteGpib cbWriteGpib, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT GpibInitialize(funcGetGpibID cbGpibId, LPSTR lpszRunInfo);
extern "C" BOOL PASCAL EXPORT CHECK_BARCODE_INFORMATION(int numBarCode, LPSTR lpszBarcode, LPSTR lpszRunInfo);
#endif
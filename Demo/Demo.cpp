// Demo.cpp : 定義 DLL 的初始化常式。
//

#include "stdafx.h"
#include "Demo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果這個 DLL 是動態地對 MFC DLL 連結，
//		那麼從這個 DLL 匯出的任何會呼叫
//		MFC 內部的函式，都必須在函式一開頭加上 AFX_MANAGE_STATE
//		巨集。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此處為正常函式主體
//		}
//
//		這個巨集一定要出現在每一個
//		函式中，才能夠呼叫 MFC 的內部。這意味著
//		它必須是函式內的第一個陳述式
//		，甚至必須在任何物件變數宣告前面
//		，因為它們的建構函式可能會產生對 MFC
//		DLL 內部的呼叫。
//
//		請參閱 MFC 技術提示 33 和 58 中的
//		詳細資料。
//

// CDemoApp

BEGIN_MESSAGE_MAP(CDemoApp, CWinApp)
END_MESSAGE_MAP()

GLOBAL_INFO		g_GlobalInfo;
HWND			g_hParentWnd;
TEMPLATE_PARA   g_TemplatePara;
// CDemoApp 建構

CDemoApp::CDemoApp()
{
	// TODO: 在此加入建構程式碼，
	// 將所有重要的初始設定加入 InitInstance 中
}


// 僅有的一個 CDemoApp 物件

CDemoApp theApp;


// CDemoApp 初始設定

BOOL CDemoApp::InitInstance()
{
	CWinApp::InitInstance();
	TCHAR szMessage[DLL_INFO_SIZE];

	if(!PipeLineOpen(&g_GlobalInfo.PipeID,_T("cmd.exe"),szMessage)){
		::MessageBox(NULL, szMessage, _T("PipeLineOpen Failure"), MB_OK|MB_SETFOREGROUND);
		exit(0);
	} 

	return TRUE;
}
extern "C" BOOL PASCAL EXPORT LOAD_GUI_INFORMATION(LPSTR lpszOP, LPSTR lpszBarcode, LPSTR lpszPN, SFCS_MODE eSfcsMode, BOOL bStopOnFailed, HWND hParentWnd, LPSTR lpszBarcode2, funcGetActualReport fGetActualReport, LPSTR lpszBarcode3, LPSTR lpszBarcode4, funcAssignVariable fAssignVariable, CString *strScanSrc, ...)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    TCHAR szMessage[DLL_INFO_SIZE] = "TEST MESSAGE";

	g_hParentWnd = hParentWnd;
	g_GlobalInfo.fGetActualReport = fGetActualReport;
	g_GlobalInfo.fAssignVariable = fAssignVariable;
	g_GlobalInfo.fAssignVariable(TRUE,"[$TEST$]", "12345", 0);
    //::MessageBox(NULL, szMessage, _T("LOAD_GUI_INFORMATION"), MB_OK|MB_SETFOREGROUND);
	return TRUE;
}
extern "C" BOOL PASCAL EXPORT RestartDutTest()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TCHAR szMessage[DLL_INFO_SIZE],szLocalPath[MAX_PATH];
	TCHAR szFilePath[MAX_PATH];

	GetLocalDirectory(szLocalPath,szMessage);	
	sprintf_s(szFilePath,sizeof(szFilePath),"%s%s",szLocalPath,PIPE_COMMAND);
	if(CheckFileExist(szFilePath,szMessage))
		DeleteFile(szFilePath);
	sprintf_s(szFilePath,sizeof(szFilePath),"%s%s",szLocalPath,PIPE_MESSAGE);
	if(CheckFileExist(szFilePath,szMessage))
		DeleteFile(szFilePath);

	return TRUE;
}
extern "C" BOOL PASCAL EXPORT CleanToDutParameter(LPSTR ActionName)
{
    TCHAR szMessage[DLL_INFO_SIZE];
    PTCHAR actionInfo = ActionName;
    
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	if(strcmp(ActionName,_T("WIFI_LOAD_DUT_REQUIRED")) == NULL)
		 theApp.m_LoadDutRequired.Clean();
	else if(strcmp(ActionName,_T("WIFI_INITIALIZE_FTAPI")) == NULL)
		return theApp.m_WiFiInitialize.Clean();
	else if(strcmp(ActionName,_T("WIFI_CONNECT_TEST")) == NULL)
		return theApp.m_WifiConnectTest.Clean();
	else if(strcmp(ActionName,_T("BACKDOOR_TEST")) == NULL)
		return theApp.m_BackdoorTest.Clean();
    else if(strcmp(ActionName,_T("CUSTOMIZE_TEST")) == NULL)
        return theApp.m_CCUSTOMIZE_TEST.Clean();

   //::MessageBox(NULL, actionInfo, _T("CleanToDutParameter"), MB_OK|MB_SETFOREGROUND);
	
	return TRUE;
}
extern "C" BOOL PASCAL EXPORT SetToDutParameter(LPSTR ActionName, LPSTR ParameterName,LPSTR ParameterValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    PTCHAR actionInfo = ActionName;
    PTCHAR ParameterNameInfo = ParameterName;
    PTCHAR ParameterValueInfo = ParameterValue;

	if(strcmp(ActionName,_T("WIFI_LOAD_DUT_REQUIRED")) == NULL)
		return theApp.m_LoadDutRequired.SetParameter(ParameterName, ParameterValue);
	else if(strcmp(ActionName,_T("WIFI_INITIALIZE_FTAPI")) == NULL)
		return theApp.m_WiFiInitialize.SetParameter(ParameterName, ParameterValue);
	else if(strcmp(ActionName,_T("WIFI_CONNECT_TEST")) == NULL)
		return theApp.m_WifiConnectTest.SetParameter(ParameterName, ParameterValue);
	else if(strcmp(ActionName,_T("BACKDOOR_TEST")) == NULL)
		return theApp.m_BackdoorTest.SetParameter(ParameterName, ParameterValue);
    else if(strcmp(ActionName,_T("CUSTOMIZE_TEST")) == NULL)
        return theApp.m_CCUSTOMIZE_TEST.SetParameter(ParameterName, ParameterValue);

    ::MessageBox(NULL, actionInfo, _T("actionInfo"), MB_OK|MB_SETFOREGROUND);
    ::MessageBox(NULL, ParameterNameInfo, _T("ParameterNameInfo"), MB_OK|MB_SETFOREGROUND);
    ::MessageBox(NULL, ParameterValueInfo, _T("ParameterValueInfo"), MB_OK|MB_SETFOREGROUND);
	return TRUE;
}
extern "C" BOOL PASCAL EXPORT PreShootToDutParameter(LPSTR ActionName, LPSTR lpszRunInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(strcmp(ActionName,_T("WIFI_LOAD_DUT_REQUIRED")) == NULL)
		return theApp.m_LoadDutRequired.PreTest(lpszRunInfo);
	else if(strcmp(ActionName,_T("WIFI_INITIALIZE_FTAPI")) == NULL)
		return theApp.m_WiFiInitialize.PreTest(lpszRunInfo);
	else if(strcmp(ActionName,_T("WIFI_CONNECT_TEST")) == NULL)
		return theApp.m_WifiConnectTest.PreTest(lpszRunInfo);
	else if(strcmp(ActionName,_T("BACKDOOR_TEST")) == NULL)
		return theApp.m_BackdoorTest.PreTest(lpszRunInfo);
    else if(strcmp(ActionName,_T("CUSTOMIZE_TEST")) == NULL)
        return theApp.m_CCUSTOMIZE_TEST.PreTest(lpszRunInfo);
	return TRUE;
}
extern "C" BOOL PASCAL EXPORT LoadDutRequired(BOOL bRetry, LPSTR lpszRunInfo, PERRORREPORT pErrorReport)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.m_LoadDutRequired.Test(bRetry, lpszRunInfo, pErrorReport);
}
extern "C" BOOL PASCAL EXPORT WiFiInitialize(BOOL bRetry, LPSTR lpszRunInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.m_WiFiInitialize.Test(bRetry, lpszRunInfo);
}
extern "C" TM_RETURN PASCAL EXPORT WifiConnectTest(funcGetMessage cbListMessage, funcGetMessage cbXmlMessage, funcGetMessage cbSfcsSpecMessage, funcGetMessage cbSfcsResultMessage, ERRORREPORT &ErrorReport,int nRetryCnt,BOOL bRetry, LPSTR lpszDestIP, LPSTR lpszSrcIP)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.m_WifiConnectTest.Test(cbListMessage, cbXmlMessage, cbSfcsSpecMessage, cbSfcsResultMessage, ErrorReport, nRetryCnt, bRetry, lpszDestIP, lpszSrcIP);
}
extern "C" BOOL PASCAL EXPORT BackdoorSendData(LPSTR szCmd, LPSTR szData, DWORD &dwBytesRead, LPSTR lpszRunInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.m_BackdoorTest.SendData(szCmd, szData, dwBytesRead, lpszRunInfo);
}
extern "C" BOOL PASCAL EXPORT BackdoorResponse(LPSTR szCmd, LPSTR szData, DWORD dwBytesRead, LPSTR lpszRunInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.m_BackdoorTest.Response(szCmd, szData, dwBytesRead, lpszRunInfo);
}

extern "C" BOOL PASCAL EXPORT GpibController(funcQueryGpib cbQueryGpib, funcReadGpib cbReadGpib, funcWriteGpib cbWriteGpib, LPSTR lpszRunInfo)
{
	g_GlobalInfo.fQueryGpib = cbQueryGpib;
	g_GlobalInfo.fReadGpib = cbReadGpib;
	g_GlobalInfo.fWriteGpib = cbWriteGpib;
	return theApp.m_GpibController.Test();
}

extern "C" BOOL PASCAL EXPORT GpibInitialize(funcGetGpibID cbGpibId, LPSTR lpszRunInfo)
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	strCopy(lpszRunInfo, szMessage);
	return FALSE;
	return cbGpibId(g_GlobalInfo.GpibID, 0, 18, 0, szMessage);	
}

extern "C" BOOL PASCAL EXPORT CHECK_BARCODE_INFORMATION(int numBarCode, LPSTR lpszBarcode, LPSTR lpszRunInfo)
{
	TCHAR szMessage[DLL_INFO_SIZE]={0};
	sprintf_s(szMessage, "Confirm barcode %d success.", numBarCode);
	return TRUE;
}

extern "C" TM_RETURN PASCAL EXPORT CustomizeTest(funcGetMessage cbListMessage, funcGetMessage cbXmlMessage, funcGetMessage cbSfcsSpecMessage, funcGetMessage cbSfcsResultMessage, ERRORREPORT &ErrorReport, ...)
{
    char szMessage[DLL_INFO_SIZE];

    g_TemplatePara.cbListMessage       = cbListMessage;
    g_TemplatePara.cbXmlMessage        = cbXmlMessage;
    g_TemplatePara.cbSfcsSpecMessage   = cbSfcsSpecMessage;
    g_TemplatePara.cbSfcsResultMessage = cbSfcsResultMessage;
    g_TemplatePara.ErrorReport         = &ErrorReport;

   /* strcpy(szMessage, "CustomizeTest_TESTCustomizeTest_TESTCustomizeTest_TESTCustomizeTest_TEST");
    strcpy(errorCode, "T12345678");
    g_TemplatePara.ErrorReport->DisableSfcsLink = FALSE;
    g_TemplatePara.ErrorReport->TestResult=FALSE;
    strCopy(g_TemplatePara.ErrorReport->ErrorCode, errorCode);
    strCopy(g_TemplatePara.ErrorReport->ErrorDetail,_T("CustomizeTest Test Struct define"));	
    strCopy(g_TemplatePara.ErrorReport->FailedTest,_T("CustomizeTest"));
    strCopy(g_TemplatePara.ErrorReport->ErrorMsg,szMessage);
    return TM_RETURN_ERROR; */
    
    if(theApp.m_CCUSTOMIZE_TEST.Test(0, szMessage, &g_TemplatePara))
        return TM_RETURN_PASS;
    else
        return TM_RETURN_ERROR; 
}

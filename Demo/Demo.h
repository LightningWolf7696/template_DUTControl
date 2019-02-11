// Demo.h : Demo DLL 的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 包含此檔案前先包含 'stdafx.h'"
#endif

#include "resource.h"		// 主要符號
#include "CommFunc.h"

// CDemoApp
// 這個類別的實作請參閱 Demo.cpp
//
#include ".\MyClass\WifiConnectTest.h"
#include ".\MyClass\LoadDutRequired.h"
#include ".\MyClass\WiFiInitialize.h"
#include ".\MyClass\BackdoorTest.h"
#include ".\MyClass\GpibController.h"
#include ".\MyClass\CUSTOMIZE_TEST.h"



class CDemoApp : public CWinApp
{

public:
	CDemoApp();

// 覆寫
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()

public:
	CWifiConnectTest	m_WifiConnectTest;
	CLoadDutRequired	m_LoadDutRequired;
	CWiFiInitialize		m_WiFiInitialize;
	CBackdoorTest		m_BackdoorTest;
	CGpibController		m_GpibController;
    CCUSTOMIZE_TEST     m_CCUSTOMIZE_TEST;
};

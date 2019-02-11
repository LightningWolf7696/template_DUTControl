// Demo.h : Demo DLL ���D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�
#include "CommFunc.h"

// CDemoApp
// �o�����O����@�аѾ\ Demo.cpp
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

// �мg
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

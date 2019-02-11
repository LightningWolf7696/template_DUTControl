// DlgShowMsg.cpp : 實作檔
//

#include "stdafx.h"
#include "Resource.h"
#include "DlgShowMsg.h"
#include "afxdialogex.h"


// CDlgShowMsg 對話方塊

IMPLEMENT_DYNAMIC(CDlgShowMsg, CDialogEx)

CDlgShowMsg::CDlgShowMsg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgShowMsg::IDD, pParent)
{
	m_skipPass = FALSE;
	m_skipFail = FALSE;
}
CDlgShowMsg::~CDlgShowMsg()
{
}

void CDlgShowMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_Pass);
	DDX_Control(pDX, IDCANCEL, m_Fail);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_Text);
}


BEGIN_MESSAGE_MAP(CDlgShowMsg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CDlgShowMsg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgShowMsg::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgShowMsg 訊息處理常式


BOOL CDlgShowMsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此加入額外的初始化
	m_Result=TRUE;
	pWnd=GetDlgItem(IDC_PICTURE);
	pen.CreatePen(PS_SOLID,6,RGB(255,0,0));
	pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//透明化
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	pWnd->MoveWindow(rect.left, rect.top,image.GetWidth(),image.GetHeight(),TRUE);
	pWnd->GetClientRect(&rect);
	pDC=pWnd->GetDC();
	pDC->SelectObject(&pen);//紅色框
	pDC->SelectObject(pBrush);
	m_Text.SetWindowText(strText);
	this->SetWindowText(strCaption);
	if(!m_skipPass && m_skipFail)
		m_Pass.SetWindowText("OK");

	if(m_skipPass)
		m_Pass.ShowWindow(FALSE);
	else
		m_Pass.ShowWindow(TRUE);

	if(m_skipFail)
		m_Fail.ShowWindow(FALSE);
	else
		m_Fail.ShowWindow(TRUE);

	m_Pass.SetWindowPos(NULL,10,rect.Height()+80,100,100,SWP_NOZORDER | SWP_NOSIZE);
	m_Fail.SetWindowPos(NULL,rect.Width()-65,rect.Height()+80,100,100,SWP_NOZORDER | SWP_NOSIZE);

	this->SetForegroundWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}


void CDlgShowMsg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此加入您的訊息處理常式程式碼
	// 不要呼叫圖片訊息的 CDialogEx::OnPaint()
	//HWND hSubWnd;

	/*HWND mainHwnd = ::GetForegroundWindow();
	if (mainHwnd!=this->m_hWnd){
		::SendMessage(this->m_hWnd, WM_SYSCOMMAND, SC_PREVWINDOW, NULL);
	}*/
	if(image!=NULL){
		image.Draw(pDC->m_hDC,rect);
		pDC->Rectangle(m_red);
		
	}
}


void CDlgShowMsg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	::MoveWindow(this->m_hWnd, cx, cy,image.GetWidth()+30,image.GetHeight()+150,TRUE);
	
	// TODO: 在此加入您的訊息處理常式程式碼
}
void CDlgShowMsg::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	m_Result=TRUE;
	CDialogEx::OnOK();
}


void CDlgShowMsg::OnBnClickedCancel()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	m_Result = (!m_skipPass && m_skipFail)? TRUE:FALSE;
	CDialogEx::OnCancel();
}


HBRUSH CDlgShowMsg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此變更 DC 的任何屬性
	if (nCtlColor == CTLCOLOR_STATIC){
		pDC->SetTextColor(RGB(255, 0, 0));//red
		/*CFont font;
		font.CreatePointFont(30, "宋体");
		pDC->SelectObject(&font);*/
	}
	// TODO:  如果預設值並非想要的，則傳回不同的筆刷
	return hbr;
}
BOOL CDlgShowMsg::InitFormat(CString Picture, CString Caption, CString Text, CRect Red_Rect, UINT uType){
	strText = Text;
	strCaption = Caption;
	m_red = Red_Rect;
	image.Load(Picture);
	if(uType == MB_RETRYCANCEL)
		m_skipPass = TRUE;
	else if(uType == MB_OK)
		m_skipFail = TRUE;	
	
	
	if (image==NULL) return FALSE;
	return TRUE;
}

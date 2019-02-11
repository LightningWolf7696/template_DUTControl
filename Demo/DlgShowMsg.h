#pragma once
#include "afxwin.h"


// CDlgShowMsg ��ܤ��

class CDlgShowMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShowMsg)

public:
	CDlgShowMsg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CDlgShowMsg();
// ��ܤ�����
	enum { IDD = IDD_SHOW_MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Pic;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	CImage  image;
	CRect	rect;
	CPen	pen;
	CWnd	*pWnd;
	CDC		*pDC;
	CBrush	*pBrush;
	CStatic m_Text;
	CButton m_Pass;
	CButton m_Fail;
public:
	BOOL	m_Result;
	BOOL	m_skipPass;
	BOOL	m_skipFail;
	CRect	m_red;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL InitFormat(CString Picture, CString Caption, CString Text, CRect Red_Rect, UINT uType);

private:
	CString		strText;
	CString		strCaption;
};

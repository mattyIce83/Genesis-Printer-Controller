#pragma once
#include "afxwin.h"


// CConfirmPrint dialog

class CConfirmPrint : public CDialogEx
{
	DECLARE_DYNAMIC(CConfirmPrint)

public:
	CConfirmPrint(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfirmPrint();

	void InitialSetup();
	char m_szJobFile[128];
	BITMAPHANDLE Bitmap;
	void PreparePage();
	void DisplayPage();
	RECT crect;
	RECT inRect;
	RECT clrect;
	CDC *dcScreen;
	BOOL bHasImage;
	QUEUE_ITEM *qi;

	void CConfirmPrint::DisplayTempWarning(CPaintDC *);


// Dialog Data
	enum { IDD = IDD_CONFIRMPRINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_No;
	CButton m_Yes;
	virtual BOOL OnInitDialog();
	CStatic m_Title;
	CStatic m_Preview;
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	CEdit m_DispRes;
	CEdit m_PrnParams;
	CEdit m_JobName;
};

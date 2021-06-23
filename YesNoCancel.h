#pragma once
#include "afxwin.h"


// CYesNoCancel dialog

class CYesNoCancel : public CDialogEx
{
	DECLARE_DYNAMIC(CYesNoCancel)

public:
	CYesNoCancel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CYesNoCancel();

	char szMessage[1024];



// Dialog Data
	enum { IDD = IDD_YESNOCANCEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Message;
	CButton m_OKButton;
	CButton m_CancelButton;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

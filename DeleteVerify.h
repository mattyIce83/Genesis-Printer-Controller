#pragma once
#include "afxwin.h"


// CDeleteVerify dialog

class CDeleteVerify : public CDialogEx
{
	DECLARE_DYNAMIC(CDeleteVerify)

public:
	CDeleteVerify(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeleteVerify();

// Dialog Data
	enum { IDD = IDD_DELETEVERIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Message;
	char m_szMessage[1024];
	virtual BOOL OnInitDialog();
	CButton m_OK;
	CButton m_Cancel;
};

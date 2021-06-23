#pragma once
#include "afxwin.h"


// CUserPrompt1 dialog

class CUserPrompt1 : public CDialogEx
{
	DECLARE_DYNAMIC(CUserPrompt1)

public:
	CUserPrompt1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserPrompt1();


	char szMessage[256];


// Dialog Data
	enum { IDD = IDD_USERPROMPT1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Message;
	virtual BOOL OnInitDialog();
};

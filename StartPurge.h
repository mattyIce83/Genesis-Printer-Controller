#pragma once
#include "afxwin.h"


// CStartPurge dialog

class CStartPurge : public CDialogEx
{
	DECLARE_DYNAMIC(CStartPurge)

public:
	CStartPurge(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStartPurge();

	int Mode;

// Dialog Data
	enum { IDD = IDD_START_PURGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CButton m_StartPurge;
	CButton m_EndPurge;
	CStatic m_Message;
};

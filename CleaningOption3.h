#pragma once
#include "afxwin.h"


// CCleaningOption3 dialog

class CCleaningOption3 : public CDialogEx
{
	DECLARE_DYNAMIC(CCleaningOption3)

public:
	CCleaningOption3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCleaningOption3();

	BOOL bDualCMYK;

	int Function;


// Dialog Data
	enum { IDD = IDD_CONTROLLER4_DIALOG5B };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_HeadA;
	CButton m_HeadB;
	CButton m_BothHeads;
	afx_msg void OnBnClickedHeadA();
	afx_msg void OnBnClickedHeadB();
	afx_msg void OnBnClickedBothHeads();
	afx_msg void OnBnClickedOk();
	CStatic m_Title;
};

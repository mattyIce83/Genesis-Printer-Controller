#pragma once
#include "afxwin.h"


// CNumericEntry dialog

class CNumericEntry : public CDialogEx
{
	DECLARE_DYNAMIC(CNumericEntry)

public:
	CNumericEntry(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNumericEntry();

	void HandleKey(char);

	BOOL bIsPassword;

	int iFieldCharIndex;

	int iNumberOfCharsExpected;

	char szCode[128];
	char szDisplay[128];

	char szTitle[255];


// Dialog Data
	enum { IDD = IDD_NUMERICENTRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Output;
	CEdit m_Title;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedKb1();
	afx_msg void OnBnClickedKb2();
	afx_msg void OnBnClickedKb3();
	afx_msg void OnBnClickedKb4();
	afx_msg void OnBnClickedKb5();
	afx_msg void OnBnClickedKb6();
	afx_msg void OnBnClickedKb7();
	afx_msg void OnBnClickedKb8();
	afx_msg void OnBnClickedKb9();
	afx_msg void OnBnClickedKb0();
	virtual BOOL OnInitDialog();
};

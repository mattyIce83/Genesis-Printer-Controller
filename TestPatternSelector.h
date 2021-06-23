#pragma once


// CTestPatternSelector dialog

class CTestPatternSelector : public CDialogEx
{
	DECLARE_DYNAMIC(CTestPatternSelector)

public:
	CTestPatternSelector(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestPatternSelector();

	int Result;

// Dialog Data
	enum { IDD = IDD_TESTBAND_SELECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCustYDlg();
	afx_msg void OnBnClickedYCorDlg();
};

#pragma once


// CWhiteCleaning dialog

class CWhiteCleaning : public CDialogEx
{
	DECLARE_DYNAMIC(CWhiteCleaning)

public:
	CWhiteCleaning(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWhiteCleaning();

// Dialog Data
	enum { IDD = IDD_WHITE_CLEANING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedWhiteDrip();
	afx_msg void OnBnClickedWhiteBagMix();
	afx_msg void OnBnClickedFlush();
	afx_msg void OnBnClickedFlushClean();
	afx_msg void OnBnClickedRecirculate();
	afx_msg void OnBnClickedAirPurge();
	afx_msg void OnBnClickedAirPurge2();
};

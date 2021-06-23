#pragma once
#include "afxwin.h"


// Controller4Dlg5A dialog

class Controller4Dlg5A : public CDialogEx
{
	DECLARE_DYNAMIC(Controller4Dlg5A)

public:
	Controller4Dlg5A(CWnd* pParent = NULL);   // standard constructor
	virtual ~Controller4Dlg5A();

// Dialog Data
	enum { IDD = IDD_CONTROLLER4_DIALOG5A };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedWipeHead();
	afx_msg void OnBnClickedHeadBath();
	afx_msg void OnBnClickedCapFlush();
	afx_msg void OnBnClickedQuickClean();
	afx_msg void OnBnClickedFillA();
	afx_msg void OnBnClickedFillB();
	afx_msg void OnBnClickedFillAll();
	afx_msg void OnBnClickedDrainA();
	afx_msg void OnBnClickedDrainB();
	afx_msg void OnBnClickedDrainAll();
	CButton m_FillDrain;
	CButton m_Prime;
	afx_msg void OnBnClickedDrain();
	afx_msg void OnBnClickedAirPurge();
};

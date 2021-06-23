#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPlatenControl dialog

class CPlatenControl : public CDialogEx
{
	DECLARE_DYNAMIC(CPlatenControl)

public:
	CPlatenControl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlatenControl();

	int MoveVal;

	void DisplaySteps();


// Dialog Data
	enum { IDD = IDD_PLATENCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_RearL;
	CButton m_RearR;
	CButton m_FrontL;
	CButton m_FrontR;
	CEdit m_Steps;
	CSpinButtonCtrl m_Spin1000;
	CSpinButtonCtrl m_Spin100;
	CSpinButtonCtrl m_Spin10;
	CSpinButtonCtrl m_Spin1;
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposUd1000(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposUd100(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposUd10(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposUd1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMove();
	CButton m_Up;
	CButton m_Down;
	afx_msg void OnBnClickedDirUp();
	afx_msg void OnBnClickedDirDown();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedTotop();
	afx_msg void OnBnClickedTobottom();
	afx_msg void OnBnClickedSetasdefault();
};

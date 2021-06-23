#pragma once
#include "afxwin.h"


// CPlatenLiftCtrl dialog

class CPlatenLiftCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CPlatenLiftCtrl)

public:
	CPlatenLiftCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlatenLiftCtrl();


	void GetLastSettings();
	void SaveLastSettings();


// Dialog Data
	enum { IDD = IDD_PLATEN_LIFT_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Stepper1;
	CButton m_Stepper2;
	CButton m_Stepper3;
	CButton m_Stepper4;
	CEdit m_Ticks;
	CComboBox m_Direction;
	CComboBox m_Stepping;
	CEdit m_Current;
	afx_msg void OnBnClickedInitsteppers();
	afx_msg void OnBnClickedEnableall();
	afx_msg void OnBnClickedDisableAll();
	afx_msg void OnBnClickedEnableSel();
	afx_msg void OnBnClickedMoveticks();
	afx_msg void OnBnClickedTobottom();
	afx_msg void OnBnClickedTotop();
	afx_msg void OnBnClickedSendsettings();
	virtual BOOL OnInitDialog();
	CComboBox m_MoveMode;
	afx_msg void OnBnClickedButton1();
};

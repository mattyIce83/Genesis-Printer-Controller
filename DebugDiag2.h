#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDebugDiag2 dialog

class CDebugDiag2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDebugDiag2)

public:
	CDebugDiag2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDebugDiag2();

	void ShowCorrectionValues();
	
	void SizeDialog();

	BOOL bAdjustingLasers;

	BOOL bRunPressureUpdater;

	void ShowADCVals();

// Dialog Data
	enum { IDD = IDD_DEBUG_DIAG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedWhiteValve1Open();
	afx_msg void OnBnClickedWhiteValve1Close();
	afx_msg void OnBnClickedWhiteValve2Open();
	afx_msg void OnBnClickedWhiteValve2Close();
	afx_msg void OnBnClickedWhiteValve3Open();
	afx_msg void OnBnClickedWhiteValve3Close();
	afx_msg void OnBnClickedWhiteValve4Open();
	afx_msg void OnBnClickedWhiteValve4Close();
	afx_msg void OnBnClickedVacpumpon();
	afx_msg void OnBnClickedVacpumpoff();
	afx_msg void OnBnClickedOpencf();
	afx_msg void OnBnClickedClosecf();
	afx_msg void OnBnClickedHeaterOn();
	afx_msg void OnBnClickedHeaterOff();
	afx_msg void OnBnClickedTofrontmeasure();
	afx_msg void OnBnClickedTorearmeasure();
	afx_msg void OnBnClickedRollplus1k();
	afx_msg void OnBnClickedRollminus1k();
	afx_msg void OnBnClickedRollplus1h();
	afx_msg void OnBnClickedRollminus1h();
	afx_msg void OnBnClickedPitchplus1k();
	afx_msg void OnBnClickedPitchminus1k();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedPitchminus1h();
	CEdit m_RollValue;
	CEdit m_PitchValue;
	virtual BOOL OnInitDialog();
	CButton m_SupC;
	CButton m_SupM;
	CButton m_SupY;
	CButton m_SupK;
	afx_msg void OnBnClickedSupC();
	afx_msg void OnBnClickedSupM();
	afx_msg void OnBnClickedSupY2();
	afx_msg void OnBnClickedSupK2();
	afx_msg void OnBnClickedCldlgon();
	afx_msg void OnBnClickedWtdlg();
	afx_msg void OnBnClickedPortal();
	afx_msg void OnBnClickedNewplaten();
	afx_msg void OnBnClickedAlignFrontLow();
	afx_msg void OnBnClickedAlignFrontMid();
	afx_msg void OnBnClickedAlignFrontHigh();
	afx_msg void OnBnClickedAlignRearLow();
	afx_msg void OnBnClickedAlignRearMid();
	afx_msg void OnBnClickedAlignRearHigh();
	afx_msg void OnBnClickedAlignOff();
	afx_msg void OnDestroy();
	CButton m_AlignFrontLow;
	CButton m_AlignFrontMiddle;
	CButton m_AlignFrontHigh;
	CButton m_AlignRearLow;
	CButton m_AlignRearMiddle;
	CButton m_AlignRearHigh;
	CButton m_AlignOff;
	CEdit m_InkADC_1;
	CEdit m_InkADC_2;
	CEdit m_InkADC_3;
	CEdit m_InkADC_4;
	CEdit m_InkADC_5;
	CEdit m_InkADC_6;
	afx_msg void OnBnClickedRefreshadc();
	afx_msg void OnBnClickedTestInkMarkers();
	CEdit m_CapADC;
	CProgressCtrl m_CapPress;
	afx_msg void OnBnClickedRefreshCont();
	CButton m_IgnoreInkOut;
	afx_msg void OnBnClickedIgnoreinkout();
};

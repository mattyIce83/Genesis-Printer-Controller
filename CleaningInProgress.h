#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CleaningInProgress dialog

class CleaningInProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CleaningInProgress)

public:
	CleaningInProgress(CWnd* pParent = NULL);   // standard constructor
	virtual ~CleaningInProgress();

	CBitmap Background1;
	CBitmap Background2;
	CBitmap Background3;
	CBitmap Background4;
	CBitmap Background5;
	CBitmap Background6;

	int m_nTimer;

	int cappress;
	int maxcappress;

	BOOL bUsingAVI;

	BOOL bEnableCancelButton;

	CBitmap *backgrounds[10];

	LOGPEN logPen;
	CPen	hCPen;

	int currentbk;

	void DrawAnimSection();

	void SizeDialog();

	RECT AnimSection;

	void Exit();

// Dialog Data
	enum { IDD = IDD_CLEANING_IN_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void PreInitDialog();
public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	CAnimateCtrl m_AVIPlayer;
	CProgressCtrl m_CapPress;
	CStatic m_CapVacLab;
	CStatic m_CapVacMaxLab;
	CButton m_CancelButton;
	afx_msg void OnBnClickedCancelClean();
};

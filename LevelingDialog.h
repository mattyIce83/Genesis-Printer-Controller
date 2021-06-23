#pragma once
#include "afxcmn.h"


// CLevelingDialog dialog

class CLevelingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLevelingDialog)

public:
	CLevelingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLevelingDialog();

	CBitmap Background1;
	CBitmap Background2;
	CBitmap Background3;
	CBitmap Background4;
	CBitmap Background5;
	CBitmap Background6;
	CBitmap Background7;

	int m_nTimer;

	BOOL bUsingAVI;

	CBitmap *backgrounds[12];

	LOGPEN logPen;
	CPen	hCPen;

	int currentbk;

	void DrawAnimSection();

	void SizeDialog();

	RECT AnimSection;

	void Exit();



// Dialog Data
	enum { IDD = IDD_LEVELING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void PreInitDialog();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CAnimateCtrl m_AVIPlayer;
};

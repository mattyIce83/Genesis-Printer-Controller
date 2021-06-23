#pragma once
#include "afxwin.h"

#define PRINT_DIALOG_TIMER_EVENT	6969


// CPrinting dialog

class CPrinting : public CDialogEx
{
	DECLARE_DYNAMIC(CPrinting)

public:
	CPrinting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrinting();

	void SizeDialog();

	char m_szJobName[128];
	char m_szJumber[128];
	char m_szPrintMode[128];
	char m_szSize[128];
	char m_szJobFile[128];

	RECT crect;
	RECT clrect;
	RECT inRect;

	BOOL bHasInfo;
	BOOL bInfoAvailable;
	BOOL bSizeAvailable;
	BOOL Running;
	BOOL bEStopShown;
	int	bEStopDisplayTimer;

	float sizeX;
	float sizeY;

	void DisplayPage();

	void PreparePage();

	void InitialSetup();

	void UpdatePreviewImage(float, void *);

	void PublicContinueprint();
	void PublicClickedEstop();


	int m_nTimer;

	int printed_rasters;
	
	int unprinted_rasters;

	QUEUE_ITEM *qi;

	BITMAPHANDLE Bitmap;
	BITMAPHANDLE BitmapLight;

	CDC *dcScreen;

// Dialog Data
	enum { IDD = IDD_PRINTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CBitmapButton m_Home;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedHome();
	CMyEdit m_JobInfo;
	CBitmapButton m_EStop;
	CBitmapButton m_ContinuePrint;
	CBitmapButton m_CleanPrint;
	CBitmapButton m_CancelPrint;
	CMyEdit m_JobNumber;
	CMyEdit m_PrintMode;
	CMyEdit m_Size;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_Preview;
	afx_msg void OnBnClickedEstop();
	afx_msg void OnBnClickedCancelprint();
	afx_msg void OnBnClickedContinueprint();
	afx_msg void OnBnClickedCleanprint();
	CMyEdit m_Percent;
	CStatic m_WarningBox;
	CStatic m_Cancelling;
};

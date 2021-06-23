#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CManualPlatenHeights dialog

class CManualPlatenHeights : public CDialogEx
{
	DECLARE_DYNAMIC(CManualPlatenHeights)

public:
	CManualPlatenHeights(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManualPlatenHeights();


	void DisplayPrintHeight();
	void DisplayFeedHeight();
	void DisplayOffset();

	int oldPlatenFeedHeight;

	int oldPlatenPrintHeight;



// Dialog Data
	enum { IDD = IDD_MANUAL_PLATEN_HEIGHTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSpinButtonCtrl m_PLHeight100UD;
	CSpinButtonCtrl m_PLHeight100UD_2;
	CSpinButtonCtrl m_PLHeight10UD;
	CSpinButtonCtrl m_PLHeight10UD_2;
	virtual BOOL OnInitDialog();
	CEdit m_PrintHeight;
	CEdit m_FeedHeight;
	afx_msg void OnDeltaposFl10ud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposFl1ud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposFl10ud2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposFl1ud2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckprintheight();
	afx_msg void OnBnClickedCheckfeedheight();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedHomeheight();
	afx_msg void OnBnClickedCheckparkoffset();
	CSpinButtonCtrl m_OFHeight100UD;
	CSpinButtonCtrl m_OFHeight10UD;
	afx_msg void OnDeltaposOf10ud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposOf1ud(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_ParkOffset;
};

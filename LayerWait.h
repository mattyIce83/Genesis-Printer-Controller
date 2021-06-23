#pragma once
#include "afxwin.h"


// CLayerWait dialog

class CLayerWait : public CDialogEx
{

	DECLARE_DYNAMIC(CLayerWait)

public:

	CLayerWait(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLayerWait();

	void ShowCount(CPaintDC *);

	int m_nTimer;
	int countdown;
	RECT counter_rect;
	BOOL bWaitAborted;
	BOOL bManualMode;
	
// Dialog Data
	enum { IDD = IDD_CANCEL_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCancellayerwait();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedWaitlonger();
	afx_msg void OnBnClickedPrintnow();
	CButton m_WaitLonger;
	CStatic m_Label;
};


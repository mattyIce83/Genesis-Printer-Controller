#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CServoOffsets dialog

class CServoOffsets : public CDialogEx
{
	DECLARE_DYNAMIC(CServoOffsets)

public:
	CServoOffsets(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServoOffsets();

	CEdit *m_ValWindow[4];

	void DisplayOffset(int);

	BOOL bMoved;

// Dialog Data
	enum { IDD = IDD_SERVO_OFFSETS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_FLVal;
	CEdit m_FRVal;
	CEdit m_RLVal;
	CEdit m_RRVal;
	CSpinButtonCtrl m_FL10UD;
	CSpinButtonCtrl m_FL1UD;
	CSpinButtonCtrl m_FR10UD;
	CSpinButtonCtrl m_FR1UD;
	CSpinButtonCtrl m_RL10UD;
	CSpinButtonCtrl m_RL1UD;
	CSpinButtonCtrl m_RR10UD;
	CSpinButtonCtrl m_RR1UD;
	afx_msg void OnDeltaposFl10ud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposFl1ud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposFr10ud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposFr1ud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRl10ud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRl1ud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRr10ud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRr1ud(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTest();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedPlatenToMeasurefront();
	afx_msg void OnBnClickedPlatenToMeasurefront2();
	afx_msg void OnBnClickedHomez();
	afx_msg void OnBnClickedReleaseY();
	CButton m_HomeOnExit;
	afx_msg void OnBnClickedOk();
};

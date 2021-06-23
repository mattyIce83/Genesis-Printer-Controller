#pragma once
#include "afxwin.h"


// CAirPurge dialog

class CAirPurge : public CDialogEx
{
	DECLARE_DYNAMIC(CAirPurge)

public:
	CAirPurge(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAirPurge();

// Dialog Data
	enum { IDD = IDD_MANUAL_WHITE_PURGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_StartAirPurge;
	CButton m_CancelAirPurge;
	afx_msg void OnBnClickedStart();
};

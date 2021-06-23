#pragma once


// CPleaseWaitInit dialog

class CPleaseWaitInit : public CDialogEx
{
	DECLARE_DYNAMIC(CPleaseWaitInit)

public:
	CPleaseWaitInit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPleaseWaitInit();

	int tick;
	int m_nTimer;


// Dialog Data
	enum { IDD = IDD_PLEASE_WAIT_INIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

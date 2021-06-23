#pragma once


// CPleaseWait dialog

class CPleaseWait : public CDialogEx
{
	DECLARE_DYNAMIC(CPleaseWait)

public:
	CPleaseWait(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPleaseWait();

	int m_nTimer;

// Dialog Data
	enum { IDD = IDD_PLEASEWAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#pragma once


// CAutoSpitting dialog

class CAutoSpitting : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoSpitting)

public:
	CAutoSpitting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAutoSpitting();

// Dialog Data
	enum { IDD = IDD_AUTOSPITTING };
	
	int m_nTimer;

	void Quit();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

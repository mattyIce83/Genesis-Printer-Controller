#pragma once


// CDemoMode dialog

class CDemoMode : public CDialogEx
{
	DECLARE_DYNAMIC(CDemoMode)

public:
	CDemoMode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDemoMode();

	CBitmap Background;

	int m_nTimer;

// Dialog Data
	enum { IDD = IDD_DEMOMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

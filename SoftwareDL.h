#pragma once
#include "afxwin.h"


// CSoftwareDL dialog

class CMyEdit2: public CEdit {
	COLORREF m_cr;
	CBrush   m_br;
	DECLARE_MESSAGE_MAP()
	HBRUSH CtlColor ( CDC* pDC, UINT nCtlColor )
	{
		pDC->SetBkColor(m_cr);
		pDC->SetTextColor(RGB(255,255,255));
		return m_br;
	}
public:
	void SetBkColor(COLORREF cr)
	{
		m_cr = cr;
		m_br.CreateSolidBrush(cr);
		Invalidate();
	}
};


class CSoftwareDL : public CDialogEx
{
	DECLARE_DYNAMIC(CSoftwareDL)

public:
	CSoftwareDL(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSoftwareDL();


	int m_CharsInWindow;
	void StatusLine(char *,...);
	CWinThread *DLLoaderThread;
	BOOL bTimeToExit;
	int m_nTimer;



// Dialog Data
	enum { IDD = IDD_SOFTWARE_DL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//CEdit m_Status1;
	CMyEdit2 m_Status1;
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_OKSWDL;
};

#pragma once
#include "afxwin.h"

#include "PictureEx.h"

// CWaitDialog dialog

class CWaitDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CWaitDialog)

public:
	CWaitDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWaitDialog();


	int m_nTimer;

	char szMessage[3][128];

	CDC *hdc;

	RECT message_rect[3];


// Dialog Data
	enum { IDD = IDD_WAITDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//CStatic m_Animation;
	CPictureEx m_Animation;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};

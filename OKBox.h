#pragma once
#include "afxwin.h"


// COKBox dialog

class COKBox : public CDialogEx
{
	DECLARE_DYNAMIC(COKBox)

public:
	COKBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~COKBox();

// Dialog Data
	enum { IDD = IDD_OKBOX };


	RECT rc;
	char *Line_1;
	char *Line_2;
	char *Line_3;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	CBitmapButton m_OK;
};

#pragma once
#include "afxwin.h"


#define UPDLDG_MODE_NORMAL		0
#define UPDLDG_MODE_TBD1		1
#define UPDLDG_MODE_TBD2		2
#define UPDLDG_MODE_TBD3		3


// CUserPrompt2 dialog

class CUserPrompt2 : public CDialogEx
{
	DECLARE_DYNAMIC(CUserPrompt2)

public:

	CUserPrompt2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserPrompt2();

	char szButtonText[4][96];
	char szIllustrationFile[4][96];
	int iDefaultButton;
	int Mode;
	char szDialogTitle[128];
	int nRetCode;

	int bitmaps;

	RECT inRect;

	RECT crect;

	int iCurrentIllustration;

	BITMAPHANDLE Bitmap[4];

	CDC *dcScreen;

// Dialog Data
	enum { IDD = IDD_USER_PROMPT2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void CleanUpAndExit(int);


	DECLARE_MESSAGE_MAP()
public:
	CButton m_Button_1;
	CButton m_Button_2;
	CButton m_Button_3;
	CButton m_Button_4;
	CStatic m_Illustration;
	CButton m_Next;
	CButton m_Prev;
	virtual BOOL OnInitDialog();
	CStatic m_DialogTitle;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedPrevIllustration();
	afx_msg void OnBnClickedNextIllustration();
};

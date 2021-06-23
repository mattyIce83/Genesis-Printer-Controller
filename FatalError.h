#pragma once


// CFatalError dialog

class CFatalError : public CDialogEx
{
	DECLARE_DYNAMIC(CFatalError)

public:
	CFatalError(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFatalError();

	CBitmap Background1;

	void SizeDialog();

	
	// Dialog Data
	enum { IDD = IDD_FATAL_ERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void PreInitDialog();
	afx_msg void OnPaint();
};

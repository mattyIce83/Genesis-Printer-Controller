#pragma once
#include "afxwin.h"


// CDripWhiteTime dialog

class CDripWhiteTime : public CDialogEx
{
	DECLARE_DYNAMIC(CDripWhiteTime)

public:
	CDripWhiteTime(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDripWhiteTime();

	int Seconds;

	char szMessage[80];

	char szButtonText[64];


	void DisplayTime();
	
	// Dialog Data
	enum { IDD = IDD_WHITEDRIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Time;
	afx_msg void OnBnClickedSecondsPlus();
	afx_msg void OnBnClickedSecondsminus();
	virtual BOOL OnInitDialog();
	CButton m_GoButton;
	CStatic m_Message;
	afx_msg void OnBnClickedOk();
};

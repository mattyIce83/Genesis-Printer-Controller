#pragma once


// CDevelopmentTeam dialog

class CDevelopmentTeam : public CDialogEx
{
	DECLARE_DYNAMIC(CDevelopmentTeam)

public:
	CDevelopmentTeam(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDevelopmentTeam();

// Dialog Data
	enum { IDD = IDD_DEVELOPMENTTEAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};

#pragma once


// CPortalDBG dialog

class CPortalDBG : public CDialogEx
{
	DECLARE_DYNAMIC(CPortalDBG)

public:
	CPortalDBG(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPortalDBG();

// Dialog Data
	enum { IDD = IDD_PORTAL_DBG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedResolve();
};

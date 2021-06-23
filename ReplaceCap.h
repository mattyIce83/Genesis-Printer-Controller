#pragma once


// CReplaceCap dialog

class CReplaceCap : public CDialogEx
{
	DECLARE_DYNAMIC(CReplaceCap)

public:
	CReplaceCap(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReplaceCap();

// Dialog Data
	enum { IDD = IDD_REPLACECAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

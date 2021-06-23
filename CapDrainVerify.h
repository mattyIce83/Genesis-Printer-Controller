#pragma once


// CCapDrainVerify dialog

class CCapDrainVerify : public CDialogEx
{
	DECLARE_DYNAMIC(CCapDrainVerify)

public:
	CCapDrainVerify(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCapDrainVerify();

// Dialog Data
	enum { IDD = IDD_CAPDRAINVERIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};

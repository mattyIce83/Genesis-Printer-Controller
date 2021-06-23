#pragma once


// CNumberEntry dialog

class CNumberEntry : public CDialogEx
{
	DECLARE_DYNAMIC(CNumberEntry)

public:
	CNumberEntry(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNumberEntry();

// Dialog Data
	enum { IDD = IDD_NUMBERENTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedThree();
	afx_msg void OnBnClickedOne();
	afx_msg void OnBnClickedTwo();
	afx_msg void OnBnClickedFour();
	afx_msg void OnBnClickedFive();
	afx_msg void OnBnClickedSix();
	afx_msg void OnBnClickedSeven();
	afx_msg void OnBnClickedEight();
	afx_msg void OnBnClickedNine();
	afx_msg void OnBnClickedZero();
	afx_msg void OnBnClickedSpare();
	afx_msg void OnBnClickedBs();
	CBitmapButton m_BS;
	CBitmapButton m_EIGHT;
	CBitmapButton m_FIVE;
	CBitmapButton m_FOUR;
	CStatic m_Message;
	CBitmapButton m_NINE;
	CBitmapButton m_ONE;
	CEdit m_Output;
	CBitmapButton m_SEVEN;
	CBitmapButton m_SIX;
	CButton m_SP;
	CBitmapButton m_THREE;
	CBitmapButton m_TWO;
	CBitmapButton m_ZERO;
	virtual BOOL OnInitDialog();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	char szOutput[100];
	char szMessage[100];

	char content[100];

	CBitmapButton m_OK;
	CBitmapButton m_CANCEL;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPaint();
};

#pragma once


// CInkValveControl dialog

class CInkValveControl : public CDialogEx
{
	DECLARE_DYNAMIC(CInkValveControl)

public:
	CInkValveControl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInkValveControl();

	void OpenValve(int);
	void CloseValve(int);
	void FillChannel(int);
	void FillAllChannels(void);

	BOOL state[6];

	CProgressCtrl *indicators[6];

// Dialog Data
	enum { IDD = IDD_INK_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	CButton m_ChanLab1;
	CButton m_ChanLab2;
	CButton m_ChanLab3;
	CButton m_ChanLab4;
	CButton m_ChanLab5;
	CButton m_ChanLab6;
	CButton m_Close1;
	CButton m_Close2;
	CButton m_Close3;
	CButton m_Close4;
	CButton m_Close5;
	CButton m_Close6;
	CButton m_Fill1;
	CButton m_Fill2;
	CButton m_Fill3;
	CButton m_Fill4;
	CButton m_Fill5;
	CButton m_Fill6;
	CButton m_Open1;
	CButton m_Open2;
	CButton m_Open3;
	CButton m_Open4;
	CButton m_Open6;
	CButton m_Open5;
	CButton m_btnOK;
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOpenInkValve1();
	afx_msg void OnBnClickedOpenInkValve2();
	afx_msg void OnBnClickedOpenInkValve3();
	afx_msg void OnBnClickedOpenInkValve4();
	afx_msg void OnBnClickedOpenInkValve5();
	afx_msg void OnBnClickedOpenInkValve8();
	afx_msg void OnBnClickedOpenInkValve7();
	afx_msg void OnBnClickedOpenInkValve6();
	afx_msg void OnBnClickedCloseInkValve1();
	afx_msg void OnBnClickedCloseInkValve2();
	afx_msg void OnBnClickedCloseInkValve3();
	afx_msg void OnBnClickedCloseInkValve4();
	afx_msg void OnBnClickedCloseInkValve5();
	afx_msg void OnBnClickedCloseInkValve6();
	afx_msg void OnBnClickedFillCh1();
	afx_msg void OnBnClickedFillCh2();
	afx_msg void OnBnClickedFillCh3();
	afx_msg void OnBnClickedFillCh4();
	afx_msg void OnBnClickedFillCh5();
	afx_msg void OnBnClickedFillCh6();
	CButton m_FillAll;
	CButton m_OpenAll;
	CButton m_CloseAll;
	afx_msg void OnBnClickedOpenAll();
	afx_msg void OnBnClickedCloseAll();
	afx_msg void OnBnClickedFillAll();
	CProgressCtrl m_PosInd1;
	CProgressCtrl m_PosInd2;
	CProgressCtrl m_PosInd3;
	CProgressCtrl m_PosInd4;
	CProgressCtrl m_PosInd5;
	CProgressCtrl m_PosInd6;
};

#pragma once
#include "afxwin.h"


// CController4Dlg5 dialog

class CController4Dlg5 : public CDialogEx
{
	DECLARE_DYNAMIC(CController4Dlg5)

public:
	CController4Dlg5(CWnd* pParent = NULL);   // standard constructor
	virtual ~CController4Dlg5();

	void SizeDialog();
	int ButtonTop;
	int ButtonHeight;
	int ButtonWidth;

// Dialog Data
	enum { IDD = IDD_CONTROLLER4_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_Home;
	virtual BOOL OnInitDialog();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedHome();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedHlp();
	CButton m_CleanLevel1;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedNozzleCheck1();
	afx_msg void OnPaint();
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCleanLevel1();
	CButton m_HeadA;
	CButton m_HeadB;
	CButton m_CleanLevel2;
	CButton m_CleanLevel3;
	afx_msg void OnBnClickedCleanLevel3();
	afx_msg void OnBnClickedCleanLevel2();
	CButton m_NozzleCheck;
	afx_msg void OnBnClickedHeadBath();
	afx_msg void OnBnClickedCapFlush();
	afx_msg void OnBnClickedWipeHead();
	afx_msg void OnBnClickedQuickClean();
	afx_msg void OnBnClickedAdvanced();
	CButton m_Advanced;
	//CBitmapButton m_Advanced;
	afx_msg void OnBnClickedDrainCapStn();
	CButton m_DrainCap;
	afx_msg void OnBnClickedWipeit();
	CButton m_Wipeit;
	CButton m_DDNozzleCheck;
	afx_msg void OnBnClickedDdNozzleCheck();
	afx_msg void OnBnClickedFillCap();
	CButton m_FillCap;
	CButton m_HomeHead;
	CButton m_HeadToMaint;
	afx_msg void OnBnClickedHomehead();
	afx_msg void OnBnClickedHeadtomaint();
	CButton m_EndOfDay;
	afx_msg void OnBnClickedEndOfDay();
};

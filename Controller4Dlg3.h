#pragma once
#include "afxwin.h"
#include "afxcmn.h"


typedef struct step_order_loc {

   short loc;
   unsigned char stepper;

} step_order_loc;


// CController4Dlg3 dialog

class CMyComboBox2: public CComboBox {
	COLORREF m_cr;
	CBrush   m_br;
	DECLARE_MESSAGE_MAP()
	HBRUSH CtlColor ( CDC* pDC, UINT nCtlColor )
	{
		pDC->SetBkColor(m_cr);
		pDC->SetTextColor(RGB(255,255,255));
		return m_br;
	}
public:
	void SetBkColor(COLORREF cr)
	{
		m_cr = cr;
		m_br.CreateSolidBrush(cr);
		Invalidate();
	}
};


class CController4Dlg3 : public CDialogEx
{
	DECLARE_DYNAMIC(CController4Dlg3)

public:
	CController4Dlg3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CController4Dlg3();

	void SizeDialog();
	int ButtonTop;
	int ButtonHeight;
	int ButtonWidth;
	CFont pNewFont;
	int InkSpitState[8];
	CBitmapButton *buttons[8];
	void ReconcileIcons();
	BOOL bInTestMode;
	BOOL old_active;
	unsigned char GetTestChannels();

	void AllSpitOff();

	void DisplayHeight();

	void SetLiftSteppersHeight(int , int , int , int);

	void SetObstructionLaserButtonText();


// Dialog Data
	enum { IDD = IDD_CONTROLLER4_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CBitmapButton m_Home;

	afx_msg void OnBnClickedHome();
	afx_msg void OnBnClickedRight();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedWhite1();
	afx_msg void OnBnClickedTestUsb();
	afx_msg void OnBnClickedLevelOne();
	afx_msg void OnPaint();
	CBitmapButton m_SpitOnOffAA;
	CBitmapButton m_SpitOnOffAB;
	CBitmapButton m_SpitOnOffAC;
	CBitmapButton m_SpitOnOffAD;
	CBitmapButton m_SpitOnOffBA;
	CBitmapButton m_SpitOnOffBB;
	CBitmapButton m_SpitOnOffBC;
	CBitmapButton m_SpitOnOffBD;
	afx_msg void OnBnClickedSpitonoffAa();
	afx_msg void OnBnClickedSpitonoffAb();
	afx_msg void OnBnClickedSpitonoffAc();
	afx_msg void OnBnClickedSpitonoffAd();
	afx_msg void OnBnClickedSpitonoffBa();
	afx_msg void OnBnClickedSpitonoffBb();
	afx_msg void OnBnClickedSpitonoffBc();
	afx_msg void OnBnClickedSpitonoffBd();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnBnClickedDiag2();
	afx_msg void OnBnClickedAllHeadA();
	afx_msg void OnBnClickedAllHeadB();
	afx_msg void OnBnClickedLevelPlaten();
	afx_msg void OnBnClickedHeadMaintPos();
	afx_msg void OnBnClickedHomeHead();
	CButton m_AdvancedDiag;
	CButton m_LoadSoftware;
	afx_msg void OnBnClickedLoadSoftware();
//	CMyComboBox2 m_PTOffset;
//	afx_msg void OnCbnSelchangePtOffset();
	afx_msg void OnBnClickedPower();
	CButton m_DemoMode;
	afx_msg void OnBnClickedDemoMode();
	CEdit m_DefaultHeight;
	afx_msg void OnEnChangeDefaultheight();
	afx_msg void OnBnClickedHomez();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCheckfeedheight();
	CButton m_CheckFeed;
	CButton m_RehomeZ;
	CButton m_IgnoreObstruction;
	afx_msg void OnBnClickedIgnoreObstruction();
	CButton m_AutoHeight;
	CButton m_AutoLevel;
	CButton m_HeadToMaint;
	CButton m_HomeHead;
	CStatic m_PlatenGapText;
	CButton m_ManualHeight;
	CButton m_ManualLevel;
	afx_msg void OnBnClickedLevelPlaten4();
	CStatic m_PlatenHeightsGroup;
	CEdit m_FeedHeight;
	CStatic m_PrintHeightText;
	CStatic m_FeedHeightText;
	CButton m_CheckPrint;
	afx_msg void OnBnClickedManualHeight();
	afx_msg void OnBnClickedCheckprintheight();
	CStatic m_PlatenLevelingGroup;
	CStatic m_OnDemandNozzleFireGroup;
	CStatic m_AdvancedGroup;
	afx_msg void OnBnClickedLevelPlaten3();
	afx_msg void OnBnClickedRemoteAction();
	CButton m_RemoteMaint;
	CButton m_ShutDown;
	CBitmapButton m_PlatenGapUp;
	CBitmapButton m_PlatenGapDown;
	CListBox m_PlatenGapList;
	afx_msg void OnLbnSelchangePlatengap();
	afx_msg void OnBnClickedPlatenGapPlus();
	afx_msg void OnBnClickedPlatenGapMinus();
	afx_msg void OnBnClickedParkplaten();
	CButton m_ParkPlaten;
};

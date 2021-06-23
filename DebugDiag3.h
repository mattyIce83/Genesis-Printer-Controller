#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDebugDiag3 dialog

class CDebugDiag3 : public CDialogEx
{
	DECLARE_DYNAMIC(CDebugDiag3)

public:
	CDebugDiag3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDebugDiag3();


	CBitmap hOffIndicator;
	CBitmap hOffIndicator2;
	CBitmap hOnIndicator;

	void UpdateValveIcons();

	unsigned char GetChannels();

	BOOL bRunPressureUpdater;

	void VacSelButtons();
	void VacSelButtonsUpdate();

	void UpdateVacDisplay();

	int m_nTimer;

	void SizeDialog();

	CCriticalSection DspCC;


// Dialog Data
	enum { IDD = IDD_DEBUG_DIAG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Black;
	CButton m_Cyan;
	CButton m_Flush;
	CButton m_Magenta;
	CButton m_White;
	CButton m_Yellow;
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOpenpeb();
	afx_msg void OnBnClickedClosepeb();
	afx_msg void OnBnClickedOpenink();
	afx_msg void OnBnClickedCloseink();
	CStatic m_HeadAVac;
	CStatic m_HeadBVac;
	CStatic m_VP_Indicator;
	CStatic m_VS_Indicator;
	afx_msg void OnBnClickedVacrelease();
	afx_msg void OnBnClickedVacpumpoff();
	afx_msg void OnBnClickedVacpumpon();
	afx_msg void OnBnClickedCap();
	afx_msg void OnBnClickedUncap();
	afx_msg void OnBnClickedHomehead();
	afx_msg void OnBnClickedHeadtomaint();
	afx_msg void OnBnClickedWipe();
	afx_msg void OnBnClickedVacseal();
	CStatic m_F_Indicator_I;
	CStatic m_C_Indicator_I;
	CStatic m_M_Indicator_I;
	CStatic m_Y_Indicator_I;
	CStatic m_K_Indicator_I;
	CStatic m_W_Indicator_I;
	CStatic m_Cap_Indicator;
	CStatic m_HeadClamp_A_Indicator;
	CStatic m_HeadClamp_B_Indicator;
	afx_msg void OnBnClickedHeadClampAOn();
	afx_msg void OnBnClickedHeadClampAOff();
	afx_msg void OnBnClickedHeadClampBOn();
	afx_msg void OnBnClickedHeadClampBOff();
	CButton m_VacSideA;
	CButton m_VacSideB;
	afx_msg void OnBnClickedVacselA();
	afx_msg void OnBnClickedVacselB();
	CStatic m_DamperClamp_Indicator;
	afx_msg void OnBnClickedDamperClampOpen();
	afx_msg void OnBnClickedDamperClampClose();
	afx_msg void OnBnClickedFlushpumpon();
	afx_msg void OnBnClickedFlushpumpoff();
	afx_msg void OnBnClickedOpencloseall();
	afx_msg void OnBnClickedWhite();
	CButton m_AllChannels;
	CButton m_WhiteR;
	CStatic m_WR_Indicator_I;
	afx_msg void OnBnClickedWhitePumpLeft();
	afx_msg void OnBnClickedWhitePumpRight();
	afx_msg void OnBnClickedCyanPump();
	afx_msg void OnBnClickedMagentaPump();
	afx_msg void OnBnClickedYellowPump();
	afx_msg void OnBnClickedBlackPump();
	afx_msg void OnBnClickedFlushPump();
	CStatic m_PumpWL_Indicator;
	CStatic m_PumpWR_Indicator;
	CStatic m_PumpC_Indicator;
	CStatic m_PumpM_Indicator;
	CStatic m_PumpY_Indicator;
	CStatic m_PumpK_Indicator;
	CStatic m_PumpF_Indicator;
	CButton m_WLOnOff;
	CButton m_WROnOff;
	CButton m_COnOff;
	CButton m_MOnOff;
	CButton m_YOnOff;
	CButton m_KOnOff;
	CButton m_FOnOff;
	afx_msg void OnBnClickedWhitemixForward();
	afx_msg void OnBnClickedWhitemixBackward();
	CButton m_WMixForward;
	CButton m_WMixBackward;
	afx_msg void OnBnClickedWhitemixOff();
	CStatic m_WMixForwardIndicator;
	CStatic m_WMixBackwardIndicator;
	CButton m_FLBagPresent;
	CButton m_CBagPresent;
	CButton m_MBagPresent;
	CButton m_YBagPresent;
	CButton m_KBagPresent;
	CButton m_WBagPresent;
	CEdit m_FBagPressure;
	CEdit m_CBagPressure;
	CEdit m_MBagPressure;
	CEdit m_YBagPressure;
	CEdit m_KBagPressure;
	CEdit m_WBagPressure;
	CStatic m_FBagIndicator;
	CStatic m_CBagIndicator;
	CStatic m_MBagIndicator;
	CStatic m_YBagIndicator;
	CStatic m_KBagIndicator;
	CStatic m_WBagIndicator;
	afx_msg void OnBnClickedRefreshbags();
	afx_msg void OnBnClickedDamperClampClose2();
	afx_msg void OnBnClickedDamperClampOpen2();
	afx_msg void OnBnClickedRefreshAdc();
	afx_msg void OnBnClickedRefreshCont();
	CStatic m_CapVacADC;
	CStatic m_CapVacPSI;
	CProgressCtrl m_CapVacLevel;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_RefreshOnce;
	CButton m_RefreshCont;
	afx_msg void OnBnClickedHeadClampAOn2();
	afx_msg void OnBnClickedHeadClampBOn2();
	afx_msg void OnBnClickedDamperClampClose3();
	afx_msg void OnBnClickedDamperClampClose4();
	CStatic m_FlushThrough_Indicator;
	afx_msg void OnBnClickedWhiteDrip();
};

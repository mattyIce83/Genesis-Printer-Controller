// DebugDiag3.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "DebugDiag3.h"
#include "Controller4Dlg1.h"
#include "afxdialogex.h"
#include "WhiteCleaning.h"

#include "parser.h"
#include "HEAD_structs.h"

// CDebugDiag dialog

extern CController4Dlg1 *theDlg;

extern void SendPlatenHeightOffset();

extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;



CDebugDiag3 *dlg3, *thisDlg;

// CDebugDiag3 dialog

IMPLEMENT_DYNAMIC(CDebugDiag3, CDialogEx)

static BOOL bNoPMW[4];

#define REFRESH_MS	400

void PressureUpdater3(void *p) {
int i;

	while(thisDlg->bRunPressureUpdater) {

		/**/ TRACE("Getting ink levels (4)\n");
		USBPrinterCommand(USB_CMD_GET_INK_STATE, 0xFF);

		for(i = 0; i < REFRESH_MS; i++) {
			if(!thisDlg->bRunPressureUpdater) return;
			Sleep(1);
		}

	}


}


void UpdateValveMarkers(unsigned short valves) {

	if(dlg3 != NULL)
		dlg3->UpdateValveIcons();


}

CDebugDiag3::CDebugDiag3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebugDiag3::IDD, pParent)
{

}

CDebugDiag3::~CDebugDiag3()
{
}

void CDebugDiag3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BLACK, m_Black);
	DDX_Control(pDX, IDC_CYAN, m_Cyan);
	DDX_Control(pDX, IDC_FLUSH, m_Flush);
	DDX_Control(pDX, IDC_MAGENTA, m_Magenta);
	DDX_Control(pDX, IDC_WHITE, m_White);
	DDX_Control(pDX, IDC_YELLOW, m_Yellow);
	DDX_Control(pDX, IDC_F_INDICATOR2, m_HeadAVac);
	DDX_Control(pDX, IDC_C_INDICATOR2, m_HeadBVac);
	DDX_Control(pDX, IDC_VP_INDICATOR, m_VP_Indicator);
	DDX_Control(pDX, IDC_VS_INDICATOR, m_VS_Indicator);
	DDX_Control(pDX, IDC_F_INDICATOR_I, m_F_Indicator_I);
	DDX_Control(pDX, IDC_C_INDICATOR_I, m_C_Indicator_I);
	DDX_Control(pDX, IDC_M_INDICATOR_I, m_M_Indicator_I);
	DDX_Control(pDX, IDC_Y_INDICATOR_I, m_Y_Indicator_I);
	DDX_Control(pDX, IDC_K_INDICATOR_I, m_K_Indicator_I);
	DDX_Control(pDX, IDC_W_INDICATOR_I, m_W_Indicator_I);
	DDX_Control(pDX, IDC_CAP_INDICATOR, m_Cap_Indicator);
	DDX_Control(pDX, IDC_HEAD_CLAMP_A_INDICATOR, m_HeadClamp_A_Indicator);
	DDX_Control(pDX, IDC_HEAD_CLAMP_A_INDICATOR2, m_HeadClamp_B_Indicator);
	DDX_Control(pDX, IDC_VACSEL_A, m_VacSideA);
	DDX_Control(pDX, IDC_VACSEL_B, m_VacSideB);
	DDX_Control(pDX, IDC_DAMPER_CLAMP, m_DamperClamp_Indicator);
	DDX_Control(pDX, IDC_OPENCLOSEALL, m_AllChannels);
	DDX_Control(pDX, IDC_WHITE5, m_WhiteR);
	DDX_Control(pDX, IDC_W_INDICATOR_I2, m_WR_Indicator_I);
	DDX_Control(pDX, IDC_WHITEPUMP_LEFT_INDICATOR, m_PumpWL_Indicator);
	DDX_Control(pDX, IDC_WHITEPUMP_RIGHT_INDICATOR, m_PumpWR_Indicator);
	DDX_Control(pDX, IDC_CYANPUMP_INDICATOR, m_PumpC_Indicator);
	DDX_Control(pDX, IDC_MAGENTAPUMP_INDICATOR, m_PumpM_Indicator);
	DDX_Control(pDX, IDC_YELLOWPUMP_INDICATOR, m_PumpY_Indicator);
	DDX_Control(pDX, IDC_BLACKPUMP_INDICATOR, m_PumpK_Indicator);
	DDX_Control(pDX, IDC_FLUSHPUMP_INDICATOR, m_PumpF_Indicator);
	DDX_Control(pDX, IDC_WHITE_PUMP_LEFT, m_WLOnOff);
	DDX_Control(pDX, IDC_WHITE_PUMP_RIGHT, m_WROnOff);
	DDX_Control(pDX, IDC_CYAN_PUMP, m_COnOff);
	DDX_Control(pDX, IDC_MAGENTA_PUMP, m_MOnOff);
	DDX_Control(pDX, IDC_YELLOW_PUMP, m_YOnOff);
	DDX_Control(pDX, IDC_BLACK_PUMP, m_KOnOff);
	DDX_Control(pDX, IDC_FLUSH_PUMP, m_FOnOff);
	DDX_Control(pDX, IDC_WHITEMIX_FORWARD, m_WMixForward);
	DDX_Control(pDX, IDC_WHITEMIX_BACKWARD, m_WMixBackward);
	DDX_Control(pDX, IDC_WHITEMIX_FORWARD_INDICATOR, m_WMixForwardIndicator);
	DDX_Control(pDX, IDC_WHITEMIX_BACKWARD_INDICATOR, m_WMixBackwardIndicator);
	DDX_Control(pDX, IDC_F_BAGPRESENT, m_FLBagPresent);
	DDX_Control(pDX, IDC_C_BAGPRESENT, m_CBagPresent);
	DDX_Control(pDX, IDC_M_BAGPRESENT, m_MBagPresent);
	DDX_Control(pDX, IDC_Y_BAGPRESENT, m_YBagPresent);
	DDX_Control(pDX, IDC_K_BAGPRESENT, m_KBagPresent);
	DDX_Control(pDX, IDC_W_BAGPRESENT, m_WBagPresent);
	DDX_Control(pDX, IDC_FBAGPRESS, m_FBagPressure);
	DDX_Control(pDX, IDC_CBAGPRESS, m_CBagPressure);
	DDX_Control(pDX, IDC_MBAGPRESS, m_MBagPressure);
	DDX_Control(pDX, IDC_YBAGPRESS, m_YBagPressure);
	DDX_Control(pDX, IDC_KBAGPRESS, m_KBagPressure);
	DDX_Control(pDX, IDC_WBAGPRESS, m_WBagPressure);
	DDX_Control(pDX, IDC_FLUSH_BAG_INDICATOR, m_FBagIndicator);
	DDX_Control(pDX, IDC_CYAN_BAG_INDICATOR, m_CBagIndicator);
	DDX_Control(pDX, IDC_MAGENTA_BAG_INDICATOR, m_MBagIndicator);
	DDX_Control(pDX, IDC_YELLOW_BAG_INDICATOR, m_YBagIndicator);
	DDX_Control(pDX, IDC_BLACK_BAG_INDICATOR, m_KBagIndicator);
	DDX_Control(pDX, IDC_WHITE_BAG_INDICATOR, m_WBagIndicator);
	DDX_Control(pDX, IDC_CAPVACADCLAB, m_CapVacADC);
	DDX_Control(pDX, IDC_CAPVAC_LAB, m_CapVacPSI);
	DDX_Control(pDX, IDC_PROGRESS1, m_CapVacLevel);
	DDX_Control(pDX, IDC_REFRESH_ADC, m_RefreshOnce);
	DDX_Control(pDX, IDC_REFRESH_CONT, m_RefreshCont);
	DDX_Control(pDX, IDC_DAMPER_CLAMP2, m_FlushThrough_Indicator);
}


BEGIN_MESSAGE_MAP(CDebugDiag3, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OPENINK, &CDebugDiag3::OnBnClickedOpenink)
	ON_BN_CLICKED(IDC_CLOSEINK, &CDebugDiag3::OnBnClickedCloseink)
	ON_BN_CLICKED(IDC_VACRELEASE, &CDebugDiag3::OnBnClickedVacrelease)
	ON_BN_CLICKED(IDC_VACPUMPOFF, &CDebugDiag3::OnBnClickedVacpumpoff)
	ON_BN_CLICKED(IDC_VACPUMPON, &CDebugDiag3::OnBnClickedVacpumpon)
	ON_BN_CLICKED(IDC_CAP, &CDebugDiag3::OnBnClickedCap)
	ON_BN_CLICKED(IDC_UNCAP, &CDebugDiag3::OnBnClickedUncap)
	ON_BN_CLICKED(IDC_HOMEHEAD, &CDebugDiag3::OnBnClickedHomehead)
	ON_BN_CLICKED(IDC_HEADTOMAINT, &CDebugDiag3::OnBnClickedHeadtomaint)
	ON_BN_CLICKED(IDC_WIPE, &CDebugDiag3::OnBnClickedWipe)
	ON_BN_CLICKED(IDC_VACSEAL, &CDebugDiag3::OnBnClickedVacseal)
	ON_BN_CLICKED(IDC_HEAD_CLAMP_A_ON, &CDebugDiag3::OnBnClickedHeadClampAOn)
	ON_BN_CLICKED(IDC_HEAD_CLAMP_A_OFF, &CDebugDiag3::OnBnClickedHeadClampAOff)
	ON_BN_CLICKED(IDC_HEAD_CLAMP_B_ON, &CDebugDiag3::OnBnClickedHeadClampBOn)
	ON_BN_CLICKED(IDC_HEAD_CLAMP_B_OFF, &CDebugDiag3::OnBnClickedHeadClampBOff)
	ON_BN_CLICKED(IDC_VACSEL_A, &CDebugDiag3::OnBnClickedVacselA)
	ON_BN_CLICKED(IDC_VACSEL_B, &CDebugDiag3::OnBnClickedVacselB)
	ON_BN_CLICKED(IDC_DAMPER_CLAMP_OPEN, &CDebugDiag3::OnBnClickedDamperClampOpen)
	ON_BN_CLICKED(IDC_DAMPER_CLAMP_CLOSE, &CDebugDiag3::OnBnClickedDamperClampClose)
	ON_BN_CLICKED(IDC_OPENCLOSEALL, &CDebugDiag3::OnBnClickedOpencloseall)
	ON_BN_CLICKED(IDC_WHITE, &CDebugDiag3::OnBnClickedWhite)
	ON_BN_CLICKED(IDC_WHITE_PUMP_LEFT, &CDebugDiag3::OnBnClickedWhitePumpLeft)
	ON_BN_CLICKED(IDC_WHITE_PUMP_RIGHT, &CDebugDiag3::OnBnClickedWhitePumpRight)
	ON_BN_CLICKED(IDC_CYAN_PUMP, &CDebugDiag3::OnBnClickedCyanPump)
	ON_BN_CLICKED(IDC_MAGENTA_PUMP, &CDebugDiag3::OnBnClickedMagentaPump)
	ON_BN_CLICKED(IDC_YELLOW_PUMP, &CDebugDiag3::OnBnClickedYellowPump)
	ON_BN_CLICKED(IDC_BLACK_PUMP, &CDebugDiag3::OnBnClickedBlackPump)
	ON_BN_CLICKED(IDC_FLUSH_PUMP, &CDebugDiag3::OnBnClickedFlushPump)
	ON_BN_CLICKED(IDC_WHITEMIX_FORWARD, &CDebugDiag3::OnBnClickedWhitemixForward)
	ON_BN_CLICKED(IDC_WHITEMIX_BACKWARD, &CDebugDiag3::OnBnClickedWhitemixBackward)
	ON_BN_CLICKED(IDC_WHITEMIX_OFF, &CDebugDiag3::OnBnClickedWhitemixOff)
	ON_BN_CLICKED(IDC_REFRESHBAGS, &CDebugDiag3::OnBnClickedRefreshbags)
	ON_BN_CLICKED(IDC_DAMPER_CLAMP_CLOSE2, &CDebugDiag3::OnBnClickedDamperClampClose2)
	ON_BN_CLICKED(IDC_DAMPER_CLAMP_OPEN2, &CDebugDiag3::OnBnClickedDamperClampOpen2)
	ON_BN_CLICKED(IDC_REFRESH_ADC, &CDebugDiag3::OnBnClickedRefreshAdc)
	ON_BN_CLICKED(IDC_REFRESH_CONT, &CDebugDiag3::OnBnClickedRefreshCont)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_HEAD_CLAMP_A_ON2, &CDebugDiag3::OnBnClickedHeadClampAOn2)
	ON_BN_CLICKED(IDC_HEAD_CLAMP_B_ON2, &CDebugDiag3::OnBnClickedHeadClampBOn2)
	ON_BN_CLICKED(IDC_DAMPER_CLAMP_CLOSE3, &CDebugDiag3::OnBnClickedDamperClampClose3)
	ON_BN_CLICKED(IDC_DAMPER_CLAMP_CLOSE4, &CDebugDiag3::OnBnClickedDamperClampClose4)
	ON_BN_CLICKED(IDC_WHITE_DRIP, &CDebugDiag3::OnBnClickedWhiteDrip)
END_MESSAGE_MAP()


// CDebugDiag3 message handlers


void CDebugDiag3::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);

}


void CDebugDiag3::UpdateVacDisplay() {
static int oldADC;
int cappress;
char tmp_str[80];

	DspCC.Lock();

	if(oldADC != theApp.GVars->CapADC) {

		oldADC = theApp.GVars->CapADC;

		cappress = theApp.CalculateCapPressure(theApp.GVars->CapADC);
	
		m_CapVacLevel.SetPos(cappress);

		sprintf(tmp_str, "Cap Vac Psi: %d", cappress);
		m_CapVacPSI.SetWindowText(tmp_str);

		sprintf(tmp_str, "Cap Vac ADC: %d", theApp.GVars->CapADC);
		m_CapVacADC.SetWindowText(tmp_str);


	}
	DspCC.Unlock();
}

BOOL CDebugDiag3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SizeDialog();

	bRunPressureUpdater = FALSE;

	theApp.bBlockTempThread = TRUE;

	hOffIndicator.LoadBitmap(IDB_OFF_MARKER2);
	hOffIndicator2.LoadBitmap(IDB_OFF_MARKER3);
	hOnIndicator.LoadBitmap(IDB_ON_MARKER2);
	
	thisDlg = dlg3 = this;

	if(!theApp.GVars->bDisableInkADC) {
		/**/ TRACE("Getting ink levels (5)\n");
		USBPrinterCommand(USB_CMD_GET_INK_STATE, 0xFF);
	}

	USBPrinterCommand(USB_CMD_UPDATE_VALVES, 0x01|0x02|0x04|0x08|0x10);

	if(!theApp.GVars->bDisableInkADC) {
		/**/ TRACE("Getting ink levels (3a)\n");
		USBPrinterCommand(USB_CMD_GET_INK_STATE, 0x40|0x80|0x10|0x20);
	}

	m_CapVacLevel.SetRange(0, (int)FULL_VAC_PSI);

	m_nTimer = SetTimer(0x6971, 250, 0);

	dlg3->UpdateValveIcons();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDebugDiag3::UpdateValveIcons() {

	if(theApp.GVars->Valves.bWhiteNormal) {
		if(bNoPMW[0]) {
			m_HeadClamp_A_Indicator.SetBitmap((HBITMAP)hOffIndicator2);
		}
		else {
			m_HeadClamp_A_Indicator.SetBitmap((HBITMAP)hOnIndicator);
		}
	}
	else {
		m_HeadClamp_A_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bWhiteRecirc) {
		if(bNoPMW[1]) {
			m_HeadClamp_B_Indicator.SetBitmap((HBITMAP)hOffIndicator2);
		}
		else {
			m_HeadClamp_B_Indicator.SetBitmap((HBITMAP)hOnIndicator);
		}
	}
	else {
		m_HeadClamp_B_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bWhiteFlush) {
		if(bNoPMW[2]) {
			m_DamperClamp_Indicator.SetBitmap((HBITMAP)hOffIndicator2);
		}
		else {
			m_DamperClamp_Indicator.SetBitmap((HBITMAP)hOnIndicator);
		}
	}
	else {
		m_DamperClamp_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bFlushThrough) {
		if(bNoPMW[3]) {
			m_FlushThrough_Indicator.SetBitmap((HBITMAP)hOffIndicator2);
		}
		else {
			m_FlushThrough_Indicator.SetBitmap((HBITMAP)hOnIndicator);
		}
	}
	else {
		m_FlushThrough_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}




	if(theApp.GVars->Valves.bF_InkValve) {
		m_F_Indicator_I.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_F_Indicator_I.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bC_InkValve) {
		m_C_Indicator_I.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_C_Indicator_I.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bM_InkValve) {
		m_M_Indicator_I.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_M_Indicator_I.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bY_InkValve) {
		m_Y_Indicator_I.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_Y_Indicator_I.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bK_InkValve) {
		m_K_Indicator_I.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_K_Indicator_I.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bW_InkValveL) {
		m_W_Indicator_I.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_W_Indicator_I.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bW_InkValveR) {
		m_WR_Indicator_I.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_WR_Indicator_I.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bVacPump) {
		m_VP_Indicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_VP_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bVacSeal) {
		m_VS_Indicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_VS_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bHeadASel) {
		m_HeadAVac.SetBitmap((HBITMAP)hOnIndicator);
		m_VacSideA.SetCheck(TRUE);
	}
	else {
		m_HeadAVac.SetBitmap((HBITMAP)hOffIndicator);
		m_VacSideA.SetCheck(FALSE);
	}
	if(theApp.GVars->Valves.bHeadBSel) {
		m_HeadBVac.SetBitmap((HBITMAP)hOnIndicator);
		m_VacSideB.SetCheck(TRUE);
	}
	else {
		m_HeadBVac.SetBitmap((HBITMAP)hOffIndicator);
		m_VacSideB.SetCheck(FALSE);
	}

	if(theApp.GVars->Valves.bHeadCapped) {
		m_Cap_Indicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_Cap_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	////////////////

	if(theApp.GVars->Valves.bFlushPump) {
		m_PumpF_Indicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_PumpF_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bCPump) {
		m_PumpC_Indicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_PumpC_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bMPump) {
		m_PumpM_Indicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_PumpM_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bYPump) {
		m_PumpY_Indicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_PumpY_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bKPump) {
		m_PumpK_Indicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_PumpK_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bWLPump) {
		m_PumpWL_Indicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_PumpWL_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bWRPump) {
		m_PumpWR_Indicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_PumpWR_Indicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bWMixForward) {
		m_WMixForwardIndicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_WMixForwardIndicator.SetBitmap((HBITMAP)hOffIndicator);
	}

	if(theApp.GVars->Valves.bWMixBackward) {
		m_WMixBackwardIndicator.SetBitmap((HBITMAP)hOnIndicator);
	}
	else {
		m_WMixBackwardIndicator.SetBitmap((HBITMAP)hOffIndicator);
	}


	////////////////////

	char tmp_buff[64];

	if(theApp.GVars->Valves.bFBagPresent) {
		m_FBagIndicator.SetBitmap((HBITMAP)hOnIndicator);
		m_FLBagPresent.SetCheck(TRUE);
		m_FBagPressure.ShowWindow(SW_SHOW);
		//sprintf(tmp_buff, "%02.03f", theApp.GVars->Valves.fFBagPressure);
		//sprintf(tmp_buff, "%d", theApp.GVars->inkADC[0]);
		sprintf(tmp_buff, "ADC: %d/%02.03f psi/%3.3f%%", theApp.GVars->inkADC[0], theApp.GVars->Valves.fFBagPressure, theApp.GVars->inkFullPercent[0]);
		m_FBagPressure.SetWindowText(tmp_buff);
	}
	else {
		m_FBagIndicator.SetBitmap((HBITMAP)hOffIndicator);
		m_FLBagPresent.SetCheck(FALSE);
		m_FBagPressure.ShowWindow(SW_HIDE);
	}

	if(theApp.GVars->Valves.bCBagPresent) {
		m_CBagIndicator.SetBitmap((HBITMAP)hOnIndicator);
		m_CBagPresent.SetCheck(TRUE);
		m_CBagPressure.ShowWindow(SW_SHOW);
		//sprintf(tmp_buff, "%02.03f", theApp.GVars->Valves.fCBagPressure);
		//sprintf(tmp_buff, "%d", theApp.GVars->inkADC[3]);
		sprintf(tmp_buff, "ADC: %d/%02.03f psi/%3.3f%%", theApp.GVars->inkADC[3], theApp.GVars->Valves.fCBagPressure, theApp.GVars->inkFullPercent[3]);
		m_CBagPressure.SetWindowText(tmp_buff);
	}
	else {
		m_CBagIndicator.SetBitmap((HBITMAP)hOffIndicator);
		m_CBagPresent.SetCheck(FALSE);
		m_CBagPressure.ShowWindow(SW_HIDE);

	}

	if(theApp.GVars->Valves.bMBagPresent) {
		m_MBagIndicator.SetBitmap((HBITMAP)hOnIndicator);
		m_MBagPresent.SetCheck(TRUE);
		m_MBagPressure.ShowWindow(SW_SHOW);
		//sprintf(tmp_buff, "%02.03f", theApp.GVars->Valves.fMBagPressure);
		sprintf(tmp_buff, "%d", theApp.GVars->inkADC[2]);
		m_MBagPressure.SetWindowText(tmp_buff);
	}
	else {
		m_MBagIndicator.SetBitmap((HBITMAP)hOffIndicator);
		m_MBagPresent.SetCheck(FALSE);
		m_MBagPressure.ShowWindow(SW_HIDE);

	}

	if(theApp.GVars->Valves.bYBagPresent) {
		m_YBagIndicator.SetBitmap((HBITMAP)hOnIndicator);
		m_YBagPresent.SetCheck(TRUE);
		m_YBagPressure.ShowWindow(SW_SHOW);
//		sprintf(tmp_buff, "%02.03f", theApp.GVars->Valves.fYBagPressure);
		sprintf(tmp_buff, "%d", theApp.GVars->inkADC[1]);
		m_YBagPressure.SetWindowText(tmp_buff);
	}
	else {
		m_YBagIndicator.SetBitmap((HBITMAP)hOffIndicator);
		m_YBagPresent.SetCheck(FALSE);
		m_YBagPressure.ShowWindow(SW_HIDE);

	}

	if(theApp.GVars->Valves.bKBagPresent) {
		m_KBagIndicator.SetBitmap((HBITMAP)hOnIndicator);
		m_KBagPresent.SetCheck(TRUE);
		m_KBagPressure.ShowWindow(SW_SHOW);
		//sprintf(tmp_buff, "%02.03f", theApp.GVars->Valves.fKBagPressure);
		sprintf(tmp_buff, "%d", theApp.GVars->inkADC[4]);
		m_KBagPressure.SetWindowText(tmp_buff);
	}
	else {
		m_KBagIndicator.SetBitmap((HBITMAP)hOffIndicator);
		m_KBagPresent.SetCheck(FALSE);
		m_KBagPressure.ShowWindow(SW_HIDE);

	}

	if(theApp.GVars->Valves.bWBagPresent) {
		m_WBagIndicator.SetBitmap((HBITMAP)hOnIndicator);
		m_WBagPresent.SetCheck(TRUE);
		m_WBagPressure.ShowWindow(SW_SHOW);
		//sprintf(tmp_buff, "%02.03f", theApp.GVars->Valves.fWBagPressure);
		sprintf(tmp_buff, "%d", theApp.GVars->inkADC[5]);
		m_WBagPressure.SetWindowText(tmp_buff);
	}
	else {
		m_WBagIndicator.SetBitmap((HBITMAP)hOffIndicator);
		m_WBagPresent.SetCheck(FALSE);
		m_WBagPressure.ShowWindow(SW_HIDE);
	}

	// VacSelButtonsUpdate();

}

void CDebugDiag3::OnDestroy()
{

	KillTimer(m_nTimer);

	dlg3 = NULL;

	CDialogEx::OnDestroy();

	if(thisDlg->bRunPressureUpdater) {
		thisDlg->bRunPressureUpdater = FALSE;
		Sleep(500);
	}

	theApp.bBlockTempThread = FALSE;

	// TODO: Add your message handler code here
}

void CDebugDiag3::OnBnClickedOpenpeb() {

	BOOL state = FALSE;

	unsigned long param = 0;

	param |= GetChannels() << 24;

	if(!state)
		param |= (1 << 16);

	USBPrinterCommand(USB_CMD_OPEN_PEB, param);

}


void CDebugDiag3::OnBnClickedClosepeb()
{

	BOOL state = TRUE;

	unsigned long param = 0;

	param |= GetChannels() << 24;

	if(!state)
		param |= (1 << 16);

	USBPrinterCommand(USB_CMD_OPEN_PEB, param);

}


void CDebugDiag3::OnBnClickedOpenink()
{
	BOOL state = FALSE;

	unsigned long param = 0;

	param |= GetChannels() << 24;

	if(!state)
		param |= (1 << 16);

	USBPrinterCommand(USB_CMD_OPEN_INK, param);
}


void CDebugDiag3::OnBnClickedCloseink()
{
	BOOL state = TRUE;

	unsigned long param = 0;

	param |= GetChannels() << 24;

	if(!state)
		param |= (1 << 16);

	USBPrinterCommand(USB_CMD_OPEN_INK, param);
}

unsigned char CDebugDiag3::GetChannels() {
unsigned char ret = 0;


	if(m_Flush.GetCheck()) ret |= 0x01;
	if(m_Cyan.GetCheck()) ret |= 0x02;
	if(m_Magenta.GetCheck()) ret |= 0x04;
	if(m_Yellow.GetCheck()) ret |= 0x08;
	if(m_Black.GetCheck()) ret |= 0x10;
	if(m_White.GetCheck()) ret |= 0x20;
	if(m_WhiteR.GetCheck()) ret |= 0x40;

	return ret;

}

void CDebugDiag3::OnBnClickedVacrelease()
{
	USBPrinterCommand(USB_CMD_VAC_VALVE, 1);
}


void CDebugDiag3::OnBnClickedVacpumpoff()
{
	USBPrinterCommand(USB_CMD_VAC_PUMP, 0);
}


void CDebugDiag3::OnBnClickedVacpumpon()
{

	if(!theApp.InksOKToPrint()) {
		theDlg->OKBox("*** WARNING ***", "ONE OR MORE INK", "CHANNEL EMPTY!");
	}

	USBPrinterCommand(USB_CMD_VAC_PUMP, 1);
}


void CDebugDiag3::OnBnClickedCap()
{
	USBPrinterCommand(USB_CMD_CAP, 1);
	USBPrinterCommand(USB_CMD_UPDATE_VALVES, 0x04);
}


void CDebugDiag3::OnBnClickedUncap()
{
	USBPrinterCommand(USB_CMD_CAP, 0);
	USBPrinterCommand(USB_CMD_UPDATE_VALVES, 0x04);
}


void CDebugDiag3::OnBnClickedHomehead()
{
	USBPrinterCommand(USB_CMD_CAP, 0);
	USBPrinterCommand(USB_CMD_PARK, 0);
	//USBPrinterCommand(USB_CMD_CAP, 1);
}


void CDebugDiag3::OnBnClickedHeadtomaint()
{
	USBPrinterCommand(USB_CMD_CAP, 0);
	USBPrinterCommand(USB_CMD_VAC_VALVE, 0);
	USBPrinterCommand(USB_HEAD_TO_MAINT, 0);
}


void CDebugDiag3::OnBnClickedWipe()
{
	USBPrinterCommand(USB_CMD_WIPE, 1);
}

void CDebugDiag3::OnBnClickedVacseal()
{
	USBPrinterCommand(USB_CMD_VAC_VALVE, 0);
}

void CDebugDiag3::OnBnClickedHeadClampAOn()
{
	bNoPMW[0] = FALSE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 2, 2, 0, 0, 0, 0, 0);
}


void CDebugDiag3::OnBnClickedHeadClampAOff()
{
	bNoPMW[0] = FALSE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 2, 0, 0, 0, 0, 0, 0);
}


void CDebugDiag3::OnBnClickedHeadClampBOn()
{
	bNoPMW[1] = FALSE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 4, 2, 0, 0, 0, 0, 0);
}


void CDebugDiag3::OnBnClickedHeadClampBOff()
{
	bNoPMW[1] = FALSE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 4, 0, 0, 0, 0, 0, 0);
}


void CDebugDiag3::VacSelButtons() {
BOOL AIsOpen, BIsOpen;

	AIsOpen = m_VacSideA.GetCheck();
	BIsOpen = m_VacSideB.GetCheck();

	if(AIsOpen && !BIsOpen) {
		USBPrinterCommand(USB_CMD_VAC_SELECT, 0); // open a
	}
	if(!AIsOpen && BIsOpen) {
		USBPrinterCommand(USB_CMD_VAC_SELECT, 1); // open B
	}
	if(AIsOpen && BIsOpen) {
		USBPrinterCommand(USB_CMD_VAC_SELECT, 2); // open both
	}
	if(!AIsOpen && !BIsOpen) {
		USBPrinterCommand(USB_CMD_VAC_SELECT, 3); // close both
	}
}

void CDebugDiag3::VacSelButtonsUpdate() {

	m_VacSideA.SetCheck(theApp.GVars->Valves.bHeadASel);
	m_VacSideB.SetCheck(theApp.GVars->Valves.bHeadBSel);

}

void CDebugDiag3::OnBnClickedVacselA()
{
	VacSelButtons();
}


void CDebugDiag3::OnBnClickedVacselB()
{
	VacSelButtons();
}


void CDebugDiag3::OnBnClickedDamperClampOpen()
{
	bNoPMW[2] = FALSE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 3, 2, 0, 0, 0, 0, 0);
}


void CDebugDiag3::OnBnClickedDamperClampClose()
{
	bNoPMW[2] = FALSE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 3, 0, 0, 0, 0, 0, 0);
}

void CDebugDiag3::OnBnClickedOpencloseall() {
BOOL state = m_AllChannels.GetCheck();	
	
	m_Flush.SetCheck(state);
	m_Cyan.SetCheck(state);
	m_Magenta.SetCheck(state);
	m_Yellow.SetCheck(state);
	m_Black.SetCheck(state);
	m_White.SetCheck(state);
	m_WhiteR.SetCheck(state);

}


void CDebugDiag3::OnBnClickedWhite()
{
	// TODO: Add your control notification handler code here
}



void CDebugDiag3::OnBnClickedWhitePumpLeft() {
unsigned long var;

	var = m_WLOnOff.GetCheck() << 8 | I_WHITE_LEFT_PUMP;

	USBPrinterCommand(USB_CMD_INK_PUMP, var);


}

void CDebugDiag3::OnBnClickedWhitePumpRight()
{
unsigned long var;
	var = m_WROnOff.GetCheck() << 8 | I_WHITE_RIGHT_PUMP;
	USBPrinterCommand(USB_CMD_INK_PUMP, var);
}


void CDebugDiag3::OnBnClickedCyanPump()
{
unsigned long var;
	var = m_COnOff.GetCheck() << 8 | I_CYAN_PUMP;
	USBPrinterCommand(USB_CMD_INK_PUMP, var);
}


void CDebugDiag3::OnBnClickedMagentaPump()
{
unsigned long var;
	var = m_MOnOff.GetCheck() << 8 | I_MAGENTA_PUMP;
	USBPrinterCommand(USB_CMD_INK_PUMP, var);
}


void CDebugDiag3::OnBnClickedYellowPump()
{
unsigned long var;

	var = m_YOnOff.GetCheck() << 8 | I_YELLOW_PUMP;
	USBPrinterCommand(USB_CMD_INK_PUMP, var);
}


void CDebugDiag3::OnBnClickedBlackPump()
{
unsigned long var;

	var = m_KOnOff.GetCheck() << 8 | I_BLACK_PUMP;
	USBPrinterCommand(USB_CMD_INK_PUMP, var);
}


void CDebugDiag3::OnBnClickedFlushPump()
{
unsigned long var;

	var = m_FOnOff.GetCheck() << 8 | I_FLUSH_PUMP;
	USBPrinterCommand(USB_CMD_INK_PUMP, var);


}

void CDebugDiag3::OnBnClickedWhitemixForward()
{

	m_WMixBackward.SetCheck(FALSE);

	USBPrinterCommand(USB_CMD_WHITE_MIX, m_WMixForward.GetCheck());

}


void CDebugDiag3::OnBnClickedWhitemixBackward()
{

	m_WMixForward.SetCheck(FALSE);

	USBPrinterCommand(USB_CMD_WHITE_MIX,m_WMixBackward.GetCheck()?2:0);

}


void CDebugDiag3::OnBnClickedWhitemixOff()
{

	m_WMixBackward.SetCheck(FALSE);
	m_WMixForward.SetCheck(FALSE);

	USBPrinterCommand(USB_CMD_WHITE_MIX, 0);

}


void CDebugDiag3::OnBnClickedRefreshbags()
{

	if(!theApp.GVars->bDisableInkADC) {
		/**/ TRACE("Getting ink levels (5)\n");
		USBPrinterCommand(USB_CMD_GET_INK_STATE, 0xFF);
	}

	USBPrinterCommand(USB_CMD_UPDATE_VALVES, 0x10);


}

void CDebugDiag3::OnBnClickedDamperClampClose2()
{
	bNoPMW[3] = FALSE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 5, 0, 0, 0, 0, 0, 0);
}


void CDebugDiag3::OnBnClickedDamperClampOpen2()
{
	bNoPMW[3] = FALSE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 5, 2, 0, 0, 0, 0, 0);
}


void CDebugDiag3::OnBnClickedRefreshAdc()
{
	if(!theApp.GVars->bDisableInkADC) {
		/**/ TRACE("Getting ink levels (3a)\n");
		USBPrinterCommand(USB_CMD_GET_INK_STATE, 0x40|0x80);
	}
}


void CDebugDiag3::OnBnClickedRefreshCont()
{
	if(theApp.GVars->bDisableInkADC) {
		MessageBox("That function is disabled in the INI file", APP_NAME, MB_OK|MB_SYSTEMMODAL);
		return;
	}

	if(bRunPressureUpdater) {
		bRunPressureUpdater = FALSE;
		Sleep(20);
		m_RefreshOnce.ShowWindow(SW_SHOW);
		m_RefreshCont.SetWindowText("REFRESH CONT.");
	}
	else {

		bRunPressureUpdater = TRUE;

		m_RefreshOnce.ShowWindow(SW_HIDE);
		m_RefreshCont.SetWindowText("** STOP REFRESH **");

		theApp.TempHandlerThread = AfxBeginThread((AFX_THREADPROC)PressureUpdater3,
													0,
													THREAD_PRIORITY_NORMAL,
													0,
													0,
													&theApp.ThreadSA);

	}
}


void CDebugDiag3::OnTimer(UINT_PTR nIDEvent)
{

	UpdateVacDisplay();

	CDialogEx::OnTimer(nIDEvent);
}


void CDebugDiag3::OnBnClickedHeadClampAOn2()
{
	bNoPMW[0] = TRUE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 2, 1, 0, 0, 0, 0, 0);
}


void CDebugDiag3::OnBnClickedHeadClampBOn2()
{
	bNoPMW[1] = TRUE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 4, 1, 0, 0, 0, 0, 0);
}


void CDebugDiag3::OnBnClickedDamperClampClose3()
{
	bNoPMW[2] = TRUE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 3, 1, 0, 0, 0, 0, 0);
}


void CDebugDiag3::OnBnClickedDamperClampClose4() {

	bNoPMW[3] = TRUE;
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 5, 1, 0, 0, 0, 0, 0);

}


void CDebugDiag3::OnBnClickedWhiteDrip()
{
CWhiteCleaning dlg;

	dlg.DoModal();

}

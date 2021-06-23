// DebugDiag2.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "DebugDiag2.h"
#include "Controller4Dlg1.h"
#include "PlatenLiftCtrl.h"
#include "LayerWait.h"
#include "afxdialogex.h"
#include "PortalDBG.h"

#include "parser.h"
#include "HEAD_structs.h"


extern CController4Dlg1 *theDlg;

CDebugDiag2 *thisDlg;

extern void SendPlatenHeightOffset();

extern void StartCleaningDialog(void *);

extern void USBPrinterCommand(unsigned long , unsigned long);
void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;


// CDebugDiag2 dialog

IMPLEMENT_DYNAMIC(CDebugDiag2, CDialogEx)

CDebugDiag2::CDebugDiag2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebugDiag2::IDD, pParent)
{

}

CDebugDiag2::~CDebugDiag2()
{
}

void CDebugDiag2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_RollValue);
	DDX_Control(pDX, IDC_EDIT2, m_PitchValue);
	DDX_Control(pDX, IDC_SUP_C, m_SupC);
	DDX_Control(pDX, IDC_SUP_M, m_SupM);
	DDX_Control(pDX, IDC_SUP_Y2, m_SupY);
	DDX_Control(pDX, IDC_SUP_K2, m_SupK);
	DDX_Control(pDX, IDC_ALIGN_FRONT_LOW, m_AlignFrontLow);
	DDX_Control(pDX, IDC_ALIGN_FRONT_MID, m_AlignFrontMiddle);
	DDX_Control(pDX, IDC_ALIGN_FRONT_HIGH, m_AlignFrontHigh);
	DDX_Control(pDX, IDC_ALIGN_REAR_LOW, m_AlignRearLow);
	DDX_Control(pDX, IDC_ALIGN_REAR_MID, m_AlignRearMiddle);
	DDX_Control(pDX, IDC_ALIGN_REAR_HIGH, m_AlignRearHigh);
	DDX_Control(pDX, IDC_ALIGN_OFF, m_AlignOff);
	DDX_Control(pDX, IDC_INK_ADC_1, m_InkADC_1);
	DDX_Control(pDX, IDC_INK_ADC_2, m_InkADC_2);
	DDX_Control(pDX, IDC_INK_ADC_3, m_InkADC_3);
	DDX_Control(pDX, IDC_INK_ADC_4, m_InkADC_4);
	DDX_Control(pDX, IDC_INK_ADC_5, m_InkADC_5);
	DDX_Control(pDX, IDC_INK_ADC_6, m_InkADC_6);
	DDX_Control(pDX, IDC_CAP_ADC, m_CapADC);
	DDX_Control(pDX, IDC_CAP_PRESS, m_CapPress);
	DDX_Control(pDX, IDC_IGNOREINKOUT, m_IgnoreInkOut);
}


BEGIN_MESSAGE_MAP(CDebugDiag2, CDialogEx)
	ON_BN_CLICKED(IDC_WHITE_VALVE_1_OPEN, &CDebugDiag2::OnBnClickedWhiteValve1Open)
	ON_BN_CLICKED(IDC_WHITE_VALVE_1_CLOSE, &CDebugDiag2::OnBnClickedWhiteValve1Close)
	ON_BN_CLICKED(IDC_WHITE_VALVE_2_OPEN, &CDebugDiag2::OnBnClickedWhiteValve2Open)
	ON_BN_CLICKED(IDC_WHITE_VALVE_2_CLOSE, &CDebugDiag2::OnBnClickedWhiteValve2Close)
	ON_BN_CLICKED(IDC_WHITE_VALVE_3_OPEN, &CDebugDiag2::OnBnClickedWhiteValve3Open)
	ON_BN_CLICKED(IDC_WHITE_VALVE_3_CLOSE, &CDebugDiag2::OnBnClickedWhiteValve3Close)
	ON_BN_CLICKED(IDC_WHITE_VALVE_4_OPEN, &CDebugDiag2::OnBnClickedWhiteValve4Open)
	ON_BN_CLICKED(IDC_WHITE_VALVE_4_CLOSE, &CDebugDiag2::OnBnClickedWhiteValve4Close)
	ON_BN_CLICKED(IDC_VACPUMPON, &CDebugDiag2::OnBnClickedVacpumpon)
	ON_BN_CLICKED(IDC_VACPUMPOFF, &CDebugDiag2::OnBnClickedVacpumpoff)
	ON_BN_CLICKED(IDC_OPENCF, &CDebugDiag2::OnBnClickedOpencf)
	ON_BN_CLICKED(IDC_CLOSECF, &CDebugDiag2::OnBnClickedClosecf)
	ON_BN_CLICKED(IDC_HEATER_ON, &CDebugDiag2::OnBnClickedHeaterOn)
	ON_BN_CLICKED(IDC_HEATER_OFF, &CDebugDiag2::OnBnClickedHeaterOff)
	ON_BN_CLICKED(IDC_TOFRONTMEASURE, &CDebugDiag2::OnBnClickedTofrontmeasure)
	ON_BN_CLICKED(IDC_TOREARMEASURE, &CDebugDiag2::OnBnClickedTorearmeasure)
	ON_BN_CLICKED(IDC_ROLLPLUS1K, &CDebugDiag2::OnBnClickedRollplus1k)
	ON_BN_CLICKED(IDC_ROLLMINUS1K, &CDebugDiag2::OnBnClickedRollminus1k)
	ON_BN_CLICKED(IDC_ROLLPLUS1H, &CDebugDiag2::OnBnClickedRollplus1h)
	ON_BN_CLICKED(IDC_ROLLMINUS1H, &CDebugDiag2::OnBnClickedRollminus1h)
	ON_BN_CLICKED(IDC_PITCHPLUS1K, &CDebugDiag2::OnBnClickedPitchplus1k)
	ON_BN_CLICKED(IDC_PITCHMINUS1K, &CDebugDiag2::OnBnClickedPitchminus1k)
	ON_BN_CLICKED(IDC_BUTTON10, &CDebugDiag2::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_PITCHMINUS1H, &CDebugDiag2::OnBnClickedPitchminus1h)
	ON_BN_CLICKED(IDC_SUP_C, &CDebugDiag2::OnBnClickedSupC)
	ON_BN_CLICKED(IDC_SUP_M, &CDebugDiag2::OnBnClickedSupM)
	ON_BN_CLICKED(IDC_SUP_Y2, &CDebugDiag2::OnBnClickedSupY2)
	ON_BN_CLICKED(IDC_SUP_K2, &CDebugDiag2::OnBnClickedSupK2)
	ON_BN_CLICKED(IDC_CLDLGON, &CDebugDiag2::OnBnClickedCldlgon)
	ON_BN_CLICKED(IDC_WTDLG, &CDebugDiag2::OnBnClickedWtdlg)
	ON_BN_CLICKED(IDC_PORTAL, &CDebugDiag2::OnBnClickedPortal)
	ON_BN_CLICKED(IDC_NEWPLATEN, &CDebugDiag2::OnBnClickedNewplaten)
	ON_BN_CLICKED(IDC_ALIGN_FRONT_LOW, &CDebugDiag2::OnBnClickedAlignFrontLow)
	ON_BN_CLICKED(IDC_ALIGN_FRONT_MID, &CDebugDiag2::OnBnClickedAlignFrontMid)
	ON_BN_CLICKED(IDC_ALIGN_FRONT_HIGH, &CDebugDiag2::OnBnClickedAlignFrontHigh)
	ON_BN_CLICKED(IDC_ALIGN_REAR_LOW, &CDebugDiag2::OnBnClickedAlignRearLow)
	ON_BN_CLICKED(IDC_ALIGN_REAR_MID, &CDebugDiag2::OnBnClickedAlignRearMid)
	ON_BN_CLICKED(IDC_ALIGN_REAR_HIGH, &CDebugDiag2::OnBnClickedAlignRearHigh)
	ON_BN_CLICKED(IDC_ALIGN_OFF, &CDebugDiag2::OnBnClickedAlignOff)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_REFRESHADC, &CDebugDiag2::OnBnClickedRefreshadc)
	ON_BN_CLICKED(IDC_TEST_INK_MARKERS, &CDebugDiag2::OnBnClickedTestInkMarkers)
	ON_BN_CLICKED(IDC_REFRESH_CONT, &CDebugDiag2::OnBnClickedRefreshCont)
	ON_BN_CLICKED(IDC_IGNOREINKOUT, &CDebugDiag2::OnBnClickedIgnoreinkout)
END_MESSAGE_MAP()


// CDebugDiag2 message handlers


void CDebugDiag2::OnBnClickedWhiteValve1Open() 
{
	/**/USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 0, 1, 0, 0, 0, 0, 0);
	//USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 0, 0, 0, 0, 0, 0, 0);
}


void CDebugDiag2::OnBnClickedWhiteValve1Close()
{
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 0, 0, 0, 0, 0, 0, 0);
	//USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 0, 1, 0, 0, 0, 0, 0);
}


void CDebugDiag2::OnBnClickedWhiteValve2Open()
{
	//USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 1, 1, 0, 0, 0, 0, 0);
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 1, 0, 0, 0, 0, 0, 0);

}

void CDebugDiag2::OnBnClickedWhiteValve2Close()
{
	//USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 1, 0, 0, 0, 0, 0, 0);
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 1, 1, 0, 0, 0, 0, 0);
}


void CDebugDiag2::OnBnClickedWhiteValve3Open()
{
	//USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 2, 1, 0, 0, 0, 0, 0);
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 2, 0, 0, 0, 0, 0, 0);
}


void CDebugDiag2::OnBnClickedWhiteValve3Close()
{
	//USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 2, 0, 0, 0, 0, 0, 0);
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 2, 1, 0, 0, 0, 0, 0);
}


void CDebugDiag2::OnBnClickedWhiteValve4Open()
{
	//USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 3, 1, 0, 0, 0, 0, 0);
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 3, 0, 0, 0, 0, 0, 0);
}


void CDebugDiag2::OnBnClickedWhiteValve4Close()
{
	//USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 3, 0, 0, 0, 0, 0, 0);
	USBPrinterCommandEx(USB_CMD_OPEN_INK_VALVE, 3, 1, 0, 0, 0, 0, 0);
}


void CDebugDiag2::OnBnClickedVacpumpon()
{
	USBPrinterCommand(USB_CMD_VAC_PUMP, 1);
}


void CDebugDiag2::OnBnClickedVacpumpoff()
{
	USBPrinterCommand(USB_CMD_VAC_PUMP, 0);
}


void CDebugDiag2::OnBnClickedOpencf() {

	unsigned long param = 0x01;

	param = param << 24;

	param |= (1 << 16);

	USBPrinterCommand(USB_CMD_OPEN_INK, param);

}


void CDebugDiag2::OnBnClickedClosecf()
{

	unsigned long param = 0x01;

	param = param << 24;


	USBPrinterCommand(USB_CMD_OPEN_INK, param);

}


void CDebugDiag2::OnBnClickedHeaterOn()
{
	USBPrinterCommand(USB_CMD_SET_HEADTEMP, theApp.GVars->HeadTemp[0]);
	USBPrinterCommand(USB_CMD_SET_HEADTEMP2, theApp.GVars->HeadTemp[1]);

	USBPrinterCommand(USB_CMD_ENABLE_HEATERS, 1);

}


void CDebugDiag2::OnBnClickedHeaterOff()
{
	USBPrinterCommand(USB_CMD_ENABLE_HEATERS, 0);
}


void CDebugDiag2::OnBnClickedTofrontmeasure()
{

	USBPrinterCommand(USB_CMD_PLATENROLL_COR, theApp.GVars->PlatenRollCorrection);

	USBPrinterCommand(USB_CMD_PLATENPITCH_COR, theApp.GVars->PlatenPitchCorrection);

	USBPrinterCommand(USB_CMD_PLATENTOMEASURE, 0);

}


void CDebugDiag2::OnBnClickedTorearmeasure()
{

	USBPrinterCommand(USB_CMD_PLATENROLL_COR, theApp.GVars->PlatenRollCorrection);

	USBPrinterCommand(USB_CMD_PLATENPITCH_COR, theApp.GVars->PlatenPitchCorrection);

	USBPrinterCommand(USB_CMD_PLATENTOMEASURE, 1);
}


void CDebugDiag2::OnBnClickedRollplus1k()
{
	theApp.GVars->PlatenRollCorrection  += 1000;
	ShowCorrectionValues();
	theApp.WriteINI();
	USBPrinterCommand(USB_CMD_PLATENROLL_COR, theApp.GVars->PlatenRollCorrection);

}


void CDebugDiag2::OnBnClickedRollminus1k()
{
	theApp.GVars->PlatenRollCorrection  -= 1000;
	ShowCorrectionValues();
	theApp.WriteINI();
	USBPrinterCommand(USB_CMD_PLATENROLL_COR, theApp.GVars->PlatenRollCorrection);
}


void CDebugDiag2::OnBnClickedRollplus1h()
{
	theApp.GVars->PlatenRollCorrection  += 100;
	ShowCorrectionValues();
	theApp.WriteINI();
	USBPrinterCommand(USB_CMD_PLATENROLL_COR, theApp.GVars->PlatenRollCorrection);
}


void CDebugDiag2::OnBnClickedRollminus1h()
{
	theApp.GVars->PlatenRollCorrection  -= 100;
	ShowCorrectionValues();
	theApp.WriteINI();
	USBPrinterCommand(USB_CMD_PLATENROLL_COR, theApp.GVars->PlatenRollCorrection);
}


void CDebugDiag2::OnBnClickedPitchplus1k()
{
	theApp.GVars->PlatenPitchCorrection  += 1000;
	ShowCorrectionValues();
	theApp.WriteINI();
	USBPrinterCommand(USB_CMD_PLATENPITCH_COR, theApp.GVars->PlatenPitchCorrection);
}


void CDebugDiag2::OnBnClickedPitchminus1k()
{
	theApp.GVars->PlatenPitchCorrection  -= 1000;
	ShowCorrectionValues();
	theApp.WriteINI();
	USBPrinterCommand(USB_CMD_PLATENPITCH_COR, theApp.GVars->PlatenPitchCorrection);
}


void CDebugDiag2::OnBnClickedButton10()
{
	theApp.GVars->PlatenPitchCorrection  += 100;
	ShowCorrectionValues();
	theApp.WriteINI();
	USBPrinterCommand(USB_CMD_PLATENPITCH_COR, theApp.GVars->PlatenPitchCorrection);
}


void CDebugDiag2::OnBnClickedPitchminus1h()
{
	theApp.GVars->PlatenPitchCorrection  -= 100;
	ShowCorrectionValues();
	theApp.WriteINI();
	USBPrinterCommand(USB_CMD_PLATENPITCH_COR, theApp.GVars->PlatenPitchCorrection);
}

void CDebugDiag2::ShowCorrectionValues() {
	char tmp_buff[128];

	sprintf(tmp_buff, "%d", theApp.GVars->PlatenRollCorrection);
	m_RollValue.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%d", theApp.GVars->PlatenPitchCorrection);
	m_PitchValue.SetWindowText(tmp_buff);
}

void CDebugDiag2::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);

}


BOOL CDebugDiag2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SizeDialog();

	thisDlg = this;

	bRunPressureUpdater = FALSE;
	m_CapPress.SetRange(0, (int)FULL_VAC_PSI);

	ShowCorrectionValues();

	m_SupC.SetCheck(theApp.SupColor[0]);
	m_SupM.SetCheck(theApp.SupColor[1]);
	m_SupY.SetCheck(theApp.SupColor[2]);
	m_SupK.SetCheck(theApp.SupColor[3]);

	bAdjustingLasers = FALSE;

	ShowADCVals();

	m_IgnoreInkOut.SetCheck(theApp.GVars->bInkOutDisableIgnore);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDebugDiag2::OnBnClickedSupC()
{
	if(m_SupC.GetCheck()) theApp.SupColor[0] = TRUE;
	else theApp.SupColor[0] = FALSE;
}


void CDebugDiag2::OnBnClickedSupM()
{
	if(m_SupM.GetCheck()) theApp.SupColor[1] = TRUE;
	else theApp.SupColor[1] = FALSE;
}


void CDebugDiag2::OnBnClickedSupY2()
{
	if(m_SupY.GetCheck()) theApp.SupColor[2] = TRUE;
	else theApp.SupColor[2] = FALSE;
}


void CDebugDiag2::OnBnClickedSupK2()
{
	if(m_SupK.GetCheck()) theApp.SupColor[3] = TRUE;
	else theApp.SupColor[3] = FALSE;
}

static void tmpwrkth(void *p) {

	Sleep(5000);
	theApp.CleaningDialogActive = FALSE;

}
void CDebugDiag2::OnBnClickedCldlgon()
{
	theApp.CleaningDialogThread = AfxBeginThread((AFX_THREADPROC)StartCleaningDialog,
												0,
												THREAD_PRIORITY_NORMAL,
												0,
												0,
												&theApp.ThreadSA);

	AfxBeginThread((AFX_THREADPROC)tmpwrkth,
												0,
												THREAD_PRIORITY_NORMAL,
												0,
												0,
												&theApp.ThreadSA);

}



void CDebugDiag2::OnBnClickedWtdlg()
{
	CLayerWait dlg;
	dlg.DoModal();
}


void CDebugDiag2::OnBnClickedPortal() {
CPortalDBG dlg;

	dlg.DoModal();

}


void CDebugDiag2::OnBnClickedNewplaten()
{

	CPlatenLiftCtrl dlg;

	dlg.DoModal();

}


void CDebugDiag2::OnBnClickedAlignFrontLow()
{

	//m_AlignFrontLow.SetCheck(FALSE);
	m_AlignFrontMiddle.SetCheck(FALSE);
	m_AlignFrontHigh.SetCheck(FALSE);
	m_AlignRearLow.SetCheck(FALSE);
	m_AlignRearMiddle.SetCheck(FALSE);
	m_AlignRearHigh.SetCheck(FALSE);
	m_AlignOff.SetCheck(FALSE);

	if(m_AlignFrontLow.GetCheck()) {

		bAdjustingLasers = TRUE;

		USBPrinterCommand(USB_CMD_ENABLE_LASERS, 1);
		USBPrinterCommand(USB_CMD_LED_STRIP, 16);

	}

	else {
		bAdjustingLasers = FALSE;
		m_AlignOff.SetCheck(TRUE);
	}

}


void CDebugDiag2::OnBnClickedAlignFrontMid()
{
	
	m_AlignFrontLow.SetCheck(FALSE);
	//m_AlignFrontMiddle.SetCheck(FALSE);
	m_AlignFrontHigh.SetCheck(FALSE);
	m_AlignRearLow.SetCheck(FALSE);
	m_AlignRearMiddle.SetCheck(FALSE);
	m_AlignRearHigh.SetCheck(FALSE);
	m_AlignOff.SetCheck(FALSE);

	if(m_AlignFrontMiddle.GetCheck()) {

		bAdjustingLasers = TRUE;

		USBPrinterCommand(USB_CMD_ENABLE_LASERS, 1);
		USBPrinterCommand(USB_CMD_LED_STRIP, 15);

	}

	else {
		bAdjustingLasers = FALSE;
		m_AlignOff.SetCheck(TRUE);
	}
	

	
}


void CDebugDiag2::OnBnClickedAlignFrontHigh()
{


	m_AlignFrontLow.SetCheck(FALSE);
	m_AlignFrontMiddle.SetCheck(FALSE);
	//m_AlignFrontHigh.SetCheck(FALSE);
	m_AlignRearLow.SetCheck(FALSE);
	m_AlignRearMiddle.SetCheck(FALSE);
	m_AlignRearHigh.SetCheck(FALSE);
	m_AlignOff.SetCheck(FALSE);

	if(m_AlignFrontHigh.GetCheck()) {

		bAdjustingLasers = TRUE;

		USBPrinterCommand(USB_CMD_ENABLE_LASERS, 1);
		USBPrinterCommand(USB_CMD_LED_STRIP, 14);

	}

	else {
		bAdjustingLasers = FALSE;
		m_AlignOff.SetCheck(TRUE);
	}
	


}


void CDebugDiag2::OnBnClickedAlignRearLow()
{

	m_AlignFrontLow.SetCheck(FALSE);
	m_AlignFrontMiddle.SetCheck(FALSE);
	m_AlignFrontHigh.SetCheck(FALSE);
	//m_AlignRearLow.SetCheck(FALSE);
	m_AlignRearMiddle.SetCheck(FALSE);
	m_AlignRearHigh.SetCheck(FALSE);
	m_AlignOff.SetCheck(FALSE);

	if(m_AlignRearLow.GetCheck()) {

		bAdjustingLasers = TRUE;

		USBPrinterCommand(USB_CMD_ENABLE_LASERS, 1);
		USBPrinterCommand(USB_CMD_LED_STRIP, 13);

	}

	else {
		bAdjustingLasers = FALSE;
		m_AlignOff.SetCheck(TRUE);
	}
	

}


void CDebugDiag2::OnBnClickedAlignRearMid()
{

	m_AlignFrontLow.SetCheck(FALSE);
	m_AlignFrontMiddle.SetCheck(FALSE);
	m_AlignFrontHigh.SetCheck(FALSE);
	m_AlignRearLow.SetCheck(FALSE);
	//m_AlignRearMiddle.SetCheck(FALSE);
	m_AlignRearHigh.SetCheck(FALSE);
	m_AlignOff.SetCheck(FALSE);

	if(m_AlignRearMiddle.GetCheck()) {

		bAdjustingLasers = TRUE;

		USBPrinterCommand(USB_CMD_ENABLE_LASERS, 1);
		USBPrinterCommand(USB_CMD_LED_STRIP, 12);

	}

	else {
		bAdjustingLasers = FALSE;
		m_AlignOff.SetCheck(TRUE);
	}


}


void CDebugDiag2::OnBnClickedAlignRearHigh()
{

	m_AlignFrontLow.SetCheck(FALSE);
	m_AlignFrontMiddle.SetCheck(FALSE);
	m_AlignFrontHigh.SetCheck(FALSE);
	m_AlignRearLow.SetCheck(FALSE);
	m_AlignRearMiddle.SetCheck(FALSE);
	//m_AlignRearHigh.SetCheck(FALSE);
	m_AlignOff.SetCheck(FALSE);

	if(m_AlignRearHigh.GetCheck()) {

		bAdjustingLasers = TRUE;

		USBPrinterCommand(USB_CMD_ENABLE_LASERS, 1);
		USBPrinterCommand(USB_CMD_LED_STRIP, 11);

	}

	else {
		bAdjustingLasers = FALSE;
		m_AlignOff.SetCheck(TRUE);
	}


}


void CDebugDiag2::OnBnClickedAlignOff()
{


	m_AlignFrontLow.SetCheck(FALSE);
	m_AlignFrontMiddle.SetCheck(FALSE);
	m_AlignFrontHigh.SetCheck(FALSE);
	m_AlignRearLow.SetCheck(FALSE);
	m_AlignRearMiddle.SetCheck(FALSE);
	m_AlignRearHigh.SetCheck(FALSE);
	m_AlignOff.SetCheck(TRUE);

	bAdjustingLasers = FALSE;
	USBPrinterCommand(USB_CMD_ENABLE_LASERS, 0);
	USBPrinterCommand(USB_CMD_LED_STRIP, 0);


}


void CDebugDiag2::OnDestroy()
{

	bRunPressureUpdater = FALSE;

	Sleep(50); // give the pressure udpater thread time to close


	if(bAdjustingLasers) {
		USBPrinterCommand(USB_CMD_ENABLE_LASERS, 0);
		USBPrinterCommand(USB_CMD_LED_STRIP, 0);
	}


	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}

void CDebugDiag2::ShowADCVals() {
char tmp_buff[64];
float cappress;


	sprintf(tmp_buff, "%d", theApp.GVars->inkADC[0]);
	m_InkADC_1.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%d", theApp.GVars->inkADC[1]);
	m_InkADC_2.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%d", theApp.GVars->inkADC[2]);
	m_InkADC_3.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%d", theApp.GVars->inkADC[3]);
	m_InkADC_4.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%d", theApp.GVars->inkADC[4]);
	m_InkADC_5.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%d", theApp.GVars->inkADC[5]);
	m_InkADC_6.SetWindowText(tmp_buff);

	cappress = theApp.CalculateCapPressure(theApp.GVars->CapADC);
	
	sprintf(tmp_buff, "%d (%2.03f psi)", theApp.GVars->CapADC, cappress); /// CALCULATE PSI HERE
	m_CapADC.SetWindowText(tmp_buff);

	m_CapPress.SetPos((int)cappress);

}

void CDebugDiag2::OnBnClickedRefreshadc()
{
	int timeout = 0;

	if(theApp.GVars->bDisableInkADC) {
		MessageBox("That function is disabled in the INI file", APP_NAME, MB_OK|MB_SYSTEMMODAL);
		return;
	}

	memset((char *)theApp.GVars->inkADC, 0, sizeof(int) * 6);
	
	ShowADCVals();

	Sleep(0);

	if(!theApp.GVars->bDisableInkADC) {
		/**/ TRACE("Getting ink levels (3)\n");
		USBPrinterCommand(USB_CMD_GET_INK_STATE, 0xFF);
	}

	while(timeout < 3000 && theApp.GVars->inkADC[0] == 0 && theApp.GVars->inkADC[5] == 0) {
		Sleep(1);
		timeout++;
	}

	Sleep(500);

	ShowADCVals();
}


void CDebugDiag2::OnBnClickedTestInkMarkers() {
int i;

	for(i = 0; i < 5; i++) {
		theApp.GVars->inkADC[i] = 180;
	}
	theApp.GVars->inkADC[2] = 5;

	for(i = 0; i < 5; i++) {
		theDlg->RedrawInkMarkers(i);
	}


}


#define REFRESH_MS	400

void PressureUpdater(void *p) {
int i;

	while(thisDlg->bRunPressureUpdater) {

		/**/ TRACE("Getting ink levels (4)\n");
		USBPrinterCommand(USB_CMD_GET_INK_STATE, 0xFF);

		for(i = 0; i < REFRESH_MS; i++) {
			if(!thisDlg->bRunPressureUpdater) return;
			Sleep(1);
		}

		thisDlg->ShowADCVals();
		

	}


}

void CDebugDiag2::OnBnClickedRefreshCont() {

	if(theApp.GVars->bDisableInkADC) {
		MessageBox("That function is disabled in the INI file", APP_NAME, MB_OK|MB_SYSTEMMODAL);
		return;
	}

	if(bRunPressureUpdater) {
		bRunPressureUpdater = FALSE;
		Sleep(20);
	}
	else {

		bRunPressureUpdater = TRUE;

		theApp.TempHandlerThread = AfxBeginThread((AFX_THREADPROC)PressureUpdater,
													0,
													THREAD_PRIORITY_NORMAL,
													0,
													0,
													&theApp.ThreadSA);

	}


}

void CDebugDiag2::OnBnClickedIgnoreinkout() {

	theApp.GVars->bInkOutDisableIgnore = m_IgnoreInkOut.GetCheck();

	theApp.WriteINI();

}

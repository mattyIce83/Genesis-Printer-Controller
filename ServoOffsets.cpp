// ServoOffsets.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "ServoOffsets.h"
#include "afxdialogex.h"
#include "parser.h"
//#define DEFINE_RASTER_OFFSET_ARRAYS
#include "HEAD_structs.h"
#include "Controller4Dlg1.h"

extern CController4Dlg1 *theDlg;

extern CController4App theApp;
// CServoOffsets dialog

extern void USBPrinterCommandEx(unsigned long command, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
extern void USBPrinterCommand(unsigned long , unsigned long);
extern void SendPlatenLevelingOffset();

IMPLEMENT_DYNAMIC(CServoOffsets, CDialogEx)

CServoOffsets::CServoOffsets(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServoOffsets::IDD, pParent)
{

	bMoved = FALSE;


}

CServoOffsets::~CServoOffsets()
{
}

void CServoOffsets::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FL_VAL, m_FLVal);
	DDX_Control(pDX, IDC_FR_VAL, m_FRVal);
	DDX_Control(pDX, IDC_RL_VAL, m_RLVal);
	DDX_Control(pDX, IDC_RR_VAL, m_RRVal);
	DDX_Control(pDX, IDC_FL_10UD, m_FL10UD);
	DDX_Control(pDX, IDC_FL_1UD, m_FL1UD);
	DDX_Control(pDX, IDC_FR_10UD, m_FR10UD);
	DDX_Control(pDX, IDC_FR_1UD, m_FR1UD);
	DDX_Control(pDX, IDC_RL_10UD, m_RL10UD);
	DDX_Control(pDX, IDC_RL_1UD, m_RL1UD);
	DDX_Control(pDX, IDC_RR_10UD, m_RR10UD);
	DDX_Control(pDX, IDC_RR_1UD, m_RR1UD);
	DDX_Control(pDX, IDC_CHECK1, m_HomeOnExit);
}


BEGIN_MESSAGE_MAP(CServoOffsets, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FL_10UD, &CServoOffsets::OnDeltaposFl10ud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FL_1UD, &CServoOffsets::OnDeltaposFl1ud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FR_10UD, &CServoOffsets::OnDeltaposFr10ud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FR_1UD, &CServoOffsets::OnDeltaposFr1ud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RL_10UD, &CServoOffsets::OnDeltaposRl10ud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RL_1UD, &CServoOffsets::OnDeltaposRl1ud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RR_10UD, &CServoOffsets::OnDeltaposRr10ud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RR_1UD, &CServoOffsets::OnDeltaposRr1ud)
	ON_BN_CLICKED(IDC_TEST, &CServoOffsets::OnBnClickedTest)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SEND, &CServoOffsets::OnBnClickedSend)
	ON_BN_CLICKED(IDC_PLATEN_TO_MEASUREFRONT, &CServoOffsets::OnBnClickedPlatenToMeasurefront)
	ON_BN_CLICKED(IDC_PLATEN_TO_MEASUREFRONT2, &CServoOffsets::OnBnClickedPlatenToMeasurefront2)
	ON_BN_CLICKED(IDC_HOMEZ, &CServoOffsets::OnBnClickedHomez)
	ON_BN_CLICKED(IDC_RELEASE_Y, &CServoOffsets::OnBnClickedReleaseY)
	ON_BN_CLICKED(IDOK, &CServoOffsets::OnBnClickedOk)
END_MESSAGE_MAP()


// CServoOffsets message handlers

void CServoOffsets::DisplayOffset(int index) {

	char tmp_buff[120];

	sprintf(tmp_buff, "%d", theApp.GVars->LiftServoCorr[index]);

	m_ValWindow[index]->SetWindowText(tmp_buff);

}

void CServoOffsets::OnDeltaposFl10ud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) theApp.GVars->LiftServoCorr[0] += 100;
	else theApp.GVars->LiftServoCorr[0] -= 100;

	if(theApp.GVars->LiftServoCorr[0] < 0)
		theApp.GVars->LiftServoCorr[0] = 0;


	DisplayOffset(0);

	*pResult = 0;
}


void CServoOffsets::OnDeltaposFl1ud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0) theApp.GVars->LiftServoCorr[0] += 10;
	else theApp.GVars->LiftServoCorr[0] -= 10;

	if(theApp.GVars->LiftServoCorr[0] < 0)
		theApp.GVars->LiftServoCorr[0] = 0;

	DisplayOffset(0);

	*pResult = 0;
}


void CServoOffsets::OnDeltaposFr10ud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0) theApp.GVars->LiftServoCorr[3] += 100;
	else theApp.GVars->LiftServoCorr[3] -= 100;

	if(theApp.GVars->LiftServoCorr[3] < 0)
		theApp.GVars->LiftServoCorr[3] = 0;

	DisplayOffset(3);

	*pResult = 0;
}


void CServoOffsets::OnDeltaposFr1ud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0) theApp.GVars->LiftServoCorr[3] += 10;
	else theApp.GVars->LiftServoCorr[3] -= 10;

	if(theApp.GVars->LiftServoCorr[3] < 0)
		theApp.GVars->LiftServoCorr[3] = 0;

	DisplayOffset(3);

	*pResult = 0;
}


void CServoOffsets::OnDeltaposRl10ud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0) theApp.GVars->LiftServoCorr[1] += 100;
	else theApp.GVars->LiftServoCorr[1] -= 100;

	if(theApp.GVars->LiftServoCorr[1] < 0)
		theApp.GVars->LiftServoCorr[1] = 0;


	DisplayOffset(1);

	*pResult = 0;
}


void CServoOffsets::OnDeltaposRl1ud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0) theApp.GVars->LiftServoCorr[1] += 10;
	else theApp.GVars->LiftServoCorr[1] -= 10;

	if(theApp.GVars->LiftServoCorr[1] < 0)
		theApp.GVars->LiftServoCorr[1] = 0;


	DisplayOffset(1);

	*pResult = 0;
}


void CServoOffsets::OnDeltaposRr10ud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0) theApp.GVars->LiftServoCorr[2] += 100;
	else theApp.GVars->LiftServoCorr[2] -= 100;

	if(theApp.GVars->LiftServoCorr[2] < 0)
		theApp.GVars->LiftServoCorr[2] = 0;


	DisplayOffset(2);

	*pResult = 0;
}


void CServoOffsets::OnDeltaposRr1ud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0) theApp.GVars->LiftServoCorr[2] += 10;
	else theApp.GVars->LiftServoCorr[2] -= 10;

	if(theApp.GVars->LiftServoCorr[2] < 0)
		theApp.GVars->LiftServoCorr[2] = 0;

	DisplayOffset(2);

	*pResult = 0;
}


BOOL CServoOffsets::OnInitDialog()
{
int i;

	CDialogEx::OnInitDialog();

	m_ValWindow[0] = &m_FLVal;
	m_ValWindow[1] = &m_RLVal;
	m_ValWindow[2] = &m_RRVal;
	m_ValWindow[3] = &m_FRVal;


	WINDOWPLACEMENT  wp1, wp2, wp3, wp4, wp5, wp6, wp7, wp8;

	m_FL10UD.GetWindowPlacement(&wp1);
	m_FL1UD.GetWindowPlacement(&wp2);
	m_FR10UD.GetWindowPlacement(&wp3);
	m_FR1UD.GetWindowPlacement(&wp4);
	m_RL10UD.GetWindowPlacement(&wp5);
	m_RL1UD.GetWindowPlacement(&wp6);
	m_RR10UD.GetWindowPlacement(&wp7);
	m_RR1UD.GetWindowPlacement(&wp8);

	m_FL10UD.GetWindowPlacement(&wp1);
	m_FL1UD.GetWindowPlacement(&wp2);
	m_FR10UD.GetWindowPlacement(&wp3);
	m_FR1UD.GetWindowPlacement(&wp4);
	m_RL10UD.GetWindowPlacement(&wp5);
	m_RL1UD.GetWindowPlacement(&wp6);
	m_RR10UD.GetWindowPlacement(&wp7);
	m_RR1UD.GetWindowPlacement(&wp8);

	wp1.rcNormalPosition.right = wp1.rcNormalPosition.left + 90;
	wp2.rcNormalPosition.right = wp2.rcNormalPosition.left + 90;
	wp3.rcNormalPosition.right = wp3.rcNormalPosition.left + 90;
	wp4.rcNormalPosition.right = wp4.rcNormalPosition.left + 90;
	wp5.rcNormalPosition.right = wp5.rcNormalPosition.left + 90;
	wp6.rcNormalPosition.right = wp6.rcNormalPosition.left + 90;
	wp7.rcNormalPosition.right = wp7.rcNormalPosition.left + 90;
	wp8.rcNormalPosition.right = wp8.rcNormalPosition.left + 90;

	m_FL10UD.SetWindowPlacement(&wp1);
	m_FL1UD.SetWindowPlacement(&wp2);
	m_FR10UD.SetWindowPlacement(&wp3);
	m_FR1UD.SetWindowPlacement(&wp4);
	m_RL10UD.SetWindowPlacement(&wp5);
	m_RL1UD.SetWindowPlacement(&wp6);
	m_RR10UD.SetWindowPlacement(&wp7);
	m_RR1UD.SetWindowPlacement(&wp8);

	for(i = 0; i < 4; i++) {
		DisplayOffset(i);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CServoOffsets::OnBnClickedTest() {

	int overshoot;

	if(!theApp.CheckMaxStepperThrow(&overshoot)) {

		char message1[128];

		sprintf(message1, "%d units too high", overshoot);

		theDlg->OKBox("** Invalid Height **","Requested height is", message1);

		return;

	}

	SendPlatenLevelingOffset();


	USBPrinterCommand(USB_CMD_HOME_PLATEN_Z, 1);

	theApp.WriteINI();

}


void CServoOffsets::OnDestroy()
{

	if(bMoved) {
		USBPrinterCommand(USB_CMD_LED_STRIP, 0);
		USBPrinterCommand(USB_CMD_RELEASE_Y, 0);
		if(m_HomeOnExit.GetCheck()) {
			USBPrinterCommand(USB_CMD_HOME_PLATEN, 0);
		}
	}

	CDialogEx::OnDestroy();

}


void CServoOffsets::OnBnClickedSend()
{

	SendPlatenLevelingOffset();

}


void CServoOffsets::OnBnClickedPlatenToMeasurefront() {

	bMoved = TRUE;

	USBPrinterCommand(USB_CMD_PLATENTOMEASURE, 0);
}


void CServoOffsets::OnBnClickedPlatenToMeasurefront2()
{

	bMoved = TRUE;

	USBPrinterCommand(USB_CMD_PLATENTOMEASURE, 1);
}




void CServoOffsets::OnBnClickedHomez()
{
	SendPlatenLevelingOffset();
	
	USBPrinterCommand(USB_CMD_HOME_PLATEN_Z, 1);
}


void CServoOffsets::OnBnClickedReleaseY() {

	bMoved = TRUE;
	USBPrinterCommand(USB_CMD_RELEASE_Y, 1);
	USBPrinterCommand(USB_CMD_LED_STRIP, 1);
}


void CServoOffsets::OnBnClickedOk()
{

	int overshoot;

	if(!theApp.CheckMaxStepperThrow(&overshoot)) {

		char message1[128];

		sprintf(message1, "%d units too high", overshoot);

		theDlg->OKBox("** Invalid Height **","Requested height is", message1);

		return;

	}


	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

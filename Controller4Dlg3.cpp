// Controller4Dlg3.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg3.h"
#include "Controller4Dlg4.h"
#include "afxdialogex.h"
#include "parser.h"
#include "DebugDiag.h"
#include "SoftwareDL.h"
#include "DemoMode.h"
#include "DeleteVerify.h"
#include "ServoOffsets.h"
#include "ManualPlatenHeights.h"

#include "HEAD_structs.h"


extern CController4App theApp;
extern void CheckUSBIntegrity();
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;

extern void SendPlatenLevelingOffset();

extern CController4Dlg1 *theDlg;


extern void USBPrinterCommand(unsigned long , unsigned long);
extern BOOL UnlockAdvanced();
extern BOOL CheckAdvanced();


// CController4Dlg3 dialog

IMPLEMENT_DYNAMIC(CController4Dlg3, CDialogEx)

CController4Dlg3::CController4Dlg3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CController4Dlg3::IDD, pParent)
{

}

CController4Dlg3::~CController4Dlg3()
{
}

void CController4Dlg3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOME, m_Home);
	DDX_Control(pDX, IDC_SPITONOFF_AA, m_SpitOnOffAA);
	DDX_Control(pDX, IDC_SPITONOFF_AB, m_SpitOnOffAB);
	DDX_Control(pDX, IDC_SPITONOFF_AC, m_SpitOnOffAC);
	DDX_Control(pDX, IDC_SPITONOFF_AD, m_SpitOnOffAD);
	DDX_Control(pDX, IDC_SPITONOFF_BA, m_SpitOnOffBA);
	DDX_Control(pDX, IDC_SPITONOFF_BB, m_SpitOnOffBB);
	DDX_Control(pDX, IDC_SPITONOFF_BC, m_SpitOnOffBC);
	DDX_Control(pDX, IDC_SPITONOFF_BD, m_SpitOnOffBD);
	DDX_Control(pDX, IDC_DIAG_2, m_AdvancedDiag);
	DDX_Control(pDX, IDC_LOAD_SOFTWARE, m_LoadSoftware);
	//DDX_Control(pDX, IDC_PLT_OFFSET, m_PTOffset);
	DDX_Control(pDX, IDC_DEMO_MODE, m_DemoMode);
	DDX_Control(pDX, IDC_DEFAULTHEIGHT, m_DefaultHeight);
	DDX_Control(pDX, IDC_CHECKFEEDHEIGHT, m_CheckFeed);
	DDX_Control(pDX, IDC_HOMEZ, m_RehomeZ);
	DDX_Control(pDX, IDC_IGNORE_OBSTRUCTION, m_IgnoreObstruction);
	DDX_Control(pDX, IDC_LEVEL_PLATEN, m_AutoHeight);
	DDX_Control(pDX, IDC_LEVEL_PLATEN3, m_AutoLevel);
	DDX_Control(pDX, IDC_HEAD_MAINT_POS, m_HeadToMaint);
	DDX_Control(pDX, IDC_HOME_HEAD, m_HomeHead);
	DDX_Control(pDX, IDC_PLATEN_GAP_TEXT, m_PlatenGapText);
	DDX_Control(pDX, IDC_MANUAL_HEIGHT, m_ManualHeight);
	DDX_Control(pDX, IDC_LEVEL_PLATEN4, m_ManualLevel);
	DDX_Control(pDX, IDC_PLATEN_HEIGHTS_GROUP, m_PlatenHeightsGroup);
	DDX_Control(pDX, IDC_FEEDHEIGHT, m_FeedHeight);
	DDX_Control(pDX, IDC_PRINT_HEIGHT_TEXT, m_PrintHeightText);
	DDX_Control(pDX, IDC_FEED_HEIGHT_TEXT, m_FeedHeightText);
	DDX_Control(pDX, IDC_CHECKPRINTHEIGHT, m_CheckPrint);
	DDX_Control(pDX, IDC_PLATEN_LEVELING_GROUP, m_PlatenLevelingGroup);
	DDX_Control(pDX, IDC_NOZZLE_FIRE_GROUP, m_OnDemandNozzleFireGroup);
	DDX_Control(pDX, IDC_ADVANCED_GROUP, m_AdvancedGroup);
	DDX_Control(pDX, IDC_REMOTE_ACTION, m_RemoteMaint);
	DDX_Control(pDX, IDC_POWER, m_ShutDown);
	DDX_Control(pDX, IDC_PLATEN_GAP_PLUS, m_PlatenGapUp);
	DDX_Control(pDX, IDC_PLATEN_GAP_MINUS, m_PlatenGapDown);
	DDX_Control(pDX, IDC_PLATENGAP, m_PlatenGapList);
	DDX_Control(pDX, IDC_PARKPLATEN, m_ParkPlaten);
}


BEGIN_MESSAGE_MAP(CController4Dlg3, CDialogEx)
	ON_BN_CLICKED(IDC_HOME, &CController4Dlg3::OnBnClickedHome)
	ON_BN_CLICKED(IDC_RIGHT, &CController4Dlg3::OnBnClickedRight)
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_WHITE1, &CController4Dlg3::OnBnClickedWhite1)
	ON_BN_CLICKED(IDC_TEST_USB, &CController4Dlg3::OnBnClickedTestUsb)
	ON_BN_CLICKED(IDC_LEVEL_ONE, &CController4Dlg3::OnBnClickedLevelOne)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SPITONOFF_AA, &CController4Dlg3::OnBnClickedSpitonoffAa)
	ON_BN_CLICKED(IDC_SPITONOFF_AB, &CController4Dlg3::OnBnClickedSpitonoffAb)
	ON_BN_CLICKED(IDC_SPITONOFF_AC, &CController4Dlg3::OnBnClickedSpitonoffAc)
	ON_BN_CLICKED(IDC_SPITONOFF_AD, &CController4Dlg3::OnBnClickedSpitonoffAd)
	ON_BN_CLICKED(IDC_SPITONOFF_BA, &CController4Dlg3::OnBnClickedSpitonoffBa)
	ON_BN_CLICKED(IDC_SPITONOFF_BB, &CController4Dlg3::OnBnClickedSpitonoffBb)
	ON_BN_CLICKED(IDC_SPITONOFF_BC, &CController4Dlg3::OnBnClickedSpitonoffBc)
	ON_BN_CLICKED(IDC_SPITONOFF_BD, &CController4Dlg3::OnBnClickedSpitonoffBd)
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDC_DIAG_2, &CController4Dlg3::OnBnClickedDiag2)
	ON_BN_CLICKED(IDC_ALL_HEAD_A, &CController4Dlg3::OnBnClickedAllHeadA)
	ON_BN_CLICKED(IDC_ALL_HEAD_B, &CController4Dlg3::OnBnClickedAllHeadB)
	ON_BN_CLICKED(IDC_LEVEL_PLATEN, &CController4Dlg3::OnBnClickedLevelPlaten)
	ON_BN_CLICKED(IDC_HEAD_MAINT_POS, &CController4Dlg3::OnBnClickedHeadMaintPos)
	ON_BN_CLICKED(IDC_HOME_HEAD, &CController4Dlg3::OnBnClickedHomeHead)
	ON_BN_CLICKED(IDC_LOAD_SOFTWARE, &CController4Dlg3::OnBnClickedLoadSoftware)
//	ON_CBN_SELCHANGE(IDC_PLT_OFFSET, &CController4Dlg3::OnCbnSelchangePtOffset)
	ON_BN_CLICKED(IDC_POWER, &CController4Dlg3::OnBnClickedPower)
	ON_BN_CLICKED(IDC_DEMO_MODE, &CController4Dlg3::OnBnClickedDemoMode)
	ON_EN_CHANGE(IDC_DEFAULTHEIGHT, &CController4Dlg3::OnEnChangeDefaultheight)
	ON_BN_CLICKED(IDC_HOMEZ, &CController4Dlg3::OnBnClickedHomez)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECKFEEDHEIGHT, &CController4Dlg3::OnBnClickedCheckfeedheight)
	ON_BN_CLICKED(IDC_IGNORE_OBSTRUCTION, &CController4Dlg3::OnBnClickedIgnoreObstruction)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_LEVEL_PLATEN4, &CController4Dlg3::OnBnClickedLevelPlaten4)
	ON_BN_CLICKED(IDC_MANUAL_HEIGHT, &CController4Dlg3::OnBnClickedManualHeight)
	ON_BN_CLICKED(IDC_CHECKPRINTHEIGHT, &CController4Dlg3::OnBnClickedCheckprintheight)
	ON_BN_CLICKED(IDC_LEVEL_PLATEN3, &CController4Dlg3::OnBnClickedLevelPlaten3)
	ON_BN_CLICKED(IDC_REMOTE_ACTION, &CController4Dlg3::OnBnClickedRemoteAction)
	ON_LBN_SELCHANGE(IDC_PLATENGAP, &CController4Dlg3::OnLbnSelchangePlatengap)
	ON_BN_CLICKED(IDC_PLATEN_GAP_PLUS, &CController4Dlg3::OnBnClickedPlatenGapPlus)
	ON_BN_CLICKED(IDC_PLATEN_GAP_MINUS, &CController4Dlg3::OnBnClickedPlatenGapMinus)
	ON_BN_CLICKED(IDC_PARKPLATEN, &CController4Dlg3::OnBnClickedParkplaten)
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP(CMyComboBox2, CComboBox)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


// CController4Dlg3 message handlers

void CController4Dlg3::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);

	
}

BOOL CController4Dlg3::OnInitDialog()
{
int result;


	theApp.currentWindow = TOOLS_WINDOW;


	result = m_Home.AutoLoad(IDC_HOME, this);

	

	m_SpitOnOffAA.LoadBitmaps(IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER);
	m_SpitOnOffAB.LoadBitmaps(IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER);
	m_SpitOnOffAC.LoadBitmaps(IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER);
	m_SpitOnOffAD.LoadBitmaps(IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER);
	m_SpitOnOffBA.LoadBitmaps(IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER);
	m_SpitOnOffBB.LoadBitmaps(IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER);
	m_SpitOnOffBC.LoadBitmaps(IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER);
	m_SpitOnOffBD.LoadBitmaps(IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER);

	InkSpitState[0] = InkSpitState[1] = InkSpitState[2] = InkSpitState[3] = InkSpitState[4] = InkSpitState[5] = InkSpitState[6] = InkSpitState[7] = 0;

	m_PlatenGapUp.AutoLoad(IDC_PLATEN_GAP_PLUS, this);
	m_PlatenGapDown.AutoLoad(IDC_PLATEN_GAP_MINUS, this);


	buttons[0] = &m_SpitOnOffAA;
	buttons[1] = &m_SpitOnOffAB;
	buttons[2] = &m_SpitOnOffAC;
	buttons[3] = &m_SpitOnOffAD;
	buttons[4] = &m_SpitOnOffBA;
	buttons[5] = &m_SpitOnOffBB;
	buttons[6] = &m_SpitOnOffBC;
	buttons[7] = &m_SpitOnOffBD;

	old_active = bInTestMode = FALSE;

	CDialogEx::OnInitDialog();

	m_PlatenGapList.AddString("Normal");
	m_PlatenGapList.AddString("Narrow");
	m_PlatenGapList.AddString("Wide");



	m_ManualHeight.SetFont(&theDlg->pSmallFont);
	m_ManualLevel.SetFont(&theDlg->pSmallFont);

	m_CheckFeed.SetFont(&theDlg->pSmallFont);
	m_CheckPrint.SetFont(&theDlg->pSmallFont);

	m_RehomeZ.SetFont(&theDlg->pSmallFont);
	m_AutoHeight.SetFont(&theDlg->pSmallFont);
	m_AutoLevel.SetFont(&theDlg->pSmallFont);
	m_HeadToMaint.SetFont(&theDlg->pSmallFont);
	m_HomeHead.SetFont(&theDlg->pSmallFont);


	m_FeedHeightText.SetFont(&theDlg->pSmallFont);
	m_PrintHeightText.SetFont(&theDlg->pSmallFont);

	m_AdvancedDiag.SetFont(&theDlg->pSmallFont);
	m_LoadSoftware.SetFont(&theDlg->pSmallFont);
	m_RemoteMaint.SetFont(&theDlg->pSmallFont);
	m_DemoMode.SetFont(&theDlg->pSmallFont);
	m_ShutDown.SetFont(&theDlg->pSmallFont);

	//m_PTOffset.SetFont(&theDlg->pNewFont);
	//m_PTOffset.SetBkColor(RGB(0,0,0));

	m_PlatenGapList.SetFont(&theDlg->pNewFont);

	m_DefaultHeight.SetFont(&theDlg->pNewFont);

	m_FeedHeight.SetFont(&theDlg->pNewFont);

	m_IgnoreObstruction.SetFont(&theDlg->pSmallFont);
	
	m_ParkPlaten.SetFont(&theDlg->pSmallFont);
	
	//m_PlatenGapText.SetFont(&theDlg->pMediumFont);

	m_PlatenHeightsGroup.SetFont(&theDlg->pMediumFont);

	m_OnDemandNozzleFireGroup.SetFont(&theDlg->pMediumFont);

	m_PlatenLevelingGroup.SetFont(&theDlg->pMediumFont);

	m_AdvancedGroup.SetFont(&theDlg->pMediumFont);

	SizeDialog();

	// theApp.ReadINI();

	//m_PTOffset.SetCurSel(theApp.GVars->CurrentHeightOffset);
	m_PlatenGapList.SetCurSel(theApp.GVars->CurrentHeightOffset);

	char tmp_buff[64];
	sprintf(tmp_buff, "%d", theApp.GVars->iDefaultPlatenHeight);
	m_DefaultHeight.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%d", theApp.GVars->iPlatenFeedHeight);
	m_FeedHeight.SetWindowText(tmp_buff);

	m_IgnoreObstruction.SetCheck(theApp.GVars->bIgnoreObstruction);

	SetObstructionLaserButtonText();

	if(theApp.GVars->iPrinterType == 2)
		m_RehomeZ.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control

	// EXCEPTION: OCX Property Pages should return FALSE
}

void CController4Dlg3::SetObstructionLaserButtonText() {

	if(!theApp.GVars->bIgnoreObstruction) {
		m_IgnoreObstruction.SetWindowText("USING\nOBSTRUCTION\nLASERS");
	}
	else {
		m_IgnoreObstruction.SetWindowText("IGNORING\nOBSTRUCTION\nLASERS");
	}



}

void CController4Dlg3::OnBnClickedHome() {

	theApp.bHomePressed = TRUE;
	CDialogEx::OnCancel();

}

void CController4Dlg3::AllSpitOff() {
int i;

	for(i = 0; i < 0; i++) {
		InkSpitState[i] = 0;
	}

	bInTestMode = old_active = FALSE;

	ReconcileIcons();

}

void CController4Dlg3::OnBnClickedRight() {
CController4Dlg4 dlg;

	if(bInTestMode) {
		/// turn off test mode
		USBPrinterCommand(USB_CMD_PRINTER_TEST_MODE, 0);
		AllSpitOff();
	}

	dlg.DoModal();


}


void CController4Dlg3::OnSetFocus(CWnd* pOldWnd)
{

	if(theApp.bHomePressed) {

		CDialogEx::OnCancel();

	}

	CDialogEx::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}


void CController4Dlg3::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
}


void CController4Dlg3::OnDestroy()
{

	if(bInTestMode) {
		/// turn off test mode
		USBPrinterCommand(USB_CMD_PRINTER_TEST_MODE, 0);
	}


	old_active = bInTestMode = FALSE;

	CDialogEx::OnDestroy();

	pNewFont.DeleteObject();

}


void CController4Dlg3::OnBnClickedWhite1()
{
	// TODO: Add your control notification handler code here
}




void CController4Dlg3::OnBnClickedTestUsb()
{
	CheckUSBIntegrity();

}


void CController4Dlg3::OnBnClickedLevelOne() {
	
	USBPrinterCommand(USB_CMD_CLEAN, 1);



}


void CController4Dlg3::OnPaint()
{
		CPaintDC dc(this); // device context for painting
	
		CRect rect;
		GetClientRect(&rect);

		// FILL THE BACKGROUND
		//dc.FillRect(&rect, &theApp.hBGbrush);

		CDialog::OnPaint();
		if(theApp.GVars->bDualCMYK) {
			theApp.DisplayBitmap(&dc, &theApp.hBackgroundToolsDualCMYK, 0, 0, DB_SHOWUPPERLEFT);
		}
		else {
			theApp.DisplayBitmap(&dc, &theApp.hBackgroundTools, 0, 0, DB_SHOWUPPERLEFT);
		}
}


void CController4Dlg3::OnBnClickedSpitonoffAa()
{
	InkSpitState[0] = !InkSpitState[0];
	ReconcileIcons();
}


void CController4Dlg3::OnBnClickedSpitonoffAb()
{
	InkSpitState[1] = !InkSpitState[1];
	ReconcileIcons();
}


void CController4Dlg3::OnBnClickedSpitonoffAc()
{
	InkSpitState[2] = !InkSpitState[2];
	ReconcileIcons();
}


void CController4Dlg3::OnBnClickedSpitonoffAd()
{
	InkSpitState[3] = !InkSpitState[3];
	ReconcileIcons();
}


void CController4Dlg3::OnBnClickedSpitonoffBa()
{
	InkSpitState[4] = !InkSpitState[4];
	ReconcileIcons();
}


void CController4Dlg3::OnBnClickedSpitonoffBb()
{
	InkSpitState[5] = !InkSpitState[5];
	ReconcileIcons();
}


void CController4Dlg3::OnBnClickedSpitonoffBc()
{
	InkSpitState[6] = !InkSpitState[6];
	ReconcileIcons();
}


void CController4Dlg3::OnBnClickedSpitonoffBd()
{
	InkSpitState[7] = !InkSpitState[7];
	ReconcileIcons();
}


void CController4Dlg3::ReconcileIcons() {
int i, total_active = 0;
//static int old_active;
static BOOL oldState;


	for(i = 0; i < 8; i++) {

		if(InkSpitState[i]) {
			buttons[i]->LoadBitmaps(IDB_ON_MARKER, IDB_ON_MARKER, IDB_ON_MARKER, IDB_ON_MARKER);
			total_active++;
		}
		else {
			buttons[i]->LoadBitmaps(IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER, IDB_OFF_MARKER);
		}
		buttons[i]->Invalidate();
	}

	if(total_active) {

		//if(!oldState) {
			/// turn on test mode
			USBPrinterCommand(USB_CMD_PRINTER_TEST_MODE, 1);
			Sleep(0);
			Sleep(20);
			bInTestMode = TRUE;
		//}

	}
	else {

		//if(oldState) {
			/// turn off test mode
			USBPrinterCommand(USB_CMD_PRINTER_TEST_MODE, 0);
			Sleep(0);
			Sleep(20);
			bInTestMode = FALSE;
		//}

	}

	if(bInTestMode /*old_active != total_active*/) {

		USBPrinterCommand(USB_CMD_SET_TEST_CHANNELS, GetTestChannels());
		Sleep(0);
		Sleep(20);

	}
	
	oldState = bInTestMode;
	old_active = total_active;

	if(!theApp.InksOKToPrint() && total_active) {
		theDlg->OKBox("*** WARNING ***", "ONE OR MORE INK", "CHANNEL EMPTY!");
	}


}

unsigned char CController4Dlg3::GetTestChannels() {
unsigned char test_channels = 0;

	/*
	if(InkSpitState[0]) test_channels |= 0x01;
	if(InkSpitState[1]) test_channels |= 0x02;
	if(InkSpitState[2]) test_channels |= 0x04;
	if(InkSpitState[3]) test_channels |= 0x08;

	if(InkSpitState[4]) test_channels |= 0x10;
	if(InkSpitState[5]) test_channels |= 0x20;
	if(InkSpitState[6]) test_channels |= 0x40;
	if(InkSpitState[7]) test_channels |= 0x80;
	*/

	if(InkSpitState[0]) test_channels |= 0x10;
	if(InkSpitState[1]) test_channels |= 0x20;
	if(InkSpitState[2]) test_channels |= 0x40;
	if(InkSpitState[3]) test_channels |= 0x80;

	if(InkSpitState[4]) test_channels |= 0x04;
	if(InkSpitState[5]) test_channels |= 0x08;
	if(InkSpitState[6]) test_channels |= 0x01;
	if(InkSpitState[7]) test_channels |= 0x02;


	/**/ TRACE("Test Channels: 0x%02X\n", test_channels);

	return test_channels;

}


void CController4Dlg3::OnKillFocus(CWnd* pNewWnd) {


	/* 
	if(bInTestMode) {
		/// turn off test mode
		USBPrinterCommand(USB_CMD_PRINTER_TEST_MODE, 0);
	}
	*/


	CDialogEx::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
}


void CController4Dlg3::OnBnClickedDiag2() {


	if(!theApp.GVars->bAdvancedFeaturesUnlocked) {
		theApp.GVars->bAdvancedFeaturesUnlocked = UnlockAdvanced();
	}
	
	if(theApp.GVars->bAdvancedFeaturesUnlocked) {
	
		if(!theApp.InksOKToPrint()) {
			theDlg->OKBox("*** WARNING ***", "ONE OR MORE INK", "CHANNEL EMPTY!");
		}

		CDebugDiag 	dlg;

		if(bInTestMode) {
			/// turn off test mode
			USBPrinterCommand(USB_CMD_PRINTER_TEST_MODE, 0);
			AllSpitOff();
		}


		dlg.DoModal();


		char tmp_buff[64];
		sprintf(tmp_buff, "%d", theApp.GVars->iDefaultPlatenHeight);
		m_DefaultHeight.SetWindowText(tmp_buff);



	}



}


void CController4Dlg3::OnBnClickedAllHeadA() 
{
	InkSpitState[3] = InkSpitState[2] = InkSpitState[1] = InkSpitState[0] = !InkSpitState[0];
	ReconcileIcons();
}


void CController4Dlg3::OnBnClickedAllHeadB()
{
	InkSpitState[7] = InkSpitState[6] = InkSpitState[5] = InkSpitState[4] = !InkSpitState[4];
	ReconcileIcons();
}


void CController4Dlg3::OnBnClickedLevelPlaten() {

	//if(!CheckAdvanced())
	//	return;


	if(theApp.GVars->bIgnorePlatenHeight) {
		theDlg->OKBox("Platen Level", "feature disabled",NULL);
		return;
	}

	if(theApp.bPendingAutoLevelRequest) {

		theDlg->OKBox("Platen Level", "Already Pending", NULL);

		return;
	}

	theApp.bPendingAutoLevelRequest = TRUE;

	USBPrinterCommand(USB_CMD_AUTO_PLATEN_HEIGHT, 1);

	Sleep(500);

}


void CController4Dlg3::OnBnClickedHeadMaintPos()
{

#ifdef MAINT_POS_SAFETY
	if(theApp.GHeadPosition == 0) {
#endif

		USBPrinterCommand(USB_CMD_CAP, 0);
		USBPrinterCommand(USB_CMD_VAC_VALVE, 0);
		USBPrinterCommand(USB_HEAD_TO_MAINT, 0);

#ifdef MAINT_POS_SAFETY
	}
	else {

		theDlg->OKBox("Head Must Be", "Be Parked First", "");

	}
#endif
}


void CController4Dlg3::OnBnClickedHomeHead() {

	//USBPrinterCommand(USB_CMD_PARK, 0);
	USBPrinterCommandEx(USB_CMD_PARK, 0, 0, 1, 0, 0, 0, 0); /// <=== request to cap immediately


}


void CController4Dlg3::OnBnClickedLoadSoftware() {
CSoftwareDL dlg;


	if(theApp.GMasterStatus != MASTER_STATUS_IDLE)
		return;

	if(!CheckAdvanced())
		return;


	//if((resp = AfxMessageBox("Are You Sure You want to install new software?", APP_NAME, MB_SYSTEMMODAL|MB_YESNO) == IDYES)) {
	//if((resp = AfxMessageBox("Are You Sure You want to install new software?", MB_SYSTEMMODAL|MB_YESNO) == IDYES)) {


	CDeleteVerify vdlg;
	int ret;
		
	strcpy(vdlg.m_szMessage, "Are you sure you want to\ninstall new software");

	if(bInTestMode) {
		/// turn off test mode
		USBPrinterCommand(USB_CMD_PRINTER_TEST_MODE, 0);
		AllSpitOff();
	}

	ret = vdlg.DoModal();

	if(ret == 1) {

		dlg.DoModal();
	
	}



}


/*
void CController4Dlg3::OnCbnSelchangePtOffset() {

	int i =	m_PTOffset.GetCurSel();

	if(i < 0 || i > 2) i = 0;
	theApp.GVars->CurrentHeightOffset = i;

	//USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT_OFFSET, theApp.GVars->PlatenHeightOffset[theApp.GVars->CurrentHeightOffset]);
	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->PlatenHeightOffset[theApp.GVars->CurrentHeightOffset], SAVE_POS_AS_PLATENGAP, 0, 0, 0, 0, 0);

	theApp.WriteINI();

}
*/


void CController4Dlg3::OnBnClickedPower()
{

	/*if(!theApp.GVars->bAdvancedFeaturesUnlocked) {
		theApp.GVars->bAdvancedFeaturesUnlocked = UnlockAdvanced();
	}
	*/
	CDeleteVerify vdlg;
	int ret;
		
	strcpy(vdlg.m_szMessage, "Are you sure you want\n to exit?");

	ret = vdlg.DoModal();

	if(ret == 1) {
	//if(theApp.GVars->bAdvancedFeaturesUnlocked) {
		theApp.CloseRequest = TRUE;
		theApp.bHomePressed = TRUE;
		CDialogEx::OnCancel();
	//}
	}

}


void CController4Dlg3::OnBnClickedDemoMode()
{

	if(!CheckAdvanced())
		return;

	if(!theApp.GVars->bAdvancedFeaturesUnlocked) {
		theApp.GVars->bAdvancedFeaturesUnlocked = UnlockAdvanced();
	}
	
	if(theApp.GVars->bAdvancedFeaturesUnlocked) {

		if(!theApp.bPauseMode) {

			theDlg->HandleButtonPress(CENTER_BUTTON);

		}

		CDemoMode dlg;

		if(bInTestMode) {
			/// turn off test mode
			USBPrinterCommand(USB_CMD_PRINTER_TEST_MODE, 0);
			AllSpitOff();
		}


		dlg.DoModal();

		CDialogEx::OnCancel();

	}

}


void CController4Dlg3::OnEnChangeDefaultheight()
{



}


void CController4Dlg3::SetLiftSteppersHeight(int s1_steps, int s2_steps, int s3_steps, int s4_steps) {
int height = 0, c, d, total_moves = 0, n = 4, exists;
step_order_loc orderloc[4], tmp_loc, new_orderloc[4];

   orderloc[0].stepper = 0x01;
   orderloc[0].loc = s1_steps;

   orderloc[1].stepper = 0x02;
   orderloc[1].loc = s2_steps;

   orderloc[2].stepper = 0x04;
   orderloc[2].loc = s3_steps;

   orderloc[3].stepper = 0x08;
   orderloc[3].loc = s4_steps;

   // First bubble sort them in order

	for(c = 0; c < (n - 1); c++) {

		for(d = 0; d < n - c - 1; d++) {

			if(orderloc[d].loc > orderloc[d + 1].loc) {

				memcpy((char *)&tmp_loc, (char *)&orderloc[d], sizeof(step_order_loc));

				memcpy((char *)&orderloc[d], (char *)&orderloc[d + 1], sizeof(step_order_loc));

				memcpy((char *)&orderloc[d + 1], (char *)&tmp_loc, sizeof(step_order_loc));

			}

		}


	}


	// now copy unique heights to new list
	memcpy((char *)&new_orderloc[0], (char *)&orderloc[0], sizeof(step_order_loc));
	total_moves = 1;
  
	for(c = 0; c < n - 1; c++) {

		if(orderloc[c + 1].loc <= new_orderloc[c].loc) {

			new_orderloc[c].stepper |= orderloc[c + 1].stepper;

		}
		else if(orderloc[c + 1].loc > new_orderloc[c].loc) {
			exists = 0;
			for(d = 0; d < 4; d++) {
				if(orderloc[c + 1].loc == new_orderloc[d].loc) {
					exists = 1;
				}
			}
			if(!exists) {
				memcpy((char *)&new_orderloc[total_moves++], (char *)&orderloc[c + 1], sizeof(step_order_loc));
			}
			else {
				new_orderloc[total_moves - 1].stepper |= orderloc[c + 1].stepper;
			}
		}

	}
	
	// and make sure all steppers with higher values get set too
	for(c = 0; c < total_moves; c++) {
		for(d = 0; d < total_moves; d++) {

			if(c != d && new_orderloc[d].loc >= new_orderloc[c].loc) {
				new_orderloc[c].stepper |= new_orderloc[d].stepper;
			}

		}
	}


	for(c = 0; c < total_moves; c++) {

		if(new_orderloc[c].loc > 0) {
			//StatusLine("Move %d: %d (0x%02X)\r\n", c + 1, new_orderloc[c].loc - height, new_orderloc[c].stepper);
		}

		height = new_orderloc[c].loc;


	}


}

void CController4Dlg3::OnBnClickedHomez()
{

	SendPlatenLevelingOffset();
	
	USBPrinterCommand(USB_CMD_HOME_PLATEN_Z, 1);
}


void CController4Dlg3::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CController4Dlg3::DisplayHeight() {
char tmp_buff[64];

	sprintf(tmp_buff, "%d", theApp.GVars->iDefaultPlatenHeight); 
	m_DefaultHeight.SetWindowText(tmp_buff);


}



void CController4Dlg3::OnBnClickedCheckfeedheight() {

	if(!CheckAdvanced())
		return;


	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 15001); /// special number, means go to feed position

}


void CController4Dlg3::OnBnClickedIgnoreObstruction()
{

	if(!CheckAdvanced()) {
		m_IgnoreObstruction.SetCheck(theApp.GVars->bIgnoreObstruction);
		return;
	}

	theApp.GVars->bIgnoreObstruction = m_IgnoreObstruction.GetCheck();
	SetObstructionLaserButtonText();
	theApp.WriteINI();
	USBPrinterCommand(USB_CMD_IGNORE_OBSTRUCTION, theApp.GVars->bIgnoreObstruction);

}


void CController4Dlg3::OnBnClickedLevelPlaten4()
{

	if(!CheckAdvanced())
		return;

	CServoOffsets dlg;

	dlg.DoModal();

}


void CController4Dlg3::OnBnClickedManualHeight() {
CManualPlatenHeights dlg;

	if(!CheckAdvanced())
		return;

	dlg.DoModal();

	char tmp_buff[64];
	sprintf(tmp_buff, "%d", theApp.GVars->iDefaultPlatenHeight);
	m_DefaultHeight.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%d", theApp.GVars->iPlatenFeedHeight);
	m_FeedHeight.SetWindowText(tmp_buff);

}


void CController4Dlg3::OnBnClickedCheckprintheight()
{

	if(!CheckAdvanced())
		return;


	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 15500); /// special number, means go to print position
}


void CController4Dlg3::OnBnClickedLevelPlaten3()
{
	if(theApp.GVars->bIgnorePlatenHeight) {
		theDlg->OKBox("Platen Level", "feature disabled",NULL);
		return;
	}

	if(theApp.bPendingAutoLevelRequest) {

		theDlg->OKBox("Platen Level", "Already Pending", NULL);

		return;
	}

	theApp.bPendingAutoLevelRequest = TRUE;

	USBPrinterCommand(USB_CMD_AUTO_PLATEN_HEIGHT, 2);

	SendPlatenLevelingOffset();

	USBPrinterCommand(USB_CMD_HOME_PLATEN_Z, 1);

	Sleep(500);
}


void CController4Dlg3::OnBnClickedRemoteAction()
{
	// TODO: Add your control notification handler code here
}


void CController4Dlg3::OnLbnSelchangePlatengap()
{
	theApp.GVars->CurrentHeightOffset = m_PlatenGapList.GetCurSel();
	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->PlatenHeightOffset[theApp.GVars->CurrentHeightOffset], SAVE_POS_AS_PLATENGAP, 0, 0, 0, 0, 0);
	theApp.WriteINI();
}


void CController4Dlg3::OnBnClickedPlatenGapPlus()
{
int cursel = m_PlatenGapList.GetCurSel();

	cursel--;

	if(cursel < 0)
		cursel = 2;

	m_PlatenGapList.SetCurSel(cursel);

	OnLbnSelchangePlatengap();
}


void CController4Dlg3::OnBnClickedPlatenGapMinus() {

	int cursel = m_PlatenGapList.GetCurSel();

	cursel++;

	if(cursel > 2)
		cursel = 0;

	m_PlatenGapList.SetCurSel(cursel);

	OnLbnSelchangePlatengap();


}


void CController4Dlg3::OnBnClickedParkplaten() {
	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 15002); /// special number, means go to feed position
}

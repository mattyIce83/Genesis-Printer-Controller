// ManualPlatenHeights.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "ManualPlatenHeights.h"
#include "Controller4Dlg1.h"
#include "afxdialogex.h"
#include "parser.h"
#define STEPS_DEFINED
#include "HEAD_structs.h"


extern CController4Dlg1 *theDlg;
extern CController4App theApp;
extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;

extern void SendPlatenLevelingOffset();


// CManualPlatenHeights dialog

IMPLEMENT_DYNAMIC(CManualPlatenHeights, CDialogEx)

CManualPlatenHeights::CManualPlatenHeights(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualPlatenHeights::IDD, pParent)
{

}

CManualPlatenHeights::~CManualPlatenHeights()
{
}

void CManualPlatenHeights::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FL_10UD, m_PLHeight100UD);
	DDX_Control(pDX, IDC_FL_10UD_2, m_PLHeight100UD_2);
	DDX_Control(pDX, IDC_FL_1UD, m_PLHeight10UD);
	DDX_Control(pDX, IDC_FL_1UD_2, m_PLHeight10UD_2);
	DDX_Control(pDX, IDC_PRINT_HEIGHT, m_PrintHeight);
	DDX_Control(pDX, IDC_FEED_HEIGHT, m_FeedHeight);
	DDX_Control(pDX, IDC_OF_10UD, m_OFHeight100UD);
	DDX_Control(pDX, IDC_OF_1UD, m_OFHeight10UD);
	DDX_Control(pDX, IDC_PARKEDOFFSET, m_ParkOffset);
}


BEGIN_MESSAGE_MAP(CManualPlatenHeights, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FL_10UD, &CManualPlatenHeights::OnDeltaposFl10ud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FL_1UD, &CManualPlatenHeights::OnDeltaposFl1ud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FL_10UD_2, &CManualPlatenHeights::OnDeltaposFl10ud2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FL_1UD_2, &CManualPlatenHeights::OnDeltaposFl1ud2)
	ON_BN_CLICKED(IDOK, &CManualPlatenHeights::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECKPRINTHEIGHT, &CManualPlatenHeights::OnBnClickedCheckprintheight)
	ON_BN_CLICKED(IDC_CHECKFEEDHEIGHT, &CManualPlatenHeights::OnBnClickedCheckfeedheight)
	ON_BN_CLICKED(IDCANCEL, &CManualPlatenHeights::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_HOMEHEIGHT, &CManualPlatenHeights::OnBnClickedHomeheight)
	ON_BN_CLICKED(IDC_CHECKPARKOFFSET, &CManualPlatenHeights::OnBnClickedCheckparkoffset)
	ON_NOTIFY(UDN_DELTAPOS, IDC_OF_10UD, &CManualPlatenHeights::OnDeltaposOf10ud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_OF_1UD, &CManualPlatenHeights::OnDeltaposOf1ud)
END_MESSAGE_MAP()


// CManualPlatenHeights message handlers


BOOL CManualPlatenHeights::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	oldPlatenFeedHeight = theApp.GVars->iPlatenFeedHeight;

	oldPlatenPrintHeight = theApp.GVars->iDefaultPlatenHeight;

	m_PLHeight100UD.SetFont(&theDlg->pMediumFont);
	m_PLHeight10UD.SetFont(&theDlg->pMediumFont);

	m_PLHeight100UD_2.SetFont(&theDlg->pMediumFont);
	m_PLHeight10UD_2.SetFont(&theDlg->pMediumFont);



	WINDOWPLACEMENT  wp1, wp2;

	m_PLHeight100UD.GetWindowPlacement(&wp1);
	m_PLHeight10UD.GetWindowPlacement(&wp2);

	wp1.rcNormalPosition.right = wp1.rcNormalPosition.left + 90;
	wp2.rcNormalPosition.right = wp2.rcNormalPosition.left + 90;

	m_PLHeight100UD.SetWindowPlacement(&wp1);
	m_PLHeight10UD.SetWindowPlacement(&wp2);


	m_PLHeight100UD_2.GetWindowPlacement(&wp1);
	m_PLHeight10UD_2.GetWindowPlacement(&wp2);

	wp1.rcNormalPosition.right = wp1.rcNormalPosition.left + 90;
	wp2.rcNormalPosition.right = wp2.rcNormalPosition.left + 90;

	m_PLHeight100UD_2.SetWindowPlacement(&wp1);
	m_PLHeight10UD_2.SetWindowPlacement(&wp2);


	m_OFHeight100UD.GetWindowPlacement(&wp1);
	m_OFHeight10UD.GetWindowPlacement(&wp2);

	wp1.rcNormalPosition.right = wp1.rcNormalPosition.left + 90;
	wp2.rcNormalPosition.right = wp2.rcNormalPosition.left + 90;

	m_OFHeight100UD.SetWindowPlacement(&wp1);
	m_OFHeight10UD.SetWindowPlacement(&wp2);


	theApp.ReadINI();

	DisplayPrintHeight();

	DisplayFeedHeight();

	DisplayOffset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CManualPlatenHeights::DisplayPrintHeight() {

	char tmp_buff[64];
	sprintf(tmp_buff, "%d", theApp.GVars->iDefaultPlatenHeight);
	m_PrintHeight.SetWindowText(tmp_buff);


}


void CManualPlatenHeights::DisplayFeedHeight() {

	char tmp_buff[64];

	sprintf(tmp_buff, "%d", theApp.GVars->iPlatenFeedHeight);
	m_FeedHeight.SetWindowText(tmp_buff);


}

void CManualPlatenHeights::DisplayOffset() {

	char tmp_buff[64];

	sprintf(tmp_buff, "%d", theApp.GVars->iPlatenParkOffset);
	m_ParkOffset.SetWindowText(tmp_buff);


}


void CManualPlatenHeights::OnDeltaposFl10ud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) theApp.GVars->iDefaultPlatenHeight += 100;
	else theApp.GVars->iDefaultPlatenHeight -= 100;

	if(theApp.GVars->iDefaultPlatenHeight < 0)
		theApp.GVars->iDefaultPlatenHeight = 0;

	DisplayPrintHeight();



	*pResult = 0;
}


void CManualPlatenHeights::OnDeltaposFl1ud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) theApp.GVars->iDefaultPlatenHeight += 10;
	else theApp.GVars->iDefaultPlatenHeight -= 10;

	if(theApp.GVars->iDefaultPlatenHeight < 0)
		theApp.GVars->iDefaultPlatenHeight = 0;

	DisplayPrintHeight();


	*pResult = 0;
}


void CManualPlatenHeights::OnDeltaposFl10ud2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) theApp.GVars->iPlatenFeedHeight += 100;
	else theApp.GVars->iPlatenFeedHeight -= 100;

	if(theApp.GVars->iPlatenFeedHeight < 0)
		theApp.GVars->iPlatenFeedHeight = 0;

	DisplayFeedHeight();



	*pResult = 0;
}


void CManualPlatenHeights::OnDeltaposFl1ud2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) theApp.GVars->iPlatenFeedHeight += 10;
	else theApp.GVars->iPlatenFeedHeight -= 10;

	if(theApp.GVars->iPlatenFeedHeight < 0)
		theApp.GVars->iPlatenFeedHeight = 0;

	DisplayFeedHeight();

	*pResult = 0;
}


void CManualPlatenHeights::OnBnClickedOk()
{

	int overshoot;

	if(!theApp.CheckMaxStepperThrow(&overshoot)) {

		char message1[128];

		sprintf(message1, "%d units too high", overshoot);

		theDlg->OKBox("** Invalid Height **","Requested height is", message1);

		return;

	}




	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->PlatenHeightOffset[theApp.GVars->CurrentHeightOffset], SAVE_POS_AS_PLATENGAP, 0, 0, 0, 0, 0);
	Sleep(100);
	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iPlatenFeedHeight, SAVE_POS_AS_FEEDINGPOS, 0, 0, 0, 0, 0);
	Sleep(100);
	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iDefaultPlatenHeight, SAVE_POS_AS_PRINTPOS, 0, 0, 0, 0, 0);
	Sleep(100);
	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iPlatenParkOffset, SAVE_POS_AS_SPECIAL_2, 0, 0, 0, 0, 0);
	if(theApp.GVars->iPlatenParkOffset > 1) {
		USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iPlatenParkOffset, SAVE_POS_AS_SPECIAL_2, 0, 0, 0, 0, 0);
	}


	theApp.WriteINI();

	CDialogEx::OnOK();
}


void CManualPlatenHeights::OnBnClickedCheckprintheight()
{

	int overshoot;

	if(!theApp.CheckMaxStepperThrow(&overshoot)) {

		char message1[128];

		sprintf(message1, "%d units too high", overshoot);

		theDlg->OKBox("** Invalid Height **","Requested height is", message1);

		theApp.GVars->iDefaultPlatenHeight -= overshoot;

		if(theApp.GVars->iDefaultPlatenHeight < 0)
			theApp.GVars->iDefaultPlatenHeight = 0;

	}

	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iDefaultPlatenHeight, SAVE_POS_AS_PRINTPOS, 0, 0, 0, 0, 0);
	Sleep(100);
	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 15500); // special number. means go to saved print pos.
}


void CManualPlatenHeights::OnBnClickedCheckfeedheight()
{
	int overshoot;

	if(!theApp.CheckMaxStepperThrow(&overshoot)) {

		char message1[128];

		sprintf(message1, "%d units too high", overshoot);

		theDlg->OKBox("** Invalid Height **","Requested height is", message1);

		theApp.GVars->iPlatenFeedHeight -= overshoot;

		if(theApp.GVars->iPlatenFeedHeight < 0)
			theApp.GVars->iPlatenFeedHeight = 0;

	}

	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iPlatenFeedHeight, SAVE_POS_AS_FEEDINGPOS, 0, 0, 0, 0, 0);
	Sleep(100);
	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 15001); /// special number, means go to feed position

}


void CManualPlatenHeights::OnBnClickedCancel()
{

	theApp.GVars->iPlatenFeedHeight = oldPlatenFeedHeight;

	theApp.GVars->iDefaultPlatenHeight = oldPlatenPrintHeight;


	CDialogEx::OnCancel();
}


void CManualPlatenHeights::OnBnClickedHomeheight()
{
	SendPlatenLevelingOffset();
	
	USBPrinterCommand(USB_CMD_HOME_PLATEN_Z, 1);
}


void CManualPlatenHeights::OnBnClickedCheckparkoffset()
{
	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iPlatenParkOffset, SAVE_POS_AS_SPECIAL_2, 0, 0, 0, 0, 0);
	Sleep(100);
	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 15002); /// special number, means go to feed position
}


void CManualPlatenHeights::OnDeltaposOf10ud(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) theApp.GVars->iPlatenParkOffset += 100;
	else {
		theApp.GVars->iPlatenParkOffset -= 100;
		if(theApp.GVars->iPlatenParkOffset < 0)
			theApp.GVars->iPlatenParkOffset = 0;
	}

	DisplayOffset();

	*pResult = 0;
}


void CManualPlatenHeights::OnDeltaposOf1ud(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) theApp.GVars->iPlatenParkOffset += 10;
	else {
		theApp.GVars->iPlatenParkOffset -= 10;
		if(theApp.GVars->iPlatenParkOffset < 0)
			theApp.GVars->iPlatenParkOffset = 0;
	}

	DisplayOffset();

	*pResult = 0;
}

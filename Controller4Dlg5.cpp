// Controller4Dlg5.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg5.h"
#include "InkValveControl.h"
#include "afxdialogex.h"
#include "parser.h"
#define STEPS_DEFINED
#include "HEAD_structs.h"
#include "Controller4Dlg5A.h"
#include "CleaningOption3.h"

extern CController4App theApp;

extern CController4Dlg1 *theDlg;

extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;

extern void SendPlatenHeightOffset();
extern BOOL CheckAdvanced();


// CController4Dlg5 dialog

static 	CFont pNewFont;


IMPLEMENT_DYNAMIC(CController4Dlg5, CDialogEx)

CController4Dlg5::CController4Dlg5(CWnd* pParent /*=NULL*/)
	: CDialogEx(CController4Dlg5::IDD, pParent)
{

}

CController4Dlg5::~CController4Dlg5()
{
}

void CController4Dlg5::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOME, m_Home);
	DDX_Control(pDX, IDC_HEAD_A, m_HeadA);
	DDX_Control(pDX, IDC_HEAD_B, m_HeadB);
	DDX_Control(pDX, IDC_CLEAN_LEVEL_1, m_CleanLevel1);
	DDX_Control(pDX, IDC_CLEAN_LEVEL_2, m_CleanLevel2);
	DDX_Control(pDX, IDC_CLEAN_LEVEL_3, m_CleanLevel3);
	DDX_Control(pDX, IDC_NOZZLE_CHECK1, m_NozzleCheck);
	DDX_Control(pDX, IDC_ADVANCED, m_Advanced);
	DDX_Control(pDX, IDC_DRAIN_CAP_STN, m_DrainCap);
	DDX_Control(pDX, IDC_WIPEIT, m_Wipeit);
	DDX_Control(pDX, IDC_DD_NOZZLE_CHECK, m_DDNozzleCheck);
	DDX_Control(pDX, IDC_FILL_CAP, m_FillCap);
	DDX_Control(pDX, IDC_HOMEHEAD, m_HomeHead);
	DDX_Control(pDX, IDC_HEADTOMAINT, m_HeadToMaint);
	DDX_Control(pDX, IDC_END_OF_DAY, m_EndOfDay);
}


BEGIN_MESSAGE_MAP(CController4Dlg5, CDialogEx)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_HOME, &CController4Dlg5::OnBnClickedHome)
	ON_BN_CLICKED(IDC_RIGHT, &CController4Dlg5::OnBnClickedRight)
	ON_BN_CLICKED(IDC_HLP, &CController4Dlg5::OnBnClickedHlp)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_NOZZLE_CHECK1, &CController4Dlg5::OnBnClickedNozzleCheck1)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CController4Dlg5::OnNMCustomdrawProgress1)
	ON_BN_CLICKED(IDC_CLEAN_LEVEL_1, &CController4Dlg5::OnBnClickedCleanLevel1)
	ON_BN_CLICKED(IDC_CLEAN_LEVEL_3, &CController4Dlg5::OnBnClickedCleanLevel3)
	ON_BN_CLICKED(IDC_CLEAN_LEVEL_2, &CController4Dlg5::OnBnClickedCleanLevel2)
	ON_BN_CLICKED(IDC_HEAD_BATH, &CController4Dlg5::OnBnClickedHeadBath)
	ON_BN_CLICKED(IDC_CAP_FLUSH, &CController4Dlg5::OnBnClickedCapFlush)
	ON_BN_CLICKED(IDC_WIPE_HEAD, &CController4Dlg5::OnBnClickedWipeHead)
	ON_BN_CLICKED(IDC_QUICK_CLEAN, &CController4Dlg5::OnBnClickedQuickClean)
	ON_BN_CLICKED(IDC_ADVANCED, &CController4Dlg5::OnBnClickedAdvanced)
	ON_BN_CLICKED(IDC_DRAIN_CAP_STN, &CController4Dlg5::OnBnClickedDrainCapStn)
	ON_BN_CLICKED(IDC_WIPEIT, &CController4Dlg5::OnBnClickedWipeit)
	ON_BN_CLICKED(IDC_DD_NOZZLE_CHECK, &CController4Dlg5::OnBnClickedDdNozzleCheck)
	ON_BN_CLICKED(IDC_FILL_CAP, &CController4Dlg5::OnBnClickedFillCap)
	ON_BN_CLICKED(IDC_HOMEHEAD, &CController4Dlg5::OnBnClickedHomehead)
	ON_BN_CLICKED(IDC_HEADTOMAINT, &CController4Dlg5::OnBnClickedHeadtomaint)
	ON_BN_CLICKED(IDC_END_OF_DAY, &CController4Dlg5::OnBnClickedEndOfDay)
END_MESSAGE_MAP()


// CController4Dlg5 message handlers


void CController4Dlg5::SizeDialog() {
	SetWindowPlacement(&theApp.MainWP);

	//m_Home.SetWindowPlacement(&theApp.HomeWP);


}

BOOL CController4Dlg5::OnInitDialog() {
int result;

	theApp.currentWindow = CLEANING_WINDOW;


	result = m_Home.AutoLoad(IDC_HOME, this);


	CDialogEx::OnInitDialog();

	m_CleanLevel1.SetFont(&theDlg->pMediumFont);
	m_CleanLevel2.SetFont(&theDlg->pMediumFont);
	m_CleanLevel3.SetFont(&theDlg->pMediumFont);
	m_DDNozzleCheck.SetFont(&theDlg->pMediumFont);
	m_NozzleCheck.SetFont(&theDlg->pMediumFont);
	m_FillCap.SetFont(&theDlg->pMediumFont);
	m_Advanced.SetFont(&theDlg->pMediumFont);
	m_HomeHead.SetFont(&theDlg->pMediumFont);
	m_HeadToMaint.SetFont(&theDlg->pMediumFont);
	m_EndOfDay.SetFont(&theDlg->pMediumFont);

	m_DrainCap.SetFont(&theDlg->pMediumFont);
	m_Wipeit.SetFont(&theDlg->pMediumFont);
	//m_DDNozzleCheck.SetFont(&theDlg->pMediumFont);

	if(theApp.GVars->iSpecialFlags & 0x01) {

		m_NozzleCheck.SetWindowText("NOZZLE CHECK\n(Easy does it, Nina!");

	}


	SizeDialog();

	m_HeadA.SetCheck(TRUE);
	m_HeadB.SetCheck(TRUE);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CController4Dlg5::OnSetFocus(CWnd* pOldWnd)
{
	CDialogEx::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}


void CController4Dlg5::OnBnClickedHome() {
	theApp.bHomePressed = TRUE;
	CDialogEx::OnCancel();
}


void CController4Dlg5::OnBnClickedRight() {
	// TODO: Add your control notification handler code here
}


void CController4Dlg5::OnBnClickedHlp() {
	// TODO: Add your control notification handler code here
}


void CController4Dlg5::OnDestroy()
{

	//USBPrinterCommand(USB_CMD_PRINTER_TEST_MODE, 0);

	CDialogEx::OnDestroy();

	pNewFont.DeleteObject();
}


void CController4Dlg5::OnBnClickedNozzleCheck1()
{

	if(!theApp.InksOKToPrint()) {
		theDlg->OKBox("UNABLE TO PRINT", "ONE OR MORE INK", "CHANNEL EMPTY!");
		return;
	}

	SendPlatenHeightOffset();
	Sleep(500);
	
	USBPrinterCommand(USB_CMD_NOZZLE_CHECK, 0);
	Sleep(500);

	theApp.StartWaitDialog(".. NOZZLE CHECK ..");

	theApp.bNozzleCheck = TRUE;
			
	while(!theApp.RunWaitDialog) {
		Sleep(100);
	}


}

void CController4Dlg5::OnPaint() {
		CPaintDC dc(this); // device context for painting
	
		CRect rect;
		GetClientRect(&rect);

		// FILL THE BACKGROUND
		//dc.FillRect(&rect, &theApp.hBGbrush);

		CDialog::OnPaint();
		theApp.DisplayBitmap(&dc, &theApp.hBackground, 0, 0, DB_SHOWUPPERLEFT);
}


void CController4Dlg5::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CController4Dlg5::OnBnClickedCleanLevel1() {
unsigned char heads = 0;

	if(!theApp.InksOKToPrint()) {
		theDlg->OKBox("UNABLE TO FULFILL", "REQUEST ONE OR MORE", " INK CHANNEL EMPTY!");
		return;
	}

	if(theApp.GPlatenPosition != 1) {
 		USBPrinterCommand(USB_CMD_TRAY_POS_REPORT_REQ, 1);
		Sleep(0);
		Sleep(1000);
		if(theApp.GPlatenPosition != 1) {
			theDlg->OKBox("Please home the", "platen before initiating", "a head clean");
			return;
		}
	}
	

	if(m_HeadA.GetCheck())
		heads |= 0x01;
	if(m_HeadB.GetCheck())
		heads |= 0x02;

	if(!heads) {
		theDlg->OKBox("You Must Select", "At Least", "One Head");
		return;
	}

	USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)heads, 1, 0, 0, 0, 0, 0);

	theApp.GVars->iColorJobCount = 0;
	theApp.GVars->iWhiteJobCount = 0;


}

void CController4Dlg5::OnBnClickedCleanLevel3()
{
unsigned char heads = 0;

	/*if(m_HeadA.GetCheck())
		heads |= 0x01;
	if(m_HeadB.GetCheck())
		heads |= 0x02;

	if(!heads) {
		theDlg->OKBox("You Must Select", "At Least", "One Head");
		return;
	}
	*/

	//USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)heads, 3, 0, 0, 0, 0, 0);

	CCleaningOption3 dlg;

	dlg.bDualCMYK = theApp.GVars->bDualCMYK;

	dlg.DoModal();

}


void CController4Dlg5::OnBnClickedCleanLevel2()
{


	if(!theApp.InksOKToPrint()) {
		theDlg->OKBox("UNABLE TO EXECUTE", "ONE OR MORE INK", "CHANNEL EMPTY!");
		return;
	}

	CCleaningOption3 dlg;

	dlg.bDualCMYK = theApp.GVars->bDualCMYK;
	dlg.Function = 0;

	dlg.DoModal();


}


void CController4Dlg5::OnBnClickedHeadBath()
{
	USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 5, 0, 0, 0, 0, 0);
}


void CController4Dlg5::OnBnClickedCapFlush()
{
	USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 6, 0, 0, 0, 0, 0);
}


void CController4Dlg5::OnBnClickedWipeHead()
{
	USBPrinterCommand(USB_CMD_CAP, 0);
	USBPrinterCommand(USB_CMD_WIPE, 1);
	USBPrinterCommand(USB_CMD_CAP, 1);
}


void CController4Dlg5::OnBnClickedQuickClean()
{
	USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)7, 7, 0, 0, 0, 0, 0);
}


void CController4Dlg5::OnBnClickedAdvanced() {
Controller4Dlg5A dlg;

	if(!CheckAdvanced())
		return;

	dlg.DoModal();


}


void CController4Dlg5::OnBnClickedDrainCapStn()
{

	// the "1" tells it to execute the beginning of the procedure
	// call it with a "2" to do the end of the procedure
	// call it with a "3" to do the whole routine
	USBPrinterCommandEx(USB_CMD_CLEAN, 1, 11, 0, 0, 0, 0, 0);

}


void CController4Dlg5::OnBnClickedWipeit() {

	USBPrinterCommand(USB_CMD_CAP, 0);
	USBPrinterCommand(USB_CMD_WIPE, 1);
	USBPrinterCommand(USB_CMD_CAP, 1);

}


void CController4Dlg5::OnBnClickedDdNozzleCheck()
{

	if(!theApp.InksOKToPrint()) {
		theDlg->OKBox("UNABLE TO PRINT", "ONE OR MORE INK", "CHANNEL EMPTY!");
		return;
	}


	SendPlatenHeightOffset();
	Sleep(500);
	
	USBPrinterCommand(USB_CMD_NOZZLE_CHECK, 1);
	Sleep(500);

}


void CController4Dlg5::OnBnClickedFillCap()
{


	USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 6, 0, 0, 0, 0, 0);

}


void CController4Dlg5::OnBnClickedHomehead()
{
	USBPrinterCommandEx(USB_CMD_PARK, 0, 0, 1, 0, 0, 0, 0); /// <=== request to cap immediately
}


void CController4Dlg5::OnBnClickedHeadtomaint()
{
		USBPrinterCommand(USB_CMD_CAP, 0);
		USBPrinterCommand(USB_CMD_VAC_VALVE, 0);
		USBPrinterCommand(USB_HEAD_TO_MAINT, 0);
}


void CController4Dlg5::OnBnClickedEndOfDay()
{

	if(!theApp.InksOKToPrint()) {
		theDlg->OKBox("UNABLE TO EXECUTE", "ONE OR MORE INK", "CHANNEL EMPTY!");
		return;
	}

	if(theApp.CancelContinue("DO YOU WISH TO EXECUTE\nEND-OF-DAY CLEANING?", 0)) {
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)0, 12, 0, 0, 0, 0, 0); // do first block
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)1, 12, 0, 0, 0, 0, 0); // do second block. The rest is handled by the callbacks
	}



}

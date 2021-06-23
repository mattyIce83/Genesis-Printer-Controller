// Controller4Dlg5A.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg5A.h"
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



// Controller4Dlg5A dialog

IMPLEMENT_DYNAMIC(Controller4Dlg5A, CDialogEx)

Controller4Dlg5A::Controller4Dlg5A(CWnd* pParent /*=NULL*/)
	: CDialogEx(Controller4Dlg5A::IDD, pParent)
{

}

Controller4Dlg5A::~Controller4Dlg5A()
{
}

void Controller4Dlg5A::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILL_A, m_FillDrain);
	DDX_Control(pDX, IDC_DRAIN_A, m_Prime);
}


BEGIN_MESSAGE_MAP(Controller4Dlg5A, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_WIPE_HEAD, &Controller4Dlg5A::OnBnClickedWipeHead)
	ON_BN_CLICKED(IDC_HEAD_BATH, &Controller4Dlg5A::OnBnClickedHeadBath)
	ON_BN_CLICKED(IDC_CAP_FLUSH, &Controller4Dlg5A::OnBnClickedCapFlush)
	ON_BN_CLICKED(IDC_QUICK_CLEAN, &Controller4Dlg5A::OnBnClickedQuickClean)
	ON_BN_CLICKED(IDC_FILL_A, &Controller4Dlg5A::OnBnClickedFillA)
	ON_BN_CLICKED(IDC_FILL_B, &Controller4Dlg5A::OnBnClickedFillB)
	ON_BN_CLICKED(IDC_FILL_ALL, &Controller4Dlg5A::OnBnClickedFillAll)
	ON_BN_CLICKED(IDC_DRAIN_A, &Controller4Dlg5A::OnBnClickedDrainA)
	ON_BN_CLICKED(IDC_DRAIN_B, &Controller4Dlg5A::OnBnClickedDrainB)
	ON_BN_CLICKED(IDC_DRAIN_ALL, &Controller4Dlg5A::OnBnClickedDrainAll)
	ON_BN_CLICKED(IDC_DRAIN, &Controller4Dlg5A::OnBnClickedDrain)
	ON_BN_CLICKED(IDC_AIR_PURGE, &Controller4Dlg5A::OnBnClickedAirPurge)
END_MESSAGE_MAP()


// Controller4Dlg5A message handlers


BOOL Controller4Dlg5A::OnInitDialog() {

	
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void Controller4Dlg5A::OnPaint()
{
		CPaintDC dc(this); // device context for painting
	
		CRect rect;
		GetClientRect(&rect);

		// FILL THE BACKGROUND
		//dc.FillRect(&rect, &theApp.hBGbrush);

		CDialog::OnPaint();
		theApp.DisplayBitmap(&dc, &theApp.hBackground, 0, 0, DB_SHOWUPPERLEFT);
}


void Controller4Dlg5A::OnBnClickedWipeHead()
{
	USBPrinterCommand(USB_CMD_CAP, 0);
	//USBPrinterCommand(USB_CMD_WIPE, 1);
	USBPrinterCommand(USB_CMD_WIPE, 2);
	USBPrinterCommand(USB_CMD_CAP, 1);
}


void Controller4Dlg5A::OnBnClickedHeadBath()
{
	USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 5, 0, 0, 0, 0, 0);
}


void Controller4Dlg5A::OnBnClickedCapFlush()
{
	
	USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 6, 0, 0, 0, 0, 0);

}


void Controller4Dlg5A::OnBnClickedQuickClean()
{
	USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 7, 0, 0, 0, 0, 0);
}


void Controller4Dlg5A::OnBnClickedFillA()
{
	//USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)1, 8, 0, 0, 0, 0, 0);

	if(!theApp.InksOKToPrint()) {
		theDlg->OKBox("UNABLE TO EXECUTE", "ONE OR MORE INK", "CHANNEL EMPTY!");
		return;
	}

	CCleaningOption3 dlg;

	dlg.bDualCMYK = theApp.GVars->bDualCMYK;
	dlg.Function = 1;

	dlg.DoModal();



}


void Controller4Dlg5A::OnBnClickedFillB()
{
	USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)2, 8, 0, 0, 0, 0, 0);
}


void Controller4Dlg5A::OnBnClickedFillAll()
{
	USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 8, 0, 0, 0, 0, 0);
}


void Controller4Dlg5A::OnBnClickedDrainA()
{

	if(!theApp.InksOKToPrint()) {
		theDlg->OKBox("UNABLE TO EXECUTE", "ONE OR MORE INK", "CHANNEL EMPTY!");
		return;
	}

	CCleaningOption3 dlg;

	dlg.bDualCMYK = theApp.GVars->bDualCMYK;
	dlg.Function = 2;

	dlg.DoModal();

	//USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)1, 9, 0, 0, 0, 0, 0);

}


void Controller4Dlg5A::OnBnClickedDrainB()
{
	if(theApp.GVars->bDualCMYK)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)2, 9, 0, 0, 0, 0, 0);
	else
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)2, 10, 0, 0, 0, 0, 0);
}


void Controller4Dlg5A::OnBnClickedDrainAll()
{
	USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 9, 0, 0, 0, 0, 0);
}


void Controller4Dlg5A::OnBnClickedDrain()
{

	if(!theApp.InksOKToPrint()) {
		theDlg->OKBox("UNABLE TO EXECUTE", "ONE OR MORE INK", "CHANNEL EMPTY!");
		return;
	}

	CCleaningOption3 dlg;

	dlg.bDualCMYK = theApp.GVars->bDualCMYK;
	dlg.Function = 3;

	dlg.DoModal();


}


void Controller4Dlg5A::OnBnClickedAirPurge() {

	if(!theApp.InksOKToPrint()) {
		theDlg->OKBox("UNABLE TO EXECUTE", "ONE OR MORE INK", "CHANNEL EMPTY!");
		return;
	}

	CCleaningOption3 dlg;

	dlg.bDualCMYK = theApp.GVars->bDualCMYK;
	dlg.Function = 4;

	dlg.DoModal();

}

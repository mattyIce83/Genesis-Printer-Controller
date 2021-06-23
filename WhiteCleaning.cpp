// WhiteCleaning.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "Controller4Dlg1.h"
#include "WhiteCleaning.h"
#include "afxdialogex.h"
#include "DripWhiteTime.h"
#include "resource.h"
#include "parser.h"
#define STEPS_DEFINED
#include "HEAD_structs.h"
#include "StartPurge.h"
#include "ReplaceCap.h"


extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;



extern CController4App theApp;
extern CController4Dlg1 *theDlg;

// CWhiteCleaning dialog

IMPLEMENT_DYNAMIC(CWhiteCleaning, CDialogEx)

CWhiteCleaning::CWhiteCleaning(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWhiteCleaning::IDD, pParent)
{

}

CWhiteCleaning::~CWhiteCleaning()
{
}

void CWhiteCleaning::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWhiteCleaning, CDialogEx)
	ON_BN_CLICKED(IDC_WHITE_DRIP, &CWhiteCleaning::OnBnClickedWhiteDrip)
	ON_BN_CLICKED(IDC_WHITE_BAG_MIX, &CWhiteCleaning::OnBnClickedWhiteBagMix)
	ON_BN_CLICKED(IDC_FLUSH, &CWhiteCleaning::OnBnClickedFlush)
	ON_BN_CLICKED(IDC_FLUSH_CLEAN, &CWhiteCleaning::OnBnClickedFlushClean)
	ON_BN_CLICKED(IDC_RECIRCULATE, &CWhiteCleaning::OnBnClickedRecirculate)
	ON_BN_CLICKED(IDC_AIR_PURGE, &CWhiteCleaning::OnBnClickedAirPurge)
	ON_BN_CLICKED(IDC_AIR_PURGE2, &CWhiteCleaning::OnBnClickedAirPurge2)
END_MESSAGE_MAP()


// CWhiteCleaning message handlers


void CWhiteCleaning::OnBnClickedWhiteDrip() {
CDripWhiteTime dlg;
int result;

	dlg.Seconds = theApp.GVars->iWhiteDripTime;

	strcpy(dlg.szMessage, "Seconds to Drip:");
	strcpy(dlg.szButtonText, "Start Drip");

	result = dlg.DoModal();

	if(result == IDOK) {

		int time = dlg.Seconds * 1000;

		theApp.GVars->iWhiteDripTime = dlg.Seconds;


		theApp.WriteINI();

		USBPrinterCommandEx(USB_CMD_WHITECLEAN, (unsigned long)WHITE_DRIP_ACTION, time, 0, 0, 0, 0, 0);

	}

}


void CWhiteCleaning::OnBnClickedWhiteBagMix() {

	USBPrinterCommandEx(USB_CMD_WHITECLEAN, (unsigned long)WHITE_BAGMIX_ACTION, 0, 0, 0, 0, 0, 0);

}


void CWhiteCleaning::OnBnClickedFlush()
{
	USBPrinterCommandEx(USB_CMD_WHITECLEAN, (unsigned long)WHITE_FLUSHHEAD_ACTION, 0, 0, 0, 0, 0, 0);
}


void CWhiteCleaning::OnBnClickedFlushClean()
{
	USBPrinterCommandEx(USB_CMD_WHITECLEAN, (unsigned long)WHITE_FLUSHNOZZLES_ACTION, 0, 0, 0, 0, 0, 0);
}


void CWhiteCleaning::OnBnClickedRecirculate() {
CDripWhiteTime dlg;
int result;

	dlg.Seconds = theApp.GVars->iWhiteRecircTime;

	strcpy(dlg.szMessage, "Seconds to recirc.:");
	strcpy(dlg.szButtonText, "Start Recirc.");

	result = dlg.DoModal();

	if(result == IDOK) {

		theApp.GVars->iWhiteRecircTime = dlg.Seconds;

		int time = dlg.Seconds * 1000;

		theApp.WriteINI();

		USBPrinterCommandEx(USB_CMD_WHITECLEAN, (unsigned long)WHITE_RECIRC_ACTION, time, 0, 0, 0, 0, 0);

	}

}


void CWhiteCleaning::OnBnClickedAirPurge() {
CStartPurge	dlg;

	dlg.DoModal();

	if(dlg.Mode == 1) {
		CReplaceCap dlg;
		dlg.DoModal();
		USBPrinterCommandEx(USB_CMD_WHITECLEAN, (unsigned long)WHITE_AIRPURGE_ACTION, 2, 0, 0, 0, 0, 0);
	}
	
	//USBPrinterCommandEx(USB_CMD_WHITECLEAN, (unsigned long)WHITE_AIRPURGE_ACTION, 0, 0, 0, 0, 0, 0);
}


void CWhiteCleaning::OnBnClickedAirPurge2()
{
		USBPrinterCommandEx(USB_CMD_WHITECLEAN, (unsigned long)WHITE_AIRPURGE_ACTION, 3, 0, 0, 0, 0, 0);
}

// CleaningOption3.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "CleaningOption3.h"
#include "afxdialogex.h"
#include "parser.h"
#define STEPS_DEFINED
#include "HEAD_structs.h"


extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;


// CCleaningOption3 dialog

IMPLEMENT_DYNAMIC(CCleaningOption3, CDialogEx)

CCleaningOption3::CCleaningOption3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCleaningOption3::IDD, pParent)
{


}

CCleaningOption3::~CCleaningOption3()
{

}

void CCleaningOption3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEAD_A, m_HeadA);
	DDX_Control(pDX, IDC_HEAD_B, m_HeadB);
	DDX_Control(pDX, IDC_BOTH_HEADS, m_BothHeads);
	DDX_Control(pDX, IDC_TITLE, m_Title);
}


BEGIN_MESSAGE_MAP(CCleaningOption3, CDialogEx)
	ON_BN_CLICKED(IDC_HEAD_A, &CCleaningOption3::OnBnClickedHeadA)
	ON_BN_CLICKED(IDC_HEAD_B, &CCleaningOption3::OnBnClickedHeadB)
	ON_BN_CLICKED(IDC_BOTH_HEADS, &CCleaningOption3::OnBnClickedBothHeads)
	ON_BN_CLICKED(IDOK, &CCleaningOption3::OnBnClickedOk)
END_MESSAGE_MAP()


// CCleaningOption3 message handlers


BOOL CCleaningOption3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(Function	== 1)
		m_Title.SetWindowText("FILL SYSTEM");
	
	if(Function	== 2)
		m_Title.SetWindowText("PRIME DAMPERS");

	if(Function	== 3)
		m_Title.SetWindowText("DRAIN SYSTEM");

	if(bDualCMYK) {
		m_HeadA.SetWindowText("YYMM CHANNELS");
		m_HeadB.SetWindowText("CCKK CHANNELS");
	}
	else {
		m_HeadA.SetWindowText("CMYK CHANNELS");
		m_HeadB.SetWindowText("WHITE CHANNELS");
	}


	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCleaningOption3::OnBnClickedHeadA() {


	if(Function == 0)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)1, 2, bDualCMYK, 0, 0, 0, 0);

	if(Function == 1) {

		if(theApp.GVars->inkADC[1] < ADC_WARNING_THRESHOLD || theApp.GVars->inkADC[2] < ADC_WARNING_THRESHOLD) {

			if(!theApp.CancelContinue("** WARNING **\nONE OR MORE CHANNELS LOW ON INK\nCONTINUE?", MB_YESNO)) {

				return;
			}

		}

		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)1, 8, bDualCMYK, 0, 0, 0, 0);
	}

	if(Function == 2)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)1, 9, bDualCMYK, 0, 0, 0, 0);

	if(Function == 3)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)1, 5, bDualCMYK, 0, 0, 0, 0);

	if(Function == 4)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)1, 7, bDualCMYK, 0, 0, 0, 0);

	theApp.GVars->iColorJobCount = 0;
	theApp.GVars->iWhiteJobCount = 0;

	CDialogEx::OnOK();
}


void CCleaningOption3::OnBnClickedHeadB()
{
	if(Function == 0)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)2, 2, bDualCMYK, 0, 0, 0, 0);
	
	if(Function == 1) {

		if(theApp.GVars->inkADC[3] < ADC_WARNING_THRESHOLD || theApp.GVars->inkADC[4] < ADC_WARNING_THRESHOLD) {

			if(!theApp.CancelContinue("** WARNING **\nONE OR MORE CHANNELS LOW ON INK\nCONTINUE?", MB_YESNO)) {

				return;
			}

		}


		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)2, 8, bDualCMYK, 0, 0, 0, 0);
	}



	if(Function == 2)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)2, 9, bDualCMYK, 0, 0, 0, 0);
	
	if(Function == 3)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)2, 5, bDualCMYK, 0, 0, 0, 0);

	if(Function == 4)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)2, 7, bDualCMYK, 0, 0, 0, 0);
	
	theApp.GVars->iColorJobCount = 0;
	theApp.GVars->iWhiteJobCount = 0;

	CDialogEx::OnOK();
}


void CCleaningOption3::OnBnClickedBothHeads() {

	if(Function == 0)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 2, bDualCMYK, 0, 0, 0, 0);

	if(Function == 1) {

		if(	theApp.GVars->inkADC[1] < ADC_WARNING_THRESHOLD ||
			theApp.GVars->inkADC[2] < ADC_WARNING_THRESHOLD ||
			theApp.GVars->inkADC[3] < ADC_WARNING_THRESHOLD ||
			theApp.GVars->inkADC[4] < ADC_WARNING_THRESHOLD) {

			if(!theApp.CancelContinue("** WARNING **\nONE OR MORE CHANNELS LOW ON INK\nCONTINUE?", MB_YESNO)) {

				return;
			}

		}


		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 8, bDualCMYK, 0, 0, 0, 0);
	}

	if(Function == 2)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 9, bDualCMYK, 0, 0, 0, 0);

	if(Function == 3)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 5, bDualCMYK, 0, 0, 0, 0);

	if(Function == 4)
		USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 7, bDualCMYK, 0, 0, 0, 0);

	theApp.GVars->iColorJobCount = 0;
	theApp.GVars->iWhiteJobCount = 0;

	CDialogEx::OnOK();

}


void CCleaningOption3::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

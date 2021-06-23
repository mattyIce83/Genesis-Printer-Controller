// StartPurge.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "StartPurge.h"
#include "afxdialogex.h"
#include "parser.h"
#define STEPS_DEFINED
#include "HEAD_structs.h"
#include "ReplaceCap.h"

extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;



// CStartPurge dialog

IMPLEMENT_DYNAMIC(CStartPurge, CDialogEx)

CStartPurge::CStartPurge(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStartPurge::IDD, pParent)
{

	Mode = 0;

}

CStartPurge::~CStartPurge()
{
}

void CStartPurge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_StartPurge);
	DDX_Control(pDX, IDCANCEL, m_EndPurge);
	DDX_Control(pDX, IDC_TITLE, m_Message);
}


BEGIN_MESSAGE_MAP(CStartPurge, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStartPurge::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStartPurge::OnBnClickedCancel)
END_MESSAGE_MAP()


// CStartPurge message handlers


void CStartPurge::OnBnClickedOk() {


	if(Mode == 0) {
		Mode = 1;
		USBPrinterCommandEx(USB_CMD_WHITECLEAN, (unsigned long)WHITE_AIRPURGE_ACTION, 0, 0, 0, 0, 0, 0);
		m_StartPurge.SetWindowText("REPEAT");
		m_EndPurge.SetWindowText("END PURGE");
		m_Message.SetWindowText("");
	}
	else {
		USBPrinterCommandEx(USB_CMD_WHITECLEAN, (unsigned long)WHITE_AIRPURGE_ACTION, 1, 0, 0, 0, 0, 0);
	}

	//CDialogEx::OnOK();

}


void CStartPurge::OnBnClickedCancel()
{

CReplaceCap dlg;

	dlg.DoModal();

	Mode = 0;

	USBPrinterCommandEx(USB_CMD_WHITECLEAN, (unsigned long)WHITE_AIRPURGE_ACTION, 2, 0, 0, 0, 0, 0);
	
	CDialogEx::OnCancel();
}

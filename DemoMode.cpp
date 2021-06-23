// DemoMode.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "DemoMode.h"
#include "afxdialogex.h"
#include "parser.h"
#include "HEAD_structs.h"


extern void USBPrinterCommand(unsigned long , unsigned long);

// CDemoMode dialog

IMPLEMENT_DYNAMIC(CDemoMode, CDialogEx)

CDemoMode::CDemoMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDemoMode::IDD, pParent)
{

}

CDemoMode::~CDemoMode()
{
}

void CDemoMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDemoMode, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDCANCEL, &CDemoMode::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDemoMode message handlers


BOOL CDemoMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Background.LoadBitmap(IDB_CAUTION_SCREEN);


	if(theApp.GPlatenPosition == 1) {
		USBPrinterCommand(USB_CMD_SYM_START_BUTTON, 1);
	}
	else {
		USBPrinterCommand(USB_CMD_HOME_PLATEN, 0);
	}

	m_nTimer = SetTimer(75, 8000, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDemoMode::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	theApp.DisplayBitmap(&dc, &Background, 0, 0, DB_SHOWUPPERLEFT);
}


void CDemoMode::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	theApp.bInDemoMode = FALSE;
	//CDialogEx::OnLButtonDown(nFlags, point);
	CDialogEx::OnCancel();

}


void CDemoMode::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CDemoMode::OnTimer(UINT_PTR nIDEvent)
{

	TRACE("Demo Timer Event\n");

	
	if(nIDEvent == m_nTimer) {
	
		if(theApp.GPlatenPosition == 1) {
			USBPrinterCommand(USB_CMD_SYM_START_BUTTON, 1);
		}
		else {
			USBPrinterCommand(USB_CMD_HOME_PLATEN, 0);
		}
	
	}
	


	CDialogEx::OnTimer(nIDEvent);
}

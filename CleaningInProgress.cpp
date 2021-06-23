// CleaningInProgress.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "CleaningInProgress.h"
#include "afxdialogex.h"
#include "resource.h"
#include "parser.h"
#define STEPS_DEFINED
#include "HEAD_structs.h"


// CleaningInProgress dialog

IMPLEMENT_DYNAMIC(CleaningInProgress, CDialogEx)

extern CController4App theApp;

extern void USBPrinterCommand(unsigned long , unsigned long);

CleaningInProgress::CleaningInProgress(CWnd* pParent /*=NULL*/)
	: CDialogEx(CleaningInProgress::IDD, pParent)
{

}

CleaningInProgress::~CleaningInProgress()
{
}

void CleaningInProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMATE1, m_AVIPlayer);
	DDX_Control(pDX, IDC_CAPVAC, m_CapPress);
	DDX_Control(pDX, IDC_CAPLABEL, m_CapVacLab);
	DDX_Control(pDX, IDC_MAXVAC, m_CapVacMaxLab);
	DDX_Control(pDX, IDC_CANCEL_CLEAN, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CleaningInProgress, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CANCEL_CLEAN, &CleaningInProgress::OnBnClickedCancelClean)
END_MESSAGE_MAP()


// CleaningInProgress message handlers


void CleaningInProgress::PreInitDialog()
{
	// TODO: Add your specialized code here and/or call the base class

	Background1.LoadBitmap(IDB_CLEANING_BACKGROUND_1);
	Background2.LoadBitmap(IDB_CLEANING_BACKGROUND_2);
	Background3.LoadBitmap(IDB_CLEANING_BACKGROUND_3);
	Background4.LoadBitmap(IDB_CLEANING_BACKGROUND_4);
	Background5.LoadBitmap(IDB_CLEANING_BACKGROUND_5);
	Background6.LoadBitmap(IDB_CLEANING_BACKGROUND_6);


	backgrounds[0] = &Background1;
	backgrounds[1] = &Background2;
	backgrounds[2] = &Background3;
	backgrounds[3] = &Background4;
	backgrounds[4] = &Background5;
	backgrounds[5] = &Background6;
	backgrounds[6] = &Background5;
	backgrounds[7] = &Background4;
	backgrounds[8] = &Background3;
	backgrounds[9] = &Background2;


	currentbk = 0;

	maxcappress = 0;

	cappress = 0;

	CDialogEx::PreInitDialog();

	POINT pt;
	pt.x = 1;
	pt.y = 1;

	logPen.lopnStyle = PS_SOLID;
	logPen.lopnColor = RGB(0, 0, 0);
	logPen.lopnWidth = pt;
	hCPen.CreatePenIndirect(&logPen);

	//AnimSection.left = 50;
	//AnimSection.top = 150;
	AnimSection.left = 250;
	AnimSection.top = 300;
	AnimSection.bottom = AnimSection.top + 540;
	AnimSection.right = AnimSection.left + 980;


}

void CleaningInProgress::DrawAnimSection() {
CDC *hdc;
int i;

	hdc = GetDC();
	hdc->SelectObject(&hCPen);

	hdc->MoveTo(AnimSection.left, AnimSection.top);
	hdc->LineTo(AnimSection.right, AnimSection.top);
	hdc->LineTo(AnimSection.right, AnimSection.bottom);
	hdc->LineTo(AnimSection.left, AnimSection.bottom);
	hdc->LineTo(AnimSection.left, AnimSection.top);

	ReleaseDC(hdc);

}

void CleaningInProgress::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(!bUsingAVI)
		theApp.DisplayBitmap(&dc, backgrounds[currentbk], 0, 0, DB_SHOWUPPERLEFT);

	// DrawAnimSection();

}


void CleaningInProgress::OnDestroy()
{

	KillTimer(m_nTimer);

	CDialogEx::OnDestroy();

	Background1.DeleteObject();
	Background2.DeleteObject();
	Background3.DeleteObject();
	Background4.DeleteObject();
	Background5.DeleteObject();
	Background6.DeleteObject();

}


void CleaningInProgress::OnTimer(UINT_PTR nIDEvent) {
static int counter, lastpress;
char tmp_txt[32];

	if(!theApp.CleaningDialogActive) {
		Exit();
		return;
	}

	counter++;

	if(counter > 3) {
		counter = 0;
		
		cappress = theApp.CalculateCapPressure(theApp.GVars->CapADC);

		if(lastpress != cappress) {

			lastpress = cappress;
	
			m_CapPress.SetPos((int)cappress);
  			sprintf(tmp_txt, "Cap Vac: %d", cappress);
			m_CapVacLab.SetWindowText(tmp_txt);

			if(cappress > maxcappress) {
				maxcappress = cappress;
				sprintf(tmp_txt, "Max: %d", maxcappress);
				m_CapVacMaxLab.SetWindowText(tmp_txt);
			}

		}

	}


	currentbk++;

	if(currentbk > 9) currentbk = 0;

	/*RECT rc;

	GetClientRect(&rc);

	InvalidateRect(&rc);
	*/

	if(!bUsingAVI)
		InvalidateRect(&AnimSection, FALSE);


	CDialogEx::OnTimer(nIDEvent);
}

void CleaningInProgress::Exit() {


	KillTimer(m_nTimer);
	if(theApp.bUnlockTempThread) {
		theApp.bUnlockTempThread = FALSE;
	}
	if(bUsingAVI) {
		m_AVIPlayer.Close();
	}
	CDialogEx::OnCancel();

}

void CleaningInProgress::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);

	//m_Home.SetWindowPlacement(&theApp.HomeWP);



}

BOOL CleaningInProgress::OnInitDialog()
{
bUsingAVI = FALSE;


	CDialogEx::OnInitDialog();

	m_CapPress.SetRange(0, (int)FULL_VAC_PSI);


	SizeDialog();

	if(!m_AVIPlayer.Open("cleaning_1.avi")) {
		TRACE("OPEN FAILED WITH %d\n", GetLastError());
	}
	else {
		TRACE("OPENED\n");
		bUsingAVI = TRUE;
		m_AVIPlayer.Play( 0, -1,-1 );			 // play avi resource.
	}

	m_nTimer = SetTimer(74, 150, 0);

	if(!bUsingAVI) {
		m_AVIPlayer.ShowWindow(SW_HIDE);
	}


	if(bEnableCancelButton)
		m_CancelButton.ShowWindow(SW_SHOW);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CleaningInProgress::OnBnClickedCancelClean()
{

	USBPrinterCommand(USB_CMD_USER_RESPONSE, 2); // return 2 for cancel
	m_CancelButton.EnableWindow(FALSE);

}

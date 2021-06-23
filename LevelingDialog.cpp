// LevelingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "LevelingDialog.h"
#include "afxdialogex.h"


// CLevelingDialog dialog

IMPLEMENT_DYNAMIC(CLevelingDialog, CDialogEx)

CLevelingDialog::CLevelingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLevelingDialog::IDD, pParent)
{

}

CLevelingDialog::~CLevelingDialog()
{
}

void CLevelingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMATE1, m_AVIPlayer);
}


BEGIN_MESSAGE_MAP(CLevelingDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLevelingDialog message handlers


void CLevelingDialog::PreInitDialog()
{
	Background1.LoadBitmap(IDB_LEVELING_BACKGROUND_1);
	Background2.LoadBitmap(IDB_LEVELING_BACKGROUND_2);
	Background3.LoadBitmap(IDB_LEVELING_BACKGROUND_3);
	Background4.LoadBitmap(IDB_LEVELING_BACKGROUND_4);
	Background5.LoadBitmap(IDB_LEVELING_BACKGROUND_5);
	Background6.LoadBitmap(IDB_LEVELING_BACKGROUND_6);
	Background7.LoadBitmap(IDB_LEVELING_BACKGROUND_7);


	backgrounds[0] = &Background1;
	backgrounds[1] = &Background2;
	backgrounds[2] = &Background3;
	backgrounds[3] = &Background4;
	backgrounds[4] = &Background5;
	backgrounds[5] = &Background6;
	backgrounds[6] = &Background7;
	backgrounds[7] = &Background6;
	backgrounds[8] = &Background5;
	backgrounds[9] = &Background4;
	backgrounds[10] = &Background3;
	backgrounds[11] = &Background2;



	currentbk = 0;

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
	AnimSection.top = 100;
	AnimSection.bottom = AnimSection.top + 600;
	AnimSection.right = AnimSection.left + 880;


	CDialogEx::PreInitDialog();
}


BOOL CLevelingDialog::OnInitDialog()
{
bUsingAVI = FALSE;

	CDialogEx::OnInitDialog();

	SizeDialog();

	if(!m_AVIPlayer.Open("leveling_1.avi")) {
		TRACE("OPEN FAILED WITH %d\n", GetLastError());
	}
	else {
		TRACE("OPENED\n");
		bUsingAVI = TRUE;
		m_AVIPlayer.Play( 0, -1,-1 );			 // play avi resource.
	}

	if(!bUsingAVI) {
		m_AVIPlayer.ShowWindow(SW_HIDE);
	}

	m_nTimer = SetTimer(74, 150, 0);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLevelingDialog::DrawAnimSection() {
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

void CLevelingDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting


	if(!bUsingAVI)
		theApp.DisplayBitmap(&dc, backgrounds[currentbk], 0, 0, DB_SHOWUPPERLEFT);

	// DrawAnimSection();
}


void CLevelingDialog::OnDestroy()
{


	Background1.DeleteObject();
	Background2.DeleteObject();
	Background3.DeleteObject();
	Background4.DeleteObject();
	Background5.DeleteObject();
	Background6.DeleteObject();
	Background7.DeleteObject();


	CDialogEx::OnDestroy();
}


void CLevelingDialog::OnTimer(UINT_PTR nIDEvent)
{
	if(!theApp.bPendingAutoLevelRequest) {
		Exit();
		return;
	}
	currentbk++;

	if(currentbk > 11) currentbk = 0;

	/*RECT rc;

	GetClientRect(&rc);

	InvalidateRect(&rc);
	*/

	if(!bUsingAVI)
		InvalidateRect(&AnimSection, FALSE);

	CDialogEx::OnTimer(nIDEvent);
}


void CLevelingDialog::Exit() {

	KillTimer(m_nTimer);
	if(theApp.bUnlockTempThread) {
		theApp.bUnlockTempThread = FALSE;
	}
	if(bUsingAVI) {
		m_AVIPlayer.Close();
	}

	CDialogEx::OnCancel();

}

void CLevelingDialog::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);

	//m_Home.SetWindowPlacement(&theApp.HomeWP);



}

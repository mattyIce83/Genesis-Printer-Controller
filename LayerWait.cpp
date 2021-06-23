// LayerWait.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "LayerWait.h"
#include "Controller4Dlg1.h"
#include "afxdialogex.h"


// CLayerWait dialog

extern CController4App theApp;
extern CController4Dlg1 *theDlg;

int delay_min[5] = {0, 0, 1, 2, 3};
int delay_sec[5] = {0, 0, 15, 30, 45};


IMPLEMENT_DYNAMIC(CLayerWait, CDialogEx)

CLayerWait::CLayerWait(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLayerWait::IDD, pParent)
{

}

CLayerWait::~CLayerWait()
{
}

void CLayerWait::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WAITLONGER, m_WaitLonger);
	DDX_Control(pDX, IDC_TEXTLABEL, m_Label);
}


BEGIN_MESSAGE_MAP(CLayerWait, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CLayerWait::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CANCELLAYERWAIT, &CLayerWait::OnBnClickedCancellayerwait)
	ON_BN_CLICKED(IDCANCEL, &CLayerWait::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_WAITLONGER, &CLayerWait::OnBnClickedWaitlonger)
	ON_BN_CLICKED(IDC_PRINTNOW, &CLayerWait::OnBnClickedPrintnow)
END_MESSAGE_MAP()


// CLayerWait message handlers


BOOL CLayerWait::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	bWaitAborted = FALSE;
	bManualMode = FALSE;

	if(theApp.GVars->LayerDelaySec < 0) theApp.GVars->LayerDelaySec = 0;
	if(theApp.GVars->LayerDelaySec > 4) theApp.GVars->LayerDelaySec = 0;


	if(theApp.GVars->LayerDelaySec == 0) {
		bManualMode = TRUE;
		m_WaitLonger.ShowWindow(SW_HIDE);
		m_Label.ShowWindow(SW_HIDE);
		countdown = 60 * 60;
	}
	else {
		countdown = 0; // delay_min[theApp.GVars->LayerDelayMin] * 60;
		countdown += delay_sec[theApp.GVars->LayerDelaySec];
	}

	// Counter location
	GetClientRect(&counter_rect);
	counter_rect.top = counter_rect.bottom - 80;
	counter_rect.left = 240;
	counter_rect.right = counter_rect.left + 220;

	m_nTimer = SetTimer(7171, 1000, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CLayerWait::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(!bManualMode) {
		ShowCount(&dc);
	}

}

void CLayerWait::ShowCount(CPaintDC *dc) {
int min, sec;

	min = countdown / 60;
	sec = countdown - (min * 60);

	if(theApp.GVars == NULL) return;

	char temp_txt[128];

	dc->SelectObject(&theDlg->pDigitalFont);
	dc->SetTextColor(RGB(0, 0, 0));
	dc->SetBkMode(TRANSPARENT);
	sprintf(temp_txt, "%02d:%02d", min, sec);
	dc->DrawText(temp_txt, strlen(temp_txt), &counter_rect, DT_CENTER|DT_END_ELLIPSIS);

}


void CLayerWait::OnTimer(UINT_PTR nIDEvent)
{
	
	if(nIDEvent == m_nTimer) {

		countdown--;

		InvalidateRect(&counter_rect, TRUE);

		if(countdown < 1) 
			OnBnClickedOk();

	}

	CDialogEx::OnTimer(nIDEvent);
}


void CLayerWait::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CLayerWait::OnDestroy()
{

	KillTimer(m_nTimer);

	CDialogEx::OnDestroy();

	
}


void CLayerWait::OnBnClickedCancellayerwait()
{
	OnBnClickedCancel();
}


void CLayerWait::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CLayerWait::OnBnClickedWaitlonger()
{
	countdown += 15;
	Sleep(500);
}


void CLayerWait::OnBnClickedPrintnow()
{

	bWaitAborted = TRUE;
	OnBnClickedOk();
}

// WaitDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "Controller4Dlg1.h"
#include "WaitDialog.h"
#include "afxdialogex.h"

extern CController4Dlg1 *theDlg;

// CWaitDialog dialog

IMPLEMENT_DYNAMIC(CWaitDialog, CDialogEx)

CWaitDialog::CWaitDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWaitDialog::IDD, pParent)
{

}

CWaitDialog::~CWaitDialog()
{
}

void CWaitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMATION, m_Animation);
}


BEGIN_MESSAGE_MAP(CWaitDialog, CDialogEx)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWaitDialog message handlers


BOOL CWaitDialog::OnInitDialog()
{
WINDOWPLACEMENT wp, Location;
int i;


	CDialogEx::OnInitDialog();

	GetWindowPlacement(&wp);

	theDlg->GetWindowPlacement(&Location);
	//theDlg->m_Preview.GetWindowPlacement(&Location);

	Location.rcNormalPosition.right /= 2; // center the swirlie over the center of the controls

	int ctrl_width = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
	int dest_width = Location.rcNormalPosition.right - Location.rcNormalPosition.left;

	int ctrl_height = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
	int dest_height = Location.rcNormalPosition.bottom - Location.rcNormalPosition.top;


	wp.rcNormalPosition.left = ((dest_width - ctrl_width) / 2) + Location.rcNormalPosition.left;
	wp.rcNormalPosition.right = wp.rcNormalPosition.left + ctrl_width;

	wp.rcNormalPosition.top = ((dest_height - ctrl_height) / 2) + Location.rcNormalPosition.top;
	wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + ctrl_height;


	SetWindowPlacement(&wp);


	RECT rc;

	GetClientRect(&rc);

	rc.top = rc.bottom - 25;

	for(i = 0; i < 3; i++) {
		SetRect(&message_rect[i], rc.left, rc.top, rc.right, rc.bottom);
	}

	if(strlen(szMessage[0]) || strlen(szMessage[1]) || strlen(szMessage[2])) {
	
		hdc = GetDC();

		//hdc->SelectObject(&theDlg->pNewFont);
		hdc->SelectObject(&theDlg->pMediumFont);
		//hdc->SetTextColor(RGB(255, 255, 255));
		//hdc->SetTextColor(RGB(0, 0, 0));
		hdc->SetTextColor(RGB(255, 255, 0));
		hdc->SetBkColor(RGB(0,0,0));
		hdc->SetBkMode(TRANSPARENT);



		for(i = 0; i < 3; i++) {

			/*
			message_rect[i].left = Location.rcNormalPosition.left;
			message_rect[i].right = Location.rcNormalPosition.right;
			message_rect[i].top = Location.rcNormalPosition.top;
			message_rect[i].bottom = Location.rcNormalPosition.bottom;
			*/

			if(strlen(szMessage[i])) {
				//hdc->DrawText(szMessage[i], strlen(szMessage[i]), &message_rect[i], DT_CENTER|DT_END_ELLIPSIS);
				hdc->DrawText(szMessage[i], strlen(szMessage[i]), &message_rect[i], DT_CENTER|DT_NOCLIP);
			}

		}

	}

	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd,GWL_EXSTYLE) ^ WS_EX_LAYERED);

	::SetLayeredWindowAttributes(m_hWnd, RGB(0xDD,0xDD,0xFF), 0, LWA_COLORKEY);

	if (m_Animation.Load(MAKEINTRESOURCE(IDR_GIF2),"GIF"))
		m_Animation.Draw();

	m_nTimer = SetTimer(74, 150, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CWaitDialog::OnTimer(UINT_PTR nIDEvent)
{
	Sleep(0);

	if(!theApp.RunWaitDialog) {
		OnOK();
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CWaitDialog::OnEraseBkgnd(CDC* pDC)
{
CRect clientRect ;

  GetClientRect(&clientRect) ;
  //pDC->FillSolidRect(clientRect, RGB(0xDD,0xDD,0xFF)) ;  // paint background in magenta
  pDC->FillSolidRect(clientRect, RGB(0,0,0)) ;  // paint background in magenta

  Sleep(0);
	//return CDialogEx::OnEraseBkgnd(pDC);
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CWaitDialog::OnPaint()
{
int i;

	CPaintDC dc(this); // device context for painting

	dc.SelectObject(&theDlg->pMediumFont);
	dc.SetTextColor(RGB(255, 255, 0));
	//dc.SetBkColor(RGB(255,0,255));
	dc.SetBkMode(TRANSPARENT);


	for(i = 0; i < 3; i++) {

		if(strlen(szMessage[i])) {

			dc.FillSolidRect(&message_rect[i], RGB(0,0,0)) ;  // paint background in magenta

			dc.DrawText(szMessage[i], strlen(szMessage[i]), &message_rect[i], DT_CENTER|DT_END_ELLIPSIS);
		}

	}

}

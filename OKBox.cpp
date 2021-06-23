// OKBox.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "afxdialogex.h"
#include "Controller4Dlg1.h"

extern CController4Dlg1 *theDlg;

// COKBox dialog

IMPLEMENT_DYNAMIC(COKBox, CDialogEx)

COKBox::COKBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(COKBox::IDD, pParent)
{

}

COKBox::~COKBox()
{
}

void COKBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_OK);
}


BEGIN_MESSAGE_MAP(COKBox, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// COKBox message handlers


BOOL COKBox::OnInitDialog()
{
	m_OK.AutoLoad(IDOK, this);


	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void COKBox::OnPaint() {
	CPaintDC dc(this); 

	RECT rc;

	GetClientRect(&rc);

	dc.FillRect(&rc, &theDlg->DrakGrayBackground); 

	rc.top += 50;

	dc.SelectObject(&theDlg->pMediumFont);
	dc.SetTextColor(RGB(0, 0, 0));
	dc.SetBkMode(TRANSPARENT);


	if(Line_1 != NULL) {
		dc.DrawText(Line_1, strlen(Line_1), &rc, DT_CENTER|DT_END_ELLIPSIS);
	}

	rc.top += 40;

	if(Line_2 != NULL) {
		dc.DrawText(Line_2, strlen(Line_2), &rc, DT_CENTER|DT_END_ELLIPSIS);
	}

	rc.top += 40;

	if(Line_3 != NULL) {
		dc.DrawText(Line_3, strlen(Line_3), &rc, DT_CENTER|DT_END_ELLIPSIS);
	}

	theApp.DisplayBitmap(&dc, &theApp.hXButton, 0, 0, DB_SHOWATPOS);
}

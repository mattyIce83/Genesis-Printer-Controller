// DevelopmentTeam.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "DevelopmentTeam.h"
#include "afxdialogex.h"

extern CController4App theApp;

// CDevelopmentTeam dialog

IMPLEMENT_DYNAMIC(CDevelopmentTeam, CDialogEx)

CDevelopmentTeam::CDevelopmentTeam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDevelopmentTeam::IDD, pParent)
{

}

CDevelopmentTeam::~CDevelopmentTeam()
{
}

void CDevelopmentTeam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDevelopmentTeam, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDevelopmentTeam message handlers


BOOL CDevelopmentTeam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDevelopmentTeam::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	theApp.DisplayBitmap(&dc, &theApp.hDevTeamBackground, 0, 0, DB_SHOWUPPERLEFT);

}

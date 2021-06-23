// FatalError.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "FatalError.h"
#include "afxdialogex.h"


// CFatalError dialog

IMPLEMENT_DYNAMIC(CFatalError, CDialogEx)

CFatalError::CFatalError(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFatalError::IDD, pParent)
{

}

CFatalError::~CFatalError()
{
}

void CFatalError::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFatalError, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CFatalError message handlers


BOOL CFatalError::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SizeDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CFatalError::PreInitDialog()
{
	
	Background1.LoadBitmap(IDB_FATAL_ERROR);


	CDialogEx::PreInitDialog();
}


void CFatalError::OnPaint()
{
	CPaintDC dc(this); // device context for painting


	theApp.DisplayBitmap(&dc, &Background1, 0, 0, DB_SHOWUPPERLEFT);

	// DrawAnimSection();
}

void CFatalError::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);

	//m_Home.SetWindowPlacement(&theApp.HomeWP);



}

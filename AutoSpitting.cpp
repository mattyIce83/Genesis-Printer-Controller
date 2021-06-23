// AutoSpitting.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "AutoSpitting.h"
#include "afxdialogex.h"


// CAutoSpitting dialog

IMPLEMENT_DYNAMIC(CAutoSpitting, CDialogEx)

CAutoSpitting::CAutoSpitting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoSpitting::IDD, pParent)
{

}

CAutoSpitting::~CAutoSpitting()
{
}

void CAutoSpitting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CAutoSpitting::Quit() {
	
	CDialogEx::OnCancel();

}

BEGIN_MESSAGE_MAP(CAutoSpitting, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAutoSpitting message handlers


BOOL CAutoSpitting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_nTimer = SetTimer(79, 150, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAutoSpitting::OnDestroy()
{

	KillTimer(m_nTimer);

	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}


void CAutoSpitting::OnTimer(UINT_PTR nIDEvent)
{

	if(theApp.bQuitAutoSpitDialog) {
		CDialogEx::OnOK();
	}

	CDialogEx::OnTimer(nIDEvent);
}

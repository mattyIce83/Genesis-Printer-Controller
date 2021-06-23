// PleaseWait.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "PleaseWait.h"
#include "afxdialogex.h"


// CPleaseWait dialog

extern CController4App theApp;

IMPLEMENT_DYNAMIC(CPleaseWait, CDialogEx)

CPleaseWait::CPleaseWait(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPleaseWait::IDD, pParent)
{

}

CPleaseWait::~CPleaseWait()
{
}

void CPleaseWait::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPleaseWait, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPleaseWait message handlers


BOOL CPleaseWait::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_nTimer = SetTimer(0x73, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CPleaseWait::OnTimer(UINT_PTR nIDEvent)
{
	
	if(theApp.PrintingWindow) {
		OnOK();
	}



	CDialogEx::OnTimer(nIDEvent);
}

// PleaseWaitInit.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "PleaseWaitInit.h"
#include "afxdialogex.h"


// CPleaseWaitInit dialog

IMPLEMENT_DYNAMIC(CPleaseWaitInit, CDialogEx)

CPleaseWaitInit::CPleaseWaitInit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPleaseWaitInit::IDD, pParent)
{

}

CPleaseWaitInit::~CPleaseWaitInit()
{
}

void CPleaseWaitInit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPleaseWaitInit, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPleaseWaitInit message handlers


BOOL CPleaseWaitInit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	tick = 0;
	m_nTimer = SetTimer(7272, 500, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CPleaseWaitInit::OnDestroy()
{
	
	KillTimer(m_nTimer);

	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}


void CPleaseWaitInit::OnTimer(UINT_PTR nIDEvent)
{
	
	tick++;

	if(tick >= 4) OnOK();

	CDialogEx::OnTimer(nIDEvent);
}

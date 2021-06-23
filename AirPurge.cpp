// AirPurge.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "AirPurge.h"
#include "afxdialogex.h"
#include "StartPurge.h"

// CAirPurge dialog

IMPLEMENT_DYNAMIC(CAirPurge, CDialogEx)

CAirPurge::CAirPurge(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAirPurge::IDD, pParent)
{

}

CAirPurge::~CAirPurge()
{
}

void CAirPurge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START, m_StartAirPurge);
	DDX_Control(pDX, IDOK, m_CancelAirPurge);
}


BEGIN_MESSAGE_MAP(CAirPurge, CDialogEx)
	ON_BN_CLICKED(IDC_START, &CAirPurge::OnBnClickedStart)
END_MESSAGE_MAP()


// CAirPurge message handlers


void CAirPurge::OnBnClickedStart()
{
CStartPurge dlg;

	if(dlg.DoModal() == IDOK) {
	}
	else {
	}
	

}

// PortalDBG.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "PortalDBG.h"
#include "afxdialogex.h"


// CPortalDBG dialog

IMPLEMENT_DYNAMIC(CPortalDBG, CDialogEx)

CPortalDBG::CPortalDBG(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPortalDBG::IDD, pParent)
{

}

CPortalDBG::~CPortalDBG()
{
}

void CPortalDBG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPortalDBG, CDialogEx)
	ON_BN_CLICKED(IDC_RESOLVE, &CPortalDBG::OnBnClickedResolve)
END_MESSAGE_MAP()


// CPortalDBG message handlers


void CPortalDBG::OnBnClickedResolve()
{
	char ip[100];

	if(theApp.NtwrkObj.GetHostIP("www.c-horse.net", ip)) {
		TRACE("IP IS: %s\n", ip);
	}

	else {
		TRACE("GetHostIP  FAILED\n");
	}

}

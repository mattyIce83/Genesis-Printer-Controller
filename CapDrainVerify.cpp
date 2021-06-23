// CapDrainVerify.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "CapDrainVerify.h"
#include "afxdialogex.h"


// CCapDrainVerify dialog

IMPLEMENT_DYNAMIC(CCapDrainVerify, CDialogEx)

CCapDrainVerify::CCapDrainVerify(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCapDrainVerify::IDD, pParent)
{

}

CCapDrainVerify::~CCapDrainVerify()
{
}

void CCapDrainVerify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCapDrainVerify, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CCapDrainVerify::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCapDrainVerify::OnBnClickedOk)
END_MESSAGE_MAP()


// CCapDrainVerify message handlers


void CCapDrainVerify::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CCapDrainVerify::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

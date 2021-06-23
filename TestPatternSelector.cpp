// TestPatternSelector.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "TestPatternSelector.h"
#include "afxdialogex.h"


// CTestPatternSelector dialog

IMPLEMENT_DYNAMIC(CTestPatternSelector, CDialogEx)

CTestPatternSelector::CTestPatternSelector(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestPatternSelector::IDD, pParent)
{

}

CTestPatternSelector::~CTestPatternSelector()
{
}

void CTestPatternSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestPatternSelector, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CTestPatternSelector::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CUST_Y_DLG, &CTestPatternSelector::OnBnClickedCustYDlg)
	ON_BN_CLICKED(IDC_Y_COR_DLG, &CTestPatternSelector::OnBnClickedYCorDlg)
END_MESSAGE_MAP()


// CTestPatternSelector message handlers


void CTestPatternSelector::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	Result = -1;;

	CDialogEx::OnCancel();
}


void CTestPatternSelector::OnBnClickedCustYDlg()
{
	Result = 0;
	CDialogEx::OnCancel();
}


void CTestPatternSelector::OnBnClickedYCorDlg()
{
	Result = 1;
	CDialogEx::OnCancel();
}

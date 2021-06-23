// YesNoCancel.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "YesNoCancel.h"
#include "afxdialogex.h"


// CYesNoCancel dialog

IMPLEMENT_DYNAMIC(CYesNoCancel, CDialogEx)

CYesNoCancel::CYesNoCancel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CYesNoCancel::IDD, pParent)
{

}

CYesNoCancel::~CYesNoCancel()
{
}

void CYesNoCancel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CYesNoCancel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CYesNoCancel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CYesNoCancel::OnBnClickedCancel)
END_MESSAGE_MAP()


// CYesNoCancel message handlers


BOOL CYesNoCancel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Message.SetWindowText(szMessage);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CYesNoCancel::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CYesNoCancel::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

// DeleteVerify.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "DeleteVerify.h"
#include "afxdialogex.h"
#include "Controller4Dlg1.h"


extern CController4App theApp;
extern CController4Dlg1 *theDlg;


// CDeleteVerify dialog

IMPLEMENT_DYNAMIC(CDeleteVerify, CDialogEx)

CDeleteVerify::CDeleteVerify(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeleteVerify::IDD, pParent)
{

}

CDeleteVerify::~CDeleteVerify()
{
}

void CDeleteVerify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
}


BEGIN_MESSAGE_MAP(CDeleteVerify, CDialogEx)
END_MESSAGE_MAP()


// CDeleteVerify message handlers


BOOL CDeleteVerify::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Message.SetWindowText(m_szMessage);
	m_Message.SetFont(&theDlg->pMediumFont);
	m_OK.SetFont(&theDlg->pMediumFont);
	m_Cancel.SetFont(&theDlg->pMediumFont);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

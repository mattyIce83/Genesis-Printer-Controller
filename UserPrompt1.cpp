// UserPrompt1.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "UserPrompt1.h"
#include "afxdialogex.h"
#include "Controller4Dlg1.h"


extern CController4App theApp;
extern CController4Dlg1 *theDlg;

// CUserPrompt1 dialog

IMPLEMENT_DYNAMIC(CUserPrompt1, CDialogEx)

CUserPrompt1::CUserPrompt1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserPrompt1::IDD, pParent)
{

}

CUserPrompt1::~CUserPrompt1()
{
}

void CUserPrompt1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
}


BEGIN_MESSAGE_MAP(CUserPrompt1, CDialogEx)
END_MESSAGE_MAP()


// CUserPrompt1 message handlers


BOOL CUserPrompt1::OnInitDialog()
{
	

	CDialogEx::OnInitDialog();

	m_Message.SetFont(&theDlg->pMediumFont);

	m_Message.SetWindowText(szMessage);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

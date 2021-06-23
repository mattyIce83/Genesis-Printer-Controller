// DripWhiteTime.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "DripWhiteTime.h"
#include "afxdialogex.h"


// CDripWhiteTime dialog

IMPLEMENT_DYNAMIC(CDripWhiteTime, CDialogEx)

CDripWhiteTime::CDripWhiteTime(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDripWhiteTime::IDD, pParent)
{

}

CDripWhiteTime::~CDripWhiteTime()
{
}

void CDripWhiteTime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIME, m_Time);
	DDX_Control(pDX, IDOK, m_GoButton);
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
}


BEGIN_MESSAGE_MAP(CDripWhiteTime, CDialogEx)
	ON_BN_CLICKED(IDC_SECONDS_PLUS, &CDripWhiteTime::OnBnClickedSecondsPlus)
	ON_BN_CLICKED(IDC_SECONDSMINUS, &CDripWhiteTime::OnBnClickedSecondsminus)
	ON_BN_CLICKED(IDOK, &CDripWhiteTime::OnBnClickedOk)
END_MESSAGE_MAP()


// CDripWhiteTime message handlers


void CDripWhiteTime::OnBnClickedSecondsPlus() {

	Seconds++;

	DisplayTime();


}


void CDripWhiteTime::OnBnClickedSecondsminus()
{
	if(Seconds > 1) {

		Seconds--;

		DisplayTime();

	}

}

void CDripWhiteTime::DisplayTime() {
char tmp[32];


	sprintf(tmp, "%d", Seconds);

	m_Time.SetWindowText(tmp);


}
BOOL CDripWhiteTime::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Message.SetWindowText(szMessage);

	m_GoButton.SetWindowText(szButtonText);

	DisplayTime();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDripWhiteTime::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

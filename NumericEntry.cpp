// NumericEntry.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "NumericEntry.h"
#include "Controller4Dlg1.h"
#include "afxdialogex.h"


// CNumericEntry dialog
extern CController4Dlg1 *theDlg;


IMPLEMENT_DYNAMIC(CNumericEntry, CDialogEx)

CNumericEntry::CNumericEntry(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNumericEntry::IDD, pParent)
{

}

CNumericEntry::~CNumericEntry()
{
}

void CNumericEntry::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OUTPUT, m_Output);
	DDX_Control(pDX, IDC_TITLE, m_Title);
}


BEGIN_MESSAGE_MAP(CNumericEntry, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNumericEntry::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNumericEntry::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_KB_1, &CNumericEntry::OnBnClickedKb1)
	ON_BN_CLICKED(IDC_KB_2, &CNumericEntry::OnBnClickedKb2)
	ON_BN_CLICKED(IDC_KB_3, &CNumericEntry::OnBnClickedKb3)
	ON_BN_CLICKED(IDC_KB_4, &CNumericEntry::OnBnClickedKb4)
	ON_BN_CLICKED(IDC_KB_5, &CNumericEntry::OnBnClickedKb5)
	ON_BN_CLICKED(IDC_KB_6, &CNumericEntry::OnBnClickedKb6)
	ON_BN_CLICKED(IDC_KB_7, &CNumericEntry::OnBnClickedKb7)
	ON_BN_CLICKED(IDC_KB_8, &CNumericEntry::OnBnClickedKb8)
	ON_BN_CLICKED(IDC_KB_9, &CNumericEntry::OnBnClickedKb9)
	ON_BN_CLICKED(IDC_KB_0, &CNumericEntry::OnBnClickedKb0)
END_MESSAGE_MAP()


// CNumericEntry message handlers

void CNumericEntry::HandleKey(char k) {

	szCode[iFieldCharIndex] = k;

	if(bIsPassword) szDisplay[iFieldCharIndex] = 0x95;
	else szDisplay[iFieldCharIndex] = k;

	m_Output.SetWindowText(szDisplay);

	iFieldCharIndex++;

	if(iFieldCharIndex >= iNumberOfCharsExpected) {
		Sleep(500);
		OnOK();
	}


}

void CNumericEntry::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CNumericEntry::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CNumericEntry::OnBnClickedKb1()
{
	HandleKey('1');
}


void CNumericEntry::OnBnClickedKb2()
{
	HandleKey('2');
}


void CNumericEntry::OnBnClickedKb3()
{
	HandleKey('3');
}


void CNumericEntry::OnBnClickedKb4()
{
	HandleKey('4');
}


void CNumericEntry::OnBnClickedKb5()
{
	HandleKey('5');
}


void CNumericEntry::OnBnClickedKb6()
{
	HandleKey('6');
}


void CNumericEntry::OnBnClickedKb7()
{
	HandleKey('7');
}


void CNumericEntry::OnBnClickedKb8()
{
	HandleKey('8');
}


void CNumericEntry::OnBnClickedKb9()
{
	HandleKey('9');
}


void CNumericEntry::OnBnClickedKb0()
{
	HandleKey('0');
}


BOOL CNumericEntry::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	iFieldCharIndex = 0;
	memset(szCode, 0, 128);
	memset(szDisplay, 0, 128);

	m_Title.SetWindowText(szTitle);

	if(bIsPassword)
		m_Output.SetFont(&theDlg->pLargeFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

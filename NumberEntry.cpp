// NumberEntry.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "NumberEntry.h"
#include "afxdialogex.h"


static 	CFont pNewFont;
static 	CFont pNewFont2;

// CNumberEntry dialog

IMPLEMENT_DYNAMIC(CNumberEntry, CDialogEx)

CNumberEntry::CNumberEntry(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNumberEntry::IDD, pParent)
{

}

CNumberEntry::~CNumberEntry()
{
}

void CNumberEntry::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BS, m_BS);
	DDX_Control(pDX, IDC_EIGHT, m_EIGHT);
	DDX_Control(pDX, IDC_FIVE, m_FIVE);
	DDX_Control(pDX, IDC_FOUR, m_FOUR);
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
	DDX_Control(pDX, IDC_NINE, m_NINE);
	DDX_Control(pDX, IDC_ONE, m_ONE);
	DDX_Control(pDX, IDC_OUTPUT, m_Output);
	DDX_Control(pDX, IDC_SEVEN, m_SEVEN);
	DDX_Control(pDX, IDC_SIX, m_SIX);
	DDX_Control(pDX, IDC_SPARE, m_SP);
	DDX_Control(pDX, IDC_THREE, m_THREE);
	DDX_Control(pDX, IDC_TWO, m_TWO);
	DDX_Control(pDX, IDC_ZERO, m_ZERO);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_CANCEL);
}


BEGIN_MESSAGE_MAP(CNumberEntry, CDialogEx)
	ON_BN_CLICKED(IDC_THREE, &CNumberEntry::OnBnClickedThree)
	ON_BN_CLICKED(IDC_ONE, &CNumberEntry::OnBnClickedOne)
	ON_BN_CLICKED(IDC_TWO, &CNumberEntry::OnBnClickedTwo)
	ON_BN_CLICKED(IDC_FOUR, &CNumberEntry::OnBnClickedFour)
	ON_BN_CLICKED(IDC_FIVE, &CNumberEntry::OnBnClickedFive)
	ON_BN_CLICKED(IDC_SIX, &CNumberEntry::OnBnClickedSix)
	ON_BN_CLICKED(IDC_SEVEN, &CNumberEntry::OnBnClickedSeven)
	ON_BN_CLICKED(IDC_EIGHT, &CNumberEntry::OnBnClickedEight)
	ON_BN_CLICKED(IDC_NINE, &CNumberEntry::OnBnClickedNine)
	ON_BN_CLICKED(IDC_ZERO, &CNumberEntry::OnBnClickedZero)
	ON_BN_CLICKED(IDC_SPARE, &CNumberEntry::OnBnClickedSpare)
	ON_BN_CLICKED(IDC_BS, &CNumberEntry::OnBnClickedBs)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDOK, &CNumberEntry::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNumberEntry::OnBnClickedCancel)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CNumberEntry message handlers


void CNumberEntry::OnBnClickedThree()
{
	strcat(content, "3");
	m_Output.SetWindowText(content);
}


void CNumberEntry::OnBnClickedOne()
{
	strcat(content, "1");
	m_Output.SetWindowText(content);
}


void CNumberEntry::OnBnClickedTwo()
{
	strcat(content, "2");
	m_Output.SetWindowText(content);
}


void CNumberEntry::OnBnClickedFour()
{
	strcat(content, "4");
	m_Output.SetWindowText(content);
}


void CNumberEntry::OnBnClickedFive()
{
	strcat(content, "5");
	m_Output.SetWindowText(content);
}


void CNumberEntry::OnBnClickedSix()
{
	strcat(content, "6");
	m_Output.SetWindowText(content);
}


void CNumberEntry::OnBnClickedSeven()
{
	strcat(content, "7");
	m_Output.SetWindowText(content);
}


void CNumberEntry::OnBnClickedEight()
{
	strcat(content, "8");
	m_Output.SetWindowText(content);
}


void CNumberEntry::OnBnClickedNine()
{
	strcat(content, "9");
	m_Output.SetWindowText(content);
}


void CNumberEntry::OnBnClickedZero()
{
	strcat(content, "0");
	m_Output.SetWindowText(content);
}


void CNumberEntry::OnBnClickedSpare()
{
	// TODO: Add your control notification handler code here
}


void CNumberEntry::OnBnClickedBs() {
int l;

	l = strlen(content);
	if(l) {
		l--;
		content[l] = 0;
		m_Output.SetWindowText(content);
	}

}


BOOL CNumberEntry::OnInitDialog() {
int result;	
	
	content[0] = 0;

	result = m_ONE.AutoLoad(IDC_ONE, this);
	result = m_TWO.AutoLoad(IDC_TWO, this);
	result = m_THREE.AutoLoad(IDC_THREE, this);
	result = m_FOUR.AutoLoad(IDC_FOUR, this);
	result = m_FIVE.AutoLoad(IDC_FIVE, this);
	result = m_SIX.AutoLoad(IDC_SIX, this);
	result = m_SEVEN.AutoLoad(IDC_SEVEN, this);
	result = m_EIGHT.AutoLoad(IDC_EIGHT, this);
	result = m_NINE.AutoLoad(IDC_NINE, this);
	result = m_ZERO.AutoLoad(IDC_ZERO, this);
	result = m_BS.AutoLoad(IDC_BS, this);
	result = m_OK.AutoLoad(IDOK, this);
	result = m_CANCEL.AutoLoad(IDCANCEL, this);


	CDialogEx::OnInitDialog();

	CFont* pOldFont = m_Output.GetFont();

	LOGFONT logfont, logfont2;

	pOldFont->GetObject(sizeof(logfont),&logfont);
 
	logfont.lfHeight = 20;
	logfont.lfWidth = 7;
 
	logfont.lfWeight = FW_HEAVY;
 
	logfont.lfPitchAndFamily = FF_DONTCARE;
 
	strcpy(logfont.lfFaceName, "Helvetica");  
 
	logfont.lfEscapement = logfont.lfOrientation = 0;
 
	memcpy((char *)&logfont2, (char *)&logfont, sizeof(LOGFONT));

	pNewFont.CreateFontIndirect(&logfont);

	logfont2.lfHeight = 30;
	logfont2.lfWidth = 10;

	pNewFont2.CreateFontIndirect(&logfont2);

	m_Message.SetFont(&pNewFont);

	m_Output.SetFont(&pNewFont2);

	m_Message.SetWindowText(szMessage);

	strcat(content, "1");
	m_Output.SetWindowText(content);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CNumberEntry::OnSetFocus(CWnd* pOldWnd) {

	m_Message.SetFont(&pNewFont);

	m_Output.SetFont(&pNewFont);

	CDialogEx::OnSetFocus(pOldWnd);


}


void CNumberEntry::OnBnClickedOk() {

	strcpy(szOutput, content);

	pNewFont.DeleteObject();
	pNewFont2.DeleteObject();

	CDialogEx::OnOK();
}


void CNumberEntry::OnBnClickedCancel()
{
	

	pNewFont.DeleteObject();
	pNewFont2.DeleteObject();

	CDialogEx::OnCancel();
}


void CNumberEntry::OnPaint() {

	CPaintDC dc(this); // device context for painting

	theApp.DisplayBitmap(&dc, &theApp.hNumberBackground, 0, 0, DB_SHOWUPPERLEFT);


}

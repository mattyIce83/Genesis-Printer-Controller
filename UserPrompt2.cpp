// UserPrompt2.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "UserPrompt2.h"
#include "afxdialogex.h"


// CUserPrompt2 dialog

IMPLEMENT_DYNAMIC(CUserPrompt2, CDialogEx)

CUserPrompt2::CUserPrompt2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserPrompt2::IDD, pParent)
{

}

CUserPrompt2::~CUserPrompt2()
{
}

void CUserPrompt2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_1, m_Button_1);
	DDX_Control(pDX, IDC_BUTTON_2, m_Button_2);
	DDX_Control(pDX, IDC_BUTTON_3, m_Button_3);
	DDX_Control(pDX, IDC_BUTTON_4, m_Button_4);
	DDX_Control(pDX, IDC_ILLUSTRATION, m_Illustration);
	DDX_Control(pDX, IDC_NEXT_ILLUSTRATION, m_Next);
	DDX_Control(pDX, IDC_PREV_ILLUSTRATION, m_Prev);
	DDX_Control(pDX, IDC_DIALOG_TITLE, m_DialogTitle);
}


BEGIN_MESSAGE_MAP(CUserPrompt2, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_1, &CUserPrompt2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CUserPrompt2::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CUserPrompt2::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CUserPrompt2::OnBnClickedButton4)
	ON_BN_CLICKED(IDOK, &CUserPrompt2::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PREV_ILLUSTRATION, &CUserPrompt2::OnBnClickedPrevIllustration)
	ON_BN_CLICKED(IDC_NEXT_ILLUSTRATION, &CUserPrompt2::OnBnClickedNextIllustration)
END_MESSAGE_MAP()


// CUserPrompt2 message handlers


BOOL CUserPrompt2::OnInitDialog() {
int i, nRet;

	bitmaps = 0;

	CDialogEx::OnInitDialog();

	dcScreen = m_Illustration.GetDC();

	nRetCode = -1;

	iCurrentIllustration = 0;

	m_Illustration.GetClientRect(&inRect);
	m_Illustration.GetClientRect(&crect);

	dcScreen = m_Illustration.GetDC();

	for(i = 0; i < 4; i++) {
		if(strlen(szIllustrationFile[i])) {
			nRet = L_LoadBitmap (szIllustrationFile[i], &Bitmap[i], sizeof(BITMAPHANDLE), 24, ORDER_BGR, NULL, NULL);
			if(nRet == SUCCESS) {
				bitmaps++;
			}
		}
	}

	if(bitmaps > 1) {
		m_Next.ShowWindow(SW_SHOW);
		m_Prev.ShowWindow(SW_SHOW);
	}

	if(strlen(szDialogTitle)) {
		m_DialogTitle.SetWindowText(szDialogTitle);
		m_DialogTitle.ShowWindow(SW_SHOW);
	}

	if(strlen(szButtonText[0])) {
		m_Button_1.SetWindowText(szButtonText[0]);
		m_Button_1.ShowWindow(SW_SHOW);
	}

	if(strlen(szButtonText[1])) {
		m_Button_2.SetWindowText(szButtonText[1]);
		m_Button_2.ShowWindow(SW_SHOW);
	}

	if(strlen(szButtonText[2])) {
		m_Button_3.SetWindowText(szButtonText[2]);
		m_Button_3.ShowWindow(SW_SHOW);
	}

	if(strlen(szButtonText[3])) {
		m_Button_4.SetWindowText(szButtonText[3]);
		m_Button_4.ShowWindow(SW_SHOW);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CUserPrompt2::OnBnClickedButton1()
{

	CleanUpAndExit(1);
}


void CUserPrompt2::OnBnClickedButton2()
{
	CleanUpAndExit(2);
}


void CUserPrompt2::OnBnClickedButton3()
{
	CleanUpAndExit(3);
}


void CUserPrompt2::OnBnClickedButton4()
{
	CleanUpAndExit(4);
}


void CUserPrompt2::OnBnClickedOk()
{

	CDialogEx::OnOK();
}

void CUserPrompt2::CleanUpAndExit(int exitcode) {
int i;

	for(i = 0; i < 4; i++) {

		if(Bitmap[i].Flags.Allocated) {
			L_FreeBitmap(&Bitmap[i]);
		}

	}

	nRetCode = exitcode;

	CDialogEx::OnOK();

}


void CUserPrompt2::OnPaint() {
int ret;

	CPaintDC dc(this); // device context for painting

	if(Bitmap[iCurrentIllustration].Flags.Allocated) {

		ret = L_PaintDC (	dcScreen->m_hDC,
							&Bitmap[iCurrentIllustration],
							&inRect,
							NULL,
							&crect,
							&crect,
							SRCCOPY);

		/**/ TRACE("Painting %s\n", szIllustrationFile[iCurrentIllustration]);

	}

}



void CUserPrompt2::OnBnClickedPrevIllustration() {

	if(iCurrentIllustration > 0) {
		iCurrentIllustration--;
		//RECT cr;
		//m_Illustration.GetClientRect(&cr);
		//m_Illustration.InvalidateRect(&cr);
		//m_Illustration.UpdateWindow();
		m_Illustration.Invalidate();
		m_Illustration.UpdateWindow();
		Invalidate();
	}

}


void CUserPrompt2::OnBnClickedNextIllustration() {
	
	if(iCurrentIllustration < (bitmaps - 1)) {
		iCurrentIllustration++;
		//RECT cr;
		//m_Illustration.GetClientRect(&cr);
		//m_Illustration.InvalidateRect(&cr);
		//m_Illustration.Invalidate();
		//m_Illustration.UpdateWindow();
		Invalidate();
	}

}

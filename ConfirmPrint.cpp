// ConfirmPrint.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "ConfirmPrint.h"
#include "afxdialogex.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg2.h"
#include "Controller4Dlg3.h"
#include "afxdialogex.h"

extern CController4App theApp;
extern CController4Dlg1 *theDlg;


// CConfirmPrint dialog

IMPLEMENT_DYNAMIC(CConfirmPrint, CDialogEx)

CConfirmPrint::CConfirmPrint(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfirmPrint::IDD, pParent)
{

}

CConfirmPrint::~CConfirmPrint()
{
}

void CConfirmPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_No);
	DDX_Control(pDX, IDOK, m_Yes);
	DDX_Control(pDX, IDC_TITLE, m_Title);
	DDX_Control(pDX, IDC_PRINTPREVIEW, m_Preview);
	DDX_Control(pDX, IDC_RES_DISP, m_DispRes);
	DDX_Control(pDX, IDC_PRN_PARAMS, m_PrnParams);
	DDX_Control(pDX, IDC_JOBNAME, m_JobName);
}


BEGIN_MESSAGE_MAP(CConfirmPrint, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConfirmPrint::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CConfirmPrint message handlers


BOOL CConfirmPrint::OnInitDialog() {

	CDialogEx::OnInitDialog();

	m_Yes.SetFont(&theDlg->pMediumFont);
	m_No.SetFont(&theDlg->pMediumFont);
	m_Title.SetFont(&theDlg->pMediumFont);
	m_DispRes.SetFont(&theDlg->pMediumFont);
	m_PrnParams.SetFont(&theDlg->pMediumFont);
	m_JobName.SetFont(&theDlg->pMediumFont);

	char message[256];

	sprintf(message, "%s", qi->JobName);
	m_JobName.SetWindowText(message);

	message[0] = 0;
	if(qi->resolution.x == 600 && qi->resolution.y == 600) {
		strcpy(message, "Normal Qlty, ");
	}
	if(qi->resolution.x == 600 && qi->resolution.y == 1200) {
		strcpy(message, "Fine Qlty, ");
	}

	m_DispRes.SetWindowText(message);


	InitialSetup();

	if((theApp.GVars->CurHeadTemp[0] < theApp.GVars->HeadTemp[0] && ((theApp.GVars->HeadTemp[0] - theApp.GVars->CurHeadTemp[0]) > 5)) ||
		(theApp.GVars->CurHeadTemp[1] < theApp.GVars->HeadTemp[1] && ((theApp.GVars->HeadTemp[1] - theApp.GVars->CurHeadTemp[1]) > 5))) {

		//CPaintDC dc(this); // device context for painting

			if(theApp.GVars->bUseHeaters) DisplayTempWarning(NULL);
		
		/*RECT rc;
		GetClientRect(&rc);
		rc.top += 60;
		rc.left += 160;
		rc.right = rc.left + 350;
		rc.bottom = rc.top + 70;

		InvalidateRect(&rc, FALSE);
		*/
	
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CConfirmPrint::DisplayTempWarning(CPaintDC *dc) {

	char temp_txt[64];
	sprintf(temp_txt, "A:%d%c F .. B:%d%c F", theApp.GVars->CurHeadTemp[0], 0xB0, theApp.GVars->CurHeadTemp[1], 0xB0);

	theDlg->OKBox("HEAD TEMPERATURE", "WARNING", temp_txt);

	/*
	RECT rc;
	char temp_txt[64];
	GetClientRect(&rc);
	rc.top += 60;
	rc.left += 160;
	rc.right = rc.left + 350;
	rc.bottom = rc.top + 70;
	dc->SelectObject(&theDlg->pMediumFont);
	dc->SetTextColor(RGB(255, 0, 0));
	dc->SetBkMode(TRANSPARENT);
	sprintf(temp_txt, "TEMP WARNING: A:%d%c F .. B:%d%c F", theApp.GVars->CurHeadTemp[0], 0xB0, theApp.GVars->CurHeadTemp[1], 0xB0);
	dc->DrawText(temp_txt, strlen(temp_txt), &rc, DT_LEFT|DT_END_ELLIPSIS);
	*/

}

void CConfirmPrint::InitialSetup() {

	HANDLE fh;

	fh = CreateFile(m_szJobFile,
					GENERIC_READ,
					FILE_SHARE_READ,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
  					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);

	if(fh == INVALID_HANDLE_VALUE) {

		bHasImage = FALSE;
		return;

	}

	int size, nRet, i, ofst;
	char smallbuff[256], *tptr, *sptr = (char *)&size;
	unsigned long bytes;

	BOOL found = FALSE;

	ReadFile(fh, (char *)smallbuff, 256, &bytes, NULL);

	for(i = 0; i < 256; i++) {
		if( (smallbuff[i] == 0x1B &&
			smallbuff[i + 1] == 0x40 &&
			smallbuff[i + 2] == 0x50) ||

			(smallbuff[i] == 0x1B &&
			smallbuff[i + 1] == 0x40 &&
			smallbuff[i + 2] == 'W')) {

			found = TRUE;
			ofst = i + 3;
			break;

		}

	}

	if(!found)  {
		bHasImage = FALSE;
		return;
	}

	tptr = smallbuff + ofst;

	sptr[0] = tptr[0];
	sptr[1] = tptr[1];
	sptr[2] = tptr[2];
	sptr[3] = tptr[3];
	

	SetFilePointer(	fh,
					ofst,
					NULL,
					FILE_BEGIN);

	unsigned char *pbuff = (unsigned char *)calloc(1, size + 16);

	ReadFile(fh, pbuff, size + 4, &bytes, NULL);


	CloseHandle(fh);

	nRet = L_LoadBitmapMemory(pbuff, &Bitmap, sizeof(BITMAPHANDLE), 24, ORDER_BGR, size, NULL, NULL);

	if(nRet != SUCCESS) {
		nRet = L_LoadBitmapMemory(pbuff + 4, &Bitmap, sizeof(BITMAPHANDLE), 24, ORDER_BGR, size, NULL, NULL);
	}

	free(pbuff);

	PreparePage();

	bHasImage = TRUE;


}

void CConfirmPrint::PreparePage() {

int ret;

	WINDOWPLACEMENT wp;

	m_Preview.GetWindowPlacement(&wp);

	m_Preview.GetClientRect(&crect);

	crect.right = crect.left + (wp.rcNormalPosition.right - wp.rcNormalPosition.left);

	float f;

	if(Bitmap.Width > Bitmap.Height) {
		f = (float)Bitmap.Height / (float)Bitmap.Width;
		crect.bottom = crect.right * f;
	}
	else {
		f = (float)Bitmap.Width / (float)Bitmap.Height;
		crect.right = crect.bottom * f;
	}

	dcScreen = m_Preview.GetDC();

	SetRect(&inRect, 0, 0, Bitmap.Width, Bitmap.Height);

	wp.rcNormalPosition.right = wp.rcNormalPosition.left + (crect.right - crect.left);
	wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + (crect.bottom - crect.top);
	wp.rcNormalPosition.left -= 4;
	wp.rcNormalPosition.right += 4;
	wp.rcNormalPosition.top -= 4;
	wp.rcNormalPosition.bottom += 4;
	
	m_Preview.SetWindowPlacement(&wp);

	memcpy((char *)&clrect, &crect, sizeof(RECT));

	crect.left += 7;
	crect.top += 7;

}

void CConfirmPrint::DisplayPage() {
int ret;

	if(!bHasImage) return;

	
	RECT crop_rect1;
	RECT crop_rect2;

	SetRect(&crop_rect1, clrect.left, clrect.top, clrect.right, clrect.bottom);

	crop_rect1.left += 4;
	crop_rect1.top += 4;

	ret = L_PaintDC (	dcScreen->m_hDC,
				&Bitmap,
				&inRect,
				NULL,
				&crect,
				&crop_rect1,
				SRCCOPY);

}


void CConfirmPrint::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CConfirmPrint::OnPaint() {

	CPaintDC dc(this); // device context for painting

	DisplayPage();

	if((theApp.GVars->CurHeadTemp[0] < theApp.GVars->HeadTemp[0] && ((theApp.GVars->HeadTemp[0] - theApp.GVars->CurHeadTemp[0]) > 3)) ||
		(theApp.GVars->CurHeadTemp[1] < theApp.GVars->HeadTemp[1] && ((theApp.GVars->HeadTemp[1] - theApp.GVars->CurHeadTemp[1]) > 3))) {

		CPaintDC dc(this); // device context for painting

		// DisplayTempWarning(&dc);	
	
	}


}

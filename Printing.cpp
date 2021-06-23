// Printing.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg4.h"
#include "Printing.h"
#include "afxdialogex.h"
#include "SerialPort.h"
#include "SerialSupp.h"
#include "ftd2xx.h"
#include "ftdisupp.h"
//#define NO_DIFF_GS
#include "graphics_state.h"
#include "HEAD_structs.h"


extern FTDIComm FPGAusb;
extern FTDIComm AUXusb;

extern CController4App theApp;
extern CController4Dlg1 *theDlg;
extern COM_SETTINGS com_settings[2];
extern void FTDPurge(FTDIComm *);
extern void USBPrinterCommand(unsigned long , unsigned long);
extern void FreeWeaveBuffers();

CPrinting *pdlg;

static int hits, state, hits2, cancelling_displayed;

// CPrinting dialog

IMPLEMENT_DYNAMIC(CPrinting, CDialogEx)

CPrinting::CPrinting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPrinting::IDD, pParent)
{

}

CPrinting::~CPrinting()
{
}

void CPrinting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOME, m_Home);
	DDX_Control(pDX, IDC_JOBINFO, m_JobInfo);
	DDX_Control(pDX, IDC_ESTOP, m_EStop);
	DDX_Control(pDX, IDC_CONTINUEPRINT, m_ContinuePrint);
	DDX_Control(pDX, IDC_CLEANPRINT, m_CleanPrint);
	DDX_Control(pDX, IDC_CANCELPRINT, m_CancelPrint);
	DDX_Control(pDX, IDC_NUMBER, m_JobNumber);
	DDX_Control(pDX, IDC_PRINTMODE, m_PrintMode);
	DDX_Control(pDX, IDC_PRINT_SIZE, m_Size);
	DDX_Control(pDX, IDC_PRINTPREVIEW, m_Preview);
	DDX_Control(pDX, IDC_PERCENT_DONE, m_Percent);
	DDX_Control(pDX, IDC_WARNING_BOX, m_WarningBox);
	DDX_Control(pDX, IDC_CANCELLING_JOB, m_Cancelling);
}


BEGIN_MESSAGE_MAP(CPrinting, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDOK, &CPrinting::OnBnClickedOk)
	ON_BN_CLICKED(IDC_HOME, &CPrinting::OnBnClickedHome)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ESTOP, &CPrinting::OnBnClickedEstop)
	ON_BN_CLICKED(IDC_CANCELPRINT, &CPrinting::OnBnClickedCancelprint)
	ON_BN_CLICKED(IDC_CONTINUEPRINT, &CPrinting::OnBnClickedContinueprint)
	ON_BN_CLICKED(IDC_CLEANPRINT, &CPrinting::OnBnClickedCleanprint)
END_MESSAGE_MAP()


// CPrinting message handlers


BOOL CPrinting::OnInitDialog() {

	Running = FALSE;
	theApp.ClosePrintingWindow = FALSE;
	theApp.NoMoreCancel = FALSE;
	theApp.bProlongPrintWindow = FALSE;
	theApp.bCancelRequested = FALSE;

	bInfoAvailable = FALSE;
	bHasInfo = FALSE;
	m_Home.AutoLoad(IDC_HOME, this);
	hits = state = hits2 = cancelling_displayed = 0;
	bEStopDisplayTimer = 0;

	m_EStop.AutoLoad(IDC_ESTOP, this);
	m_ContinuePrint.AutoLoad(IDC_CONTINUEPRINT, this);
	m_CleanPrint.AutoLoad(IDC_CLEANPRINT, this);
	m_CancelPrint.AutoLoad(IDC_CANCELPRINT, this);

	CDialogEx::OnInitDialog();

	pdlg = this;

	m_JobInfo.SetFont(&theDlg->pMediumFont);
	m_JobNumber.SetFont(&theDlg->pMediumFont);
	m_PrintMode.SetFont(&theDlg->pMediumFont);
	m_Size.SetFont(&theDlg->pMediumFont);
	m_Percent.SetFont(&theDlg->pMediumFont);

	m_JobInfo.SetBkColor(RGB(13,13,13));
	m_JobNumber.SetBkColor(RGB(13,13,13));
	m_PrintMode.SetBkColor(RGB(13,13,13));
	m_Size.SetBkColor(RGB(13,13,13));
	//m_Percent.SetBkColor(RGB(137,137,137));
	m_Percent.SetBkColor(RGB(0,0,0));

	m_ContinuePrint.ShowWindow(SW_HIDE);
	m_CleanPrint.ShowWindow(SW_HIDE);
	m_CancelPrint.ShowWindow(SW_HIDE);

	SizeDialog();

	//if(bHasInfo) {
	if(qi != NULL) {
		bHasInfo = TRUE;
		InitialSetup();
	}
	
	if(theApp.GPlatenPosition != 2) {
		bEStopShown = FALSE;
		m_EStop.ShowWindow(SW_HIDE);
	}
	else {
		bEStopShown = TRUE;
		m_EStop.ShowWindow(SW_SHOW);
		/**/ TRACE("Show EButton: %s %d\n", __FILE__, __LINE__);
	}

	m_nTimer = SetTimer(PRINT_DIALOG_TIMER_EVENT, 250, 0);
	theApp.PrintingWindow = TRUE;
	Running = TRUE;


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPrinting::InitialSetup() {

	m_JobInfo.SetWindowText(m_szJobName);
	m_JobNumber.SetWindowText(m_szJumber);
	m_PrintMode.SetWindowText(m_szPrintMode);
	m_Size.SetWindowText(m_szSize);

	HANDLE fh;

	fh = CreateFile(m_szJobFile,
					GENERIC_READ,
					FILE_SHARE_READ,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
  					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);

	if(fh == INVALID_HANDLE_VALUE) {

		theDlg->OKBox("Error Opening", m_szJobFile, "");
		theApp.ClosePrintingWindow = TRUE;
		//theApp.GMasterStatus = MASTER_STATUS_ABORTING;
		theApp.GMasterStatus = MASTER_STATUS_IDLE;
		CController4Dlg4 dl;
		dl.DeleteQueueItem(qi, TRUE);
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

	nRet = L_CopyBitmap(&BitmapLight, &Bitmap, sizeof(BITMAPHANDLE));

	if(qi->bIsWhite) {
		nRet = L_ConvertToColoredGray(&BitmapLight, 300, 590, 11, 500, 300, 200);
		nRet = L_InvertBitmap(&BitmapLight);
		nRet = L_ContBrightIntBitmap (&BitmapLight, 0, 500, 0); 
	}
	else {
		nRet = L_ConvertToColoredGray(&BitmapLight, 300, 590, 11, 500, 300, 200);
		nRet = L_ContBrightIntBitmap (&BitmapLight, 0, 500, 0); 
	}

	PreparePage();

}

void CPrinting::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);



}


void CPrinting::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
}


void CPrinting::OnDestroy()
{

	if(theApp.GMasterStatus != MASTER_STATUS_ABORTING)
		theApp.RunWaitDialog = FALSE;

	/**/ TRACE("Runwait false: %s %d\n", __FILE__, __LINE__);

	KillTimer(m_nTimer);

	pdlg = NULL;

	Running = FALSE;

	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}


void CPrinting::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
}


void CPrinting::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CPrinting::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	theApp.DisplayBitmap(&dc, &theApp.hBackgroundPrinting, 0, 0, DB_SHOWUPPERLEFT);

	DisplayPage();

	if(com_settings[FPGA_BACKCHANNEL].active) {
		RECT rc;
		char temp_txt[16];
		GetClientRect(&rc);
		rc.top += 50;
		dc.SelectObject(&theDlg->pMediumFont);
		dc.SetTextColor(RGB(2255, 255, 255));
		dc.SetBkMode(TRANSPARENT);
		sprintf(temp_txt, "Tmp A: %d B: %d", theApp.GVars->CurHeadTemp[0], theApp.GVars->CurHeadTemp[1]);
		dc.DrawText(temp_txt, strlen(temp_txt), &rc, DT_CENTER|DT_END_ELLIPSIS);
	}

}


void CPrinting::OnSetFocus(CWnd* pOldWnd)
{
	CDialogEx::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}


void CPrinting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CPrinting::OnBnClickedHome()
{
	if(Bitmap.Flags.Allocated)
		L_FreeBitmap(&Bitmap);
	if(BitmapLight.Flags.Allocated)
		L_FreeBitmap(&BitmapLight);

	//theApp.bHomePressed = TRUE;
	KillTimer(m_nTimer);
	pdlg = NULL;
	CDialogEx::OnCancel();
}

/*-------------------------------------------------------------------------------------*/
/*-                                                                                   -*/
/*-------------------------------------------------------------------------------------*/
void PrintingDialogLoop(void *p) {
CPrinting dlg;
parserIOStruct *ios;
char tmp_buff[128];

	/**/ TRACE("PrintingDialogLoop top\n");

	ios = (parserIOStruct *)p;
	dlg.qi = (QUEUE_ITEM *)ios->qitem;
	dlg.bSizeAvailable = FALSE;
	if(dlg.qi != NULL) {
		sprintf(dlg.m_szJobName, "Job: %s", dlg.qi->JobName);
		sprintf(dlg.m_szJumber, "Print Dir: %s  / ID: %d", dlg.qi->iPrintDirection == 0?"Uni-Direct.":"Bi-Direct.", dlg.qi->RipJobNumber);
		if(dlg.qi->resolution.x == 600 && dlg.qi->resolution.y == 600)
			sprintf(dlg.m_szPrintMode, "Quality: Fast / Mode: %s / Density: %s", dlg.qi->iDualCMYK == 1?"CMYK X 2":"CMYK", dlg.qi->iPrintMode == 0?"Normal":"Double");
		else if(dlg.qi->resolution.x == 600 && dlg.qi->resolution.y == 1200)
			sprintf(dlg.m_szPrintMode, "Quality: Fine / Mode: %s / Density: %s", dlg.qi->iDualCMYK == 1?"CMYK X 2":"CMYK", dlg.qi->iPrintMode == 0?"Normal":"Double");
		else
			sprintf(dlg.m_szPrintMode, "Quality:  %d x %d / Mode: %s / Density: %s", dlg.qi->resolution.x, dlg.qi->resolution.y, dlg.qi->iDualCMYK == 1?"CMYK X 2":"CMYK", dlg.qi->iPrintMode == 0?"Normal":"Double");
		sprintf(dlg.m_szSize, "Size: %.02f x %.02f", ios->sizeX, ios->sizeY);
		strcpy(dlg.m_szJobFile, dlg.qi->szSourceFile);
		dlg.bHasInfo = TRUE;
	}
	else {
		dlg.bHasInfo = FALSE;
	}

	/**/ TRACE("PrintingDialogLoop middle\n");

	dlg.DoModal();

	while(dlg.Running) {
		Sleep(0);
	}

	theApp.PrintingWindow = FALSE;

	/**/ TRACE("PrintingDialogLoop end\n");

}

void CPrinting::OnTimer(UINT_PTR nIDEvent) {

	if(nIDEvent == PRINT_DIALOG_TIMER_EVENT) {

		if(bEStopDisplayTimer && theApp.iPrintedBandCount >= MINIMUM_BANDS_BEFORE_STOPBUTTON) {
	 		bEStopDisplayTimer = 0;
			bEStopShown = TRUE;
			m_EStop.ShowWindow(SW_SHOW);
			/**/ TRACE("Show EButton: %s %d\n", __FILE__, __LINE__);
		}

		/*
		if(!bEStopShown && 
			theApp.GMasterStatus == MASTER_STATUS_PRINTING &&
			theApp.GPlatenPosition == 2 &&
			!theApp.NoMoreCancel) {
	 		bEStopShown = TRUE;
			m_EStop.ShowWindow(SW_SHOW);
			// TRACE("Show EButton: %s %d\n", __FILE__, __LINE__);
		}
		*/

		if(theApp.NoMoreCancel) {
			//theApp.NoMoreCancel = FALSE;
			m_EStop.ShowWindow(SW_HIDE);
		}
	
		if(theApp.GMasterStatus == MASTER_STATUS_IDLE || theApp.GMasterStatus == MASTER_STATUS_PRINTING || theApp.GMasterStatus == MASTER_STATUS_ABORTING/* || theApp.GMasterStatus == MASTER_STATUS_CANCELLING*/) {

			if(theApp.ClosePrintingWindow) {
				m_EStop.ShowWindow(SW_HIDE);
				if(!theApp.bProlongPrintWindow) {
					OnBnClickedHome();
					//FreeWeaveBuffers(); // 9/29/16 // doesn't help
				}


			}


		}

		if((theApp.GMasterStatus == MASTER_STATUS_CANCELLING || theApp.GMasterStatus == MASTER_STATUS_ABORTING) && !cancelling_displayed) {

			cancelling_displayed = 1;

			//m_Cancelling.ShowWindow(SW_SHOW);
			theApp.StartWaitDialog("CANCELLING...");
			
		}

		if(theApp.bObstruction && !cancelling_displayed) {
			hits2++;
			if(hits2 > 1) {
				state = !state;
				if(state) {
					m_WarningBox.ShowWindow(SW_SHOW);
				}
				else {
					m_WarningBox.ShowWindow(SW_HIDE);
				}
				hits2 = 0;
			}
		}
		else {
			if(state) {
					m_WarningBox.ShowWindow(SW_HIDE);
					state = 0;
			}
		}

		if(!bHasInfo && qi != NULL && bInfoAvailable) {
			sprintf(m_szJobName, "Job: %s", qi->JobName);
			//sprintf(m_szJumber, "Number: %d", qi->JobNumber);
			sprintf(m_szJumber, "ID: %d", qi->RipJobNumber);

			if(qi->resolution.x == 600 && qi->resolution.y == 600)
				sprintf(m_szPrintMode, "Mode: Fast");
			else if(qi->resolution.x == 600 && qi->resolution.y == 1200)
				sprintf(m_szPrintMode, "Mode: Fine");
			else
				sprintf(m_szPrintMode, "Mode: %d x %d", qi->resolution.x, qi->resolution.y);

			InitialSetup();
			bHasInfo = TRUE;
		}

		if(bSizeAvailable) {
			bSizeAvailable = FALSE;
			sprintf(m_szSize, "Size: %.02f x %.02f", sizeX, sizeY);
			m_Size.SetWindowText(m_szSize);
			m_Size.Invalidate();
		}


	}


	CDialogEx::OnTimer(nIDEvent);
}

void CPrinting::PreparePage() {

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

	WINDOWPLACEMENT pcwp;

	m_Percent.GetWindowPlacement(&pcwp);
	int l, r;
	l = wp.rcNormalPosition.left + (((wp.rcNormalPosition.right - wp.rcNormalPosition.left) - (pcwp.rcNormalPosition.right - pcwp.rcNormalPosition.left)) / 2);
	r = l + pcwp.rcNormalPosition.right - pcwp.rcNormalPosition.left;
	pcwp.rcNormalPosition.left = l;
	pcwp.rcNormalPosition.right = r;
	m_Percent.SetWindowPlacement(&pcwp);


}

void CPrinting::DisplayPage() {
int ret;

	if(Running != 1) // yes, it has to be "1"
		return;

	if(!Bitmap.Flags.Allocated)
		return;

	if(bHasInfo) {

		RECT crop_rect1;
		RECT crop_rect2;

		SetRect(&crop_rect1, clrect.left, clrect.top + printed_rasters, clrect.right, clrect.bottom);

		crop_rect1.left += 4;
		crop_rect1.top += 4;


		SetRect(&crop_rect2, clrect.left, clrect.top, clrect.right, clrect.bottom - unprinted_rasters);

		crop_rect2.left += 4;
		crop_rect2.top += 4;


		ret = L_PaintDC (	dcScreen->m_hDC,
					&Bitmap,
					&inRect,
					NULL,
					&crect,
					&crop_rect2,
					SRCCOPY);

		ret = L_PaintDC (	dcScreen->m_hDC,
					&BitmapLight,
					&inRect,
					NULL,
					&crect,
					&crop_rect1,
					SRCCOPY);	/* Normal Paint */

					
	}

}

void CPrinting::UpdatePreviewImage(float percent, void *vgs) {
int total_rasters;
RECT trect;
char msg_buff[25];
displayGraphicState *gs = (displayGraphicState *)vgs;


	if(pdlg == NULL)
		return;

	if(Running != 1)
		return;
	
	sprintf(msg_buff, "%d%%", (int)percent);
	m_Percent.SetWindowText(msg_buff);

	m_Preview.GetClientRect(&trect);
	total_rasters = trect.bottom - trect.top;


	printed_rasters = total_rasters * (percent / 100);

	unprinted_rasters = total_rasters - printed_rasters;

	DisplayPage();

}


void CPrinting::OnBnClickedEstop() {

	if(theApp.bObstruction) {
		theApp.GMasterStatus = MASTER_STATUS_ABORTING;
		m_EStop.ShowWindow(SW_HIDE);
		theApp.ClosePrintingWindow = TRUE;
		theApp.bJobWasCancelled = TRUE;
		return;

	}

	//theApp.SendBandCS.Lock(INFINITE);

	/*
	while(theApp.bSendingBand) {
	
		Sleep(0);

	}
	*/

	theApp.GMasterStatus = MASTER_STATUS_PAUSED;

	m_EStop.ShowWindow(SW_HIDE);
	m_ContinuePrint.ShowWindow(SW_SHOW);
	//m_CleanPrint.ShowWindow(SW_SHOW);;
	m_CancelPrint.ShowWindow(SW_SHOW);;

	//theApp.SendBandCS.Unlock();


}


void CPrinting::OnBnClickedCancelprint() {

	Sleep(0);

	theApp.SendBandCS.Lock(INFINITE);
	
	//m_EStop.ShowWindow(SW_SHOW);
	m_ContinuePrint.ShowWindow(SW_HIDE);
	m_CleanPrint.ShowWindow(SW_HIDE);;
	m_CancelPrint.ShowWindow(SW_HIDE);;

	Sleep(0);

	FTDPurge(&FPGAusb);
	FTDPurge(&AUXusb);

	theApp.bJobWasCancelled = TRUE;
	theApp.GMasterStatus = MASTER_STATUS_CANCELLING;
	theApp.ClosePrintingWindow = TRUE;

	theApp.bCancelComplete = 0;
	USBPrinterCommand(USB_CMD_BOUNCE_CMD, BOUNCE_SIGNAL_CANCEL_COMPLETE);
	USBPrinterCommand(USB_CMD_BOUNCE_CMD, USB_CMD_JOB_CLEANUP);	
	theApp.bCancelRequested = 1;
	TRACE("Cancel requested\n");
	theApp.SendBandCS.Unlock();

	/*
	Sleep(0);
	//m_EStop.ShowWindow(SW_SHOW);
	m_ContinuePrint.ShowWindow(SW_HIDE);
	m_CleanPrint.ShowWindow(SW_HIDE);;
	m_CancelPrint.ShowWindow(SW_HIDE);;
	Sleep(0);
	FTDPurge(&FPGAusb);
	FTDPurge(&AUXusb);
	theApp.bJobWasCancelled = TRUE;
	theApp.GMasterStatus = MASTER_STATUS_CANCELLING;
	theApp.ClosePrintingWindow = TRUE;
	theApp.bCancelComplete = 0;
	USBPrinterCommand(USB_CMD_BOUNCE_CMD, BOUNCE_SIGNAL_CANCEL_COMPLETE);	
	USBPrinterCommand(USB_CMD_BOUNCE_CMD, USB_CMD_JOB_CLEANUP);	
	theApp.bCancelRequested = 1;
	TRACE("Cancel requested\n");
	*/
	
}


void CPrinting::OnBnClickedContinueprint() {
	//m_EStop.ShowWindow(SW_SHOW);

	m_ContinuePrint.ShowWindow(SW_HIDE);
	m_CleanPrint.ShowWindow(SW_HIDE);
	m_CancelPrint.ShowWindow(SW_HIDE);

	if(!theApp.NoMoreCancel)  {
		// TRACE("Show EButton: %s %d\n", __FILE__, __LINE__);
		//m_EStop.ShowWindow(SW_SHOW);
		bEStopDisplayTimer = 1;
	}

	/**/ TRACE("Showing ESTOP on continueprint\n");
	if(theApp.GPlatenPosition == 1) {
		USBPrinterCommand(USB_CMD_SYM_START_BUTTON, 1);
	}
	if(theApp.bCancelRequested) {
		theApp.bCancelRequested;
		theApp.GMasterStatus = MASTER_STATUS_CANCELLING;
	}
	else {
		theApp.GMasterStatus = MASTER_STATUS_PRINTING;
		/**/ TRACE("Setting status to printing [3]\n");
	}

}


void CPrinting::OnBnClickedCleanprint() {

	/**/ TRACE("Showing ESTOP on cleanprint\n");
	m_EStop.ShowWindow(SW_SHOW);
	/**/ TRACE("Show EButton: %s %d\n", __FILE__, __LINE__);
	m_ContinuePrint.ShowWindow(SW_HIDE);
	m_CleanPrint.ShowWindow(SW_HIDE);;
	m_CancelPrint.ShowWindow(SW_HIDE);;


}


void CPrinting::PublicContinueprint() {
	//OnBnClickedContinueprint();
	//m_EStop.ShowWindow(SW_SHOW);
	m_ContinuePrint.ShowWindow(SW_HIDE);
	m_CleanPrint.ShowWindow(SW_HIDE);
	m_CancelPrint.ShowWindow(SW_HIDE);
	/**/ TRACE("Show EButton: %s %d\n", __FILE__, __LINE__);
	m_EStop.ShowWindow(SW_SHOW);
	theApp.GMasterStatus = MASTER_STATUS_PRINTING;
	/**/ TRACE("Setting status to printing [4]\n");


}

void CPrinting::PublicClickedEstop() {
	OnBnClickedEstop();
}
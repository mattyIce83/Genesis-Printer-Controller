// Controller4Dlg4.cpp : implementation file
//

#include "stdafx.h"

#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg4.h"
#include "Controller4Dlg5.h"
#include "parser.h"
#include "HEAD_structs.h"
#include "DeleteVerify.h"
#include "afxdialogex.h"
#include "parser.h"
#include "Printing.h"
#include "ConfirmPrint.h"

extern CController4App theApp;
extern displayGraphicState GS;
extern QUEUE_ITEM *current_queue_item;

CController4Dlg4 *theJobDlg;

extern CController4Dlg1 *theDlg;
extern void ParserLoop(void *);

extern int GbandNo;

extern void PrintingDialogLoop(void *);
extern void USBPrinterCommand(unsigned long , unsigned long);

static CMyCtrlList *m_JobListPtr;


static BOOL selected_items[MAX_ITEMS_IN_QUEUE];
// CController4Dlg4 dialog

IMPLEMENT_DYNAMIC(CController4Dlg4, CDialogEx)



CController4Dlg4::CController4Dlg4(CWnd* pParent /*=NULL*/)
	: CDialogEx(CController4Dlg4::IDD, pParent)
{

}

CController4Dlg4::~CController4Dlg4()
{
}

void CController4Dlg4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONFIRMBANDS, m_ConfirmBands);
	DDX_Control(pDX, IDC_JOBLIST, m_JobList);
	DDX_Control(pDX, IDC_REPRINT, m_Reprint);
	DDX_Control(pDX, IDC_PROTECT, m_Protect);
	DDX_Control(pDX, IDC_IMPORTJOBS, m_Import);
	DDX_Control(pDX, IDC_EXPORT, m_Export);
	DDX_Control(pDX, IDC_BUTTON1, m_Delete);
	DDX_Control(pDX, IDC_SCROLL_LEFT, m_LeftArrow);
	DDX_Control(pDX, IDC_SCROLL_RIGHT, m_RightArrow);
	DDX_Control(pDX, IDC_JOBSPECS, m_JobSpecs);
	DDX_Control(pDX, IDC_PRODRUN, m_ProdRun);
}


BEGIN_MESSAGE_MAP(CController4Dlg4, CDialogEx)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_HOME, &CController4Dlg4::OnBnClickedHome)
	ON_BN_CLICKED(IDC_RIGHT, &CController4Dlg4::OnBnClickedRight)
	ON_BN_CLICKED(IDC_HLP, &CController4Dlg4::OnBnClickedHlp)
	ON_BN_CLICKED(IDC_PROTECT, &CController4Dlg4::OnBnClickedProtect)
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDC_REPRINT, &CController4Dlg4::OnBnClickedReprint)
	ON_NOTIFY(NM_CLICK, IDC_JOBLIST, &CController4Dlg4::OnNMClickJoblist)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_IMPORTJOBS, &CController4Dlg4::OnBnClickedImportjobs)
	ON_BN_CLICKED(IDC_EXPORT, &CController4Dlg4::OnBnClickedExport)
	ON_BN_CLICKED(IDC_CONFIRMBANDS, &CController4Dlg4::OnBnClickedConfirmbands)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_JOBLIST, &CController4Dlg4::OnLvnItemchangedJoblist)
	ON_BN_CLICKED(IDC_BUTTON1, &CController4Dlg4::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_KEYDOWN, IDC_JOBLIST, &CController4Dlg4::OnKeydownJoblist)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_JOBLIST, &CController4Dlg4::OnItemactivateJoblist)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_JOBLIST, &CController4Dlg4::OnColumnclickJoblist)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_SCROLL_LEFT, &CController4Dlg4::OnBnClickedScrollLeft)
	ON_BN_CLICKED(IDC_SCROLL_RIGHT, &CController4Dlg4::OnBnClickedScrollRight)
	ON_BN_DOUBLECLICKED(IDC_SCROLL_RIGHT, &CController4Dlg4::OnBnDoubleclickedScrollRight)
	ON_BN_DOUBLECLICKED(IDC_SCROLL_LEFT, &CController4Dlg4::OnBnDoubleclickedScrollLeft)
	ON_BN_CLICKED(IDC_PRODRUN, &CController4Dlg4::OnBnClickedProdrun)
END_MESSAGE_MAP()


// CController4Dlg4 message handlers

BOOL CheckfileExist(char *szFile) {
FILE *f;

	f = fopen(szFile, "r");

	if(f == NULL)
		return FALSE;

	else {
		fclose(f);
		return TRUE;
	}


}


void CController4Dlg4::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);



}


BOOL CController4Dlg4::OnInitDialog() {
int result;

	bButtonDown = FALSE;

	theApp.QueueRedrawRequest = FALSE;

	dcScreen = this->GetDC();

	m_ResX = GetDeviceCaps(dcScreen->m_hDC, HORZRES);
	m_ResY = GetDeviceCaps(dcScreen->m_hDC, VERTRES);


	result = m_Home.AutoLoad(IDC_HOME, this);
	result = m_Reprint.AutoLoad(IDC_REPRINT, this);
	result = m_Export.AutoLoad(IDC_EXPORT, this);
	result = m_Delete.AutoLoad(IDC_BUTTON1, this);
	result = m_Protect.AutoLoad(IDC_PROTECT, this);
	m_LeftArrow.AutoLoad(IDC_SCROLL_LEFT, this);
	m_RightArrow.AutoLoad(IDC_SCROLL_RIGHT, this);
	m_ProdRun.AutoLoad(IDC_PRODRUN, this);

	theJobDlg = this;

	CDialogEx::OnInitDialog();

	m_JobList.HideScrollBars(LCSB_CLIENTDATA, SB_VERT);
	m_JobList.HideScrollBars(LCSB_CLIENTDATA);
	m_JobList.HideScrollBars(LCSB_NCOVERRIDE);
	m_JobList.HideScrollBars(LCSB_NCOVERRIDE);


	m_JobListPtr = &m_JobList;

	theApp.QueueWND = this->m_hWnd;


	///////////////////////////////////////
	//////////////////////////////////////

 
	m_JobList.SetFont(&theDlg->pMediumFont);

	m_JobSpecs.SetFont(&theDlg->pMediumFont);

	//m_JobList.SetTextColor(RGB(255,255,255));
	/**/m_JobList.SetTextBkColor(RGB(0,0,0));
	//m_JobList.SetTextBkColor(RGB(68,68,68));


	m_Delete.SetFont(&theDlg->pMediumFont);
	m_Import.SetFont(&theDlg->pSmallFont);


	m_Export.SetFont(&theDlg->pMediumFont);
	m_Protect.SetFont(&theDlg->pMediumFont);


	////////////////////////////////////
	////////////////////////////////////

	SizeDialog();

	RECT listrect;

	/**/m_JobList.SetBkColor(RGB(13,13,13));
	//m_JobList.SetBkColor(RGB(68,68,68));


	m_JobList.GetClientRect(&listrect);


	logBrush2.lbStyle = BS_SOLID;
	logBrush2.lbColor = RGB(255, 255, 255);
	hBGbrush2.CreateBrushIndirect(&logBrush2);

	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(0, 0, 0);
	hBGbrush.CreateBrushIndirect(&logBrush);

	POINT pt;
	pt.x = 1;
	pt.y = 1;

	logPen.lopnStyle = PS_SOLID;
	logPen.lopnColor = RGB(0, 0, 0);
	logPen.lopnWidth = pt;
	hCPen.CreatePenIndirect(&logPen);

	DisplayQueue();

	m_JobList.GetWindowPlacement(&m_JobList.pos);

	m_JobList.SetFont(&theDlg->pMediumFont);

	m_nTimer = SetTimer(TEMP_DISPLAY_UPDATE, 15000, 0);

	m_nScrollTimer = SetTimer(SCROLL_UPDATE, 250, 0);


	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CController4Dlg4::OnSetFocus(CWnd* pOldWnd) {

	theApp.currentWindow = JOBS_WINDOW;

	TRACE("currentwindow: %d\n", theApp.currentWindow);

	m_JobList.SetFont(&theDlg->pMediumFont);

	if(theApp.bHomePressed) {
		CDialogEx::OnCancel();
	}

	RestoreSelections();

	CDialogEx::OnSetFocus(pOldWnd);

	m_JobList.GetFocus();

}


void CController4Dlg4::SetNewCurrentJob() {
int i, x;
LV_ITEM lvi;
QUEUE_ITEM *qiptr;
BOOL has_sel = FALSE;


	if(SaveSelections() != 1)
		return;


	x = m_JobList.GetItemCount();

	for(i = 0; i < x; i++) {
		lvi.mask=LVIF_STATE;
		lvi.iItem=i;
		lvi.iSubItem=0;
		lvi.stateMask=0xFFFF;		// get all state flags
		m_JobList.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED) {
			qiptr = (QUEUE_ITEM *)m_JobList.GetItemData(i);
			has_sel = TRUE;
			break;
		}
	}

	if(has_sel) {
		theApp.GVars->CurrentJobNumber = qiptr->JobNumber;
		theDlg->InvalidateRect(&theDlg->ButtonArea[CURRENT_COUNTER_BUTTON].slice[0]);
		theApp.WriteINI();
	}

}

void CController4Dlg4::OnBnClickedHome() {


	SetNewCurrentJob();


	theApp.bHomePressed = TRUE;
	CDialogEx::OnCancel();
}


void CController4Dlg4::OnBnClickedRight() {
CController4Dlg5 dlg;

	dlg.DoModal();

}


void CController4Dlg4::OnBnClickedHlp()
{
	// TODO: Add your control notification handler code here
}


void CController4Dlg4::OnBnClickedProtect() {

	int sels = SaveSelections();

	if(sels >= theApp.GVars->MaxJobs - 2) {
		//MessageBox("TOO MANY PROTECTED ITEMS", APP_NAME, MB_OK|MB_SYSTEMMODAL);
		theDlg->OKBox("Too many", "protected items", NULL);
		return;
	}

	if(sels > 1) {
		ProtectItems();
		theApp.WriteQueue();
		m_JobList.SetFocus();
	}
	else if(!sels){
		//MessageBox("No items selected", APP_NAME, MB_YESNO|MB_SYSTEMMODAL);
		theDlg->OKBox("No items", "selected", NULL);

	}
	else {
		ProtectItem();
		theApp.WriteQueue();
		m_JobList.SetFocus();
	}


}


void CController4Dlg4::OnKillFocus(CWnd* pNewWnd)
{

	SaveSelections();

	CDialogEx::OnKillFocus(pNewWnd);

}


void ExecuteReprint(QUEUE_ITEM *qiptr, BOOL bUseVerify) {
int i;

	if(bUseVerify) {

		CConfirmPrint entrydlg;

		strcpy(entrydlg.m_szJobFile, qiptr->szFileName[0]);

		entrydlg.qi = qiptr;

		i = entrydlg.DoModal();

		if(i != 1)
			return;

	}

	int copies = 1;

	theDlg->bIsAReprint = TRUE;

	if(!strlen(qiptr->szSourceFile)) {
		strcpy(qiptr->szSourceFile, qiptr->szFileName[0]);
	}

	theDlg->StatusLine("File: %s\n", qiptr->szSourceFile);
	TRACE("File: %s\n", qiptr->szSourceFile);


	theDlg->StatusLine("Creating Worker Thread\n");

	SECURITY_ATTRIBUTES ThreadSA;

	ThreadSA.nLength = sizeof(SECURITY_ATTRIBUTES);
	ThreadSA.bInheritHandle = TRUE;
	ThreadSA.lpSecurityDescriptor  = NULL;

	theApp.bInJob = 1;
	theDlg->StatusLine("Resuming Formatter Thread\n");
	theApp.HeadFormatterThread->ResumeThread();

	parserIOStruct ios;
	ios.szSourceFile = qiptr->szSourceFile;
	ios.theDlg = (void *)NULL;
	ios.Done = FALSE;
	ios.copies = copies;
	ios.qitem = qiptr; //GetQItemInfo();
	ios.sizeX = GS.PaperDimensionX;
	ios.sizeY = GS.PaperDimensionY;
	if(ios.sizeX == 0.0) ios.sizeX = qiptr->printarea.x;
	if(ios.sizeY == 0.0) ios.sizeY = qiptr->printarea.y;

	if(theApp.bCancelRequested) {
		theApp.GMasterStatus = MASTER_STATUS_CANCELLING;
		return;
	}

	if(theApp.GMasterStatus == MASTER_STATUS_CANCELLING)
		return;

	theApp.GMasterStatus = MASTER_STATUS_PRINTING;
	/**/ TRACE("Setting status to printing [1]\n");

	GbandNo = 0;

#ifdef SEND_PLATEN_ON_CENTER_BUTTON

	if(theApp.GPlatenPosition == 1 && theApp.bCenterButton) {
		/***/ TRACE("USB_CMD_SYM_START_BUTTON, 2 at %s line %s\n", __FILE__, __LINE__);
		//theApp.bCenterButton = FALSE;
		USBPrinterCommand(USB_CMD_SYM_START_BUTTON, 2);
		//USBPrinterCommand(USB_CMD_SYM_START_BUTTON, 1);

	}

#endif

	if(theApp.bPrintingSecondLayer) {
		if(!theApp.bQuickReturn || !theApp.GVars->bEnableColorQuickReturn) {
			/***/ TRACE("USB_CMD_SYM_START_BUTTON, 2 at %s line %s\n", __FILE__, __LINE__);
			USBPrinterCommand(USB_CMD_SYM_START_BUTTON, 2);
		}
		theApp.bPrintingSecondLayer = FALSE;
		//Sleep(500);
	}

	/**/ TRACE("Launching PrintingDialogLoop (1)\n");

	theApp.PrintingThread = AfxBeginThread((AFX_THREADPROC)PrintingDialogLoop,
												(void *)&ios,
												0,
												0,
												0,
												&ThreadSA);

	Sleep(250);


	theApp.WorkerThread = AfxBeginThread((AFX_THREADPROC)ParserLoop,
												(void *)&ios,
												0,
												0,
												0,
												&ThreadSA);

	while(!ios.Done)
		Sleep(0);

	Sleep(300);

	
}


void CController4Dlg4::OnBnClickedReprint() {
int i, x;
LV_ITEM lvi;
QUEUE_ITEM *qiptr;
BOOL has_sel = FALSE;

	theApp.bCenterButton = FALSE;

	if(theApp.bInJob)
		return;

	if(SaveSelections() != 1)
		return;


	x = m_JobList.GetItemCount();

	for(i = 0; i < x; i++) {
		lvi.mask=LVIF_STATE;
		lvi.iItem=i;
		lvi.iSubItem=0;
		lvi.stateMask=0xFFFF;		// get all state flags
		m_JobList.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED) {
			qiptr = (QUEUE_ITEM *)m_JobList.GetItemData(i);
			has_sel = TRUE;
			break;
		}
	}

	if(has_sel) {

		ExecuteReprint(qiptr, /*TRUE*/ FALSE);

	
	}

}


QUEUE_ITEM *CController4Dlg4::GetQItemInfo() {
int i, x;
LV_ITEM lvi;
QUEUE_ITEM *qiptr = NULL;

	x = m_JobList.GetItemCount();

	for(i = 0; i < x; i++) {
		
		if(selected_items[i]) {
			// make item highlighted
			lvi.mask=LVIF_STATE;
			lvi.iItem=i;
			lvi.iSubItem=0;
			lvi.stateMask=0xFFFF;		// get all state flags
			m_JobList.GetItem(&lvi);
			qiptr = (QUEUE_ITEM *)m_JobList.GetItemData(i);
			return qiptr;
		}
	}
	return NULL;
}

void CController4Dlg4::RestoreSelections() {
int i, x;
LV_ITEM lvi;
QUEUE_ITEM *qiptr = NULL;

	x = m_JobList.GetItemCount();

	for(i = 0; i < x; i++) {
		
		if(selected_items[i]) {
			// make item highlighted
			lvi.mask=LVIF_STATE;
			lvi.iItem=i;
			lvi.iSubItem=0;
			lvi.stateMask=0xFFFF;		// get all state flags
			m_JobList.GetItem(&lvi);
			lvi.state |= (LVIS_SELECTED | LVIS_FOCUSED);
			m_JobList.SetItem(&lvi);
			/**/TRACE("Selecting item %d\n", i);
		}
	}

}

int CController4Dlg4::SaveSelections() {
int i, x, sels = 0;
LV_ITEM lvi;
QUEUE_ITEM *qiptr = NULL;

	memset((char *)&selected_items, 0, sizeof(BOOL) * MAX_ITEMS_IN_QUEUE);

	x = m_JobList.GetItemCount();

	for(i = 0; i < x; i++) {
		lvi.mask=LVIF_STATE;
		lvi.iItem=i;
		lvi.iSubItem=0;
		lvi.stateMask=0xFFFF;		// get all state flags
		m_JobList.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED) {
			/**/ TRACE("ITEM %d is selected\n", i);
			selected_items[i] = TRUE;
			sels++;
		}
	}

	return sels;
}

void CController4Dlg4::OnNMClickJoblist(NMHDR *pNMHDR, LRESULT *pResult) {

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

int i, x;
LV_ITEM lvi;
QUEUE_ITEM *qiptr = NULL;

	x = m_JobList.GetItemCount();

	for(i = 0; i < x; i++) {
		lvi.mask=LVIF_STATE;
		lvi.iItem=i;
		lvi.iSubItem=0;
		lvi.stateMask=0xFFFF;		// get all state flags
		m_JobList.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED) {
			qiptr = (QUEUE_ITEM *)m_JobList.GetItemData(i);
			DisplayQueueItem(qiptr);
			break;
		}
	}
	if(qiptr == NULL) {
		theApp.bHasPreview = FALSE;
		DisplayQueueItem(qiptr);
	}

	*pResult = 0;
}


void CController4Dlg4::OnPaint() {

	CPaintDC dc(this); // device context for painting
	
	CDialog::OnPaint();

	//theApp.DisplayBitmap(&dc, &theApp.hBackground, 0, 0, DB_SHOWUPPERLEFT);
	/**/theApp.DisplayBitmap(&dc, &theApp.hBackgroundJobs, 0, 0, DB_SHOWUPPERLEFT);

	ShowTemp(&dc);

}


void CController4Dlg4::OnStnClickedPreview() {
int x;

	x = 0;

}
	

void CController4Dlg4::OnDestroy()
{

	theApp.QueueWND = NULL;
	KillTimer(m_nTimer);
	KillTimer(m_nScrollTimer);

	CDialogEx::OnDestroy();


	hBGbrush2.DeleteObject();
	hBGbrush.DeleteObject();
	hCPen.DeleteObject();
	m_PicDC.DeleteDC();

}


void CController4Dlg4::OnBnClickedImportjobs() {
BOOL bOldHalt = theApp.HaltJobScanner;

	/**/ TRACE("HaltEvent 648\n");
	theApp.HaltJobScanner = TRUE;
	theApp.ImportSupp.ImportFiles(theApp.GVars, theApp.QueueItems);

	/**/ TRACE("HaltEvent 652\n");
	theApp.HaltJobScanner = bOldHalt;


}


void CController4Dlg4::OnBnClickedExport() {



}



void CController4Dlg4::OnBnClickedConfirmbands()
{
	// TODO: Add your control notification handler code here
}

void CController4Dlg4::SetProductionModeButton(QUEUE_ITEM *qi) {
	
	if(qi != NULL && theApp.GVars->bDeleteAfterPrinting && theApp.GVars->MaxJobs == 1) {

		if(qi->iProdMode == PRODMODE_SINGLE) {
			m_ProdRun.LoadBitmaps("SINGLERUNU", "SINGLERUND");
		}
		else {
			m_ProdRun.LoadBitmaps("PRODRUNU", "PRODRUND");
		}
		m_ProdRun.ShowWindow(SW_SHOW);
		m_ProdRun.Invalidate(FALSE);
	}
	else {
		m_ProdRun.ShowWindow(SW_HIDE);
	}


}

void CController4Dlg4::OnLvnItemchangedJoblist(NMHDR *pNMHDR, LRESULT *pResult) {
int error;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	/**/  TRACE("OnLVNItemChanged\n");

	int sels = SaveSelections();


	if(sels == 0) {
		//m_Reprint.ShowWindow(SW_HIDE);
		m_Export.ShowWindow(SW_HIDE);
		m_Delete.ShowWindow(SW_HIDE);
		m_Protect.ShowWindow(SW_HIDE);
		m_ProdRun.ShowWindow(SW_HIDE);
		
	}
	else {
		//m_Reprint.ShowWindow(SW_SHOW);
		//m_Export.ShowWindow(SW_SHOW);
		m_Delete.ShowWindow(SW_SHOW);
		m_Protect.ShowWindow(SW_SHOW);
	}

	if(sels > 1 || sels == 0) {

		/**/m_Reprint.ShowWindow(SW_HIDE);

		//if(!m_Protect.LoadBitmaps("LOCK_CLOSEDU", "LOCK_CLOSEDD" "LOCK_CLOSEDF", NULL)) {
		if(!m_Protect.LoadBitmaps("LOCK_CLOSEDU", "LOCK_CLOSEDD" "LOCK_CLOSEDF")) {
			error = GetLastError();
			TRACE("Error: %d\n", error);
		}
		else 
			m_Protect.Invalidate(TRUE);
	
		m_JobSpecs.SetWindowText("");
	}

	if(sels == 1) {

		m_Reprint.ShowWindow(SW_SHOW);
	
		QUEUE_ITEM *qi = GetQItemInfo();

		SetProductionModeButton(qi);


		if(qi != NULL) {


			if(qi->Flags & Q_ITEM_LOCKED) {
				//if(!m_Protect.LoadBitmaps("LOCK_OPENU", "LOCK_OPEND", "LOCK_OPENF", NULL)) {
				if(!m_Protect.LoadBitmaps("LOCK_OPENU", "LOCK_OPEND", "LOCK_OPENF")) {
					error = GetLastError();
				}
				m_Protect.Invalidate(TRUE);
			}
			else {
				//if(!m_Protect.LoadBitmaps("LOCK_CLOSEDU", "LOCK_CLOSEDD" "LOCK_CLOSEDF", NULL)) {
				if(!m_Protect.LoadBitmaps("LOCK_CLOSEDU", "LOCK_CLOSEDD" "LOCK_CLOSEDF")) {
					error = GetLastError();
				}
				m_Protect.Invalidate(TRUE);
			}

			char szSpecs[255], szQuality[64];

			if(qi->resolution.x == 600 && qi->resolution.y == 600)
				sprintf(szQuality, "Fast");
			else if(qi->resolution.x == 600 && qi->resolution.y == 1200)
				sprintf(szQuality, "Fine");
			else
				sprintf(szQuality, "%d x %d", qi->resolution.x, qi->resolution.y);

			sprintf(szSpecs, "ID: %d / Prt Dir: %s / Quality: %s / Mode: %s Density: %s",
							qi->RipJobNumber,
							qi->iPrintDirection == 0?"Uni-Direct.":"Bi-Direct.",
							szQuality,
							qi->iDualCMYK == 1?"CMYK X 2":"CMYK",
							qi->iPrintMode == 0?"Normal":"Double");

			m_JobSpecs.SetWindowText(szSpecs);

		}

	
	}

	*pResult = 0;
}


void CController4Dlg4::OnBnClickedButton1() {

	int sels = SaveSelections();

	if(sels) {
		CDeleteVerify dlg;
		int ret;
		
		strcpy(dlg.m_szMessage, "Are you sure you want to\npermanently delete the selected items?");

		ret = dlg.DoModal();

		if(ret == 1) {
			OnBnClickedDelete();
			m_Reprint.ShowWindow(SW_HIDE);
			m_Export.ShowWindow(SW_HIDE);
			m_Delete.ShowWindow(SW_HIDE);
			m_Protect.ShowWindow(SW_HIDE);

		}

	}
	else {
		//MessageBox("No items selected", APP_NAME, MB_YESNO|MB_SYSTEMMODAL);
		theDlg->OKBox("No Items", "selected", NULL);

	}
}

void CController4Dlg4::OnClose()
{

	CDialogEx::OnClose();
}



void CController4Dlg4::OnKeydownJoblist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CController4Dlg4::OnItemactivateJoblist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CController4Dlg4::OnColumnclickJoblist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CController4Dlg4::ShowTemp(CPaintDC *dc) {

		RECT rc;
		char temp_txt[32];
		GetClientRect(&rc);
		rc.top += 60;
		rc.left += 60;
		rc.right = rc.left + 300;
		rc.bottom = rc.top + 70;
		dc->SelectObject(&theDlg->pMediumFont);
		/**/dc->SetTextColor(RGB(2255, 255, 255));
		//dc->SetTextColor(RGB(128, 128, 128));
		dc->SetBkMode(TRANSPARENT);
		sprintf(temp_txt, "A:%d%c F .. B:%d%c F", theApp.GVars->CurHeadTemp[0], 0xB0, theApp.GVars->CurHeadTemp[1], 0xB0);
		dc->DrawText(temp_txt, strlen(temp_txt), &rc, DT_LEFT|DT_END_ELLIPSIS);


}


void CController4Dlg4::OnTimer(UINT_PTR nIDEvent) {
char temp_txt[32];

	if(nIDEvent == SCROLL_UPDATE) {
		
		// TRACE("Scroll Update\n");

		if(theApp.QueueRedrawRequest) {

			theApp.QueueRedrawRequest = FALSE;

			Sleep(500);

			DisplayQueue();

		}


		if(m_LeftArrow.bButtonActive) {
			m_LeftArrow.ActiveCount++;
			if(m_LeftArrow.ActiveCount >=4) {
				/**/ TRACE("Scroll Left\n");
				CSize s;
				s.cy = 0;
				s.cx = -120;
				m_JobList.Scroll(s);
			}
		}

		if(m_RightArrow.bButtonActive) {
			m_RightArrow.ActiveCount++;
			if(m_RightArrow.ActiveCount >=4) {
				/**/ TRACE("Scroll Right\n");
				CSize s;
				s.cy = 0;
				s.cx = 120;
				m_JobList.Scroll(s);
			}

		}


	}


	if(nIDEvent == TEMP_DISPLAY_UPDATE) {

		CPaintDC dc(this); // device context for painting


		RECT rc;
		GetClientRect(&rc);

		ShowTemp(&dc);
		rc.top += 60;
		rc.left += 60;
		rc.right = rc.left + 300;
		rc.bottom = rc.top + 70;
		InvalidateRect(&rc, FALSE);

	}

	CDialogEx::OnTimer(nIDEvent);

}



void CController4Dlg4::OnMouseMove(UINT nFlags, CPoint point)
{

	CDialogEx::OnMouseMove(nFlags, point);
}

void CController4Dlg4::OnLButtonDown(UINT nFlags, CPoint point) {

	/**/ TRACE("Ctrl4Dlg BUTTON DOWN\n");


	m_JobListPtr->bButtonDown = TRUE;

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CController4Dlg4::OnLButtonUp(UINT nFlags, CPoint point)
{

	/**/ TRACE("Ctrl4Dlg BUTTON UP\n");

	CDialogEx::OnLButtonUp(nFlags, point);

	m_JobListPtr->bButtonDown = FALSE;

}


void CController4Dlg4::OnBnClickedScrollLeft()
{
CSize s;

	s.cy = 0;
	s.cx = -120;
	m_JobList.Scroll(s);
}


void CController4Dlg4::OnBnClickedScrollRight()
{
CSize s;

	s.cy = 0;
	s.cx = 120;
	m_JobList.Scroll(s);

}


void CController4Dlg4::OnBnDoubleclickedScrollRight() {
int items, i;
CSize s;

	items = m_JobList.GetItemCount();

	s.cy = 0;
	s.cx = 120;

	for(i = 0; i < items; i++) {
		m_JobList.Scroll(s);
	}


}


void CController4Dlg4::OnBnDoubleclickedScrollLeft() {
int items, i;
CSize s;

	items = m_JobList.GetItemCount() + 1;

	s.cy = 0;
	s.cx = -120;

	for(i = 0; i < items; i++) {
		m_JobList.Scroll(s);
	}


}



void CController4Dlg4::OnBnClickedProdrun() {

	QUEUE_ITEM *qi = GetQItemInfo();

	if(qi->iProdMode == PRODMODE_SINGLE)
		qi->iProdMode = PRODMODE_MULTIPLE;
	else
		qi->iProdMode = PRODMODE_SINGLE;


	SetProductionModeButton(qi);
	
}

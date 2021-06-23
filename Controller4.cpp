
// Controller4.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "parser.h"
//#define DEFINE_RASTER_OFFSET_ARRAYS
#include "HEAD_structs.h"
#include "WaitDialog.h"
#include "YesNoCancel.h"
#include "UserPrompt2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern displayGraphicState GS;
extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;



//extern CController4Dlg *theDlg;
extern CController4Dlg1 *theDlg;


// CController4App

BEGIN_MESSAGE_MAP(CController4App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CController4App construction

CController4App::CController4App()
{

	//m_pszAppName = "GENESIS CONTROLLER";

	//strcpy(m_pszAppName, "GENESIS CONROLLER");

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	bHardStop = bSendingBand = bSendingAuxBand = FALSE;


}




// The one and only CController4App object

CController4App theApp;


// CController4App initialization

BOOL CController4App::InitInstance() {
int i;


	if ( !m_singleInstance.Create( AFX_IDS_APP_TITLE) ) {
	        // Return indicating that this instance
	        // of the app should be shut down
        return FALSE;
    }

#ifndef USE_MOE_LOGGING
	bNewLogEntries = FALSE;
	LogSupp.logfile = NULL;
#endif

	bTestBandMode = FALSE;

	RunWaitDialog = FALSE;

	for(i = 0; i < 4; i++) {
		SupColor[i] = 0;
	}

#ifndef USE_MOE_LOGGING
	ConnectedToLogServer = 0;
#endif

	InkPressureRequested = FALSE;

	bNozzleCheck = FALSE;

	CloseRequest = FALSE;

	bPauseMode = FALSE;

	bCancelRequested = FALSE;
	
	renumbers = NULL;

	CurrentJob = NULL;
	
	CleaningDialogActive = 0;

	HaltJobScanner = FALSE;

	bPendingAutoPrintRequest = FALSE;

	GMasterStatus =	MASTER_STATUS_INITIALIZING;

	hRepeatDataHandle = INVALID_HANDLE_VALUE;

	bBlockTempThread = FALSE;

	ParserCount = 0;

	bPrinterPrepped = 0;

	PrintingWindow = FALSE;

	GPlatenPosition = 0xFF;

	GHeadPosition = 0xFF;

	bReceivedReadyFromFPGA = FALSE;

	bUnlockTempThread = 0;

	bInDemoMode = 0;
	
	bPendingAutoLevelRequest = FALSE;

	bPendingTempRequest = FALSE;
	
	bPendingHumRequest = FALSE;

	bIsAutoPrintJob = FALSE;

	CoInitialize(NULL);

	PreviewBuffer = NULL;

	bCreatingSwath = FALSE;

	current_seconds = 0;
	time_of_last_print = 0;


	if(!CreateDirectory("LOGS", NULL)) {
		TRACE("Could not create logs folder\n");
	}

	hBackground.LoadBitmap(IDB_BACKGROUND);
	hSettingsBackground.LoadBitmap(IDB_SETTINGS_BACKGROUND);
	hBackgroundJobs.LoadBitmap(IDB_BACKGROUND_JOBS);
	hBackgroundTools.LoadBitmap(IDB_BACKGROUND_TOOLS);
	hBackgroundToolsDualCMYK.LoadBitmap(IDB_BACKGROUND_TOOLS_DUALCMYK);
	hBackgroundPrinting.LoadBitmap(IDB_BACKGROUND_PRINTING);
	hNumberBackground.LoadBitmap(IDB_NUMBERSBACK);
	hSWDLBackground.LoadBitmap(IDB_SWDL_BACKGROUND);
	hDevTeamBackground.LoadBitmap(IDB_DEVELOPMENT_TEAM);
	hXButton.LoadBitmap(IDB_XBUTTON);

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	int nRet = L_LoadBitmap("lock_icon.bmp", &lock_icon, sizeof(BITMAPHANDLE), 32, ORDER_BGR, NULL, NULL);
	if(nRet) {
		nRet = L_FlipBitmap(&lock_icon);
	}

	BOOL qic = theApp.m_QueueImages.Create(ICON_X_SIZE, ICON_Y_SIZE, ILC_COLOR24, 16, 200);

	int count, index;

	if(qic) {

		HICON hIcon;

		hIcon = LoadIcon(IDI_PSJOB);

		index = m_QueueImages.Add(hIcon);

		count = m_QueueImages.GetImageCount();
	
	}

	for(count = 0; count < TOTAL_CHANNELS; count++) {

		GS.bits[count] = NULL;
		
	}


	CController4Dlg1 dlg;
	theApp.QueueWND = NULL;
	m_pMainWnd = &dlg;
	
	GetCurrentDirectory(MAX_PATH, LaunchDir);
	
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	if(lock_icon.Flags.Allocated)
		L_FreeBitmap(&lock_icon);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

/*-----------------------------------------------------------*/
/*- Edit selected item                                      -*/
/*-----------------------------------------------------------*/
int CController4App::GetJobNumber(int mode) {
int ret;

	if(theApp.GVars->NextJobNumber > 9999)
		theApp.GVars->NextJobNumber = 0;


	ret = theApp.GVars->NextJobNumber;

	if(mode) {
		theApp.GVars->NextJobNumber++;
	}
	theApp.WriteINI();

	return ret;
}


void CController4App::DisplayBitmap(CDC *PDC, CBitmap *pBitmap, int X, int Y, int pos) {
int px, py;

	BITMAP BM;
	CDC MemDC;
	CRect rect;

	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(pBitmap);
	pBitmap->GetObject(sizeof(BM), &BM);

	
	if(pos == DB_SHOWATPOS) {
		px = X;
		py = Y;
	}
	else {
		switch(pos) {
			case DB_SHOWUPPERLEFT:
				px = 0;
				py = 0;
				break;
			default:
				px = X;
				py = Y;
		}

	}	
	PDC->BitBlt(px, py, BM.bmWidth, BM.bmHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();

}

void CController4App::DisplayError(char *string, int errorno) {

	LPVOID lpMsgBuf;

	char message[256];

	FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
					NULL,
					errorno, //GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL);

	// Display the string.

	sprintf(message, "%s%s", string, (char *)lpMsgBuf);

	MessageBox( GetFocus(), message, PRODUCT_NAME, MB_OK|MB_SYSTEMMODAL|MB_ICONHAND );

	// Free the buffer.

	LocalFree( lpMsgBuf );

}

BOOL CController4App::isQueueWindowOpen() {

	GetFocus();
	GetActiveWindow();

	return FALSE;
}


void CController4App::OverTempWarning(int head) {
char tmp1[64], tmp2[64], tmp3[64];


	sprintf(tmp1, "Head %d OverTemp", head + 1);
	sprintf(tmp2, "Temp Setting: %d", theApp.GVars->HeadTemp[head]);
	sprintf(tmp3, "Reported Temp: %d",  theApp.GVars->CurHeadTemp[head]);
	theDlg->OKBox(tmp1, tmp2, tmp3);
	

}

/*------------------------------------------------------------*/
/*-                                                          -*/
/*------------------------------------------------------------*/
void WDLThread(void *p) {
CWaitDialog dlg;

	if(p == NULL) {
		strcpy(dlg.szMessage[0], "");
		strcpy(dlg.szMessage[1], "");
		strcpy(dlg.szMessage[2], "");
	}
	else {
		strcpy(dlg.szMessage[0], "");
		strcpy(dlg.szMessage[1], (char *)p);
		strcpy(dlg.szMessage[2], "");
	}

	theApp.RunWaitDialog = TRUE;

	dlg.DoModal();


}


/*------------------------------------------------------------*/
/*- Start the wait dialog spinner                            -*/
/*------------------------------------------------------------*/
void CController4App::StartWaitDialog(char *szMessage) {


	theApp.ThreadSA.nLength = sizeof(SECURITY_ATTRIBUTES);
	theApp.ThreadSA.bInheritHandle = TRUE;
	theApp.ThreadSA.lpSecurityDescriptor  = NULL;


	theApp.WaitDialogThread = AfxBeginThread((AFX_THREADPROC)WDLThread,
															(void *)szMessage,
															THREAD_PRIORITY_HIGHEST,
															0,	
															0,
															&theApp.ThreadSA);
	Sleep(100);


}

/*------------------------------------------------------------*/
/*- Proocess auto-level data from FPGA                       -*/
/*------------------------------------------------------------*/
void CController4App::ProcessLevelInfo(unsigned long p1, unsigned long p2) {
	int offsets[4], new_offsets[4] = {0,0,0,0}, largest = 0, i, smallest = 999999;

	offsets[0] = (p1 & 0xFFFF0000) >> 16;
	offsets[1] = (p1 & 0x0000FFFF);

	offsets[2] = (p2 & 0xFFFF0000) >> 16;
	offsets[3] = (p2 & 0x0000FFFF);

	// TRACE("Offset ticks: %d %d %d %d\n", offsets[0] , offsets[1] , offsets[2] , offsets[3]);

	/**/ TRACE("*** Laser Locations:\nFront Right: %d\nFront Left: %d\nRear Right: %d\nRear Left: %d\n", offsets[0] , offsets[1] , offsets[2] , offsets[3]);

	// first grab the largest and smallest offsets;
	for(i = 0; i < 4; i++) {
		if(offsets[i] > largest) 
			largest = offsets[i];
		if(offsets[i] < smallest)
			smallest = offsets[i];
	}

	for(i = 0; i < 4; i++) {

		new_offsets[i] = offsets[i] - smallest;

	}

	// TRACE("New Offsets: %d %d %d %d\n", new_offsets[0] , new_offsets[1] , new_offsets[2] , new_offsets[3]);
	/**/ TRACE("*** New Offsets:\nFront Right: %d\nFront Left: %d\nRear Right: %d\nRear Left: %d\n", new_offsets[0] , new_offsets[1] , new_offsets[2] , new_offsets[3]);

	theApp.GVars->LiftServoCorr[0] = new_offsets[0],
	theApp.GVars->LiftServoCorr[1] = new_offsets[1],
	theApp.GVars->LiftServoCorr[2] = new_offsets[2],
	theApp.GVars->LiftServoCorr[3] = new_offsets[3],


	theApp.WriteINI();

}

/*------------------------------------------------------------*/
/*- Proocess auto-level data from FPGA                       -*/
/*------------------------------------------------------------*/
BOOL CController4App::InksOKToPrint() {
int i;
BOOL ret = TRUE;

	if(theApp.GVars->bInkOutDisableIgnore) return TRUE;

	if(	!theApp.GVars->Valves.bFBagPresent ||
		!theApp.GVars->Valves.bCBagPresent ||
		!theApp.GVars->Valves.bMBagPresent ||
		!theApp.GVars->Valves.bYBagPresent ||
		!theApp.GVars->Valves.bKBagPresent) {

			ret = FALSE;
	}

	if(!theApp.GVars->bDualCMYK && !theApp.GVars->Valves.bWBagPresent) {
		ret = FALSE;
	}


	if(ret) {

		for(i = 0; i < 5; i++) {
	
			if(theApp.GVars->InkAlarm[i])
				ret = FALSE;
			
			if(theApp.GVars->inkADC[i] == 0 || theApp.GVars->inkADC[i] == 1	)
				ret = FALSE;

			if(!ret)
				break;
	
		}

		if(!theApp.GVars->bDualCMYK) {
			if(theApp.GVars->inkADC[5] == 0 || theApp.GVars->inkADC[5] == 1	)
				ret = FALSE;
		}

	}

	return ret;

}

/*------------------------------------------------------------*/
/*-                                                          -*/
/*------------------------------------------------------------*/
BOOL CController4App::CancelContinue(char *message, unsigned long options) {
CYesNoCancel dlg;

	strcpy(dlg.szMessage, message);


	return(dlg.DoModal() == IDOK);



}

/*------------------------------------------------------------*/
/*-  In this state, it's at the front wating for a job       -*/
/*------------------------------------------------------------*/
BOOL CController4App::IsWaitingForJob() {


	return(theApp.GPlatenPosition == 2 &&
			!bPauseMode &&
			QueueItems == 0 &&
			GVars->iJobWait &&
			GMasterStatus == MASTER_STATUS_IDLE &&
			GVars->bPrintOnJobArrival &&
			currentWindow == MAIN_RUNNING_WINDOW);



}

/*------------------------------------------------------------*/
/*-  Send offset for loaded platen to FPGA                   -*/
/*------------------------------------------------------------*/
void CController4App::SendPlatenSelection() {
float sop_offset;

	if(theApp.GVars->iPrinterType == 2) { // mod base
		//sop_offset =  (GVars->PlatenHeight[GVars->iSelectedPlaten] - 18.0) * 735;
		sop_offset =  ((GVars->PlatenHeight[GVars->iSelectedPlaten] - 18.0) * 1200) + GVars->PlatenOffset[GVars->iSelectedPlaten];
	}
	else {
		(sop_offset =  (GVars->PlatenHeight[GVars->iSelectedPlaten] - 18.0) * 4900) + GVars->PlatenOffset[GVars->iSelectedPlaten];
	}

	USBPrinterCommand(USB_CMD_SET_SOP_OFFSET, (int)sop_offset);
	USBPrinterCommand(USB_CMD_SET_SOP_DELAY, GVars->PlatenDelay[GVars->iSelectedPlaten]);

}

/*------------------------------------------------------------*/
/*-  PauseThreads                                            -*/
/*------------------------------------------------------------*/
void CController4App::SlowWorkerThreads(int mode) {
int i;

	CWinThread *threads[5] = {
		WeaverThread,
		TempHandlerThread,
		HeadFormatterThread,
		hAuxUSBSendThread,
	};

	int priorities[5] = {
		0,
		0,
		THREAD_PRIORITY_TIME_CRITICAL,
		THREAD_PRIORITY_TIME_CRITICAL,
	};



	if(mode) {

		for(i = 0; i < 4; i++) {
			threads[i]->SetThreadPriority(THREAD_PRIORITY_LOWEST);
		}


	}
	else {
		for(i = 0; i < 4; i++) {
			threads[i]->SetThreadPriority(priorities[5]);
		}

	}



}

/*------------------------------------------------------------*/
/*-                                                          -*/
/*------------------------------------------------------------*/
BOOL CController4App::CheckAutoClean( ) {
BOOL ret = FALSE;

	/// only do this if head is home
	if(theApp.GHeadPosition != 0)
		return FALSE;


	if(theApp.GVars->iColorJobsBeforeClean > 0) {
		if(theApp.GVars->iColorJobCount >= theApp.GVars->iColorJobsBeforeClean) {

			USBPrinterCommandEx(USB_CMD_PARK, 0, 0, 1, 0, 0, 0, 0);


			theApp.GVars->iColorJobCount = 0;
			theApp.GVars->iWhiteJobCount = 0;

#ifdef AUTO_CLEAN_LEVEL_1
			USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)0x03, 1, 0, 0, 0, 0, 0);
#endif
#ifdef AUTO_CLEAN_LEVEL_2
			USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 2, theApp.GVars->bDualCMYK, 0, 0, 0, 0);
#endif
		
			ret = TRUE;

		}


	}

	if(theApp.GVars->iWhiteJobsBeforeClean > 0) {
		if(theApp.GVars->iWhiteJobCount >= theApp.GVars->iWhiteJobsBeforeClean) {

			USBPrinterCommandEx(USB_CMD_PARK, 0, 0, 1, 0, 0, 0, 0);

			theApp.GVars->iColorJobCount = 0;
			theApp.GVars->iWhiteJobCount = 0;

			USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)0x03, 1, 0, 0, 0, 0, 0);

			ret = TRUE;

		}
	
	
	}


	return ret;
}

/*------------------------------------------------------------*/
/*-                                                          -*/
/*------------------------------------------------------------*/
int CController4App::LargestStepperOffset() {
int largest = 0, i;

	for(i = 0; i < 4; i++) {
		
		if(theApp.GVars->PlatenHeightOffset[i] > largest)
			largest = theApp.GVars->PlatenHeightOffset[i];

	}

	return largest;

}

/*------------------------------------------------------------*/
/*-                                                          -*/
/*------------------------------------------------------------*/
BOOL CController4App::CheckMaxStepperThrow(int *difference) {
BOOL ret = TRUE;
int FeedOrPrint = theApp.GVars->iDefaultPlatenHeight;

	if(theApp.GVars->iPlatenFeedHeight > theApp.GVars->iDefaultPlatenHeight)
		FeedOrPrint = theApp.GVars->iPlatenFeedHeight;

	if(theApp.GVars->iPlatenParkOffset > FeedOrPrint)
		FeedOrPrint = theApp.GVars->iPlatenParkOffset;

	if((LargestStepperOffset() + FeedOrPrint/* + theApp.GVars->iPlatenParkOffset*/) > GVars->iMaxStepperThrow) {

		if(difference != NULL) {
			*difference = (LargestStepperOffset() + FeedOrPrint/* + theApp.GVars->iPlatenParkOffset*/) - GVars->iMaxStepperThrow;
		}

		return FALSE;

	}

	if(difference != NULL)
		*difference - 0;

	return TRUE;

}


/*------------------------------------------------------------*/
/*-                                                          -*/
/*------------------------------------------------------------*/
int CController4App::ConfigurableUserDlg(	int Mode,

											char *szDialogTitle,

											char *Button1_Text,
											char *Button2_Text,
											char *Button3_Text,
											char *Button4_Text,

											char *IllustrationFile1,
											char *IllustrationFile2,
											char *IllustrationFile3,
											char *IllustrationFile4,

											int iDefaultButton) {

CUserPrompt2 dlg;

	dlg.Mode = Mode;
	
	if(strlen(szDialogTitle)) strcpy(dlg.szDialogTitle, szDialogTitle);

	if(Button1_Text != NULL) strcpy(dlg.szButtonText[0], Button1_Text);
	else dlg.szButtonText[0][0] = 0;

	if(Button2_Text != NULL) strcpy(dlg.szButtonText[1], Button2_Text);
	else dlg.szButtonText[1][0] = 0;

	if(Button3_Text != NULL) strcpy(dlg.szButtonText[2], Button3_Text);
	else dlg.szButtonText[2][0] = 0;
	
	if(Button4_Text != NULL) strcpy(dlg.szButtonText[3], Button4_Text);
	else dlg.szButtonText[3][0] = 0;

	////////////////////
	if(IllustrationFile1 != NULL) strcpy(dlg.szIllustrationFile[0], IllustrationFile1);
	else dlg.szIllustrationFile[0][0] = 0;

	if(IllustrationFile2 != NULL) strcpy(dlg.szIllustrationFile[1], IllustrationFile2);
	else dlg.szIllustrationFile[1][0] = 0;

	if(IllustrationFile3 != NULL) strcpy(dlg.szIllustrationFile[2], IllustrationFile3);
	else dlg.szIllustrationFile[2][0] = 0;
	
	if(IllustrationFile4 != NULL) strcpy(dlg.szIllustrationFile[3], IllustrationFile4);
	else dlg.szIllustrationFile[3][0] = 0;

	dlg.iDefaultButton = iDefaultButton;

	dlg.DoModal();

	return (dlg.nRetCode);



}

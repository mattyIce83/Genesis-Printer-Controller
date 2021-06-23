// Controller4Dlg1.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg2.h"
#include "Controller4Dlg3.h"
#include "Controller4Dlg4.h"
#include "Controller4Dlg5.h"
#include "DevelopmentTeam.h"
#include "Printing.h"
#include "afxdialogex.h"
#include "parser.h"
#include "PleaseWaitInit.h"

#include "resource.h"

#include "ftd2xx.h"
#include "ftdisupp.h"

#include "HEAD_structs.h"

#ifdef USE_MOE_LOGGING
	#include "NetworkDLLinterface.h"   /// MOE - new module
#endif

GVARS *GVarsPtr;

// CController4Dlg1 dialog



CController4Dlg1 *theDlg;
extern CController4App theApp;

void ParserLoop(void *);

void HotFolderScanner(void *);

extern void SendHeaterTimeout();

extern FTDIComm FPGAusb;
extern FTDIComm AUXusb;
extern FTDIComm JOBusb;
extern GVARS G_Vars;
extern void HeadFormatThread(void *);
extern void JobIOThread(void *);
extern void Weaver(void *);
extern void AuxUSBSendThread(void *);

extern PtrToWrite m_pWrite;
extern PtrToRead m_pRead;
extern PtrToOpen m_pOpen; 
extern PtrToOpenEx m_pOpenEx; 
extern PtrToListDevices m_pListDevices; 
extern PtrToClose m_pClose;
extern PtrToResetDevice m_pResetDevice;
extern PtrToPurge m_pPurge;
extern PtrToSetTimeouts m_pSetTimeouts;
extern PtrToGetQueueStatus m_pGetQueueStatus;
extern PtrToGetStatus m_pGetStatus;
extern PtrToSetEventNotification m_pSetEventNotification;
extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;

extern void ExecuteReprint(QUEUE_ITEM *, BOOL);
extern QUEUE_ITEM *FindOtherLayer(QUEUE_ITEM *, int);
extern void AddNumberToBitmap(QUEUE_ITEM *, BITMAPHANDLE *);
extern BOOL QueueItemStillValid(QUEUE_ITEM *);
extern void ReadRenumberRecords();
extern void OpenLogFile();
extern void CloseLogFile();
extern void LogAction(char *);
extern void SendPlatenLevelingOffset();


extern BOOL		bDriverLoaded;
extern HMODULE m_hmodule;

extern HMODULE m_hmodule;


BOOL ExecutePrintRequest();

IMPLEMENT_DYNAMIC(CController4Dlg1, CDialogEx)

CController4Dlg1::CController4Dlg1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CController4Dlg1::IDD, pParent)
{
	iBlinkObstruction = 0;
	bObstructionMarkerState = FALSE;
}

CController4Dlg1::~CController4Dlg1()
{

	//	iBlinkObstruction = 0;
	//	bObstructionMarkerState = FALSE;

}

void CController4Dlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW, m_Preview);
	DDX_Control(pDX, IDC_STATUS_1, m_Status1);
	DDX_Control(pDX, IDC_SYM_PRINT, m_SymPrint);
	DDX_Control(pDX, IDC_SCAN_MARKER, m_ScanIndicator);
	DDX_Control(pDX, IDC_BAG_MARKER_1, m_BagMarker1);
	DDX_Control(pDX, IDC_BAG_MARKER_2, m_BagMarker2);
	DDX_Control(pDX, IDC_BAG_MARKER_3, m_BagMarker3);
	DDX_Control(pDX, IDC_BAG_MARKER_4, m_BagMarker4);
	DDX_Control(pDX, IDC_BAG_MARKER_5, m_BagMarker5);
	DDX_Control(pDX, IDC_BAG_MARKER_0, m_BagMarker0);
	DDX_Control(pDX, IDC_SCANNER_DEBUG, m_ForceButton);
	DDX_Control(pDX, IDC_PRODRUN_ICON, m_PRIcon);
	DDX_Control(pDX, IDC_OBSTRUCTION, m_ObstructionSign);
}


BEGIN_MESSAGE_MAP(CController4Dlg1, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDOK, &CController4Dlg1::OnBnClickedOk)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_SYM_PRINT, &CController4Dlg1::OnBnClickedSymPrint)
	ON_STN_CLICKED(IDC_SCAN_MARKER, &CController4Dlg1::OnStnClickedScanMarker)
	ON_BN_CLICKED(IDC_SCANNER_DEBUG, &CController4Dlg1::OnBnClickedScannerDebug)
END_MESSAGE_MAP()


/*-----------------------------------------------------------------------------------------------------*/
/*-  Initialization loop                                                                              -*/
/*-----------------------------------------------------------------------------------------------------*/
#ifdef USE_MOE_LOGGING
void IPtaskHandler(void *v)  /// MOE
{
	theApp.NetworkDLLinterface.IPtaskHandler();
}
#else
void LogUpdateHandler(void *v) {
int i;

	while(!theApp.quit_threads) {

		for(i = 0; i < theApp.GVars->LogUploadInterval; i++) {
			Sleep(1000);
			if(theApp.quit_threads)
				return;
		}

		if(theApp.GMasterStatus == MASTER_STATUS_IDLE && theApp.bNewLogEntries) {

			theApp.UploadLog();

		}

		
	}

}
#endif

/*-----------------------------------------------------------------------------------------------------*/
/*-  Initialization loop                                                                              -*/
/*-----------------------------------------------------------------------------------------------------*/
void TempHandler(void *v) {
int i, d;
int cycle1 = 0, cycle2 = 0;

	Sleep(2000);


	while(!theApp.quit_threads) {
	
		Sleep(30000);

		if(!theApp.bReceivedReadyFromFPGA) continue;

		if(theApp.GMasterStatus == MASTER_STATUS_IDLE && !theApp.bBlockTempThread && !theApp.CleaningDialogActive && !theApp.HaltJobScanner && !theApp.bPendingAutoLevelRequest) {

			Sleep(500);

			LogAction("Requesting Head Temp 0");
			USBPrinterCommand(USB_CMD_REPORT_HEAD_TEMP, 0);
			Sleep(1000);
			LogAction("Requesting Head Temp 1");
			USBPrinterCommand(USB_CMD_REPORT_HEAD_TEMP, 1);
			Sleep(1000);

#ifndef DISABLE_AMBIENT_TEMP
			if(!theApp.GVars->bSuppressAmbTemp) {
				cycle1++;
				if(cycle1 > 3) {
					cycle1 = 0;
					if(!theApp.bBlockTempThread) {
						if(!theApp.bPendingTempRequest && !theApp.bPendingHumRequest) {
							LogAction("Requesting Ambient Temp");
							theApp.bPendingTempRequest = TRUE;
							USBPrinterCommand(USB_CMD_REPORT_AMBIENT_TEMP, 0);
							cycle2 = 0;
							Sleep(1000);
						}
						else {
							cycle2++;
						}
					}

					if(!theApp.bBlockTempThread) {
						if(!theApp.bPendingTempRequest && !theApp.bPendingHumRequest) {
							LogAction("Requesting Humidiy");
							theApp.bPendingHumRequest = TRUE;
							USBPrinterCommand(USB_CMD_REPORT_HUMIDITY, 0);
							Sleep(1000);
						}
					}
				}

				if(cycle2 > 5) {
					cycle2 = 0;
					theApp.bPendingHumRequest = theApp.bPendingTempRequest = FALSE;
				}
			}
#endif

			if(!theApp.GVars->Versions.requested) {
				theApp.GVars->Versions.requested = TRUE;
				USBPrinterCommand(USB_CMD_REPORT_VERSIONS, 0);
			}

		}
	
		//Sleep(15000);
		//Sleep(30000);
		//Sleep(60000);
	}


	

}


/*-----------------------------------------------------------------------------------------------------*/
/*-  Initialization loop                                                                              -*/
/*-----------------------------------------------------------------------------------------------------*/
void AppInitProc(void *v) {
int i;


	// wait for things to settle
	Sleep(STARTUP_DELAY);
	/// ADD CODE HERE TO LOOK FOR OTHER "READY" INDICATORS
	theApp.ReadINI();
	for(i = 0; i < 6; i++) {
		theApp.GVars->inkFullPercent[i] = 0.0;
		theApp.GVars->inkADC[i] = 0;
	}

	if(theApp.GVars->MaxJobs < 4 && !theApp.GVars->bDualCMYK) {
		theApp.GVars->MaxJobs = 3;
		theApp.WriteINI();
	}

#ifndef USE_MOE_LOGGING
	///MOE 2016-5-17 static linked pointers to be removed
	theApp.LogSupp.SetPointer(LOGGER_PTR_GVARS, (void *)theApp.GVars);
	theApp.LogSupp.SetPointer(LOGGER_PTR_IPFUNCS, (void *)&theApp.NtwrkObj);
	theApp.NtwrkObj.SetPointer(NTWRK_PTR_GVARS, (void *)theApp.GVars);
	///MOE 2016-5-17
#endif
	// strcpy(theApp.GVars->szLogIP, "97.68.245.147");
	// strcpy(theApp.GVars->szLogIP, "97.68.245.146");
	// strcpy(theApp.GVars->szLogIP, "192.168.0.15");


	/*
	if(theApp.GVars->bHideDebugWindows) {
		theDlg->m_Status1.ShowWindow(SW_HIDE);
	}
	else {
		theDlg->m_Status1.ShowWindow(SW_SHOW);
	}
	*/
	if(!theApp.GVars->bHideDebugWindows) {
		theDlg->m_Status1.ShowWindow(SW_SHOW);
	}

#ifndef USE_MOE_LOGGING
	///MOE 2016-5-17
	if(!theApp.NtwrkObj.GetHostIP(theApp.GVars->szLogURL, theApp.GVars->szLogIP)) {
		strcpy(theApp.GVars->szLogIP, theApp.GVars->szFailSafeIP);
	}
#else
	if(theApp.GVars->bNoLogging)
	{
		theDlg->StatusLine("Network logging OFF\n");
	}
	else
	{
		///MOE 2016-5-17 original was editied
		///MOE 2016-7-21 edit - start, now just pass a pointer for the status line message
		char *statusMessage = new char [256];
		theApp.NetworkDLLinterface.LoadNetworkDLL(statusMessage); // import network DLL
		theDlg->StatusLine(statusMessage);
		delete statusMessage;
		///MOE 2016-7-21 edit - end
	}
	///MOE 2016-5-17
#endif

	//if(theApp.GVars->bDualCMYK) {
	//	theDlg->m_BagMarker5.ShowWindow(SW_HIDE);
	//}

	theApp.GVars->bYLinearizationSent = 0;

	/// Just in case not in the INI file:...
	/*
	if(theApp.GVars->Advance[0] == 1) theApp.GVars->Advance[0] = 8560;
	if(theApp.GVars->Advance[1] == 1) theApp.GVars->Advance[1] = 4280;
	if(theApp.GVars->Advance[2] == 1) theApp.GVars->Advance[2] = 2140;
	if(theApp.GVars->Advance[3] == 1) theApp.GVars->Advance[3] = 1070;
	*/

	/// Just in case not in the INI file:...
	if(theApp.GVars->AdvanceGM[0] == 1) theApp.GVars->AdvanceGM[0] = 2560;
	if(theApp.GVars->AdvanceGM[1] == 1) theApp.GVars->AdvanceGM[1] = 1284;
	if(theApp.GVars->AdvanceGM[2] == 1) theApp.GVars->AdvanceGM[2] = 642;
	if(theApp.GVars->AdvanceGM[3] == 1) theApp.GVars->AdvanceGM[3] = 321;


	//com_settings[FPGA_BACKCHANNEL].port = theApp.GVars->FPGA_COM;
	//com_settings[DEBUGGER_BACKCHANNEL].port = theApp.GVars->Backchannel_COM;

	//OpenUtilityCOMs();
	/*theDlg->StatusLine("Com ports: %d (%s)  %d (%s)\n", theApp.GVars->FPGA_COM, com_settings[FPGA_BACKCHANNEL].active?"Active":"Inactive", 
														theApp.GVars->Backchannel_COM, com_settings[DEBUGGER_BACKCHANNEL].active?"Active":"Inactive");*/


	ReadRenumberRecords();
	theApp.ReadQueue();
	//theJobDlg->DisplayQueue();

	
	// Init USB mechanism

	theDlg->StatusLine("Loading FTD2XX driver\n");

	m_hmodule = LoadLibrary("Ftd2xx.dll");	

	if(m_hmodule == NULL) {
		AfxMessageBox("Error loading FTD2XX.DLL");
	}
	else {

		m_pWrite = (PtrToWrite)GetProcAddress(m_hmodule, "FT_Write");
		m_pRead = (PtrToRead)GetProcAddress(m_hmodule, "FT_Read");
		m_pOpen = (PtrToOpen)GetProcAddress(m_hmodule, "FT_Open");
		m_pOpenEx = (PtrToOpenEx)GetProcAddress(m_hmodule, "FT_OpenEx");
		m_pListDevices = (PtrToListDevices)GetProcAddress(m_hmodule, "FT_ListDevices");
		m_pClose = (PtrToClose)GetProcAddress(m_hmodule, "FT_Close");
		m_pResetDevice = (PtrToResetDevice)GetProcAddress(m_hmodule, "FT_ResetDevice");
		m_pPurge = (PtrToPurge)GetProcAddress(m_hmodule, "FT_Purge");
		m_pSetTimeouts = (PtrToSetTimeouts)GetProcAddress(m_hmodule, "FT_SetTimeouts");
		m_pGetQueueStatus = (PtrToGetQueueStatus)GetProcAddress(m_hmodule, "FT_GetQueueStatus");
		m_pGetStatus = (PtrToGetStatus)GetProcAddress(m_hmodule, "FT_GetStatus");
		m_pSetEventNotification = (PtrToSetEventNotification)GetProcAddress(m_hmodule, "FT_SetEventNotification");


		if(m_pWrite == NULL ||
			m_pRead == NULL ||
			m_pOpen == NULL ||
			m_pOpenEx == NULL ||
			m_pListDevices == NULL ||
			m_pClose == NULL ||
			m_pResetDevice == NULL ||
			m_pPurge == NULL ||
			m_pSetTimeouts == NULL ||
			m_pGetQueueStatus == NULL ||
			m_pGetStatus == NULL ||
			m_pSetEventNotification == NULL) {

				theDlg->StatusLine("Error getting module function addresses\n");

		}

		bDriverLoaded = TRUE;

#ifdef USE_CHANNEL_2_FOR_BACKCHANNEL
		InitUSB(&FPGAusb, USB_OUTPUT_DEVICE_NAME, 0);
#else
		InitUSB(&FPGAusb, USB_OUTPUT_DEVICE_NAME, 2);
#endif
		if(FPGAusb.status != FT_OK) {
			theApp.NoUSB = TRUE;
		}
		else {
			theApp.NoUSB = FALSE;
		}

		if(!theApp.NoUSB && theApp.GVars->bNoUIBeforeFPGAReady) {


			theApp.StartWaitDialog("INITIALIZING...");

		
#ifdef USE_CHANNEL_2_FOR_BACKCHANNEL
			InitUSB(&AUXusb, USB_AUX_DEVICE_NAME, 2);
#else
			InitUSB(&AUXusb, USB_AUX_DEVICE_NAME, 0);
#endif
			if(AUXusb.status != FT_OK) {
				theApp.NoUSB = TRUE;
			}
			else {
				theApp.NoUSB = FALSE;
			}
		}

	}

	if(theApp.NoUSB) {
		theDlg->m_SymPrint.ShowWindow(SW_SHOW);
	}

	//theJobDlg->m_ConfirmBands.SetCheck(theApp.GVars->ConfirmBands);

	theApp.ThreadSA.nLength = sizeof(SECURITY_ATTRIBUTES);
	theApp.ThreadSA.bInheritHandle = TRUE;
	theApp.ThreadSA.lpSecurityDescriptor  = NULL;


	theApp.WeaverThread = AfxBeginThread((AFX_THREADPROC)Weaver,
														(void *)NULL,
														0,
														0,	
														0,
														&theApp.ThreadSA);

	theApp.TempHandlerThread = AfxBeginThread((AFX_THREADPROC)TempHandler,
												0,
												THREAD_PRIORITY_NORMAL,
												0,
												0,
												&theApp.ThreadSA);

	/// MOE
	if(!theApp.GVars->bNoLogging) // SupressLogging was set TRUE in ini file
	{
#ifdef USE_MOE_LOGGING
		if (theApp.NetworkDLLinterface.bNetworkDLLloaded) // Failed to load DLL above so dont start its thread
		{
#endif
			theDlg->StatusLine("Networking Thread");  //LogUpdateHandler,  theApp.NetworkDLLinterface.IPtaskHandler(pParamDummy)
#ifdef USE_MOE_LOGGING
		theApp.LogHandlerThread = AfxBeginThread((AFX_THREADPROC)IPtaskHandler,
#else
		theApp.LogHandlerThread = AfxBeginThread((AFX_THREADPROC)LogUpdateHandler,
#endif
														0,
														THREAD_PRIORITY_LOWEST,
														0,
														0,
														&theApp.ThreadSA);
			theDlg->StatusLine(" - OK\n");
#ifdef USE_MOE_LOGGING
		}
	/// MOE
#endif
	}

	theApp.Inited = 1;

	/**/ TRACE("Setting GMasterStatus to idle (464)\n");
	theApp.GMasterStatus = MASTER_STATUS_IDLE;

	USBPrinterCommand(USB_CMD_PING, 1);

	/*
	USBPrinterCommand(USB_CMD_SET_HEADTEMP, theApp.GVars->HeadTemp[0]);
	USBPrinterCommand(USB_CMD_SET_HEADTEMP2, theApp.GVars->HeadTemp[1]);
	USBPrinterCommand(USB_CMD_SET_BUTTONMODE, theApp.GVars->bOneButtonOperation);
	USBPrinterCommand(USB_CMD_ENABLE_HEATERS, 1);
	*/

	theApp.GVars->bUseHeaters = 1;

	if(theApp.CurrentJob == NULL) {
		//theApp.CurrentJob = (void *)theApp.q_item_top;

		/**/TRACE("CurJobCS lock 1\n");
		theApp.CurJobCS.Lock(INFINITE);
		theApp.CurrentJob = (void *)theApp.FindJobByNumber(theApp.GVars->CurrentJobNumber, 1);
		/**/TRACE("CurJobCS unlock 1\n");
		theApp.CurJobCS.Unlock();

	}


	theDlg->PreparePreview();

	RECT frc;
	theDlg->GetClientRect(&frc);
	theDlg->InvalidateRect(&frc);

	if(theApp.GVars->bDualCMYK) {
		theDlg->m_BagMarker5.ShowWindow(SW_HIDE);
	}

	theApp.GetEncryptedRegistryEntries(theApp.GVars->szSN, &theApp.GVars->TotalJobs, NULL, NULL);

}


/*----------------------------------------------------------------*/
/*-   Send status message to message window                      -*/
/*----------------------------------------------------------------*/
void CController4Dlg1::StatusLine(char *message,...) {
int 	len;
char	tmpbuf[1024];
va_list	args;
static unsigned char n_virgin;

	if(theApp.GVars == NULL || theApp.GVars->bHideDebugWindows) {
		return;
	}

	if(theApp.quit_threads)
		return;

	va_start (args, message);
	vsprintf (tmpbuf, message, args);
	va_end (args);

	len = strlen(tmpbuf);

	if(!n_virgin) {
		m_Status1.SetLimitText(MESSAGE_BUFFER_SIZE);
		n_virgin = 1;
	}

	if(len + m_CharsInWindow >= MESSAGE_BUFFER_SIZE) {
		m_Status1.SetWindowText("");
		m_CharsInWindow = 0;
	}

	m_Status1.SetSel(m_CharsInWindow, m_CharsInWindow,FALSE);
	m_Status1.ReplaceSel(tmpbuf,FALSE);

	m_CharsInWindow += len;
	
}



void CController4Dlg1::DefineButtonAreas() {

	memset((char *)ButtonArea, 0, sizeof(polybuttonarea) * TOTAL_BUTTONS);

	/// Define Tools Button
	//////////////////////////////////
	ButtonArea[TOOLS_BUTTON].slices = 9;

	ButtonArea[TOOLS_BUTTON].slice[0].left = CIRCLE_LEFT_BOUNDARY + 205;
	ButtonArea[TOOLS_BUTTON].slice[0].top = CIRCLE_TOP_BOUDARY;
	ButtonArea[TOOLS_BUTTON].slice[0].right = CIRCLE_LEFT_BOUNDARY + 255;
	ButtonArea[TOOLS_BUTTON].slice[0].bottom = ButtonArea[TOOLS_BUTTON].slice[0].top + SLICE_HEIGHT;

	ButtonArea[TOOLS_BUTTON].slice[1].left = CIRCLE_LEFT_BOUNDARY + 120;
	ButtonArea[TOOLS_BUTTON].slice[1].top = ButtonArea[TOOLS_BUTTON].slice[0].bottom + 1;
	ButtonArea[TOOLS_BUTTON].slice[1].right = CIRCLE_LEFT_BOUNDARY + 255;
	ButtonArea[TOOLS_BUTTON].slice[1].bottom = ButtonArea[TOOLS_BUTTON].slice[1].top + SLICE_HEIGHT;

	ButtonArea[TOOLS_BUTTON].slice[2].left = CIRCLE_LEFT_BOUNDARY + 70;
	ButtonArea[TOOLS_BUTTON].slice[2].top = ButtonArea[TOOLS_BUTTON].slice[1].bottom + 1;
	ButtonArea[TOOLS_BUTTON].slice[2].right = CIRCLE_LEFT_BOUNDARY + 255;
	ButtonArea[TOOLS_BUTTON].slice[2].bottom = ButtonArea[TOOLS_BUTTON].slice[2].top + SLICE_HEIGHT;

	ButtonArea[TOOLS_BUTTON].slice[3].left = CIRCLE_LEFT_BOUNDARY + 40;
	ButtonArea[TOOLS_BUTTON].slice[3].top = ButtonArea[TOOLS_BUTTON].slice[2].bottom + 1;
	ButtonArea[TOOLS_BUTTON].slice[3].right = CIRCLE_LEFT_BOUNDARY + 165;
	ButtonArea[TOOLS_BUTTON].slice[3].bottom = ButtonArea[TOOLS_BUTTON].slice[3].top + SLICE_HEIGHT;

	ButtonArea[TOOLS_BUTTON].slice[4].left = CIRCLE_LEFT_BOUNDARY + 20;
	ButtonArea[TOOLS_BUTTON].slice[4].top = ButtonArea[TOOLS_BUTTON].slice[3].bottom + 1;
	ButtonArea[TOOLS_BUTTON].slice[4].right = CIRCLE_LEFT_BOUNDARY + 135;
	ButtonArea[TOOLS_BUTTON].slice[4].bottom = ButtonArea[TOOLS_BUTTON].slice[4].top + SLICE_HEIGHT;

	ButtonArea[TOOLS_BUTTON].slice[5].left = CIRCLE_LEFT_BOUNDARY + 8;
	ButtonArea[TOOLS_BUTTON].slice[5].top = ButtonArea[TOOLS_BUTTON].slice[4].bottom + 1;
	ButtonArea[TOOLS_BUTTON].slice[5].right = CIRCLE_LEFT_BOUNDARY + 122;
	ButtonArea[TOOLS_BUTTON].slice[5].bottom = ButtonArea[TOOLS_BUTTON].slice[5].top + SLICE_HEIGHT;

	ButtonArea[TOOLS_BUTTON].slice[6].left = CIRCLE_LEFT_BOUNDARY;
	ButtonArea[TOOLS_BUTTON].slice[6].top = ButtonArea[TOOLS_BUTTON].slice[5].bottom + 1;
	ButtonArea[TOOLS_BUTTON].slice[6].right = CIRCLE_LEFT_BOUNDARY + 119;
	ButtonArea[TOOLS_BUTTON].slice[6].bottom = ButtonArea[TOOLS_BUTTON].slice[6].top + SLICE_HEIGHT;

	ButtonArea[TOOLS_BUTTON].slice[7].left = CIRCLE_LEFT_BOUNDARY + 8;
	ButtonArea[TOOLS_BUTTON].slice[7].top = ButtonArea[TOOLS_BUTTON].slice[6].bottom + 1;
	ButtonArea[TOOLS_BUTTON].slice[7].right = CIRCLE_LEFT_BOUNDARY + 119;
	ButtonArea[TOOLS_BUTTON].slice[7].bottom = ButtonArea[TOOLS_BUTTON].slice[7].top + SLICE_HEIGHT;

	ButtonArea[TOOLS_BUTTON].slice[8].left = CIRCLE_LEFT_BOUNDARY + 20;
	ButtonArea[TOOLS_BUTTON].slice[8].top = ButtonArea[TOOLS_BUTTON].slice[7].bottom + 1;
	ButtonArea[TOOLS_BUTTON].slice[8].right = CIRCLE_LEFT_BOUNDARY + 70;
	ButtonArea[TOOLS_BUTTON].slice[8].bottom = ButtonArea[TOOLS_BUTTON].slice[8].top + SLICE_HEIGHT;


	/// Define Maintenance Button
	//////////////////////////////////
	ButtonArea[MAINT_BUTTON].slices = 9;

	ButtonArea[MAINT_BUTTON].slice[0].left = CIRCLE_RIGHT_BOUNDARY - 255;
	ButtonArea[MAINT_BUTTON].slice[0].top = CIRCLE_TOP_BOUDARY;
	ButtonArea[MAINT_BUTTON].slice[0].right = ButtonArea[MAINT_BUTTON].slice[0].left + 50;
	ButtonArea[MAINT_BUTTON].slice[0].bottom = ButtonArea[MAINT_BUTTON].slice[0].top + SLICE_HEIGHT;
	
	ButtonArea[MAINT_BUTTON].slice[1].left = CIRCLE_RIGHT_BOUNDARY - 255;
	ButtonArea[MAINT_BUTTON].slice[1].top = ButtonArea[MAINT_BUTTON].slice[0].bottom + 1;
	ButtonArea[MAINT_BUTTON].slice[1].right = CIRCLE_RIGHT_BOUNDARY - 120;
	ButtonArea[MAINT_BUTTON].slice[1].bottom = ButtonArea[MAINT_BUTTON].slice[1].top + SLICE_HEIGHT;

	ButtonArea[MAINT_BUTTON].slice[2].left = CIRCLE_RIGHT_BOUNDARY - 255;
	ButtonArea[MAINT_BUTTON].slice[2].top = ButtonArea[MAINT_BUTTON].slice[1].bottom + 1;
	ButtonArea[MAINT_BUTTON].slice[2].right = CIRCLE_RIGHT_BOUNDARY - 78;
	ButtonArea[MAINT_BUTTON].slice[2].bottom = ButtonArea[MAINT_BUTTON].slice[2].top + SLICE_HEIGHT;

	ButtonArea[MAINT_BUTTON].slice[3].left = CIRCLE_RIGHT_BOUNDARY - 160;
	ButtonArea[MAINT_BUTTON].slice[3].top = ButtonArea[MAINT_BUTTON].slice[2].bottom + 1;
	ButtonArea[MAINT_BUTTON].slice[3].right = CIRCLE_RIGHT_BOUNDARY - 40;
	ButtonArea[MAINT_BUTTON].slice[3].bottom = ButtonArea[MAINT_BUTTON].slice[3].top + SLICE_HEIGHT;

	ButtonArea[MAINT_BUTTON].slice[4].left = CIRCLE_RIGHT_BOUNDARY - 132;
	ButtonArea[MAINT_BUTTON].slice[4].top = ButtonArea[MAINT_BUTTON].slice[3].bottom + 1;
	ButtonArea[MAINT_BUTTON].slice[4].right = CIRCLE_RIGHT_BOUNDARY - 22;
	ButtonArea[MAINT_BUTTON].slice[4].bottom = ButtonArea[MAINT_BUTTON].slice[4].top + SLICE_HEIGHT;

	ButtonArea[MAINT_BUTTON].slice[5].left =CIRCLE_RIGHT_BOUNDARY - 120;
	ButtonArea[MAINT_BUTTON].slice[5].top = ButtonArea[MAINT_BUTTON].slice[4].bottom + 1;
	ButtonArea[MAINT_BUTTON].slice[5].right = CIRCLE_RIGHT_BOUNDARY - 5;
	ButtonArea[MAINT_BUTTON].slice[5].bottom = ButtonArea[MAINT_BUTTON].slice[5].top + SLICE_HEIGHT;

	ButtonArea[MAINT_BUTTON].slice[6].left = CIRCLE_RIGHT_BOUNDARY - 119;
	ButtonArea[MAINT_BUTTON].slice[6].top = ButtonArea[MAINT_BUTTON].slice[5].bottom + 1;
	ButtonArea[MAINT_BUTTON].slice[6].right = CIRCLE_RIGHT_BOUNDARY;
	ButtonArea[MAINT_BUTTON].slice[6].bottom = ButtonArea[MAINT_BUTTON].slice[6].top + SLICE_HEIGHT;

	ButtonArea[MAINT_BUTTON].slice[7].left = CIRCLE_RIGHT_BOUNDARY - 121;
	ButtonArea[MAINT_BUTTON].slice[7].top = ButtonArea[MAINT_BUTTON].slice[6].bottom + 1;
	ButtonArea[MAINT_BUTTON].slice[7].right = CIRCLE_RIGHT_BOUNDARY - 5;
	ButtonArea[MAINT_BUTTON].slice[7].bottom = ButtonArea[MAINT_BUTTON].slice[7].top + SLICE_HEIGHT;

	ButtonArea[MAINT_BUTTON].slice[8].left = CIRCLE_RIGHT_BOUNDARY - 70;
	ButtonArea[MAINT_BUTTON].slice[8].top = ButtonArea[MAINT_BUTTON].slice[7].bottom + 1;
	ButtonArea[MAINT_BUTTON].slice[8].right = CIRCLE_RIGHT_BOUNDARY - 18;
	ButtonArea[MAINT_BUTTON].slice[8].bottom = ButtonArea[MAINT_BUTTON].slice[8].top + SLICE_HEIGHT;


	/// Define Settings Button
	//////////////////////////////////
	ButtonArea[SETTINGS_BUTTON].slices = 5;

	ButtonArea[SETTINGS_BUTTON].slice[0].left = CIRCLE_LEFT_BOUNDARY + 100;
	ButtonArea[SETTINGS_BUTTON].slice[0].top = CIRCLE_TOP_BOUDARY + CIRCLE_HEIGHT - (SLICE_HEIGHT * 4);
	ButtonArea[SETTINGS_BUTTON].slice[0].right = ButtonArea[SETTINGS_BUTTON].slice[0].left  + 70;
	ButtonArea[SETTINGS_BUTTON].slice[0].bottom = ButtonArea[SETTINGS_BUTTON].slice[0].top + SLICE_HEIGHT;
	
	ButtonArea[SETTINGS_BUTTON].slice[1].left = CIRCLE_RIGHT_BOUNDARY - 175;
	ButtonArea[SETTINGS_BUTTON].slice[1].top = CIRCLE_TOP_BOUDARY + CIRCLE_HEIGHT - (SLICE_HEIGHT * 4);
	ButtonArea[SETTINGS_BUTTON].slice[1].right = ButtonArea[SETTINGS_BUTTON].slice[1].left + 70;
	ButtonArea[SETTINGS_BUTTON].slice[1].bottom = ButtonArea[SETTINGS_BUTTON].slice[1].top + SLICE_HEIGHT;
	
	ButtonArea[SETTINGS_BUTTON].slice[2].left = CIRCLE_LEFT_BOUNDARY + 80;
	ButtonArea[SETTINGS_BUTTON].slice[2].top = ButtonArea[SETTINGS_BUTTON].slice[1].bottom + 1;
	ButtonArea[SETTINGS_BUTTON].slice[2].right = CIRCLE_RIGHT_BOUNDARY - 80;
	ButtonArea[SETTINGS_BUTTON].slice[2].bottom = ButtonArea[SETTINGS_BUTTON].slice[2].top + SLICE_HEIGHT;

	ButtonArea[SETTINGS_BUTTON].slice[3].left = CIRCLE_LEFT_BOUNDARY + 140;
	ButtonArea[SETTINGS_BUTTON].slice[3].top = ButtonArea[SETTINGS_BUTTON].slice[2].bottom + 1;
	ButtonArea[SETTINGS_BUTTON].slice[3].right = CIRCLE_RIGHT_BOUNDARY - 140;
	ButtonArea[SETTINGS_BUTTON].slice[3].bottom = ButtonArea[SETTINGS_BUTTON].slice[3].top + SLICE_HEIGHT;

	ButtonArea[SETTINGS_BUTTON].slice[4].left = CIRCLE_LEFT_BOUNDARY + 210;
	ButtonArea[SETTINGS_BUTTON].slice[4].top = ButtonArea[SETTINGS_BUTTON].slice[3].bottom + 1;
	ButtonArea[SETTINGS_BUTTON].slice[4].right = CIRCLE_RIGHT_BOUNDARY - 210;
	ButtonArea[SETTINGS_BUTTON].slice[4].bottom = ButtonArea[SETTINGS_BUTTON].slice[4].top + SLICE_HEIGHT - 10;


	/// Define Center Button
	//////////////////////////////////
	ButtonArea[CENTER_BUTTON].slices = 6;

	ButtonArea[CENTER_BUTTON].slice[0].left = CIRCLE_LEFT_BOUNDARY + 210;
	ButtonArea[CENTER_BUTTON].slice[0].top = CIRCLE_TOP_BOUDARY + (SLICE_HEIGHT * 3) + 25;
	ButtonArea[CENTER_BUTTON].slice[0].right = CIRCLE_RIGHT_BOUNDARY - 215;
	ButtonArea[CENTER_BUTTON].slice[0].bottom = ButtonArea[CENTER_BUTTON].slice[0].top + SLICE_HEIGHT;

	ButtonArea[CENTER_BUTTON].slice[1].left = CIRCLE_LEFT_BOUNDARY + 170;
	ButtonArea[CENTER_BUTTON].slice[1].top = ButtonArea[CENTER_BUTTON].slice[0].bottom + 1;
	ButtonArea[CENTER_BUTTON].slice[1].right = CIRCLE_RIGHT_BOUNDARY - 170;
	ButtonArea[CENTER_BUTTON].slice[1].bottom = ButtonArea[CENTER_BUTTON].slice[1].top + SLICE_HEIGHT;

	ButtonArea[CENTER_BUTTON].slice[2].left = CIRCLE_LEFT_BOUNDARY + 145;
	ButtonArea[CENTER_BUTTON].slice[2].top = ButtonArea[CENTER_BUTTON].slice[1].bottom + 1;
	ButtonArea[CENTER_BUTTON].slice[2].right = CIRCLE_RIGHT_BOUNDARY - 145;
	ButtonArea[CENTER_BUTTON].slice[2].bottom = ButtonArea[CENTER_BUTTON].slice[2].top + SLICE_HEIGHT;

	ButtonArea[CENTER_BUTTON].slice[3].left = CIRCLE_LEFT_BOUNDARY + 150;
	ButtonArea[CENTER_BUTTON].slice[3].top = ButtonArea[CENTER_BUTTON].slice[2].bottom + 1;
	ButtonArea[CENTER_BUTTON].slice[3].right = CIRCLE_RIGHT_BOUNDARY - 150;
	ButtonArea[CENTER_BUTTON].slice[3].bottom = ButtonArea[CENTER_BUTTON].slice[3].top + SLICE_HEIGHT;

	ButtonArea[CENTER_BUTTON].slice[4].left = CIRCLE_LEFT_BOUNDARY + 170;
	ButtonArea[CENTER_BUTTON].slice[4].top = ButtonArea[CENTER_BUTTON].slice[3].bottom + 1;
	ButtonArea[CENTER_BUTTON].slice[4].right = CIRCLE_RIGHT_BOUNDARY - 170;
	ButtonArea[CENTER_BUTTON].slice[4].bottom = ButtonArea[CENTER_BUTTON].slice[4].top + SLICE_HEIGHT;

	ButtonArea[CENTER_BUTTON].slice[5].left = CIRCLE_LEFT_BOUNDARY + 210;
	ButtonArea[CENTER_BUTTON].slice[5].top = ButtonArea[CENTER_BUTTON].slice[4].bottom + 1;
	ButtonArea[CENTER_BUTTON].slice[5].right = CIRCLE_RIGHT_BOUNDARY - 215;
	ButtonArea[CENTER_BUTTON].slice[5].bottom = ButtonArea[CENTER_BUTTON].slice[5].top + SLICE_HEIGHT;

	ButtonArea[MAIN_COUNTER_BUTTON].slices = 1;
	ButtonArea[MAIN_COUNTER_BUTTON].slice[0].left = 45;
	ButtonArea[MAIN_COUNTER_BUTTON].slice[0].top = 655;
	ButtonArea[MAIN_COUNTER_BUTTON].slice[0].right = ButtonArea[MAIN_COUNTER_BUTTON].slice[0].left + 135;
	ButtonArea[MAIN_COUNTER_BUTTON].slice[0].bottom = ButtonArea[MAIN_COUNTER_BUTTON].slice[0].top + 70;

	ButtonArea[CURRENT_COUNTER_BUTTON].slices = 1;
	ButtonArea[CURRENT_COUNTER_BUTTON].slice[0].left = 470;
	ButtonArea[CURRENT_COUNTER_BUTTON].slice[0].top = ButtonArea[MAIN_COUNTER_BUTTON].slice[0].top;
	ButtonArea[CURRENT_COUNTER_BUTTON].slice[0].right = ButtonArea[CURRENT_COUNTER_BUTTON].slice[0].left + 135;
	ButtonArea[CURRENT_COUNTER_BUTTON].slice[0].bottom = ButtonArea[MAIN_COUNTER_BUTTON].slice[0].bottom;



	/// Job Area
	//////////////////////////////////
	ButtonArea[JOB_BUTTON].slices = 1;

	ButtonArea[JOB_BUTTON].slice[0].left = 700;
	ButtonArea[JOB_BUTTON].slice[0].top = 30;
	ButtonArea[JOB_BUTTON].slice[0].right = ButtonArea[JOB_BUTTON].slice[0].left + 600;
	ButtonArea[JOB_BUTTON].slice[0].bottom = ButtonArea[JOB_BUTTON].slice[0].top + 650;



	// Current Counter location
	GetClientRect(&current_counter_rect);
	current_counter_rect.top = current_counter_rect.bottom - 118;
	current_counter_rect.left = ButtonArea[CURRENT_COUNTER_BUTTON].slice[0].left;
	current_counter_rect.right = ButtonArea[CURRENT_COUNTER_BUTTON].slice[0].right;



	/// Vanity Screen
	//////////////////////////////////
	ButtonArea[VANITY_SCREEN].slices = 1;
	ButtonArea[VANITY_SCREEN].slice[0].left = 0;
	ButtonArea[VANITY_SCREEN].slice[0].top = WINDOW_HEIGHT - 50;
	ButtonArea[VANITY_SCREEN].slice[0].right = ButtonArea[VANITY_SCREEN].slice[0].left + 50;
	ButtonArea[VANITY_SCREEN].slice[0].bottom = ButtonArea[VANITY_SCREEN].slice[0].top + 50;


	/// Job Name
	GetClientRect(&job_name_rect);
	job_name_rect.left = CIRCLE_WIDTH + 170 ;
	job_name_rect.right -= 60;
	job_name_rect.top = job_name_rect.bottom - 125;
	job_name_rect.bottom = job_name_rect.top + 35;


	////  Print Mode;
	GetClientRect(&print_mode_rect);
	print_mode_rect.left = CIRCLE_WIDTH + 170 ;
	print_mode_rect.right -= 60;
	print_mode_rect.top = job_name_rect.bottom;
	print_mode_rect.bottom = print_mode_rect.top + 35;

	////  Size rectangle;
	size_rect.left = print_mode_rect.left;
	size_rect.right = print_mode_rect.right;
	size_rect.top = print_mode_rect.bottom;
	size_rect.bottom = size_rect.top + 40;

	////  Job number
	job_number_rect.left = size_rect.left;
	job_number_rect.right = size_rect.right + 40;
	job_number_rect.top = size_rect.top;
	job_number_rect.bottom = size_rect.bottom;

	// Head Temp location
	GetClientRect(&temp_rect);
	temp_rect.top = temp_rect.bottom - 60;
	temp_rect.left = CIRCLE_LEFT_BOUNDARY;
	temp_rect.right = CIRCLE_RIGHT_BOUNDARY;

	// Global Counter location
	GetClientRect(&counter_rect);
	counter_rect.top = counter_rect.bottom - 118;
	counter_rect.left = ButtonArea[MAIN_COUNTER_BUTTON].slice[0].left;
	counter_rect.right = ButtonArea[MAIN_COUNTER_BUTTON].slice[0].right;


	// Current Counter location
	GetClientRect(&current_counter_rect);
	current_counter_rect.top = current_counter_rect.bottom - 118;
	current_counter_rect.left = ButtonArea[CURRENT_COUNTER_BUTTON].slice[0].left;
	current_counter_rect.right = ButtonArea[CURRENT_COUNTER_BUTTON].slice[0].right;



}


void CController4Dlg1::SizeDialog() {


	WINDOWPLACEMENT Option1WP, Option2WP, Option3WP, Option4WP, StatusWP;

	GetWindowPlacement(&theApp.MainWP);

	if(theApp.GVars != NULL && !theApp.GVars->bHideDebugWindows) {
		m_Status1.GetWindowPlacement(&StatusWP);
		m_Status1.SetWindowPlacement(&StatusWP);
	}

	theApp.MainWP.rcNormalPosition.right = theApp.MainWP.rcNormalPosition.left + WINDOW_WIDTH;
	theApp.MainWP.rcNormalPosition.bottom = theApp.MainWP.rcNormalPosition.top + WINDOW_HEIGHT;
	SetWindowPlacement(&theApp.MainWP);

	GetWindowPlacement(&theApp.MasterWP);

}

void CController4Dlg1::LoadBackgroundBitmaps() {

	if(!theApp.bPauseMode) {
		hMainBackground.LoadBitmap(IDB_MAIN_BACKGROUND_P);
		hMainBackgroundSettings.LoadBitmap(IDB_MAIN_BACKGROUND_SETTINGS_P);
		hMainBackgroundMaint.LoadBitmap(IDB_MAIN_BACKGROUND_MAINT_P);
		hMainBackgroundTools.LoadBitmap(IDB_MAIN_BACKGROUND_TOOLS_P);
		hMainBackgroundCenter.LoadBitmap(IDB_MAIN_BACKGROUND_CENTER_P);
		//hMainBackgroundPower.LoadBitmap(IDB_BACKGROUND_POWER);
	}

	else {
		hMainBackground.LoadBitmap(IDB_MAIN_BACKGROUND);
		hMainBackgroundSettings.LoadBitmap(IDB_MAIN_BACKGROUND_SETTINGS);
		hMainBackgroundMaint.LoadBitmap(IDB_MAIN_BACKGROUND_MAINT);
		hMainBackgroundTools.LoadBitmap(IDB_MAIN_BACKGROUND_TOOLS);
		hMainBackgroundCenter.LoadBitmap(IDB_MAIN_BACKGROUND_CENTER);
		//hMainBackgroundPower.LoadBitmap(IDB_BACKGROUND_POWER);
	}

}

BOOL CController4Dlg1::OnInitDialog()
{
int i;


	//OpenLogFile();

	bButtonDown = FALSE;

	currentDlg = NULL;

	theApp.ScannerDelay = 0;

	iBlinkObstruction = 0;
	bObstructionMarkerState = FALSE;

	theApp.time_of_last_print = time(0); // initial state...

	CartWarnImage[0] = &hCartWarnF;
	CartWarnImage[1] = &hCartWarnY;
	CartWarnImage[2] = &hCartWarnM;
	CartWarnImage[3] = &hCartWarnC;
	CartWarnImage[4] = &hCartWarnK;
	CartWarnImage[5] = &hCartWarnW;

	hCartWarnF.LoadBitmap(IDB_CART_WARN_F);
	hCartWarnY.LoadBitmap(IDB_CART_WARN_Y);
	hCartWarnM.LoadBitmap(IDB_CART_WARN_M);
	hCartWarnC.LoadBitmap(IDB_CART_WARN_C);
	hCartWarnK.LoadBitmap(IDB_CART_WARN_K);
	hCartWarnW.LoadBitmap(IDB_CART_WARN_W);

	memset((char *)&Bitmap, 0, sizeof(BITMAPHANDLE));

	theApp.currentWindow	= MAIN_RUNNING_WINDOW;

	TRACE("currentwindow: %d\n", theApp.currentWindow);

	theApp.bHomePressed = FALSE;

	GotPrevSize = FALSE;

	m_CharsInWindow = 0;

	CDialogEx::OnInitDialog();

	WINDOWPLACEMENT bmwp;

	m_BagMarker0.GetWindowPlacement(&bmwp);
	InkMarkersOriginal[0].SetRect(	bmwp.rcNormalPosition.left,
							bmwp.rcNormalPosition.top,
							bmwp.rcNormalPosition.right,
							bmwp.rcNormalPosition.bottom);

	m_BagMarker1.GetWindowPlacement(&bmwp);
	InkMarkersOriginal[1].SetRect(	bmwp.rcNormalPosition.left,
							bmwp.rcNormalPosition.top,
							bmwp.rcNormalPosition.right,
							bmwp.rcNormalPosition.bottom);

	m_BagMarker2.GetWindowPlacement(&bmwp);
	InkMarkersOriginal[2].SetRect(	bmwp.rcNormalPosition.left,
							bmwp.rcNormalPosition.top,
							bmwp.rcNormalPosition.right,
							bmwp.rcNormalPosition.bottom);

	m_BagMarker3.GetWindowPlacement(&bmwp);
	InkMarkersOriginal[3].SetRect(	bmwp.rcNormalPosition.left,
							bmwp.rcNormalPosition.top,
							bmwp.rcNormalPosition.right,
							bmwp.rcNormalPosition.bottom);

	m_BagMarker4.GetWindowPlacement(&bmwp);
	InkMarkersOriginal[4].SetRect(	bmwp.rcNormalPosition.left,
							bmwp.rcNormalPosition.top,
							bmwp.rcNormalPosition.right,
							bmwp.rcNormalPosition.bottom);

	m_BagMarker5.GetWindowPlacement(&bmwp);
	InkMarkersOriginal[5].SetRect(	bmwp.rcNormalPosition.left,
							bmwp.rcNormalPosition.top,
							bmwp.rcNormalPosition.right,
							bmwp.rcNormalPosition.bottom);

	// m_Status1.ShowWindow(SW_HIDE);

	theDlg = this;

	theApp.q_item_top = NULL;
	theApp.QueueItems = 0;
	theApp.processing_qitem = NULL;
	queue = NULL;

	theApp.AppInitThread = NULL;
	theApp.WorkerThread = NULL;

	theApp.Inited = FALSE;

	theApp.qitem = FALSE;
	theApp.processing_qitem = FALSE;

	///////////////////////////////
	///////////////////////////////

	// Dark Gray brush
	lbDrakGrayBackground.lbStyle = BS_SOLID;
	lbDrakGrayBackground.lbColor = RGB(135, 121, 136);
	DrakGrayBackground.CreateBrushIndirect(&lbDrakGrayBackground);

	SizeDialog();


	CFont* pOldFont = this->GetFont(); // m_Status1.GetFont();

	LOGFONT logfont;
	LOGFONT logfont2;
	LOGFONT logfont3;
	LOGFONT logfont4;
	LOGFONT logfont5;
	LOGFONT logfont6;
	LOGFONT logfontvert;

	pOldFont->GetObject(sizeof(logfont), &logfont);
 
	logfont.lfHeight = 30;
	logfont.lfWidth = 15;


	logfont.lfWeight = FW_THIN;
	logfont.lfPitchAndFamily = FF_MODERN;
	strcpy(logfont.lfFaceName, "Courier");  
	logfont.lfEscapement = logfont.lfOrientation = 0;
	pNewFont.CreateFontIndirect(&logfont);
 
	if(theApp.GVars != NULL && !theApp.GVars->bHideDebugWindows) {
		m_Status1.SetFont(&pNewFont);
	}

	pOldFont->GetObject(sizeof(logfont), &logfont2);
	
	CFont* pOldFont2 = this->GetFont(); // m_Status1.GetFont();
	pOldFont2->GetObject(sizeof(logfont),&logfont2);
 
	//logfont2.lfHeight = 35;
	logfont2.lfHeight = 25;
	//logfont2.lfWidth = 15;
	logfont2.lfWidth = 10;
	logfont2.lfWeight = FW_THIN;
	logfont2.lfPitchAndFamily = FF_DONTCARE;
	/**/strcpy(logfont2.lfFaceName, "Helvetica");  
	//strcpy(logfont2.lfFaceName, "Tahoma");  
	logfont2.lfEscapement = logfont2.lfOrientation = 0;
	pMediumFont.CreateFontIndirect(&logfont2);


	CFont* pOldFont3 = this->GetFont(); // m_Status1.GetFont();
	pOldFont3->GetObject(sizeof(logfont),&logfont3);
 
	logfont3.lfHeight = 17;
	logfont3.lfWidth = 7;
	logfont3.lfWeight = FW_THIN;
	logfont3.lfPitchAndFamily = FF_DONTCARE;
	strcpy(logfont3.lfFaceName, "Helvetica");  
	logfont3.lfEscapement = 0;
	logfont3.lfOrientation = 0;
	pSmallFont.CreateFontIndirect(&logfont3);


	/*
	logfontvert.lfHeight = 17;
	logfontvert.lfWidth = 7;
	logfontvert.lfWeight = FW_THIN;
	logfontvert.lfPitchAndFamily = FF_DONTCARE;
	strcpy(logfont3.lfFaceName, "Helvetica");  
	logfontvert.lfEscapement = 0;
	logfontvert.lfOrientation = 0;
	// pSmallFontVert.CreateFontIndirect(&logfontvert);
	*/
	BOOL fok;
	fok = pSmallFontVert.CreateFont(17,7,900,0,FALSE,FALSE,FALSE,0,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_ROMAN,"Helvetica");
	//fok = pSmallFontVert.CreateFont(17,7,450,0,FALSE,FALSE,FALSE,0,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_ROMAN,"Helvetica");


	CFont* pOldFont4 = this->GetFont(); // m_Status1.GetFont();
	pOldFont4->GetObject(sizeof(logfont),&logfont4);

	logfont4.lfHeight = 23;
	logfont4.lfWidth = 8;
	logfont4.lfWeight = FW_THIN;
	logfont4.lfPitchAndFamily = FF_DONTCARE;
	strcpy(logfont4.lfFaceName, "Helvetica");  
	logfont4.lfEscapement = logfont4.lfOrientation = 0;
	pSmallFont2.CreateFontIndirect(&logfont4);


	CFont* pOldFont5 = this->GetFont(); // m_Status1.GetFont();
	pOldFont5->GetObject(sizeof(logfont),&logfont5);

	logfont5.lfHeight = 53;
	logfont5.lfWidth = 28;
	logfont5.lfWeight = FW_THIN;
	logfont5.lfPitchAndFamily = FF_DONTCARE;
	strcpy(logfont5.lfFaceName, "Let's go Digital");  
	logfont5.lfEscapement = logfont5.lfOrientation = 0;
	pDigitalFont.CreateFontIndirect(&logfont5);


	CFont* pOldFont6 = this->GetFont(); // m_Status1.GetFont();
	pOldFont6->GetObject(sizeof(logfont),&logfont6);

	logfont6.lfHeight = 60;
	logfont6.lfWidth = 25;
	logfont6.lfWeight = FW_BOLD;
	logfont6.lfPitchAndFamily = FF_DONTCARE;
	strcpy(logfont6.lfFaceName, "Helvetica");  
	logfont6.lfEscapement = logfont6.lfOrientation = 0;
	pLargeFont.CreateFontIndirect(&logfont6);


	hScanningGreen.LoadBitmap(IDB_SCAN_INDICATOR_GREEN);
	hScanningYellow.LoadBitmap(IDB_SCAN_INDICATOR_YELLOW);
	hScanningRed.LoadBitmap(IDB_SCAN_INDICATOR_RED);


	mb_mem = GlobalAlloc(GHND|GMEM_ZEROINIT, MESSAGE_BUFFER_SIZE);

	if(theApp.GVars != NULL && !theApp.GVars->bHideDebugWindows) {
		m_Status1.SetHandle(mb_mem);
	}


	POINT pt;
	pt.x = 1;
	pt.y = 1;

	logPen.lopnStyle = PS_SOLID;
	logPen.lopnColor = RGB(0, 0, 0);
	logPen.lopnWidth = pt;
	hCPen.CreatePenIndirect(&logPen);

	POINT lpt;
	lpt.x = 4;
	lpt.y = 4;

	logPenX.lopnStyle = PS_SOLID;
	logPenX.lopnColor = RGB(255, 0, 0);
	logPenX.lopnWidth = lpt;
	hCPenX.CreatePenIndirect(&logPenX);

	logPenX2.lopnStyle = PS_SOLID;
	logPenX2.lopnColor = RGB(255, 255, 255);
	logPenX2.lopnWidth = lpt;
	hCPenX2.CreatePenIndirect(&logPenX2);


	logBrushWhite.lbStyle = BS_SOLID;
	logBrushWhite.lbColor = RGB(255, 255, 255);
	hBGbrushWhite.CreateBrushIndirect(&logBrushWhite);

	logBrushGray.lbStyle = BS_SOLID;
	logBrushGray.lbColor = RGB(137, 137, 137);
	hBGbrushGray.CreateBrushIndirect(&logBrushGray);


	dcScreen = m_Preview.GetDC();

	m_Preview.GetWindowPlacement(&pwp);
	m_XOffset = pwp.rcNormalPosition.left;
	m_YOffset = pwp.rcNormalPosition.top;

	m_ResX = GetDeviceCaps(dcScreen->m_hDC, HORZRES);
	m_ResY = GetDeviceCaps(dcScreen->m_hDC, VERTRES);

	m_AspectX = 1.0;
	m_AspectY = (float)m_ResX / (float)m_ResY;


	//::GetClientRect(m_Preview, &rc);
	
	m_Preview.GetClientRect(&rc);

	m_PicDC.CreateCompatibleDC(dcScreen);


	LoadBackgroundBitmaps();


	iPrevButtons = iButtons = 0;

	DefineButtonAreas();

	StatusLine("%s\r\n", PRODUCT_NAME);


	theApp.SendBandCS.Lock(INFINITE);
	theApp.SendBandCS.Unlock();


	theApp.ThreadSA.nLength = sizeof(SECURITY_ATTRIBUTES);
	theApp.ThreadSA.bInheritHandle = TRUE;
	theApp.ThreadSA.lpSecurityDescriptor  = NULL;

	theApp.quit_threads = FALSE;
	theApp.bRunWPThreads = FALSE;

	theApp.AppInitThread = AfxBeginThread((AFX_THREADPROC)	AppInitProc,
											(void *)NULL,
											0,
											0,
											0,
											&theApp.ThreadSA);


	theApp.ThreadSA.nLength = sizeof(SECURITY_ATTRIBUTES);
	theApp.ThreadSA.bInheritHandle = TRUE;
	theApp.ThreadSA.lpSecurityDescriptor  = NULL;


	theApp.HeadFormatterThread = AfxBeginThread((AFX_THREADPROC)	HeadFormatThread,
															(void *)NULL,
															//THREAD_PRIORITY_HIGHEST,
															/**/THREAD_PRIORITY_TIME_CRITICAL,
															0,
															0,
															&theApp.ThreadSA);

	theApp.hAuxUSBSendThread = AfxBeginThread((AFX_THREADPROC)AuxUSBSendThread,
												(void *)&AUXusb,
												THREAD_PRIORITY_TIME_CRITICAL,
												0,
												0,
												&theApp.ThreadSA);

	theApp.hScannerThread = AfxBeginThread((AFX_THREADPROC)HotFolderScanner,
												(void *)NULL,
												//THREAD_PRIORITY_NORMAL,
												THREAD_PRIORITY_BELOW_NORMAL,
												0,
												0,
												&theApp.ThreadSA);
	

	theApp.HeadFormatterThread->SuspendThread();
	theDlg->StatusLine("Halting Formatter Thread\n");

	SetWindowText(PRODUCT_NAME);

	m_nTimer = SetTimer(TEMP_DISPLAY_UPDATE2, 15000, 0);

	m_nTimer2 = SetTimer(HALF_SECOND_TIMER, 500, 0);

	theApp.currentWindow = MAIN_RUNNING_WINDOW;

	//m_PicDC.FillRect(&rc, &hBGbrushWhite);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// for debug only... draw the outline of all the button slices
void CController4Dlg1::DrawButtonSlices(CPaintDC *dc, int button) {
CDC *hdc;
int i;

	hdc = GetDC();
	hdc->SelectObject(&hCPen);

	for(i = 0; i < ButtonArea[button].slices; i++) {
		
		hdc->MoveTo(ButtonArea[button].slice[i].left, ButtonArea[button].slice[i].top);
		hdc->LineTo(ButtonArea[button].slice[i].right, ButtonArea[button].slice[i].top);
		hdc->LineTo(ButtonArea[button].slice[i].right, ButtonArea[button].slice[i].bottom);
		hdc->LineTo(ButtonArea[button].slice[i].left, ButtonArea[button].slice[i].bottom);
		hdc->LineTo(ButtonArea[button].slice[i].left, ButtonArea[button].slice[i].top);
	}

	ReleaseDC(hdc);

}

void CController4Dlg1::OnPaint()
{
int i;

	//theApp.DisplayCS.Lock(0);

	CPaintDC dc(this); // device context for painting

	switch(iButtons) {
		case	TOOLS_BUTTON:
			theApp.DisplayBitmap(&dc, &hMainBackgroundTools, 0, 0, DB_SHOWUPPERLEFT);
			break;
		case	MAINT_BUTTON:
			theApp.DisplayBitmap(&dc, &hMainBackgroundMaint, 0, 0, DB_SHOWUPPERLEFT);
			break;
		case	SETTINGS_BUTTON:
			theApp.DisplayBitmap(&dc, &hMainBackgroundSettings, 0, 0, DB_SHOWUPPERLEFT);
			break;
		case	CENTER_BUTTON:
			theApp.DisplayBitmap(&dc, &hMainBackgroundCenter, 0, 0, DB_SHOWUPPERLEFT);
			break;
		
		default:
			theApp.DisplayBitmap(&dc, &hMainBackground, 0, 0, DB_SHOWUPPERLEFT);
			break;

	}

	if(theApp.currentWindow == MAIN_RUNNING_WINDOW) {
		DisplayPreview();
		ShowTemp(&dc);
		ShowVersion(&dc);
		ShowCount(&dc);
		ShowCurrentCount(&dc);
	}

	BOOL bMissing = FALSE;
	if(theApp.GVars != NULL && !theApp.GVars->bDisableInkADC && (theApp.bReceivedReadyFromFPGA || !theApp.GVars->bNoUIBeforeFPGAReady)) {
		COLORREF fillClr;
		for(i = 0; i < 6; i++) {
			switch(i) {
				case 0:
					fillClr = RGB(180, 180, 180);
					bMissing = !theApp.GVars->Valves.bFBagPresent;
					break;
				case 1:
					fillClr = RGB(255, 255, 0);
					bMissing = !theApp.GVars->Valves.bYBagPresent;
					break;
				case 2:
					fillClr = RGB(255, 0, 255);
					bMissing = !theApp.GVars->Valves.bMBagPresent;
					break;
				case 3:
					fillClr = RGB(0, 255, 255);
					bMissing = !theApp.GVars->Valves.bCBagPresent;
					break;
				case 4:
					fillClr = RGB(0, 0, 0);
					bMissing = !theApp.GVars->Valves.bKBagPresent;
					break;
				case 5:
					if(theApp.GVars != NULL && !theApp.GVars->bDualCMYK) {
						fillClr = RGB(255, 255, 255);
					}
					else {
						fillClr = RGB(42, 177, 255);
					}
					bMissing = !theApp.GVars->Valves.bWBagPresent;
					break;
			}
			if(theApp.GVars != NULL) {
			
				if((theApp.GVars->bDualCMYK && i == 5)/* || bMissing*/)
					continue;

				if(bMissing) {
					dc.FillSolidRect((LPRECT)InkMarkersOriginal[i], fillClr);
					theApp.DisplayBitmap(&dc, CartWarnImage[i], InkMarkersOriginal[i].left, InkMarkersOriginal[i].top + 10, DB_SHOWATPOS);				
					continue;
				}


				theApp.CalculateInkFullVars();

				if((theApp.GVars->inkADC[i] == 0 || theApp.GVars->inkADC[i] == 1)) {

					dc.FillSolidRect((LPRECT)InkMarkersOriginal[i], fillClr);

					dc.SelectObject(&theDlg->pMediumFont);

					if(i == 2)
						dc.SetTextColor(RGB(255, 255, 255));
					else
						dc.SetTextColor(RGB(255, 0, 0));
					dc.SetBkMode(TRANSPARENT);
					CRect tr;
					tr.CopyRect(&InkMarkersOriginal[i]);
					tr.top += 15;
					dc.DrawText("?", 1, &tr, DT_CENTER|DT_VCENTER);
				}
				else {

					if(theApp.GVars->InkAlarm[i]) {
						dc.FillSolidRect((LPRECT)InkMarkersOriginal[i], fillClr);
						if(i == 2) {
							dc.SelectObject(&hCPenX2);
						}
						else {
							dc.SelectObject(&hCPenX);
						}
						dc.MoveTo(InkMarkersOriginal[i].left + X_MARKER_X_OFFSET, InkMarkersOriginal[i].top + X_MARKER_Y_OFFSET);
						dc.LineTo(InkMarkersOriginal[i].right - X_MARKER_X_OFFSET, InkMarkersOriginal[i].bottom - X_MARKER_Y_OFFSET);
						dc.MoveTo(InkMarkersOriginal[i].right - X_MARKER_X_OFFSET, InkMarkersOriginal[i].top + X_MARKER_Y_OFFSET);
						dc.LineTo(InkMarkersOriginal[i].left + X_MARKER_X_OFFSET, InkMarkersOriginal[i].bottom - X_MARKER_Y_OFFSET);
					}
					else {
						dc.FillSolidRect((LPRECT)InkMarkersDisplay[i], fillClr);
					}

				}
			}
		}
	}

	//theApp.DisplayCS.Unlock();


	// DrawButtonSlices(&dc, TOOLS_BUTTON);
	// DrawButtonSlices(&dc, MAINT_BUTTON);
	// DrawButtonSlices(&dc, SETTINGS_BUTTON);
	// DrawButtonSlices(&dc, CENTER_BUTTON);
	// DrawButtonSlices(&dc, JOB_BUTTON);
	// DrawButtonSlices(&dc, VANITY_SCREEN);
	// DrawButtonSlices(&dc, MAIN_COUNTER_BUTTON);
	// DrawButtonSlices(&dc, CURRENT_COUNTER_BUTTON);

}

void CController4Dlg1::RedrawInkMarkers(int channel) {
//int i;

	//for(i = 0; i < 5; i++) {
	
		InvalidateRect(theDlg->InkMarkersOriginal[channel], FALSE);		
	
	//}


}


int CController4Dlg1::FindPressedButton(CPoint *point) {
int b, i;

	for(b = 0; b < TOTAL_BUTTONS; b++) {

		for(i = 0; i < ButtonArea[b].slices; i++) {

			if( point->x > ButtonArea[b].slice[i].left &&
				point->x < ButtonArea[b].slice[i].right &&
				point->y > ButtonArea[b].slice[i].top &&
				point->y < ButtonArea[b].slice[i].bottom)

				return b;
		}

	}

	/// see if ink markers were touched
	/*
	if(theApp.GVars != NULL && !theApp.GVars->bDisableInkADC) {
		if(	theApp.GMasterStatus == MASTER_STATUS_IDLE &&
			point->x > InkMarkersOriginal[0].left &&
			point->x < InkMarkersOriginal[5].right &&
			point->y > InkMarkersOriginal[0].top &&
			point->y < InkMarkersOriginal[0].bottom) {
			TRACE("Getting ink levels (1)\n");
  			USBPrinterCommand(USB_CMD_GET_INK_STATE, 0xFF);
		
		}
	}*/
	return 0;

}

void CController4Dlg1::OnLButtonDown(UINT nFlags, CPoint point)
{
RECT rc;

	iButtons = FindPressedButton(&point);
	bButtonDown = TRUE;
	button_duration = 0;
	if(iButtons) {
		time(&time_at_click);
		GetClientRect(&rc);
		if(iButtons != JOB_BUTTON) {
			rc.right /= 2;
		}
		InvalidateRect(&rc, FALSE);
		Sleep(0);
	}
	
	if(theApp.GVars != NULL && !theApp.GVars->bDisableInkADC) {
		if(	theApp.GMasterStatus == MASTER_STATUS_IDLE &&
			point.x > InkMarkersOriginal[0].left &&
			point.x < InkMarkersOriginal[5].right &&
			point.y > InkMarkersOriginal[0].top &&
			point.y < InkMarkersOriginal[0].bottom) {
			/**/ TRACE("Getting ink levels (1)\n");
  			USBPrinterCommand(USB_CMD_GET_INK_STATE, 0xFF);
		
		}
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);


}


void CController4Dlg1::OnLButtonUp(UINT nFlags, CPoint point)
{
RECT rc;


	bButtonDown = FALSE;
	if(iButtons) {

		time(&time_at_release);
		
		button_duration = difftime(time_at_release, time_at_click);

		// see if user slid their finger off the button (as to not execute)
		if(iButtons == MAIN_COUNTER_BUTTON || iButtons == CURRENT_COUNTER_BUTTON || FindPressedButton(&point) == iButtons) {
			HandleButtonPress(iButtons);
		}

		GetClientRect(&rc);
	
		InvalidateRect(&rc, FALSE);

	}

	iButtons = 0;

	CDialogEx::OnLButtonUp(nFlags, point);


}

/*----------------------------------------------*/
/*-                                            -*/
/*----------------------------------------------*/
static void PressWhileNotReady() {
CPleaseWaitInit dlg;

	dlg.DoModal();



}

void CController4Dlg1::HandleButtonPress(int button) {

	theApp.QueueCS.Lock();

	theApp.QueueCS.Unlock();

	if(theApp.GVars == NULL)
		return;

	if(!theApp.bReceivedReadyFromFPGA && !theApp.NoUSB && theApp.GVars->bNoUIBeforeFPGAReady) {
		PressWhileNotReady();

		if(button != TOOLS_BUTTON)
			return;

	}

	GetWindowPlacement(&theApp.MasterWP);

	if(button == MAIN_COUNTER_BUTTON) {

		if(button_duration >= 2) {
			theApp.GVars->CurrentCount = 0;
			InvalidateRect(&ButtonArea[MAIN_COUNTER_BUTTON].slice[0]);
			theApp.WriteINI();
		}
	}

	if(button == CURRENT_COUNTER_BUTTON) {

		if(button_duration >= 2) {
			if(theApp.CurrentJob != NULL) {
				QUEUE_ITEM *qi = (QUEUE_ITEM *)theApp.CurrentJob;
				qi->iCount = 0;
				theApp.WriteQueue();
			}
			InvalidateRect(&ButtonArea[CURRENT_COUNTER_BUTTON].slice[0]);
		}
	}

	if(button == JOB_BUTTON) {

		CController4Dlg4 jbdlg;

		if(!theApp.Inited) return;

		jbdlg.DoModal();

	}

	if(button == TOOLS_BUTTON) {

		if(!theApp.Inited)  {
			return;
		}
		
		CController4Dlg3 tbdlg;

		currentDlg = (void *)&tbdlg;

		tbdlg.DoModal();

		currentDlg = NULL;

	}

	if(button == SETTINGS_BUTTON) {

		CController4Dlg2 sbdlg;

		if(!theApp.Inited) return;

		sbdlg.DoModal();

	}

	if(button == MAINT_BUTTON) {

		if(!theApp.Inited) return;

		if(	theApp.IsWaitingForJob()) {

			theApp.GVars->iJobWait = 0;

			int timeout = 0;

			theApp.StartWaitDialog(".. PLEASE WAIT ..");
			
			while(!theApp.RunWaitDialog) {
				Sleep(100);
			}

			Sleep(0);

			theApp.GHeadPosition = 0xFF;
			//USBPrinterCommand(USB_CMD_HOME_PLATEN, 0);
			USBPrinterCommandEx(USB_CMD_PARK, 0, 0, 1, 0, 0, 0, 0); /// <=== request to cap immediately

			while(theApp.GHeadPosition != 0) {

				timeout++;
				Sleep(1000);
				if(timeout > 12)
					break;

			}

			theApp.RunWaitDialog = FALSE;

			/**/ TRACE("Runwait false: %s %d\n", __FILE__, __LINE__);

		}


		CController4Dlg5 mbdlg;

		mbdlg.DoModal();

	}

	if(button == VANITY_SCREEN) {

		if(!theApp.Inited) return;

		CDevelopmentTeam dtdlg;

		dtdlg.DoModal();
	
	}

	if(button == CENTER_BUTTON) {

		if(!theApp.Inited) return;

		theApp.bPauseMode = !theApp.bPauseMode;
		USBPrinterCommand(USB_CMD_SET_CONTROLLER_MODE, theApp.bPauseMode);

		hMainBackground.DeleteObject();
		hMainBackgroundSettings.DeleteObject();
		hMainBackgroundMaint.DeleteObject();
		hMainBackgroundTools.DeleteObject();
		hMainBackgroundCenter.DeleteObject();

		LoadBackgroundBitmaps();

		RECT rc;

		GetClientRect(&rc);

		InvalidateRect(&rc, FALSE);

	}


}

void CController4Dlg1::OnDestroy()
{

	KillTimer(m_nTimer);
	KillTimer(m_nTimer2);

	CloseLogFile();
	
	hCPen.Detach();
	
	CDialogEx::OnDestroy();

	
}


void CController4Dlg1::AdjustCanvas() {

#define RULER_WIDTH_Y		50
#define SPACER_Y			10

#define RULER_WIDTH_X		50
#define SPACER_X			10


	WINDOWPLACEMENT CanvasWP;

	
	// Get Width and Height of Canvas
	m_Preview.GetWindowPlacement(&CanvasWP);
	m_CanvasPixelWidth = CanvasWP.rcNormalPosition.right - CanvasWP.rcNormalPosition.left;
	m_CanvasPixelHeight = CanvasWP.rcNormalPosition.bottom - CanvasWP.rcNormalPosition.top;

	m_FactorX = (float)m_ResX / (float)m_ResY;
	m_FactorY = (float)m_ResY / (float)m_ResX;

/*
	if(CurrentPlaten->y > CurrentPlaten->x) {
	
		CurrentPlaten->ScreenLoc.top = RULER_WIDTH_Y;
		CurrentPlaten->ScreenLoc.bottom = m_CanvasPixelHeight - SPACER_Y;

		CurrentPlaten->factorY = CurrentPlaten->WidthPoints / CurrentPlaten->HeightPoints;

		float w = (float)(CurrentPlaten->ScreenLoc.bottom - CurrentPlaten->ScreenLoc.top) * CurrentPlaten->factorY;

		CurrentPlaten->ScreenLoc.left = (m_CanvasPixelWidth - w) / 2;
		CurrentPlaten->ScreenLoc.right = CurrentPlaten->ScreenLoc.left + w;


	}

	else {
	
		CurrentPlaten->ScreenLoc.left = RULER_WIDTH_X;
		CurrentPlaten->ScreenLoc.right = m_CanvasPixelWidth - SPACER_X;

		CurrentPlaten->factorX = CurrentPlaten->HeightPoints / CurrentPlaten->WidthPoints;

		float h = (float)(CurrentPlaten->ScreenLoc.right - CurrentPlaten->ScreenLoc.left) * CurrentPlaten->factorX;

		CurrentPlaten->ScreenLoc.top = (m_CanvasPixelHeight - h) / 2;
		CurrentPlaten->ScreenLoc.bottom = CurrentPlaten->ScreenLoc.top + h;
	}

	CurrentPlaten->PixelWidth = CurrentPlaten->ScreenLoc.right - CurrentPlaten->ScreenLoc.left;
	CurrentPlaten->PixelHeight = CurrentPlaten->ScreenLoc.bottom - CurrentPlaten->ScreenLoc.top;

	CurrentPlaten->pixValX = CurrentPlaten->WidthInches / CurrentPlaten->PixelWidth;
	CurrentPlaten->pixValY = CurrentPlaten->HeightInches / CurrentPlaten->PixelHeight;

	CurrentPlaten->DppX = (float)CurrentPlaten->PixelWidth / CurrentPlaten->WidthInches;
	CurrentPlaten->DppY = (float)CurrentPlaten->PixelHeight / CurrentPlaten->HeightInches;

	m_ZeroX = CurrentPlaten->ScreenLoc.left;
	m_ZeroY = CurrentPlaten->ScreenLoc.top;

	DrawPlatenOutline();
*/


}


void MakeCompositePreview(QUEUE_ITEM *w_q, QUEUE_ITEM *c_q, BITMAPHANDLE *Bitmap, int content) {
// content 1 means Bitmap is loaded with white data
// content 0 means Bitmap is loaded with color data
BITMAPHANDLE auxBitmap, CompBitmap, *WhiteBitmap, *ColorBitmap;
HANDLE fh;
QUEUE_ITEM *qi;
int nRet;


	if(content == 1) {
		qi = c_q;
		WhiteBitmap = Bitmap;
		ColorBitmap = &auxBitmap;
	}
	else {
		qi = w_q;
		WhiteBitmap = &auxBitmap;
		ColorBitmap = Bitmap;
	}

	fh = CreateFile(qi->szFileName[0],
					GENERIC_READ,
					FILE_SHARE_READ,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
  					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);

	if(fh == INVALID_HANDLE_VALUE) {

		theDlg->OKBox("Can't Open", qi->szFileName[0], NULL);
		return;

	}

	nRet = L_CreateBitmap(	&CompBitmap,
							sizeof (BITMAPHANDLE),
							TYPE_CONV,
							Bitmap->Width ,
							Bitmap->Height ,
							24,
							ORDER_BGR,
							NULL,
							TOP_LEFT,
							NULL, 0);



	int size, i, ofst;
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
		theDlg->OKBox("Can't find", "preview", NULL);
		CloseHandle(fh);
		return;
	}

	tptr = smallbuff + ofst;

	sptr[0] = tptr[0];
	sptr[1] = tptr[1];
	sptr[2] = tptr[2];
	sptr[3] = tptr[3];
	

	if(!SetFilePointer(	fh,
					ofst,
					NULL,
					FILE_BEGIN) ) {
		theDlg->OKBox("Error reading", "preview", NULL);
		CloseHandle(fh);
		return;

	}

	unsigned char *pbuff = (unsigned char *)calloc(1, size + 16);

	ReadFile(fh, pbuff, size + 4, &bytes, NULL);


	CloseHandle(fh);

	nRet = L_LoadBitmapMemory(pbuff, &auxBitmap, sizeof(BITMAPHANDLE), 24, ORDER_BGR, size, NULL, NULL);
	
	if(nRet != SUCCESS) {
		nRet = L_LoadBitmapMemory(pbuff + 4, &auxBitmap, sizeof(BITMAPHANDLE), 24, ORDER_BGR, size, NULL, NULL);
	}

	free(pbuff);

	if(nRet != SUCCESS) {
		MessageBox(GetFocus(), "ERROR LOADING AUXILIARY BITMAP", "GENESIS CONTROLLER", MB_OK|MB_SYSTEMMODAL);
		free(pbuff);
	}


	int r, p;
	unsigned char *whitebuffer, *colorbuffer, *cmyk_whitebuffer, *cmyk_colorbuffer, *xbptr, *zbptr, *btptr;
	unsigned char BackGround[4] = {0,0,0,255}; // <--- put shirt color in here

	whitebuffer = (unsigned char *)calloc(1, WhiteBitmap->Width * 4);
	colorbuffer = (unsigned char *)calloc(1, ColorBitmap->Width * 4);
		
	cmyk_whitebuffer = (unsigned char *)calloc(1, WhiteBitmap->Width * 4);
	cmyk_colorbuffer = (unsigned char *)calloc(1, ColorBitmap->Width * 4);
		
	for(i = 0; i < ColorBitmap->Height; i++) {

		// get white raster and put it in the bitmap
		L_GetBitmapRow(WhiteBitmap, whitebuffer, i, WhiteBitmap->BytesPerLine);
		// get color raster and put it in the temp buffer;
		L_GetBitmapRow(ColorBitmap, colorbuffer, i, ColorBitmap->BytesPerLine);

		nRet = L_ConvertColorSpace(	whitebuffer, cmyk_whitebuffer, WhiteBitmap->Width, CCS_RGB, CCS_CMYK); 
		nRet = L_ConvertColorSpace(	colorbuffer, cmyk_colorbuffer, ColorBitmap->Width, CCS_RGB, CCS_CMYK); 


		float n_bkgndIntensity;
		int p, xx;

		xbptr = cmyk_whitebuffer;
		zbptr = cmyk_colorbuffer;
		btptr = whitebuffer;

		for(xx = 0; xx < ColorBitmap->Width; xx++) {
			
			n_bkgndIntensity = (float)xbptr[3] / (float)255;
				
			xbptr[0] = BackGround[0] * n_bkgndIntensity;
			xbptr[1] = BackGround[1] * n_bkgndIntensity;
			xbptr[2] = BackGround[2] * n_bkgndIntensity;
			xbptr[3] = BackGround[3] * n_bkgndIntensity;

			n_bkgndIntensity = (float)(255 - xbptr[3]) / (float)255;

			for(p = 0; p < 4; p++) {

				if(((float)xbptr[p] + ((float)zbptr[p] * n_bkgndIntensity)) > 255)
					xbptr[p] = 255;
				else
					xbptr[p] += (float)zbptr[p] * n_bkgndIntensity;

			}
				
			xbptr += 4;
			zbptr += 4;
		
		}
		
		nRet = L_ConvertColorSpace(	cmyk_whitebuffer, whitebuffer, ColorBitmap->Width, CCS_CMYK, CCS_RGB); 

		nRet = L_PutBitmapRow(&CompBitmap, whitebuffer, i, CompBitmap.BytesPerLine);


	}

	L_CopyBitmap(Bitmap, &CompBitmap, sizeof(BITMAPHANDLE));
	L_FreeBitmap(&CompBitmap);



}

void CController4Dlg1::PreparePreview() {
HANDLE fh;
QUEUE_ITEM *qi = (QUEUE_ITEM *)theApp.CurrentJob;
QUEUE_ITEM *aux_qi = NULL;

	if(qi == NULL) {
		LastCurrentJob = 0;
		return;
	}

	LastCurrentJob = qi->JobNumber;

	fh = CreateFile(qi->szFileName[0],
					GENERIC_READ,
					FILE_SHARE_READ,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
  					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);

	if(fh == INVALID_HANDLE_VALUE) {

		theDlg->OKBox("Can't Open", qi->szFileName[0], NULL);

		LastCurrentJob = 0;

		CController4Dlg4 dl;

		dl.DeleteQueueItem(qi, TRUE);

		theApp.CurrentJob = theApp.q_item_top;

		PreparePreview();

		//theApp.CurrentJob = NULL;

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
		theDlg->OKBox("Can't find", "preview", NULL);
		CloseHandle(fh);
		return;
	}

	tptr = smallbuff + ofst;

	sptr[0] = tptr[0];
	sptr[1] = tptr[1];
	sptr[2] = tptr[2];
	sptr[3] = tptr[3];
	

	if(!SetFilePointer(	fh,
					ofst,
					NULL,
					FILE_BEGIN) ) {
		theDlg->OKBox("Error reading", "preview", NULL);
		CloseHandle(fh);
		return;

	}

	unsigned char *pbuff = (unsigned char *)calloc(1, size + 16);

	ReadFile(fh, pbuff, size + 4, &bytes, NULL);


	CloseHandle(fh);

#ifdef USE_PREVIEW_DISPLAY_CS
	MainWindowCS.Lock(INFINITE);
#endif

	if(Bitmap.Flags.Allocated)
		L_FreeBitmap(&Bitmap);

	nRet = L_LoadBitmapMemory(pbuff, &Bitmap, sizeof(BITMAPHANDLE), 24, ORDER_BGR, size, NULL, NULL);
	
	if(nRet != SUCCESS) {
		nRet = L_LoadBitmapMemory(pbuff + 4, &Bitmap, sizeof(BITMAPHANDLE), 24, ORDER_BGR, size, NULL, NULL);
	}

	free(pbuff);

	if(qi->bIsWhite)
		aux_qi = FindOtherLayer(qi, FIND_COLOR);
	else
		aux_qi = FindOtherLayer(qi, FIND_WHITE);

	// see if it's a composite
	if(aux_qi != NULL) {
		int content;
		QUEUE_ITEM *c_q, *w_q;

		if(qi->bIsWhite) {
			w_q = qi;
			c_q = aux_qi;
			content = 1;
		}
		else {
			c_q = qi;
			w_q = aux_qi;
			content = 0;
		}

		MakeCompositePreview(w_q, c_q, &Bitmap, content);

	}

#ifdef USE_PREVIEW_DISPLAY_CS
	MainWindowCS.Unlock();
#endif
	
	GotPrevSize = FALSE;

	m_Preview.SetWindowPlacement(&pwp);

	PreparePage();

}


void CController4Dlg1::PreparePage() {

int ret;

	WINDOWPLACEMENT wp;

	if(!GotPrevSize) {
		m_Preview.GetWindowPlacement(&prv_wp);
		m_Preview.GetClientRect(&prv_rect);
		GotPrevSize = TRUE;
	}

	memcpy((char *)&wp, (char *)&prv_wp, sizeof(WINDOWPLACEMENT));
	memcpy((char *)&crect, (char *)&prv_rect, sizeof(RECT));

	crect.right = crect.left + (wp.rcNormalPosition.right - wp.rcNormalPosition.left);

	float f, s = 1.0;
	BOOL tooBig = FALSE;
	int o_l = prv_wp.rcNormalPosition.right - prv_wp.rcNormalPosition.left;
	int o_h = prv_wp.rcNormalPosition.bottom - prv_wp.rcNormalPosition.top;

shrink_it:
	
	tooBig = FALSE;

	if(Bitmap.Width > Bitmap.Height) {
		f = (float)Bitmap.Height / (float)Bitmap.Width;
		crect.bottom = crect.right * f * s;
	}
	else {
		f = (float)Bitmap.Width / (float)Bitmap.Height;
		crect.right = crect.bottom * f *s;
	}

	if((crect.right - crect.left) > o_l || (crect.bottom - crect.top) > o_h) {
		tooBig = TRUE;
	}

	if(tooBig) {
		s -= .1;
		goto shrink_it;
	}


	dcScreen = m_Preview.GetDC();

	SetRect(&inRect, 0, 0, Bitmap.Width, Bitmap.Height);

	wp.rcNormalPosition.right = wp.rcNormalPosition.left + (crect.right - crect.left);
	wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + (crect.bottom - crect.top);
	wp.rcNormalPosition.left -= 4;
	wp.rcNormalPosition.right += 4;
	wp.rcNormalPosition.top -= 4;
	wp.rcNormalPosition.bottom += 4;
	
	// now center the new size inside the preview area
	int n_l = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
	int df = o_l - n_l;
	df /= 2;
	if(df > 1) {
		wp.rcNormalPosition.left += df;
		wp.rcNormalPosition.right += df;
	}

	m_Preview.SetWindowPlacement(&wp);

	memcpy((char *)&clrect, &crect, sizeof(RECT));

	crect.left += 7;
	crect.top += 7;

}

void CController4Dlg1::DisplayPreview() {
int ret;

	
	if(theApp.CurrentJob != NULL) {

		dcScreen = m_Preview.GetDC();

		RECT crop_rect1;

		SetRect(&crop_rect1, clrect.left, clrect.top, clrect.right, clrect.bottom);

		crop_rect1.left += 4;
		crop_rect1.top += 4;

#ifdef USE_PREVIEW_DISPLAY_CS
		MainWindowCS.Lock(INFINITE);
#endif
		if(Bitmap.Flags.Allocated) {

			ret = L_PaintDC (	dcScreen->m_hDC,
								&Bitmap,
								&inRect,
								NULL,
								&crect,
								&crop_rect1,
								SRCCOPY);	/* Normal Paint */

		}

		DisplayJobInfo();

#ifdef USE_PREVIEW_DISPLAY_CS

		MainWindowCS.Unlock();

#endif

	}


}

void CController4Dlg1::DisplayJobInfo() {
char szJobName[128], szJobNumber[128], szSize[128], szPrintMode[128];


	if(!QueueItemStillValid((QUEUE_ITEM *)theApp.CurrentJob)) {
		theApp.CurrentJob = theApp.q_item_top;

	}

	/**/TRACE("CurJobCS lock 2\n");
	theApp.CurJobCS.Lock(INFINITE);

	QUEUE_ITEM *qi = (QUEUE_ITEM *)theApp.CurrentJob;

	if(theApp.CurrentJob != NULL) {
		CDC *hdc;
		hdc = GetDC();
		sprintf(szJobName, "%s", qi->JobName);
		sprintf(szJobNumber, "(%d)", qi->RipJobNumber);
		sprintf(szSize, "Area: %.02f x %.02f  /  Print Dir: %s", qi->printarea.x, qi->printarea.y, qi->iPrintDirection?"Bi-Direct.":"Uni-Direct.");

		if(qi->resolution.x == 600 && qi->resolution.y == 600)
			sprintf(szPrintMode, "Quality: Fast / Mode: %s / Density: %s", qi->iDualCMYK == 1?"CMYK X 2":"CMYK", qi->iPrintMode == 0?"Normal":"Double");
		else if(qi->resolution.x == 600 && qi->resolution.y == 1200)
			sprintf(szPrintMode, "Quality: Fine / Mode: %s / Density: %s", qi->iDualCMYK == 1?"CMYK X 2":"CMYK", qi->iPrintMode == 0?"Normal":"Double");
		else
			sprintf(szPrintMode, "Quality:  %d x %d / Mode: %s / Density: %s", qi->resolution.x, qi->resolution.y, qi->iDualCMYK == 1?"CMYK X 2":"CMYK", qi->iPrintMode == 0?"Normal":"Double");


		hdc->SelectObject(&theDlg->pMediumFont);
		hdc->SetTextColor(RGB(0, 0, 0));
		hdc->SetBkMode(TRANSPARENT);
		hdc->DrawText(szJobName, strlen(szJobName), &job_name_rect, DT_CENTER|DT_END_ELLIPSIS);
		hdc->DrawText(szSize, strlen(szSize), &size_rect, DT_CENTER|DT_END_ELLIPSIS);
		
		//print_mode_rect.left = prv_wp.rcNormalPosition.left;
		//print_mode_rect.right = prv_wp.rcNormalPosition.right;

		hdc->DrawText(szPrintMode, strlen(szPrintMode), &print_mode_rect, DT_CENTER|DT_END_ELLIPSIS);

		hdc->SelectObject(&theDlg->pNewFont);
		hdc->SetTextColor(RGB(0, 0, 0));
		hdc->SetBkMode(TRANSPARENT);
		hdc->DrawText(szJobNumber, strlen(szJobNumber), &job_number_rect, DT_RIGHT|DT_END_ELLIPSIS);

		

	}

	/**/TRACE("CurJobCS unlock 2\n");
	theApp.CurJobCS.Unlock();

}

void CController4Dlg1::ShowTemp(CPaintDC *dc) {
static int temps[2];
float farenheight;


	if(theApp.GVars == NULL) return;

	// if(theApp.GVars->CurHeadTemp[0] == 0 && theApp.GVars->CurHeadTemp[1] == 0) return;

	//if(temps[0] != theApp.GVars->CurHeadTemp[0] || temps[1] != theApp.GVars->CurHeadTemp[1]) {
	
		char temp_txt[128];

		dc->SelectObject(&theDlg->pSmallFont2);
		dc->SetTextColor(RGB(255, 255, 255));
		dc->SetBkMode(TRANSPARENT);

		farenheight = theApp.GVars->AbientTemp * 9;
		farenheight /= 5;
		farenheight += 32;

		SIZE sz;

		//sprintf(temp_txt, "Head Tmp: [A:%d%cF .. B:%d%cF]  Ambient: [%.02f%cF / %.02f%%]", theApp.GVars->CurHeadTemp[0], 0xB0, theApp.GVars->CurHeadTemp[1],  0xB0, farenheight /*theApp.GVars->AbientTemp*/, 0xB0, theApp.GVars->Humidity);
		
		if(theApp.GVars->AbientTemp != 0) {
			sprintf(temp_txt, "Ambient: %.02f%cF / %.02f%%", farenheight /*theApp.GVars->AbientTemp*/, 0xB0, theApp.GVars->Humidity);
			dc->DrawText(temp_txt, strlen(temp_txt), &temp_rect, DT_CENTER|DT_END_ELLIPSIS);
			GetTextExtentPoint32(dc->m_hDC, temp_txt, strlen(temp_txt), &sz);
		}


		temps[0] = theApp.GVars->CurHeadTemp[0];
		temps[1] = theApp.GVars->CurHeadTemp[1];
	//}
}


void CController4Dlg1::ShowVersion(CPaintDC *dc) {

	if(theApp.GVars == NULL) return;

	/*
	RECT rc;
	char temp_txt[32];
	GetClientRect(&rc);
	rc.top += 25;
	rc.left += 45;
	rc.right = rc.left + 300;
	rc.bottom = rc.top + 20;

	//dc->SelectObject(&theDlg->pSmallFont);

	*/

	RECT rc;
	char temp_txt[64];
	GetClientRect(&rc);
	rc.top += 250;
	rc.left += 15;
	rc.right = rc.left + 200;
	rc.bottom = rc.top + 300;

	dc->SelectObject(&theDlg->pSmallFontVert);


	//dc->SetTextColor(RGB(68, 68, 68));
	dc->SetTextColor(RGB(255, 255, 0));
	dc->SetBkMode(TRANSPARENT);
	if(theApp.GVars->bUseOneBitPP) {
		sprintf(temp_txt, "%s [1b]", PRODUCT_NAME);
	}
	else {
		sprintf(temp_txt, "%s", PRODUCT_NAME);
	}
	//dc->DrawText(temp_txt, strlen(temp_txt), &rc, DT_LEFT|DT_END_ELLIPSIS);
	dc->DrawText(temp_txt, strlen(temp_txt), &rc, DT_LEFT|DT_NOCLIP);

}

void CController4Dlg1::ShowCount(CPaintDC *dc) {

	if(theApp.GVars == NULL) return;

	char temp_txt[128];

	dc->SelectObject(&theDlg->pDigitalFont);
	//dc->SetTextColor(RGB(8, 155, 3));
	dc->SetTextColor(RGB(0, 0, 0));
	dc->SetBkMode(TRANSPARENT);
	sprintf(temp_txt, "%04d", theApp.GVars->CurrentCount);
	dc->DrawText(temp_txt, strlen(temp_txt), &counter_rect, DT_CENTER|DT_END_ELLIPSIS);


}

void CController4Dlg1::ShowCurrentCount(CPaintDC *dc) {

	if(theApp.GVars == NULL || theApp.currentWindow != MAIN_RUNNING_WINDOW) return;

	char temp_txt[128];

	dc->SelectObject(&theDlg->pDigitalFont);
	//dc->SetTextColor(RGB(8, 155, 3));
	dc->SetTextColor(RGB(0, 0, 0));

	dc->SetBkMode(TRANSPARENT);

	/**/TRACE("CurJobCS lock 3\n");
	theApp.CurJobCS.Lock();

	if(theApp.CurrentJob != NULL) {
		QUEUE_ITEM *qi = (QUEUE_ITEM *)theApp.CurrentJob;
		sprintf(temp_txt, "%04d", qi->iCount);
	}
	else {
		sprintf(temp_txt, "%04d", 0);
	}
	dc->DrawText(temp_txt, strlen(temp_txt), &current_counter_rect, DT_CENTER|DT_END_ELLIPSIS);

	/**/TRACE("CurJobCS unlock 3\n");
	theApp.CurJobCS.Unlock();


}

time_t CController4Dlg1::GetSystemSecs() {
unsigned long secs;

	theApp.current_seconds = time(0);
	
	return theApp.current_seconds;

}

BOOL CController4Dlg1::TimeToFillCap() {

	theApp.current_seconds = time(0);

	if((theApp.current_seconds - theApp.time_of_last_print) >= (theApp.GVars->iCapFillTimer * 60)) {
		
		return TRUE;

	}

	return FALSE;

}

void CController4Dlg1::UpdatePRIconNoBlink() {

	theApp.QueueCS.Lock();

	if(theApp.GMasterStatus == MASTER_STATUS_IDLE && theApp.CurrentJob != NULL && theApp.currentWindow == MAIN_RUNNING_WINDOW) {

		QUEUE_ITEM *qi = (QUEUE_ITEM *)theApp.CurrentJob;

		if(qi->iProdMode == PRODMODE_MULTIPLE) {
			m_PRIcon.ShowWindow(SW_SHOW);
		}
		else {
			m_PRIcon.ShowWindow(SW_HIDE);
		}
	}
	else {
		m_PRIcon.ShowWindow(SW_HIDE);
	}

	theApp.QueueCS.Unlock();

}


void CController4Dlg1::UpdatePRIcon() {
static BOOL PRIconVisible;

	return;

	theApp.QueueCS.Lock();

	if(theApp.GMasterStatus == MASTER_STATUS_IDLE && theApp.CurrentJob != NULL && theApp.currentWindow == MAIN_RUNNING_WINDOW) {

		QUEUE_ITEM *qi = (QUEUE_ITEM *)theApp.CurrentJob;

		if(qi->iProdMode == PRODMODE_MULTIPLE) {

			if(PRIconVisible) {
				// TRACE("PRIcon Off\n");
				m_PRIcon.ShowWindow(SW_HIDE);
				PRIconVisible = FALSE;
				//m_PRIcon.Invalidate(FALSE);
			}
			else {
				// TRACE("PRIcon On\n");
				m_PRIcon.ShowWindow(SW_SHOW);
				PRIconVisible = TRUE;
				//m_PRIcon.Invalidate(FALSE);
			}
		}
		else {
			// TRACE("PRIcon Off(2)\n");
			m_PRIcon.ShowWindow(SW_HIDE);
			PRIconVisible = FALSE;
			//m_PRIcon.Invalidate(FALSE);
		}
	}
	else {
		if(PRIconVisible) {
			// TRACE("PRIcon Off(3)\n");
			m_PRIcon.ShowWindow(SW_HIDE);
			PRIconVisible = FALSE;
			//m_PRIcon.Invalidate(FALSE);
		}
	}

	theApp.QueueCS.Unlock();

}

// #define DEBUG_FILL_TIMER
void CController4Dlg1::OnTimer(UINT_PTR nIDEvent) {
char temp_txt[32];
static int halted_counter, auto_print_countdown;


	if(theApp.GVars == NULL)
		return;

	if(nIDEvent == HALF_SECOND_TIMER) {

		if(iBlinkObstruction) {

			iBlinkObstruction--;

			if(iBlinkObstruction < 1)
				m_ObstructionSign.ShowWindow(SW_HIDE);

			else {
				bObstructionMarkerState = !bObstructionMarkerState;
				m_ObstructionSign.ShowWindow(bObstructionMarkerState?SW_SHOW:SW_HIDE);
			}

		}

		UpdatePRIcon();

		/// if printer is at the front, waiting fro a job
		/// don't wait longer than GVars->iJobWaitTimeout seconds
		if(	theApp.IsWaitingForJob()) {

			theApp.GVars->iJobWait += 500;

			if(theApp.GVars->iJobWait >= (theApp.GVars->iJobWaitTimeout * 1000)) {

				theApp.GVars->iJobWait = 0;

				/**/ TRACE("Timed out waiting for Job\n");
				USBPrinterCommandEx(USB_CMD_PARK, 0, 0, 1, 0, 0, 0, 0); /// <=== request to cap immediately
				USBPrinterCommand(USB_CMD_HOME_PLATEN, 0);

			}

		}

		if( theApp.QueueItems == 1 &&
			theApp.GVars->bPrintOnJobArrival &&
			theApp.bPendingAutoPrintRequest &&
			theApp.GPlatenPosition == 2 &&
			!theApp.bPauseMode &&
			theApp.currentWindow == MAIN_RUNNING_WINDOW) {

			theApp.bPendingAutoPrintRequest = FALSE;
			auto_print_countdown = 4;

		}

		else if(auto_print_countdown) {

			auto_print_countdown--;

			if(auto_print_countdown == 0) {

				theApp.bIsAutoPrintJob = TRUE;
				
				ExecutePrintRequest();

				Sleep(500);

				theApp.bPrinterPrepped = 1; /// cludge... 

			}

		}

	}

	/// 15 second interval timer
	if(nIDEvent == TEMP_DISPLAY_UPDATE2) {

		if(theApp.GMasterStatus == MASTER_STATUS_IDLE && theApp.currentWindow == MAIN_RUNNING_WINDOW && theApp.time_of_last_print != 0) {

#ifdef DEBUG_FILL_TIMER
			GetSystemSecs();
			if(theApp.current_seconds - theApp.time_of_last_print >= 120) {
#else
			if(TimeToFillCap()) {
#endif
				USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)3, 6, 0, 0, 0, 0, 0);
				theApp.time_of_last_print = 0;
		
			}

		}

		if(theApp.GMasterStatus == MASTER_STATUS_IDLE && theApp.HaltJobScanner && theApp.currentWindow == 0) {
			/// catch-all... if printer idle and job scanners halted and current window is main window, release the scanner
			halted_counter++;
			if(halted_counter >= 3) {
				halted_counter = 0;
				theApp.HaltJobScanner = FALSE;
			}
		}
		else
			halted_counter = 0;

		CPaintDC dc(this); // device context for painting
		ShowTemp(&dc);

	}

	CDialogEx::OnTimer(nIDEvent);
}


void CController4Dlg1::OKBox(char *line1, char *line2, char *line3) {
COKBox erdlg;
	

	erdlg.Line_1 = line1;
	erdlg.Line_2 = line2;
	erdlg.Line_3 = line3;

	erdlg.DoModal();


}


void CController4Dlg1::OnSetFocus(CWnd* pOldWnd)
{
QUEUE_ITEM *qi;
RECT frc;

	theApp.bHomePressed = FALSE;

	theApp.currentWindow = MAIN_RUNNING_WINDOW;

	TRACE("currentwindow: %d\n", theApp.currentWindow);

	if(theApp.Inited) {

		if(theApp.bJobWasCancelled) {
			theApp.ScannerDelay = 1;
			theApp.bJobWasCancelled = FALSE;
			/**/ TRACE("Restarting job scanner in Setfocus (cancelled job)\n");

			/**/ TRACE("HaltEvent 2393\n");

			theApp.HaltJobScanner = FALSE;

			/*
			if(theApp.hScannerThread->ResumeThread() == 0xFFFFFFFF) {
				MessageBox("** COULD NOT resume scanner thread", APP_NAME, MB_OK|MB_SYSTEMMODAL);
			}
			*/
		}

		qi = theApp.FindJobByNumber(theApp.GVars->CurrentJobNumber, 1);


		//TRACE("CurJobCS lock 4\n");
		//theApp.CurJobCS.Lock(INFINITE);

		theApp.CurrentJob = (void *)qi;
		if(qi == NULL) {
			LastCurrentJob = theApp.GVars->CurrentJobNumber = 0;
			RECT rc;
			GetClientRect(&rc);
			InvalidateRect(&rc);
		}
		
		if(qi != NULL) 
			theApp.GVars->CurrentJobNumber = qi->JobNumber;

		if(qi != NULL && LastCurrentJob != theApp.GVars->CurrentJobNumber) {

			//theApp.DisplayCS.Lock(0);

			m_Preview.GetClientRect(&frc);
			dcScreen->FillRect(&frc, &hBGbrushGray);
	
			PreparePreview();
			theApp.WriteINI();

			m_Preview.InvalidateRect(&frc, TRUE);

			//theApp.DisplayCS.Unlock();


		}

		UpdatePRIconNoBlink();

		if(theApp.CloseRequest) {

			Power();
		}

		m_Preview.GetClientRect(&frc);
		InvalidateRect(&frc);


		//theApp.CurJobCS.Unlock();


	}

	CDialogEx::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}

void CController4Dlg1::ForcePreviewRefresh(BOOL bForceAlways ) {

	/**/ TRACE("FORCE REDRAW. Last: %d, Current: %d\n", LastCurrentJob, theApp.GVars->CurrentJobNumber);

	/**/ StatusLine("FORCE REDRAW. Last: %d, Current: %d\r\n", LastCurrentJob, theApp.GVars->CurrentJobNumber);

	if(LastCurrentJob != theApp.GVars->CurrentJobNumber || bForceAlways) {

		//theApp.DisplayCS.Lock(0);

		//TRACE("CurJobCS lock 5\n");
		//	theApp.CurJobCS.Lock(INFINITE);

		theApp.CurrentJob = (void *)theApp.FindJobByNumber(theApp.GVars->CurrentJobNumber, 1);
		RECT frc;

		/**/ StatusLine("FORCE REDRAW. CurrentJob: 0x%X\r\n", theApp.CurrentJob);
		/**/ TRACE("FORCE REDRAW. CurrentJob: 0x%X\n", theApp.CurrentJob);

		//m_Preview.GetClientRect(&frc);
		//m_Preview.InvalidateRect(&frc, TRUE);

		GetClientRect(&frc);
		InvalidateRect(&frc, TRUE);

		PreparePreview();
		//theApp.DisplayCS.Unlock();

		//theApp.CurJobCS.Unlock( );


	}


}

void CController4Dlg1::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void CController4Dlg1::Power() {
int result;


	FPGAusb.quit_threads = TRUE;;
	AUXusb.quit_threads = TRUE;;
	JOBusb.quit_threads = TRUE;

	theApp.quit_threads = TRUE;

	Sleep(0);

	Sleep(1000);

	DWORD code;


	GetExitCodeThread(theApp.TempHandlerThread, &code);
	if(code == STILL_ACTIVE) {
		TerminateThread(theApp.TempHandlerThread->m_hThread, 0);
	}


	GetExitCodeThread(theApp.WeaverThread, &code);
	if(code == STILL_ACTIVE) {
		TerminateThread(theApp.WeaverThread->m_hThread, 0);
	}

	GetExitCodeThread(theApp.hJobIOThread, &code);
	if(code == STILL_ACTIVE) {
		TerminateThread(theApp.hJobIOThread->m_hThread, 0);
	}

	GetExitCodeThread(theApp.AppInitThread, &code);
	if(code == STILL_ACTIVE) {
		TerminateThread(theApp.AppInitThread->m_hThread, 0);
	}

	GetExitCodeThread(theApp.WorkerThread, &code);
	if(code == STILL_ACTIVE) {
		TerminateThread(theApp.WorkerThread->m_hThread, 0);
	}

	GetExitCodeThread(FPGAusb.hReaderThread, &code);
	if(code == STILL_ACTIVE) {
		TerminateThread(FPGAusb.hReaderThread->m_hThread, 0);
	}
	GetExitCodeThread(FPGAusb.hWriterThread, &code);
	if(code == STILL_ACTIVE) {
		TerminateThread(FPGAusb.hWriterThread->m_hThread, 0);
	}

	GetExitCodeThread(AUXusb.hReaderThread, &code);
	if(code == STILL_ACTIVE) {
		TerminateThread(FPGAusb.hReaderThread->m_hThread, 0);
	}
	GetExitCodeThread(AUXusb.hWriterThread, &code);
	if(code == STILL_ACTIVE) {
		TerminateThread(FPGAusb.hWriterThread->m_hThread, 0);
	}



	GetExitCodeThread(JOBusb.hWriterThread, &code);
	if(code == STILL_ACTIVE) {
		TerminateThread(JOBusb.hWriterThread->m_hThread, 0);
	}

	GetExitCodeThread(theApp.PrintingThread, &code);
	if(code == STILL_ACTIVE) {
		TerminateThread(theApp.PrintingThread->m_hThread, 0);
	}


#define SHUTDOWN_FORCE_OTHERS           0x00000001
#define SHUTDOWN_FORCE_SELF             0x00000002
#define SHUTDOWN_RESTART                0x00000004
#define SHUTDOWN_POWEROFF               0x00000008
#define SHUTDOWN_NOREBOOT               0x00000010
#define SHUTDOWN_GRACE_OVERRIDE         0x00000020
#define SHUTDOWN_INSTALL_UPDATES        0x00000040
#define SHUTDOWN_RESTARTAPPS            0x00000080
#define SHUTDOWN_SKIP_SVC_PRESHUTDOWN   0x00000100
#define SHUTDOWN_HYBRID                 0x00000200
#define SHUTDOWN_RESTART_BOOTOPTIONS    0x00000400


	STARTUPINFO			startup;
	PROCESS_INFORMATION	process;
	SECURITY_ATTRIBUTES	sa;
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	memset((char *)&startup, 0, sizeof(STARTUPINFO));
	startup.cb = sizeof (STARTUPINFO);
	BOOL success;

	success = CreateProcess (	"GENESIS_LAUNCHER.EXE",
								"GENESIS_LAUNCHER.EXE /terminate",
								NULL,
								NULL,
								FALSE,
								0, //CREATE_DEFAULT_ERROR_MODE|NORMAL_PRIORITY_CLASS,
								(LPVOID)NULL,
								(LPSTR)NULL,
								&startup,
								&process);


	OnOK();

}


void CController4Dlg1::RefreshTemp() {

	InvalidateRect(&temp_rect);

}

void CController4Dlg1::OnMouseMove(UINT nFlags, CPoint point)
{
int currentIButton = FindPressedButton(&point);

	if(bButtonDown && currentIButton != iButtons) {
		iButtons = currentIButton;
		GetClientRect(&rc);
		if(iButtons != JOB_BUTTON && iButtons != CURRENT_COUNTER_BUTTON && iButtons != MAIN_COUNTER_BUTTON) {
			rc.right /= 2;
		}
		InvalidateRect(&rc, FALSE);
	}
	/// see if ink markers were touched

	//TRACE("MouseMove: %d %d [%d]\n", point.x, point.y, theApp.InkPressureRequested); 

	CDialogEx::OnMouseMove(nFlags, point);
}

void SendSpeeds() {
int i;

	for(i = 0; i < 10; i++) {

		if(theApp.GVars->X_Speed[i] > 1000 || theApp.GVars->X_Accel[i] > 50) {
			USBPrinterCommandEx(USB_CMD_SET_X_SPEED, i, theApp.GVars->X_Speed[i], theApp.GVars->X_Accel[i], 0, 0, 0, 0);
		}

	}


}


void HandleFPGAInit(int p1, int p2) {
int i;

	if(p2 >= 900) {
		theApp.GVars->bUseOneBitPP = TRUE;
		theApp.GVars->bBitPPSet = TRUE;
	}
	else {
		theApp.GVars->bUseOneBitPP = FALSE;
		theApp.GVars->bBitPPSet = TRUE;
	}

	USBPrinterCommand(USB_CMD_SET_HEADTEMP, theApp.GVars->HeadTemp[0]);
	Sleep(200);
	USBPrinterCommand(USB_CMD_SET_HEADTEMP2, theApp.GVars->HeadTemp[1]);
	Sleep(200);
	USBPrinterCommand(USB_CMD_ENABLE_HEATERS, 1);
	Sleep(200);
	USBPrinterCommand(USB_CMD_SET_CAPDRY_JOBS, theApp.GVars->JobsBeforeDry);
	USBPrinterCommand(USB_CMD_SET_BUTTONMODE, theApp.GVars->bOneButtonOperation);

	SendHeaterTimeout();

	SendPlatenLevelingOffset();

	theApp.SendPlatenSelection();

	USBPrinterCommand(USB_CMD_IGNORE_PLATEN_HEIGHT, theApp.GVars->bIgnorePlatenHeight);

	USBPrinterCommand(USB_CMD_SETCAPTIMEOUT, theApp.GVars->iHeadCapTimeout);

	USBPrinterCommand(USB_CMD_SETAUTOSPITINTERVAL, theApp.GVars->iPeriodicSpitInterval);

	USBPrinterCommand(USB_CMD_SETAUTOSPITDURATION, theApp.GVars->iPeriodicSpitDuration);

	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iPlatenFeedHeight, SAVE_POS_AS_FEEDINGPOS, 0, 0, 0, 0, 0);

	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iDefaultPlatenHeight, SAVE_POS_AS_PRINTPOS, 0, 0, 0, 0, 0);

	if(theApp.GVars->iPlatenParkOffset > 1) {
		USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iPlatenParkOffset, SAVE_POS_AS_SPECIAL_2, 0, 0, 0, 0, 0);

	}

	/// foo foo USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iDefaultPlatenHeight, SAVE_POS_AS_PRINTPOS, 0, 0, 0, 0, 0);

	USBPrinterCommand(USB_CMD_IGNORE_OBSTRUCTION, theApp.GVars->bIgnoreObstruction);

	USBPrinterCommand(USB_CMD_ENABLE_INKBAG_TIMER, theApp.GVars->bInkBagAutoOffDisabled);

	if(theApp.GVars->bInkBagAutoOffDisabled) {
		
		if(theApp.GVars->bDualCMYK) {
			USBPrinterCommand(USB_CMD_OPEN_INK, 0x1E010000);
		}
		else {
			USBPrinterCommand(USB_CMD_OPEN_INK, 0x7E010000);
		}

	}

	USBPrinterCommand(USB_CMD_PRINTER_TEST_MODE, 0);

	Sleep(100);


	if(theApp.GVars->iPlatenParkOffset > 1) {
		USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 15002); /// special number, means go to parked position
	}
	else {
		USBPrinterCommand(USB_CMD_HOME_PLATEN_Z, 1);
	}

	if(!theApp.GVars->bDisableInkADC) {
		/**/ TRACE("Getting ink levels (2)\n");
		USBPrinterCommand(USB_CMD_GET_INK_STATE, 0xFF);
		USBPrinterCommand(USB_CMD_UPDATE_VALVES, 0x10);

	}

	//theApp.RunWaitDialog = FALSE;

	USBPrinterCommand(USB_CMD_IGNORE_UNDERRRUNS, theApp.GVars->bIgnoreUnderruns);

	SendSpeeds();


	USBPrinterCommand(USB_CMD_JOB_CLEANUP, 0);

	USBPrinterCommand(USB_CMD_BOUNCE_CMD, BOUNCE_SIGNAL_PREINITCOMPLETE);

	theApp.bReceivedReadyFromFPGA = TRUE;

	/*
	if(theApp.bPendingAutoLevelRequest) {
		theApp.bPendingAutoLevelRequest = FALSE;
	}
	*/


}



BOOL ExecutePrintRequest() {

	if(!theApp.Inited) return FALSE;

	if(theApp.GVars->bPendingCleanRequest) return FALSE;

	/**/ TRACE("ExecutePrintRequest m1\n");

retry_execute:

	if(theApp.CurrentJob != NULL) {

		/**/ TRACE("ExecutePrintRequest m2\n");

		if(QueueItemStillValid((QUEUE_ITEM *)theApp.CurrentJob)) {

			/**/ TRACE("ExecutePrintRequest m3\n");

			theApp.bCenterButton = TRUE;
			QUEUE_ITEM *aux_qi = NULL, *qi = (QUEUE_ITEM *)theApp.CurrentJob;

			if(!qi->bIsWhite) {
				aux_qi = FindOtherLayer(qi, FIND_WHITE);
			}

			if(aux_qi != NULL) {
	
				/**/ TRACE("ExecutePrintRequest m4\n");

				ExecuteReprint(aux_qi, FALSE);
			}
			else {
				
				/**/ TRACE("ExecutePrintRequest m5\n");
				
				ExecuteReprint(qi, FALSE);
			}
		}
		else {
				
		// MessageBox(GetFocus(), "Job pointer no longer valid", APP_NAME, MB_OK|MB_SYSTEMMODAL);

		/**/ TRACE("ExecutePrintRequest m6\n");

		theApp.CurrentJob = theApp.q_item_top;

		goto retry_execute;

		/**/TRACE("CurJobCS lock 6\n");
			//theApp.CurJobCS.Lock(INFINITE);
		
			theApp.CurrentJob = theApp.q_item_top;

			if(theApp.CurrentJob != NULL) {
				theApp.GVars->CurrentJobNumber = ((QUEUE_ITEM *)theApp.CurrentJob)->JobNumber;
				theDlg->InvalidateRect(&theDlg->ButtonArea[CURRENT_COUNTER_BUTTON].slice[0]);
			}

			RECT frc;
			theDlg->m_Preview.GetClientRect(&frc);
			theDlg->m_Preview.Invalidate(TRUE);
			//theApp.CurJobCS.Unlock( );
			

		}
	}

	/**/ TRACE("ExecutePrintRequest end\n");

	return TRUE;
}

void CController4Dlg1::OnBnClickedSymPrint()
{
	ExecutePrintRequest();
}


void CController4Dlg1::OnStnClickedScanMarker() {

}


void CController4Dlg1::OnBnClickedScannerDebug()
{
char msg[256];
int ret;

	unsigned long suspend_count = theApp.TempHandlerThread->ResumeThread();



	sprintf(msg, "GMStatus: %d\nHaltJS: %d\nCurWin: %d\nQF: %d\nRT Ret: %d\nPrs Cnt: %d\nWFP: %d\nScnDl: %d",
					theApp.GMasterStatus,
					theApp.HaltJobScanner,
					theApp.currentWindow,
					theApp.quit_threads,
					suspend_count,
					theApp.ParserCount,
					theApp.bWaitingForParser,
					theApp.ScannerDelay);

	ret = MessageBox(msg, "GenAPP", MB_CANCELTRYCONTINUE|MB_SYSTEMMODAL);
	
	if(ret == IDTRYAGAIN) {
		theApp.HaltJobScanner = 0;
		theApp.currentWindow = 0;
		theApp.ScannerDelay = 0;
		/**/ TRACE("Setting GMasterStatus to idle (2888)\n");
		theApp.GMasterStatus = 0;
	}

	//theApp.currentWindow = 0;
	//theApp.HaltJobScanner = 0;
}

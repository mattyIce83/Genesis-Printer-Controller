
// Controller4.h : main header file for the PROJECT_NAME application
//

#pragma once



// #define HEAP_DEBUG

#include "SingleInstance.h"
#include <time.h>

#define APP_NAME	"Belquette Genesis Controller"

//#define DEFAULT_PASSWORD	"062264"
#define DEFAULT_PASSWORD	"000000"

//  #define LOG_LOGGING

// #define USE_MOE_LOGGING

/**/ #define ENABLE_DELETE_ONLY_JOB
	
#define MINIMUM_BANDS_BEFORE_STOPBUTTON	1
	
	// #define DEBUG_ADDRASTER

// #define USE_BLACK_CHANNEL_ONLY

// #define  VERIFY_BAND_SEND

// #define DEBUG_START_OF_IMAGE

// #define COUNT_DOT_THREES

// #define USE_REPEATHANDLE

//#define HARD_WIRE_BIDIRECTIONAL

//#define HARD_WIRE_UNIDIRECTIONAL

// #define MAINT_POS_SAFETY   // <---- enables a mechanism to display an error if the head is not reported to tbe home and you
							 // select head-to-maintenance. In any case, the FPGA will not move it if it's not home


// #define VERBOSE_BAND_DEBUG
// #define NO_LEAD_OUT_RASTERS
// #define NO_LEAD_IN_RASTERS

// #define USE_ACK
// #define GET_TEMPS

// #define DISABLE_AMBIENT_TEMP

// #define ROUND_PIXEL_WIDTH_TO_WORD   // <--- increases the pixelwidth parameter of bands to be divisible by 4


/**/ #define AUTO_CLEAN_LEVEL_1

// #define AUTO_CLEAN_LEVEL_2


#define DEFAULT_LOGPORT	8500

#define USE_PREVIEW_DISPLAY_CS

// #define VERBOSE_WEAVE_LOCK

// #define VERBOSE_BAND_LOCK

// #define ENABLE_NETWORK_STUFF

//#define NO_RASTER_SHUFFLE

// #define DEBUG_BAND_DATA
// #define FILE_PER_BAND
// #define DEBUG_PARSER_DATA

/*
#define LOCK_CS	TRACE("Going to Lock CS at %s / %d\n", __FILE__, __LINE__); WeaveCS.Lock(INFINITE); TRACE("CS Locked at %s / %d\n", __FILE__, __LINE__);
#define UNLOCK_CS	TRACE("Going to Unlock CS at %s / %d\n", __FILE__, __LINE__); WeaveCS.Unlock(); TRACE("CS Unlocked at %s / %d\n", __FILE__, __LINE__);
*/

#ifdef VERBOSE_WEAVE_LOCK
	#define LOCK_CS		TRACE("WeaveLock %s / %d\n", __FILE__, __LINE__); WeaveCS.Lock(INFINITE);
	#define UNLOCK_CS	TRACE("Weaveunlock %s / %d\n", __FILE__, __LINE__); WeaveCS.Unlock();
#else
	#define LOCK_CS		WeaveCS.Lock(INFINITE);
	#define UNLOCK_CS	WeaveCS.Unlock();
#endif

/*
#define LOCK_BANDQUEUE		TRACE("Going to Lock BQ at %s / %d\n", __FILE__, __LINE__);  BandQueueCS.Lock(); TRACE("BQ Locked at %s / %d\n", __FILE__, __LINE__);
#define UNLOCK_BANDQUEUE	TRACE("Going to Unlock BQ at %s / %d\n", __FILE__, __LINE__); BandQueueCS.Unlock(); TRACE("BQ Unlocked at %s / %d\n", __FILE__, __LINE__);
*/

#ifdef VERBOSE_BAND_LOCK
	#define LOCK_BANDQUEUE		TRACE("BAND LOCK %s / %d\n", __FILE__, __LINE__); BandQueueCS.Lock(INFINITE);
	#define UNLOCK_BANDQUEUE	TRACE("BAND UNLOCK %s / %d\n", __FILE__, __LINE__); BandQueueCS.Unlock();
#else
	#define LOCK_BANDQUEUE		BandQueueCS.Lock(INFINITE);
	#define UNLOCK_BANDQUEUE	BandQueueCS.Unlock();
#endif

//#define	CENTERING_OFFSET	600    /// to move the image left or right by 1/1200 of an inch. Increase = away from cap, decrease = towards cap
/**/#define	CENTERING_OFFSET	900    /// to move the image left or right by 1/1200 of an inch. Increase = away from cap, decrease = towards cap
//#define	CENTERING_OFFSET	1800    /// to move the image left or right by 1/1200 of an inch. Increase = away from cap, decrease = towards cap

#define INK_ALARM_PERCENT		5.0

#define X_MARKER_X_OFFSET		20	
#define X_MARKER_Y_OFFSET		20

#define MAXIMUM_PRINTABLE_WIDTH		15.5

#define Q_ITEM_LOCKED			   0x02

#define USE_CHANNEL_2_FOR_BACKCHANNEL

#define MASTER_STATUS_IDLE			0
#define MASTER_STATUS_PRINTING		1
#define MASTER_STATUS_PAUSED		2
#define MASTER_STATUS_ERROR			4
#define MASTER_STATUS_BUSY			5
#define MASTER_STATUS_INITIALIZING	6
#define MASTER_STATUS_IMPORTING		7
#define MASTER_STATUS_CANCELLING	8   // Cancelling a job that was already printing
#define MASTER_STATUS_ABORTING		9	// Aborting a job that was not yet printing. Used to cancel out of an obstruction state

#define COUNT_MODE_PRINTED			0
#define COUNT_MODE_NOT_PRINTED		1


#define LAYER_TP_WHITE				0x01
#define LAYER_TP_SINGLE_COLOR		0x02
#define LAYER_TP_DUAL_COLOR			0x03
#define LAYER_TP_COLOR_WHL			0x04
#define LAYER_TP_DOUBLE_WHITE		0x05
#define LAYER_TP_DOUBLE_WHITE_ONLY	0x06
#define LAYER_TP_600X300_RR			0x08

#define MAX_PREVIEW_LENGTH		3600

#define MAIN_RUNNING_WINDOW		0
#define SETTINGS_WINDOW			2
#define TOOLS_WINDOW			3
#define JOBS_WINDOW				4
#define CLEANING_WINDOW			5


#define MAX_ITEMS_IN_QUEUE	1000

#define COMPLETION_HIT_MAX	15

#define COUNT_INK


#define	ADD_MODE_NEW		0
#define ADD_MODE_MODIFY		1
#define ADD_MODE_REPLACE	2


#define	SC_CYAN_BIT				0x01
#define	SC_MAGENTA_BIT			0x02
#define	SC_YELLOW_BIT			0x04
#define	SC_BLACK_BIT			0x08
#define	SC_WHITE_BIT			0x10

//#define WINDOW_WIDTH	490
//#define WINDOW_HEIGHT	800
//#define WINDOW_WIDTH	1350
#define WINDOW_WIDTH	1280
//#define WINDOW_HEIGHT	768
#define WINDOW_HEIGHT	800
#define TOTAL_SCREENS	3

#define DB_SHOWATPOS		0
#define DB_SHOWUPPERRIGHT	1
#define DB_SHOWUPPERLEFT	2

#define FIND_COLOR			0
#define FIND_WHITE			1
#define FIND_ANY			2

#define ACTION_HOME			0

#define STARTUP_DELAY		1000

#define RAPID_RETURN_TRAY_TIMEOUT	30

#define	PREVIEW_X_RESOLUTION	60
#define	PREVIEW_Y_RESOLUTION	60

#define PRODUCT_NAME	"Genesis Controller (Build 1628)"
#define BUILD_NUMBER	1628

#include "globals.h"

#include "resource.h"		// main symbols

#include "l_bitmap.h"

#include "Ltdlg.h"

///MOE 2016-5-17
#include "GENESIS_NWRK\GENESIS_NTWRK\IPFunctions.h"
#ifdef USE_MOE_LOGGING
	#include "NetworkDLLinterface.h"
#else
	#include "GENESIS_NWRK\GENESIS_NTWRK\log_supp.h"    /// remove
#endif

#include "CONTROLLER4_AUX\Importsupp.h"

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols



class CMyEdit: public CEdit {
	COLORREF m_cr;
	CBrush   m_br;
	DECLARE_MESSAGE_MAP()
	HBRUSH CtlColor ( CDC* pDC, UINT nCtlColor )
	{
		pDC->SetBkColor(m_cr);
		//pDC->SetTextColor(RGB(255,255,255));
		//pDC->SetTextColor(RGB(128,128,128));
		pDC->SetTextColor(RGB(0,128,200));
		return m_br;
	}
public:
	void SetBkColor(COLORREF cr)
	{
		m_cr = cr;
		m_br.CreateSolidBrush(cr);
		Invalidate();
	}
};

class CMyComboBox: public CComboBox {
	COLORREF m_cr;
	CBrush   m_br;
	DECLARE_MESSAGE_MAP()
	HBRUSH CtlColor ( CDC* pDC, UINT nCtlColor )
	{
		pDC->SetBkColor(m_cr);
		pDC->SetTextColor(RGB(255,255,255));
		return m_br;
	}
public:
	void SetBkColor(COLORREF cr)
	{
		m_cr = cr;
		m_br.CreateSolidBrush(cr);
		Invalidate();
	}
};


// CController4App:
// See Controller4.cpp for the implementation of this class
//
class CController4App : public CWinApp
{
public:
	CController4App();

    CSingleInstance m_singleInstance;

	CBitmap hBackground;
	CBitmap hSettingsBackground;
	CBitmap hNumberBackground;
	CBitmap hBackgroundJobs;
	CBitmap hBackgroundTools;
	CBitmap hBackgroundToolsDualCMYK;
	CBitmap hBackgroundPrinting;
	CBitmap hXButton;
	CBitmap hSWDLBackground;
	CBitmap hDevTeamBackground;
	
	HANDLE hRepeatDataHandle;

	void DisplayBitmap(CDC *, CBitmap *, int , int , int);

	void OverTempWarning(int);

	WINDOWPLACEMENT MainWP;
	WINDOWPLACEMENT HomeWP;
	WINDOWPLACEMENT HelpWP;
	WINDOWPLACEMENT LeftWP;
	WINDOWPLACEMENT RightWP;

	BITMAPHANDLE Prv_Bitmap[2];
	BITMAPHANDLE lock_icon;

	BOOL NoUSB;
	BOOL PrintingWindow;
	BOOL bJobWasCancelled;
	BOOL bPrinterPrepped;
	BOOL bYPrintMovementSet;
	BOOL bBlockTempThread;
	BOOL bCenterButton;
	BOOL bInDemoMode;
	BOOL bQuickReturn;
	BOOL bLeadOutComplete;
	BOOL SupColor[4];
	BOOL HaltJobScanner;
	BOOL bPauseMode;
	BOOL bObstruction;
	BOOL bReceivedReadyFromFPGA;
	BOOL ClosePrintingWindow;
	BOOL NoMoreCancel;
	BOOL bProlongPrintWindow;
	BOOL bQuitAutoSpitDialog;
	BOOL InkPressureRequested;
	BOOL bCancelComplete;
	BOOL bCancelRequested;
	BOOL bCreatingSwath;
	BOOL QueueRedrawRequest;
	BOOL bNozzleCheck;
	BOOL bSendingBand;
	BOOL bSendingAuxBand;
	BOOL bHardStop;


	BOOL bIsAutoPrintJob;

	dup_renumber *renumbers;

	int ScannerDelay;

	int LastLayerType;

	int GFirst_non_blank_raster;

	int GSentRasters;
	int GVerticalPos;
	int GBufferedFeed;
	int TotalPixelBands;
	int TotalPixelBytes;

	int iLeadIN;
	int iLeadOUT;

	int ActualJobRasters;

	int ParserCount;

	int iPrintedBandCount;


	BOOL bWaitingForParser;

	void *CurrentJob;

	BOOL CleaningDialogActive;

	BOOL CloseRequest;

	BOOL bTestBandMode;

	void StartWaitDialog(char *);
	
	BOOL InksOKToPrint();
	
	CImportSupp ImportSupp;

	int ConfigurableUserDlg(int, char *,    char *, char *, char *, char *,     char *, char *, char *, char *, int);


/// MOE 2016-5-17 remove most of these network DLL locals from here?
#ifndef USE_MOE_LOGGING
	BOOL bNewLogEntries;          // removed by MOE
	BOOL ConnectedToLogServer;    // removed by MOE
	int LogUploadInterval;        // removed by MOE
#endif

	CIPFunctions NtwrkObj;            /// MOE - deprieciated, for now local network parameters are still here

#ifdef USE_MOE_LOGGING
	CNetworkDLLinterface NetworkDLLinterface; /// MOE - new module
#else
	CLogger	LogSupp;  
#endif


#ifndef USE_MOE_LOGGING
	// network DLL related local functions - moved
	BOOL LogJobInfo(int, QUEUE_ITEM *, void *);
	BOOL UploadLog( );
	BOOL ConnectToLogServer(char *, int);
#endif
///MOE 2016-5-17

// Overrides
public:
	virtual BOOL InitInstance();

	int currentWindow;

	char LaunchDir[MAX_PATH];

	HWND QueueWND;

	WINDOWPLACEMENT MasterWP;

	BOOL bHomePressed;

	QUEUE_ITEM *q_item_top;

	QUEUE_ITEM *printing_qitem;

	CImageList m_QueueImages;

	time_t current_seconds;
	time_t time_of_last_print;

	GVARS *GVars;

	CCriticalSection		DisplayCS;
	CCriticalSection		QueueCS;
	CCriticalSection		USBCommandCS;
	CCriticalSection		OverflowCS;
	CCriticalSection		CurJobCS;
	CCriticalSection		SendBandCS;


	int QueueItems;

	BOOL Inited;
	BOOL quit_threads;
	BOOL bRunWPThreads;
	BOOL bHasPreview;
	BOOL bInJob;

	BOOL bPendingTempRequest;
	BOOL bPendingHumRequest;
	BOOL bPendingAutoLevelRequest;
	BOOL bPendingAutoPrintRequest;

	BOOL bDualHead;

	BOOL RunWaitDialog;

	int HTCounter;

	CWinThread *AppInitThread;
	CWinThread *WorkerThread;
	CWinThread *HeadFormatterThread;
	CWinThread *hJobIOThread;
	CWinThread *WeaverThread;
	CWinThread *PrintingThread;
	CWinThread *hAuxUSBSendThread;
	CWinThread *hScannerThread;
	CWinThread *TempHandlerThread;
	CWinThread *CleaningDialogThread;
	CWinThread *CDemoModeThread;
	CWinThread *AuxLayerPrinterThread;
	CWinThread *LogHandlerThread;
	CWinThread *WaitDialogThread;


	QUEUE_ITEM *qitem;
	QUEUE_ITEM *processing_qitem;

	SECURITY_ATTRIBUTES ThreadSA;

	void CreateNewQueueItem(char *);
	void WriteINI();
	void ReadINI();
	void ReadMotionVals();
	void CloseDown();
	void ReadQueue();
	void WriteQueue();
	void DeleteQueueItem(QUEUE_ITEM *);
	void DisplayQueueItem(QUEUE_ITEM *);
	int GetJobNumber(int);
	void CalculateInkFullVars();
	float CalculateCapPressure(int);
	void ProcessLevelInfo(unsigned long, unsigned long );
	BOOL CancelContinue(char *, unsigned long);
	BOOL IsWaitingForJob();
	void SendPlatenSelection();
	void SlowWorkerThreads(int);
	BOOL CheckAutoClean( );
	int LargestStepperOffset();
	
	BOOL CheckMaxStepperThrow(int *);
	
	BOOL GetEncryptedRegistryEntries(char *, int *, unsigned char *, unsigned char *);

	QUEUE_ITEM *FindJobByNumber(int, int);


	int ProtectedItems();

	void FindOrphans();
	BOOL FindICONInQueue(char *);
	BOOL FindPRNInQueue(char *);

	unsigned char *PreviewBuffer;
	unsigned char *PreviewBufferIndex;

	BITMAPHANDLE Bitmap[2];

	int GMasterStatus;

	int GPlatenPosition;

	BOOL bPrintingSecondLayer;

	int GHeadPosition;

	BOOL bUnlockTempThread;

	void DisplayError(char *, int);

	int CompletionHits;
	BOOL BlockView;


	int Y_Count;
	int Y_Total;
	float Y_Percent_Complete; // used to keep track of the botto of the real-time preview

	BOOL isQueueWindowOpen();

	HANDLE hAuxBandReadyEvent;
	BOOL AuxWriteStarted;

	unsigned char *AuxBandData;
	unsigned char *AuxBufferData;
	int	AuxBufferDataLength;

	// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CController4App theApp;
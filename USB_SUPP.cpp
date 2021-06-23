// *****************************************
// ImageToHeadDlg.cpp : implementation file
// *****************************************
// 

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg4.h"
#include "Controller4Dlg3.h"
#include "afxdialogex.h"
#include "ftd2xx.h"
#include <AFXMT.H>
#include "ftdisupp.h"
#include "parser.h"
#define STEPS_DEFINED
#include "HEAD_structs.h"
#include "Printing.h"
#include "CleaningInProgress.h"
#include "LevelingDialog.h"
#include "FatalError.h"
#include "UserPrompt1.h"
#include "AutoSpitting.h"
#include "CapDrainVerify.h"

extern CController4Dlg4 *theJobDlg;
extern BOOL QueueItemStillValid(QUEUE_ITEM *);
extern BOOL WriteEncodedPrintCount(int);


// #define ALLOW_INVALID_HEAD_TEMP  /// <== this will override the bad temp failsafe
#define MAX_BAD_TEMP_READINGS	3

// #define DISABLE_LOGGING



FTDIComm		FPGAusb;
FTDIComm		AUXusb;

FTDIComm		JOBusb;

extern CController4Dlg1 *theDlg;
extern CPrinting *pdlg;
extern void USBPrinterCommand(unsigned long , unsigned long);
extern BOOL ExecutePrintRequest();
extern void HandleFPGAInit(int, int);
extern BOOL DeleteOldestPrinted();


CleaningInProgress cl_dlg;

#define SEND_CTRL_TIMEOUT                    2000

//// Sensor flags
#define FRONT_HIGH_LIMIT					0x80
#define FRONT_HIGH_PRINT					0x40
#define FRONT_LOW_PRINT						0x20
#define REAR_HIGH_LIMIT						0x10
#define REAR_HIGH_PRINT						0x08
#define REAR_LOW_PRINT						0x04
#define PLATEN_EDGE							0x02
#define PLATEN_DETECT						0x01




///// MASTER COMMANDS
#define CONTROLLER_CMD_REPEAT_LAST				0x01
#define CONTROLLER_CMD_Y_ERROR					0x02
#define CONTROLLER_CMD_X_ERROR					0x03
#define CONTROLLER_CMD_CANCEL_PRINT				0x04
#define CONTROLLER_CMD_PAUSE_PRINT				0x05
#define CONTROLLER_CMD_RESUME_PRINT				0x06
#define CONTROLLER_CMD_FPGA_READY				0x07
#define CONTROLLER_CMD_FPGA_NOT_READY			0x08
#define CONTROLLER_CMD_TRAY_POS_REPORT			0x09
#define CONTROLLER_CMD_READY_FOR_PRINT			0x0A
#define CONTROLLER_CMD_SENSOR_ERROR				0x0B
#define CONTROLLER_CMD_CLEANING_ERROR			0x0C
#define CONTROLLER_CMD_NOZZLECHECK_ERROR		0x0D
#define CONTROLLER_CMD_CLEANING					0x0E
#define CONTROLLER_CMD_STATUS_REPORT			0x0F
#define CONTROLLER_CMD_VERSION_REPORT			0x10
#define CONTROLLER_CMD_HEAD_POS_REPORT			0x11	
#define CONTROLLER_CMD_PRINT_REQUEST_FROM_FPGA	0x12
#define CONTROLLER_CMD_MODE_REQUEST_FROM_FPGA	0x13
#define CONTROLLER_CMD_PLATEN_LEVEL_STATE       0x14
#define CONTROLLER_CMD_FPGA_INIT                0x15
#define CONTROLLER_CMD_FATAL_ERROR              0x16
#define CONTROLLER_CMD_USER_PROMPT              0x17
#define CONTROLLER_CMD_BOUNCE	                0x18
#define CONTROLLER_CMD_FPGA_RESTARTING          0x19
#define CONTROLLER_CMD_HEADS_WARMING            0x1A
#define CONTROLLER_CMD_DEBUG					0x1B

/// Y error types
#define  Y_ERROR_TYPE_OBSTRUCTION				0x01
#define  Y_ERROR_TYPE_NO_PLATEN					0x02
#define  Y_ERROR_TYPE_OBSTRUCTION_BEFORE		0x03
#define  Y_ERROR_TYPE_OBSTRUCTION_DURING		0x04
#define  Y_ERROR_TYPE_LASER_OUT					0x05
#define  Y_ERROR_TYPE_FATAL_FOLLOWING			0x06
#define  Y_ERROR_TYPE_ERROR_LEAVING_HOME		0x07
#define  Y_ERROR_TYPE_ERROR_FINDING_HOME		0x08
#define  Y_ERROR_TYPE_ERROR_REACHING_DEST		0x09
#define  Y_ERROR_TYPE_ERROR_MOVING_TO_STRT		0x0A
#define  Y_ERROR_TYPE_ERROR_MOVE_TIMEOUT		0x0B
#define  Y_ERROR_TYPE_ERROR_CLEARERROR			0x0C



/// X error types
#define  X_ERROR_TYPE_FATAL_FOLLOWING        0x01
#define  X_ERROR_TYPE_ERROR_LEAVING_HOME     0x02
#define  X_ERROR_TYPE_ERROR_FINDING_HOME     0x03
#define  X_ERROR_TYPE_ERROR_REACHING_DEST    0x04
#define  X_ERROR_TYPE_ERROR_POSITIONING_CAP  0x05
#define  X_ERROR_TYPE_ERROR_MOVE_TIMEOUT     0x06
#define  X_ERROR_TYPE_ERROR_CAP_NOT_DOWN     0x07
#define  X_ERROR_TYPE_ERROR_CAP_NOT_UP       0x08


/// sensor error types
#define  S_ERROR_TYPE_CANT_LEVEL				0x01

/// Other error types
#define  ERROR_X_NOT_HOME						0x01
#define  ERROR_Y_NOT_HOME						0x02
#define  CLEAN_CYCLE_BEGIN						0x04
#define  CLEAN_CYCLE_END						0x05
#define  ERROR_FLUID_NOT_DETECTED				0x06
#define  CRITICAL_TRANSPORT_ERROR				0x07
#define  ERROR_CLEANING_TIMEOUT					0x08
#define  ERROR_HEAD_PROTECT_SIGNAL_LOW			0x10


// Status Messages
#define TEMP_HEAD_1								0x01
#define TEMP_HEAD_2								0x02
#define VALVE_POSITIONS_1						0x03
#define VALVE_POSITIONS_2						0x04
#define VALVE_POSITIONS_3						0x05
#define VALVE_POSITIONS_4						0x06
#define TEMP_AMBIENT							0x07
#define HUMIDITY								0x08
#define INK_CTRL_STATUS							0x09
#define VAC_CHECK_RESULT						0x0A
#define AUTO_SPIT_STATE							0x0B
#define VALVE_POSITIONS_5						0x0C
#define INK_PRESSURE_SENSORS					0x0D

// Version Messages
#define MAIN_FPGA								0x01
#define AUX_FPGA								0x02
#define MUX_FPGA								0x03
#define HEAD_PIC								0x04

// INK CTRL Statuses
#define PIC_RESET_DETECTED						0x01
#define PIC_UNRESPONSIVE						0x02
#define PIC_RESPONSE_INVALID					0x03
#define PIC_TEMP_INVALID						0x04
#define PIC_TEMP_TIMEOUT						0x05
#define PIC_AMBTEMP_TIMEOUT						0x06
#define PIC_AMBHUM_TIMEOUT						0x07
#define PIC_AUX_TIMEOUT							0x08
#define PIC_AUX_BADRESP							0x09
#define PIC_AUX_READY_TIMEOUT					0x0A


/// Platen Level states
#define PLATEN_LEVEL_BEGIN                   0x01
#define PLATEN_LEVEL_END                     0x02
#define PLATEN_LEVEL_GOTHEIGHT				 0x03	
#define PLATEN_LEVEL_LEVELINFO				 0x04


/// User Prompts
#define REMOVE_INK_BAG                       0x01
#define REPLACE_INK_BAG                      0x02
#define INSERT_CLEANING_BAG                  0x03
#define REMOVE_CLEANING_BAG                  0x04





char *levelerrors[10] = {
/* 0 */		"Unknown error",
/* 1 */		"Initial home fail",
/* 2 */		"Initial position fail",
/* 3 */		"Can't find edge (1)",
/* 4 */		"Second position fail",
/* 5 */		"Can't find edge (2)",
/* 6 */		"Roll Timeout (2)",
/* 7 */		"Can't find edge (3)",
/* 8 */		"Re-home fail",
/* 9 */		"Roll timeout (1)",
};


typedef struct _controller_command {
	unsigned char initiator;
	unsigned char command;
	unsigned long p1;
	unsigned long p2;
	unsigned long p3;
	unsigned long p4;
	unsigned char terminator;
} controller_command;

#define USB_READ_PACKET_SIZE 19

controller_command ctrl_cmd;

BOOL		bDriverLoaded = FALSE;
HMODULE m_hmodule = NULL;

// #define DEBUG_READ

//#define TRACE_USB_RECEIVE

//#define DISABLE_OUTPUT  // <---- for testing without printer

/**/ #define BYPASS_USB_QUEUE // <---- always leave this defined for now.

PtrToWrite m_pWrite;
PtrToRead m_pRead;
PtrToOpen m_pOpen; 
PtrToOpenEx m_pOpenEx; 
PtrToListDevices m_pListDevices; 
PtrToClose m_pClose;
PtrToResetDevice m_pResetDevice;
PtrToPurge m_pPurge;
PtrToSetTimeouts m_pSetTimeouts;
PtrToGetQueueStatus m_pGetQueueStatus;
PtrToGetStatus m_pGetStatus;
PtrToSetEventNotification m_pSetEventNotification;

extern void FlipDWORD(unsigned long *);
extern void UpdateValveMarkers(unsigned short);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;


static FILE *logfile = NULL;
static time_t	current_time;
static int bad_temp_readings[2] = {0,0};

//static CAutoSpitting autospitting_dlg = NULL;

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void LogAction(char *szMessage) {

	if(logfile == NULL) return;

	current_time = time(0);   // get time now

    struct tm * now = localtime( &current_time );

	time(&current_time);

	fprintf(logfile, "@ [%02d/%02d/%04d] %02d:%02d:%02d: %s\n", 
		now->tm_mon + 1, now->tm_mday, now->tm_year + 1900,
		now->tm_hour, now->tm_min, now->tm_sec,
		szMessage);

	fflush(logfile);

}


/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void OpenLogFile() {

#ifdef DISABLE_LOGGING

	logfile = NULL;
	return;

#endif

	if(logfile != NULL) fclose(logfile);

	logfile = fopen("debug.log", "r");

	if(logfile == NULL) {
		logfile = fopen("debug.log", "w");
	}

	else {
		fclose(logfile);
		logfile = fopen("debug.log", "a");
	}

	if(logfile != NULL) {
		LogAction("**** ||||| ****** Log Open");
	}

}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void CloseLogFile() {

	if(logfile != NULL) {
		LogAction("Log Close **** ||||| ****** ");
		fclose(logfile);
	}

}
	
/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void ClearLogFile(BOOL reopen) {

	if(logfile != NULL) {
		CloseLogFile();
	}
	DeleteFile("debug.log");
	if(reopen) {
		OpenLogFile();
	}

}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void StartCleaningDialog(void *p) {
CleaningInProgress dlg;
	
	theApp.CleaningDialogActive = TRUE;

	dlg.bEnableCancelButton = (int)p;

	dlg.DoModal();

	Sleep(0);


}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void StartLevelingDialog(void *p) {
CLevelingDialog dlg;
	

	dlg.DoModal();

	Sleep(0);


}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void FatalErrorScreen() {
CFatalError dlg;

	dlg.DoModal();


}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void HandleUserPrompt(controller_command *ctrl_cmd) {
CUserPrompt1 dlg1;
int ret;

	switch(ctrl_cmd->p1) {

		case REMOVE_INK_BAG:
				strcpy(dlg1.szMessage, "PLEASE REMOVE WHITE\nINK BAG AND\nPRESS OK");
				ret = dlg1.DoModal();
				if(ret == 1) USBPrinterCommand(USB_CMD_USER_RESPONSE, 1);
				else  USBPrinterCommand(USB_CMD_USER_RESPONSE, 2); // return 2 for cancel
				break;

		case REPLACE_INK_BAG:
				strcpy(dlg1.szMessage, "PLEASE INSERT WHITE\nINK BAG AND\nPRESS OK");
				ret = dlg1.DoModal();
				if(ret == 1) USBPrinterCommand(USB_CMD_USER_RESPONSE, 1);
				else  USBPrinterCommand(USB_CMD_USER_RESPONSE, 2); // return 2 for cancel
				break;

		case INSERT_CLEANING_BAG:
				strcpy(dlg1.szMessage, "PLEASE INSERT\nCLEANING CARTRIDGE\nAND PRESS OK");
				ret = dlg1.DoModal();
				if(ret == 1) USBPrinterCommand(USB_CMD_USER_RESPONSE, 1);
				else  USBPrinterCommand(USB_CMD_USER_RESPONSE, 2); // return 2 for cancel
				break;

		case REMOVE_CLEANING_BAG:
				strcpy(dlg1.szMessage, "PLEASE REMOVE\nCLEANING CARTRIDGE\nAND PRESS OK");
				ret = dlg1.DoModal();
				if(ret == 1) USBPrinterCommand(USB_CMD_USER_RESPONSE, 1);
				else  USBPrinterCommand(USB_CMD_USER_RESPONSE, 2); // return 2 for cancel
				break;


	}



}


void SpittingMessageThread(void *p) {
CAutoSpitting autospitting_dlg;

	autospitting_dlg.DoModal();


}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void SpittingMessage(int state) {
static int current_state;

	if(!current_state) {

		theApp.bQuitAutoSpitDialog = FALSE;

		current_state = TRUE;

		AfxBeginThread((AFX_THREADPROC)SpittingMessageThread,
									0,
									THREAD_PRIORITY_NORMAL,
									0,
									0,
									&theApp.ThreadSA);
	}
	else {
		theApp.bQuitAutoSpitDialog = TRUE;
		current_state = FALSE;
	}

}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void FinishCancel() {

	/**/ TRACE("Finishing the cancel (setting idle)\n");

	theApp.ClosePrintingWindow = TRUE;

	theApp.RunWaitDialog = FALSE;
	
	theApp.ScannerDelay = 3;

	theApp.GMasterStatus = MASTER_STATUS_IDLE;
	

}


/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
int HandleMultiResponse(int p2, int p3, int p4) {
int ret;

	// end of day stuff
	if(p2 == 12) {

		if(p3 == 1) {

			// first section complete. repeat, continue or cancel		
			ret = theApp.ConfigurableUserDlg(0,
												"** END-OF-DAY (CHECK CAP) **",
									
												"CAP IS EMPTY\nCONTINUE",  // returns a 1
												"REPEAT DRAIN\nOPERATION", // returns a 2
												NULL,  // would return a 3
												"CANCEL", // returns a 4

												"HELP_DOCS\\CAP_EMPTY_VERIFY_1.jpg",
												"HELP_DOCS\\CAP_EMPTY_VERIFY_2.jpg",
												NULL,
												NULL,

												0);

			switch(ret) {

				case 4: // cancel
					USBPrinterCommandEx(USB_CMD_CLEAN, 4, 12, 0, 0, 0, 0, 0);
					break;

				case 2: // repeat
					USBPrinterCommandEx(USB_CMD_CLEAN, 1, 12, 0, 0, 0, 0, 0);
					break;

				case 1: // continue
					USBPrinterCommandEx(USB_CMD_CLEAN, 2, 12, 0, 0, 0, 0, 0);
					break;
			}

			return 1;
			
		}
	
		// first section complete. repeat, continue or cancel		
		if(p3 == 2) {
			// first section complete. repeat, continue or cancel		
			ret = theApp.ConfigurableUserDlg(0,
												"** END-OF-DAY (CHECK CAP) **",
									
												"CAP IS FULL\nCONTINUE",  // returns a 1
												"REPEAT FILL\nOPERATION", // returns a 2
												NULL,  // would return a 3
												"CANCEL", // returns a 4

												"HELP_DOCS\\CAP_FULL_VERIFY_1.jpg",
												"HELP_DOCS\\CAP_FULL_VERIFY_2.jpg",
												NULL,
												NULL,
												0);

			switch(ret) {

				case 1: // continue
				case 4: // cancel
					/// in either case... finish
					USBPrinterCommandEx(USB_CMD_CLEAN, 4, 12, 0, 0, 0, 0, 0);
					break;

				case 2: // repeat
					USBPrinterCommandEx(USB_CMD_CLEAN, 2, 12, 0, 0, 0, 0, 0);
					break;

			}

			return 1;

		}

		if(p3 == 4) {
			//// something here if you need to trap the end of the EOD
			////
		}


	}


}


/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void HandleUSBPacket(FTDIComm *u) {
unsigned char *cptr;
char szMessage[128];
char szMessage2[128];
unsigned long vp1, vp2, vp3;
float oldtemp, oldhum;
BOOL Bool;
INPUT ip;
KEYBDINPUT kb;
time_t last_pause_req = 0, now;
clock_t ticks;
int i;

	//memcpy((char *)&ctrl_cmd, (char *)u->read_buffer, sizeof(ctrl_cmd));
	ctrl_cmd.initiator = u->read_buffer[0];
	ctrl_cmd.command = u->read_buffer[1];

	cptr = (unsigned char *)&ctrl_cmd.p1;
	cptr[3] = u->read_buffer[2]; 
	cptr[2] = u->read_buffer[3]; 
	cptr[1] = u->read_buffer[4]; 
	cptr[0] = u->read_buffer[5]; 

	cptr = (unsigned char *)&ctrl_cmd.p2;
	cptr[3] = u->read_buffer[6]; 
	cptr[2] = u->read_buffer[7]; 
	cptr[1] = u->read_buffer[8]; 
	cptr[0] = u->read_buffer[9]; 
	
	cptr = (unsigned char *)&ctrl_cmd.p3;
	cptr[3] = u->read_buffer[10]; 
	cptr[2] = u->read_buffer[11]; 
	cptr[1] = u->read_buffer[12]; 
	cptr[0] = u->read_buffer[13]; 
		
	cptr = (unsigned char *)&ctrl_cmd.p4;
	cptr[3] = u->read_buffer[14]; 
	cptr[2] = u->read_buffer[15]; 
	cptr[1] = u->read_buffer[16]; 
	cptr[0] = u->read_buffer[17]; 

	ctrl_cmd.terminator = u->read_buffer[18];

	 u->read_buffer_length = 0;

	 // TRACE("Packet: %d %d %d %d %d %d %d\n", ctrl_cmd.initiator, ctrl_cmd.command, ctrl_cmd.p1, ctrl_cmd.p2, ctrl_cmd.p3, ctrl_cmd.p4, ctrl_cmd.terminator);

	switch(ctrl_cmd.command) {


		case CONTROLLER_CMD_FPGA_RESTARTING:
			if(theApp.GVars->bNoUIBeforeFPGAReady) {
				if(!theApp.RunWaitDialog) {
					theApp.StartWaitDialog("INITIALIZING...");
				}
			}
			break;

		case CONTROLLER_CMD_DEBUG:
			TRACE("||**|| [%u] ||**|| FPGA_DBG: %d %d %d\n", GetTickCount(), ctrl_cmd.p1, ctrl_cmd.p2, ctrl_cmd.p3, ctrl_cmd.p4);
			break;


		case CONTROLLER_CMD_HEADS_WARMING:
			if(ctrl_cmd.p1) {
				if(theApp.RunWaitDialog) {
					theApp.RunWaitDialog = FALSE;
					/**/ TRACE("Runwait false: %s %d\n", __FILE__, __LINE__);
					Sleep(1000);
				}
 				theApp.StartWaitDialog("Warming Up...");
			}
			else {
				/**/ TRACE("Runwait false: %s %d\n", __FILE__, __LINE__);
				theApp.RunWaitDialog = FALSE;
				Sleep(100);
			}
			break;


		case CONTROLLER_CMD_BOUNCE:

			if(ctrl_cmd.p4)
				theApp.GVars->bUnderFlow = TRUE;

			if(ctrl_cmd.p1 == BOUNCE_SIGNAL_CANCEL_COMPLETE) {
				theApp.bCancelComplete = 1;
				TRACE("Cancel Complete!");
				theApp.GMasterStatus = MASTER_STATUS_CANCELLING;
				USBPrinterCommand(USB_CMD_HOME_PLATEN,0);
				USBPrinterCommand(USB_CMD_JOB_CLEANUP, 0);
				/**/USBPrinterCommand(USB_CMD_PARK, 0);

				/**/ TRACE("Cancel Complete Bounceback\n");

				break;
			}

			if(ctrl_cmd.p1 == BOUNCE_SIGNAL_PREINITCOMPLETE) {
				/**/ TRACE("Runwait false: %s %d\n", __FILE__, __LINE__);
				theApp.RunWaitDialog = FALSE;
				break;
			}

			if(ctrl_cmd.p1 == BOUNCE_SIGNAL_NOMORECANCEL) {

				theApp.NoMoreCancel = TRUE;
				break;

			}

			if(ctrl_cmd.p1 == BOUNCE_SIGNAL_LAST_BAND) {
				
				/**/ TRACE("Last Band ping\n");
				//// Bump The master print counter and save it
				/*
				if(theApp.GVars->TotalJobs >= 0) {
					theApp.GVars->TotalJobs++;
					WriteEncodedPrintCount(theApp.GVars->TotalJobs);
				}
				*/

				theApp.ClosePrintingWindow = TRUE;

				if(!theApp.GVars->bDisableInkADC && !theApp.InkPressureRequested) {
					/**/ TRACE("Getting ink levels (5)\n");
					USBPrinterCommand(USB_CMD_GET_INK_STATE, 0xFF);
				}

				if(	theApp.GVars->bDualCMYK ||
					//theApp.LastLayerType == LAYER_TP_DOUBLE_COLOR ||
					theApp.LastLayerType == LAYER_TP_SINGLE_COLOR ||
					theApp.LastLayerType == LAYER_TP_DOUBLE_WHITE_ONLY ||
					theApp.LastLayerType == LAYER_TP_SINGLE_COLOR_SINGLE_WAVE) {
					BOOL bRefresh = FALSE;
#ifdef ENABLE_DELETE_ONLY_JOB
					if(theApp.GVars->MaxJobs == 1 && theApp.GVars->bDeleteAfterPrinting/* && theApp.printing_qitem->iProdMode != PRODMODE_MULTIPLE*/) {

						while(DeleteOldestPrinted()) {
							Sleep(500);
						}
	
						bRefresh = TRUE;

					}
#endif
					if(bRefresh) {

						Sleep(0);/// relinquish timeslice
						if(theApp.currentWindow == MAIN_RUNNING_WINDOW) {
							theDlg->ForcePreviewRefresh(TRUE);
						}

					}

					/**/ TRACE("HaltEvent 569\n");
					theApp.HaltJobScanner = FALSE;

#ifdef DEBUG_HALTSCANNER
					TRACE("File %s line %d HaltJobScanner = %d\n", __FILE__, __LINE__, theApp.HaltJobScanner);
#endif
					//if(theApp.GVars->bDualCMYK) {
						/**/ TRACE("Setting GMasterStatus to idle (582)\n");
						theApp.GMasterStatus = MASTER_STATUS_IDLE;
					//}


				}

			}

			break;

		case CONTROLLER_CMD_USER_PROMPT:
			HandleUserPrompt(&ctrl_cmd);
			break;

		case CONTROLLER_CMD_FATAL_ERROR:
			if(ctrl_cmd.p2 == 1) { // MEANS UNDERFLOW ERRO
				theDlg->OKBox("PIXEL DATA", "UNDERFLOW ERROR", "");
			}
			FatalErrorScreen();
			break;

		case CONTROLLER_CMD_PLATEN_LEVEL_STATE:

			if(ctrl_cmd.p1 == PLATEN_LEVEL_LEVELINFO) {

				TRACE("LevelInfo: %d %d\n", ctrl_cmd.p2, ctrl_cmd.p3);

				theApp.ProcessLevelInfo(ctrl_cmd.p2, ctrl_cmd.p3);

			}

			if(ctrl_cmd.p1 == PLATEN_LEVEL_GOTHEIGHT) {
				
				theApp.GVars->iLaserLoc = ctrl_cmd.p2;
				theApp.GVars->iDefaultPlatenHeight = theApp.GVars->iLaserLoc + theApp.GVars->iLaserToPrintHeightOffset;
				theApp.GVars->iPlatenFeedHeight = theApp.GVars->iLaserLoc + theApp.GVars->iLaserToFeedHeightOffset;
				
				if(theApp.currentWindow == TOOLS_WINDOW) {
				
					char tmp_buff[16];
					sprintf(tmp_buff, "%d", theApp.GVars->iDefaultPlatenHeight);
					if(theDlg->currentDlg != NULL) {
						((CController4Dlg3 *)theDlg->currentDlg)->m_DefaultHeight.SetWindowText(tmp_buff);
					}
				
				}

				theApp.WriteINI();

			}

			if(ctrl_cmd.p1 == PLATEN_LEVEL_END) {
				/**/TRACE("Auto Level completed...\n");
				theApp.bPendingAutoLevelRequest = FALSE;
				if(ctrl_cmd.p2 == 0) {
					if(ctrl_cmd.p3 < 0 || ctrl_cmd.p3 > 8) ctrl_cmd.p3 = 0;
					theDlg->OKBox("Platen auto-level", "** ERROR **", levelerrors[ctrl_cmd.p3]);
				}
				Sleep(100);
				/// STOP THE ANIMATION TOO
			}
			if(ctrl_cmd.p1 == PLATEN_LEVEL_BEGIN) {
				/**/TRACE("Auto Level started...\n");
				/// START THE ANIMATION
				theApp.CleaningDialogThread = AfxBeginThread((AFX_THREADPROC)StartLevelingDialog,
															0,
															THREAD_PRIORITY_NORMAL,
															0,
															0,
															&theApp.ThreadSA);
			}

			break;
		

		case CONTROLLER_CMD_FPGA_INIT:
			HandleFPGAInit(ctrl_cmd.p1, ctrl_cmd.p2);
			break;

		case CONTROLLER_CMD_PRINT_REQUEST_FROM_FPGA:

   			/**/ TRACE("** Print Request...\n", !theApp.bPauseMode);
			if((theApp.currentWindow == MAIN_RUNNING_WINDOW  || theApp.currentWindow == JOBS_WINDOW) && !theApp.bPauseMode && !theApp.ParserCount) {
	
				theApp.bCancelRequested = 0;

				if(theApp.currentWindow	== MAIN_RUNNING_WINDOW) {			
					/**/ TRACE("** Executing Print (ExecutePrintRequest())\n");
					ExecutePrintRequest();
				}
				else if(theApp.currentWindow == JOBS_WINDOW) {			
					/**/ TRACE("** Executing Print (ExecutePrintRequest2())\n");
					theJobDlg->OnBnClickedReprint();
				}
			}
			else {
				/**/ TRACE("** NOT Executing Print\n Curwin: %d, Pause: %d, ParsCount: %d", theApp.currentWindow, theApp.bPauseMode, theApp.ParserCount);
			}
			break;


		case CONTROLLER_CMD_MODE_REQUEST_FROM_FPGA:
			/**/ TRACE("** Mode Request\n");

			/// must have been at the back... cancel any auto-print requests
			theApp.bPendingAutoPrintRequest = FALSE;

			if(SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &Bool, 0)) {
				if(Bool) {
					/// Tell FPGA we were sleeping
					USBPrinterCommand(USB_CMD_PRINT_MODE_REPORT, 2);
					/// Now wake up!
					ip.type = INPUT_KEYBOARD;
					ip.ki.dwFlags = KEYEVENTF_SCANCODE;
					ip.ki.wScan = 'a';
					SendInput(1, &ip, sizeof(INPUT));
					break;
				}
			}

			if(!theApp.InksOKToPrint()) {
				theApp.bPauseMode = TRUE;
				USBPrinterCommand(USB_CMD_SET_CONTROLLER_MODE, theApp.bPauseMode);

				theDlg->hMainBackground.DeleteObject();
				theDlg->hMainBackgroundSettings.DeleteObject();
				theDlg->hMainBackgroundMaint.DeleteObject();
				theDlg->hMainBackgroundTools.DeleteObject();
				theDlg->hMainBackgroundCenter.DeleteObject();

				theDlg->LoadBackgroundBitmaps();

				RECT rc;

				theDlg->GetClientRect(&rc);

				theDlg->InvalidateRect(&rc, FALSE);

				theDlg->OKBox("UNABLE TO PRINT", "ONE OR MORE INK", "CHANNEL EMPTY!");

			}

			if( (theApp.currentWindow == MAIN_RUNNING_WINDOW  || theApp.currentWindow == JOBS_WINDOW) &&
				!theApp.bPauseMode &&
				!theApp.ParserCount &&
				((theApp.GVars->bPrintOnJobArrival && theApp.GVars->bDualCMYK) || theApp.QueueItems > 0) &&
				theApp.InksOKToPrint()) {

				if(theApp.bTestBandMode) {
					USBPrinterCommand(USB_CMD_PRINT_MODE_REPORT, 1);
					break;
				}


				if(theApp.currentWindow == JOBS_WINDOW) { // jobs queue
					if(theJobDlg->SaveSelections() != 1) {
						USBPrinterCommand(USB_CMD_PRINT_MODE_REPORT, 0);
					}
					else { /// main window here
				
						if(theApp.CurrentJob != NULL) {

							if(QueueItemStillValid((QUEUE_ITEM *)theApp.CurrentJob)) {
								USBPrinterCommand(USB_CMD_PRINT_MODE_REPORT, 1);
							}
							else {
								USBPrinterCommand(USB_CMD_PRINT_MODE_REPORT, 0);
							}
						}
						else {
							USBPrinterCommand(USB_CMD_PRINT_MODE_REPORT, 0);
						}
					}
				}
				else {
					USBPrinterCommand(USB_CMD_PRINT_MODE_REPORT, 1);
					/**/ TRACE("Mode Request. Responding(1): %d\n", !theApp.bPauseMode);
				}
			}
			else {
				USBPrinterCommand(USB_CMD_PRINT_MODE_REPORT, 0); // not so much
			/**/ TRACE("Mode Request. Rseponding(2): 0\n");
			}
			break;


		case CONTROLLER_CMD_CLEANING_ERROR:

			switch(ctrl_cmd.p1) {
				case ERROR_FLUID_NOT_DETECTED:
					theDlg->OKBox("** ERROR **" , "NO CLEANING", "FLUID DETECTED");
					break;
			}

			break;

		case CONTROLLER_CMD_CLEANING:

			switch(ctrl_cmd.p1) {

				case CLEAN_CYCLE_BEGIN:

					// wait a couple of seconds just in case)
					if(theApp.CleaningDialogActive) {
						for(i = 0; i < 60; i++) {
							Sleep(10);
							if(!theApp.CleaningDialogActive)
								break;
						}
					}

					if(!theApp.CleaningDialogActive) {
						theApp.CleaningDialogThread = AfxBeginThread((AFX_THREADPROC)StartCleaningDialog,
																	(void *)ctrl_cmd.p4,
																	THREAD_PRIORITY_NORMAL,
																	0,
																	0,
																	&theApp.ThreadSA);
					}
					break;

				case CLEAN_CYCLE_END:

					theApp.CleaningDialogActive = FALSE;

					/**/ TRACE("CLEAN END: %d %d %d\n", ctrl_cmd.p2, ctrl_cmd.p3, ctrl_cmd.p4);

					if(ctrl_cmd.p2 == 11) {
						
						//// move this to HandleMultiReponse() eventually
						if(ctrl_cmd.p3 == 0x01) {
							/// end of DryCapEX()
							//MessageBox(GetFocus(), "Otra comemierderia\nPress OK to continue", APP_NAME, MB_OK|MB_SYSTEMMODAL);

							CCapDrainVerify dlg;
							int res = dlg.DoModal();
							if(res == IDOK)
								USBPrinterCommandEx(USB_CMD_CLEAN, 2, 11, 0, 0, 0, 0, 0); // do the end of the routine
							else
								USBPrinterCommandEx(USB_CMD_CLEAN, 1, 11, 0, 0, 0, 0, 0); // do the end of the routine
						}
					}
					///
					if(ctrl_cmd.p2 == 12) { // <-- others too, later
						HandleMultiResponse(ctrl_cmd.p2, ctrl_cmd.p3, ctrl_cmd.p4);
					}
					break;


				case VAC_CHECK_RESULT:
					if(ctrl_cmd.p2 == 0 || ctrl_cmd.p2 == 1) {
						theApp.GVars->Valves.VacOK[ctrl_cmd.p2] = ctrl_cmd.p3;
						UpdateValveMarkers(1);
					}
					break;

			}

			break;

		case CONTROLLER_CMD_REPEAT_LAST:
				break;

		case CONTROLLER_CMD_SENSOR_ERROR:
		
			
			   switch(ctrl_cmd.p1) {

				    case ERROR_HEAD_PROTECT_SIGNAL_LOW:
						theDlg->OKBox("** HV ERROR **" , "Protect line active", "no printing can occur");
						break;

				    case S_ERROR_TYPE_CANT_LEVEL:
						sprintf(szMessage, "Status: 0x%02X. Error: %d", ctrl_cmd.p2, ctrl_cmd.p3);
						theDlg->OKBox("** ERROR AUTO" , "LEVELING PLATEN", szMessage);
						break;

					default:
	  					sprintf(szMessage, "Status: 0x%02X", ctrl_cmd.p2);
						theDlg->OKBox("** SENSOR **", "** ERROR **", szMessage);
						break;
				}

				break;


		case CONTROLLER_CMD_Y_ERROR:
				switch(ctrl_cmd.p1) {

					case Y_ERROR_TYPE_OBSTRUCTION:
						/**/ TRACE("Y obstruction: %d\n", ctrl_cmd.p2);
						theApp.bObstruction = TRUE; // ctrl_cmd.p2 != 0; TODO... SET TYPE HERE
						if(theApp.bNozzleCheck) {  // means obstructing during move to nozzle check
							theDlg->OKBox("** OBSTRUCTION **", "Nozzle check failed", "");
						}
						else if(theApp.bPauseMode && theApp.currentWindow == MAIN_RUNNING_WINDOW) {
							theDlg->m_ObstructionSign.ShowWindow(SW_SHOW);
							theDlg->iBlinkObstruction = 6;
							
						}
						break;

					case Y_ERROR_TYPE_ERROR_CLEARERROR:
						theApp.bObstruction = FALSE;
						break;


				}
				break;

		case CONTROLLER_CMD_X_ERROR:
				switch(ctrl_cmd.p1) {

					case X_ERROR_TYPE_FATAL_FOLLOWING:
						theDlg->OKBox("** ERROR **", "Fatal following error",NULL);
						break;

					case X_ERROR_TYPE_ERROR_LEAVING_HOME:
						theDlg->OKBox("** ERROR **", "Failed to leave", "home position");
						break;

					case X_ERROR_TYPE_ERROR_FINDING_HOME:
						theDlg->OKBox("** ERROR **", "Failed to find", "home position");
						break;

					case X_ERROR_TYPE_ERROR_REACHING_DEST:
						theDlg->OKBox("** ERROR **", "Failed to reach", "destination");
						break;

					case X_ERROR_TYPE_ERROR_POSITIONING_CAP:
						theDlg->OKBox("** ERROR **", "Cap Position", "Error");
						break;

					case  X_ERROR_TYPE_ERROR_MOVE_TIMEOUT:
						theDlg->OKBox("** ERROR **", "X Move ", "Timeout");
						break;

					case X_ERROR_TYPE_ERROR_CAP_NOT_UP:
						theDlg->OKBox("** ERROR **", "Failed to ", "Cap Head");
						break;

					case X_ERROR_TYPE_ERROR_CAP_NOT_DOWN:
						//theDlg->OKBox("** ERROR **", "Failed to ", "Uncap Head");
						theDlg->OKBox("** ERROR **", "Head Not", "Capped");
						break;

					default:
						sprintf(szMessage, "Unknown Error: %d", ctrl_cmd.p1);
						theDlg->OKBox("** ERROR **", "X axis error", szMessage);
						break;
				}
				break;


		case CONTROLLER_CMD_CANCEL_PRINT:
				break;

		case CONTROLLER_CMD_PAUSE_PRINT:

				/**/ TRACE("CONTROLLER_CMD_PAUSE_PRINT\n");

				// if at front waiting for job, treat it as an abort
				if(theApp.IsWaitingForJob()) {

					/**/ TRACE("Cancel waiting for Job\n");
					theApp.GVars->iJobWait = 0;
					USBPrinterCommand(USB_CMD_PARK,theApp.GVars->bEnableColorQuickReturn?theApp.bQuickReturn:0);
					USBPrinterCommand(USB_CMD_HOME_PLATEN, theApp.bQuickReturn);

					break;
				}


				 time(&now);
				 
				 if(difftime(now, last_pause_req) < 1) {
					 return;
				 }

				 last_pause_req = now;

				 // if in obstruction mode, try 
				 // printing again
				 if(theApp.bObstruction) {
					 // theApp.bObstruction = FALSE;
					 // USBPrinterCommand(USB_CMD_JOB_CLEANUP, 0);
	 				 USBPrinterCommand(USB_CMD_SYM_START_BUTTON, 2);
					 break;
				 }

				if(	theApp.PrintingWindow &&
					theApp.GMasterStatus != MASTER_STATUS_PAUSED) {

					if(pdlg != NULL) {

						if(pdlg->m_EStop.IsWindowVisible()) {
							pdlg->OnBnClickedEstop();
						}
					}

				}

				else if(theApp.PrintingWindow && theApp.GMasterStatus == MASTER_STATUS_PAUSED) {
					if(pdlg != NULL) {
						if(!pdlg->m_EStop.IsWindowVisible()) {
							/**/ TRACE(" CALLING OnBnClickedContinueprint [1]\n");
							pdlg->OnBnClickedContinueprint();
						}
					}
				}
				break;

		case CONTROLLER_CMD_FPGA_READY:
				break;
		case CONTROLLER_CMD_FPGA_NOT_READY:
				break;

		case CONTROLLER_CMD_HEAD_POS_REPORT:
				theApp.GHeadPosition = ctrl_cmd.p1;
				theDlg->StatusLine("Head Pos: %d\r\n", theApp.GHeadPosition);

				/**/ TRACE("Head Pos: %d\n", theApp.GHeadPosition);

				if(theApp.GHeadPosition == 0 && (theApp.bJobWasCancelled || theApp.GMasterStatus == MASTER_STATUS_ABORTING)) {
					FinishCancel();
				}


				break;


		case CONTROLLER_CMD_TRAY_POS_REPORT:
				theApp.GPlatenPosition = ctrl_cmd.p1;

				if(theApp.GPlatenPosition == 2 && theApp.GVars->bPrintOnJobArrival)
					theApp.GVars->iJobWait = 1;
				else
					theApp.GVars->iJobWait = 0;


				theDlg->StatusLine("PlatenPos: %d\r\n", theApp.GPlatenPosition);
				/**/ TRACE("Platen Pos: %d\n", theApp.GPlatenPosition);
				if(	theApp.GPlatenPosition == 2 &&
					theApp.PrintingWindow &&
					theApp.GMasterStatus != MASTER_STATUS_PAUSED &&
					theApp.GMasterStatus != MASTER_STATUS_CANCELLING &&
					theApp.GMasterStatus != MASTER_STATUS_ABORTING) {
					/**/ TRACE(" CALLING OnBnClickedContinueprint [2]\n");

					pdlg->OnBnClickedContinueprint();
				}
				if(theApp.GPlatenPosition == 1) {

					if(theApp.bNozzleCheck) {
						theApp.bNozzleCheck = FALSE;
						theApp.RunWaitDialog = FALSE;
						/**/ TRACE("Runwait false: %s %d\n", __FILE__, __LINE__);
					}

					if(theApp.bObstruction && theApp.PrintingWindow) {
						if(pdlg != NULL) {
							if(!pdlg->m_EStop.IsWindowVisible()) {
								pdlg->m_EStop.ShowWindow(SW_SHOW);
								/**/ TRACE("Show EButton: %s %d\n", __FILE__, __LINE__);
							}
						}
					}
					else if(theApp.GVars->bPendingCleanRequest) {

						theApp.GVars->bPendingCleanRequest = FALSE;
						USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)0x03, 1, 0, 0, 0, 0, 0);


					}
					if(theApp.bUnlockTempThread && theApp.GMasterStatus == MASTER_STATUS_IDLE) {
						theApp.bUnlockTempThread = FALSE;
						Sleep(2000);
						theApp.bBlockTempThread = FALSE;
					}

				}
				theApp.GVars->bTrayPosChange = TRUE;
				break;

		case CONTROLLER_CMD_READY_FOR_PRINT:
				/**/TRACE("PRINTER IS READY!! (Prev: %d)\n", theApp.bPrinterPrepped);
				theApp.bPrinterPrepped = 1;
				Sleep(100); /// 
				break;

		case CONTROLLER_CMD_VERSION_REPORT:
	
				switch(ctrl_cmd.p1) {
				    case MAIN_FPGA:
						LogAction("MAIN FPGA Version Reported from FPGA");
						theApp.GVars->Versions.Main = ctrl_cmd.p2 * 256 + ctrl_cmd.p3;

						sprintf(szMessage, "Main FPGA version report: %d", theApp.GVars->Versions.Main); 
						LogAction(szMessage);

						break;
					case AUX_FPGA:
						LogAction("AUX FPGA Version Reported from FPGA");
						theApp.GVars->Versions.Aux = ctrl_cmd.p2 * 256 + ctrl_cmd.p3;

						sprintf(szMessage, "AUX FPGA version report: %d", theApp.GVars->Versions.Aux); 
						LogAction(szMessage);

						break;
					case MUX_FPGA:
						LogAction("MUX FPGA Version Reported from FPGA");
						theApp.GVars->Versions.Mux = ctrl_cmd.p2 * 256 + ctrl_cmd.p3;
						sprintf(szMessage, "MUX FPGA version report: %d", theApp.GVars->Versions.Mux); 
						LogAction(szMessage);
						break;
					case HEAD_PIC:
						LogAction("PIC Version Reported from FPGA");
						theApp.GVars->Versions.PIC_Maj = ctrl_cmd.p2;
						theApp.GVars->Versions.PIC_Min = ctrl_cmd.p3;
						theApp.GVars->Versions.PIC_Rev = ctrl_cmd.p4;
						sprintf(szMessage, "PIC FPGA version report: %d.%d.%d", theApp.GVars->Versions.PIC_Maj, theApp.GVars->Versions.PIC_Min, theApp.GVars->Versions.PIC_Rev); 
						LogAction(szMessage);
						break;
				}			
				theApp.GVars->Versions.values_present = TRUE;
				break;

		case CONTROLLER_CMD_STATUS_REPORT:

				switch(ctrl_cmd.p1) {

					case INK_PRESSURE_SENSORS:

						if(!theApp.GVars->bDisableInkADC) {
							if(ctrl_cmd.p2 >= 0 && ctrl_cmd.p2 <= 5) {
								//BOOL foo;
								/**/ TRACE("Set ADC %d: %d\n", ctrl_cmd.p2, ctrl_cmd.p3);
								theApp.GVars->inkADC[ctrl_cmd.p2] = ctrl_cmd.p3;
								theDlg->RedrawInkMarkers(ctrl_cmd.p2);
								theApp.InkPressureRequested = FALSE;
								/*
								if(ctrl_cmd.p2 == 5) {
									foo = 0;
								}
								*/
							}
							else if(ctrl_cmd.p2 == 7) {
								/**/ TRACE("Cap ADC: %d\n", ctrl_cmd.p3);
								theApp.GVars->CapADC = ctrl_cmd.p3;
							}
							else {
								TRACE("ADC index out of range (%d)\n", ctrl_cmd.p2);
							}
						}
						break;


					case AUTO_SPIT_STATE:
						SpittingMessage(ctrl_cmd.p2);
						break;

				    case TEMP_HEAD_1:
						oldtemp = theApp.GVars->CurHeadTemp[0] ;
						theApp.GVars->CurHeadTemp[0] = ctrl_cmd.p2;

#ifndef ALLOW_INVALID_HEAD_TEMP
						if(theApp.GVars->bBadTempFailsafe) {
							if(theApp.GVars->bUseHeaters && (theApp.GVars->CurHeadTemp[0] > (theApp.GVars->HeadTemp[0] + 4) || theApp.GVars->CurHeadTemp[0] < 0)) {

								bad_temp_readings[0]++;

								if(bad_temp_readings[0] > MAX_BAD_TEMP_READINGS) {

									USBPrinterCommand(USB_CMD_SET_HEADTEMP, 60);
									USBPrinterCommand(USB_CMD_SET_HEADTEMP2, 60);

									USBPrinterCommand(USB_CMD_ENABLE_HEATERS, 0);

									theApp.GVars->bUseHeaters = 0;
									theApp.OverTempWarning(0);

								}
							}
							else {
								bad_temp_readings[0] = 0;
							}
						}
#endif
						if(oldtemp != theApp.GVars->CurHeadTemp[0] && theApp.currentWindow == MAIN_RUNNING_WINDOW) {
							theDlg->RefreshTemp();
						}
						sprintf(szMessage, "Head 1 temp report: %d", theApp.GVars->CurHeadTemp[0]); 
						LogAction(szMessage);

						break;

				    case TEMP_HEAD_2:
						oldtemp = theApp.GVars->CurHeadTemp[1];
						theApp.GVars->CurHeadTemp[1] = ctrl_cmd.p2;
#ifndef ALLOW_INVALID_HEAD_TEMP
						if(theApp.GVars->bBadTempFailsafe) {

							if(theApp.GVars->bUseHeaters && (theApp.GVars->CurHeadTemp[1] > (theApp.GVars->HeadTemp[1] + 4) || theApp.GVars->CurHeadTemp[1] < 0)) {

								bad_temp_readings[1]++;

								if(bad_temp_readings[1] > MAX_BAD_TEMP_READINGS) {
									USBPrinterCommand(USB_CMD_SET_HEADTEMP, 70);
									USBPrinterCommand(USB_CMD_SET_HEADTEMP2, 70);

									USBPrinterCommand(USB_CMD_ENABLE_HEATERS, 0);

									theApp.GVars->bUseHeaters = 0;
									theApp.OverTempWarning(1);

								}
							}
							else {
								bad_temp_readings[1] = 0;
							}
						}
#endif
						if(oldtemp != theApp.GVars->CurHeadTemp[1] && theApp.currentWindow == MAIN_RUNNING_WINDOW) {
							theDlg->RefreshTemp();
						}
						sprintf(szMessage, "Head 2 temp report: %d", theApp.GVars->CurHeadTemp[1]); 
						LogAction(szMessage);
						break;

					case TEMP_AMBIENT:
						oldtemp = theApp.GVars->AbientTemp;
						sprintf(szMessage, "%d.%d", ctrl_cmd.p2, ctrl_cmd.p3);
						theApp.GVars->AbientTemp = (float)atof(szMessage);
						if(oldtemp != theApp.GVars->AbientTemp && theApp.currentWindow == MAIN_RUNNING_WINDOW) {
							theDlg->RefreshTemp();
						}
						theApp.bPendingTempRequest = FALSE;

						sprintf(szMessage, "ambient temp report: %f", theApp.GVars->AbientTemp); 
						LogAction(szMessage);
						/**/ theDlg->InvalidateRect(&theDlg->temp_rect, FALSE);

						break;

					case HUMIDITY:
						oldhum = theApp.GVars->Humidity;
						sprintf(szMessage, "%d.%d", ctrl_cmd.p2, ctrl_cmd.p3);
						theApp.GVars->Humidity = (float)atof(szMessage);
						if(oldhum != theApp.GVars->Humidity && theApp.currentWindow == MAIN_RUNNING_WINDOW) {
							theDlg->RefreshTemp();
						}
						theApp.bPendingHumRequest = FALSE;

						sprintf(szMessage, "ambient humidity report: %f", theApp.GVars->Humidity); 
						LogAction(szMessage);
						/**/ theDlg->InvalidateRect(&theDlg->temp_rect, FALSE);

						break;


					case VALVE_POSITIONS_1:
						/**/ TRACE("VPOS 1 0x%08X\n", ctrl_cmd.p2);
						theApp.GVars->Valves.bF_InkValve = (ctrl_cmd.p2 & 0x00000001)?TRUE:FALSE;
						theApp.GVars->Valves.bC_InkValve = (ctrl_cmd.p2 & 0x00000002)?TRUE:FALSE;
						theApp.GVars->Valves.bM_InkValve = (ctrl_cmd.p2 & 0x00000004)?TRUE:FALSE;
						theApp.GVars->Valves.bY_InkValve = (ctrl_cmd.p2 & 0x00000008)?TRUE:FALSE;
						theApp.GVars->Valves.bK_InkValve = (ctrl_cmd.p2 & 0x00000010)?TRUE:FALSE;
						theApp.GVars->Valves.bW_InkValveL = (ctrl_cmd.p2 & 0x00000020)?TRUE:FALSE;
						theApp.GVars->Valves.bW_InkValveR = (ctrl_cmd.p2 & 0x00000040)?TRUE:FALSE;
						UpdateValveMarkers(1);
						break;

					case VALVE_POSITIONS_2:
						/**/ TRACE("VPOS 2 0x%08X\n", ctrl_cmd.p2);
						theApp.GVars->Valves.bVacSeal = (ctrl_cmd.p2 & 0x00000001)?TRUE:FALSE;
						theApp.GVars->Valves.bHeadASel= (ctrl_cmd.p2 & 0x00000002)?TRUE:FALSE;
						theApp.GVars->Valves.bHeadBSel =(ctrl_cmd.p2 & 0x00000004)?TRUE:FALSE;
						theApp.GVars->Valves.bHeadCSel =(ctrl_cmd.p2 & 0x00000008)?TRUE:FALSE;
						theApp.GVars->Valves.bHeadCapped = (ctrl_cmd.p2 & 0x00000010)?TRUE:FALSE;
						UpdateValveMarkers(2);
						break;

					case VALVE_POSITIONS_3:
						/**/ TRACE("VPOS 3 0x%08X\n", ctrl_cmd.p2);
						theApp.GVars->Valves.bFlushPump = (ctrl_cmd.p2 & I_FLUSH_PUMP)?TRUE:FALSE;
						theApp.GVars->Valves.bCPump = (ctrl_cmd.p2 & I_CYAN_PUMP)?TRUE:FALSE;
						theApp.GVars->Valves.bMPump = (ctrl_cmd.p2 & I_MAGENTA_PUMP)?TRUE:FALSE;
						theApp.GVars->Valves.bYPump = (ctrl_cmd.p2 & I_YELLOW_PUMP)?TRUE:FALSE;
						theApp.GVars->Valves.bKPump = (ctrl_cmd.p2 & I_BLACK_PUMP)?TRUE:FALSE;
						theApp.GVars->Valves.bWLPump = (ctrl_cmd.p2 & I_WHITE_LEFT_PUMP)?TRUE:FALSE;
						theApp.GVars->Valves.bWRPump = (ctrl_cmd.p2 & I_WHITE_RIGHT_PUMP)?TRUE:FALSE;
						UpdateValveMarkers(3);
						break;

					case VALVE_POSITIONS_4:
						
						/**/ TRACE("VPOS 4 0x%08X\n", ctrl_cmd.p2);
						
						//theApp.GVars->Valves.bDamperBladder_A = (ctrl_cmd.p2 & 0x00000001)?TRUE:FALSE;
						//theApp.GVars->Valves.bDamperBladder_B = (ctrl_cmd.p2 & 0x00000002)?TRUE:FALSE;

						theApp.GVars->Valves.bWhiteNormal  = (ctrl_cmd.p2 & 0x00000001)?TRUE:FALSE;
						theApp.GVars->Valves.bWhiteRecirc = (ctrl_cmd.p2 & 0x00000002)?TRUE:FALSE;
						theApp.GVars->Valves.bWhiteFlush = (ctrl_cmd.p2 & 0x00000020)?TRUE:FALSE;
						theApp.GVars->Valves.bFlushThrough = (ctrl_cmd.p2 & 0x00000040)?TRUE:FALSE;
						
						theApp.GVars->Valves.bVacPump = (ctrl_cmd.p2 & 0x00000004)?TRUE:FALSE;
						theApp.GVars->Valves.bWMixForward = (ctrl_cmd.p2 & I_WHITE_FORWARD_PUMP)?TRUE:FALSE;
						theApp.GVars->Valves.bWMixBackward = (ctrl_cmd.p2 & I_WHITE_BACKWARD_PUMP)?TRUE:FALSE;
						UpdateValveMarkers(4);
						break;

					case VALVE_POSITIONS_5:
						/**/ TRACE("VPOS 5 0x%08X\n", ctrl_cmd.p2);
						theApp.GVars->Valves.bFBagPresent = (ctrl_cmd.p2 & 0x00000001)?TRUE:FALSE;
						theApp.GVars->Valves.bCBagPresent = (ctrl_cmd.p2 & 0x00000002)?TRUE:FALSE;
						theApp.GVars->Valves.bMBagPresent = (ctrl_cmd.p2 & 0x00000004)?TRUE:FALSE;
						theApp.GVars->Valves.bYBagPresent = (ctrl_cmd.p2 & 0x00000008)?TRUE:FALSE;
						theApp.GVars->Valves.bKBagPresent = (ctrl_cmd.p2 & 0x00000010)?TRUE:FALSE;
						theApp.GVars->Valves.bWBagPresent = (ctrl_cmd.p2 & 0x00000020)?TRUE:FALSE;
						for(i = 0; i < 6; i++)
							theDlg->RedrawInkMarkers(i);
						UpdateValveMarkers(5);
						break;

					case INK_CTRL_STATUS:
						switch(ctrl_cmd.p2) {
							case PIC_RESET_DETECTED:
									theApp.GVars->PICResets++;

									sprintf(szMessage, "HEAD PIC Reset Detected (%d)", theApp.GVars->PICResets); 
									LogAction(szMessage);

									break;

							case PIC_UNRESPONSIVE:
									theApp.GVars->PICUnresponsive++;
									sprintf(szMessage, "HEAD PIC Unresponsive (%d)", theApp.GVars->PICUnresponsive); 
									LogAction(szMessage);
									break;

							case PIC_RESPONSE_INVALID:
									theApp.GVars->PICBadResponse++;
									sprintf(szMessage, "HEAD PIC Bad Response (%d)", theApp.GVars->PICBadResponse); 
									LogAction(szMessage);
									break;

							case PIC_TEMP_INVALID:
									theApp.GVars->PICBadTemp++;
									sprintf(szMessage, "HEAD PIC Temp Failsafe (%d)", theApp.GVars->PICBadTemp); 
									LogAction(szMessage);
									break;

							case PIC_TEMP_TIMEOUT:
									theApp.GVars->PICHeadTempTimeout++;
									sprintf(szMessage, "HEAD PIC Temp Timeout(%d)", theApp.GVars->PICHeadTempTimeout); 
									LogAction(szMessage);
									break;

							case PIC_AMBTEMP_TIMEOUT:
									theApp.GVars->PICAmbTempTimeout++;
									sprintf(szMessage, "HEAD PIC AmbTemp Timeout(%d)", theApp.GVars->PICAmbTempTimeout); 
									LogAction(szMessage);
									break;

							case PIC_AMBHUM_TIMEOUT:
									theApp.GVars->PICAmbHumTimeout++;
									sprintf(szMessage, "HEAD PIC AmbHum Timeout(%d)", theApp.GVars->PICAmbHumTimeout); 
									LogAction(szMessage);
									break;

							case PIC_AUX_TIMEOUT:
									theApp.GVars->AUXTimeout++;
									sprintf(szMessage, "AUX Timeout (%d)", theApp.GVars->AUXTimeout); 
									LogAction(szMessage);
									break;

							case PIC_AUX_BADRESP:
									theApp.GVars->AUXBadResp++;
									sprintf(szMessage, "AUX Bad Resp. (%d)", theApp.GVars->AUXBadResp); 
									LogAction(szMessage);
									break;

							case PIC_AUX_READY_TIMEOUT:
									theApp.GVars->AUXReadyTimeout++;
									sprintf(szMessage, "AUX Bad Resp. (%d)", theApp.GVars->AUXReadyTimeout); 
									LogAction(szMessage);
									break;

						}
						break;
				}
				break;
	


	}

}

/*----------------------------------------------------------------*/
/*-   Purge any pending writes                                   -*/
/*----------------------------------------------------------------*/
void FTDPurge(FTDIComm *u) {

	(*m_pPurge)(u->m_ftHandle, FT_PURGE_TX);


}
/*----------------------------------------------------------------*/
/*-   Threaded write with read queue support                     -*/
/*-   Write function. Puts write packet in the write queue       -*/
/*----------------------------------------------------------------*/
BOOL FTDWrite(FTDIComm *u, unsigned char *buffer, unsigned long length) {
FTDIComQueueItem *qitem, *tmpqi;

	unsigned long written;

#ifndef DISABLE_OUTPUT
	if(m_pWrite != NULL)
		(*m_pWrite)(u->m_ftHandle,buffer, length, &written);
#else
	Sleep(500);
#endif

	return TRUE;

}


/*----------------------------------------------------------------*/
/*- Threaded read with read queue support                        -*/
/*- Reader thread (puts packets in the read queue as they come in)-*/
/*----------------------------------------------------------------*/
void ReaderThread(void *v) {
 
	return;


}

/*----------------------------------------------------------------*/
/*- Threaded read with read queue support                        -*/
/*- Reader thread (puts packets in the read queue as they come in)-*/
/*----------------------------------------------------------------*/
void ReaderThread2(void *v) {
FTDIComm *u;
unsigned long bytesread = 0;
unsigned char tmp_read[USB_READ_CHUNKS];
FT_STATUS state;
BOOL bReadPending = FALSE;
FTDIComQueueItem *qitem, *tmpqi;
DWORD RxBytes, EventMask, ret, TxBytes, EventDWord;
HANDLE hEvent;
int totalbytes = 0;
BOOL bInPacket = FALSE;


// return;

	u = (FTDIComm *)v;

	hEvent = CreateEvent( NULL, false, false, "FTDIReadEvent");
	EventMask = FT_EVENT_RXCHAR;
	
	m_pSetEventNotification(u->m_ftHandle, EventMask, hEvent);

	while(!u->quit_threads) {

		ret = WaitForSingleObject(hEvent, 1000);

		if(ret == WAIT_TIMEOUT)
			continue;

		(*m_pGetStatus)(u->m_ftHandle, &RxBytes, &TxBytes, &EventDWord);
		
		if(RxBytes) {
			state = (*m_pRead)(u->m_ftHandle, tmp_read, RxBytes, &bytesread);
		}
		else {
			continue;
		}

		if(state == FT_OK && bytesread) {
			// something came in
			u->WriteCS.Lock();
			int i;

			/*
			for(i = 0; i < bytesread && u->read_buffer_length < USB_READ_BUFFER_LENGTH; i++) {
				u->read_buffer[u->read_buffer_length] = tmp_read[i];
				u->read_buffer_length++;
				if(u->read_buffer_length >= USB_READ_PACKET_SIZE) {
				
					HandleUSBPacket(u);
				
				}
			}
			*/

			for(i = 0; i < bytesread && u->read_buffer_length < USB_READ_BUFFER_LENGTH; i++) {

				if(!bInPacket && tmp_read[i] == 0xAA) {
					bInPacket = TRUE;
				}

				if(bInPacket) {
					u->read_buffer[u->read_buffer_length] = tmp_read[i];
					u->read_buffer_length++;
					if(u->read_buffer_length >= USB_READ_PACKET_SIZE) {
				
						HandleUSBPacket(u);
						bInPacket = FALSE;
				
					}
				}
			}

			u->WriteCS.Unlock();
		}

	}

	CloseHandle(hEvent);

}


/*----------------------------------------------------------------*/
/*- Delete all queued write packets                              -*/
/*----------------------------------------------------------------*/
void ClearUSBWriteQueue(FTDIComm *usb) {
FTDIComQueueItem *i1 = usb->WriteQueueTop, *i2;

	

	while(i1 != NULL) {
		HeapLock(usb->hHeap);
		i2 = i1->next;
		//free(i1->buffer);
		HeapFree(usb->hHeap, 0, i1->buffer);
		//free(i1);
		HeapFree(usb->hHeap, 0, i1);
		i1 = i2;
		HeapUnlock(usb->hHeap);
	}
	usb->write_queue_items = 0;
	usb->write_buffer_length = 0;
	if(usb->bActive) {
		
		//BOOL ret = PurgeComm((HANDLE)usb->m_ftHandle, PURGE_TXABORT);
		
		(*m_pPurge)(usb->m_ftHandle, FT_PURGE_TX);
	}


}

/*----------------------------------------------------------------*/
/*- Delete all queued read packets                               -*/
/*----------------------------------------------------------------*/
void ClearUSBReadQueue(FTDIComm *usb) {
FTDIComQueueItem *i1 = usb->ReadQueueTop, *i2;

	while(i1 != NULL) {
		HeapLock(usb->hHeap);
		i2 = i1->next;
		//free(i1->buffer);
		HeapFree(usb->hHeap, 0, i1->buffer);
		//free(i1);
		HeapFree(usb->hHeap, 0, i1);
		i1 = i2;
		HeapUnlock(usb->hHeap);
	}
	usb->read_queue_items = 0;
	usb->read_buffer_length = 0;
	if(usb->bActive)
		(*m_pPurge)(usb->m_ftHandle, FT_PURGE_RX);

}

/*----------------------------------------------------------------*/
/*- Delete all queued read packets                               -*/
/*----------------------------------------------------------------*/
void ResetUSB(FTDIComm *usb) {

	theDlg->StatusLine("Resetting USB read/write\n");
	ClearUSBWriteQueue(usb);
	ClearUSBReadQueue(usb);

}

/*----------------------------------------------------------------*/
/*- Initialize USB pipeline                                      -*/
/*----------------------------------------------------------------*/
BOOL InitUSB(FTDIComm *usb, char *szName, BOOL UseReaderThread) {
int retry  = 0;

	theDlg->StatusLine("Initializing USB subsystem\n");

	if(usb->Inited == USB_INITED_BYTE) {
		
		// if was already activated

		ClearUSBWriteQueue(usb); // delete all queued reads & writes
		ClearUSBReadQueue(usb);

		memset((char *)usb, 0, sizeof(FTDIComm));
		usb->Inited = USB_INITED_BYTE;

		// terminate the read and write threads
		usb->quit_threads = TRUE;
		Sleep(700);
#ifndef BYPASS_USB_QUEUE
		TerminateThread(usb->hWriterThread->m_hThread, 0);
#endif
		TerminateThread(usb->hReaderThread->m_hThread, 0);

		if(usb->hHeap != NULL && usb->hHeap != INVALID_HANDLE_VALUE) {
			HeapDestroy(usb->hHeap);
		}

	}

retry_usb_init:

	usb->status = (*m_pOpenEx)(szName, FT_OPEN_BY_DESCRIPTION, &usb->m_ftHandle);

	if(usb->status != FT_OK) {

		if(!retry) {
		     Sleep(5000);
			 retry++;
			 goto retry_usb_init;
		}

		theDlg->StatusLine("Unable to connect to %s\n", szName);
		theDlg->OKBox("** ERROR **", "Communicating with print", "hardware (1)");
		
		return FALSE;
	}
	else {

		theDlg->StatusLine("USB device %s loaded and connected\n", szName);
		theDlg->StatusLine("Starting threads\n");

		strcpy(usb->szName, szName);

		usb->ThreadSA.nLength = sizeof(SECURITY_ATTRIBUTES);
		usb->ThreadSA.bInheritHandle = TRUE;
		usb->ThreadSA.lpSecurityDescriptor  = NULL;
			
		usb->quit_threads = FALSE;

		usb->hHeap = HeapCreate(0,0,0);

		// Launch Threads

		if(UseReaderThread == 1) {
			usb->hReaderThread = AfxBeginThread((AFX_THREADPROC)	ReaderThread,
																(void *)usb,
																0,
																0,
																0,
																&usb->ThreadSA);
		}

		if(UseReaderThread == 2) {

			usb->hReaderThread = AfxBeginThread((AFX_THREADPROC)	ReaderThread2,
																(void *)usb,
																THREAD_PRIORITY_BELOW_NORMAL, //THREAD_PRIORITY_NORMAL,
																0,
																0,
																&usb->ThreadSA);
		}

		usb->bActive = TRUE;
		usb->Inited = USB_INITED_BYTE;

		theDlg->StatusLine("USB I/O threads running\n");
		theDlg->StatusLine("USB pipeline running\n");

		return TRUE;

	}

}

/*----------------------------------------------------------------*/
/*- Kill usb pipeline                                            -*/
/*----------------------------------------------------------------*/
void CloseUSB(FTDIComm *usb) {

	if(usb->Inited == USB_INITED_BYTE) {
		
		ResetUSB(usb);
		
		(*m_pClose)(usb->m_ftHandle);

		// terminate the read and write threads
		usb->quit_threads = TRUE;
		Sleep(1000);
		
		BOOL ts;

		if(usb->hReaderThread != NULL)
			ts = TerminateThread(usb->hReaderThread->m_hThread, 0);

		if(bDriverLoaded && m_hmodule != NULL) {
			FreeLibrary(m_hmodule);
			bDriverLoaded = FALSE;
			m_hmodule = NULL;
		}

	}

}


/*----------------------------------------------------------------*/
/*- Send command to the FPG over the USB pipeline                -*/
/*----------------------------------------------------------------*/
void USBPrinterCommand(unsigned long command, unsigned long parameter) {
CTH_BAND_INFO packet;
unsigned char nulls[16];

	theApp.USBCommandCS.Lock();

	memset((char *)&packet, 0, sizeof(CTH_BAND_INFO));

	packet.bytelength = USB_COMMAND;
	packet.start_x = command;
	packet.special_x = parameter;


	if(command != USB_CMD_REPORT_HEAD_TEMP)
		theDlg->StatusLine("CMD: 0x%04X | 0x%04X \n", command, parameter );

	// now flip all the dwords
	FlipDWORD(&packet.bytelength);
	FlipDWORD(&packet.start_x);
	FlipDWORD(&packet.length_x);
	FlipDWORD(&packet.special_x);
	FlipDWORD(&packet.y_step);
	FlipDWORD(&packet.direction);
	FlipDWORD(&packet.lastband);
	FlipDWORD(&packet.flags);

	CTH_BAND_INFO *rptkt = (CTH_BAND_INFO *)calloc(1, sizeof(CTH_BAND_INFO));

	memcpy((char *)rptkt, (char *)&packet, sizeof(CTH_BAND_INFO));

	memset(nulls, 0, 16);
	memset(rptkt->prefix, USB_PREFIX, 4);

	if(!theApp.NoUSB) {
		FTDWrite(&FPGAusb, (unsigned char *)rptkt, sizeof(CTH_BAND_INFO));
	}

	theApp.InkPressureRequested = TRUE;

	theApp.USBCommandCS.Unlock();

}

/*----------------------------------------------------------------*/
/*- Send command to the FPG over the USB pipeline                -*/
/*----------------------------------------------------------------*/
void USBPrinterCommandEx(unsigned long command, unsigned long parameter1,
												unsigned long parameter2,
												unsigned long parameter3,
												unsigned long parameter4,
												unsigned long parameter5,
												unsigned long parameter6,
												unsigned long parameter7) {
CTH_BAND_INFO packet;
unsigned char nulls[16];

	theApp.USBCommandCS.Lock();

	memset((char *)&packet, 0, sizeof(CTH_BAND_INFO));

	packet.bytelength = USB_COMMAND;
	packet.start_x = command;

	packet.length_x = parameter1;
	packet.special_x = parameter2;
	packet.y_step = parameter3;
	packet.direction = parameter4;
	packet.lastband = parameter5;
	packet.flags = parameter6;

	theDlg->StatusLine("CMD: %04X | \n", command );

	// now flip all the dwords
	FlipDWORD(&packet.bytelength);
	FlipDWORD(&packet.start_x);
	FlipDWORD(&packet.length_x);
	FlipDWORD(&packet.special_x);
	FlipDWORD(&packet.y_step);
	FlipDWORD(&packet.direction);
	FlipDWORD(&packet.lastband);
	FlipDWORD(&packet.flags);

	CTH_BAND_INFO *rptkt = (CTH_BAND_INFO *)calloc(1, sizeof(CTH_BAND_INFO));

	memcpy((char *)rptkt, (char *)&packet, sizeof(CTH_BAND_INFO));

	memset(nulls, 0, 16);
	memset(rptkt->prefix, USB_PREFIX, 4);

	if(!theApp.NoUSB) {
		FTDWrite(&FPGAusb, (unsigned char *)rptkt, sizeof(CTH_BAND_INFO));
	}

	theApp.USBCommandCS.Unlock();

}


/*----------------------------------------------------------------*/
/*- Test USB channel integrity                                 -*/
/*----------------------------------------------------------------*/
void CheckUSBIntegrity() {
unsigned char buff[1000];
int i;

	for(i = 0; i < 256; i++) {
		buff[i] = (unsigned char)i;
	}
	
	if(!theApp.NoUSB) {
		FTDWrite(&FPGAusb, buff, 256);
	}



}

/*----------------------------------------------------------------*/
/*- Send pixel data down the auxiliary channel                   -*/
/*----------------------------------------------------------------*/
void AuxUSBSendThread(void *v) {
int ret;
FTDIComm *u;

	u = (FTDIComm *)v;

	theApp.hAuxBandReadyEvent = CreateEvent( NULL, false, false, "AuxBandReadyEvent");

	while(!u->quit_threads) {

		ret = WaitForSingleObject(theApp.hAuxBandReadyEvent, 700);

		if(ret == WAIT_TIMEOUT)
			continue;


		else {
			//FTDWrite(u, theApp.AuxBandData, sizeof(CTH_BAND_INFO));
			// TRACE("Send AuxBand\n");
			theApp.bSendingAuxBand = TRUE;
			FTDWrite(u, theApp.AuxBufferData, theApp.AuxBufferDataLength);
			theApp.bSendingAuxBand = FALSE;
			// TRACE("AuxBand sent\n");
		}


	}

	CloseHandle(theApp.hAuxBandReadyEvent);

}

/*----------------------------------------------------------------*/
/*- Send Heater timeout                                           */
/*----------------------------------------------------------------*/
void SendHeaterTimeout() {
	
	int timeout = theApp.GVars->iHeaterTimeout * 60 * 60 * 1000;


	USBPrinterCommand(USB_CMD_SET_HEADTEMPTIMEOUT, timeout);

}

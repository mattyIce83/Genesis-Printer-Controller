//  formatter.cpp
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg4.h"
#include "LayerWait.h"
#include "PleaseWait.h"
#include "resource.h"
#include "ftd2xx.h"
#include "ftdisupp.h"
#include "Printing.h"

#include "parser.h"
#define STEPS_DEFINED
#include "HEAD_structs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**/ #define SIMULATE_PRINTING_DELAY


extern CPrinting *pdlg;
extern CController4Dlg1 *theDlg;
extern CController4Dlg4 *theJobDlg;
extern BOOL CreateHeadSwath(PRINT_HEAD *, INBAND *, BOOL);
extern void InitHead(PRINT_HEAD *);
extern CTH_BAND_INFO *PrepareBandPacket(PRINT_HEAD *, BOOL, displayGraphicState *, BOOL, int *);
extern BOOL FTDWrite(FTDIComm *, unsigned char *, unsigned long);
extern FTDIComm		FPGAusb;
extern FTDIComm		AUXusb;
extern displayGraphicState GS;
extern weave_pattern weave[TOTAL_MICROWEAVE_PATTERNS];
extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;
extern void SelectMW(displayGraphicState *);
extern void FlipDWORD(unsigned long *);
extern BOOL GetHeadTemps();
extern void FTDPurge(FTDIComm *);
extern QUEUE_ITEM *FindOtherLayer(QUEUE_ITEM *, int);
extern int GbandNo;
extern void PrintingDialogLoop(void *);
extern void ParserLoop(void *);
extern void ExecuteReprint(QUEUE_ITEM *, BOOL);
extern BOOL QueueItemStillValid(QUEUE_ITEM *);
extern BOOL OKToAddJob();
extern BOOL DeleteOldestPrinted();
extern BOOL WriteEncodedPrintCount(int);

#ifdef COUNT_DOT_THREES
	extern int 	ThreeCount[4];
	extern int 	TwoCount[4];
	extern int 	OneCount[4];
#endif
extern BOOL max_Y_received[TOTAL_CHANNELS];

extern int BiDirectional_Correction;

extern unsigned char *flip_band; // <-- reusable <---
extern int cur_flip_band_size;

#ifdef COUNT_DOT_THREES
	extern void CountDotThrees(PRINT_HEAD *);
#endif


BOOL GBand_Dir;

int GBandNumber;

int GLastBand;


// #define NUMBER_OF_INPUT_BANDS			4      /////////
/**/ #define NUMBER_OF_INPUT_BANDS			8      ////////// this one
// #define NUMBER_OF_INPUT_BANDS			16      ////////// this one
/**/ #define NUMBER_OF_WORKING_HEADS		4
// #define NUMBER_OF_WORKING_HEADS		8



// working bands are the ones that initially fill up
static INBAND *input_band[NUMBER_OF_INPUT_BANDS];

// index of the current bands being used
static int current_read_band, current_print_band, current_head;

// used for marking channels as "used"
static unsigned char channellist[8] = {CHANNEL_1, CHANNEL_2, CHANNEL_3, CHANNEL_4, CHANNEL_5, CHANNEL_6, CHANNEL_7, CHANNEL_8};

// make sure bands get printed in order even if out of order in memory
static int current_band_number;
static int current_headfmt_number;

static BOOL GeojFlag;

int INPUT_BAND_HEIGHT;


// serialization of swath list records
CCriticalSection		BandQueueCS;
//CCriticalSection		HeadQueueCS;

PRINT_HEAD head[NUMBER_OF_WORKING_HEADS];

extern offset_list	GOffsets;

/*-------------------------------------------------------------------------------*/
/*- Make sure all buffers are allocated                                         -*/
/*-------------------------------------------------------------------------------*/
BOOL BandsAllocated() {
int i;

	for(i = 0; i < NUMBER_OF_INPUT_BANDS; i++) {
		if(input_band[i] == NULL)
			return	FALSE;
	}

	return TRUE;

}

/*-------------------------------------------------------------------------------*/
/*-  See if the first two bands are available                                   -*/
/*-------------------------------------------------------------------------------*/
BOOL SomeBandsAvailable() {

	if( input_band[0]->state == BAND_STATE_UNUSED &&
		input_band[1]->state == BAND_STATE_UNUSED &&
		input_band[2]->state == BAND_STATE_UNUSED &&
		input_band[3]->state == BAND_STATE_UNUSED) {

		return TRUE;
	}

	return FALSE;
}


/*-------------------------------------------------------------------------------*/
/*-  See if all band data has been sent                                         -*/
/*-------------------------------------------------------------------------------*/
static BOOL NoMoreBands(PRINT_HEAD *hd, int bandnumber) {
int i, acum;

retrynmb:

	if(theApp.GMasterStatus == MASTER_STATUS_ABORTING)
		return TRUE;

	acum = 0;

	for(i = 0; i < TOTAL_CHANNELS; i++) {
		if(!max_Y_received[i]) return FALSE;
	}

	for(i = 0; i < NUMBER_OF_WORKING_HEADS; i++) {
	
		if(&head[i] != hd && head[i].state != HEAD_STATE_UNUSED) {
			return FALSE;
		}

	}

	for(i = 0; i < NUMBER_OF_INPUT_BANDS; i++) {

		if(input_band[i] != NULL) {
			acum += input_band[i]->state;
		}

	}

	if(acum > 1) return FALSE;

	else {
		// return true UNLESS... the lead-out has not been
		// issued. In which case, wait until lead out is complete
		// then make sure there's no new bands to send
		#ifndef NO_LEAD_OUT_RASTERS
		if(!theApp.bLeadOutComplete) {
			int lo_timeout = 0;
			while(!theApp.bLeadOutComplete) {
				Sleep(1);
				lo_timeout++;
				if(lo_timeout > 4000) {
				//if(lo_timeout > 20000) {
					Sleep(0);
					break;
				}
			}
			Sleep(500);
			goto retrynmb;
		}
		#endif
		return TRUE;
	}

}


/*----------------------------------------------------------------------------------*/
/*-                                                                                -*/
/*----------------------------------------------------------------------------------*/
void InitiateJobPrint(QUEUE_ITEM  *qiptr) {

	theDlg->bIsAReprint = TRUE;

	if(!strlen(qiptr->szSourceFile)) {
		strcpy(qiptr->szSourceFile, qiptr->szFileName[0]);
	}

	theDlg->StatusLine("File: %s\n", qiptr->szSourceFile);


	theDlg->StatusLine("Creating Worker Thread\n");

	SECURITY_ATTRIBUTES ThreadSA;

	ThreadSA.nLength = sizeof(SECURITY_ATTRIBUTES);
	ThreadSA.bInheritHandle = TRUE;
	ThreadSA.lpSecurityDescriptor  = NULL;

	theApp.bInJob = 1;
	theDlg->StatusLine("Resuming Formatter Thread\n");
	theApp.HeadFormatterThread->ResumeThread();
	theApp.TempHandlerThread->SuspendThread();


	parserIOStruct ios;
	ios.szSourceFile = qiptr->szSourceFile;
	ios.theDlg = (void *)NULL;
	ios.Done = FALSE;
	ios.copies = 1;
	ios.qitem = qiptr; //GetQItemInfo();
	ios.sizeX = GS.PaperDimensionX;
	ios.sizeY = GS.PaperDimensionY;
	if(ios.sizeX == 0.0) ios.sizeX = qiptr->printarea.x;
	if(ios.sizeY == 0.0) ios.sizeY = qiptr->printarea.y;

	if(theApp.GMasterStatus == MASTER_STATUS_CANCELLING) {
		/**/ TRACE("Status is cancelling... returning out\n");
		return;
	}

	theApp.GMasterStatus = MASTER_STATUS_PRINTING;
	/**/ TRACE("Setting status to printing [2]\n");

	GbandNo = 0;

	/**/ TRACE("Launching PrintingDialogLoop (2)\n");

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

	Sleep(500);

}

/*-------------------------------------------------------------------------------*/
/*-  Send second pass (same data)                                               -*/
/*-------------------------------------------------------------------------------*/
static void SendSecondPass(displayGraphicState *gs) {
int i;
CTH_BAND_INFO bi;
unsigned long bytes;

	/**/ theDlg->StatusLine("Closing repeat file and starting second pass\r\n");
	if(theApp.hRepeatDataHandle != INVALID_HANDLE_VALUE) {
		CloseHandle(theApp.hRepeatDataHandle);
	}
	
	USBPrinterCommand(USB_CMD_RAPID_RETURN, gs->ResolutionY);

	theApp.hRepeatDataHandle = CreateFile(	"TMP_DATA_REPEAT.BIN",
											GENERIC_READ,
											FILE_SHARE_READ,
											(LPSECURITY_ATTRIBUTES)NULL,
											OPEN_EXISTING,
  											FILE_ATTRIBUTE_NORMAL,
											(HANDLE)NULL);

	if(theApp.hRepeatDataHandle != INVALID_HANDLE_VALUE) {

		for(i = 0; i < theApp.TotalPixelBands; i++) {
		
			ReadFile(theApp.hRepeatDataHandle, (unsigned char *)&bi, sizeof(CTH_BAND_INFO), &bytes, NULL);
			ReadFile(theApp.hRepeatDataHandle, head[0].formatted_data, theApp.TotalPixelBytes, &bytes, NULL);
			FTDWrite(&FPGAusb, (unsigned char *)&bi, sizeof(CTH_BAND_INFO));
			FTDWrite(&FPGAusb, head[0].formatted_data, theApp.TotalPixelBytes);
		
		}

		USBPrinterCommand(USB_CMD_HOME_PLATEN,0);
		USBPrinterCommand(USB_CMD_PARK,0);
		GLastBand = GBandNumber;
		theApp.bInJob = FALSE;

		CloseHandle(theApp.hRepeatDataHandle);

	}


	theApp.hRepeatDataHandle = INVALID_HANDLE_VALUE;


}


/*-------------------------------------------------------------------------------*/
/*-  Auxiliary procedure to send layers                                         -*/
/*-------------------------------------------------------------------------------*/
void AuxLayerPrinterProc(void *v) {
CLayerWait dlg;
CPleaseWait pwdlg;
int result;
QUEUE_ITEM *q = (QUEUE_ITEM *)v;
BOOL bZeroDelay = (/*theApp.GVars->LayerDelayMin == 1 && */ theApp.GVars->LayerDelaySec == 1);

	if(theApp.GVars->bPendingCleanRequest) return;

	/**/ TRACE("***************AuxLayerPrinterProc begin\n");

	theApp.bProlongPrintWindow = FALSE;

	while(theApp.PrintingWindow) {
		Sleep(0);
	}

	/**/ TRACE("HaltEvent 334\n");

	theApp.HaltJobScanner = TRUE;

	if(bZeroDelay) {
		result = 1;
	}
	else {
		result = dlg.DoModal();
	}

	/**/ TRACE("AuxLayerPrinterProc after dialog\n");


	if(result != 2) {


		Sleep(0);
	

		/// pwdlg.Create(IDD_PLEASEWAIT,  AfxGetMainWnd());

		
		/**/ TRACE("AuxLayerPrinterProc waiting 1 second\n");

		//if(!bZeroDelay)
			Sleep(1000);
		//	Sleep(250);


		if(theApp.GPlatenPosition != 1 && theApp.GPlatenPosition != 2) {

			/**/ TRACE("AuxLayerPrinterProc marker 1\n");

			USBPrinterCommand(USB_CMD_TRAY_POS_REPORT_REQ, 1);
			Sleep(0);
			Sleep(1000);

		}

		//theApp.bCenterButton = TRUE;
		
		theApp.bPrintingSecondLayer = TRUE;

		if(dlg.bWaitAborted) { // force the ready flag (bad idea?)
			USBPrinterCommand(USB_CMD_FORCE_RAPIDCOLORPASS, 1);
		}

		/**/ TRACE("********* AuxLayerPrinterProc marker 1a\n");

		ExecuteReprint(q, FALSE);

	}
	else {

		/**/ TRACE("AuxLayerPrinterProc marker 2\n");


		USBPrinterCommand(USB_CMD_HOME_PLATEN,0);
		USBPrinterCommand(USB_CMD_PARK,0);
		// theApp.bInJob = FALSE;
		/**/ TRACE("HaltEvent 380\n");
		theApp.HaltJobScanner = FALSE;
		/**/ TRACE("Setting GMasterStatus to idle (393)\n");
		theApp.GMasterStatus = MASTER_STATUS_IDLE;
	}

}

/*-------------------------------------------------------------------------------*/
/*-                                                                             -*/
/*-------------------------------------------------------------------------------*/
static void DoAutoColorLayer() {

	QUEUE_ITEM *q = FindOtherLayer(theApp.printing_qitem, FIND_COLOR);

	if(q != NULL) {
		
		theApp.AuxLayerPrinterThread = AfxBeginThread((AFX_THREADPROC)AuxLayerPrinterProc,
													(void *)q,
													0, //THREAD_PRIORITY_NORMAL,
													0,
													0,
													&theApp.ThreadSA);
		Sleep(0);
		Sleep(500);

	}

}

static void SetQuickReturn(displayGraphicState *gs) {

	if(((gs->layerType & 0x0F) == LAYER_TP_WHITE || (gs->layerType & 0x0F) == LAYER_TP_DOUBLE_WHITE )) {

		if( (gs->layerType & 0x0F) == LAYER_TP_WHITE ||
			(gs->layerType & 0x0F) == LAYER_TP_DOUBLE_WHITE) {

			if(theApp.GVars->LayerDelaySec <= 3 && theApp.GVars->bEnableColorQuickReturn) {
				theApp.bQuickReturn = TRUE;
			}
			else {
				theApp.bQuickReturn = FALSE;
			}

		}
		else {
				theApp.bQuickReturn = FALSE;
		}

	}
	else {
			theApp.bQuickReturn = FALSE;
	}

}


/*-------------------------------------------------------------------------------*/
/*-                                                                             -*/
/*-------------------------------------------------------------------------------*/
void SaveHeadSwath(CTH_BAND_INFO *band, PRINT_HEAD *hd, displayGraphicState *gs){
char szFileName[MAX_PATH];
int i;
static BOOL bIsWhite;
QUEUE_ITEM *qi = (QUEUE_ITEM *)theApp.CurrentJob;
static int Width, Height;
static iXY Resolution;
unsigned long bytes;
static HANDLE fh;


	if(GbandNo == 0) {

		bIsWhite = (gs->layerType & 0x0F) == LAYER_TP_DOUBLE_WHITE || (gs->layerType & 0x0F) == LAYER_TP_DOUBLE_WHITE_ONLY;

		//GVars->PlatenHeight[GVars->iSelectedPlaten]

		Width = qi->printarea.x;
		Height = qi->printarea.y;
		
		Resolution.x = gs->ResolutionX;
		Resolution.y = gs->ResolutionY;

		CreateDirectory("BAND_DEBUG", NULL);
		CreateDirectory("BAND_DEBUG\\COLOR", NULL);
		CreateDirectory("BAND_DEBUG\\WHITE", NULL);

		for(i = 0; i < 100; i++) {
			if(bIsWhite) {
				DeleteFile("BAND_DEBUG\\WHITE\\Composite.tif");
				DeleteFile("BAND_DEBUG\\WHITE\\metrics.dat");
				sprintf(szFileName, "BAND_DEBUG\\WHITE\\BAND%d.GBF", i);
				DeleteFile(szFileName);
				sprintf(szFileName, "BAND_DEBUG\\WHITE\\BAND%dC.TIF", i);
				DeleteFile(szFileName);
				sprintf(szFileName, "BAND_DEBUG\\WHITE\\BAND%dM.TIF", i);
				DeleteFile(szFileName);
				sprintf(szFileName, "BAND_DEBUG\\WHITE\\BAND%dY.TIF", i);
				DeleteFile(szFileName);
				sprintf(szFileName, "BAND_DEBUG\\WHITE\\BAND%dK.TIF", i);
				DeleteFile(szFileName);
				sprintf(szFileName, "BAND_DEBUG\\WHITE\\BAND%dCmp.TIF", i);
				DeleteFile(szFileName);
			}
			else {
				DeleteFile("BAND_DEBUG\\COLOR\\Composite.tif");
				DeleteFile("BAND_DEBUG\\COLOR\\metrics.dat");
				sprintf(szFileName, "BAND_DEBUG\\COLOR\\BAND%d.GBF", i);
				DeleteFile(szFileName);
				sprintf(szFileName, "BAND_DEBUG\\COLOR\\BAND%dC.TIF", i);
				DeleteFile(szFileName);
				sprintf(szFileName, "BAND_DEBUG\\COLOR\\BAND%dM.TIF", i);
				DeleteFile(szFileName);
				sprintf(szFileName, "BAND_DEBUG\\COLOR\\BAND%dY.TIF", i);
				DeleteFile(szFileName);
				sprintf(szFileName, "BAND_DEBUG\\COLOR\\BAND%dK.TIF", i);
				DeleteFile(szFileName);
				sprintf(szFileName, "BAND_DEBUG\\COLOR\\BAND%dCmp.TIF", i);
				DeleteFile(szFileName);
			}
		}

		if(bIsWhite)
			sprintf(szFileName, "BAND_DEBUG\\WHITE\\metrics.dat");
		else
			sprintf(szFileName, "BAND_DEBUG\\COLOR\\metrics.dat");

		fh = CreateFile(szFileName, 
						GENERIC_WRITE,
						0,
						(LPSECURITY_ATTRIBUTES)NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE)NULL);

		// Platen Width
		WriteFile(fh, (unsigned char *)&theApp.GVars->PlatenWidth[theApp.GVars->iSelectedPlaten], sizeof(float), &bytes, NULL);
		// Platen Height
		WriteFile(fh, (unsigned char *)&theApp.GVars->PlatenHeight[theApp.GVars->iSelectedPlaten], sizeof(float), &bytes, NULL);
		// Platen Offset
		WriteFile(fh, (unsigned char *)&theApp.GVars->PlatenOffset[theApp.GVars->iSelectedPlaten], sizeof(float), &bytes, NULL);
		/// Resolution
		WriteFile(fh, (unsigned char *)&Resolution, sizeof(iXY), &bytes, NULL);

		float centering;
		centering = ((float)MAXIMUM_PRINTABLE_WIDTH) - ((float)gs->pixelWidth / (float)gs->ResolutionX);
		centering /= (float)2;
		centering *= 1200;
		if(theApp.GVars->iCenteringOffset != 1)
			centering += theApp.GVars->iCenteringOffset;
		else
			centering += CENTERING_OFFSET;

		/// SOL
		WriteFile(fh, (unsigned char *)&centering, sizeof(float), &bytes, NULL);

		CloseHandle(fh);



	}

	if (bIsWhite) {

		sprintf(szFileName, "BAND_DEBUG\\WHITE\\BAND%d.GBF", GbandNo++);

	}
	else {

		sprintf(szFileName, "BAND_DEBUG\\COLOR\\BAND%d.GBF", GbandNo++);

	}


	DeleteFile(szFileName);

	fh = CreateFile(	szFileName, 
						GENERIC_WRITE,
						0,
						(LPSECURITY_ATTRIBUTES)NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE)NULL);


	if(fh == INVALID_HANDLE_VALUE) {
	
		return;
	
	}


	WriteFile(fh, (unsigned char *)band, sizeof(CTH_BAND_INFO), &bytes, NULL);
	WriteFile(fh, hd->formatted_data,  hd->TotalBytes, &bytes, NULL);


	CloseHandle(fh);

}

// #define TRANSMISSION_TIMER
/*-------------------------------------------------------------------------------*/
/*-  return address to head data                                                -*/
/*-------------------------------------------------------------------------------*/
void SendBand(PRINT_HEAD *hd, displayGraphicState *gs, BOOL advance_media) {
unsigned char *send_buffer;
unsigned char rsp[1024];
int rl, feed_amount, i;
unsigned long bytes;
#ifdef TRANSMISSION_TIMER
LARGE_INTEGER frequency;        // ticks per second
LARGE_INTEGER t1, t2;           // ticks
double elapsedTime;
#endif

	/**/ TRACE("*SENDBAND*\n");

	if(!theApp.NoUSB) {
		while(!(theApp.bPrinterPrepped && theApp.GPlatenPosition == 2) || theApp.GMasterStatus == MASTER_STATUS_PAUSED) {
			if(theApp.GMasterStatus == MASTER_STATUS_ABORTING) {
				theApp.TotalPixelBands++;
				theApp.TotalPixelBytes = hd->TotalBytes;
				/**/ TRACE("*Aborting sendband\n");
				goto abort_sendBand;
			}
			if(theApp.GMasterStatus == MASTER_STATUS_IDLE) {
				/**/ TRACE("SENDBAND exiting because status is %d\n", theApp.GMasterStatus);
				return;
			}
			Sleep(0);
		}
	}

	theApp.SendBandCS.Lock(INFINITE);

	if(theApp.GMasterStatus == MASTER_STATUS_CANCELLING || (theApp.GMasterStatus == MASTER_STATUS_IDLE && theApp.bJobWasCancelled)) {
		theApp.HTCounter++;
		if(theApp.HTCounter >= GET_HEAD_TEMP_INTERVAL) {
			theApp.HTCounter = 0;
		}

		LOCK_BANDQUEUE;
	   	hd->state = HEAD_STATE_UNUSED;
		UNLOCK_BANDQUEUE;

		//FTDPurge(&FPGAusb);
		//FTDPurge(&AUXusb);

		// 	theApp.SendBandCS.Unlock();
		//return;

		/**/ TRACE("Going to SKIPBAND\n");
		/**/goto skip_sendband;

	}

	CTH_BAND_INFO *band;

	LOCK_BANDQUEUE
	hd->state = HEAD_STATE_PRINTING;
	UNLOCK_BANDQUEUE

	GBandNumber++;

	if(hd->ByteLength == USB_COMMAND) {

#ifdef VERBOSE_BAND_DEBUG
		/**/ TRACE("Band is advance (%d) %d %d\n", advance_media, USB_CMD_FEED, gs->Y_Advance);
#endif
		if(advance_media) {
			USBPrinterCommand(USB_CMD_FEED, gs->Y_Advance);
			
			feed_amount = gs->Y_Advance;

			// theDlg->StatusLine("Empty Feed: %d\r\n", gs->Y_Advance);
		}
	}
	else {

#ifdef VERBOSE_BAND_DEBUG
		/**/ TRACE("Band is pixel data\n", advance_media, USB_CMD_FEED, gs->Y_Advance);
#endif


		if(!theApp.bYPrintMovementSet) {
			theApp.bYPrintMovementSet = TRUE;
		}


		band = PrepareBandPacket(hd, 0, gs, advance_media, &feed_amount);
		band->prefix[0] = band->prefix[1] = band->prefix[2] = band->prefix[3] = USB_PREFIX;
		
		// theDlg->StatusLine("Band Feed: %d\r\n", feed_amount);   

		if(theApp.GVars->ConfirmBands) {
			MessageBox(GetFocus(), "PRESS OK TO SEND BAND", APP_NAME, MB_OK|MB_SYSTEMMODAL);
		}


		/////////////
		int auxWriteTimeout = 0;

		// memcpy(hd->formatted_data_aux, hd->formatted_data, hd->TotalBytes);
		// memset(hd->formatted_data, 0, hd->TotalBytes); /// <---- REMOVE THIS!!!!
		// memset(hd->formatted_data_aux, 0, hd->TotalBytes); /// <---- REMOVE THIS!!!!


#ifdef COUNT_DOT_THREES
	CountDotThrees(hd);
#endif

			
			
		if(!theApp.NoUSB) {

			if(theApp.bDualHead) {
				theApp.AuxBandData = (unsigned char *)band;
				theApp.AuxBufferData = hd->formatted_data_aux;
				theApp.AuxBufferDataLength = hd->TotalBytes;
				SetEvent(theApp.hAuxBandReadyEvent);
				Sleep(50);
			}

			// MessageBox(GetFocus(), "Click OK to Send Band", "DEBUG", MB_OK|MB_SYSTEMMODAL);

#ifdef VERBOSE_BAND_DEBUG
			/**/ TRACE("Send Band Data...\n");
#endif

#ifdef TRANSMISSION_TIMER
			QueryPerformanceFrequency(&frequency);
			QueryPerformanceCounter(&t1);
#endif
			/**/ TRACE("Sending Band\n");
			if(theApp.GVars->bUseOneBitPP) {
				memcpy(hd->formatted_data_1bit, (unsigned char *)band, sizeof(CTH_BAND_INFO));
				FTDWrite(&FPGAusb, hd->formatted_data_1bit, hd->TotalBytes + sizeof(CTH_BAND_INFO));
			}
			else {
				FTDWrite(&FPGAusb, (unsigned char *)band, sizeof(CTH_BAND_INFO));
				FTDWrite(&FPGAusb, hd->formatted_data, hd->TotalBytes);
			}

			theApp.iPrintedBandCount++;

#ifdef TRANSMISSION_TIMER
			QueryPerformanceCounter(&t2);
			elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
			//theDlg->StatusLine("Band Send Time: %ld\n", elapsedTime);
			/**/ TRACE("Send Time: %lf\n", elapsedTime);
#endif


#ifdef VERBOSE_BAND_DEBUG
			/**/ TRACE("Band Data Sent...\n");
#endif

			theApp.TotalPixelBands++;
			theApp.TotalPixelBytes = hd->TotalBytes;

		}

		else {
			SaveHeadSwath(band, hd, gs);
		}

		theApp.HTCounter++;
		if(theApp.HTCounter >= GET_HEAD_TEMP_INTERVAL) {
			theApp.HTCounter = 0;
#ifdef GET_TEMPS
			if(!GetHeadTemps()) {
				theDlg->StatusLine("Could not get head temp\n");
			}
#endif
		}

#ifdef SIMULATE_PRINTING_DELAY
		if(theApp.NoUSB)
			Sleep(500); // to simulate actual printing delay
#endif


		/**/ //// for debug only
		FlipDWORD(&band->bytelength);
		FlipDWORD(&band->start_x);
		FlipDWORD(&band->length_x);
		FlipDWORD(&band->special_x);
		FlipDWORD(&band->y_step);
		FlipDWORD(&band->direction);
		FlipDWORD(&band->lastband);
		FlipDWORD(&band->flags);
		
		//theDlg->StatusLine("BND: %d|%d|%d|%d\r\n", band->start_x, band->bytelength, band->length_x,band->y_step);   

		//TRACE("channel 1 band length_x: %d\n", band->length_x);
		/**/TRACE("channel 1 band length_x: %d\n", band->bytelength);

		////////////////////////////
		////////////////////////////
		/**/
		
		free(band);
	}
	
skip_sendband:

	if(NoMoreBands(hd, GBandNumber)) {

		/**/ TRACE("NomoreBands true at band %d\n", GBandNumber);

		if(theApp.GVars->TotalJobs >= 0) {
			theApp.GVars->TotalJobs++;
			WriteEncodedPrintCount(theApp.GVars->TotalJobs);
		}


		theApp.time_of_last_print = time(0);

		theApp.bQuickReturn = FALSE;

		if(QueueItemStillValid(theApp.printing_qitem)) {

			if(theApp.GVars->bAutoSendColorLayer && theApp.bCenterButton) {
				SetQuickReturn(gs);
			}

		}

		if((gs->layerType & 0x0F) != LAYER_TP_WHITE && (gs->layerType & 0x0F) != LAYER_TP_DOUBLE_WHITE ) {

			theApp.GVars->CurrentCount++;

			theDlg->InvalidateRect(&theDlg->ButtonArea[MAIN_COUNTER_BUTTON].slice[0]);
			theDlg->InvalidateRect(&theDlg->ButtonArea[CURRENT_COUNTER_BUTTON].slice[0]);

			theApp.GVars->iColorJobCount++;
		}

		if(!theApp.bJobWasCancelled && (gs->layerType & 0x0F) == LAYER_TP_DOUBLE_WHITE_ONLY) {
			theApp.GVars->iWhiteJobCount++;
		}



		if(!theApp.bJobWasCancelled && theApp.GMasterStatus != MASTER_STATUS_CANCELLING && theApp.GMasterStatus != MASTER_STATUS_ABORTING) {
			theApp.printing_qitem->iCount++;
			theApp.WriteQueue();
		}

		USBPrinterCommand(USB_CMD_BOUNCE_CMD, BOUNCE_SIGNAL_NOMORECANCEL);

		USBPrinterCommand(USB_CMD_HOME_PLATEN, theApp.bQuickReturn);

		theApp.bProlongPrintWindow = theApp.bQuickReturn; // causes the white print dialog to remain longer

		USBPrinterCommand(USB_CMD_BOUNCE_CMD, BOUNCE_SIGNAL_LAST_BAND);

		//USBPrinterCommand(USB_CMD_PARK,0);

		//// if no printer, close the window now
		if(theApp.NoUSB) {
			theApp.ClosePrintingWindow = TRUE;

#ifdef ENABLE_DELETE_ONLY_JOB
			if(theApp.GVars->MaxJobs == 1 && theApp.GVars->bDeleteAfterPrinting/* && theApp.printing_qitem->iProdMode != PRODMODE_MULTIPLE*/) {

				while(DeleteOldestPrinted()) {
					Sleep(500);
				}

			}
#endif

			/**/ TRACE("HaltEvent 619\n");
			theApp.HaltJobScanner = FALSE;

		}



		GLastBand = GBandNumber;
		theApp.bInJob = FALSE;

		if(theApp.GVars->iColorJobsBeforeClean > 0 && theApp.GVars->iWhiteJobsBeforeClean > 0) {
			if(theApp.GVars->iColorJobCount >= theApp.GVars->iColorJobsBeforeClean ||
				theApp.GVars->iWhiteJobCount >= theApp.GVars->iWhiteJobsBeforeClean) {

					theApp.GVars->iColorJobCount = 0;
					theApp.GVars->iWhiteJobCount = 0;

					//USBPrinterCommandEx(USB_CMD_CLEAN, (unsigned long)0x03, 1, 0, 0, 0, 0, 0);
					//theApp.GVars->bPendingCleanRequest = TRUE;


			}
		}

		theApp.ScannerDelay = 1;

		/*
		if(theApp.hScannerThread->ResumeThread() == 0xFFFFFFFF) {
			MessageBox(GetFocus(), "** COULD NOT resume scanner thread", APP_NAME, MB_OK|MB_SYSTEMMODAL);
		}
		*/

		//CController4Dlg4 dlg;
		//dlg.CheckForJobOverflow();

		//dlgptr->CheckForJobOverflow();
		while(!OKToAddJob()) {
			if(!DeleteOldestPrinted()) {
				break;
			}
		}


		if(QueueItemStillValid(theApp.printing_qitem)) {

			if(theApp.GVars->bAutoSendColorLayer && theApp.bCenterButton) {

				if(((gs->layerType & 0x0F) == LAYER_TP_WHITE || (gs->layerType & 0x0F) == LAYER_TP_DOUBLE_WHITE )) {

					SetQuickReturn(gs);

					/**/ TRACE("QUICKRETURN: %d\n", theApp.bQuickReturn);

					USBPrinterCommand(USB_CMD_PARK,theApp.GVars->bEnableColorQuickReturn?theApp.bQuickReturn:0);


					if(theApp.NoUSB) {
						Sleep(2000);
						theApp.GHeadPosition = 0;
						theApp.GPlatenPosition = 1;
					}


					int tmout = 0;

					if(theApp.bQuickReturn) {

						while(theApp.GPlatenPosition != 1 && theApp.GHeadPosition != 0) {
				
							Sleep(1000);

							tmout++;

							if(tmout > RAPID_RETURN_TRAY_TIMEOUT) {
								TRACE("RAPID_RETURN_TRAY_TIMEOUT exceeded\n");
								break;
							}
				
						}
						/**/ TRACE("Platen and Head home!!\n");
					}

					if(tmout < RAPID_RETURN_TRAY_TIMEOUT) {

						/// it needs this if no delay...
						//if(theApp.GVars->LayerDelayMin == 1 && theApp.GVars->LayerDelaySec == 1) {
							//USBPrinterCommand(USB_CMD_SYM_START_BUTTON, 1);
						//}
						DoAutoColorLayer();
						Sleep(0);
						advance_media = 0;

					}
					else {
						theDlg->OKBox("** ERROR **", "HEAD RETURN TIMEOUT", NULL);
						USBPrinterCommand(USB_CMD_PARK, 0);
					}

				}
				else {
					USBPrinterCommand(USB_CMD_PARK, 0);
				}

			}
			else {
				USBPrinterCommand(USB_CMD_PARK, 0);
			}

		}
		else {
			USBPrinterCommand(USB_CMD_PARK, 0);
		}

		unsigned long suspend_count = theApp.TempHandlerThread->ResumeThread();
		if(suspend_count == 0xFFFFFFFF) {
			MessageBox(GetFocus(), "** COULD NOT resume scanner thread", APP_NAME, MB_OK|MB_SYSTEMMODAL);
		}
		else {
			while(suspend_count > 1) {
				suspend_count = theApp.hScannerThread->ResumeThread();
				if(suspend_count == 0xFFFFFFFF) {
					MessageBox(GetFocus(), "** COULD NOT resume scanner thread (2)", APP_NAME, MB_OK|MB_SYSTEMMODAL);
				}
			}
		}

		/*
		if(	(gs->layerType & 0x0F) == LAYER_TP_SINGLE_COLOR || 
			gs->layerType & 0x0F) == LAYER_TP_DOUBLE_COLOR || 
			gs->layerType & 0x0F) == LAYER_TP_DOUBLE_WHITE_ONLY || 
			(gs->layerType & 0x0F) == LAYER_TP_SINGLE_COLOR_SINGLE_WAVE) {

			TRACE("HaltEvent 752\n");
			theApp.HaltJobScanner = FALSE;

		}
		*/

		theApp.LastLayerType = (gs->layerType & 0x0F);
		if(theApp.GMasterStatus == MASTER_STATUS_CANCELLING) {
			/**/ TRACE("HaltEvent 760\n");
			theApp.HaltJobScanner = FALSE;
		}

		if(!theApp.bCenterButton) {
			/**/ TRACE("Restarting job scanner because we printed from queue screen\n");
			/**/ TRACE("HaltEvent 766\n");
			theApp.HaltJobScanner = FALSE;
		}

		if(theApp.NoUSB) {
			if(theApp.LastLayerType == LAYER_TP_SINGLE_COLOR || theApp.LastLayerType == LAYER_TP_SINGLE_COLOR_SINGLE_WAVE) {
				/**/ TRACE("HaltEvent 772\n");
				theApp.HaltJobScanner = FALSE;
				theApp.GMasterStatus = MASTER_STATUS_IDLE;
			}
		}

		/**/
		////////////// added 9/29/16
		USBPrinterCommand(USB_CMD_BOUNCE_CMD, BOUNCE_SIGNAL_LAST_BAND);


	} /// no more bands

	if(advance_media) {

		/// Update the completion preview

		theApp.GSentRasters += TOTAL_NOZZLES_HW;

		theApp.Y_Percent_Complete = (float)(theApp.GSentRasters + gs->TopMargin) / (float)gs->pixelHeight;

		theApp.Y_Percent_Complete *= (float)100;

		if(theApp.Y_Percent_Complete > 100.00)
			theApp.Y_Percent_Complete = 100.00;

		pdlg->UpdatePreviewImage(theApp.Y_Percent_Complete, (void *)gs);

	}

	theApp.SendBandCS.Unlock();


abort_sendBand:
	LOCK_BANDQUEUE
	hd->state = HEAD_STATE_UNUSED;
	UNLOCK_BANDQUEUE

}


	
/*-------------------------------------------------------------------------------*/
/*- Get the next available head index                                           -*/
/*-------------------------------------------------------------------------------*/
static int GetAvailableHead() {
int i;

	//LOCK_BANDQUEUE

	for(i = 0; i < NUMBER_OF_WORKING_HEADS; i++) {

		if(head[i].state == HEAD_STATE_UNUSED) {
	
			UNLOCK_BANDQUEUE

			return i;

		}

	}
	
	//UNLOCK_BANDQUEUE

	return -1;

}


/*-------------------------------------------------------------------------------*/
/*- Format for Print Head                                                       -*/
/*-------------------------------------------------------------------------------*/
void HeadFormatThread(void *p) {
int i;
time_t lTime, lTimeout;
BOOL bDidSomething = FALSE;


	Sleep(2000);

	time(&lTime); // reset the timeout

	while(!theApp.quit_threads) {

		if(theApp.bRunWPThreads) {
			// prevent CPU hogging
			if(!theApp.bInJob) {
				Sleep(2000);
				continue;
			}
			for(i = 0; i < NUMBER_OF_INPUT_BANDS; i++) {
				LOCK_BANDQUEUE;
				if(input_band[i] != NULL && input_band[i]->band == current_headfmt_number && input_band[i]->state == BAND_WEAVED_READY) {
					//UNLOCK_BANDQUEUE;
					time(&lTime); // reset the timeout
					current_head = GetAvailableHead();

					BOOL free_band = TRUE;

					free_band = TRUE;

resend_band:
					if(current_head != -1 && CreateHeadSwath(&head[current_head], input_band[i], free_band)) {

#ifdef HEAP_DEBUG
						if(!_CrtCheckMemory()) {
							MessageBox(GetFocus(), "HEAP CORRUPTED(1)", "DEBUG", MB_OK|MB_SYSTEMMODAL);
						}
#endif


#ifdef DEBUG_INCOMING_SWATHS
						 theDlg->StatusLine("Swath %d Created, sending\r\n", current_headfmt_number);
#endif
						 int ready_timeout = 0;
						 while(theApp.GMasterStatus == MASTER_STATUS_IDLE) {

							 /// ADD TIMEOUT ERROR HERE
							 Sleep(100); 

							 ready_timeout += 100;

							 if(theApp.bJobWasCancelled) {
								 break;
							 }

							else if(ready_timeout > 3000) {
								/**/theDlg->OKBox("*** ERROR **", "BAND READY TIMEOUT", "");
								//theApp.bInJob = FALSE; /// <=== is this safe???
								//theApp.ParserCount = 0; /// <=== is this safe???
								theApp.bHardStop = TRUE;
								break;
							}

						 }

						 if(theApp.GMasterStatus != MASTER_STATUS_IDLE) {
#ifdef VERBOSE_BAND_DEBUG
							 /**/ TRACE("Calling Sendband (%d)\n", theApp.GMasterStatus);
#endif
							theApp.bSendingBand = TRUE;
							SendBand(&head[current_head], &input_band[i]->gs, free_band);
							Sleep(20);
							while(theApp.bSendingAuxBand)
								Sleep(0);
							theApp.bSendingBand = FALSE;

						 }

						if(free_band)
							current_headfmt_number++;
						time(&lTime); // reset the timeout
						break;

					}
				}
				else {
					UNLOCK_BANDQUEUE;
				}
			}
			Sleep(0);
		}
		else {
			Sleep(0);
		}
	}


}

/*-------------------------------------------------------------------------------*/
/*- Check to see if raster is blank                                             -*/
/*-------------------------------------------------------------------------------*/
BOOL isBlank(unsigned char *ras, int length) {
int i;

	for(i = 0; i < length; i++) {
		if(ras[i] != 0)
			return FALSE;
	}

	return TRUE;

}


/*----------------------------------------------------------------------------------*/
/*- Set up the INBAND structures that will be filled and used by the Head formatter -*/
/*- an array of INBAND structures is used. One can be printing while one can be    -*/
/*- filling and another waiting                                                    -*/
/*----------------------------------------------------------------------------------*/
void AllocateWeaveBuffers(displayGraphicState *gs) {
int b, i;

	/**/ TRACE("AllocateBuffers\n");

	// first halt all threads that use this stuff
	theDlg->StatusLine("Halting Formatter Thread\n");
	theApp.HeadFormatterThread->SuspendThread();

	SelectMW(gs);

	INPUT_BAND_HEIGHT = weave[gs->cur_mw].nozzles_used;

	GBand_Dir = 0;

	for(b = 0; b < NUMBER_OF_INPUT_BANDS; b++) {

		// if already allocated, free them
		if(input_band[b] != NULL) {

			if(input_band[b]->address != NULL) {
				free(input_band[b]->address);
			}
			if(input_band[b]->addressAux != NULL) {
				free(input_band[b]->addressAux);
			}

			free(input_band[b]);
			input_band[b] = NULL;

		}


#ifdef HARD_WIRE_UNIDIRECTIONAL
		gs->PrintDirection = 0;
#endif
		// allocate inband structures
		input_band[b] = (INBAND *)calloc(1, sizeof(INBAND));
		input_band[b]->byteWidth = gs->alloc_raslen;
		input_band[b]->pixelWidth = input_band[b]->byteWidth * 8 / gs->bitsPerPixel;
		input_band[b]->pixelHeight = INPUT_BAND_HEIGHT;
		input_band[b]->bufferLength = input_band[b]->byteWidth * INPUT_BAND_HEIGHT * TOTAL_CHANNELS;
		input_band[b]->PrintDirection = gs->PrintDirection;
		input_band[b]->address = (unsigned char *)calloc(1, input_band[b]->bufferLength * 1.25);

		if(theApp.bDualHead) {
			input_band[b]->addressAux = (unsigned char *)calloc(1, input_band[b]->bufferLength * 1.25);
		}

		input_band[b]->totalChannels = TOTAL_CHANNELS;
		input_band[b]->Resolution.x = gs->ResolutionX;
		input_band[b]->Resolution.y = gs->ResolutionY;
		memcpy(&input_band[b]->gs, gs, sizeof(displayGraphicState));

		for(i = 0; i < TOTAL_CHANNELS; i++) {
			input_band[b]->plane[i] = input_band[b]->address + (i * (input_band[b]->byteWidth * INPUT_BAND_HEIGHT));
			if(theApp.bDualHead) {
				input_band[b]->planeAux[i] = input_band[b]->addressAux + (i * (input_band[b]->byteWidth * INPUT_BAND_HEIGHT));
			}
		}

	}

	current_read_band = current_print_band = current_head = 0;
	
	current_band_number = 0;
	current_headfmt_number = 0;

	for(b = 0; b < NUMBER_OF_WORKING_HEADS; b++) {
		InitHead(&head[b]);
	}

	// now resume the thread
	theDlg->StatusLine("Resuming Formatter Thread\n");
	theApp.HeadFormatterThread->ResumeThread();
	
}

/*-------------------------------------------------------------------------------*/
/*-                                                                             -*/
/*-------------------------------------------------------------------------------*/
void FreeWeaveBuffers() {
int b;

	/**/ TRACE("FreeWeaveBuffers\n");
	//theApp.bRunWPThreads = FALSE;

	// if in the middle of creating a swath;
	while(theApp.bCreatingSwath) {
		Sleep(0);
	}

	theDlg->StatusLine("Halting Formatter Thread\n");
	theApp.HeadFormatterThread->SuspendThread();

	if(cur_flip_band_size) {
		if(flip_band != NULL)
			free(flip_band);
	}
	flip_band = NULL;
	cur_flip_band_size = 0;
	
	for(b = 0; b < NUMBER_OF_INPUT_BANDS; b++) {

		// if already allocated, free them
		if(input_band[b] != NULL) {

			if(input_band[b]->address != NULL) {
				free(input_band[b]->address);
			}
			if(input_band[b]->addressAux != NULL) {
				free(input_band[b]->addressAux);
			}

			free(input_band[b]);
			input_band[b] = NULL;
		}
	}

	current_band_number = 0;
	current_headfmt_number = 0;

	theDlg->StatusLine("Resuming Formatter Thread\n");
	theApp.HeadFormatterThread->ResumeThread();


}

/*-------------------------------------------------------------------------------*/
/*- Current read band is full, mark it and get the index to the next one to fill-*/
/*- if one isn't available, then block until one is                             -*/
/*-------------------------------------------------------------------------------*/
static int ReadBandIsFull() {
int i;
BOOL gotone = FALSE;
	
	LOCK_BANDQUEUE
	// mark this band full
	input_band[current_read_band]->state = BAND_WEAVED_READY;
	UNLOCK_BANDQUEUE

	// now get the next one or wait until one is available

	while(!gotone) {
		
		for(i = 0; i < NUMBER_OF_INPUT_BANDS; i++) {
		
			if(input_band[i] != NULL && input_band[i]->state == BAND_STATE_UNUSED) {
				gotone = TRUE; // i will become the new "current_read_band"
				theApp.bRunWPThreads = TRUE; // turn the weave and print thread on if not already on
				break;
			}
		
		}

		if(!gotone)
			Sleep(20);

	}
	// TRACE("Next band: %d\n", i);
	return i;
}

/*-------------------------------------------------------------------------------*/
/*-  Find the first left and right non-blank pixels                             -*/
/*-------------------------------------------------------------------------------*/
static void FindFirstPixels(offset_list *list, INBAND *band, displayGraphicState *gs) {
int i, c, r, left = gs->byteWidth, right = gs->byteWidth;
unsigned char *p;
unsigned char *rasterptr[TOTAL_CHANNELS], *rasters[TOTAL_CHANNELS][TOTAL_NOZZLES_HW];

	return;

	for(c = 0; c < TOTAL_CHANNELS; c++) {
		rasterptr[c] = band->plane[c];
		for(i = 0; i < TOTAL_NOZZLES_HW; i++) {
			rasters[c][i] = rasterptr[c] + (i * band->byteWidth);
		}
	}

	// find left side
	for(c = 0; c < TOTAL_CHANNELS; c++) {
		
		for(r = 0; r < TOTAL_NOZZLES_HW; r++) {

			p = rasters[c][r];

			for(i = 0; i < gs->byteWidth; i++, p++) {

				if(*p && i < left) {
					left = i;
					break;
				}
	
			}
		}
	}	
	

	// find right side

	for(c = 0; c < TOTAL_CHANNELS; c++) {

		for(r = 0; r < TOTAL_NOZZLES_HW; r++) {

			p = rasters[c][r] + gs->byteWidth - 1;
		
			for(i = 0; i < gs->byteWidth; i++, p--) {

				if(*p && i < right) {
					right = i;
					break;
				}
	
			}
		}
	}

	if(left < ADDITIONAL_X_REWIND)
		list->elements[band->band].FirstPix[0] = 0;
	else
		list->elements[band->band].FirstPix[0] = left - ADDITIONAL_X_REWIND;

	if(right < ADDITIONAL_X_REWIND)
		list->elements[band->band].FirstPix[1] = 0;
	else
		list->elements[band->band].FirstPix[1] = right - ADDITIONAL_X_REWIND;

	// just so the next one is never zero, copy to next one as well...
	memcpy((char *)&list->elements[band->band + 1], (char *)&list->elements[band->band], sizeof(LROffst));

	list->ilastused = band->band;

}

/*-------------------------------------------------------------------------------*/
/*- Add a raster to the currently filling band                                  -*/
/*-------------------------------------------------------------------------------*/
void AddRaster(unsigned char *ras, int channel, int length, int forceEnd, displayGraphicState *gs) {
int i;

#ifdef DEBUG_ADDRASTER
	TRACE("Addraster %d %d %d\n", channel, length, forceEnd);
#endif

	// if first time using this band, set the plane pointers
	LOCK_BANDQUEUE
	
	memcpy((char *)&input_band[current_read_band]->gs, gs, sizeof(displayGraphicState));

	if(input_band[current_read_band]->state == BAND_STATE_UNUSED) {
		input_band[current_read_band]->state = BAND_STATE_FILLING;
		input_band[current_read_band]->band = current_band_number;
		input_band[current_read_band]->raster = 0;
		input_band[current_read_band]->UsedChannels = 0;
		for(i = 0; i < TOTAL_CHANNELS; i++) {
			input_band[current_read_band]->rasterSlider[i] = input_band[current_read_band]->plane[i];
			memset(input_band[current_read_band]->rasterSlider[i], 0, input_band[current_read_band]->byteWidth);
		}
	}
	UNLOCK_BANDQUEUE

	if(!isBlank(ras, length)) {
		// if incoming raster is not blank, mark that channeld "used";
		input_band[current_read_band]->UsedChannels |= channellist[channel];
		
		///////////////////////////////// debug only
		//if(channel == 1) {
		//	int foo;
		//	foo = 1;
		//}
		////////////////////////////////////////////


		//memcpy(input_band[current_read_band]->rasterSlider[channel], ras, length);
		memcpy(input_band[current_read_band]->rasterSlider[channel], ras, input_band[current_read_band]->byteWidth);
	}
	else {
		// otherwise, blank out that raster
		//memset(input_band[current_read_band]->rasterSlider[channel], 0, length);
		memset(input_band[current_read_band]->rasterSlider[channel], 0, input_band[current_read_band]->byteWidth);
	}

	input_band[current_read_band]->rasterSlider[channel] += input_band[current_read_band]->byteWidth;

	if(channel == TOTAL_CHANNELS - 1) {

		input_band[current_read_band]->raster++;

		// this means it's the last band so go ahead
		// and fill it with zeroes
		if(forceEnd) {
			input_band[current_read_band]->raster = INPUT_BAND_HEIGHT;
		}

		// if it's the last raster of the last color then this band is full and ready
		if(input_band[current_read_band]->raster == INPUT_BAND_HEIGHT) {
			// if not using the full head, blank out the extra rasters
			if(INPUT_BAND_HEIGHT != INPUT_BAND_HEIGHT_HW) {
				int r, c;
				for(r = INPUT_BAND_HEIGHT; r < INPUT_BAND_HEIGHT_HW; r++) {
					for(c = 0; c < TOTAL_CHANNELS; c++) {
						memset(input_band[current_read_band]->rasterSlider[c], 0, length);
						input_band[current_read_band]->rasterSlider[c] += input_band[current_read_band]->byteWidth;
					}
				}
			}
			////////////////
			FindFirstPixels(&GOffsets, input_band[current_read_band], gs);
			////////////////

			current_read_band = ReadBandIsFull(); // this will block until a band is ready to fill
			current_band_number++; // bump up the current filling band number
		}

	}

}


/*-------------------------------------------------------------------------------*/
/*- FOR DEBUG ONLY. Create a TIF of the current band to be Head                 -*/
/*-------------------------------------------------------------------------------*/
extern 	void CreateBandPreview(INBAND *inband) {
int i, c, nRet;
BITMAPHANDLE Bitmap[8];
char szFileName[128];
unsigned char *rasptr[8];


	for(c = 0; c < 8; c++) {
		nRet = L_CreateBitmap(	&Bitmap[c],
								sizeof(BITMAPHANDLE),
								TYPE_CONV,
								inband->byteWidth, 
								inband->pixelHeight,
								8,
								ORDER_GRAY,
								NULL,
								TOP_LEFT,
								NULL, 0);

		rasptr[c] = inband->plane[c];
	}

	for(i = 0; i < inband->pixelHeight; i++) {
	
		for(c = 0; c < 8; c++) {

			nRet = L_PutBitmapRow(&Bitmap[c], rasptr[c], i, inband->byteWidth);

			rasptr[c] += inband->byteWidth;

		}
	
	
	}

	for(c = 0; c < 8; c++) {
		sprintf(szFileName, "weaved_band_%d_%d.TIF", inband->band, c);
		DeleteFile(szFileName);
		nRet = L_SaveBitmap(szFileName, &Bitmap[c], FILE_TIF, 8, 0, NULL);
	}


}

/*-------------------------------------------------------------------------------*/
/*- Set a flag indicating that end of job was reached for current job           -*/
/*-------------------------------------------------------------------------------*/
void SetGEOJ(BOOL st) {
	GeojFlag = st;
}

/*-------------------------------------------------------------------------------*/
/*- Send start-of-line to printer. This auto-centers the job                    -*/
/*-------------------------------------------------------------------------------*/
void SendSOL(displayGraphicState *gs) {
float centering;

	centering = ((float)MAXIMUM_PRINTABLE_WIDTH) - ((float)gs->pixelWidth / (float)gs->ResolutionX);
	
	centering /= (float)2;

	centering *= 1200;

	if(theApp.GVars->iCenteringOffset != 1)
		centering += theApp.GVars->iCenteringOffset;
	else
		centering += CENTERING_OFFSET;

	USBPrinterCommand(USB_SET_SOL, (int)centering);

	/**/ TRACE("Sending SOL: %d (@ 1200 dpi)\n", (int)centering);

	
}

/*-------------------------------------------------------------------------------*/
/*- Pixel repeater                                                              -*/
/*-------------------------------------------------------------------------------*/
void SendPixelRepeat(int r) {

	if(r < 4 && r >= 0) {
		theDlg->StatusLine("Setting repeater to %d\r\n", r);
		USBPrinterCommand(USB_CMD_SET_PIXEL_REPEAT, r);
	}
	else {
		theDlg->StatusLine("Repeater value %d out of bounds\r\n", r);
	}


}

/*-------------------------------------------------------------------------------*/
/*- Send Head Temp                                                              -*/
/*-------------------------------------------------------------------------------*/
void SendHeadTemp(int tmp1, int tmp2) {

	USBPrinterCommand(USB_CMD_SET_HEADTEMP, tmp1);
	Sleep(100);
	USBPrinterCommand(USB_CMD_SET_HEADTEMP2, tmp2);

}

/*-------------------------------------------------------------------------------*/
/*- Send Head Temp                                                              -*/
/*-------------------------------------------------------------------------------*/
void EnableHeadHeaters(BOOL h1) {

	if(theApp.GVars->bUseHeaters) USBPrinterCommand(USB_CMD_ENABLE_HEATERS, h1);
	
}


/*-----------------------------------------------------------------------------------------------------*/
/*-  Send leveling offsets                                                                            -*/
/*-----------------------------------------------------------------------------------------------------*/
void SendPlatenLevelingOffset() {

	USBPrinterCommandEx(USB_CMD_SET_SERVOOFFSETS,

		theApp.GVars->LiftServoCorr[0],
		theApp.GVars->LiftServoCorr[1],
		theApp.GVars->LiftServoCorr[2],
		theApp.GVars->LiftServoCorr[3],

		0, 0, 0);

}



/*-----------------------------------------------------------------------------------------------------*/
/*-  Send platen gap                                                                                  -*/
/*-----------------------------------------------------------------------------------------------------*/
void SendPlatenHeightOffset() {

	SendPlatenLevelingOffset();

	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->PlatenHeightOffset[theApp.GVars->CurrentHeightOffset], SAVE_POS_AS_PLATENGAP, 0, 0, 0, 0, 0);


}

/*-------------------------------------------------------------------------------*/
/*- Send X Encoder resolution                                                   -*/
/*-------------------------------------------------------------------------------*/
void SendXEncoder(displayGraphicState *gs) {

	switch(gs->ResolutionX) {

		case 150:
			USBPrinterCommand(USB_CMD_SET_X_RESOLUTION, X_RESOLUTION_150);
			break;

		case 300:
			USBPrinterCommand(USB_CMD_SET_X_RESOLUTION, X_RESOLUTION_300);
			break;

		case 1200:
			USBPrinterCommand(USB_CMD_SET_X_RESOLUTION, X_RESOLUTION_1200);
			break;

		case 600:
		default:
			USBPrinterCommand(USB_CMD_SET_X_RESOLUTION, X_RESOLUTION_600);
			break;


	}


}
	

/*-------------------------------------------------------------------------------*/
/*- Send Layer type                                                             -*/
/*-------------------------------------------------------------------------------*/
void SendLayerType(displayGraphicState *gs) {

	TRACE("Layer type: %d\n", gs->layerType);
	USBPrinterCommand(USB_CMD_SET_LAYERTYPE, gs->layerType);

}
	
/*-------------------------------------------------------------------------------*/
/*- Send y resolution                                                           -*/
/*-------------------------------------------------------------------------------*/
void SendYResolution(displayGraphicState *gs) {

	USBPrinterCommand(USB_CMD_SET_Y_RESOLUTION, gs->ResolutionY);

	if(gs->ResolutionY == 600)
		USBPrinterCommand(USB_CMD_SET_STRIPE_1200, 2134);
	else
		USBPrinterCommand(USB_CMD_SET_STRIPE_1200, 1067);



}

/*-------------------------------------------------------------------------------*/
/*- Send bi-directional correction value                                        -*/
/*-------------------------------------------------------------------------------*/
void SendBiDiCor(displayGraphicState *gs) {
float fX, r, o;
int val;

	USBPrinterCommand(USB_SET_BIDI_COR, gs->BiDiCorrection);

}
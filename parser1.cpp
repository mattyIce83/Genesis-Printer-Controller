// Processor1.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg4.h"
#define NO_DIFF_GS
#include "HEAD_structs.h"
#include "parser.h"
#include "Printing.h"
#include "YLinearization.h"

#ifdef USE_MOE_LOGGING
#include "NetworkDLLinterface.h" /// MOE - new module
#endif

// #define SAVE_RAW_PLANES
// #define NO_PRINTER_OUTPUT

extern int CreatePageIconImage(QUEUE_ITEM *, displayGraphicState *, BITMAPHANDLE *, int);
extern void SendSOL(displayGraphicState *);
extern void SendBiDiCor(displayGraphicState *);
extern void SendXEncoder(displayGraphicState *);
extern void SendLayerType(displayGraphicState *);
extern void SendYResolution(displayGraphicState *);
extern void SendPixelRepeat(int);
extern void SendPlatenHeightOffset();
extern void USBPrinterCommand(unsigned long , unsigned long);
extern void SaveBandData(void *);
extern int CheckDuplicate(int, char *, int);

extern BOOL SomeBandsAvailable();

extern int raster_offsets_150[2][4];
extern int raster_offsets_300[2][4];
extern int raster_offsets_600[2][4];
extern int raster_offsets_1200[2][4];

extern int raster_offsets_150_aux[2][4];
extern int raster_offsets_300_aux[2][4];
extern int raster_offsets_600_aux[2][4];
extern int raster_offsets_1200_aux[2][4];


extern	int ThreeCount[4];
extern	int TwoCount[4];
extern	int OneCount[4];

#ifdef DEBUG_PARSER_DATA
	extern void SaveParserData(int, unsigned char *, int, displayGraphicState *);
#endif


#ifdef DEBUG_START_OF_IMAGE
	extern BOOL GFirstNonBlankReported;
#endif

extern int GBandNumber, GLastBand;


//extern void ClearBands();


extern CPrinting *pdlg;

extern CController4Dlg4 *theJobDlg;
extern CController4App theApp;

extern int GBandNumber;
extern int GbandNo;

extern weave_pattern weave[4];

extern int currentWindow;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
HANDLE fhout[8];
int width[8], height[8];
char szCurrentFile[MAX_PATH];

char *szOutFilenames[8] = {	"channel_0.raw",
							"channel_1.raw",
							"channel_2.raw",
							"channel_3.raw",
							"channel_4.raw",
							"channel_5.raw",
							"channel_6.raw",
							"channel_7.raw"};



extern CController4Dlg1 *theDlg;

displayGraphicState GS;

extern void AllocateWeaveBuffers(displayGraphicState *);
extern void FreeWeaveBuffers();
extern void AddRaster(unsigned char *, int, int, int, displayGraphicState *);
extern void AddRasterMW(unsigned char *, int, int, BOOL, displayGraphicState *, BOOL);
extern void PreAddRasterMW(unsigned char *, int, int, BOOL, displayGraphicState *, BOOL);
extern void InitMW();
extern void SetGEOJ(BOOL);

static float x_tn_div;
static float y_tn_div;
static unsigned char *tn_raster;
static int *p_index[MAX_PREVIEW_LENGTH];
static unsigned char ss_pixval[4] = {0x00, 0x55, 0xAA, 0xFF};

BOOL max_Y_received[TOTAL_CHANNELS] = {FALSE, FALSE, FALSE, FALSE};

static int current_raster[TOTAL_CHANNELS] = {0,0,0,0};


/*-----------------------------------------------------------------------------------------------------*/
/*-  subsample a raster. assumes 2 bpp input, 8 bpp output                                            -*/
/*-----------------------------------------------------------------------------------------------------*/
void SubSample(unsigned char *dest, unsigned char *src, int length, float factor) {
int inpix = length * 4;
int outpix = inpix / factor;
int p;
unsigned char *inras = src, *outras = dest;
int i = 0;
unsigned char z, s = 0;


	for(p = 0; p < inpix; p++) {
	
		z = *inras << i;
		z = z >> 6;
		i += 2;
		if(i > 7) {
			i = 0;
			inras++;
		}
		s++;
		if(s == (int)factor - 1) {
			*outras++ = ss_pixval[z];
			s = 0;
		}

	}
	

}

#define PIXEL unsigned char 

static void ScaleLine(PIXEL *Target, PIXEL *Source, int SrcWidth, int TgtWidth) {

  int NumPixels = TgtWidth;
  int IntPart = SrcWidth / TgtWidth;
  int FractPart = SrcWidth % TgtWidth;
  int E = 0;

  while (NumPixels-- > 0) {
    *Target++ = *Source;
    Source += IntPart;
    E += FractPart;
    if (E >= TgtWidth) {
      E -= TgtWidth;
      Source++;
    }
  }
}


/*-----------------------------------------------------------------------------------------------------*/
/*-  Send Y linearization                                                                             -*/
/*-----------------------------------------------------------------------------------------------------*/
static void SendYLinearization(displayGraphicState *gs) {
CYLinearization dlg;
int timeout = 30;
static int iLastLinearized;


	if(iLastLinearized == gs->ResolutionY) return;

	dlg.LoadVarsExt(YL_VARS_FN);
	dlg.ylVars.bExternalCall = TRUE;

	if(!theApp.GVars->UseMultipleCorArrays) {
		dlg.ylVars.iCurTestPattern = 0;
	}
	else {
		if(gs->ResolutionY == 600) {
			dlg.ylVars.iCurTestPattern = 1;
		}
		else {
			dlg.ylVars.iCurTestPattern = 0;
		}
	}

	dlg.SendValues((void *)&dlg);

	theApp.GVars->bYLinearizationSent = 1;

	iLastLinearized = gs->ResolutionY;

}


/*-----------------------------------------------------------------------------------------------------*/
/*-  Send pre-job commands                                                                            -*/
/*-----------------------------------------------------------------------------------------------------*/
static void JobPrePrep(QUEUE_ITEM *qi, displayGraphicState *gs, int channel, unsigned char *data, int length) {

	/**/ TRACE("JobPrePrep begin\n");

#ifdef USE_MOE_LOGGING
	theApp.NetworkDLLinterface.LogJobInfo(EVCD_JOB_START, theApp.printing_qitem, gs); /// MOE - new module
#else
	theApp.LogJobInfo(EVCD_JOB_START, theApp.printing_qitem, (void *)gs);
#endif

	if(length > gs->alloc_raslen)
		gs->alloc_raslen = length;

	if(pdlg != NULL && !pdlg->bHasInfo) {
		qi = theApp.processing_qitem;
		strcpy(pdlg->m_szJobFile, gs->szCurrentFile);
		pdlg->bInfoAvailable = TRUE;
	}

	//SendYLinearization(gs);

	if(gs->layerType == LAYER_TP_DOUBLE_WHITE) 
		gs->layerType |= 0x80;

	theApp.bLeadOutComplete = FALSE;

	SendLayerType(gs);

	SendYResolution(gs);

	SendPlatenHeightOffset();

	if(gs->layerType == LAYER_TP_DUAL_COLOR || gs->layerType == (LAYER_TP_DUAL_COLOR|0x80)) {
		theApp.bDualHead = TRUE;
	}
	else {
		theApp.bDualHead = FALSE;
	}

	SendXEncoder(gs);

	SendSOL(gs);

	theApp.Y_Total = gs->PaperDimensionY * gs->ResolutionY; 

#ifndef NO_PRINTER_OUTPUT

	AllocateWeaveBuffers(gs);
#endif
	SetGEOJ(FALSE);
	GbandNo = 0;
	// reset band counter
	GBandNumber = 0;


	Sleep(250);


	// and send it again... it's been hard of hearing lately
	SendSOL(gs);

	SendXEncoder(gs);

	SendSOL(gs);

	SendYResolution(gs);

	SendLayerType(gs);

	// Send bidirectional correction value

	gs->BiDiCorrection = theApp.GVars->CustBiDiCor;

	SendBiDiCor(gs);

	//theApp.GBufferedFeed = gs->TopMargin;
	//gs->TopMargin = 0;

	if(theApp.GBufferedFeed) {
		int vbands, extra_rasters, ii, cc;
		vbands = theApp.GBufferedFeed / TOTAL_NOZZLES_HW;
		extra_rasters = theApp.GBufferedFeed - (vbands * TOTAL_NOZZLES_HW);
		for(ii = 0; ii < vbands; ii++) {
			theDlg->StatusLine("Pre-advance: %d\r\n", gs->Y_Advance);
			USBPrinterCommand(USB_CMD_FEED, gs->Y_Advance);
			theApp.GSentRasters += (int)gs->ResolutionY / weave[gs->cur_mw].passes;
		}

		memset(data, 0, length);

		for(ii = 0; ii < extra_rasters; ii++) {
			for(cc = 0; cc < TOTAL_CHANNELS; cc++) {
				PreAddRasterMW(data, cc, length, FALSE, gs, FALSE);
			}
		}
		theApp.GVerticalPos += theApp.GBufferedFeed;
		for(cc = 0; cc < TOTAL_CHANNELS; cc++) {
			height[cc] += theApp.GBufferedFeed;
		}

		theApp.GBufferedFeed = 0;
	}


	/// 9/29/16
	//Sleep(3000); /// see if this helps

	// skip top blanks, if RIP uses that feature

	if(gs->TopMargin) {

		int vbands, extra_rasters, ii, cc;
		float extra;

		vbands = gs->TopMargin / TOTAL_NOZZLES_HW;
		extra_rasters = gs->TopMargin - (vbands * TOTAL_NOZZLES_HW);
		extra = (extra_rasters / TOTAL_NOZZLES_HW);
		USBPrinterCommand(USB_CMD_FEED, (gs->Y_Advance * vbands) + (gs->Y_Advance * extra));

	}


#ifdef DEBUG_PARSER_DATA
	SaveParserData(0, (unsigned char *)2, 0, gs);
#endif
	/**/ TRACE("JobPrePrep end\n");



}

/*-----------------------------------------------------------------------------------------------------*/
/*-  Close Job                                                                                        -*/
/*-----------------------------------------------------------------------------------------------------*/
static void CloseJob(QUEUE_ITEM *qi, displayGraphicState *gs, int channel, unsigned char *data, int length) {
int c;
#ifdef DEBUG_BAND_DATA
	SaveBandData(NULL);
#endif

#ifdef DEBUG_PARSER_DATA
	SaveParserData(0, (unsigned char *)1, 0, NULL);
#endif

#ifdef USE_MOE_LOGGING
	theApp.NetworkDLLinterface.LogJobInfo(EVCD_JOB_END, theApp.printing_qitem, gs); /// MOE - new module
#else
	theApp.LogJobInfo(EVCD_JOB_END, theApp.printing_qitem, (void *)gs);
#endif

	// free working buffers
	if(gs->GraphicsInitialized) {
		if(gs->compressedRaster != NULL) {
			free(gs->compressedRaster);
			gs->compressedRaster = NULL;
		}
		for(c = 0; c < 8; c++) {
			if(gs->bits[c] != NULL) {
				free(gs->bits[c]);
				gs->bits[c] = NULL;
			}
		}
	}

#ifdef HEAP_DEBUG
	if(!_CrtCheckMemory()) {
		MessageBox(GetFocus(), "HEAP CORRUPTED(1)", "DEBUG", MB_OK|MB_SYSTEMMODAL);
	}
#endif

	// close plane files
	for(c = 0; c < TOTAL_CHANNELS; c++) {

#ifdef SAVE_RAW_PLANES
		CloseHandle(fhout[c]);
#endif
		current_raster[c] = 0;

		width[c] = 0;
		height[c] = 0;

		int nRet;
		int i;

		//L_FreeBitmap(&theApp.Bitmap[c]);
		for(i = 0; i < MAX_PREVIEW_LENGTH; i++) {
			if(p_index[i] != NULL) {
				free(p_index[i]);
				p_index[i] = NULL;
			}
		}
	}
		
	theApp.qitem = theApp.processing_qitem;

	theDlg->StatusLine("** Finished caching %s\r\n", gs->JobName);


}


/*-----------------------------------------------------------------------------------------------------*/
/*-  Output Function                                                                                  -*/
/*-----------------------------------------------------------------------------------------------------*/
void OutputBits(int channel, unsigned char *data, int length) {
unsigned long bytes;
int c, nRet;
static BOOL InJob;
static int br;

	if(theApp.GMasterStatus == MASTER_STATUS_IMPORTING){
		return;
	}


	/////////////////////////////////////////////////////
	//// Null data means close and end
	/////////////////////////////////////////////////////
	if(data == NULL) { // this means close it

		if(pdlg != NULL)
			CloseJob(pdlg->qi, &GS, channel, data, length);

		InJob = FALSE;
		SetGEOJ(TRUE);
		return;

	}  // close job


	if(theApp.ActualJobRasters + GS.TopMargin > GS.pixelHeight) {
		/**/TRACE("Ignoring Raster %d\n", theApp.ActualJobRasters);
		return;
	}

	/////////////////////////////////////////////////////
	//// First time... open job, set up printer, etc.
	/////////////////////////////////////////////////////
	if(!InJob && theApp.GMasterStatus != MASTER_STATUS_IMPORTING) { /// first time
		// 

		if(pdlg != NULL)
			JobPrePrep(pdlg->qi, &GS, channel, data, length);

	}

	for(c = 0; c < TOTAL_CHANNELS; c++) {
		if(width[c] < length)
			width[c] = length;
	}
	
	height[channel]++;

#ifdef SAVE_RAW_PLANES
	WriteFile(fhout[channel], data, length, &bytes, NULL);
#endif

#ifndef NO_PRINTER_OUTPUT
	// if last raster of last plane, force end.
	BOOL lb = FALSE;

#ifndef NO_LEAD_IN_RASTERS

	if(!InJob) {
		int xx;
		br = weave[GS.cur_mw].nozzles_used * (weave[GS.cur_mw].passes - 1);

		/**/ theDlg->StatusLine("Lead In: %d\n", br);
		/**/ TRACE("Lead In: %d\n", br);
		memset(data, 0, length);
		theApp.iLeadIN = br;
		for(xx = 0; xx < br; xx++) {
			for(c = 0; c < TOTAL_CHANNELS; c++) {
				PreAddRasterMW(data, c, length, lb, &GS, FALSE);
			}
			GS.VerticalPos += ((GS.VerticalPos2 * 256) + GS.VerticalPos1);
		}

	}

#endif

	if(theApp.SupColor[channel]) {
		memset(data, 0, length);
	}

	PreAddRasterMW(data, channel, length, lb, &GS, (current_raster[channel] == theApp.GFirst_non_blank_raster && theApp.GFirst_non_blank_raster > 1));
	current_raster[channel]++;

	if(height[channel] + GS.TopMargin >= GS.pixelHeight) {
		max_Y_received[channel] = TRUE;
	}

	int z, t = TOTAL_CHANNELS;

	for(z = 0; z < TOTAL_CHANNELS; z++) {
		if(max_Y_received[z]) t--;
	}

	if(t < 1) {
		int xx;
		//br = weave[GS.cur_mw].lines * (weave[GS.cur_mw].passes - 1);
		br = weave[GS.cur_mw].nozzles_used * (weave[GS.cur_mw].passes - 1);
		memset(data, 0, length);
		if(GS.ResolutionY == 150) br *= 4;

		/*
		if(GS.pixelHeight < 4200) {
			br += (4200 - GS.pixelHeight);
		}
		*/

#ifndef NO_LEAD_OUT_RASTERS
		theApp.iLeadOUT = br;
		for(xx = 0; xx < br; xx++) {
			GS.VerticalPos += ((GS.VerticalPos2 * 256) + GS.VerticalPos1);
			for(c = 0; c < TOTAL_CHANNELS; c++) {
				PreAddRasterMW(data, c, length, lb, &GS, FALSE);
			}
		}
#endif

		theApp.bLeadOutComplete = TRUE;
		// USBPrinterCommand(USB_CMD_Y_SERVO_SETTING, 0);

	}


	InJob = TRUE;

#ifdef HEAP_DEBUG
	if(!_CrtCheckMemory()) {
		MessageBox(GetFocus(), "HEAP CORRUPTED(1)", "DEBUG", MB_OK|MB_SYSTEMMODAL);
	}
#endif

#endif

}

#include "parser.c"

/*-----------------------------------------------------------------------------------------------------*/
/*-  Find first raster marker                                                                         -*/
/*-----------------------------------------------------------------------------------------------------*/
int GetFirstNonBlankRaster(HANDLE fh) {
int curpos, curposhigh, retval = 0, firstras;
char buff[32];
unsigned long bytes, size,sizeH;

	size = GetFileSize(fh, &sizeH);

	if(SetFilePointer(fh, size - 12, NULL, FILE_BEGIN) != INVALID_SET_FILE_POINTER) {

		if(ReadFile(fh, buff, 8, &bytes, NULL)) {

			buff[8] = 0;

			if(!strcmp(buff, "**@#FRS:")) {
		
				if(ReadFile(fh, (char *)&firstras, 4, &bytes, NULL)) {
					retval = firstras;
				}
		
			}


		}

		if(SetFilePointer(fh, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
			theDlg->StatusLine("Error %d moving to beginning of file in GFNBR\r\n", GetLastError());
		}

	}

	return retval;

}


/*-----------------------------------------------------------------------------------------------------*/
/*-  BELOSE Parser Loop                                                                               -*/
/*-----------------------------------------------------------------------------------------------------*/
void ParserLoop(void *v) {
ESCP2Filter *filter;
unsigned long bytes;
unsigned long totalbytes = 0;
unsigned char readbuff[4096];
char szFileName[MAX_PATH];
HANDLE fh;
int copies, copy, timeout = 0, c;
parserIOStruct *ios = (parserIOStruct *)v;

	while(theApp.ParserCount > 0) {
		Sleep(0);
	}

	theApp.ParserCount++;

	/**/ TRACE("Instantiating Parser (status: %d)\n", theApp.GMasterStatus);

	strcpy(szFileName, ios->szSourceFile);
	copies = ios->copies;
	theApp.printing_qitem = (QUEUE_ITEM *)ios->qitem;
	ios->Done = TRUE;

	if(theApp.GMasterStatus != MASTER_STATUS_IMPORTING) {
		theApp.iPrintedBandCount = 0;
		theApp.bJobWasCancelled = FALSE;
		theApp.bObstruction = FALSE;
		FreeWeaveBuffers();
		Sleep(1000);
		theApp.GMasterStatus =	MASTER_STATUS_BUSY;
		theApp.GVars->bAdvancedFeaturesUnlocked = FALSE; // no more guru shit for you!
		memset((char *)theApp.GVars->TotalInk, 0, sizeof(int) * 8);
	}

	theApp.Y_Percent_Complete = 0;
	theApp.GVerticalPos = 0;
	theApp.GBufferedFeed = 0;
	 theApp.iLeadIN = theApp.iLeadOUT = 0;

	theApp.bDualHead = FALSE;

	if(theApp.GMasterStatus != MASTER_STATUS_IMPORTING) {

		/**/ TRACE("HaltEvent 600\n");
		/**/ theApp.HaltJobScanner = TRUE;

		Sleep(250);
		/*
		if(theApp.hScannerThread->SuspendThread() == 0xFFFFFFFF) {
			MessageBox(GetFocus(), "** COULD NOT suspend scanner thread", APP_NAME, MB_OK|MB_SYSTEMMODAL);
		}
		*/

	}

	if(theApp.GMasterStatus != MASTER_STATUS_IMPORTING) {

		if(!theApp.NoUSB) {

			/**/ TRACE("Setting bPrinterPrepped to zero\n"); 
			theApp.bPrinterPrepped = 0;
			theApp.bYPrintMovementSet = 0;

			/// if auto-print, printer is already prepped
			if(theApp.bIsAutoPrintJob) {
				/**/ TRACE("Setting bPrinterPrepped to one (658)\n"); 
				theApp.bPrinterPrepped = 1;
				theApp.bIsAutoPrintJob = FALSE;
			}
			else {
				USBPrinterCommand(USB_CMD_PREPARE_FOR_JOB, 1);
			}
			timeout = 0;
			
		}
	}


	theApp.ReadMotionVals();
	
	//theDlg->StatusLine("******  Head 1 Offsets (600) ******:\r\nA: %d B: %d C: %d D: %d\r\n******  Head 1 Offsets (1200) ******:\r\nA: %d B: %d C: %d D: %d\r\n*****************************\r\n", raster_offsets_600[0][2], raster_offsets_600[0][1], raster_offsets_600[0][0], raster_offsets_600[0][3],     raster_offsets_1200[0][2], raster_offsets_1200[0][1], raster_offsets_1200[0][0], raster_offsets_1200[0][3]);

	filter = (ESCP2Filter *)ESCOpenParser();
	/**/ TRACE("PARSING: %s\n", szFileName);
	fh = CreateFile(szFileName,
					GENERIC_READ,
					FILE_SHARE_READ,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
  					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);

	if(fh == INVALID_HANDLE_VALUE) {
		theDlg->StatusLine("ERROR %d OPENING FILE\r\n", GetLastError());
		int error = GetLastError();
		theApp.GMasterStatus =	MASTER_STATUS_IDLE;
		theApp.bInJob = FALSE;
		goto endparserloop;
	}
	else
		strcpy(szCurrentFile, szFileName);

#ifdef DEBUG_START_OF_IMAGE
	GFirstNonBlankReported = FALSE;
#endif

#ifdef COUNT_DOT_THREES
	ThreeCount[0] = ThreeCount[1] = ThreeCount[2] = ThreeCount[3] = 0;
	TwoCount[0] = TwoCount[1] = TwoCount[2] = TwoCount[3] = 0;
	OneCount[0] = OneCount[1] = OneCount[2] = OneCount[3] = 0;
#endif

	char IDBuffer[64];
	ReadFile(fh, IDBuffer, 64, &bytes, NULL);
	if(strstr(IDBuffer, "Belquette Genesis Print File V01") == NULL) {
		theDlg->OKBox("Error", "Invalid Print File", NULL);
		CloseHandle(fh);
		goto endparserloop;
	}

	theApp.GFirst_non_blank_raster = GetFirstNonBlankRaster(fh);
	GS.TopMargin =  GS.LeftMargin = 0;


	/**/ theDlg->StatusLine("In-file FNBR: %d\r\n", theApp.GFirst_non_blank_raster);

	if(theApp.GMasterStatus == MASTER_STATUS_IMPORTING) {
		strcpy(GS.szCurrentFile, szFileName);
	}

	theApp.Y_Count = 0;
	if(theApp.GMasterStatus != MASTER_STATUS_IMPORTING && theApp.GMasterStatus != MASTER_STATUS_CANCELLING) {
		/**/ TRACE("Setting status to printing [706]\n");
		theApp.GMasterStatus = MASTER_STATUS_PRINTING;
		theApp.GVars->iJobWait = 0; /// just in case
		theApp.GHeadPosition = 0xFF;
	}

	theApp.HTCounter = 0;

	/// Init the raster tree
	PreAddRasterMW(NULL, 0, 0, 0, 0, 0);
	for(c = 0; c < TOTAL_CHANNELS; c++) {
		width[c] = 0;
		height[c] = 0;
	}

	
	theApp.bBlockTempThread = TRUE;
	theApp.TotalPixelBands = 0;

	while(1) {

		if(theApp.bHardStop || theApp.GMasterStatus == MASTER_STATUS_CANCELLING) {
			//theApp.bHardStop = FALSE;
			//theApp.bInJob = FALSE;
			theApp.GMasterStatus = MASTER_STATUS_CANCELLING;
			break;
		}

		ReadFile(fh, readbuff, 4096, &bytes, NULL);

		totalbytes += bytes;

		if(bytes < 1)
			break;

		ESCParser(filter, readbuff, bytes);

		if(theApp.GMasterStatus == MASTER_STATUS_IMPORTING && GS.GraphicsInitialized) {
			break;
		}

	
	}

	CloseHandle(fh);

	if(theApp.GMasterStatus != MASTER_STATUS_IMPORTING) {

		OutputBits(0, NULL, 0);

		if(pdlg != NULL) {
			// pdlg->UpdatePreviewImage(100.00, (void *)&GS);
			Sleep(0);
			Sleep(200);
		}

	}

	TRACE("Freeing Filter\n");
	free(filter);
	
	if(theApp.GMasterStatus == MASTER_STATUS_CANCELLING || theApp.GMasterStatus == MASTER_STATUS_ABORTING) {
		if(!theApp.bHardStop) {
			USBPrinterCommand(USB_CMD_HOME_PLATEN,0);
			USBPrinterCommand(USB_CMD_PARK,0);
			USBPrinterCommand(USB_CMD_FLUSH_PIXEL_PIPELINES, 0);
		}
		//GLastBand = GBandNumber;
		theApp.bInJob = FALSE;
		theApp.bHardStop = FALSE;
		// wait for printing dialog to close
		/*
		while(theApp.PrintingWindow) {
			Sleep(200);
		}

		int maxwaittime = 0;
		while(!theApp.bLeadOutComplete) {
			Sleep(200);
			maxwaittime++;
			if(maxwaittime > 50) {
				break;
			}
		}

		theApp.ScannerDelay = 2; // wait for things to settle before scanning again
		theApp.GMasterStatus = MASTER_STATUS_IDLE;
		*/


		/**/ TRACE("*** Cancelling, marker 1\n");
	}

endparserloop:
	//theApp.bBlockTempThread = FALSE;
	theApp.bUnlockTempThread = TRUE;


	if(theApp.GMasterStatus == MASTER_STATUS_IMPORTING && theApp.currentWindow == MAIN_RUNNING_WINDOW) {
		if(theApp.q_item_top != NULL) {
			theApp.GVars->CurrentJobNumber = theApp.q_item_top->JobNumber;
		}
		else {
			theApp.GVars->CurrentJobNumber = 0;
		}
		theDlg->ForcePreviewRefresh(FALSE);
	}


	if(	theApp.GMasterStatus == MASTER_STATUS_IMPORTING &&
		theApp.QueueItems == 1 &&
		theApp.GVars->bPrintOnJobArrival) {

		theApp.bPendingAutoPrintRequest = TRUE;

	}

	// TRACE("Setting GMasterStatus to idle (811)\n");
	if(theApp.GMasterStatus == MASTER_STATUS_IMPORTING)
		theApp.GMasterStatus =	MASTER_STATUS_IDLE;

	theApp.ParserCount--;



}


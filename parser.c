#include "tiffpack.c"



// #define DEBUG_COMMAND_STREAM


BOOL EOJ_Set = 0;

RASTERCMDI currentRaster;
UNITSCMD unitscommand;
pixeldimensionscmd pixeldimensions;
pagemargins c_cmd;
spotsizecmd spotsize;
resolutioncmd resolution;
jobnumbercmd jobnumber;
jobnumbercmd previewlen;

BOOL Save[8];

QUEUE_ITEM *current_queue_item;

void PASCAL cmd_PageUnits(char *v, int len) {

	unitscommand.nL = v[3];
	unitscommand.nH = v[4];

	unitscommand.n = (unitscommand.nH * 256) + unitscommand.nL;

}

void PASCAL cmd_Direction(char *v, int len) {

	GS.PrintDirection = v[3];

#ifdef HARD_WIRE_UNIDIRECTIONAL
	GS.PrintDirection = 0;
#endif

#ifdef HARD_WIRE_BIDIRECTIONAL
	GS.PrintDirection = 1;
#endif


}


void PASCAL cmd_PageMargin(char *v, int len) {

	c_cmd.t1 = v[3];
	c_cmd.t2 = v[4];
	c_cmd.t3 = v[5];
	c_cmd.t4 = v[6];

	c_cmd.l1 = v[7];
	c_cmd.l2 = v[8];
	c_cmd.l3 = v[9];
	c_cmd.l4 = v[10];

	c_cmd.t = (c_cmd.t4 * 256 * 256 * 256) + (c_cmd.t3 * 256 * 256) + (c_cmd.t2 * 256) + c_cmd.t1;
	c_cmd.l = (c_cmd.l4 * 256 * 256 * 256) + (c_cmd.l3 * 256 * 256) + (c_cmd.l2 * 256) + c_cmd.l1;

	GS.TopMargin = c_cmd.t;
	GS.LeftMargin = c_cmd.l;
	/**/ TRACE("** MARGINS: top: %d  left: %d\n", GS.TopMargin, GS.LeftMargin);


}

void PASCAL cmd_PixelDimensions(char *v, int len) {

	pixeldimensions.w1 = v[3];
	pixeldimensions.w2 = v[4];
	pixeldimensions.w3 = v[5];
	pixeldimensions.w4 = v[6];

	pixeldimensions.l1 = v[7];
	pixeldimensions.l2 = v[8];
	pixeldimensions.l3 = v[9];
	pixeldimensions.l4 = v[10];

	pixeldimensions.b1 = v[11];
	pixeldimensions.b2 = v[12];
	pixeldimensions.b3 = v[13];
	pixeldimensions.b4 = v[14];


	pixeldimensions.w = (pixeldimensions.w4 * 256 * 256 * 256) + (pixeldimensions.w3 * 256 * 256) + (pixeldimensions.w2 * 256) + pixeldimensions.w1;
	pixeldimensions.l = (pixeldimensions.l4 * 256 * 256 * 256) + (pixeldimensions.l3 * 256 * 256) + (pixeldimensions.l2 * 256) + pixeldimensions.l1;
	pixeldimensions.b = (pixeldimensions.b4 * 256 * 256 * 256) + (pixeldimensions.b3 * 256 * 256) + (pixeldimensions.b2 * 256) + pixeldimensions.b1;

	GS.pixelWidth = pixeldimensions.w;
	GS.pixelHeight = pixeldimensions.l;
	GS.byteWidth = pixeldimensions.b;

	/**/ TRACE(">> Dimensions: %d x %d (%d) Top: %d\n", GS.pixelWidth, GS.pixelHeight, GS.byteWidth, GS.TopMargin);

}


void PASCAL cmd_BitsPerPixel(char *v, int len) {

	GS.bitsPerPixel = (int)v[3];

}


void PASCAL cmd_SpotSize(char *v, int len) {

	spotsize.d = v[3];

	switch(spotsize.d) {
	
		case 16:
			spotsize.index = 0;
			break;
	
		case 17:
			spotsize.index = 1;
			break;

		case 18:
			spotsize.index = 2;
			break;

		case 6:
			spotsize.index = 4;
			break;

		case 5:
			spotsize.index = 5;
			break;
	
	}

}
void PASCAL cmd_Resolution(char *v, int len) {

	resolution.xL = v[3];
	resolution.xH = v[4];

	resolution.yL = v[5];
	resolution.yH = v[6];


	resolution.resX = ((resolution.xH * 256) + resolution.xL);
	resolution.resY = ((resolution.yH * 256) + resolution.yL);

	GS.ResolutionX = (unsigned long)resolution.resX;
	GS.ResolutionY = (unsigned long)resolution.resY;

	GS.PaperDimensionX = (float)pixeldimensions.w / (float)resolution.resX;
	GS.PaperDimensionY = (float)pixeldimensions.l / (float)resolution.resY;

	GS.iPlatenSize.x = GS.fPlatenSize.x * (float)resolution.resX;
	GS.iPlatenSize.y = GS.fPlatenSize.y * (float)resolution.resY;


	if(pdlg != NULL) {
		pdlg->sizeX = GS.PaperDimensionX; 
		pdlg->sizeY = GS.PaperDimensionY; 
		pdlg->bSizeAvailable = TRUE;
	}

}
void PASCAL cmd_HorizontalPos(char *v, int len) {

	GS.HorizontalPosM1 = v[2];
	GS.HorizontalPosM2 = v[3];
	
	GS.HorizontalPos =	(GS.HorizontalPosM2 * 256) + GS.HorizontalPosM1;

	if(GS.HorizontalPos) {
		GS.XOffset =  (GS.HorizontalPos / 8) * currentRaster.b;
	}
	else {
		GS.XOffset = 0;
	}

}


void PASCAL cmd_StartGraphicsMode(char *v, int len) {
int action = (int)v[3], c;
float width_inches;
int widthbytes;

	if(GS.GraphicsInitialized) {

	}

	if(action) {

		if(GS.compressedRaster != NULL) {
			free(GS.compressedRaster);
			GS.compressedRaster = NULL;
		}
		for(c = 0; c < 8; c++) {
			if(GS.bits[c] != NULL) {
				free(GS.bits[c]);
				GS.bits[c] = NULL;
			}
		}

		width_inches = (float)GS.PaperDimensionX;
		widthbytes = (int)((width_inches * resolution.resX * GS.bitsPerPixel) / 8);
		while(widthbytes % 4)
			widthbytes++;
		theApp.ActualJobRasters = 0;


		if(theApp.GMasterStatus != MASTER_STATUS_IMPORTING) {

			GS.compressedRaster = (unsigned char *)calloc(1, widthbytes * 2);

			for(c = 0; c < 8; c++) {
				GS.bits[c] = (unsigned char *)calloc(1, widthbytes * 2);
			}

			GS.alloc_raslen = widthbytes;

		}


		if(!theDlg->bIsAReprint) {
			theApp.QueueCS.Lock();
			current_queue_item = theDlg->CreateNewQueueItem(GS.szCurrentFile);
			current_queue_item->bIsWhite = GS.bIsWhite;
			current_queue_item->iPrintDirection = GS.PrintDirection;
			current_queue_item->iPrintMode = (GS.layerType & 0x80)?1:0;
			current_queue_item->iDualCMYK = (GS.layerType  & 0x0F) == LAYER_TP_DUAL_COLOR;
			current_queue_item->RipJobNumber = GS.RIPJobNumber;
			/**/ TRACE("Job RIP #: %d (%s)\n", GS.RIPJobNumber, current_queue_item->bIsWhite?"White":"Color");
			current_queue_item->iconindex = CreatePageIconImage(current_queue_item, &GS, &theApp.Bitmap[!GS.bIsWhite], 0);
			theApp.QueueCS.Unlock();
			theApp.WriteQueue();

			if(theApp.currentWindow == JOBS_WINDOW) {
				theJobDlg->DisplayQueue();
			}
		}
		if(theApp.GMasterStatus != MASTER_STATUS_IMPORTING) {
			InitMW();
		}


	}

	GS.GraphicsInitialized = action;




}

void PASCAL cmd_VerticalPos(char *v, int len) {
int c;
	GS.VerticalPos1 = v[2];
	GS.VerticalPos2 = v[3];

	c = ((GS.VerticalPos2 * 256) + GS.VerticalPos1);

	if(c != 1) {
		theDlg->OKBox("Invalid", "V move", "value");
		c = 1;
	}

	GS.VerticalPos += c;
	theApp.ActualJobRasters += c;

	if(theApp.ActualJobRasters + GS.TopMargin > GS.pixelHeight) {
		//theDlg->OKBox("Pixel Height", "Raster Count", "Mismatch");
		TRACE("TOO MANY RASTERS... Rasters: %d. PH: %d Top: %d\n", theApp.ActualJobRasters, GS.pixelHeight, GS.TopMargin);
	}

	


	GS.RasterReady = TRUE;
#ifdef SPECIAL_TOP_OF_PAGE
	if(c > 1) {
		theApp.GBufferedFeed += c;
	}
	else {
#endif
		for(c = 0; c < TOTAL_CHANNELS; c++) {
			OutputBits(c, GS.bits[c], GS.RasterWidth[c]);
		}
#ifdef SPECIAL_TOP_OF_PAGE
	}
#endif

}
void PASCAL cmd_RasterData(char *v, int len) {

	currentRaster.r = v[2];		// color
	currentRaster.c = v[3];		// compression
	currentRaster.b = v[4];		// pits per pixel
	currentRaster.mL = v[5];	// rasters low byte
	currentRaster.mH = v[6];	// rasters high byte
	currentRaster.nL = v[7];	// width low byte
	currentRaster.nH = v[8];	// width high byte

	currentRaster.n = (currentRaster.nH * 256) + currentRaster.nL;
	currentRaster.m = (currentRaster.mH * 256) + currentRaster.mL;

	switch(currentRaster.r) {

		case 0: // Cyan
			GS.CurrentChannel = 0;
			break;
		case 1: // Magenta
			GS.CurrentChannel = 1;
			break;

		case 2: // Yellow
			GS.CurrentChannel = 2;
			break;
		case 3: // Black
			GS.CurrentChannel = 3;
			break;
		case 4: // lk
			GS.CurrentChannel = 4;
			break;
		case 5: // lc
			GS.CurrentChannel = 5;
			break;
		case 6: // lm
			GS.CurrentChannel = 6;
			break;
		case 7: // llk
			GS.CurrentChannel = 7;
			break;
	}


	GS.cmprasindex = 0;

}

void PASCAL cmd_JobName(char *v, int len) {
int offset;
char *tptr;

	
	tptr = strstr(&v[3], "Local Job ");

	if(tptr == &v[3]) {
		tptr += strlen("Local Job ");
	}
	else {
		tptr = &v[3];
	}
	
	strncpy(GS.JobName, tptr, 126);

	GS.JobName[124] = 0;


}

void PASCAL cmd_JobComment(char *v, int len) {


	//strncpy(GS.JobComment, &v[3], 126);


}

void PASCAL cmd_EndJob(char *v, int len) {

	EOJ_Set = TRUE;
	SetGEOJ(TRUE);
	theDlg->StatusLine("** EOJ\n");

}

void PASCAL cmd_LangVersion(char *v, int len) {

	GS.Version = v[3];

}

void PASCAL cmd_LayerType(char *v, int len) {

	GS.layerType = v[3];

	if(theApp.GVars->bEnableDuplicateHandling) {
		GS.RIPJobNumber = CheckDuplicate(GS.RIPJobNumber, GS.JobName, GS.layerType);
	}



#ifdef USE_REPEATHANDLE
	if(GS.layerType == LAYER_TP_600X300_RR || GS.layerType == (LAYER_TP_600X300_RR|0x80) && theApp.GVars->bEnable600x300RR) {
		DeleteFile("TMP_DATA_REPEAT.BIN");
		theApp.hRepeatDataHandle = CreateFile(	"TMP_DATA_REPEAT.BIN",
												GENERIC_WRITE,
												FILE_SHARE_READ|FILE_SHARE_WRITE,
												(LPSECURITY_ATTRIBUTES)NULL,
												OPEN_ALWAYS,
												FILE_ATTRIBUTE_NORMAL,
												(HANDLE)NULL);

	}
#endif

}

void PASCAL cmd_JobNumber(char *v, int len) {

	jobnumber.n1 = v[3];
	jobnumber.n2 = v[4];
	jobnumber.n3 = v[5];
	jobnumber.n4 = v[6];


	jobnumber.n = (jobnumber.n4 * 256 * 256 * 256) + (jobnumber.n3 * 256 * 256) + (jobnumber.n2 * 256) + jobnumber.n1;
	
	/*if(theApp.GVars->bEnableDuplicateHandling) {
		GS.RIPJobNumber = CheckDuplicate(jobnumber.n, GS.JobName, GS.layerType);
	}
	else {*/
		GS.RIPJobNumber = jobnumber.n;
	//}
	GS.JobNumber = theApp.GetJobNumber(1);

}

void PASCAL cmd_ColorPreview(char *v, int len) {

	previewlen.n1 = v[3];
	previewlen.n2 = v[4];
	previewlen.n3 = v[5];
	previewlen.n4 = v[6];

	previewlen.n = (previewlen.n4 * 256 * 256 * 256) + (previewlen.n3 * 256 * 256) + (previewlen.n2 * 256) + previewlen.n1;

	GS.PreviewLen = previewlen.n;

	GS.bIsWhite = 0;

}

void PASCAL cmd_WhitePreview(char *v, int len) {

	previewlen.n1 = v[3];
	previewlen.n2 = v[4];
	previewlen.n3 = v[5];
	previewlen.n4 = v[6];

	previewlen.n = (previewlen.n4 * 256 * 256 * 256) + (previewlen.n3 * 256 * 256) + (previewlen.n2 * 256) + previewlen.n1;

	GS.PreviewLen = previewlen.n;

	GS.bIsWhite = 1;

}

BELOSE_CMD esc_commands[TOTAL_COMMANDS] = {

	// Units for page parameters such as margins, etc. (in dpi)
	// ESC @ U X1 X2 Y1 Y2    (X short  Y short)
	CMD_UNITS,				3, 7,  {ESC, '@', 'U'}, (PDEV_HANDLER)cmd_PageUnits,

	// Print Direction
	// ESC @ D x   (for x, 0 =  uni, 1 = bi)
	CMD_PRINT_DIRECTION,	3, 4,  {ESC, '@', 'D'},	(PDEV_HANDLER)cmd_Direction,

	
	// Page margins. Offsets of image
	// ESC @ M TOP1 TOP2 TOP3 TOP4 LEFT1 LEFT2 LEFT3 LEFT4
	CMD_PAGE_MARGINS,		3, 11, {ESC, '@', 'M'}, (PDEV_HANDLER)cmd_PageMargin,

	// Page Dimensions. Actual page size after margins applied
	// ESC @ S X1 X2 X3 X4 Y1 Y2 Y3 Y4
	CMD_PAGE_LENGTH,		3, 15, {ESC, '@', 'S'}, (PDEV_HANDLER)cmd_PixelDimensions,


	// Microweave. TBD
	// ESC @ I x  (x is tbd)
	CMD_PITSPERPIXEL,			3, 4,  {ESC, '@', 'I'}, (PDEV_HANDLER)cmd_BitsPerPixel,


	// Spot Size
	// ESC @ S x (x is size in pl)
	CMD_SPOTSIZE,			3, 4,  {ESC, '@', 'T'}, (PDEV_HANDLER)cmd_SpotSize,


	// Resolution
	// ESC @ R X1 X2 Y1 Y2
	CMD_RESOLUTION,			3, 7,  {ESC, '@', 'R'}, (PDEV_HANDLER)cmd_Resolution,


	// Raster X Start Position
	// ESC $ OF1 OF2 OF3 OF4
	CMD_HORIZONTAL_POS,		2, 6,  {ESC, '$'}, (PDEV_HANDLER)cmd_HorizontalPos,

	// Raster Data
	// ESC r color compress bpp RASTERS1 RASTERS2 WIDTH1 WIDTH2
	CMD_RASTER_DATA,		2, 9,  {ESC, 'r'}, (PDEV_HANDLER)cmd_RasterData,

	// Vertical Pos
	// ESC V X1 X2
	CMD_VERTICAL_POS,		2, 6,  {ESC, 'V'}, (PDEV_HANDLER)cmd_VerticalPos,

	// Start/ End Graphics
	// ESC @ G    1 = start graphics 2 = end graphics
	CMD_START_GRAPHICS,		3, 4,  {ESC, '@', 'G'}, (PDEV_HANDLER)cmd_StartGraphicsMode,

	// Job Name
	// ESC @ N    32 characters total (3 command and 29 for name)
	CMD_JOB_NAME,		3, 128,  {ESC, '@', 'N'}, (PDEV_HANDLER)cmd_JobName,

	// Job Comment
	// ESC @ C    32 characters total (3 command and 29 for name)
	CMD_JOB_COMMENT,		3, 32,  {ESC, '@', 'C'}, (PDEV_HANDLER)cmd_JobComment,

	// End of job
	// ESC @ E 0
	CMD_EOJ,			3, 4,  {ESC, '@', 'E', '0'}, (PDEV_HANDLER)cmd_EndJob,

	// Language Version
	// ESC @ X 0
	CMD_VER,			3, 4,  {ESC, '@', 'X', '0'}, (PDEV_HANDLER)cmd_LangVersion,

	// Job Number
	// ESC @ B X1 X2 X3 X4
	CMD_JOBNUMBER,		3, 7,  {ESC, '@', 'B'}, (PDEV_HANDLER)cmd_JobNumber,

	// Layer Type
	// ESC @ Y 0
	CMD_LAYERTYPE,		3, 4,  {ESC, '@', 'Y', '0'}, (PDEV_HANDLER)cmd_LayerType,

	// Color preview
	// ESC @ P 
	CMD_BEGINCOLORPREVIEW,	3, 7,  {ESC, '@', 'P'}, (PDEV_HANDLER)cmd_ColorPreview,

	// White preview
	// ESC @ W
	CMD_BEGINWHITEPREVIEW,	3, 7,  {ESC, '@', 'W'}, (PDEV_HANDLER)cmd_WhitePreview,

};


/*-----------------------------------------------------------------------------------------------------*/
/*-  ESC_P2 Parser                                                                                    -*/
/*-----------------------------------------------------------------------------------------------------*/
void *ESCOpenParser() {

ESCP2Filter *filter;

	filter = (ESCP2Filter *)calloc(1, sizeof(ESCP2Filter));

	filter->firstBuff = TRUE;

	int i;
	for(i = 0; i < TOTAL_CHANNELS; i++) {
		max_Y_received[i] = FALSE;
	}

	if(filter)
		filter->state = searching;

	return (void *)filter;


}

/*-----------------------------------------------------------------------------------------------------*/
/*-  Match command character                                                                          -*/
/*-----------------------------------------------------------------------------------------------------*/
BOOL MatchCMDchar(ESCP2Filter *esc_filter, char ch) {
int i, ii;


	for(i = 0; i < TOTAL_COMMANDS; i++) {
		for(ii = 0; ii <  esc_filter->ESCBindex + 1; ii++) {
			if(ii == esc_filter->ESCBindex) {
				if(ch == esc_commands[i].command[ii] ) {
					esc_filter->last = esc_commands[i].ID;
					return TRUE;
				}
				else {
					break;
				}
			}
			else {
				if(esc_commands[i].command[ii] != esc_filter->ESCBuffer[ii]) {
					break;
				}
			}
		}
	}
	
	return FALSE;

}


/*-----------------------------------------------------------------------------------------------------*/
/*-  Count ink volume                                                                                    -*/
/*-----------------------------------------------------------------------------------------------------*/
static void CountInk(char *data, int length, int Channel, int layertype) {
int i, inkindex;
char *ptr = data;
int jobtype = layertype & 0x7F, inkmultiplier = 1;

	if(layertype & 0x80) inkmultiplier++;

	switch(jobtype) {

		case LAYER_TP_WHITE:
		case LAYER_TP_DOUBLE_WHITE:
			inkindex = 4;
			break;

		case LAYER_TP_DUAL_COLOR:
				inkmultiplier++;
		case LAYER_TP_SINGLE_COLOR:
		case LAYER_TP_600X300_RR:
		default:
			inkindex = Channel;
			break;

	}

	for(i = 0; i < length; i++) {
		
		if(*ptr & 0x03) theApp.GVars->TotalInk[inkindex] += inkmultiplier;
		if(*ptr & 0x0C) theApp.GVars->TotalInk[inkindex] += inkmultiplier;
		if(*ptr & 0x03) theApp.GVars->TotalInk[inkindex] += inkmultiplier;
		if(*ptr & 0x30) theApp.GVars->TotalInk[inkindex] += inkmultiplier;
		if(*ptr & 0xC0) theApp.GVars->TotalInk[inkindex] += inkmultiplier;
		ptr++;

	}



}

/*-----------------------------------------------------------------------------------------------------*/
/*-  ESC_P2 Parser                                                                                    -*/
/*-----------------------------------------------------------------------------------------------------*/
BOOL ESCParser(ESCP2Filter *esc_filter, unsigned char *buffer, int length) {
int i, c, nRet;
static int totalRasters[8];

	if(esc_filter->firstBuff) {
		for(i = 0; i < 8; i++) {
			totalRasters[i] = 0;
			GS.bits[i] = NULL;
		}

		char tmp_fn[MAX_PATH];
		strncpy(tmp_fn, GS.szCurrentFile, MAX_PATH);
		memset((char *)&GS, 0, sizeof(GS));
		if(strlen(tmp_fn)) {
			strcpy(GS.szCurrentFile, tmp_fn);
		}

		//GS.PrintDirection = theDlg->m_BiDirectional.GetCheck();

#ifdef HARD_WIRE_UNIDIRECTIONAL
		GS.PrintDirection = 0;
#endif
		esc_filter->firstBuff = FALSE;

	}

	if(esc_filter == NULL)
		return FALSE;

	esc_filter->index = 0;

	for(c = 0; c < length; c++) {

		switch(esc_filter->state) {

			case searching:
				if(MatchCMDchar(esc_filter, buffer[c])) {
					esc_filter->ESCBuffer[esc_filter->ESCBindex++] = buffer[c];
					esc_filter->state = lookupCMD;
				}
				break;

			case lookupCMD:
				if(MatchCMDchar(esc_filter, buffer[c])) {
					esc_filter->ESCBuffer[esc_filter->ESCBindex++] = buffer[c];
					if(esc_filter->ESCBindex == esc_commands[esc_filter->last].length) {
						// found command
						esc_filter->state = inCMD;
						esc_filter->cmdLength = esc_commands[esc_filter->last].cmd_length;
						esc_filter->cmdFunc = esc_commands[esc_filter->last].handler;
						memcpy(esc_filter->ESCcmdBuffer, esc_filter->ESCBuffer, esc_filter->ESCBindex);
					}
				}
				else {
					esc_filter->ESCBindex = 0;
					esc_filter->state = searching;
					memset(esc_filter->ESCBuffer, 0, 10);
				}
				break;

			case inCMD:
		
				esc_filter->ESCcmdBuffer[esc_filter->ESCBindex++] = buffer[c];
				
				if(esc_filter->ESCBindex == esc_filter->cmdLength) {

					//////////// HANDLE COMMAND HERE /////////////
					(esc_filter->cmdFunc)(esc_filter->ESCcmdBuffer, esc_filter->ESCBindex + 1);

					if(theApp.GMasterStatus == MASTER_STATUS_IMPORTING && GS.GraphicsInitialized) {
						return TRUE;
					}

#ifdef DEBUG_COMMAND_STREAM
						TRACE("CMD: %d\n", esc_filter->last);
#endif


					esc_filter->ESCBindex = 0;
					
					if(esc_filter->last == CMD_RASTER_DATA) {
						esc_filter->state = inRasterDataSearching;
						esc_filter->rasterCount = 0;
						esc_filter->rasterCompCount = 0;
					}
					else if(esc_filter->last == CMD_BEGINWHITEPREVIEW || esc_filter->last == CMD_BEGINCOLORPREVIEW) {
						esc_filter->state = inPreviewCapture;
						esc_filter->rasterCount = 0; // <=== used to count bytes, not rasters in this mode
						if(theApp.PreviewBuffer != NULL) {
							free(theApp.PreviewBuffer);
						}
						theApp.PreviewBuffer = (unsigned char *)calloc(1, GS.PreviewLen + 100);
						theApp.PreviewBufferIndex = theApp.PreviewBuffer;
					}
					else {
						esc_filter->state = searching;
					}
					
					memset(esc_filter->ESCBuffer, 0, 10);
					memset(esc_filter->ESCcmdBuffer, 0, 10);

				}
				break;

			case inPreviewCapture:

					*theApp.PreviewBufferIndex++ = buffer[c];

					esc_filter->rasterCount++;

					if(esc_filter->rasterCount == GS.PreviewLen) {

						if(theApp.Bitmap[!GS.bIsWhite].Flags.Allocated) {
							L_FreeBitmap(&theApp.Bitmap[!GS.bIsWhite]);
						}
						nRet = L_LoadBitmapMemory(theApp.PreviewBuffer, &theApp.Bitmap[!GS.bIsWhite], sizeof(BITMAPHANDLE), 24, ORDER_BGR, GS.PreviewLen, NULL, NULL);
						GS.fPlatenSize.x = (float)theApp.Bitmap[!GS.bIsWhite].Width / (float)theApp.Bitmap[!GS.bIsWhite].XResolution;
						GS.fPlatenSize.y = (float)theApp.Bitmap[!GS.bIsWhite].Height / (float)theApp.Bitmap[!GS.bIsWhite].YResolution;
						esc_filter->state = searching;

					}

					break;

			case inRasterDataSearching:
				if(currentRaster.c) {
					if(buffer[c] & 0x80) {
						esc_filter->state = inRasterDataInRun;
						buffer[c] -= 0x80;
						esc_filter->rasterCount += (129 - buffer[c]);
						buffer[c] += 0x80;
					}
					else {
						esc_filter->state = inRasterDataInLiteral;
						esc_filter->rasterCompCount = buffer[c] + 1; 
					}
				}
				else {
					esc_filter->state = inRasterDataInLiteral;
					esc_filter->rasterCompCount = currentRaster.n;
					esc_filter->rasterCount = 0;  
				}
				GS.compressedRaster[GS.cmprasindex++] = buffer[c];
				break;

			case inRasterDataInRun:

				GS.compressedRaster[GS.cmprasindex++] = buffer[c];

				if(esc_filter->rasterCount >= (int)currentRaster.n) {
					
					////// process raster here
					int rlength = GS.cmprasindex;

					GS.cmprasindex = 0;

					memset(GS.bits[GS.CurrentChannel],0, GS.alloc_raslen);

					if(currentRaster.c) {
						TiffUnPack((unsigned char *)GS.compressedRaster, (unsigned char *)GS.bits[GS.CurrentChannel] + GS.XOffset, &rlength);
					}
					else {
						memcpy((char *)GS.bits[GS.CurrentChannel] + GS.XOffset, (char *)GS.compressedRaster, rlength);
					}

#ifdef COUNT_INK
					CountInk((char *)GS.bits[GS.CurrentChannel] + GS.XOffset, rlength, GS.CurrentChannel, GS.layerType);
#endif

					GS.RasterWidth[GS.CurrentChannel] = rlength;

					totalRasters[GS.CurrentChannel]++;

					esc_filter->state = searching;
					esc_filter->rasterCount = 0;
					esc_filter->rasterCompCount = 0;
				}
				else {
					esc_filter->state = inRasterDataSearching;
				}
				break;

			case inRasterDataInLiteral:

				GS.compressedRaster[GS.cmprasindex++] = buffer[c];

				esc_filter->rasterCompCount--;
				esc_filter->rasterCount++;
				if(esc_filter->rasterCompCount == 0) {
					if(esc_filter->rasterCount >= (int)currentRaster.n) {

						////// process raster here
						int rlength = GS.cmprasindex;

						GS.cmprasindex = 0;

						memset(GS.bits[GS.CurrentChannel],0, GS.alloc_raslen);

						if(currentRaster.c) {
							TiffUnPack((unsigned char *)GS.compressedRaster, (unsigned char *)GS.bits[GS.CurrentChannel] + GS.XOffset, &rlength);
						}
						else {
							memcpy((char *)GS.bits[GS.CurrentChannel] + GS.XOffset, (char *)GS.compressedRaster, rlength);
						}

						GS.RasterWidth[GS.CurrentChannel] = rlength;


						totalRasters[GS.CurrentChannel]++;
						esc_filter->state = searching;
						esc_filter->rasterCount = 0;
						esc_filter->rasterCompCount = 0;
					}
					else {
						esc_filter->state = inRasterDataSearching;
					}
				}
				break;
		}
		esc_filter->index++;
	}

	return TRUE;

}

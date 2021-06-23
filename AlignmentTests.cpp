//  Alignment test utilties

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "parser.h"
//#define DEFINE_RASTER_OFFSET_ARRAYS
#include "HEAD_structs.h"
#include "TestBands.h"
#include "ftd2xx.h"
#include "ftdisupp.h"

#define DP_CYAN_PLANE		0
#define DP_MAGENTA_PLANE	1
#define DP_YELLOW_PLANE		2
#define DP_BLACK_PLANE		3

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CController4Dlg1 *theDlg;

#define DP_BLACK_PLANE			3
#define DP_CYAN_PLANE			0
#define DP_MAGENTA_PLANE		1
#define DP_YELLOW_PLANE			2
#define DP_WHITE_PLANE			4
#define DP_COMPOSITE_PLANE		5

static 	COLORREF GrayPal[256];

static BITMAPHANDLE			BitmapC;
static BITMAPHANDLE			BitmapM;
static BITMAPHANDLE			BitmapY;
static BITMAPHANDLE			BitmapK;
static BITMAPHANDLE			BitmapW;
static BITMAPHANDLE			BitmapCmps;



static BITMAPHANDLE			*CMYKArray[6] = {&BitmapC, &BitmapM, &BitmapY, &BitmapK, &BitmapW, &BitmapCmps};

static BITMAPHANDLE working_bmp[TOTAL_CHANNELS];
static BITMAPHANDLE snd_bmp[TOTAL_CHANNELS];

static int raster_offsets[2][4];

static char *ColorNames[6] = {"Cyan Plane", "Magenta Plane", "Yellow Plane", "Black Plane", "White Plane", "Composite"};

static unsigned char FlipMap[256];
static BOOL FlipMapInited = FALSE;
/*
static unsigned char color_mask_bit1[TOTAL_CHANNELS] = {0x80, 0x20, 0x08, 0x02};
static unsigned char color_mask_bit2[TOTAL_CHANNELS] = {0x40, 0x10, 0x04, 0x01};
*/
static unsigned char color_mask_bit1[TOTAL_CHANNELS] = {0x08, 0x80, 0x20, 0x02};
static unsigned char color_mask_bit2[TOTAL_CHANNELS] = {0x04, 0x40, 0x10, 0x01};



void SendTestBand(testbandVars *, int);

extern void USBPrinterCommand(unsigned long , unsigned long);
extern void InitHead(PRINT_HEAD *);
extern void FlipDWORD(unsigned long *);
extern BOOL FTDWrite(FTDIComm *, unsigned char *, unsigned long);
extern FTDIComm FPGAusb;

extern int raster_offsets_150[2][4];
extern int raster_offsets_300[2][4];
extern int raster_offsets_600[2][4];
extern int raster_offsets_1200[2][4];

extern void SendPlatenHeightOffset();

extern BOOL CopyFlags[4];


void MirrorBand(INBAND *);
static int YReses[4] = {150, 300, 600, 1200};
/*-------------------------------------------------------------------------------*/
/*-  Init the flip table for mirroring 2-bit rasters	                            -*/
/*-------------------------------------------------------------------------------*/
static void InitFlipMap() {
unsigned int i;

	memset(FlipMap, 0, 256);
	
	for(i = 0; i < 256; i++) {

		// flip the 2-bpp pixels, not the byte
		if(i & 0x01) FlipMap[i] |= 0x40;
		if(i & 0x02) FlipMap[i] |= 0x80;
		if(i & 0x04) FlipMap[i] |= 0x10;
		if(i & 0x08) FlipMap[i] |= 0x20;
		if(i & 0x10) FlipMap[i] |= 0x04;
		if(i & 0x20) FlipMap[i] |= 0x08;
		if(i & 0x40) FlipMap[i] |= 0x01;
		if(i & 0x80) FlipMap[i] |= 0x02;

	}
	FlipMapInited = TRUE;
}


#define MCPS_BUFFSIZE		23000
#define Y_NUMBER_OFFSET		4
/*-------------------------------------------------------------------------------*/
/*-                                                                             -*/
/*-------------------------------------------------------------------------------*/
void AddNumberToBand(int plane, int number) {
HANDLE fh, cfh;
char tmp_buff[100], szNumber[10], cDigits[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}, szError[128];
unsigned char *outbuff, *inbuff, *digitbuff, *tmp_ras, *slide, *slide2;
unsigned long bytes;
HRSRC rsrc;
HGLOBAL ps;
int numberrsc[10] = {IDR_NUMBER_0,IDR_NUMBER_1,IDR_NUMBER_2,IDR_NUMBER_3,IDR_NUMBER_4,IDR_NUMBER_5,IDR_NUMBER_6,IDR_NUMBER_7,IDR_NUMBER_8,IDR_NUMBER_9}, nlist[3];
int offsets[3] = /*{13, 47, 81},*/ {81, 47, 13}, i, digits, ii, size, nRet, p;
BITMAPHANDLE bhDigit;

	sprintf(szNumber, "%03d", number + 1);
	digits = strlen(szNumber);

	outbuff = (unsigned char *)calloc(1, MCPS_BUFFSIZE);

	for(i = 0; i < digits; i++) {
		for(ii = 0; ii < 10; ii++) {
			if(szNumber[i] == cDigits[ii]) {
				nlist[i] = ii;
				break;
			}
		}
	}


	for(i = 0; i < digits; i++) {

		rsrc = FindResource(AfxGetInstanceHandle(), (LPCTSTR)numberrsc[nlist[i]], "TIFF_IMAGE"); 

		ps = LoadResource(AfxGetInstanceHandle(), rsrc);

		if(ps == NULL) {
			::MessageBox(::GetFocus(), "Error loading CALIB PS resource", PRODUCT_NAME, MB_OK|MB_SYSTEMMODAL);
			free(outbuff);
			return;
		}

		size = SizeofResource(AfxGetInstanceHandle( ), rsrc);

		memcpy(outbuff, ps, size);

		FreeResource(ps);

		nRet = L_LoadBitmapMemory(outbuff, &bhDigit, sizeof(BITMAPHANDLE), 24, ORDER_BGR, size, NULL, NULL);

		if(nRet != SUCCESS) {
			sprintf(szError, "Error: %d", nRet);
			theDlg->OKBox("LoadBitmapMeory", szError, NULL);
		}

		nRet = L_ReverseBitmap(&bhDigit);

		if(nRet != SUCCESS) {
			sprintf(szError, "Error: %d", nRet);
			theDlg->OKBox("ReverseBitmap", szError, NULL);
		}


		L_INT RedFact , GreenFact , BlueFact;
		RedFact    = 300;
		GreenFact = 590; 
		BlueFact   = 110;


		nRet = L_GrayScaleBitmapExt(&bhDigit, RedFact, GreenFact, BlueFact);

		if(nRet != SUCCESS) {
			sprintf(szError, "Error: %d", nRet);
			theDlg->OKBox("GrayScaleBitmapExt", szError, NULL);
		}


		// nRet = L_SaveBitmap("digit.tif", &bhDigit, FILE_TIF, 8, 0, NULL);

		inbuff = (unsigned char *)calloc(1,CMYKArray[plane]->BytesPerLine);
		digitbuff = (unsigned char *)calloc(1, bhDigit.BytesPerLine);
		tmp_ras = (unsigned char *)calloc(1, bhDigit.BytesPerLine);

		for(ii = 0; ii < bhDigit.Height; ii++) {

			slide = digitbuff;
			slide2 = tmp_ras;

			nRet = L_GetBitmapRow(&bhDigit, digitbuff, ii, bhDigit.BytesPerLine);

			if(nRet != bhDigit.BytesPerLine) {
				sprintf(szError, "Error: %d", nRet);
				theDlg->OKBox("GetBitmapRow (1)", szError, NULL);
			}


			nRet = L_GetBitmapRow(CMYKArray[plane], inbuff, ii + Y_NUMBER_OFFSET, CMYKArray[plane]->BytesPerLine);

			if(nRet != CMYKArray[plane]->BytesPerLine) {
				sprintf(szError, "Error: %d", nRet);
				theDlg->OKBox("GetBitmapRow (2)", szError, NULL);
			}


			for(p = 0; p < bhDigit.Width; p++) {
				*slide2 = 0xFF - (slide[0] | slide[1] | slide[2]);
				if(*slide2 > 0) *slide2 = 0xFF;
				slide += 3;
				slide2++;
			}

			memcpy(inbuff + offsets[i], tmp_ras, bhDigit.Width);

			nRet = L_PutBitmapRow(CMYKArray[plane], inbuff, ii + Y_NUMBER_OFFSET, bhDigit.Width + offsets[i]);

			if(nRet != bhDigit.Width + offsets[i]) {
				sprintf(szError, "Error: %d", nRet);
				theDlg->OKBox("PutBitmapRow (2)", szError, NULL);
			}

		}
		free(digitbuff);
		free(inbuff);
		free(tmp_ras);

		// DeleteFile("numbered.tif");
		// nRet = L_SaveBitmap("numbered.tif", CMYKArray[plane], FILE_TIF, 8, 0, NULL);


	}
	L_FreeBitmap(&bhDigit);
	free(outbuff);

}

/*-------------------------------------------------------------------------------*/
/*-                                                                             -*/
/*-------------------------------------------------------------------------------*/
BOOL ConvertPlane(int bmIndex, BOOL noScreen) {
int r, nRet;
BITMAPHANDLE *bm;
RGBQUAD FixedPalette[256];
RGBQUAD TwoBitPalette[256];
char szError[128];

	bm = CMYKArray[bmIndex];

	theDlg->StatusLine("ConvertPlane invoked\n");
	
	theDlg->StatusLine("Converting to 2bpp & screening\n");

	nRet = L_GetFixedPalette(FixedPalette, 2);

	if(nRet != SUCCESS) {
		sprintf(szError, "Error: %d", nRet);
		theDlg->OKBox("GetFixedPalette", szError, NULL);
	}



	// tweak the palette

	TwoBitPalette[0].rgbRed			= FixedPalette[3].rgbRed;
	TwoBitPalette[0].rgbGreen		= FixedPalette[3].rgbGreen;
	TwoBitPalette[0].rgbBlue		= FixedPalette[3].rgbBlue;
	TwoBitPalette[0].rgbReserved	= FixedPalette[3].rgbReserved;

	TwoBitPalette[1].rgbRed			= FixedPalette[2].rgbRed;
	TwoBitPalette[1].rgbGreen		= FixedPalette[2].rgbGreen;
	TwoBitPalette[1].rgbBlue		= FixedPalette[2].rgbBlue;
	TwoBitPalette[1].rgbReserved	= FixedPalette[2].rgbReserved;

	TwoBitPalette[2].rgbRed			= FixedPalette[1].rgbRed;
	TwoBitPalette[2].rgbGreen		= FixedPalette[1].rgbGreen;
	TwoBitPalette[2].rgbBlue		= FixedPalette[1].rgbBlue;
	TwoBitPalette[2].rgbReserved	= FixedPalette[1].rgbReserved;


	TwoBitPalette[3].rgbRed			= FixedPalette[0].rgbRed;
	TwoBitPalette[3].rgbGreen		= FixedPalette[0].rgbGreen;
	TwoBitPalette[3].rgbBlue		= FixedPalette[0].rgbBlue;
	TwoBitPalette[3].rgbReserved	= FixedPalette[0].rgbReserved;

	if(snd_bmp[bmIndex].Flags.Allocated)
		L_FreeBitmap(&snd_bmp[bmIndex]);

	nRet = L_CreateBitmap(&snd_bmp[bmIndex],
							sizeof(BITMAPHANDLE),
							TYPE_CONV,
							bm->Width,
							bm->Height,
							2,
							ORDER_GRAY, // color order not used
							NULL,
							TOP_LEFT,
							NULL,
							0);

   	if(nRet != SUCCESS) {
		sprintf(szError, "Error: %d", nRet);
		theDlg->OKBox("CreateBitmap", szError, NULL);
	}

	if(noScreen)
		bm->DitheringMethod = CRF_NODITHERING;

	else
		bm->DitheringMethod = FLOYD_STEIN_DITHERING;

	unsigned char *inbuff, *outbuff;

	inbuff = (unsigned char *)calloc(1, bm->BytesPerLine);
	outbuff = (unsigned char *)calloc(1, bm->BytesPerLine);

	nRet = L_StartDithering(bm, TwoBitPalette, 4);

   	if(nRet != SUCCESS) {
		sprintf(szError, "Error: %d", nRet);
		theDlg->OKBox("StartDithering", szError, NULL);
	}


	if(nRet != SUCCESS) {
		theDlg->StatusLine("StartDithering returned: %d\n", nRet);
		return FALSE;
	}

	L_AccessBitmap(bm);

	L_AccessBitmap(&snd_bmp[bmIndex]);

	for(r = 0; r < bm->Height; r++) {

		nRet = L_GetBitmapRow(bm, inbuff, r, bm->BytesPerLine);

   		if(nRet != bm->BytesPerLine) {
			sprintf(szError, "Error: %d", nRet);
			theDlg->OKBox("GetBitmapRow (3)", szError, NULL);
		}


		if(nRet < 1) {
			theDlg->StatusLine("L_GetBitmapRow returned: %d\n", nRet);
			return FALSE;
		}

		nRet = L_DitherLine(bm, inbuff, outbuff);

   		if(nRet != SUCCESS) {
			sprintf(szError, "Error: %d", nRet);
			theDlg->OKBox("DitherLine (3)", szError, NULL);
		}


		if(nRet != SUCCESS) {
			theDlg->StatusLine("L_DitherLine returned: %d\n", nRet);
			return FALSE;
		}

		nRet = L_PutBitmapRow(&snd_bmp[bmIndex], outbuff, r, snd_bmp[bmIndex].BytesPerLine);

   		if(nRet != snd_bmp[bmIndex].BytesPerLine) {
			sprintf(szError, "Error: %d", nRet);
			theDlg->OKBox("PutBitmapRow (2)", szError, NULL);
		}


		if(nRet < 1) {
			theDlg->StatusLine("L_PutBitmapRow returned: %d\n", nRet);
			return FALSE;
		}

	}
	

	theDlg->StatusLine("Saving\n");

	char szSaveName[MAX_PATH];

	sprintf(szSaveName, "CONVERTED%d.TIF", bmIndex);

	DeleteFile(szSaveName);

	nRet = L_SaveBitmap(szSaveName, &snd_bmp[bmIndex], FILE_TIF, 2, 0, NULL); 

   	if(nRet != SUCCESS) {
		sprintf(szError, "Error: %d", nRet);
		theDlg->OKBox("SaveBitmap", szError, NULL);
	}


	char tmpmsg[128];

	//sprintf(tmpmsg, "Created: %s", szSaveName);

	// MessageBox(GetFocus(), tmpmsg, "GENAPP", MB_OK|MB_SYSTEMMODAL);

	L_ReleaseBitmap(&snd_bmp[bmIndex]);
	L_ReleaseBitmap(bm);
	
	nRet = L_StopDithering(bm);

	if(nRet != SUCCESS) {
		sprintf(szError, "Error: %d", nRet);
		theDlg->OKBox("StopDithering", szError, NULL);
	}

	if(nRet != SUCCESS) {
		theDlg->StatusLine("L_StopDithering returned: %d\n", nRet);
		return FALSE;
	}

	theDlg->StatusLine("Done\n");

	free(inbuff);
	free(outbuff);

	return TRUE;

}


/*-------------------------------------------------------------------------------------*/
/*-                                                                                   -*/
/*-------------------------------------------------------------------------------------*/
static void SetupOFN(OPENFILENAME *ofn) {
char FileName[MAX_PATH];
char szFilter[MAX_PATH];
char szTitle[MAX_PATH];
CString Filter, Title;
int i, fl;
char szCurrentDir[MAX_PATH];

	memset((char *)ofn, 0, sizeof(OPENFILENAME));

	GetCurrentDirectory(MAX_PATH, szCurrentDir);

	Filter.LoadString(IDS_IMPORTFILTER_2);
	Title.LoadString(IDS_DEST_IMPORT_FOLDER);
	strcpy(szTitle, (LPCTSTR)Title);

	memset(ofn, 0, sizeof(ofn));
	ofn->lStructSize = sizeof(OPENFILENAME);
	ofn->hwndOwner = AfxGetMainWnd()->m_hWnd;
	ofn->hInstance = NULL;

	/** construct filter ( |'s become \0's) */
	strcpy(szFilter,(LPCTSTR)Filter);
	fl = (int)strlen(szFilter);
	for(i = 0; i < fl; i++) {
		if(szFilter[i] == '|') {
			szFilter[i] = 0;
		}
	}
	/**/

	ofn->lpstrFilter = szFilter;
	ofn->lpstrCustomFilter = NULL;
	ofn->nMaxCustFilter = 0;
	ofn->nFilterIndex = 1;

	sprintf(FileName, "%s\\output", szCurrentDir);

	ofn->lpstrFile = FileName;
	ofn->nMaxFile = 500;
	ofn->lpstrFileTitle = szTitle;
	ofn->nMaxFileTitle = 500;
	ofn->Flags = OFN_FILEMUSTEXIST|OFN_ENABLESIZING|OFN_HIDEREADONLY;
	ofn->lpstrDefExt = "*.TIF";
	ofn->lCustData = NULL;
	ofn->lpfnHook = NULL;
	ofn->lpTemplateName = NULL;


}

/*-------------------------------------------------------------------------------------*/
/*-                                                                                   -*/
/*-------------------------------------------------------------------------------------*/
BOOL SelectTestBandFile(char *szFile) {
OPENFILENAME ofn;

	SetupOFN(&ofn);

	if(GetOpenFileName(&ofn)) {
		strcpy(szFile, ofn.lpstrFile);
		return TRUE;
	}

	szFile = NULL;
	return FALSE;

}


/*-------------------------------------------------------------------------------------*/
/*-                                                                                   -*/
/*-------------------------------------------------------------------------------------*/
BOOL OnFileOpenTestBand(char *szInFile, int inkorder) {
CString Filter, Title;
int i;
OPENFILENAME ofn;
BOOL bInRetry = FALSE;
char szError[128];
int channelorder[2][4] = {0,1,2,3,  0,3,2,1};						   

	for(i = 0; i < 256; i++) {
		GrayPal[i] = RGB(i,i,i);
	}


	if(szInFile != NULL) {
		ofn.lpstrFile = szInFile;
	}
	
	else {

		SetupOFN(&ofn);

	}


	if(szInFile != NULL || GetOpenFileName(&ofn)) {


		for(i = 0; i < 6; i++) {
			if(CMYKArray[i]->Flags.Allocated)
				L_FreeBitmap(CMYKArray[i]);
		}


		theDlg->StatusLine("File: %s\n", ofn.lpstrFile);

		int nRet;

		nRet = L_LoadFileCMYKArray (ofn.lpstrFile, CMYKArray, 4, sizeof(BITMAPHANDLE), 8, LOADFILE_ALLOCATE|LOADFILE_STORE, NULL, NULL, NULL, NULL);

retryLoad:

		if(nRet == SUCCESS) {

			theDlg->StatusLine("Graphic Loaded\n");

			if(!bInRetry) {
				nRet = L_LoadBitmap (ofn.lpstrFile, CMYKArray[DP_COMPOSITE_PLANE], sizeof(BITMAPHANDLE), 24, ORDER_BGR, NULL, NULL);
			}

			int i;
			for(i = 0; i < 4; i++) {
				L_InvertBitmap(CMYKArray[i]);
			}



		}
		else {

			if(!bInRetry) {

				bInRetry = TRUE;

				if(nRet == -918) {

					theDlg->StatusLine("Graphic is not CMYK...\n", nRet);

					if(0) {
						MessageBox(GetFocus(), "FILE MUST BE CMYK COLOR SPACE", "GENESIS TEST PLATFORM 1", MB_OK|MB_SYSTEMMODAL);
					}
					else {

						theDlg->StatusLine("Converting to CMYK...\n");

						nRet = L_LoadBitmap (ofn.lpstrFile, CMYKArray[DP_COMPOSITE_PLANE], sizeof(BITMAPHANDLE), 24, ORDER_BGR, NULL, NULL);
	
						if(nRet == SUCCESS) {
							
							unsigned char *cmykBuffer, *rgbBuffer, *planes[4];

							rgbBuffer = (unsigned char *)calloc(1, CMYKArray[DP_COMPOSITE_PLANE]->BytesPerLine);

							cmykBuffer = (unsigned char *)calloc(1, CMYKArray[DP_COMPOSITE_PLANE]->Width * 4);

							int c;

							for(c = 0; c < 4; c++) {

								planes[c] = (unsigned char *)calloc(1, CMYKArray[DP_COMPOSITE_PLANE]->Width);

								nRet = L_CreateBitmap(CMYKArray[c],
													sizeof(BITMAPHANDLE),
													TYPE_CONV,
													CMYKArray[DP_COMPOSITE_PLANE]->Width, 
													CMYKArray[DP_COMPOSITE_PLANE]->Height, 
													8,
													ORDER_GRAY,
													(L_RGBQUAD *)GrayPal,
													TOP_LEFT,
													NULL,
													0);

   								if(nRet != SUCCESS) {
									sprintf(szError, "Error: %d", nRet);
									theDlg->OKBox("CreateBitmap (cmyk)", szError, NULL);
								}


							}
							
							if(nRet == SUCCESS) {

								int r, p;
								unsigned char *sptr, *dptr;

								for(c = 0; c < 4; c++) {

									theDlg->StatusLine("Generating %s\n", ColorNames[c]);

									for(r = 0; r < CMYKArray[DP_COMPOSITE_PLANE]->Height; r++) {
										
										dptr = planes[c];
									
										sptr = cmykBuffer;
								
										nRet = L_GetBitmapRow(CMYKArray[DP_COMPOSITE_PLANE], rgbBuffer, r, CMYKArray[DP_COMPOSITE_PLANE]->BytesPerLine);

   										if(nRet != CMYKArray[DP_COMPOSITE_PLANE]->BytesPerLine) {
											sprintf(szError, "Error: %d", nRet);
											theDlg->OKBox("GetBitmapRow (5)", szError, NULL);
										}

										nRet = L_ConvertColorSpace(rgbBuffer, cmykBuffer, CMYKArray[DP_COMPOSITE_PLANE]->Width, CCS_RGB, CCS_CMYK);


   										if(nRet != SUCCESS) {
											sprintf(szError, "Error: %d", nRet);
											theDlg->OKBox("ConvertColorSpace", szError, NULL);
										}


										for(p = 0; p < CMYKArray[DP_COMPOSITE_PLANE]->Width; p++) {

											dptr[p] = sptr[c];

											sptr += 4;

										}

										nRet = L_PutBitmapRow(CMYKArray[channelorder[inkorder][c]], planes[c], r, CMYKArray[c]->BytesPerLine);
								
   										if(nRet != CMYKArray[c]->BytesPerLine) {
											sprintf(szError, "Error: %d", nRet);
											theDlg->OKBox("PutBitmapRow", szError, NULL);
										}

									}

								}

								theDlg->StatusLine("Done\n");

								if(nRet > -1) {
									nRet = SUCCESS;
									goto retryLoad;
								}

							}
							else {
								theDlg->StatusLine("Error %d creating CMYK bitmaps\n", nRet);
								return FALSE;
							}


						}
						else {

							if(nRet != SUCCESS) {
								sprintf(szError, "Error: %d", nRet);
								theDlg->OKBox("ERROR", "Conversion Failed", NULL);
							}


							theDlg->StatusLine("Conversion failed. Error %d when opening RGB image\n", nRet);
							return FALSE;
						}
						

					}

				}
				else {
					theDlg->StatusLine("Graphic load error: %d\n", nRet);
					return FALSE;
				}

			}

		}

		return TRUE;
	}
	
	return FALSE;

}


/*-------------------------------------------------------------------------------------*/
/*-                                                                                   -*/
/*-------------------------------------------------------------------------------------*/
BOOL LoadTestBand(int channel) {


	if(OnFileOpenTestBand(NULL, 0)) {
		//return(ConvertPlane(channel, FALSE));
		return(ConvertPlane(channel, TRUE));
	}

	return FALSE;


}

/*-------------------------------------------------------------------------------------*/
/*-                                                                                   -*/
/*-------------------------------------------------------------------------------------*/
void PrintTestBand(void *p) {
int i, reps, c;
int timeout = 0;
testbandVars *vars = (testbandVars *)p;

	vars->SendThreadRunning = TRUE;

	reps = vars->Rep;

	vars->Progress->SetPos(0);

	vars->Progress->ShowWindow(SW_SHOW);

	theApp.bPrinterPrepped = 0;
	theApp.bYPrintMovementSet = 0;

	theApp.bTestBandMode = TRUE;

	
	SendPlatenHeightOffset();

	// USBPrinterCommand(USB_CMD_PREPARE_FOR_JOB, 1);
	
	Sleep(500);

	if(!theApp.NoUSB) {
		while(!(/*theApp.bPrinterPrepped && */theApp.GPlatenPosition == 2) || theApp.GMasterStatus == MASTER_STATUS_PAUSED) {
			Sleep(1);
			timeout++;
			if(timeout > 20000) {
				MessageBox(GetFocus(), "PrintTestBand Timeout", "GENAPP", MB_OK|MB_SYSTEMMODAL);
				goto abortsendtestband;
			}
		}
	}

	USBPrinterCommand(USB_SET_SOL, (13 - vars->LM) * 1200);

	USBPrinterCommand(USB_SET_BIDI_COR, theApp.GVars->CustBiDiCor);

	USBPrinterCommand(USB_CMD_SET_LAYERTYPE, LAYER_TP_SINGLE_COLOR);

	if(vars->bForce1Bit)
		USBPrinterCommand(USB_CMD_SET_ONEWAVE, 1);

	USBPrinterCommand(USB_CMD_SET_Y_RESOLUTION, YReses[vars->YParams - 1]);

	vars->Progress->SetPos(50);

	switch(vars->XResolution) {

		case 0: //150
			USBPrinterCommand(USB_CMD_SET_X_RESOLUTION, X_RESOLUTION_150);
			break;

		case 1: //300:
			USBPrinterCommand(USB_CMD_SET_X_RESOLUTION, X_RESOLUTION_300);
			break;

		case 2: // 600:
			USBPrinterCommand(USB_CMD_SET_X_RESOLUTION, X_RESOLUTION_600);
			break;

		case 3:
		default:
			USBPrinterCommand(USB_CMD_SET_X_RESOLUTION, X_RESOLUTION_1200);
			break;

	}

	if(vars->Top > 0)
		USBPrinterCommand(USB_CMD_FEED, (int)(vars->Top * (float)4000)); // initial position

	vars->iDirection = DIRECTION_AWAY;

	for(i = 0; i < reps; i++) {

		for(c = 0; c < 4; c++) {
			if(vars->ChannelToUse[c] > 0) {
				if(OnFileOpenTestBand(vars->szTestFiles[c], 0)) {
					//ConvertPlane(vars->ChannelToUse[c] - 1, FALSE);
					ConvertPlane(vars->ChannelToUse[c] - 1, TRUE);
					SendTestBand(vars, vars->ChannelToUse[c] - 1);
					if(vars->bBiDirectional) {
						vars->iDirection = !vars->iDirection;
					}

					//USBPrinterCommand(USB_CMD_FEED, vars->YAdvance);
				}
			}
		}
	}

	Sleep(2000);

	USBPrinterCommand(USB_CMD_HOME_PLATEN, 0);
	USBPrinterCommand(USB_CMD_PARK, 0);

abortsendtestband:

	vars->Progress->SetPos(100);

	Sleep(2000);

	vars->Status->SetWindowText(" ");
	vars->Progress->ShowWindow(SW_HIDE);
	vars->SendThreadRunning = FALSE;

	theApp.bTestBandMode = FALSE;


}

/*-------------------------------------------------------------------------------*/
/*-  Flip the band about the X axis for bi-directional printing                 -*/
/*-------------------------------------------------------------------------------*/
static void MirrorBuffer(unsigned char *buffer, int rasters, int length) {
int r, p, c;
unsigned char *rptr, *dptr;
unsigned char temp_ras[2880];

	/**/ return;  // do nothing


	if(!FlipMapInited)
		InitFlipMap();


	for(r = 0; r <rasters; r++) {

		rptr = buffer + (length * (r + 1)) - 1;
		dptr = buffer + (length * r);

		for(p = 0; p < length; p++) {
			temp_ras[p] = FlipMap[*rptr];
			rptr--;
		}
		memcpy(dptr, temp_ras, length);

	}


}

/*-------------------------------------------------------------------------------*/
/*-                                                                             -*/
/*-------------------------------------------------------------------------------*/
static void SaveLastBand(PRINT_HEAD *head) {
HANDLE fh;
unsigned long bytes;


	DeleteFile("LastBand.bin");

	fh = CreateFile(	"LastBand.bin", 
						GENERIC_WRITE,
						0,
						(LPSECURITY_ATTRIBUTES)NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE)NULL);
	
	if(fh != INVALID_HANDLE_VALUE) {
	
		WriteFile(fh, head->formatted_data, head->TotalBytes, &bytes, NULL);

		CloseHandle(fh);
	
	}

}

/*-------------------------------------------------------------------------------*/
/*-  Temporary quickie test stuff                                               -*/
/*-                                                                             -*/
/*-                                                                             -*/
/*-                                                                             -*/
/*-------------------------------------------------------------------------------*/
void SendTestBand(testbandVars *vars, int index) {
PRINT_HEAD head;
int i, r, p, c, n, iBand, bands, nRet, b, ti = index, raster_index = 0;
float fbands;
unsigned char *buffer[TOTAL_CHANNELS] = {NULL, NULL, NULL, NULL}, pixel_chunk[TOTAL_NOZZLES_HW * BITS_PER_PIXEL * 2];
int adjusted_lengths[TOTAL_CHANNELS];
int padding[TOTAL_CHANNELS], BiDirectional_Correction = 0;
BOOL bBiDirectional = FALSE, bDoublePrint = FALSE, bSecondPass = FALSE;
BOOL iDirection = DIRECTION_HOME;
BOOL bFirstSwath = TRUE;
int max_offset = 0, z;
char szError[128];


	vars->Status->SetWindowText("Formatting & initializing");

	HANDLE fh;

	switch(vars->XResolution) {
		case 0: // 150
			memcpy((char *)raster_offsets, (char *)raster_offsets_150, sizeof(int) * 4 * 2);
			break;
		case 1: // 300
			memcpy((char *)raster_offsets, (char *)raster_offsets_300, sizeof(int) * 4 * 2);
			break;
		case 2: // 600
			memcpy((char *)raster_offsets, (char *)raster_offsets_600, sizeof(int) * 4 * 2);
			break;
		case 3: // 1200
			memcpy((char *)raster_offsets, (char *)raster_offsets_1200, sizeof(int) * 4 * 2);
			break;
	}
	

	r = 0;

	fbands = (float)snd_bmp[ti].Height / (float)TOTAL_NOZZLES_HW;

	bands = (int)fbands;

	if(fbands > (float)bands) {
		bands++;
	}


	BiDirectional_Correction = 0;

	bBiDirectional = vars->bBiDirectional;

	if(bBiDirectional)
		iDirection = vars->iDirection;
	else
		iDirection = DIRECTION_AWAY; // <-- will be flipped after head inited so start like this


	for(p = 0; p < TOTAL_CHANNELS; p++) {

		if(p == index) {
			L_CopyBitmap(&working_bmp[p], &snd_bmp[p], sizeof(BITMAPHANDLE));
			if(bBiDirectional && iDirection == DIRECTION_AWAY) {
				L_ReverseBitmap(&working_bmp[p]);
			}
		}
	
	
	}


	bDoublePrint = FALSE;

	int rasters_remain = snd_bmp[ti].Height;

	for(iBand = 0; iBand < bands; iBand++) { /////////////////////////
		char bndmsg[128];
		sprintf(bndmsg, "Click Yes to send band %d\nOr No to abort", iBand + 1);

		if(vars->bVerifyBands/*theApp.GVars->ConfirmBands*/) {

			int retv = MessageBox(GetFocus(),bndmsg, "GENESIS CONTROL APP", MB_SYSTEMMODAL|MB_YESNO);

			if(retv != 6) {

				for(c = 0; c < TOTAL_CHANNELS; c++) {
					free(buffer[c]);
					buffer[c] = NULL;
				}
				vars->Status->SetWindowText("Print aborted\r\n");
				return;
			}

		}

		bSecondPass = FALSE;

bandrepeat:

		InitHead(&head);


		head.printdirection = iDirection;

		max_offset = 0;


		head.pixelWidth = snd_bmp[ti].Width;
		head.ByteLength = snd_bmp[ti].BytesPerLine;


		for(c = 0; c < TOTAL_CHANNELS; c++) {
			adjusted_lengths[c] = head.pixelWidth + raster_offsets[head.printdirection][c];

			if(raster_offsets[0][c] > max_offset)
				max_offset = raster_offsets[0][c];
			if(raster_offsets[1][c] > max_offset)
				max_offset = raster_offsets[1][c];

		}


		if(bBiDirectional || bDoublePrint)
			head.flags |= JOB_INFO_BIDIRECTIONAL;

		head.printdirection = iDirection;

		/*
		for(c = 0; c < TOTAL_CHANNELS; c++) {
			if(head.flags & JOB_INFO_BIDIRECTIONAL && head.printdirection == DIRECTION_HOME) {
				padding[c] = raster_offsets[head.printdirection][c] + BiDirectional_Correction;
			}
			else {
				padding[c] = raster_offsets[head.printdirection][c];
			}
		}
		*/

		for(c = 0; c < TOTAL_CHANNELS; c++) {
			padding[c] = raster_offsets[head.printdirection][c];
		}

		//theDlg->StatusLine("Offsets: %d  %d  %d  %d\r\n", raster_offsets[head.printdirection][0], raster_offsets[head.printdirection][1], raster_offsets[head.printdirection][2], raster_offsets[head.printdirection][3]);

		if(!bSecondPass) {
			for(c = 0; c < TOTAL_CHANNELS; c++) {
				if(buffer[c] == NULL)
					buffer[c] = (unsigned char *)calloc(1, snd_bmp[ti].BytesPerLine * TOTAL_NOZZLES_HW * 2);
			}
		}
		unsigned char *rptr;

		int rasters;

		b = 0;

		for(c = 0; c < TOTAL_CHANNELS; c++) {
			for(n = 0; n < TOTAL_NOZZLES_HW; n++) {
				head.Raster[c][n] = buffer[c] + (n * snd_bmp[ti].BytesPerLine);
			}
		}

		//// fill horizontal buffers

		// load the buffers
		int rasters_to_send = rasters_remain < TOTAL_NOZZLES_HW?rasters_remain:TOTAL_NOZZLES_HW;

		for(p = 0; p < TOTAL_CHANNELS; p++) {

			if(p == index || CopyFlags[p]) {

				rptr = buffer[p];

				memset(rptr, 0, snd_bmp[p].BytesPerLine * TOTAL_NOZZLES_HW);
				
				// channels 0 and 2 are ok (top is top)
				//if(p == 0 || p == 2) {
					//r = 0;
					r = raster_index;
					for(c = 0; c < rasters_to_send; c++) {
						//nRet = L_GetBitmapRow(&snd_bmp[p], rptr, r++, snd_bmp[p].BytesPerLine);
						nRet = L_GetBitmapRow(&working_bmp[index /*p*/], rptr, r++, snd_bmp[index /*p*/].BytesPerLine);

   						if(nRet != snd_bmp[index /*p*/].BytesPerLine) {
							sprintf(szError, "Error: %d", nRet);
							theDlg->OKBox("GetBitmapRow (6)", szError, NULL);
						}

						if(vars->bForce1Bit) {
							for(z = 0; z < snd_bmp[index /*p*/].BytesPerLine; z++) {
								if(rptr[z] & 0x80) rptr[z] ^= 0x80;
								if(rptr[z] & 0x20) rptr[z] ^= 0x20;
								if(rptr[z] & 0x08) rptr[z] ^= 0x08;
								if(rptr[z] & 0x02) rptr[z] ^= 0x02;
							}
						}
						rptr += snd_bmp[index /*p*/].BytesPerLine;
					}
				//}
				// channels 1 and 3 are up-side-down
				//else {
				//	r = TOTAL_NOZZLES_HW - 1;
				//	r += raster_index;
				//	for(c = 0; c < rasters_to_send; c++) {
				//		nRet = L_GetBitmapRow(&snd_bmp[p], rptr, r--, snd_bmp[p].BytesPerLine);
				//		rptr += snd_bmp[p].BytesPerLine;
				//	}
				//}
			}
		}
			
		if(!bDoublePrint || (bDoublePrint && bSecondPass)) {
			rasters_remain -= rasters_to_send;
			raster_index += rasters_to_send;
		}

		for(c = 0; c < TOTAL_CHANNELS; c++) {
			if(c == index && head.printdirection == DIRECTION_AWAY) {

				if(!theApp.GVars->bDontMirrorTestBands)
					MirrorBuffer(buffer[c], TOTAL_NOZZLES_HW, snd_bmp[ti].BytesPerLine);

			}
		}

		unsigned char *tptr;

		tptr = head.formatted_data;

		//for(p = 0; p < ((int)head.pixelWidth + raster_offsets[0][TOTAL_CHANNELS - 1]); p++) {
		for(p = 0; p < ((int)head.pixelWidth + max_offset); p++) {
		
			// zero the holding chunks
			memset(pixel_chunk, 0, TOTAL_NOZZLES_HW * BITS_PER_PIXEL);

			for(r = 0; r < TOTAL_NOZZLES_HW; r++) {

				// build the high-order bit chunk
				for(c = 0; c < TOTAL_CHANNELS; c++) {
					if(padding[c] < 1 && p < adjusted_lengths[c]) {
						if(*head.Raster[c][r] & 0x80) {
							pixel_chunk[r] |= color_mask_bit1[c];
							// _CrtCheckMemory();
						}
						if(*head.Raster[c][r] & 0x40) {
							pixel_chunk[r] |= color_mask_bit2[c];
							// _CrtCheckMemory();
						}
					}
				}
			}
		

			//copy the chunks into the master buffer
			memcpy(tptr, pixel_chunk, TOTAL_NOZZLES_HW);
			// _CrtCheckMemory();
			tptr += TOTAL_NOZZLES_HW;

			if(tptr > (head.formatted_data + HEAD_SWATH_LENGH)) {
				tptr = tptr;
			}


			if(b == 3) {
				for(c = 0; c < TOTAL_CHANNELS; c++) {
					if(padding[c] < 1 && p < adjusted_lengths[c]) {
						for(r = 0; r < TOTAL_NOZZLES_HW; r++) {
							head.Raster[c][r]++;
						}
					}
					else {
						padding[c]--;
					}
				}
				b = 0;
			}
			else {
				for(c = 0; c < TOTAL_CHANNELS; c++) {
					if(padding[c] < 1 && p < adjusted_lengths[c]) {
						for(r = 0; r < TOTAL_NOZZLES_HW; r++) {
							*head.Raster[c][r] = *head.Raster[c][r] << BITS_PER_PIXEL;
						}
					}
					else {
						padding[c]--;
					}
				}
				b++;
			}
		}

		head.pixelWidth += max_offset;
		head.TotalBytes = head.pixelWidth * TOTAL_NOZZLES_HW;

		theDlg->StatusLine("Band is %d bytes long\r\n", head.TotalBytes);

		/// prepare band record

		CTH_BAND_INFO band;

		if(bDoublePrint || bBiDirectional)
			head.flags |= JOB_INFO_BIDIRECTIONAL;


		//newPacket.lastband = lastBand;
		band.prefix[0] = band.prefix[1] = band.prefix[2] = band.prefix[3] = 0xAA;
		band.lastband = 0;
		band.start_x = head.Start;
		band.length_x = head.pixelWidth;
		band.bytelength = head.TotalBytes;
		band.special_x = head.special_x;
		band.direction = head.printdirection;
		band.flags = head.flags;

		// SaveLastBand(&head);

		/*
		if(bDoublePrint && !bSecondPass)
			band.y_step = 0;
		else
			band.y_step = 2123;
		*/
		band.y_step = vars->YAdvance;


		//band.direction = head.printdirection;
	
		// now flip all the dwords
		FlipDWORD(&band.bytelength);
		FlipDWORD(&band.start_x);
		FlipDWORD(&band.length_x);
		FlipDWORD(&band.special_x);
		FlipDWORD(&band.y_step);
		FlipDWORD(&band.direction);
		FlipDWORD(&band.lastband);
		FlipDWORD(&band.flags);


			vars->Status->SetWindowText("Sending band ");

		int x;

		if(FTDWrite(&FPGAusb, (unsigned char *)&band, sizeof(CTH_BAND_INFO))) {

			if(!FTDWrite(&FPGAusb, head.formatted_data, head.TotalBytes)) {
				vars->Status->SetWindowText("... ERROR");
			}
			else {
				vars->Status->SetWindowText("... Done\r\n");
			}
		}

		if(bDoublePrint && !bSecondPass) {
		
			// raster_index -= rasters_to_send;

			head.flags |= JOB_INFO_BIDIRECTIONAL;
			iDirection	= !iDirection;
			bSecondPass = TRUE;
			goto bandrepeat;
		}
		if(bDoublePrint && bSecondPass) {
			head.flags |= JOB_INFO_BIDIRECTIONAL;
			iDirection	= !iDirection;
		}
		if(vars->bBiDirectional /*bBiDirectional*/) {
			head.flags |= JOB_INFO_BIDIRECTIONAL;
			iDirection	= !iDirection;
		}

	}

	for(c = 0; c < TOTAL_CHANNELS; c++) {
		free(buffer[c]);
		buffer[c] = NULL;
	}
	head.Inited = 0;
	free(head.formatted_data);
	head.formatted_data = NULL;


}

//  BandDebug.cpp
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg4.h"
#include "ftd2xx.h"
#include "ftdisupp.h"

#include "parser.h"
#define STEPS_DEFINED
#include "HEAD_structs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//							 C     M     Y      K
BOOL ChannelsToDebug[4] = {TRUE, TRUE, FALSE, FALSE};

extern weave_pattern weave[TOTAL_MICROWEAVE_PATTERNS];

static BITMAPHANDLE Bitmap[4];
static int rasterindex[4];
static int prs_rasterindex[4];

#ifdef DEBUG_PARSER_DATA
/*-------------------------------------------------------------------------------*/
/*-                                                                             -*/
/*-------------------------------------------------------------------------------*/
void SaveParserData(int channel, unsigned char *data, int length, displayGraphicState *gs) {
int i, nRet;
char szFileName[MAX_PATH];

	if(data == NULL) {


		return;
	}

	if(data == (unsigned char *)1) {

		for(i = 0; i < TOTAL_CHANNELS; i++) {

			if(ChannelsToDebug[i]) {
				sprintf(szFileName, "PrsChannel%d.tif", i);
				DeleteFile(szFileName);

				nRet = L_SaveBitmap(szFileName, &Bitmap[i], FILE_TIF, 2, 0, NULL);
				L_FreeBitmap(&Bitmap[i]);

			}
		}

		return;
	}



	if(data == (unsigned char *)2) {
	
		for(i = 0; i < TOTAL_CHANNELS; i++) {

			if(Bitmap[i].Flags.Allocated) {
				L_FreeBitmap(&Bitmap[i]);
			}

			memset((char *)prs_rasterindex, 0, sizeof(int) * 4);

			sprintf(szFileName, "PrsChannel%d.tif", channel);

			DeleteFile(szFileName);
		
			if(ChannelsToDebug[i]) {

				nRet = L_CreateBitmap(&Bitmap[i],
										sizeof(BITMAPHANDLE),
										TYPE_CONV,
										gs->pixelWidth,
										gs->pixelHeight + (weave[gs->cur_mw].nozzles_used * (weave[gs->cur_mw].passes - 1) * 2),
										2,
										ORDER_GRAY, // color order not used
										NULL,
										TOP_LEFT,
										NULL,
										0);

				if(nRet == SUCCESS) {
					L_FillBitmap(&Bitmap[i], RGB(0,0,0));
				}
				else {
					MessageBox(GetFocus(), "Can't create bitmap for band debug", "Genesis Controller", MB_OK|MB_SYSTEMMODAL);
					continue;
				}
			}

		}
		return;
	}

	if(ChannelsToDebug[channel]) {
		nRet = L_PutBitmapRow(&Bitmap[channel], data, prs_rasterindex[channel]++,  length);
	}


}
#endif

#ifdef FILE_PER_BAND
/*-------------------------------------------------------------------------------*/
/*-                                                                             -*/
/*-------------------------------------------------------------------------------*/
void SaveBandData(void *bnd) {
int i, nRet, r;
INBAND *inband = (INBAND *)bnd;
unsigned char *ras_ptr[TOTAL_NOZZLES_HW];
char szFileName[32];


	if(bnd == NULL) return;

	for(i = 0; i < TOTAL_CHANNELS; i++) {

		if(Bitmap[i].Flags.Allocated) {
			L_FreeBitmap(&Bitmap[i]);
		}

		sprintf(szFileName, "Channel_%d(%d).tif", i, inband->band);

		DeleteFile(szFileName);
		
		if(ChannelsToDebug[i]) {

			nRet = L_CreateBitmap(&Bitmap[i],
									sizeof(BITMAPHANDLE),
									TYPE_CONV,
									inband->gs.pixelWidth,
									inband->pixelHeight,
									2,
									ORDER_GRAY, // color order not used
									NULL,
									TOP_LEFT,
									NULL,
									0);

			if(nRet == SUCCESS) {
				L_FillBitmap(&Bitmap[i], RGB(0,0,0));
			}
			else {
				MessageBox(GetFocus(), "Can't create bitmap for band debug", "Genesis Controller", MB_OK|MB_SYSTEMMODAL);
				continue;
			}

			for(r = 0; r < inband->pixelHeight; r++) {
				ras_ptr[r] = inband->plane[i] + (r * inband->byteWidth);
			}

			for(r = 0; r < inband->pixelHeight; r++) {
				nRet = L_PutBitmapRow(&Bitmap[i], ras_ptr[r], r,  inband->byteWidth);
			}
		
			nRet = L_SaveBitmap(szFileName, &Bitmap[i], FILE_TIF, 2, 0, NULL);
			L_FreeBitmap(&Bitmap[i]);

		}

	}


}


#else

/*-------------------------------------------------------------------------------*/
/*-                                                                             -*/
/*-------------------------------------------------------------------------------*/
void SaveBandData(void *bnd) {
int i, nRet, r;
INBAND *inband = (INBAND *)bnd;
unsigned char *ras_ptr[TOTAL_NOZZLES_HW];


	if(bnd == NULL) {
		char szFileName[32];

		for(i = 0; i < TOTAL_CHANNELS; i++) {


			if(Bitmap[i].Flags.Allocated) {

				sprintf(szFileName, "Channel_%d.tif", i);

				DeleteFile(szFileName);

				nRet = L_SaveBitmap(szFileName, &Bitmap[i], FILE_TIF, 2, 0, NULL);

				L_FreeBitmap(&Bitmap[i]);
			}
		
		
		}

		memset((char *)rasterindex, 0, sizeof(int) * 4);
		return;

	}

	if(inband->band == 0) {

		for(i = 0; i < TOTAL_CHANNELS; i++) {
			if(Bitmap[i].Flags.Allocated) L_FreeBitmap(&Bitmap[i]);
		}

		for(i = 0; i < TOTAL_CHANNELS; i++) {

			if(ChannelsToDebug[i]) {

				nRet = L_CreateBitmap(&Bitmap[i],
										sizeof(BITMAPHANDLE),
										TYPE_CONV,
										inband->gs.pixelWidth,
										inband->gs.pixelHeight + (weave[inband->gs.cur_mw].nozzles_used * (weave[inband->gs.cur_mw].passes - 1) * 2),
										2,
										ORDER_GRAY, // color order not used
										NULL,
										TOP_LEFT,
										NULL,
										0);

				if(nRet == SUCCESS) {
					L_FillBitmap(&Bitmap[i], RGB(0,0,0));
				}

			}

		}

	}

	for(i = 0; i < TOTAL_CHANNELS; i++) {
	
		if(ChannelsToDebug[i]) {
		
			for(r = 0; r < inband->pixelHeight; r++) {
				ras_ptr[r] = inband->plane[i] + (r * inband->byteWidth);
			}
			

			for(r = 0; r < inband->pixelHeight; r++) {
				nRet = L_PutBitmapRow(&Bitmap[i], ras_ptr[r], rasterindex[i]++,  inband->byteWidth);
			}
		
		}
	
	
	}


}
#endif

#ifdef DEBUG_BAND_QUEUE
/*----------------------------------------------------------------------------------------*/
/*-  Add raster to source map to be microweaved                                          -*/
/*----------------------------------------------------------------------------------------*/
static void SaveBand(inswath *swath, displayGraphicState *gs, int band_no) {
char szFileName[TOTAL_CHANNELS][MAX_PATH];
BITMAPHANDLE Bitmap[TOTAL_CHANNELS];
int nRet, i, c;
unsigned char *ras;

	for(c = 0; c < 1; c++) {
		sprintf(szFileName[c], "%s\\pre_weave_%d_%d.tif", theDlg->szHomeDir, c, band_no);
		DeleteFile(szFileName[c]);
		nRet = L_CreateBitmap(	&Bitmap[c],
								sizeof(BITMAPHANDLE),
								TYPE_CONV,
								gs->pixelWidth, 
								TOTAL_NOZZLES_HW,
								8,
								ORDER_GRAY,
								NULL,
								TOP_LEFT,
								NULL, 0);
	}

	for(c = 0; c < 1; c++) {
		for(i = 0; i < TOTAL_NOZZLES_HW; i++) {
			ras = swath->plines[c][i];
			L_PutBitmapRow(&Bitmap[c], ras, i, gs->byteWidth);
		}
	}
	
	for(c = 0; c < 1; c++) {
		nRet = L_SaveBitmap(szFileName[c], &Bitmap[c], FILE_TIF, 8, 0, NULL);
		L_FreeBitmap(&Bitmap[c]);
	}
}
#endif

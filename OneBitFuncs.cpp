
// ImageToHeadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "afxdialogex.h"
#include "Printing.h"

#define HDF_FILE	 1
#include "parser.h"
#define STEPS_DEFINED
//#define DEFINE_RASTER_OFFSET_ARRAYS
#include "HEAD_structs.h"

extern CCriticalSection		BandQueueCS;
//extern CCriticalSection		HeadQueueCS;
extern CController4Dlg1 *theDlg;
extern BOOL GBand_Dir;
extern weave_pattern weave[4];
extern void MirrorBand(INBAND *);
extern void FlipBand(INBAND *, BOOL);

extern int INPUT_BAND_HEIGHT;

extern BOOL isBlank(unsigned char *, int);

extern CPrinting *pdlg;

extern void SaveBandData(void *);

#ifdef PROOF_WEAVED_BANDS
	extern 	void CreateBandPreview(INBAND *);
#endif

extern int raster_offsets_150[2][4];
extern int raster_offsets_300[2][4];
extern int raster_offsets_600[2][4];
extern int raster_offsets_1200[2][4];

extern int raster_offsets_150_aux[2][4];
extern int raster_offsets_300_aux[2][4];
extern int raster_offsets_600_aux[2][4];
extern int raster_offsets_1200_aux[2][4];

static int raster_offsetsOneBit[2][4];

/*
static unsigned char color_mask_bit1OneBit[TOTAL_CHANNELS] = {0x80, 0x40, 0x20, 0x10};
static unsigned char color_mask_bit2OneBit[TOTAL_CHANNELS] = {0x08, 0x04, 0x02, 0x01};
*/

static unsigned char color_mask_bit1OneBit[TOTAL_CHANNELS] = {0x20, 0x80, 0x40, 0x10};
static unsigned char color_mask_bit2OneBit[TOTAL_CHANNELS] = {0x02, 0x08, 0x04, 0x01};


/**/ #define SKIP_OVER_WHITE_Y
/*-------------------------------------------------------------------------------*/
/*-  Format a horizontal buffer for the print head                              -*/
/*-  *Must* be 180 rasters. Can be any width divisible by 4                     -*/
/*-  Supports bi-directional as well as skip-over-white in both X and Y dir     -*/
/*-------------------------------------------------------------------------------*/
BOOL CreateHeadSwathOneBit(PRINT_HEAD *hd, INBAND *inband, BOOL release_band) {
int c, p, b, r, n;
unsigned char *tptr;
BOOL bIsEmpty = TRUE;
unsigned char Chunk[160 * 2];
int padding[TOTAL_CHANNELS];
int adjusted_lengths[TOTAL_CHANNELS];
int nRet, max_offset;

#ifdef DEBUG_BAND_DATA
	SaveBandData((void *)inband);
#endif

#ifdef SKIP_OVER_WHITE_Y
// if band is blank, just request a feed
	if(!inband->UsedChannels /*&& !theApp.NoUSB*/) {
		hd->ByteLength = USB_COMMAND;
		hd->flags = USB_CMD_FEED;
		goto end_create_head;
	}
#endif

	theApp.bCreatingSwath = TRUE;


#ifdef HEAP_DEBUG
	if(!_CrtCheckMemory()) {
		MessageBox(GetFocus(), "HEAP CORRUPTED(1)", "DEBUG", MB_OK|MB_SYSTEMMODAL);
	}
#endif

	switch(inband->Resolution.x) {
		case 150:
			memcpy((char *)raster_offsetsOneBit, (char *)raster_offsets_150, sizeof(int) * 4 * 2);
			break;
		case 300:
			memcpy((char *)raster_offsetsOneBit, (char *)raster_offsets_300, sizeof(int) * 4 * 2);
			break;
		case 600:
			memcpy((char *)raster_offsetsOneBit, (char *)raster_offsets_600, sizeof(int) * 4 * 2);
			break;
		case 1200:
			memcpy((char *)raster_offsetsOneBit, (char *)raster_offsets_1200, sizeof(int) * 4 * 2);
			break;
	}

// #define FORCE_UNIDIRECTIONAL


#ifdef FORCE_UNIDIRECTIONAL
	if(0) {
#else
	if(inband->PrintDirection) {
#endif
		hd->flags |= JOB_INFO_BIDIRECTIONAL;

		if(GBand_Dir == DIRECTION_AWAY) {
			hd->printdirection = DIRECTION_AWAY;  // moving away from capping station
		}
		else {
			hd->printdirection = DIRECTION_HOME;  // moving towards capping station
		}

		GBand_Dir = !GBand_Dir;

	}
	else {
		hd->printdirection = DIRECTION_AWAY;
		if(hd->flags & JOB_INFO_BIDIRECTIONAL) {
			hd->flags ^= JOB_INFO_BIDIRECTIONAL;
		}
	}

	LOCK_BANDQUEUE
	if(hd->state != HEAD_STATE_UNUSED) {
		UNLOCK_BANDQUEUE
	
		theApp.bCreatingSwath = FALSE;

		return FALSE;
	}

	hd->state = HEAD_STATE_FILLING;

	UNLOCK_BANDQUEUE

	FlipBand(inband, FALSE);

	if(hd->printdirection == DIRECTION_AWAY) {

		MirrorBand(inband);

	}

	memset(hd->formatted_data, 0, HEAD_SWATH_LENGH);

#ifdef HEAP_DEBUG
	if(!_CrtCheckMemory()) {
		MessageBox(GetFocus(), "HEAP CORRUPTED(1)", "DEBUG", MB_OK|MB_SYSTEMMODAL);
	}
#endif


	max_offset = 0;
	for(c = 0; c < TOTAL_CHANNELS; c++) {
		adjusted_lengths[c] = inband->pixelWidth + raster_offsetsOneBit[hd->printdirection][c];

		if(raster_offsetsOneBit[0][c] > max_offset)
			max_offset = raster_offsetsOneBit[0][c];
		if(raster_offsetsOneBit[1][c] > max_offset)
			max_offset = raster_offsetsOneBit[1][c];

	}

	hd->pixelWidth = inband->pixelWidth;
	hd->ByteLength = inband->byteWidth;

	for(c = 0; c < TOTAL_CHANNELS; c++) {
	
		padding[c] = raster_offsetsOneBit[hd->printdirection][c];

	}

	// assign destination pointers in raw buffer
	for(c = 0; c < TOTAL_CHANNELS; c++) {
		hd->RasterPtr[c] = inband->plane[c];
		for(n = 0; n < TOTAL_NOZZLES_HW; n++) {
			hd->Raster[c][n] = hd->RasterPtr[c] + (n * inband->byteWidth);
#ifdef PROOF_RAW_DATA
			DbgRas[c][n] = hd->Raster[c][n];
#endif
		}
	}


#ifdef HEAP_DEBUG
	if(!_CrtCheckMemory()) {
		MessageBox(GetFocus(), "HEAP CORRUPTED(1)", "DEBUG", MB_OK|MB_SYSTEMMODAL);
	}
#endif


	hd->UsedChannels = inband->UsedChannels;

	tptr = hd->formatted_data;

	//ApplyBandBounds(hd, inband); /// temporarily disabled

	// theDlg->StatusLine("FirstPix: L: %d  R: %d\r\n", hd->FirstPix[0], hd->FirstPix[1]);

	if(FALSE /*TRUE*/ ) { /// temporarily disabled
		//RemoveXWhiteSpace(hd);
	}
	else {
		if(hd->printdirection == DIRECTION_HOME) {
			hd->Start = hd->pixelWidth + max_offset; // raster_offsetsOneBit[0][/*TOTAL_CHANNELS - 1*/  2];
		}
		else {
			hd->Start = 0;
		}
	}

	b = 0;
	int cr = 0;

	for(p = 0; p < ((int)hd->pixelWidth + max_offset); p++) {
		// zero the holding chunks
		
		memset(Chunk, 0, 160 * 2);

		cr = 0;

		for(r = 0; r < TOTAL_NOZZLES_HW; r++) {
			// build the high-order bit chunk
			for(c = 0; c < TOTAL_CHANNELS; c++) {
				if(padding[c] < 1 && p < adjusted_lengths[c]) {
					//if(*hd->Raster[c][r] & 0xC0) {
					//if(*hd->Raster[c][r] & 0x80) {
					/**/if(*hd->Raster[c][r] & 0x40) {
						Chunk[cr] |= color_mask_bit1OneBit[c];
					}
				}
			}
			r++;
			for(c = 0; c < TOTAL_CHANNELS; c++) {
				if(padding[c] < 1 && p < adjusted_lengths[c]) {
					//if(*hd->Raster[c][r] & 0xC0) {
					//if(*hd->Raster[c][r] & 0x80) {
					/**/if(*hd->Raster[c][r] & 0x40) {
						Chunk[cr] |= color_mask_bit2OneBit[c];
					}
				}
			}
			cr++;
		}

		//copy the chunks into the master buffer
		memcpy(tptr, Chunk, 160);
		// *tptr = *tptr & 0x0F; /// <--- have to find out why this cures the artifact
		tptr += 160;

		/// slide the source pointers
		if(b == 3) {
			for(c = 0; c < TOTAL_CHANNELS; c++) {
				if(padding[c] < 1 && p < adjusted_lengths[c]) {
					for(r = 0; r < TOTAL_NOZZLES_HW; r++) {
						hd->Raster[c][r]++;
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
						*hd->Raster[c][r] = *hd->Raster[c][r] << BITS_PER_PIXEL;
					}
				}
				else {
					padding[c]--;
				}
			}
			b++;
		}

	}

#ifdef HEAP_DEBUG
		if(!_CrtCheckMemory()) {
			MessageBox(GetFocus(), "HEAP CORRUPTED(1)", "DEBUG", MB_OK|MB_SYSTEMMODAL);
		}
#endif

	hd->pixelWidth += max_offset;

#ifdef ROUND_PIXEL_WIDTH_TO_WORD
	while(hd->pixelWidth % 4)
		hd->pixelWidth++;
#endif

	hd->TotalBytes = hd->pixelWidth * 160;

end_create_head:

	LOCK_BANDQUEUE


	if(release_band) {
		// TRACE("band %X marked unused\n", inband);
		inband->state = BAND_STATE_UNUSED;
		memset(inband->address, 0, inband->bufferLength);
	}

	hd->state = HEAD_STATE_READY;
	UNLOCK_BANDQUEUE

	theApp.bCreatingSwath = FALSE;

		
	return TRUE;	
}

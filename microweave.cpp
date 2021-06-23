//  microweave.cpp
//  microweave related code

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "parser.h"
#include "HEAD_structs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CController4Dlg1 *theDlg;
extern void AddRaster(unsigned char *, int, int, int, displayGraphicState *);
extern CCriticalSection		BandQueueCS;

CCriticalSection		WeaveCS;

void Weaver(void *);

inswath *swath_top = NULL;

#define MAX_LINEBUFFS	TOTAL_CHANNELS

offset_list	GOffsets;


// #define DEBUG_BAND_QUEUE

weave_pattern weave[TOTAL_MICROWEAVE_PATTERNS] = {

	// Single Step (600 x 150)
	320,
	1,
	320,
	
	// Double-Step  (600 x 300)
	160,
	2,
	320,

	// Quad Step (600 x 600)
	80,
	4,
	320,

	// Octo Step (600 x 1200)
	40,
	8,
	320,

	// Ten Step (600 x 900 dpi)
	32,
	10,
	320,

	// Twelve Step (600 x 1800 dpi)
	26,
	12,
	312,

	// Sixteen Step (600 x 2400 dpi)
	20,
	16,
	320,

};

//LAYER_REC lr[8];

LAYER_REC lr;

inswath *filling_swath = 0, *using_swath = 0;

static raster_order_tree	RasterTree;
// index into tree structure
static int treeOrder[4] = {2, 1, 0, 3}; // cyan (0) = index 3, magenta (1) = index 1, yellow (2) = index 0, black (3) = index 3
static unsigned char channel_fuses[4] = {0x01, 0x02, 0x04, 0x08};

extern BOOL BandsAllocated();

#ifdef DEBUG_PARSER_DATA
	extern void SaveParserData(int, unsigned char *, int, displayGraphicState *);
#endif

/*----------------------------------------------------------------------------------------*/
/*- Initialize microweave params                                                         -*/
/*----------------------------------------------------------------------------------------*/
void InitMW() {
int c, l;


	memset((char *)&lr, 0, sizeof(lr));

	inswath *slide = swath_top;

	while(slide != NULL) {

		for(l = 0; l < TOTAL_NOZZLES_HW; l++) {

			for(c = 0; c < TOTAL_CHANNELS; c++) {
				if(slide->plines[c][l] != NULL) {
					free(slide->plines[c][l]);
					slide->plines[c][l] = NULL;
				}
			}

		}

		slide = slide->next;
	}

	swath_top = NULL;

}


/*----------------------------------------------------------------------------------------*/
/*-  Delete a swath                                                                      -*/
/*----------------------------------------------------------------------------------------*/
void DeleteSwath(inswath *swath) {
int c, l;

	if(swath == NULL)
		return;

	LOCK_CS

	for(c = 0; c < TOTAL_CHANNELS; c++) {
		for(l = 0; l < TOTAL_NOZZLES_HW; l++) {
			if(swath->plines[c][l] != NULL) {
				free(swath->plines[c][l]);
				swath->plines[c][l] = NULL;
			}
		}
	}

	free(swath);
	
	UNLOCK_CS

}

/*----------------------------------------------------------------------------------------*/
/*-  Create a swath                                                                      -*/
/*----------------------------------------------------------------------------------------*/
inswath *CreateSubSwath(displayGraphicState *gs) {
int c, l;
inswath *newswath;

	LOCK_CS
	//LOCK_BANDQUEUE

	newswath = (inswath *)calloc(1, sizeof(inswath));
	for(c = 0; c < TOTAL_CHANNELS; c++) {
		for(l = 0; l < TOTAL_NOZZLES_HW; l++) {
			newswath->plines[c][l] = (unsigned char *)calloc(1, gs->byteWidth * 1.5);
		}
	}

	//UNLOCK_BANDQUEUE
	UNLOCK_CS
	return newswath;
}

/*----------------------------------------------------------------------------------------*/
/*-  Select microweave index based on resolution                                         -*/
/*----------------------------------------------------------------------------------------*/
void SelectMW(displayGraphicState *gs) {
float tb;


	if(gs->ResolutionX == 300 && gs->ResolutionY == 150) {
		gs->cur_mw = WEAVE_SINGLE_PASS;
	}

	if(gs->ResolutionX == 300 && gs->ResolutionY == 300) {
		gs->cur_mw = WEAVE_DOUBLE_PASS;
	}

	if(gs->ResolutionX == 600 && gs->ResolutionY == 150) {
		gs->cur_mw = WEAVE_SINGLE_PASS;
	}

	if(gs->ResolutionX == 600 && gs->ResolutionY == 300) {
		gs->cur_mw = WEAVE_DOUBLE_PASS;
	}


	if(gs->ResolutionX == 600 && gs->ResolutionY == 600) {
		gs->cur_mw = WEAVE_QUAD_PASS;
	}

	if(gs->ResolutionX == 600 && gs->ResolutionY == 900) {
		gs->cur_mw = WEAVE_TEN_PASS;
	}

	if(gs->ResolutionX == 600 && gs->ResolutionY == 1800) {
		gs->cur_mw = WEAVE_TWELVE_PASS;
	}

	if(gs->ResolutionX == 600 && gs->ResolutionY == 2400) {
		gs->cur_mw = WEAVE_SIXTEEN_PASS;
	}

	//////////////////////////////////

	if(gs->ResolutionX == 1200 && gs->ResolutionY == 150) {
		gs->cur_mw = WEAVE_SINGLE_PASS;
	}

	if(gs->ResolutionX == 1200 && gs->ResolutionY == 300) {
		gs->cur_mw = WEAVE_DOUBLE_PASS;
	}

	if(gs->ResolutionX == 1200 && gs->ResolutionY == 600) {
		gs->cur_mw = WEAVE_QUAD_PASS;
	}

	if(gs->ResolutionX == 600 && gs->ResolutionY == 1200) {
		gs->cur_mw = WEAVE_OCTO_PASS;
	}

	if(gs->ResolutionX == 1200 && gs->ResolutionY == 1200) {
		gs->cur_mw = WEAVE_OCTO_PASS;
	}

	if(gs->ResolutionX == 1200 && gs->ResolutionY == 900) {
		gs->cur_mw = WEAVE_TEN_PASS;
	}

	if(gs->ResolutionX == 1200 && gs->ResolutionY == 1800) {
		gs->cur_mw = WEAVE_TWELVE_PASS;
	}

	if(gs->ResolutionX == 1200 && gs->ResolutionY == 2400) {
		gs->cur_mw = WEAVE_SIXTEEN_PASS;
	}


	int i;

	switch(gs->ResolutionY) {
		case 150:
			i = 0;
			break;
		case 300:
			i = 1;
			break;
		case 600:
			i = 2;
			break;
		case 1200:
			i = 3;
			break;
		case 900:
			i = 4;
			break;
		case 1800:
			i = 5;
			break;
		case 2400:
			i = 6;
			break;
	}
	
	if(theApp.GVars->iPrinterType == 2) {
		if(i > 3) {
			theDlg->OKBox("This printer does not", "support this resolution", NULL);
			i = 3;
		}

		gs->Y_Advance = theApp.GVars->AdvanceGM[i];

	}
	else {
		gs->Y_Advance = theApp.GVars->Advance[i];
	}

	gs->mw_inited = FALSE;

	tb = (float)gs->pixelHeight / (float)TOTAL_NOZZLES_HW;

	gs->TotalBands = (int)tb;

	if(tb > (float)gs->TotalBands)
		gs->TotalBands++;

	// gs->TotalBands /= weave[gs->cur_mw].passes;



}

/*----------------------------------------------------------------------------------------*/
/*-  Clear offset lists                                                                  -*/
/*----------------------------------------------------------------------------------------*/
void ClearXOffsetList(offset_list *list) {

	if(list->ielements) {
		free(list->elements);
		list->elements = NULL;
		list->ielements = 0;
	}

}
/*----------------------------------------------------------------------------------------*/
/*-  Create list to store left and right offsets. Doing int before it gets to the        -*/
/*-  band formatter thread will allow for look-ahead                                     -*/
/*----------------------------------------------------------------------------------------*/
void CreateXOffsetList(offset_list *list, displayGraphicState *gs) {
int i;

	ClearXOffsetList(list);

	i = (gs->pixelHeight + 16) * weave[gs->cur_mw].passes;

	list->elements = (LROffst *)calloc(i, sizeof(LROffst));

	list->ielements = i;


}

/*----------------------------------------------------------------------------------------*/
/*-  Initialize the microweave system                                                    -*/
/*----------------------------------------------------------------------------------------*/
void InitializeMW(displayGraphicState *gs) {
int i, l, c;

	SelectMW(gs);

	CreateXOffsetList(&GOffsets, gs);

	inswath *newswath, *slide;

	swath_top = CreateSubSwath(gs);

	for(i = 0; i < 16; i++) {
		lr.MapPosition[i] = i;
	}

	lr.gs = gs;
	lr.Pass = 0;
	lr.Height = weave[gs->cur_mw].nozzles_used / weave[gs->cur_mw].passes;
	lr.FirstNozzle = (lr.Height * weave[gs->cur_mw].passes - 1) - 1;
	//lr.FirstNozzle = (lr.Height * weave[gs->cur_mw].passes - 1);  // <--- this disn't seem to make any difference
	lr.LastMapPosition = 0;
	gs->mw_inited = TRUE;


}

/*-------------------------------------------------------------------------------*/
/*- See if band has been completely used up                                     -*/
/*-------------------------------------------------------------------------------*/
/*
static BOOL CheckUsage(inswath *swath) {
int c, r;

	LOCK_CS

	for(c = 0; c < TOTAL_CHANNELS; c++) {

		for(r = 0; r < TOTAL_NOZZLES_HW; r++) {
		
			if(!swath->markers[c][r]) {
				UNLOCK_CS
				return FALSE;
			}
		
		}

	}

	UNLOCK_CS

	return TRUE;
}
*/


/*----------------------------------------------------------------------------------------*/
/*-  Add raster to source list to be microweaved                                         -*/
/*----------------------------------------------------------------------------------------*/
void AddRasterMW(unsigned char *ras, int channel, int length, BOOL forceEnd, displayGraphicState *gs, BOOL bIsFirstNonBlank) {
int raster_index, gs_i = gs->VerticalPos - 1;
static int in_swath_index;
static int cur_raw_band;

	if(theApp.GMasterStatus == MASTER_STATUS_IMPORTING) {
		return;
	}

	if(!gs->mw_inited) {
		InitializeMW(gs);
		filling_swath = swath_top;
		theDlg->StatusLine("Setting filling_swath to swath_top\r\n");
		in_swath_index = 0;
		lr.swath = swath_top;
		cur_raw_band = 0;
		theApp.GSentRasters = 0;
	}

	if(gs_i >=  weave[gs->cur_mw].nozzles_used) {
		raster_index = gs_i / weave[gs->cur_mw].nozzles_used;
		raster_index = gs_i - (raster_index * weave[gs->cur_mw].nozzles_used);

	}
	else {
		raster_index = gs_i;
	}

	if(in_swath_index != gs_i && raster_index == 0) {

		cur_raw_band++;

		if(filling_swath->next == NULL) {

			//theDlg->StatusLine("New swath. Crb: %d\r\n", cur_raw_band);

			filling_swath->next = CreateSubSwath(gs);

			filling_swath->next->bandNo = cur_raw_band;

			filling_swath->ready = TRUE;

			filling_swath = filling_swath->next;

		}
		else {
			/// apparently, this never happens VVV
			filling_swath->ready = TRUE;
			filling_swath = filling_swath->next;
			// theDlg->StatusLine("MW Next swath %d\r\n", gs->VerticalPos);
			/**/ TRACE("filling_swath->next is not null!!\n");
			cur_raw_band++;
		}
	}
	
	in_swath_index = gs_i;	

	memcpy(filling_swath->plines[channel][raster_index], ras, length);

#ifdef DEBUG_BAND_DATA
	// if(channel == 1) TRACE("memcpy into %d-%d\n", channel, raster_index);
#endif

	// safety catch
	if(filling_swath->ready) {
		char errbuff[128];
		sprintf(errbuff, "ERROR. adding to already full swath: cb: %d gpos: %d, rpos: %d", cur_raw_band, gs->VerticalPos, raster_index);
		MessageBox(GetFocus(), errbuff, "DEBUG", MB_OK|MB_SYSTEMMODAL);
	}

	if(raster_index > (weave[gs->cur_mw].nozzles_used - 1)) {
		char errbuff[128];		sprintf(errbuff, "ERROR. Raster out of bounds: gpos: %d, rpos: %d", gs->VerticalPos, raster_index);
		MessageBox(GetFocus(), errbuff, "DEBUG", MB_OK|MB_SYSTEMMODAL);
	}

}


/*----------------------------------------------------------------------------------------*/
/*-  Add raster to source list to be microweaved, use order tree                         -*/
/*----------------------------------------------------------------------------------------*/
void PreAddRasterMW(unsigned char *ras, int channel, int length, BOOL forceEnd, displayGraphicState *gs, BOOL bIsFirstNonBlank) {
int i, channel_index;
unsigned char *out_ras;

#ifdef NO_RASTER_SHUFFLE
		if(ras == NULL)
			return;
		AddRasterMW(ras, channel, length, forceEnd, gs, bIsFirstNonBlank);  /**********/ 
#endif

	if(theApp.GMasterStatus == MASTER_STATUS_IMPORTING) {
		return;
	}

#ifdef DEBUG_PARSER_DATA
	SaveParserData(channel, ras, length, gs);
#endif

	if(ras == NULL) {
		memset((char *)&RasterTree, 0, sizeof(raster_order_tree));
		return;
	}



	if(gs->ResolutionY < 600 || theApp.GVars->bNoYShuffle) {
		AddRasterMW(ras, channel, length, forceEnd, gs, bIsFirstNonBlank);  /**********/ 
		return;
	}


	channel_index = treeOrder[channel];

	if(RasterTree.Height == 0) {
		switch((int)gs->ResolutionY) {
			case	600:
					RasterTree.Height = 3;
					break;

			case	1200:
					RasterTree.Height = 6;
					break;
		}

		RasterTree.PrintingSlot[0] = 0;
		RasterTree.PrintingSlot[1] = RasterTree.Height / 3;
		RasterTree.PrintingSlot[2] = (RasterTree.Height / 3) * 2;
		RasterTree.PrintingSlot[3] = (RasterTree.Height / 3) * 3;

		for(i = 0; i < 4; i++) {
			RasterTree.FirstPrintable[i] = RasterTree.PrintingSlot[i];
		}

	}

	memcpy(RasterTree.Raster[channel_index][RasterTree.NextSlot[channel_index]], ras, length);
	RasterTree.length[channel_index][RasterTree.NextSlot[channel_index]] = length;

	if(RasterTree.CurrentY[channel_index] >= RasterTree.FirstPrintable[channel_index]) {

		out_ras = RasterTree.Raster[channel_index][RasterTree.PrintingSlot[channel_index]];

		//AddRasterMW(out_ras, channel, length, forceEnd, gs, bIsFirstNonBlank);  /**********/ 

		AddRasterMW(RasterTree.Raster[channel_index][RasterTree.PrintingSlot[channel_index]],
					channel,
					RasterTree.length[channel_index][RasterTree.PrintingSlot[channel_index]],
					forceEnd,
					gs,
					bIsFirstNonBlank);  /**********/ 


		RasterTree.PrintingSlot[channel_index]++;

		if(RasterTree.PrintingSlot[channel_index] >= RasterTree.Height) {
			RasterTree.PrintingSlot[channel_index] = 0;
		}


	}

	RasterTree.NextSlot[channel_index]++;
	if(RasterTree.NextSlot[channel_index] >= RasterTree.Height) {
		RasterTree.NextSlot[channel_index] = 0;
	}

	RasterTree.CurrentY[channel_index]++;


}


/*-------------------------------------------------------------------------------*/
/*- Microweave worker thread                                                    -*/
/*-------------------------------------------------------------------------------*/
void Weaver(void *in) {
int i, c, p;
inswath *ts;
displayGraphicState *gs;


	lr.FirstNozzle = 0;
	lr.Nozzle = 0;
	lr.swath = swath_top;


	while(!theApp.quit_threads) {

#ifdef HEAP_DEBUG
		if(!_CrtCheckMemory()) {
			MessageBox(GetFocus(), "HEAP CORRUPTED(1)", "DEBUG", MB_OK|MB_SYSTEMMODAL);
		}
#endif

		while(lr.gs == NULL) {
			if(theApp.quit_threads)
				break;
			Sleep(10); 
		}
		
		gs = (displayGraphicState *)lr.gs;

		lr.swath = swath_top;

		if(lr.swath == NULL || lr.gs == NULL || !gs->mw_inited || !BandsAllocated()) {
			Sleep(0);
			continue;
		}


		// reset map positions
		for(i = 0; i < weave[gs->cur_mw].passes; i++) {
			lr.MapPosition[i] = i;
		}

		int lb = 0;


		// theDlg->StatusLine("Building swath\r\n");

		for(lr.Pass = 0; lr.Pass < weave[gs->cur_mw].passes; lr.Pass++) {
retrypass:
			if(!theApp.bInJob)
				break;

			if(theApp.quit_threads)
				return;
		    LOCK_CS;
			if(lr.swath == NULL || !lr.swath->ready) {
				UNLOCK_CS;
				if(theApp.quit_threads)
					return;
				Sleep(100);
				goto retrypass;
			}
			UNLOCK_CS;
			if(theApp.quit_threads) return;

			// theDlg->StatusLine("lr.MapPosition[lr.Pass]: %d  (%d)\r\n", lr.MapPosition[lr.Pass], lr.Pass);


			// theDlg->StatusLine("Swath Ready. Band: %d\r\n", lr.swath->bandNo);

			for(i = 0; i < lr.Height; i++) {

				for(c = 0; c < TOTAL_CHANNELS; c++) {
					LOCK_CS;
					AddRaster(lr.swath->plines[c][lr.MapPosition[lr.Pass]], c, gs->byteWidth, 0 , gs);
#ifdef DEBUG_BAND_DATA
					// if(c == 1)  TRACE("Addraster into %d-%d\n", c, lr.MapPosition[lr.Pass]);
#endif

					UNLOCK_CS;
				}

#ifdef HEAP_DEBUG
				if(!_CrtCheckMemory()) {
					MessageBox(GetFocus(), "HEAP CORRUPTED(1)", "DEBUG", MB_OK|MB_SYSTEMMODAL);
				}
#endif

				lr.MapPosition[lr.Pass] += weave[gs->cur_mw].passes;

			}

			while(lr.swath == NULL || lr.swath->next == NULL) {
				Sleep(0);
			}

			lr.swath = lr.swath->next;

			if(lr.swath == NULL)
				break;
		}

		LOCK_CS;
		inswath *temp_top = swath_top;
		swath_top = swath_top->next;
		UNLOCK_CS
		// theDlg->StatusLine("Deleting swath. %d\r\n", temp_top->bandNo);
		DeleteSwath(temp_top);

#ifdef HEAP_DEBUG
		if(!_CrtCheckMemory()) {
			MessageBox(GetFocus(), "HEAP CORRUPTED(1)", "DEBUG", MB_OK|MB_SYSTEMMODAL);
		}
#endif




	}

}


// Controller1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg4.h"
#include <AFXMT.H>

#include "parser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern char szCurrentFile[MAX_PATH];
extern void ParserLoop(void *);
extern CController4Dlg1 *theDlg;
extern CController4Dlg4 *theJobDlg;


GVARS G_Vars;

#include "init_keys.h"

extern displayGraphicState GS;

extern int raster_offsets_150[2][4];
extern int raster_offsets_300[2][4];
extern int raster_offsets_600[2][4];
extern int raster_offsets_1200[2][4];

extern int raster_offsets_150_aux[2][4];
extern int raster_offsets_300_aux[2][4];
extern int raster_offsets_600_aux[2][4];
extern int raster_offsets_1200_aux[2][4];

#define  ICON_DPI	6

#define WHITE_COLOR RGB(255,255,255)
#define BLACK_COLOR RGB(13,13,13)


extern QUEUE_ITEM *FindOtherLayer(QUEUE_ITEM *, int);

BOOL QueueItemStillValid(QUEUE_ITEM *);

/*-----------------------------------------------------------------------------------------------------*/
/*-  Create a new queue item and stick it in the linked list                                          -*/
/*-----------------------------------------------------------------------------------------------------*/
void CController4Dlg4::DisplayQueueItem(QUEUE_ITEM *q) {
char tmp_buff[200];
int layer = 0;

	if(q == NULL) {
		theApp.bHasPreview = FALSE;
	}

	else {

	}

	theApp.qitem = q;

	RECT rc;

	theDlg->m_Preview.GetClientRect(&rc);
	theDlg->m_Preview.InvalidateRect(&rc);

	//OnPaint();


}

/*-----------------------------------------------------------------------------------------------------*/
/*-  Create a new queue item and stick it in the linked list                                          -*/
/*-----------------------------------------------------------------------------------------------------*/
void CController4Dlg4::DeleteQueueItem(QUEUE_ITEM *q) {
int i, l;
QUEUE_ITEM *qi = theApp.q_item_top, *aux_qi;
BOOL bDeletedDisplayed = FALSE, bIsCurrentJob = FALSE;
char tmp_buff[128];
BOOL OldHalt = theApp.HaltJobScanner;

	theApp.ScannerDelay = 1;

	theApp.QueueCS.Lock();

	/// first see if this is the job we're displaying in the main window

	if(q == (QUEUE_ITEM *)theApp.CurrentJob) {
		bIsCurrentJob = TRUE;
	}



	// check for type of job here (don't delete manually inserted ones?)
	if(strlen(q->szFileName[0]))
		DeleteFile(q->szFileName[0]);


	sprintf(tmp_buff, "ICONS\\%dC.jpg", q->JobNumber);
	DeleteFile(tmp_buff);

	sprintf(tmp_buff, "ICONS\\%dW.jpg", q->JobNumber);
	DeleteFile(tmp_buff);

	for(i = 0; i < theApp.QueueItems; i++) {
	
		if(qi == q) {

			if(q->previous != NULL) {
				q->previous->next = q->next;
			}
			if(q->next != NULL) {
				q->next->previous = q->previous;
			}
			if(q == theApp.q_item_top)
				theApp.q_item_top = theApp.q_item_top->next;

			theApp.QueueItems--;

			if(q == theApp.qitem) {
				bDeletedDisplayed = TRUE;
			}
			theApp.m_QueueImages.Remove(q->iconindex);
			free(q);
			break;
		}
		qi = qi->next;
	}
	

	if(bDeletedDisplayed) {
		theApp.qitem = NULL;
		if(theApp.QueueWND != NULL)
			DisplayQueueItem(NULL);
		theApp.bHasPreview = FALSE;
		if(theApp.QueueWND != NULL)
			OnPaint();
	}

	if(bIsCurrentJob) {

		//TRACE("CurJobCS lock 7\n");
		//theApp.CurJobCS.Lock(INFINITE);
	
		theApp.CurrentJob = theApp.q_item_top;
		if(theApp.CurrentJob != NULL && QueueItemStillValid((QUEUE_ITEM *)theApp.CurrentJob)) {
			theApp.GVars->CurrentJobNumber = ((QUEUE_ITEM *)theApp.CurrentJob)->JobNumber;
		}
		else {
			theApp.GVars->CurrentJobNumber = 0;
		}
		theDlg->InvalidateRect(&theDlg->ButtonArea[CURRENT_COUNTER_BUTTON].slice[0]);
		
		theDlg->ForcePreviewRefresh(FALSE);

		//theApp.CurJobCS.Unlock();

	}

	theApp.QueueCS.Unlock();

	theApp.WriteQueue();

	/**/ TRACE("HaltEvent 161\n");
	theApp.HaltJobScanner = OldHalt;
	//theApp.HaltJobScanner = FALSE;

}

/*-----------------------------------------------------------------------------------------------------*/
/*-  Create a new queue item and stick it in the linked list                                          -*/
/*-----------------------------------------------------------------------------------------------------*/
void CController4Dlg4::DeleteQueueItem(QUEUE_ITEM *q, BOOL bDeleteFiles) {
int i, l;
QUEUE_ITEM *qi = theApp.q_item_top;
BOOL bDeletedDisplayed = FALSE;
char tmp_buff[128];

	theApp.QueueCS.Lock();

	// check for type of job here (don't delete manually inserted ones?)
	if(strlen(q->szFileName[0]))
		DeleteFile(q->szFileName[0]);

	if(bDeleteFiles) {

		DeleteFile(tmp_buff);

		sprintf(tmp_buff, "ICONS\\%dW.jpg", q->JobNumber);
		DeleteFile(tmp_buff);

		sprintf(tmp_buff, "ICONS\\%dC.jpg", q->JobNumber);
		DeleteFile(tmp_buff);


		/*sprintf(tmp_buff, "ICONS\\%dP.jpg", q->JobNumber);
		DeleteFile(tmp_buff);*/

	}

	for(i = 0; i < theApp.QueueItems; i++) {
	
		if(qi == q) {

			if(q->previous != NULL) {
				q->previous->next = q->next;
			}
			if(q->next != NULL) {
				q->next->previous = q->previous;
			}
			if(q == theApp.q_item_top)
				theApp.q_item_top = theApp.q_item_top->next;

			theApp.QueueItems--;

			if(q == theApp.qitem) {
				bDeletedDisplayed = TRUE;
			}
			theApp.m_QueueImages.Remove(q->iconindex);
			free(q);
			break;
		}
		qi = qi->next;
	}
	
	if(bDeletedDisplayed) {
		theApp.qitem = NULL;
		if(theApp.QueueWND != NULL)
			DisplayQueueItem(NULL);
		theApp.bHasPreview = FALSE;
		if(theApp.QueueWND != NULL)
			OnPaint();
	}

	theApp.QueueCS.Unlock();

	theApp.WriteQueue();

}



/*-----------------------------------------------------------------------------------------------------*/
/*-  Create a new queue item and stick it in the linked list                                          -*/
/*-----------------------------------------------------------------------------------------------------*/
void CController4Dlg4::DisplayQueue() {
int				iIcon, iItem, iSubItem, iActualItem;
LV_ITEM			lvitem;
char tmp_buff[200];
QUEUE_ITEM *qi = theApp.q_item_top;

	theApp.QueueCS.Lock();

	m_JobList.SetImageList(&theApp.m_QueueImages, LVSIL_NORMAL);

	m_JobList.DeleteAllItems();

	for(iItem = 0; iItem < theApp.QueueItems; iItem++) {

		for(iSubItem = 0; iSubItem < 3; iSubItem++) {
			iIcon = 0; // set to actual image here
			lvitem.mask = LVIF_TEXT | LVIF_IMAGE;//(iSubItem == 0? LVIF_IMAGE : 0);
			lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
			lvitem.iSubItem = iSubItem;
			switch(iSubItem) {

				case 0:
					sprintf(tmp_buff, "%s(%c)", qi->JobName, qi->bIsWhite?'w':'c');
					//lvitem.pszText = qi->JobName;
					lvitem.pszText = tmp_buff;
					break;

				case 1:
					sprintf(tmp_buff, "%d x %d", (int)qi->resolution.x, (int)qi->resolution.y);
					lvitem.pszText = tmp_buff;
					break;

			}

			lvitem.iImage = qi->iconindex;

			if (iSubItem == 0) {
				iActualItem = m_JobList.InsertItem(&lvitem);
				m_JobList.SetItemData(iActualItem, (DWORD)qi);

			}
			else {
				m_JobList.SetItem(&lvitem);
			}

		}
		qi = qi->next;
	}

	theApp.QueueCS.Unlock();


}

/*-----------------------------------------------------------------------------------------------------*/
/*-  Create a new queue item and stick it in the linked list                                          -*/
/*-----------------------------------------------------------------------------------------------------*/
void CController4Dlg4::CreateNewQueueItem(char *szFileName) {
QUEUE_ITEM *qi;
int c, layer_number;
int add_mode = ADD_MODE_NEW;

	// first see if item with that number exists
theApp.QueueCS.Lock();

	qi = theApp.q_item_top;

	while(qi != NULL) {
		if(qi->JobNumber == GS.JobNumber) {
			add_mode = ADD_MODE_MODIFY;
			break;
		}
		qi = qi->next;
	}

	theApp.QueueCS.Unlock();

	if(add_mode == ADD_MODE_NEW) {

		qi = (QUEUE_ITEM *)calloc(1, sizeof(QUEUE_ITEM));

		strcpy(qi->JobName, GS.JobName);
		qi->JobNumber = GS.JobNumber;
		qi->printarea.x = GS.PaperDimensionX;
		qi->printarea.y = GS.PaperDimensionY;
		qi->resolution.x = (float)GS.ResolutionX;
		qi->resolution.y = (float)GS.ResolutionY;

	}

	strcpy(qi->szSourceFile, GS.szCurrentFile);

	switch(GS.layerType) {

		case LAYER_TP_WHITE:
		case LAYER_TP_DOUBLE_WHITE:
			layer_number = 0;
			break;
			
		case LAYER_TP_COLOR_WHL:
		case LAYER_TP_DUAL_COLOR:
		case LAYER_TP_SINGLE_COLOR:
		case LAYER_TP_DOUBLE_WHITE_ONLY:
			layer_number = 1;
			break;

	}

	strcpy(qi->szFileName[layer_number], szFileName);

	if(add_mode == ADD_MODE_NEW) {

		theApp.QueueCS.Lock();

		if(theApp.q_item_top == NULL) {
			theApp.q_item_top = qi;
		}

		else {
		
			QUEUE_ITEM *sld = theApp.q_item_top;
	
			while(sld->next != NULL)
				sld = sld->next;

			sld->next = qi;

			qi->previous = sld;

		}

		theApp.QueueItems++;

		theApp.QueueCS.Unlock();

	}

	//CheckForJobOverflow();

	theApp.WriteQueue();

	theApp.processing_qitem = qi;

	DisplayQueue();


}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void CController4App::WriteINI() {
int i, result;
char tmp_buff[256];
char theDir[MAX_PATH];
float *tempfloat;


	strcpy(theDir, theApp.LaunchDir);
	strcat(theDir, "\\");
	strcat(theDir, INI_FILE);


	for(i = 0; i < MAXINITKEYS; i++) {
		switch((int)INIKeys[i][3]) {

			case TSTR:	//strings
				result = WritePrivateProfileString((char *)INIKeys[i][0],(char *)INIKeys[i][1], (char *)INIKeys[i][2],theDir);
				break;
		
			case TBOL:	//ints
			case TINT:	//ints
				sprintf(tmp_buff, "%d", *INIKeys[i][2]);
				result = WritePrivateProfileString((char *)INIKeys[i][0],(char *)INIKeys[i][1], tmp_buff, theDir);
				break;
		
			case TFLT: //floats
				tempfloat = (float *)INIKeys[i][2];
				sprintf(tmp_buff, "%f", *tempfloat);
				result = WritePrivateProfileString((char *)INIKeys[i][0],(char *)INIKeys[i][1], tmp_buff, theDir);
				break;

			case TSTU:	//WINDOWPLACEMENT
				result = WritePrivateProfileStruct((char *)INIKeys[i][0],(char *)INIKeys[i][1], (void *)INIKeys[i][2], sizeof(WINDOWPLACEMENT), theDir);
				break;
		
		}
	}


}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void CController4App::ReadMotionVals() {
int i, result, error;
char tmp_buff[256];
char theDir[MAX_PATH];

	strcpy(theDir, theApp.LaunchDir);
	strcat(theDir, "\\");
	strcat(theDir, INI_FILE);

	GVars = &G_Vars;

	for(i = 20; i <= 54; i++) {
		if((int)INIKeys[i][3] == TINT) {
			result = GetPrivateProfileString((char *)INIKeys[i][0],(char *)INIKeys[i][1], "1", tmp_buff, 256, theDir);
			*INIKeys[i][2] = (BOOL)atoi(tmp_buff);
		}
		if((int)INIKeys[i][3] == TSTR) {
			result = GetPrivateProfileString((char *)INIKeys[i][0],(char *)INIKeys[i][1], "1", (char *)INIKeys[i][2], 256, theDir);
		}
	}

	/// Just in case not in the INI file:...

	/// Y Advance per resolution
	if(theApp.GVars->Advance[0] == 1) theApp.GVars->Advance[0] = 2115;
	if(theApp.GVars->Advance[1] == 1) theApp.GVars->Advance[1] = 1056;
	if(theApp.GVars->Advance[2] == 1) theApp.GVars->Advance[2] = 533;
	if(theApp.GVars->Advance[3] == 1) theApp.GVars->Advance[3] = 264;


	// INter-head offset per resolution (for dual-cmyk, etc.)
	if(theApp.GVars->InterHeadOffset[0] == 1) theApp.GVars->InterHeadOffset[0] = 220;
	if(theApp.GVars->InterHeadOffset[1] == 1) theApp.GVars->InterHeadOffset[1] = 440;
	if(theApp.GVars->InterHeadOffset[2] == 1) theApp.GVars->InterHeadOffset[2] = 890;
	if(theApp.GVars->InterHeadOffset[3] == 1) theApp.GVars->InterHeadOffset[3] = 1760;

	/// Just in case not in the INI file:...
	if(theApp.GVars->AdvanceGM[0] == 1) theApp.GVars->AdvanceGM[0] = 2560;
	if(theApp.GVars->AdvanceGM[1] == 1) theApp.GVars->AdvanceGM[1] = 1284;
	if(theApp.GVars->AdvanceGM[2] == 1) theApp.GVars->AdvanceGM[2] = 642;
	if(theApp.GVars->AdvanceGM[3] == 1) theApp.GVars->AdvanceGM[3] = 321;


	/// Inter-channel offset
#ifdef OFFSET_INK_ORDER

	//// for offset ink order (different A from B)


	// 150 Channel A
	if(theApp.GVars->InterNozzleSpace150[0] != 1) {
		raster_offsets_150[0][2] = theApp.GVars->InterNozzleSpace150[0];
		raster_offsets_150[1][3] = theApp.GVars->InterNozzleSpace150[0];
		raster_offsets_150_aux[0][3] = theApp.GVars->InterNozzleSpace150[0];
		raster_offsets_150_aux[1][2] = theApp.GVars->InterNozzleSpace150[0];
	}

	// 150 Channel B
	if(theApp.GVars->InterNozzleSpace150[1] != 1) {
		raster_offsets_150[0][1] = theApp.GVars->InterNozzleSpace150[1];
		raster_offsets_150[1][0] = theApp.GVars->InterNozzleSpace150[1];
		raster_offsets_150_aux[0][0] = theApp.GVars->InterNozzleSpace150[1];
		raster_offsets_150_aux[1][1] = theApp.GVars->InterNozzleSpace150[1];
	}

	// 150 Channel C
	if(theApp.GVars->InterNozzleSpace150[2] != 1) {
		raster_offsets_150[0][0] = theApp.GVars->InterNozzleSpace150[2];
		raster_offsets_150[1][1] = theApp.GVars->InterNozzleSpace150[2];
		raster_offsets_150_aux[0][1] = theApp.GVars->InterNozzleSpace150[2];
		raster_offsets_150_aux[1][0] = theApp.GVars->InterNozzleSpace150[2];
	}

	// 150 Channel D
	if(theApp.GVars->InterNozzleSpace150[3] != 1) {
		raster_offsets_150[0][3] = theApp.GVars->InterNozzleSpace150[3];
		raster_offsets_150[1][2] = theApp.GVars->InterNozzleSpace150[3];
		raster_offsets_150_aux[0][2] = theApp.GVars->InterNozzleSpace150[3];
		raster_offsets_150_aux[1][3] = theApp.GVars->InterNozzleSpace150[3];
	}
	////////////////////////////////////
	////////////////////////////////////

	// 300 Channel A
	if(theApp.GVars->InterNozzleSpace300[0] != 1) {
		raster_offsets_300[0][2] = theApp.GVars->InterNozzleSpace300[0];
		raster_offsets_300[1][3] = theApp.GVars->InterNozzleSpace300[0];
		raster_offsets_300_aux[0][3] = theApp.GVars->InterNozzleSpace300[0];
		raster_offsets_300_aux[1][2] = theApp.GVars->InterNozzleSpace300[0];
	}

	// 300 Channel B
	if(theApp.GVars->InterNozzleSpace300[1] != 1) {
		raster_offsets_300[0][1] = theApp.GVars->InterNozzleSpace300[1];
		raster_offsets_300[1][0] = theApp.GVars->InterNozzleSpace300[1];
		raster_offsets_300_aux[0][0] = theApp.GVars->InterNozzleSpace300[1];
		raster_offsets_300_aux[1][1] = theApp.GVars->InterNozzleSpace300[1];
	}

	// 300 Channel C
	if(theApp.GVars->InterNozzleSpace300[2] != 1) {
		raster_offsets_300[0][0] = theApp.GVars->InterNozzleSpace300[2];
		raster_offsets_300[1][1] = theApp.GVars->InterNozzleSpace300[2];
		raster_offsets_300_aux[0][1] = theApp.GVars->InterNozzleSpace300[2];
		raster_offsets_300_aux[1][0] = theApp.GVars->InterNozzleSpace300[2];
	}

	// 300 Channel D
	if(theApp.GVars->InterNozzleSpace300[3] != 1) {
		raster_offsets_300[0][3] = theApp.GVars->InterNozzleSpace300[3];
		raster_offsets_300[1][2] = theApp.GVars->InterNozzleSpace300[3];
		raster_offsets_300_aux[0][2] = theApp.GVars->InterNozzleSpace300[3];
		raster_offsets_300_aux[1][3] = theApp.GVars->InterNozzleSpace300[3];
	}
	////////////////////////////////////
	////////////////////////////////////

	// 600 Channel A
	if(theApp.GVars->InterNozzleSpace600[0] != 1) {
		raster_offsets_600[0][2] = theApp.GVars->InterNozzleSpace600[0];
		raster_offsets_600[1][3] = theApp.GVars->InterNozzleSpace600[0];
		raster_offsets_600_aux[0][3] = theApp.GVars->InterNozzleSpace600[0];
		raster_offsets_600_aux[1][2] = theApp.GVars->InterNozzleSpace600[0];
	}

	// 600 Channel B
	if(theApp.GVars->InterNozzleSpace600[1] != 1) {
		raster_offsets_600[0][1] = theApp.GVars->InterNozzleSpace600[1];
		raster_offsets_600[1][0] = theApp.GVars->InterNozzleSpace600[1];
		raster_offsets_600_aux[0][0] = theApp.GVars->InterNozzleSpace600[1];
		raster_offsets_600_aux[1][1] = theApp.GVars->InterNozzleSpace600[1];
	}

	// 600 Channel C
	if(theApp.GVars->InterNozzleSpace600[2] != 1) {
		raster_offsets_600[0][0] = theApp.GVars->InterNozzleSpace600[2];
		raster_offsets_600[1][1] = theApp.GVars->InterNozzleSpace600[2];
		raster_offsets_600_aux[0][1] = theApp.GVars->InterNozzleSpace600[2];
		raster_offsets_600_aux[1][0] = theApp.GVars->InterNozzleSpace600[2];
	}

	// 600 Channel D
	if(theApp.GVars->InterNozzleSpace600[3] != 1) {
		raster_offsets_600[0][3] = theApp.GVars->InterNozzleSpace600[3];
		raster_offsets_600[1][2] = theApp.GVars->InterNozzleSpace600[3];
		raster_offsets_600_aux[0][2] = theApp.GVars->InterNozzleSpace600[3];
		raster_offsets_600_aux[1][3] = theApp.GVars->InterNozzleSpace600[3];
	}
	////////////////////////////////////
	////////////////////////////////////

	// 1200 Channel A
	if(theApp.GVars->InterNozzleSpace1200[0] != 1) {
		raster_offsets_1200[0][2] = theApp.GVars->InterNozzleSpace1200[0];
		raster_offsets_1200[1][3] = theApp.GVars->InterNozzleSpace1200[0];
		raster_offsets_1200_aux[0][3] = theApp.GVars->InterNozzleSpace1200[0];
		raster_offsets_1200_aux[1][2] = theApp.GVars->InterNozzleSpace1200[0];
	}

	// 1200 Channel B
	if(theApp.GVars->InterNozzleSpace1200[1] != 1) {
		raster_offsets_1200[0][1] = theApp.GVars->InterNozzleSpace1200[1];
		raster_offsets_1200[1][0] = theApp.GVars->InterNozzleSpace1200[1];
		raster_offsets_1200_aux[0][0] = theApp.GVars->InterNozzleSpace1200[1];
		raster_offsets_1200_aux[1][1] = theApp.GVars->InterNozzleSpace1200[1];
	}

	// 1200 Channel C
	if(theApp.GVars->InterNozzleSpace1200[2] != 1) {
		raster_offsets_1200[0][0] = theApp.GVars->InterNozzleSpace1200[2];
		raster_offsets_1200[1][1] = theApp.GVars->InterNozzleSpace1200[2];
		raster_offsets_1200_aux[0][1] = theApp.GVars->InterNozzleSpace1200[2];
		raster_offsets_1200_aux[1][0] = theApp.GVars->InterNozzleSpace1200[2];
	}

	// 1200 Channel D
	if(theApp.GVars->InterNozzleSpace1200[3] != 1) {
		raster_offsets_1200[0][3] = theApp.GVars->InterNozzleSpace1200[3];
		raster_offsets_1200[1][2] = theApp.GVars->InterNozzleSpace1200[3];
		raster_offsets_1200_aux[0][2] = theApp.GVars->InterNozzleSpace1200[3];
		raster_offsets_1200_aux[1][3] = theApp.GVars->InterNozzleSpace1200[3];
	}
	////////////////////////////////////
	////////////////////////////////////
#endif

	/// For same ink order (same Head B as Head A)


	// 150 Channel A
	if(theApp.GVars->InterNozzleSpace150[0] != 1) {
		raster_offsets_150[0][2] = theApp.GVars->InterNozzleSpace150[0];
		raster_offsets_150[1][3] = theApp.GVars->InterNozzleSpace150[0];
		raster_offsets_150_aux[0][2] = theApp.GVars->InterNozzleSpace150[0];
		raster_offsets_150_aux[1][3] = theApp.GVars->InterNozzleSpace150[0];
	}

	// 150 Channel B
	if(theApp.GVars->InterNozzleSpace150[1] != 1) {
		raster_offsets_150[0][1] = theApp.GVars->InterNozzleSpace150[1];
		raster_offsets_150[1][0] = theApp.GVars->InterNozzleSpace150[1];
		raster_offsets_150_aux[0][1] = theApp.GVars->InterNozzleSpace150[1];
		raster_offsets_150_aux[1][0] = theApp.GVars->InterNozzleSpace150[1];
	}

	// 150 Channel C
	if(theApp.GVars->InterNozzleSpace150[2] != 1) {
		raster_offsets_150[0][0] = theApp.GVars->InterNozzleSpace150[2];
		raster_offsets_150[1][1] = theApp.GVars->InterNozzleSpace150[2];
		raster_offsets_150_aux[0][0] = theApp.GVars->InterNozzleSpace150[2];
		raster_offsets_150_aux[1][1] = theApp.GVars->InterNozzleSpace150[2];
	}

	// 150 Channel D
	if(theApp.GVars->InterNozzleSpace150[3] != 1) {
		raster_offsets_150[0][3] = theApp.GVars->InterNozzleSpace150[3];
		raster_offsets_150[1][2] = theApp.GVars->InterNozzleSpace150[3];
		raster_offsets_150_aux[0][3] = theApp.GVars->InterNozzleSpace150[3];
		raster_offsets_150_aux[1][2] = theApp.GVars->InterNozzleSpace150[3];
	}
	////////////////////////////////////
	////////////////////////////////////

	// 300 Channel A
	if(theApp.GVars->InterNozzleSpace300[0] != 1) {
		raster_offsets_300[0][2] = theApp.GVars->InterNozzleSpace300[0];
		raster_offsets_300[1][3] = theApp.GVars->InterNozzleSpace300[0];
		raster_offsets_300_aux[0][2] = theApp.GVars->InterNozzleSpace300[0];
		raster_offsets_300_aux[1][3] = theApp.GVars->InterNozzleSpace300[0];
	}

	// 300 Channel B
	if(theApp.GVars->InterNozzleSpace300[1] != 1) {
		raster_offsets_300[0][1] = theApp.GVars->InterNozzleSpace300[1];
		raster_offsets_300[1][0] = theApp.GVars->InterNozzleSpace300[1];
		raster_offsets_300_aux[0][1] = theApp.GVars->InterNozzleSpace300[1];
		raster_offsets_300_aux[1][0] = theApp.GVars->InterNozzleSpace300[1];
	}

	// 300 Channel C
	if(theApp.GVars->InterNozzleSpace300[2] != 1) {
		raster_offsets_300[0][0] = theApp.GVars->InterNozzleSpace300[2];
		raster_offsets_300[1][1] = theApp.GVars->InterNozzleSpace300[2];
		raster_offsets_300_aux[0][0] = theApp.GVars->InterNozzleSpace300[2];
		raster_offsets_300_aux[1][1] = theApp.GVars->InterNozzleSpace300[2];
	}

	// 300 Channel D
	if(theApp.GVars->InterNozzleSpace300[3] != 1) {
		raster_offsets_300[0][3] = theApp.GVars->InterNozzleSpace300[3];
		raster_offsets_300[1][2] = theApp.GVars->InterNozzleSpace300[3];
		raster_offsets_300_aux[0][3] = theApp.GVars->InterNozzleSpace300[3];
		raster_offsets_300_aux[1][2] = theApp.GVars->InterNozzleSpace300[3];
	}
	////////////////////////////////////
	////////////////////////////////////

	// 600 Channel A
	if(theApp.GVars->InterNozzleSpace600[0] != 1) {
		raster_offsets_600[0][2] = theApp.GVars->InterNozzleSpace600[0];
		raster_offsets_600[1][3] = theApp.GVars->InterNozzleSpace600[0];
		raster_offsets_600_aux[0][2] = theApp.GVars->InterNozzleSpace600[0];
		raster_offsets_600_aux[1][3] = theApp.GVars->InterNozzleSpace600[0];
	}

	// 600 Channel B
	if(theApp.GVars->InterNozzleSpace600[1] != 1) {
		raster_offsets_600[0][1] = theApp.GVars->InterNozzleSpace600[1];
		raster_offsets_600[1][0] = theApp.GVars->InterNozzleSpace600[1];
		raster_offsets_600_aux[0][1] = theApp.GVars->InterNozzleSpace600[1];
		raster_offsets_600_aux[1][0] = theApp.GVars->InterNozzleSpace600[1];
	}

	// 600 Channel C
	if(theApp.GVars->InterNozzleSpace600[2] != 1) {
		raster_offsets_600[0][0] = theApp.GVars->InterNozzleSpace600[2];
		raster_offsets_600[1][1] = theApp.GVars->InterNozzleSpace600[2];
		raster_offsets_600_aux[0][0] = theApp.GVars->InterNozzleSpace600[2];
		raster_offsets_600_aux[1][1] = theApp.GVars->InterNozzleSpace600[2];
	}

	// 600 Channel D
	if(theApp.GVars->InterNozzleSpace600[3] != 1) {
		raster_offsets_600[0][3] = theApp.GVars->InterNozzleSpace600[3];
		raster_offsets_600[1][2] = theApp.GVars->InterNozzleSpace600[3];
		raster_offsets_600_aux[0][3] = theApp.GVars->InterNozzleSpace600[3];
		raster_offsets_600_aux[1][2] = theApp.GVars->InterNozzleSpace600[3];
	}
	////////////////////////////////////
	////////////////////////////////////

	// 1200 Channel A
	if(theApp.GVars->InterNozzleSpace1200[0] != 1) {
		raster_offsets_1200[0][2] = theApp.GVars->InterNozzleSpace1200[0];
		raster_offsets_1200[1][3] = theApp.GVars->InterNozzleSpace1200[0];
		raster_offsets_1200_aux[0][2] = theApp.GVars->InterNozzleSpace1200[0];
		raster_offsets_1200_aux[1][3] = theApp.GVars->InterNozzleSpace1200[0];
	}

	// 1200 Channel B
	if(theApp.GVars->InterNozzleSpace1200[1] != 1) {
		raster_offsets_1200[0][1] = theApp.GVars->InterNozzleSpace1200[1];
		raster_offsets_1200[1][0] = theApp.GVars->InterNozzleSpace1200[1];
		raster_offsets_1200_aux[0][1] = theApp.GVars->InterNozzleSpace1200[1];
		raster_offsets_1200_aux[1][0] = theApp.GVars->InterNozzleSpace1200[1];
	}

	// 1200 Channel C
	if(theApp.GVars->InterNozzleSpace1200[2] != 1) {
		raster_offsets_1200[0][0] = theApp.GVars->InterNozzleSpace1200[2];
		raster_offsets_1200[1][1] = theApp.GVars->InterNozzleSpace1200[2];
		raster_offsets_1200_aux[0][0] = theApp.GVars->InterNozzleSpace1200[2];
		raster_offsets_1200_aux[1][1] = theApp.GVars->InterNozzleSpace1200[2];
	}

	// 1200 Channel D
	if(theApp.GVars->InterNozzleSpace1200[3] != 1) {
		raster_offsets_1200[0][3] = theApp.GVars->InterNozzleSpace1200[3];
		raster_offsets_1200[1][2] = theApp.GVars->InterNozzleSpace1200[3];
		raster_offsets_1200_aux[0][3] = theApp.GVars->InterNozzleSpace1200[3];
		raster_offsets_1200_aux[1][2] = theApp.GVars->InterNozzleSpace1200[3];
	}
	////////////////////////////////////
	////////////////////////////////////


}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void CController4App::ReadINI() {
int i, result, error;
char tmp_buff[256];
char theDir[MAX_PATH];
BOOL bOldBPP;

	strcpy(theDir, theApp.LaunchDir);
	strcat(theDir, "\\");
	strcat(theDir, INI_FILE);

	GVars = &G_Vars;

	bOldBPP = theApp.GVars->bUseOneBitPP;


	for(i = 0; i < MAXINITKEYS; i++) {
		switch((int)INIKeys[i][3]) {

			case TSTR:	//strings
				result = GetPrivateProfileString((char *)INIKeys[i][0],(char *)INIKeys[i][1], "1", (char *)INIKeys[i][2], 256, theDir);
				if(!result) {
					error = GetLastError();
				}
				break;
		
			case TINT:	//ints
				result = GetPrivateProfileString((char *)INIKeys[i][0],(char *)INIKeys[i][1], "1", tmp_buff, 256, theDir);
				*INIKeys[i][2] = atoi(tmp_buff);
				break;
		
	
			case TBOL:	//ints
				result = GetPrivateProfileString((char *)INIKeys[i][0],(char *)INIKeys[i][1], "1", tmp_buff, 256, theDir);
				*INIKeys[i][2] = (BOOL)atoi(tmp_buff);
				break;
		
			case TFLT: //floats
				result = GetPrivateProfileString((char *)INIKeys[i][0],(char *)INIKeys[i][1], "1", tmp_buff, 256, theDir);
				*(float *)INIKeys[i][2] = (float)atof(tmp_buff);
				break;

			case TSTU:	//WINDOWPLACEMENT
				result = GetPrivateProfileStruct((char *)INIKeys[i][0],(char *)INIKeys[i][1], (void *)INIKeys[i][2], sizeof(WINDOWPLACEMENT), theDir);
				break;
		}
	}

	if(GVars->iMaxStepperThrow < 100)
		GVars->iMaxStepperThrow = MAX_PLATEN_STEPPER_THROW;

	if(strlen(GVars->szPassword) < 4 || strlen(GVars->szPassword) > 10) {
		strcpy(GVars->szPassword, DEFAULT_PASSWORD);
	}

	if(GVars->iJobWaitTimeout < 15) {
		GVars->iJobWaitTimeout = 60;
	}

	if(GVars->iCapFillTimer < 59) {
		GVars->iCapFillTimer = 4 * 60; // default to 4 hours
	}

	if(GVars->HeadTemp[0] < 45 || GVars->HeadTemp[0] > 120)
		GVars->HeadTemp[0] = 85;

	if(GVars->HeadTemp[1] < 45 || GVars->HeadTemp[1] > 120)
		GVars->HeadTemp[1] = 85;

	GVars->CurHeadTemp[0] = GVars->CurHeadTemp[1] = 0;

#ifndef USE_MOE_LOGGING
	if(GVars->LogUploadInterval < 10) GVars->LogUploadInterval = 10;
#endif

	if(GVars->JobsBeforeDry < 2) GVars->JobsBeforeDry = 10;

	if(GVars->fInkAlarmPercent == 0.0) GVars->fInkAlarmPercent = INK_ALARM_PERCENT;

	if(GVars->LogPort < 5) GVars->LogPort = DEFAULT_LOGPORT;

	if(GVars->CapVacAtmosphere == 1)
		GVars->CapVacAtmosphere = VAC_ATMOSPHERE_ADC;

	if(GVars->FullCapVacPsi < 5.0)
		GVars->FullCapVacPsi = FULL_VAC_PSI;

	ReadMotionVals();

	if(!GVars->bHideForceButton) {
		theDlg->m_ForceButton.ShowWindow(SW_SHOW);
	}
	else {
		theDlg->m_ForceButton.ShowWindow(SW_HIDE);
	}

	if(theApp.GVars->bBitPPSet) {
		theApp.GVars->bUseOneBitPP = bOldBPP;
	}


}

/*------------------------------------------------------------------*/
/*- Put the little lock icon on the icon                           -*/
/*------------------------------------------------------------------*/
void AddLockIcon(int width, int height, int bpp, unsigned char *buffer) {
unsigned char srcras[512], *tptr, *sptr, *yptr;
int x, y, b;

	yptr = tptr = buffer;

	for(y = 0; y < theApp.lock_icon.Height; y++) {
		
		L_GetBitmapRow(&theApp.lock_icon, srcras, y, theApp.lock_icon.BytesPerLine);
		
		sptr = srcras;

		for(x = 0; x < theApp.lock_icon.Width; x++) {

			if(sptr[0] != 0xFF) {
				for(b = 0; b < bpp; b++) {
					//tptr[b] |= sptr[b];
					tptr[b + ((width * 4) - theApp.lock_icon.BytesPerLine)] = sptr[b];
				}
			}

			tptr += bpp;
			sptr += (theApp.lock_icon.BitsPerPixel / 8);
		}

		yptr += width * bpp;
		tptr = yptr;

	}
	
}

/*------------------------------------------------------------------*/
/*- re-make the bitmap for ctrllist reflecting locked status       -*/
/*------------------------------------------------------------------*/
void UpdateLockIcon(QUEUE_ITEM *qitem) {
CBitmap bmp;
unsigned char bitbuff[ICON_X_SIZE * 4 * (ICON_Y_SIZE + 1)];

	bmp.CreateBitmap(ICON_X_SIZE, ICON_Y_SIZE, 1, 32, RGB(0,0,0));
	memcpy(bitbuff, qitem->iconpixdata, ICON_X_SIZE * ICON_Y_SIZE * 4);

	if(qitem->Flags & Q_ITEM_LOCKED) {
		//AddLockIcon(ICON_X_SIZE, ICON_X_SIZE, 4, bitbuff);
		AddLockIcon(ICON_X_SIZE, ICON_Y_SIZE, 4, bitbuff);
	}

	bmp.SetBitmapBits(ICON_X_SIZE * ICON_Y_SIZE * 4, bitbuff);

	int index = theApp.m_QueueImages.Replace(qitem->iconindex, &bmp, RGB(0,0,0));

}



/*------------------------------------------------------------------*/
/*- Make composite preview                                         -*/
/*------------------------------------------------------------------*/
BOOL CreateCompositePreview(QUEUE_ITEM *c_q, QUEUE_ITEM *w_q) {
BITMAPHANDLE WhiteBitmap, ColorBitmap, CompositeBitmap;

	


	return TRUE;
}


#define MCPS_BUFFSIZE		24000
#define Y_NUMBER_OFFSET		2
/*-------------------------------------------------------------------------------*/
/*-                                                                             -*/
/*-------------------------------------------------------------------------------*/
void AddNumberToBitmap(QUEUE_ITEM *qi, BITMAPHANDLE *Bitmap) {
HANDLE fh, cfh;
char tmp_buff[100], szNumber[10], cDigits[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}, szError[128];
unsigned char *outbuff, *inbuff, *digitbuff, *tmp_ras, *slide, *slide2;
unsigned long bytes;
HRSRC rsrc;
HGLOBAL ps;
int numberrsc[10] = {IDR_JN_DIGIT_0,IDR_JN_DIGIT_1,IDR_JN_DIGIT_2,IDR_JN_DIGIT_3,IDR_JN_DIGIT_4,IDR_JN_DIGIT_5,IDR_JN_DIGIT_6,IDR_JN_DIGIT_7,IDR_JN_DIGIT_8,IDR_JN_DIGIT_9}, nlist[6];
int offsets[6] = {13, 47, 81, 118, 155, 192}, i, digits, ii, size, nRet, p;
BITMAPHANDLE bhDigit;

	if(qi == NULL || Bitmap == NULL) return;

	sprintf(szNumber, "%06d", qi->RipJobNumber);
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

	inbuff = (unsigned char *)calloc(1, Bitmap->BytesPerLine);

#define NUMBER_BACKGROUND_HEIGHT	34
#define NUMBER_BACKGROUND_LENGTH	235

	for(p = 0; p < NUMBER_BACKGROUND_HEIGHT; p++) {

		nRet = L_GetBitmapRow(Bitmap, inbuff,p, Bitmap->BytesPerLine);
		memset(inbuff, 0xFF, NUMBER_BACKGROUND_LENGTH);
		memset(inbuff + (NUMBER_BACKGROUND_LENGTH - 4), 0, 3);
		nRet = L_PutBitmapRow(Bitmap, inbuff, p, Bitmap->BytesPerLine);

	}
	memset(inbuff, 0, NUMBER_BACKGROUND_LENGTH);
	nRet = L_PutBitmapRow(Bitmap, inbuff, p, Bitmap->BytesPerLine);


	for(i = 0; i < digits; i++) {

		rsrc = FindResource(AfxGetInstanceHandle(), (LPCTSTR)numberrsc[nlist[i]], "TIFF_IMAGE"); 

		ps = LoadResource(AfxGetInstanceHandle(), rsrc);

		if(ps == NULL) {
			::MessageBox(::GetFocus(), "Error loading JN resource", PRODUCT_NAME, MB_OK|MB_SYSTEMMODAL);
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

		nRet = L_InvertBitmap(&bhDigit);

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


			nRet = L_GetBitmapRow(Bitmap, inbuff, ii + Y_NUMBER_OFFSET, Bitmap->BytesPerLine);

			if(nRet != Bitmap->BytesPerLine) {
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

			nRet = L_PutBitmapRow(Bitmap, inbuff, ii + Y_NUMBER_OFFSET, bhDigit.Width + offsets[i]);

			if(nRet != bhDigit.Width + offsets[i]) {
				sprintf(szError, "Error: %d", nRet);
				theDlg->OKBox("PutBitmapRow (2)", szError, NULL);
			}

		}
		free(digitbuff);
		free(tmp_ras);

	}
	L_FreeBitmap(&bhDigit);
	free(outbuff);
	free(inbuff);

}



/*------------------------------------------------------------------*/
/*- Make the icon image for the queue display                      -*/
/*------------------------------------------------------------------*/
int CreatePageIconImage(QUEUE_ITEM *qitem, displayGraphicState *gs, BITMAPHANDLE *Bitmap, int mode) { //  0 = convert bitmap  1= get existing bitmap
int width, height, nRet, i;
char szFileName[MAX_PATH];
BITMAPHANDLE iconBitmap;
float fwidth, fheight;
BOOL landscape = FALSE;
QUEUE_ITEM *aux_qitem;

	width = ICON_X_SIZE;
	height = ICON_Y_SIZE;

	fwidth = (float)width / Bitmap->Width;
	fheight = (float)height / Bitmap->Height;

	if(Bitmap->Width > Bitmap->Height)
		landscape = TRUE;

	float ffactor = fwidth < fheight?fwidth:fheight;

	int i_width = (int)((float)Bitmap->Width * ffactor);
	int i_height = (int)((float)Bitmap->Height * ffactor);

	int leftoffset = (width - i_width) / 2;
	int topoffset = (height - i_height) / 2;

	unsigned char *bitbuff, *tptr;
	unsigned char *bitbuff2 = NULL, *tptr2;
	tptr = bitbuff = (unsigned char *)calloc(1, width * (height + 1) * 4);

	aux_qitem = FindOtherLayer(qitem, FIND_COLOR);

	CreateDirectory("ICONS", NULL);
	
	if(!mode) { // create icon from preview bitmap
		
		sprintf(szFileName, "ICONS\\%d%c.jpg", qitem->JobNumber, qitem->bIsWhite?'W':'C');

		nRet = L_SizeBitmap(Bitmap, i_width, i_height, SIZE_BICUBIC);

		nRet = L_CreateBitmap(&iconBitmap,
							  sizeof (BITMAPHANDLE),
							  TYPE_CONV,
							  width ,
							  height ,
							  24,
							  ORDER_BGR,
							  NULL,
							  TOP_LEFT,
							  NULL, 0);

		memset(tptr, 255, iconBitmap.BytesPerLine);

		for(i = 0; i < Bitmap->Height; i++) {
			L_GetBitmapRow(Bitmap, tptr + leftoffset * 3, i, Bitmap->BytesPerLine);
			L_PutBitmapRow(&iconBitmap, tptr, i, iconBitmap.BytesPerLine);
		}

		nRet = L_OffsetBitmap(	&iconBitmap,
								0,
								1 + topoffset ,
								WHITE_COLOR,
								FILL_CLR);

		if(!qitem->bIsWhite) {
			for(i = topoffset; i < Bitmap->Height + topoffset; i++) {
				L_PutPixelColor(&iconBitmap, i, leftoffset, BLACK_COLOR);
				L_PutPixelColor(&iconBitmap, i, iconBitmap.Width - leftoffset - 1, BLACK_COLOR);
			}
			for(i = 0; i < Bitmap->Width; i++) {
				L_PutPixelColor(&iconBitmap, topoffset, i + leftoffset, BLACK_COLOR);
				L_PutPixelColor(&iconBitmap, iconBitmap.Height - topoffset - 1, i + leftoffset, BLACK_COLOR);
			}
		}
		else {
			for(i = topoffset; i < Bitmap->Height + topoffset; i++) {
				L_PutPixelColor(&iconBitmap, i, leftoffset, WHITE_COLOR);
				L_PutPixelColor(&iconBitmap, i, iconBitmap.Width - leftoffset - 1, WHITE_COLOR);
			}
			for(i = 0; i < Bitmap->Width; i++) {
				L_PutPixelColor(&iconBitmap, topoffset, i + leftoffset, WHITE_COLOR);
				L_PutPixelColor(&iconBitmap, iconBitmap.Height - topoffset - 1, i + leftoffset, WHITE_COLOR);
			}
		}

		int x;
		/////////////////////////
		if(topoffset) {// clean the top and bottom clip area
			for(i = 0; i < topoffset; i++) {
				for(x = leftoffset; x < iconBitmap.Width - leftoffset; x++) {
					L_PutPixelColor(&iconBitmap, i, leftoffset + x, BLACK_COLOR);
					L_PutPixelColor(&iconBitmap, iconBitmap.Height - i, leftoffset + x, BLACK_COLOR);
				}
			}
		}
		if(leftoffset) {// clean the left and right clip area
			for(i = 0; i < iconBitmap.Height; i++) {
				for(x = 0; x < leftoffset - 1; x++) {
					L_PutPixelColor(&iconBitmap, i, x, BLACK_COLOR);
					L_PutPixelColor(&iconBitmap, i, iconBitmap.Width - x, BLACK_COLOR);
				}
			}
		}

		DeleteFile(szFileName);

		AddNumberToBitmap(qitem, &iconBitmap);

		nRet = L_SaveBitmap(szFileName, &iconBitmap, FILE_JPEG, 24, 128, NULL);

		L_FreeBitmap(Bitmap);
		L_FreeBitmap(&iconBitmap);

	}
	else { // open existing
		sprintf(szFileName, "ICONS\\%d%c.jpg", qitem->JobNumber, qitem->bIsWhite?'W':'C');
	}

	nRet= L_LoadBitmap(szFileName, Bitmap, sizeof(BITMAPHANDLE), 32, ORDER_BGR, NULL, NULL);

	CBitmap bmp;

	bmp.CreateBitmap(width, height, 1, 32, RGB(0,0,0));

	memset(bitbuff, 255, width * (height + 1) * 4);

	tptr = bitbuff;
	
	for(i = 0; i < height ; i++) {
	
		nRet = L_GetBitmapRow(Bitmap, tptr, i, Bitmap->BytesPerLine);

		tptr += Bitmap->BytesPerLine;

	}

	memcpy(qitem->iconpixdata, bitbuff, width * height * 4);

	if(qitem->Flags & Q_ITEM_LOCKED) {
		AddLockIcon(width, height, 4, bitbuff);
	}

	bmp.SetBitmapBits(width * height * 4, bitbuff);

	free(bitbuff);

	L_FreeBitmap(Bitmap);

	int index = theApp.m_QueueImages.Add(&bmp, RGB(0,0,0));

	/**/ TRACE("Assigning index of %d to %s\n", index, szFileName);

	return index;
	
}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
BOOL CController4App::FindICONInQueue(char *szFileName) {
QUEUE_ITEM *qi_slide = q_item_top;
BOOL ret = FALSE;
char szCmpFile[MAX_PATH];

	while(qi_slide != NULL) {

		sprintf(szCmpFile, "%dC.jpg", qi_slide->JobNumber);
		if(!strcmpi(szCmpFile, szFileName)) {
			ret = TRUE;
			break;
		}

		sprintf(szCmpFile, "%dW.jpg", qi_slide->JobNumber);
		if(!strcmpi(szCmpFile, szFileName)) {
			ret = TRUE;
			break;
		}

		qi_slide = qi_slide->next;
	}


	return ret;
}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
BOOL CController4App::FindPRNInQueue(char *szFileName) {
QUEUE_ITEM *qi_slide = q_item_top;
BOOL ret = FALSE;
char szCmpFile[MAX_PATH];

	while(qi_slide != NULL) {

		sprintf(szCmpFile, "%s\\%s", theApp.GVars->Incoming, szFileName);
		if(!strcmpi(szCmpFile, qi_slide->szFileName[0])) {
			ret = TRUE;
			break;
		}

		qi_slide = qi_slide->next;

	}

	return ret;
}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
QUEUE_ITEM *CController4App::FindJobByNumber(int number, int mode) { // mode 0 = return null if not found mode 1 = return top of queue if not found
QUEUE_ITEM *qi_slide = q_item_top;

	while(qi_slide != NULL) {
	
		if(qi_slide->JobNumber == number) return qi_slide;
	
		qi_slide = qi_slide->next;
	}
	
	if(mode == 0) return NULL;

	return q_item_top;

}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void CController4App::FindOrphans() {
WIN32_FIND_DATA  c_file;
HANDLE rc;
BOOL oktolook, foundsomething = FALSE;
char filters[2][MAX_PATH];
char tmp_buff2[MAX_PATH];
char filter[MAX_PATH];
int i;
PVOID OldValue = NULL;

	theApp.QueueCS.Lock();


	sprintf(filters[0], "%s\\ICONS", theApp.LaunchDir);
	sprintf(filters[1], "%s\\", theApp.GVars->Incoming);


	for(i = 0; i < 2; i++) {
	

		sprintf(filter, "%s\\*.*", filters[i]);

		Wow64DisableWow64FsRedirection(&OldValue);
		rc = FindFirstFile(filter, &c_file);
		Wow64RevertWow64FsRedirection(OldValue);

		if(rc != INVALID_HANDLE_VALUE) {
			oktolook = TRUE;
		}
		else {
			oktolook = FALSE;
		}

		while(oktolook) {

			if(strlen(c_file.cFileName) > 3) {
	
				foundsomething = TRUE;

				if(i == 0) {
					if(!FindICONInQueue(c_file.cFileName)) {
						// delete file here
						sprintf(tmp_buff2,"%s\\%s", filters[0], c_file.cFileName);
						DeleteFile(tmp_buff2);
					}
				}
				if(i == 1) {
					if(!FindPRNInQueue(c_file.cFileName)) {
						sprintf(tmp_buff2,"%s\\%s",theApp.GVars->Incoming, c_file.cFileName);
						DeleteFile(tmp_buff2);
					}
				}


			}
			Wow64DisableWow64FsRedirection(&OldValue);
			oktolook = FindNextFile(rc, &c_file);
			Wow64RevertWow64FsRedirection(OldValue);
		}
		FindClose(rc);


	}

	theApp.QueueCS.Unlock();

	if(foundsomething) {
		WriteQueue();
	}

}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void CController4App::ReadQueue() {
char theDir[MAX_PATH], szFileName[MAX_PATH];
HANDLE fh;
int nRet;

	strcpy(theDir, theApp.LaunchDir);
	strcat(theDir, "\\");
	strcat(theDir, QUEUE_FILE);

	QueueItems = 0;
	q_item_top = NULL;


	fh = CreateFile(theDir,
					GENERIC_READ,
					FILE_SHARE_READ,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);

	if(fh != INVALID_HANDLE_VALUE) {

		theApp.QueueCS.Lock();

		unsigned long bytes;

		QUEUE_ITEM newitem, *qi;

		while(1) {
			
			ReadFile(fh, (char *)&newitem, sizeof(QUEUE_ITEM), &bytes, NULL);

			if(bytes < sizeof(QUEUE_ITEM))
				break;

			qi = (QUEUE_ITEM *)calloc(1, sizeof(QUEUE_ITEM));
			memcpy((char *)qi, (char *)&newitem, sizeof(QUEUE_ITEM));

			if(qi->version != QUEUE_ITEM_VERSION) {
				free(qi);
				q_item_top = NULL;
				QueueItems = 0;
				break;
			}

			qi->next = NULL;
			qi->previous = NULL;

			if(q_item_top == NULL) {
				q_item_top = qi;
			}

			else {
		
				QUEUE_ITEM *sld = q_item_top;
	
				while(sld->next != NULL)
					sld = sld->next;

				sld->next = qi;

				qi->previous = sld;

			}

			sprintf(szFileName, "ICONS\\%d%c.jpg", qi->JobNumber, qi->bIsWhite?'W':'C');
			nRet= L_LoadBitmap(szFileName, &theApp.Bitmap[0], sizeof(BITMAPHANDLE), 32, ORDER_BGR, NULL, NULL);
			if(nRet == 1) {
				qi->iconindex = CreatePageIconImage(qi, &GS, &theApp.Bitmap[0], 1);
				QueueItems++;
			}
		}

		theApp.QueueCS.Unlock();

		CloseHandle(fh);

		FindOrphans();

	}
}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
void CController4App::WriteQueue() {
char theDir[MAX_PATH];
HANDLE fh;
int i;


	strcpy(theDir, theApp.LaunchDir);
	strcat(theDir, "\\");
	strcat(theDir, QUEUE_FILE);

	DeleteFile(theDir);

	fh = CreateFile(theDir,
					GENERIC_WRITE,
					FILE_SHARE_READ|FILE_SHARE_WRITE,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);

	if(fh != INVALID_HANDLE_VALUE) {

		theApp.QueueCS.Lock();

		unsigned long bytes;

		QUEUE_ITEM *qi = q_item_top;

		for(i = 0; i < QueueItems; i++) {
			WriteFile(fh, (char *)qi, sizeof(QUEUE_ITEM), &bytes, NULL);
			qi = qi->next;
		}

		theApp.QueueCS.Unlock();

		CloseHandle(fh);
	}
}


void CController4Dlg4::OnBnClickedDelete() {
int i, x, jn;
LV_ITEM lvi;
QUEUE_ITEM *qiptr, *tq;
BOOL OldHalt= theApp.HaltJobScanner;

	/**/ TRACE("HaltEvent 1491\n");
	theApp.HaltJobScanner = TRUE;

	theApp.QueueCS.Lock();

	theApp.QueueCS.Unlock();


	x = m_JobList.GetItemCount();

	for(i = 0; i < x; i++) {
		lvi.mask=LVIF_STATE;
		lvi.iItem=i;
		lvi.iSubItem=0;
		lvi.stateMask=0xFFFF;		// get all state flags
		m_JobList.GetItem(&lvi);
		
		if(lvi.state & LVIS_SELECTED) {

			qiptr = (QUEUE_ITEM *)m_JobList.GetItemData(i);

			if(!QueueItemStillValid(qiptr)) {
				ReconcileIcons();
				theApp.ReadQueue();
				DisplayQueue();
				/**/ TRACE("HaltEvent 1516\n");
				theApp.HaltJobScanner = OldHalt;
				return;
			}
			
			if(qiptr)
				jn = qiptr->JobNumber;

			QUEUE_ITEM *other_qi = NULL;

			if(qiptr->bIsWhite) other_qi = FindOtherLayer(qiptr, FIND_COLOR);
			else other_qi = FindOtherLayer(qiptr, FIND_WHITE);
			
			DeleteQueueItem(qiptr);

			m_JobList.SetItemData(i, 0);

			if(other_qi != NULL) DeleteQueueItem(other_qi);

			if(theApp.GVars->CurrentJobNumber == jn) {
				if(theApp.q_item_top == NULL) {
					theApp.GVars->CurrentJobNumber = 0;
				}
				else {
					theApp.GVars->CurrentJobNumber = theApp.q_item_top->JobNumber;
				}
			}
		}
	}
	theApp.WriteQueue();
	ReconcileIcons();

	theApp.ReadQueue();
	DisplayQueue();

	/**/ TRACE("HaltEvent 1551\n");
	theApp.HaltJobScanner = OldHalt;
	
}


void CController4Dlg4::OnLButtonDblClk(UINT nFlags, CPoint point) {
int i, x;
LV_ITEM lvi;
QUEUE_ITEM *qiptr;

	x = m_JobList.GetItemCount();

	for(i = 0; i < x; i++) {
		lvi.mask=LVIF_STATE;
		lvi.iItem=i;
		lvi.iSubItem=0;
		lvi.stateMask=0xFFFF;		// get all state flags
		m_JobList.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED) {
			qiptr = (QUEUE_ITEM *)m_JobList.GetItemData(i);
			DisplayQueueItem(qiptr);
			break;
		}
	}


	CDialog::OnLButtonDblClk(nFlags, point);
}


/*-----------------------------------------------------------------------------------------------------*/
/*-  Create a new queue item and stick it in the linked list                                          -*/
/*-----------------------------------------------------------------------------------------------------*/
QUEUE_ITEM *CController4Dlg1::CreateNewQueueItem(char *szFileName) {
QUEUE_ITEM *qi;
int c;

	qi = (QUEUE_ITEM *)calloc(1, sizeof(QUEUE_ITEM));

	qi->version = QUEUE_ITEM_VERSION;
	strcpy(qi->JobName, GS.JobName);
	qi->JobNumber = GS.JobNumber;
	qi->Flags = 0x01;   /// <===== fixme
	qi->printarea.x = GS.PaperDimensionX;
	qi->printarea.y = GS.PaperDimensionY;
	qi->resolution.x = (float)GS.ResolutionX;
	qi->resolution.y = (float)GS.ResolutionY;
//	strcpy(qi->szSourceFile, szCurrentFile);
	strcpy(qi->szFileName[0], szFileName);
	strcpy(qi->szFileName[1], "filename1");  /// <===== fixme
	strcpy(qi->szFileName[2], "filename2");  /// <===== fixme

	for(c = 0; c < 8; c++) {
		sprintf(qi->szPreview[c], "%s\\%d-%d.tif", theApp.GVars->Thumbnails, GS.JobNumber, c);
	}
	sprintf(qi->szPreview[8], "%s\\%d-w.tif", theApp.GVars->Thumbnails, GS.JobNumber);
	sprintf(qi->szPreview[9], "%s\\%d-c.tif", theApp.GVars->Thumbnails, GS.JobNumber);


	theApp.QueueCS.Lock();

	if(theApp.q_item_top == NULL) {
		theApp.q_item_top = qi;
	}

	else {

		theApp.q_item_top->previous = qi;
		qi->next = theApp.q_item_top;
		theApp.q_item_top = qi;
		
		/*
		QUEUE_ITEM *sld = theApp.q_item_top;
	
		while(sld->next != NULL)
			sld = sld->next;

		sld->next = qi;

		qi->previous = sld;
		*/
	}

	theApp.QueueItems++;

	// theJobDlg->CheckForJobOverflow();

	theApp.QueueCS.Unlock();

	theApp.WriteQueue();

	theApp.processing_qitem = qi;


	// DisplayQueue();

	return qi;


}

void CController4Dlg4::ProtectItems() {
int i, x;
LV_ITEM lvi;
QUEUE_ITEM *qiptr;

	x = m_JobList.GetItemCount();

	for(i = 0; i < x; i++) {
		lvi.mask=LVIF_STATE;
		lvi.iItem=i;
		lvi.iSubItem=0;
		lvi.stateMask=0xFFFF;		// get all state flags
		m_JobList.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED) {
			qiptr = (QUEUE_ITEM *)m_JobList.GetItemData(i);
			qiptr->bIsWhite |= Q_ITEM_LOCKED;
		}
	}
	theApp.WriteQueue();
	//DisplayQueue();

}

void CController4Dlg4::ProtectItem() {
int i, x;
LV_ITEM lvi;
QUEUE_ITEM *qiptr;

	x = m_JobList.GetItemCount();

	theApp.QueueCS.Lock();

	for(i = 0; i < x; i++) {
		lvi.mask=LVIF_STATE;
		lvi.iItem=i;
		lvi.iSubItem=0;
		lvi.stateMask=0xFFFF;		// get all state flags
		m_JobList.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED) {
			qiptr = (QUEUE_ITEM *)m_JobList.GetItemData(i);

			if(qiptr->Flags & Q_ITEM_LOCKED) {
				qiptr->Flags ^= Q_ITEM_LOCKED;
				m_Protect.LoadBitmaps("LOCK_CLOSEDU", "LOCK_CLOSEDD", "LOCK_CLOSEDF", NULL);
				m_Protect.Invalidate(TRUE);
				UpdateLockIcon(qiptr);

			}
			else {
			
				if(ProtectedItems() >= theApp.GVars->MaxJobs - 2) {
					MessageBox("TOO MANY PROTECTED ITEMS", APP_NAME, MB_OK|MB_SYSTEMMODAL);
					break;
				}

				qiptr->Flags |= Q_ITEM_LOCKED;
				m_Protect.LoadBitmaps("LOCK_OPENU", "LOCK_OPEND", "LOCK_OPENF", NULL);
				m_Protect.Invalidate(TRUE);
				UpdateLockIcon(qiptr);
			}
			break;
		}

	}

	theApp.QueueCS.Unlock();

	theApp.WriteQueue();


}

int CController4Dlg4::ProtectedItems() {
int i, x, items = 0;
QUEUE_ITEM *qiptr;

	x = m_JobList.GetItemCount();

	for(i = 0; i < x; i++) {
		qiptr = (QUEUE_ITEM *)m_JobList.GetItemData(i);
		if(qiptr->Flags & Q_ITEM_LOCKED) {
			items++;
		}

	}

	return(items);

}

int CountQueueItems( ) {
QUEUE_ITEM *slide = theApp.q_item_top, *other_qi;
int count = 0;


	while(slide != NULL) {

		if(slide->bIsWhite) other_qi = FindOtherLayer(slide, FIND_COLOR);
		else other_qi = NULL;

		if(other_qi == NULL) count++;

		slide = slide->next;
	}


	return count;
}

int CountPrintedQueueItems(int mode) {
QUEUE_ITEM *slide = theApp.q_item_top, *other_qi;
int count = 0;

	while(slide != NULL) {

		if(slide->bIsWhite) other_qi = FindOtherLayer(slide, FIND_COLOR);
		else other_qi = NULL;

		if(other_qi == NULL) {
			if(mode == COUNT_MODE_PRINTED && slide->iCount > 0) count++;
			if(mode == COUNT_MODE_NOT_PRINTED && slide->iCount < 1) count++;
		}
		else {
			if(mode == COUNT_MODE_PRINTED && slide->iCount > 0 && other_qi->iCount > 0) count++;
			if(mode == COUNT_MODE_NOT_PRINTED && slide->iCount < 1 || other_qi->iCount < 1) count++;
		}

		slide = slide->next;
	}

	return count;

}


void DeleteQueueItem(QUEUE_ITEM *q) {
int i, l;
QUEUE_ITEM *qi = theApp.q_item_top, *aux_qi;
BOOL bDeletedDisplayed = FALSE, bIsCurrentJob = FALSE;
char tmp_buff[128];
BOOL OldHalt = theApp.HaltJobScanner;

	// check for type of job here (don't delete manually inserted ones?)
	if(strlen(q->szFileName[0]))
		DeleteFile(q->szFileName[0]);


	sprintf(tmp_buff, "ICONS\\%dC.jpg", q->JobNumber);
	DeleteFile(tmp_buff);

	sprintf(tmp_buff, "ICONS\\%dW.jpg", q->JobNumber);
	DeleteFile(tmp_buff);

	for(i = 0; i < theApp.QueueItems; i++) {
	
		if(qi == q) {

			if(q->previous != NULL) {
				q->previous->next = q->next;
			}
			if(q->next != NULL) {
				q->next->previous = q->previous;
			}
			if(q == theApp.q_item_top)
				theApp.q_item_top = theApp.q_item_top->next;

			theApp.QueueItems--;

			if(q == theApp.qitem) {
				bDeletedDisplayed = TRUE;
			}
			theApp.m_QueueImages.Remove(q->iconindex);
			free(q);
			break;
		}
		qi = qi->next;
	}
	
	//TRACE("CurJobCS lock 8\n");
	//theApp.CurJobCS.Lock(INFINITE);

	if(theApp.CurrentJob == q) {

		theApp.CurrentJob = theApp.q_item_top;
		if(theApp.CurrentJob != NULL && QueueItemStillValid((QUEUE_ITEM *)theApp.CurrentJob)) {
			theApp.GVars->CurrentJobNumber = ((QUEUE_ITEM *)theApp.CurrentJob)->JobNumber;
		}
		else {
			theApp.GVars->CurrentJobNumber = 0;
		}
		theDlg->InvalidateRect(&theDlg->ButtonArea[CURRENT_COUNTER_BUTTON].slice[0]);
		
		theDlg->ForcePreviewRefresh(FALSE);

	}

	//theApp.CurJobCS.Unlock();

	theApp.QueueCS.Unlock();

	theApp.WriteQueue();

}

/// Delete oldest printer job in queue
BOOL DeleteOldestPrinted() {
QUEUE_ITEM *slide, *other_qi = NULL;

	theApp.OverflowCS.Lock();

	slide = theApp.q_item_top;

	while(slide != NULL) {
		
		other_qi = NULL;

		if(slide->iCount && slide->iProdMode == PRODMODE_SINGLE) {

			if(slide->bIsWhite) other_qi = FindOtherLayer(slide, FIND_COLOR);
			else other_qi = FindOtherLayer(slide, FIND_WHITE);

			DeleteQueueItem(slide);

			if(other_qi != NULL) {
				DeleteQueueItem(other_qi);
			}

			if(theApp.currentWindow == JOBS_WINDOW) {
				CController4Dlg4 *tdlg = (CController4Dlg4 *)theApp.QueueWND;
				tdlg->ReconcileIcons();
				theApp.ReadQueue();
				//tdlg->DisplayQueue();
				theApp.QueueRedrawRequest = TRUE;
			}
			else {
				CController4Dlg4 tdlg;
				tdlg.ReconcileIcons();
				theApp.ReadQueue();
				/*if(theApp.QueueWND != NULL) {
					Sleep(200);
					Sleep(0);
					((CController4Dlg4 *)theApp.QueueWND)->DisplayQueue();
				}*/
			}
			theApp.OverflowCS.Unlock();

			//TRACE("CurJobCS lock 9\n");
			//theApp.CurJobCS.Lock(INFINITE);

			if(theApp.CurrentJob == slide) {
				theApp.CurrentJob = theApp.q_item_top;
			}

			//theApp.CurJobCS.Unlock( );

			return TRUE;
		}

		slide = slide->next;
	}



	theApp.OverflowCS.Unlock();

	return FALSE;


}


/// See if it's ok to add another job
BOOL OKToAddJob() {
QUEUE_ITEM *slide, *other_qi = NULL;
BOOL retval = FALSE;
int printeditems = 0, total_items = 0;


	theApp.OverflowCS.Lock();

	slide = theApp.q_item_top;


	if(CountQueueItems() < theApp.GVars->MaxJobs) {
		theApp.OverflowCS.Unlock();
		return TRUE;
	}

	while(slide != NULL) {

		total_items++;

		if(slide->iCount && slide->iProdMode == PRODMODE_SINGLE)
			printeditems++;

		slide = slide->next;

	}

	if((total_items - printeditems) < theApp.GVars->MaxJobs)
		retval = TRUE;


	theApp.OverflowCS.Unlock();

	return retval;

}


/// Deletes any overflow jobs
BOOL CController4Dlg4::CheckForJobOverflow() {
QUEUE_ITEM *slide, *other_qi = NULL;
BOOL retval = TRUE, somethingdone = FALSE;

	theApp.OverflowCS.Lock();

lafov:
	
	slide = theApp.q_item_top;

	if(CountQueueItems() /*theApp.QueueItems*/ > theApp.GVars->MaxJobs) {
	
		// first, find the last job

		while(slide->next != NULL)
			slide = slide->next;

		// now back up until you find the first unlocked job
		while(slide != NULL && (slide->Flags & Q_ITEM_LOCKED)) {
			slide = slide->previous;
		}

		if(slide != NULL) {

			if(theApp.GVars->CurrentJobNumber == slide->JobNumber) {
				if(theApp.q_item_top == NULL)
					theApp.GVars->CurrentJobNumber = 0;
				else
					theApp.GVars->CurrentJobNumber = theApp.q_item_top->JobNumber;
				
				if(theApp.currentWindow == MAIN_RUNNING_WINDOW) {
					//theDlg->ForcePreviewRefresh(FALSE);
					theDlg->ForcePreviewRefresh(TRUE);
				}
			}

			QUEUE_ITEM *other_qi = NULL;

			if(slide->bIsWhite) other_qi = FindOtherLayer(slide, FIND_COLOR);
			else other_qi = FindOtherLayer(slide, FIND_WHITE);

			BOOL bTwoLayerJob = (other_qi != NULL);
			BOOL bOKToDelete = TRUE;

			if(bTwoLayerJob) {
				if(!slide->iCount || !other_qi->iCount) bOKToDelete = FALSE;
			}
			else {
				if(!slide->iCount) bOKToDelete = FALSE;
			}

			if(bOKToDelete) {

				DeleteQueueItem(slide);
				if(other_qi != NULL) DeleteQueueItem(other_qi);

				theApp.WriteQueue();
				ReconcileIcons();
				theApp.ReadQueue();
				if(theApp.QueueWND != NULL) {
					Sleep(200);
					Sleep(0);
					DisplayQueue();
				}

				// do it again
				goto lafov;
			}

		}

		else {
			// no unlocked items... leaving

			theApp.OverflowCS.Unlock();
			if(theApp.currentWindow == MAIN_RUNNING_WINDOW) {
				//theDlg->ForcePreviewRefresh(FALSE);
				theDlg->ForcePreviewRefresh(TRUE);
			}

			goto leave_check_overflow;
		}

		if(theApp.currentWindow == MAIN_RUNNING_WINDOW) {
			//theDlg->ForcePreviewRefresh(FALSE);
			theDlg->ForcePreviewRefresh(TRUE);
		}


	}

	if(theApp.currentWindow == MAIN_RUNNING_WINDOW) {
//		theDlg->ForcePreviewRefresh(FALSE);
		theDlg->ForcePreviewRefresh(TRUE);
	}

	theApp.OverflowCS.Unlock();

leave_check_overflow:
	
	int actual_jobs = CountQueueItems();

	retval = actual_jobs < theApp.GVars->MaxJobs || actual_jobs == 0;

	return retval;

}


void CController4Dlg4::ReconcileIcons() {
QUEUE_ITEM	*qiptr = theApp.q_item_top, *tq;
int i;

	theApp.m_QueueImages.Remove(0);

	for(i = 1; i < theApp.QueueItems; i++) {
		tq = qiptr->next;
		theApp.m_QueueImages.Remove(qiptr->iconindex);
		free(qiptr);
		qiptr = tq;
	}

	theApp.q_item_top = NULL;
	theApp.QueueItems = 0;

}

QUEUE_ITEM *FindOtherLayer(QUEUE_ITEM *item, int mode) {
QUEUE_ITEM *slide = theApp.q_item_top;

	while(slide != NULL) {

		if(slide != item && slide->RipJobNumber == item->RipJobNumber) {

			if(mode == FIND_ANY) {
				break;
			}

			if(mode == FIND_COLOR) {
				if(!slide->bIsWhite)
					break;
			}
			if(mode == FIND_WHITE) {
				if(slide->bIsWhite)
					break;
			}

		}

		slide = slide->next;
	}


	return slide;
}

BOOL QueueItemStillValid(QUEUE_ITEM *item) {
QUEUE_ITEM *slide = theApp.q_item_top;

	while(slide != NULL) {

		if(slide == item) {
			return TRUE;
		}

		slide = slide->next;
	}


	return FALSE;
}

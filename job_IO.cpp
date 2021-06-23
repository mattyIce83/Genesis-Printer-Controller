//  Job_IO.cpp
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "ftd2xx.h"
#include "ftdisupp.h"
#include <time.h>

#include "parser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define JOB_STATE_IDLE			0
#define JOB_STATE_RECEIVING		1
#define JOB_STATE_ERROR			2
#define JOB_STATE_FLUSHING		3
#define JOB_STAT_TIMEOUT		4
#define JOB_STAT_CLOSING		5

extern CController4Dlg1 *theDlg;

extern BOOL FTDWrite(FTDIComm *, unsigned char *, unsigned long);
extern FTDIComm		FPGAusb;
extern FTDIComm		JOBusb;
extern void			*ESCOpenParser();
extern BOOL			ESCParser(ESCP2Filter *e, unsigned char *, int);
extern void			OutputBits(int, unsigned char *, int);
extern displayGraphicState GS;

extern void SetGEOJ(BOOL);

extern BOOL EOJ_Set;

extern int GbandNo;

/**/ #define ENABLE_TIMEOUT

#define RECEIVE_TIMEOUT_TICKS		30         // (in seconds) this should be a variable FIXME

/*-------------------------------------------------------------------*/
/*-                                                                 -*/
/*-------------------------------------------------------------------*/
void JobIOThread(void *v) {
FTDIComm *u;
unsigned long bytesread = 0;
unsigned long state = JOB_STATE_IDLE;
unsigned char tmp_read[USB_READ_CHUNKS];
unsigned long read_bytes = 0;
unsigned long disk_bytes;
BOOL OkToRead = TRUE;
FTDIComQueueItem *qitem, *tmpqi;
DWORD RxBytes, EventMask, ret;
HANDLE hEvent;
int rlen;
CWinThread *ParserThread;
ESCP2Filter *filter;
char szFileName[MAX_PATH];
HANDLE fh;
BOOL parsing = FALSE;
time_t	timeout, new_time, check_time;
int jobbytes = 0;

	theDlg->StatusLine("Starting USB Job parser thread\n");

	SECURITY_ATTRIBUTES ThreadSA;

	ThreadSA.nLength = sizeof(SECURITY_ATTRIBUTES);
	ThreadSA.bInheritHandle = TRUE;
	ThreadSA.lpSecurityDescriptor  = NULL;

	// wait for everything else to come up.
	Sleep(2000);

	filter = (ESCP2Filter *)ESCOpenParser();

	theDlg->StatusLine("USB Job parser thread started\n");

	while(!JOBusb.quit_threads) {

		switch(state) {

			case JOB_STATE_IDLE:

				if(JOBusb.read_queue_items) { /// There's something in the read queue
					
					theDlg->StatusLine("USB Job Detected\n");

					jobbytes = 0;

					GbandNo = 0;

					sprintf(szFileName, "%s\\USBJOB%d.PRN", theApp.GVars->Incoming, theApp.GetJobNumber(1));
					strcpy(GS.szCurrentFile, szFileName);
					theDlg->bIsAReprint = FALSE;

					fh = CreateFile(	szFileName,
										GENERIC_WRITE,
										FILE_SHARE_READ|FILE_SHARE_WRITE,
										(LPSECURITY_ATTRIBUTES)NULL,
										OPEN_ALWAYS,
										FILE_ATTRIBUTE_NORMAL,
										(HANDLE)NULL);

					if(theApp.GMasterStatus ==	MASTER_STATUS_IDLE) {
						parsing = TRUE;
					}
					else {
						parsing = FALSE;
					}
					state = JOB_STATE_RECEIVING;
					if(theApp.GMasterStatus != MASTER_STATUS_IMPORTING)
						theApp.bInJob = TRUE;
					theDlg->StatusLine("Resuming Formatter Thread\n");
					theApp.HeadFormatterThread->ResumeThread();
					time(&new_time);
					timeout = new_time + RECEIVE_TIMEOUT_TICKS;

				}
				else Sleep(50);
				break;

			case JOB_STATE_RECEIVING:

				if(EOJ_Set) {

					CloseHandle(fh);

					// flush whatever is left in the pipeline
					while(JOBusb.read_queue_items){
						//FTDRead(&JOBusb, tmp_read, &read_bytes);
						//TRACE("Flusing %d bytes\n", read_bytes);
						Sleep(500);
					}


					EOJ_Set = FALSE;

					OutputBits(0, NULL, 0);
					
					/**/ TRACE("Setting GMasterStatus to idle (148)\n");
					theApp.GMasterStatus =	MASTER_STATUS_IDLE;
					state = JOB_STATE_IDLE;
					filter->state = searching;
					filter->firstBuff = TRUE;
					// TRACE("Total job bytes: %d\n", jobbytes);

					break;
				}

				//if(!FTDRead(&JOBusb, tmp_read, &read_bytes)) {
					// read error warning here    FIXME
				//}
				
				if(read_bytes) {
					
					jobbytes += read_bytes;

					WriteFile(fh, tmp_read, read_bytes, &disk_bytes, NULL);

					if(parsing) {
						ESCParser(filter, tmp_read, read_bytes);
					}

					time(&new_time);
					timeout = new_time + RECEIVE_TIMEOUT_TICKS;
				}
				else {
					time(&check_time);
#ifdef ENABLE_TIMEOUT
					if(check_time >= timeout && !JOBusb.read_queue_items) {
						theDlg->StatusLine("RECEIVE TIMEOUT\n");
						EOJ_Set = 1;
						SetGEOJ(TRUE);
						break;
					}
#endif

				}
				break;

			case JOB_STATE_ERROR:
				break;
			case JOB_STATE_FLUSHING:
				break;
			case JOB_STAT_TIMEOUT:
				break;
			case JOB_STAT_CLOSING:
				break;

		}


	}


}
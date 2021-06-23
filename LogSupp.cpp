
// Controller4.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "parser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern displayGraphicState GS;

//extern CController4Dlg *theDlg;
extern CController4Dlg1 *theDlg;

extern void LogAction(char *);

#ifndef USE_MOE_LOGGING
/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
BOOL CController4App::LogJobInfo(int evnt, QUEUE_ITEM *job, void *g_s) {

	if(!GVars->bNoLogging) {
#ifdef LOG_LOGGING
		char tmp_buff[128];
		sprintf(tmp_buff, "Logging job %d [%d]", job->JobNumber, evnt);
		LogAction(tmp_buff);
#endif
		bNewLogEntries = LogSupp.LogJobInfo(evnt, job, g_s);
	}
	else {
		LogAction("Logging disabled, not logging");
	}
	return TRUE;

}
#endif

#ifndef USE_MOE_LOGGING
/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
BOOL CController4App::UploadLog( ) {
BOOL retry = FALSE;

rt_upl:

	if(!ConnectedToLogServer) {
		ConnectToLogServer(theApp.GVars->szLogIP, theApp.GVars->LogPort);
	}

	if(ConnectedToLogServer) {
		if(!LogSupp.UploadLog()) {
			if(!retry) {
				retry = TRUE;
				ConnectedToLogServer = FALSE;
				goto rt_upl;
			}
		}
		bNewLogEntries = 0;
	}

	return TRUE;

}

/*----------------------------------------------------------------*/
/*-                                                              -*/
/*----------------------------------------------------------------*/
BOOL CController4App::ConnectToLogServer(char *szIP, int Port) {

	ConnectedToLogServer = NtwrkObj.ConnectToLogServer(theApp.GVars->szLogIP, Port);

	return(ConnectedToLogServer);
}
#endif

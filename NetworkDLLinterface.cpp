//#include "Controller4.h"
#include "stdafx.h"
//#include <stdio.h>
//#include <cstdio>

#include "genesis_network_dll.h"
#include "NetworkDLLtemplate.h"
#include "Controller4.h"

CDLLLoader<Cgenesis_network> dll_loader;
// Using the DLL is a set of pure method calls through this interface pointer
Cgenesis_network* network_dll_interface;

//extern void LogAction(char *);   /// improper location for externs, copy his method for now

#ifdef USE_MOE_LOGGING
/*--------------------------------------------------------------*/
/*-                                                            -*/
/*--------------------------------------------------------------*/
BOOL CNetworkDLLinterface::FileExists(char *file) {
WIN32_FIND_DATA  c_file;
HANDLE rc;
PVOID OldValue = NULL;

	Wow64DisableWow64FsRedirection(&OldValue);
	rc = FindFirstFile(file, &c_file);
	Wow64RevertWow64FsRedirection(OldValue);

	if(rc != INVALID_HANDLE_VALUE) {
		FindClose(rc);
		return TRUE;
	}
	return FALSE;

}
#endif

#ifdef USE_MOE_LOGGING
void CNetworkDLLinterface::FixString(char *s)
{
int i, l = strlen(s);

	for(i = 0; i < l; i++)
	{
		if(	s[i] == '\"' ||	s[i] == ',' || s[i] == 0x0A || s[i] == 0x0D)
		{
			s[i] = '_';
		}
	}
}
#endif

/*-----------------------------------------------------------------------------------*/
/*- This legacy ASCII file is just for local use (used by anyone?), not for sending -*/
/*-----------------------------------------------------------------------------------*/
#ifdef USE_MOE_LOGGING
void CNetworkDLLinterface::WriteRecord(log_record *rec)
{
#else
void CLogger::WriteRecord(log_record *rec) {
#endif
char szFileName[MAX_PATH];
BOOL bNewFile;
FILE *logfile;
SYSTEMTIME time;
	
	GetLocalTime(&time);
	
	sprintf(szFileName, "LOGS\\BQGENLOG_%d_%02d_%02d.LOG", time.wYear, time.wMonth, time.wDay);

	bNewFile = !FileExists(szFileName);

	logfile = fopen(szFileName, "a+");

	if(logfile == NULL) {
		logfile = fopen(szFileName, "w");
	}


	rec->log_version = LOG_VERSION;

	if(logfile != NULL) {

		/// make the first line of a new file the "field name record"
		if(bNewFile) {
			fprintf(logfile, "Version, Event, jobName, sizeX, sizeY, resolutionX, resolutionY, layerType, printMode, inkUsageC, inkUsageM, inkUsageY, inkUsageK, inkUsageW, jobNumber, completionCode, startTime, endTime, imageAreaX, imageAreaY\n");
			fflush(logfile);
		}

		fprintf(logfile, "%d,%d,\"%s\",%d,%d, %d,%d, %d,%d, %d,%d,%d,%d,%d, %d,%d, %lld,%lld, %f,%f\n",
			rec->log_version, rec->evnt, rec->jobName,
			rec->sizeX,	rec->sizeY,	rec->resolutionX, rec->resolutionY,	rec->layerType,	rec->printMode,
			rec->inkUsageC,	rec->inkUsageM,	rec->inkUsageY,	rec->inkUsageK,	rec->inkUsageW,
			rec->jobNumber,	rec->completionCode, rec->startTime, rec->endTime, rec->imageAreaX,	rec->imageAreaY);

		fflush(logfile);
		fclose(logfile);

	}

}

#ifdef USE_MOE_LOGGING
BOOL CNetworkDLLinterface::LogJobInfo(int evnt, QUEUE_ITEM *job, displayGraphicState *gs)
{
time_t t;


	if(theApp.GVars->bNoLogging)
	{
//#ifdef LOG_LOGGING
//		LogAction("Logging disabled, not logging");
//#endif
	}
	else
	{
		if (evnt == EVCD_JOB_START)
		{
			//time(&start_time);
			time(&t);
			network_dll_interface->jobLog.startTime = (unsigned long long)t; //start_time; // MOE - only thing different on start, save it locally for end
			network_dll_interface->LogJobInfo(evnt);  /// always returns TRUE right now
		}
		else if (evnt == EVCD_JOB_END)
		{
//#ifdef LOG_LOGGING
//			char tmp_buff[128];
//			sprintf(tmp_buff, "Logging job %d [%d]", job->JobNumber, evnt);
//			LogAction(tmp_buff);
//#endif

			// Populate the shared log_record structure here with data local to theApp
			network_dll_interface->jobLog.log_version = LOG_VERSION;
			network_dll_interface->jobLog.evnt = evnt;
			strcpy((char *)network_dll_interface->jobLog.jobName,job->JobName);
			FixString((char *)network_dll_interface->jobLog.jobName);
			network_dll_interface->jobLog.sizeX = gs->PageUnitsX;
			network_dll_interface->jobLog.sizeY = gs->PageUnitsY;
			network_dll_interface->jobLog.resolutionX = job->resolution.x;
			network_dll_interface->jobLog.resolutionY = job->resolution.y;
			network_dll_interface->jobLog.layerType = gs->layerType;
			network_dll_interface->jobLog.printMode = 0;                                            /// TODO Fred printMode

			network_dll_interface->jobLog.inkUsageC = theApp.GVars->TotalInk[0];
			network_dll_interface->jobLog.inkUsageM = theApp.GVars->TotalInk[1];
			network_dll_interface->jobLog.inkUsageY = theApp.GVars->TotalInk[2];
			network_dll_interface->jobLog.inkUsageK = theApp.GVars->TotalInk[3];
			network_dll_interface->jobLog.inkUsageW = theApp.GVars->TotalInk[4];

			network_dll_interface->jobLog.jobNumber = job->JobNumber;
			network_dll_interface->jobLog.completionCode = 0;                                       /// TODO Fred completionCode
			network_dll_interface->jobLog.imageAreaX = job->printarea.x;
			network_dll_interface->jobLog.imageAreaY = job->printarea.y;

			time(&t);
			network_dll_interface->jobLog.endTime = (unsigned long long)t; //start_time; // MOE - only thing different on start, save it locally for end


			///bNewLogEntries = network_dll_interface->LogJobInfo();  /// always returns TRUE right now
			network_dll_interface->LogJobInfo(evnt);  /// always returns TRUE right now

			WriteRecord(&network_dll_interface->jobLog); // For local comma delimited file
		}
		else return FALSE;
	}

	return TRUE;
}
#endif


#ifdef USE_MOE_LOGGING
/*-----------------------------------------------------------------------------------------------------*/
/*-  Initialization loop  -  thread for calling blocking IP functions                                 -*/
/*-----------------------------------------------------------------------------------------------------*/
int CNetworkDLLinterface::IPtaskHandler(void)
{
	while(!theApp.quit_threads) // TRUE = powering down, return from this function to quit thread
	{
		if(theApp.GMasterStatus == MASTER_STATUS_IDLE && network_dll_interface->IPtaskPending) //theApp.bNewLogEntries)
		{
			memset((char *)&message_buffer, 0, sizeof(message_buffer));

			network_dll_interface->ProcIPtask(message_buffer);  /// MOE - TODO  message_buffer, for now just msgbox

			if (strlen(message_buffer))
				MessageBox(GetFocus(), message_buffer, "IP thread", MB_SYSTEMMODAL|MB_OK);  // debug

			Sleep(2000);  // A little more time between tasks?
		}

		Sleep(1000);
	}

	return 0;
}
#endif

#ifdef USE_MOE_LOGGING
int CNetworkDLLinterface::LoadNetworkDLL(void) // loads DLL and gets the class pointer
{
	bNetworkDLLloaded = FALSE;

	network_dll_interface = dll_loader.GetInterface();

	// From now on using the DLL is a set of pure method calls through the interface pointer

	network_dll_interface->tester2 = 555; // test assign and read back of a data member

	if (network_dll_interface->TestInterface()) // this member function will verify the data member above
	{
		networkDLLversion = network_dll_interface->GetVersion();

		if (networkDLLversion != 0)
		{
			bNetworkDLLloaded = TRUE;

			// Pass INI file stuff
			network_dll_interface->IPAddy = theApp.GVars->IPAddy;
			network_dll_interface->Subnet = theApp.GVars->Subnet;
			network_dll_interface->Gateway = theApp.GVars->Gateway;
			network_dll_interface->AutoIP = theApp.GVars->AutoIP;
			network_dll_interface->DisplayedAdaptor = theApp.GVars->DisplayedAdaptor;

			strcpy(network_dll_interface->szLogURL, theApp.GVars->szLogURL);
			strcpy(network_dll_interface->szLogIP, theApp.GVars->szLogIP);
			strcpy(network_dll_interface->szFailSafeIP, theApp.GVars->szFailSafeIP);
			network_dll_interface->LogPort = theApp.GVars->LogPort;

			if (theApp.GVars->bNoLogging) network_dll_interface->bLoggingEnabled = FALSE;
			else network_dll_interface->bLoggingEnabled = TRUE;

			strcpy(network_dll_interface->szPrinterSerialNumber, theApp.GVars->szPrinterSerialNumber);
			strcpy(network_dll_interface->EncryptionKey, theApp.GVars->EncryptionKey);

			//BOOL bDebugNetwork;  /// MOE networking INI
			//strcpy(network_dll_interface->NetworkName, theApp.GVars->ServerName); not ready
			
			network_dll_interface->Init();
			
			return TRUE;
		}
		else return FALSE;
	}
	else return FALSE;
}
#endif
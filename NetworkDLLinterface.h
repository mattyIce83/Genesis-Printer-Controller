#pragma once

#ifndef NET_DLL_DEFINED
#define NET_DLL_DEFINED

//#include "graphics_state.h"
#include "globals.h"
#include "parser.h"

#include "log_support.h"

//extern Cgenesis_network* network_dll_interface;

class CNetworkDLLinterface : public CObject
{
public:
	BOOL bNetworkDLLloaded;  // FALSE = failed to load DLL
	int networkDLLversion;       // 0 = failed to load DLL
	BOOL ConnectedToLogServer;
	char message_buffer[256];

	//log_record jobLogRec;

	///void IPtaskHandler(void *v);
	///UINT __cdecl IPtaskHandler( LPVOID );
	int LoadNetworkDLL(void); // put in class?
	int IPtaskHandler(void);
	BOOL CNetworkDLLinterface::FileExists(char *);
	void FixString(char *);
	void WriteRecord(log_record *);
	BOOL LogJobInfo(int, QUEUE_ITEM *, displayGraphicState *);
	BOOL UploadLog(void);
	BOOL ConnectToLogServer(char *, int);


private:
	//HMODULE m_DLLHandle;
	//int* m_Interface;
};

///int GetNetworkDLLclassPointer(void); // put in class?



#endif //NET_DLL_DEFINED

//NOTES :

	//if(!theApp.NtwrkObj.GetHostIP(theApp.GVars->szLogURL, theApp.GVars->szLogIP)) {
	//	strcpy(theApp.GVars->szLogIP, theApp.GVars->szFailSafeIP);
	//}


//In DebugDiag.cpp :
//void CDebugDiag::OnBnClickedLog()
//{
//	theApp.GVars->bEventLog = m_Log.GetCheck();
//	if(theApp.GVars->bEventLog) {
//		OpenLogFile();
//	}
//	else {
//		CloseLogFile();
//	}
//}
//
//void CDebugDiag::OnBnClickedPicClearlog()
//{
//	ClearLogFile(theApp.GVars->bEventLog);
//}
//
//void CDebugDiag::OnBnClickedForceLog() {
//	//theApp.NtwrkObj.ConnectToLogServer(theApp.GVars->szLogIP, theApp.GVars->LogPort);
//
//	if(theApp.ConnectedToLogServer) {
//		MessageBox("ALREADY CONNECTED TO LOG SITE", "GENESIS CONTROLLER", MB_OK|MB_SYSTEMMODAL);
//		return;
//	}
//
//	if(theApp.ConnectToLogServer(theApp.GVars->szLogIP, theApp.GVars->LogPort)) {
//		MessageBox("CONNECTED TO LOG SITE!!", "GENESIS CONTROLLER", MB_OK|MB_SYSTEMMODAL);
//	}
//	else {
//		MessageBox("ERROR CONNECTING TO LOG SITE", "GENESIS CONTROLLER", MB_OK|MB_SYSTEMMODAL);
//	}
//}
//
//
//void CDebugDiag::OnBnClickedSendData() {
//	//theApp.NtwrkObj.UploadString("Hello World");
//	theApp.UploadLog();
//}

#pragma once

#include "log_support.h"

/// NextJobNumber !!! 0 to 9999  rips only?

#define VERSION  3

///	CIPFunctions NtwrkObj;

///	CImportSupp ImportSupp;

///	CLogger	LogSupp;

// DLL interface, this header is used by both the DLL and the module
// that wants to access the functionalities of the DLL.
class Cgenesis_network
{
public:
	virtual int TestInterface(void) = 0;
	virtual int GetVersion(void) = 0;

	virtual int Init(void) = 0;
	//virtual int Idle(void); //virtual BOOL UploadLog();
	virtual int Sleep(void) = 0;
	//virtual int Wake(void); // ?? Just wake on next event?

	virtual int ProcIPtask(char *) = 0;

	virtual int LogJobInfo(int) = 0;

	///virtual bool ConnectToLogServer(char *, int) = FALSE;
	///virtual bool DisconnectLogServer(void) = FALSE;

	//int FileExists(char *);

	unsigned short IPtaskPending;
	int tester2;
	log_record jobLog; // this can be moved to class or passed as a local later.
	//time_t start_time;
	//logpacket logpkt;

	BOOL ConnectedToLogServer;  // This could mean we have not tried yet or disconnected. Flag for Unable?
	BOOL UnableToConnect;       // This is a status flag that means I have tried and failed to connect.
	                            // At this time it will mean that it is probably not going to happen.
	                            // Alternative backup methods will come later.
	///int LogUploadInterval;
	///BOOL bNewLogEntries;

	/// MOE stuff read from INI file
	//DisplayedAdaptor=0
	//IPAddress=1
	//DefaultGateway=1
	//Subnet=1
	//AutoIP=1
	//LogURL=WEB-DEV
	//SerialNumber=GPM-A15-0005
	//EncryptionKey=DEADBEEFFEEFEEBO
	//SupressLogging =0
	//LogPort=8500
	//FailSafeIP=192.168.2.111

	unsigned long IPAddy;   /// Currently unused in INI, future use   IPAddress
	unsigned long Subnet;   /// Currently unused in INI, future use   Subnet
	unsigned long Gateway;  /// Currently unused in INI, future use   DefaultGateway
	BOOL AutoIP;            /// Currently unused in INI?? future use  AutoIP
	int DisplayedAdaptor;   /// Currently unused in INI, future use   DisplayedAdaptor=0

	char NetworkName[MAX_PATH];    /// MOE computer name of local hosting system
	char szLogURL[256];            // INI = LogURL
	char szLogIP[256];             // derived in software
	char szFailSafeIP[256];        // INI
	int LogPort;

	// Printer ID, currently in INI file
	unsigned int nPrinterSerialNumber;   // integer serial number, INTEGER PRIMARY KEY for dB
	char szPrinterSerialNumber[16];      // string serial number, should be forever tied to integer above
	char EncryptionKey[16];              // future use

	BOOL bLoggingEnabled;                // SupressLogging
	BOOL bDebugNetwork;  /// MOE networking INI

	//

};

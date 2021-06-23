// FTDI support system for DLP-USB1232H

#define USB_INITED_BYTE			0x69

//#define USB_OUTPUT_DEVICE_NAME			"DLP-USB1232H A"
#define USB_OUTPUT_DEVICE_NAME			"DLP-2232H A"
#define USB_AUX_DEVICE_NAME				"DLP-2232H B"


#define USB_INPUT_DEVICE_NAME	 		"JOB_DATA A"


#define MAX_USB_WRITE_BUFFER		11000000
//#define MAX_USB_READ_BUFFER		2000000
#define MAX_USB_READ_BUFFER		8000000
#define USB_READ_CHUNKS			100000

#define USB_READ_BUFFER_LENGTH	1024


typedef FT_STATUS (WINAPI *PtrToOpen)(PVOID, FT_HANDLE *); 
typedef FT_STATUS (WINAPI *PtrToOpenEx)(PVOID, DWORD, FT_HANDLE *); 
typedef FT_STATUS (WINAPI *PtrToListDevices)(PVOID, PVOID, DWORD);
typedef FT_STATUS (WINAPI *PtrToClose)(FT_HANDLE);
typedef FT_STATUS (WINAPI *PtrToRead)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
typedef FT_STATUS (WINAPI *PtrToWrite)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
typedef FT_STATUS (WINAPI *PtrToResetDevice)(FT_HANDLE);
typedef FT_STATUS (WINAPI *PtrToPurge)(FT_HANDLE, ULONG);
typedef FT_STATUS (WINAPI *PtrToSetTimeouts)(FT_HANDLE, ULONG, ULONG);
typedef FT_STATUS (WINAPI *PtrToGetQueueStatus)(FT_HANDLE, LPDWORD);
typedef FT_STATUS (WINAPI *PtrToGetStatus)(FT_HANDLE, LPDWORD, LPDWORD, LPDWORD);
typedef FT_STATUS (WINAPI *PtrToSetEventNotification)(FT_HANDLE, DWORD, PVOID);


typedef struct _FTDIComQueueItem {
	unsigned char type;
	unsigned int length;
	unsigned char *buffer;
	struct _FTDIComQueueItem *next;
} FTDIComQueueItem;

typedef struct _FTDIComm {

	int Inited;

	BOOL		bActive;

	char		szName[32];

	BOOL		bModuleConnected;

	FT_HANDLE m_ftHandle;

	FT_STATUS status;

	int write_buffer_length;
	int read_buffer_length;
	int read_buffer[USB_READ_BUFFER_LENGTH];

	int write_queue_items;
	FTDIComQueueItem *WriteQueueTop;

	int read_queue_items;
	FTDIComQueueItem *ReadQueueTop;

	BOOL quit_threads;

	SECURITY_ATTRIBUTES ThreadSA;
	CWinThread *hWriterThread;
	CWinThread *hReaderThread;

	HANDLE WriteEvent;
	HANDLE ReadEvent;

	CCriticalSection		WriteCS;
	CCriticalSection		ReadCS;

	HANDLE					hHeap;

} FTDIComm;


BOOL InitUSB(FTDIComm *, char *, BOOL);
void CloseUSB(FTDIComm *);
void ResetUSB(FTDIComm *);
BOOL FTDWrite(FTDIComm *, unsigned char *, unsigned long);


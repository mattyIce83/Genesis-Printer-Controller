// SerialPort.cpp : implementation file
//

#include "stdafx.h"
#include "SerialPort.h"
#include "globals.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////

// CSerialPort

CSerialPort::CSerialPort() {
}

CSerialPort::~CSerialPort() {
}


BEGIN_MESSAGE_MAP(CSerialPort, CWnd)
	//{{AFX_MSG_MAP(CSerialPort)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSerialPort message handlers

BOOL CSerialPort::OpenPort(CString pname) {
CString portname;

	portname = pname;
	
	
	bPortIsOpen = FALSE;

	hComm = CreateFile(	portname,
						GENERIC_READ | GENERIC_WRITE,
						0,
						0,
						OPEN_EXISTING,
						FILE_FLAG_NO_BUFFERING, //0,
						0);

	if(hComm==INVALID_HANDLE_VALUE) {

		TRACE("Error %d opening com port\n", GetLastError());

		return false;
	}

	else {

		bPortIsOpen = TRUE;
			
		return true;
	}
}

BOOL CSerialPort::ConfigureSerialPort(int port, DWORD BaudRate, BYTE ByteSize, DWORD fParity, BYTE Parity, BYTE StopBits) {

	
	if((m_bPortReady = GetCommState(hComm, &m_dcb))==0) {

		//theDlg->OKBox("GetCommState Error", NULL, NULL);
		
		MessageBox("GetCommState Error","Error",MB_OK+MB_ICONERROR);
		CloseHandle(hComm);
		bPortIsOpen = FALSE;
		return false;
	}

	m_dcb.BaudRate =BaudRate;
	m_dcb.ByteSize = ByteSize;
	m_dcb.Parity =Parity ;
	m_dcb.StopBits =StopBits;
	m_dcb.fBinary=TRUE;
	m_dcb.fParity=fParity;
	m_dcb.fOutX=false;
	m_dcb.fInX=false;
	m_dcb.fNull=false;
	m_dcb.fAbortOnError=TRUE;
	m_dcb.fOutxCtsFlow=FALSE;
	m_dcb.fOutxDsrFlow=false;
	m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
	m_dcb.fRtsControl = RTS_CONTROL_DISABLE;
	m_dcb.fDsrSensitivity=false;

	//if(port == JOB_IO_PORT) {
		m_dcb.fAbortOnError = FALSE;
	//}

	m_dcb.fRtsControl = RTS_CONTROL_DISABLE;
	m_dcb.fDsrSensitivity = false;
	m_dcb.fOutxCtsFlow = FALSE;
	m_dcb.fOutxDsrFlow = false;

	m_dcb.fOutxCtsFlow=false;
	m_dcb.wReserved1=0;

	m_bPortReady = SetCommState(hComm, &m_dcb);

	if(m_bPortReady ==0) {
		MessageBox("SetCommState Error","Error",MB_OK|MB_ICONERROR);
		//theDlg->OKBox("GetCommState Error", NULL, NULL);
		CloseHandle(hComm);
		bPortIsOpen = FALSE;
		return false;
	}
	return true;
}

BOOL CSerialPort::SetCommunicationTimeouts(DWORD ReadIntervalTimeout, DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant, DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant) {

	if((m_bPortReady = GetCommTimeouts (hComm, &m_CommTimeouts))==0)
		return false;

	m_CommTimeouts.ReadIntervalTimeout = ReadIntervalTimeout;
	m_CommTimeouts.ReadTotalTimeoutConstant =ReadTotalTimeoutConstant;
	m_CommTimeouts.ReadTotalTimeoutMultiplier =ReadTotalTimeoutMultiplier;
	m_CommTimeouts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant;
	m_CommTimeouts.WriteTotalTimeoutMultiplier =WriteTotalTimeoutMultiplier;
		
	m_bPortReady = SetCommTimeouts (hComm, &m_CommTimeouts);
	
	if(m_bPortReady ==0) {
		MessageBox("StCommTimeouts function failed","Com Port Error",MB_OK|MB_ICONERROR);
		//theDlg->OKBox("StCommTimeouts function failed", NULL, NULL);
		CloseHandle(hComm);
		bPortIsOpen = FALSE;
		return false;
	}
	return true;
}

BOOL CSerialPort::WriteByte(BYTE bybyte)
{
	iBytesWritten=0;
	if(WriteFile(hComm,&bybyte,1,&iBytesWritten,NULL)==0)
		return false;
	else return true;
}

int CSerialPort::WriteString(unsigned char *string, int len)
{
int c;


	for(c = 0; c < len; c++) {

		if(!WriteByte(string[c])) {
			int error = GetLastError();
			return c;
		}

		Sleep(10);
	}

	return len;

}

int CSerialPort::WriteStringLessDelay(unsigned char *string, int len)
{
int c;


	for(c = 0; c < len; c++) {

		if(!WriteByte(string[c])) {
			int error = GetLastError();
			return c;
		}

		Sleep(5);
	}

	return len;

}

int CSerialPort::WriteStringFast(unsigned char *string, int len) {
int c;

	for(c = 0; c < len; c++) {

		if(!WriteByte(string[c])) {
			int error = GetLastError();
			return c;
		}

	}

	return len;

}

int CSerialPort::WriteStringVeryFast(unsigned char *string, int len)
{
unsigned long iBytesWritten;

	WriteFile(hComm, string, len, &iBytesWritten, NULL);
	return iBytesWritten;

}

BOOL CSerialPort::ReadByte(BYTE	&resp)
{
BYTE rx;
resp=0;

DWORD dwBytesTransferred=0;

if (ReadFile (hComm, &rx, 1, &dwBytesTransferred, 0)){
			 if (dwBytesTransferred == 1){
				 resp=rx;
				 return true;}}
			  
	return false;
}

int CSerialPort::Flush() {
BYTE rx;
int bytes = 0;
DWORD dwBytesTransferred = 0;

	while(ReadFile (hComm, &rx, 1, &dwBytesTransferred, 0)){
		bytes++;
	}

	return bytes;
}

unsigned long CSerialPort::ReadString(unsigned char *dat, int len) {
DWORD dwBytesTransferred=0;

	ReadFile (hComm, dat, len, &dwBytesTransferred, NULL);
	return dwBytesTransferred;

}

BOOL CSerialPort::ReadUByte(unsigned char *byte) {
DWORD dwBytesTransferred=0;

	if (ReadFile (hComm, byte, 1, &dwBytesTransferred, 0)){
	
		if (dwBytesTransferred == 1){
			return true;
		}
	}
			  
	return false;
}


void CSerialPort::ClosePort() {
	
	if(bPortIsOpen)
		CloseHandle(hComm);
	
	bPortIsOpen = FALSE;

	
	return;
}

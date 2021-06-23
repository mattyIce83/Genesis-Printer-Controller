// DebugDiag.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "Controller4.h"
#include "DebugDiag.h"
#include "DebugDiag2.h"
#include "DebugDiag3.h"
#include "Controller4Dlg1.h"
#include "afxdialogex.h"
#include "TestBands.h"
#include "TestPatternSelector.h"
#include "YLinearization.h"
#include "ServoOffsets.h"
#include "PlatenControl.h"
#define INIKEYS_IS_EXTERN
#include "init_keys.h"
#include "INIEditor.h"
#include "UserPrompt2.h"

#include "parser.h"
#include "HEAD_structs.h"

// CDebugDiag dialog

extern CController4Dlg1 *theDlg;

extern void SendPlatenHeightOffset();
extern void OpenLogFile();
extern void CloseLogFile();
extern void LogAction(char *);
extern void ClearLogFile(BOOL);
extern void SendPlatenLevelingOffset();
extern void SendSpeeds();

extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;

extern BOOL LoadTestBand(int);

typedef struct __curve {
	unsigned long channel;
	float MaxVolt;
	float Amplitude;
	float AdjA;
	float AdjB;
	float AdjP;
	float PltAmp;
	unsigned long PltTck;
} curve;

curve crv;


IMPLEMENT_DYNAMIC(CDebugDiag, CDialogEx)

CDebugDiag::CDebugDiag(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebugDiag::IDD, pParent)
{

}

CDebugDiag::~CDebugDiag()
{
}

void CDebugDiag::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FEED_AMOUNT, m_FeedAmount);
	DDX_Control(pDX, IDC_CHANNEL, m_Channel);
	DDX_Control(pDX, IDC_MAX_VOLT, m_MaxV);
	DDX_Control(pDX, IDC_MAX_AMPLITUDE, m_Amplitude);
	DDX_Control(pDX, IDC_ADJA, m_AdjA);
	DDX_Control(pDX, IDC_ADJB, m_AdjB);
	DDX_Control(pDX, IDC_ADJP, m_AdjP);
	DDX_Control(pDX, IDC_PLT_AMP, m_PltAmp);
	DDX_Control(pDX, IDC_PLT_TKS, m_PltTck);
	DDX_Control(pDX, IDC_SLAVE_AUX_STATES, m_StateMessages);
	DDX_Control(pDX, IDC_SLAVE_AUX_STATES2, m_StateMessages2);
	DDX_Control(pDX, IDC_LOG, m_Log);
	DDX_Control(pDX, IDC_FEEDHEIGHT, m_FeedHeight);
	DDX_Control(pDX, IDC_AUTOCLOSECB, m_AutoCloseCB);
	DDX_Control(pDX, IDC_FILL_MINUTES, m_FillMinutes);
	DDX_Control(pDX, IDC_SUPERVISOR_MODE, m_SupervisorMode);
	DDX_Control(pDX, IDC_UNDERRUNS, m_Underruns);
}


BEGIN_MESSAGE_MAP(CDebugDiag, CDialogEx)
	ON_BN_CLICKED(IDC_CAP, &CDebugDiag::OnBnClickedCap)
	ON_BN_CLICKED(IDC_UNCAP, &CDebugDiag::OnBnClickedUncap)
	//ON_BN_CLICKED(IDC_FILLPEB, &CDebugDiag::OnBnClickedFillpeb)
	//ON_BN_CLICKED(IDC_BLEEDPEB, &CDebugDiag::OnBnClickedBleedpeb)
	ON_BN_CLICKED(IDC_HV_ON, &CDebugDiag::OnBnClickedHvOn)
	ON_BN_CLICKED(IDC_HV_OFF, &CDebugDiag::OnBnClickedHvOff)
	ON_BN_CLICKED(IDC_WIPE, &CDebugDiag::OnBnClickedWipe)
	ON_BN_CLICKED(IDC_HOMEPLATENZ, &CDebugDiag::OnBnClickedHomeplatenz)
	ON_BN_CLICKED(IDC_PLATENHEIGHT, &CDebugDiag::OnBnClickedPlatenheight)
	ON_BN_CLICKED(IDC_HEADTOMAINT, &CDebugDiag::OnBnClickedHeadtomaint)
	ON_BN_CLICKED(IDC_HOMEHEAD, &CDebugDiag::OnBnClickedHomehead)
	ON_BN_CLICKED(IDC_NOZZLECHECK, &CDebugDiag::OnBnClickedNozzlecheck)
	ON_BN_CLICKED(IDC_AUTOLEVEL, &CDebugDiag::OnBnClickedAutolevel)
	ON_BN_CLICKED(IDC_MAGENTA, &CDebugDiag::OnBnClickedMagenta)
	ON_BN_CLICKED(IDC_SENDCURVE, &CDebugDiag::OnBnClickedSendcurve)
	ON_BN_CLICKED(IDC_BUTTON1, &CDebugDiag::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_LASERSON, &CDebugDiag::OnBnClickedLaserson)
	ON_BN_CLICKED(IDC_LASERSOFF, &CDebugDiag::OnBnClickedLasersoff)
	ON_BN_CLICKED(IDC_CLEARFIFOS, &CDebugDiag::OnBnClickedClearfifos)
	ON_BN_CLICKED(IDC_RESETXY, &CDebugDiag::OnBnClickedResetxy)
	ON_BN_CLICKED(IDC_PAGE2, &CDebugDiag::OnBnClickedPage2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDebugDiag::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_PLATENTOTOP, &CDebugDiag::OnBnClickedPlatentotop)
	ON_BN_CLICKED(IDC_PLATENTOBOTTOM, &CDebugDiag::OnBnClickedPlatentobottom)
	ON_BN_CLICKED(IDC_TESTPATTERNS, &CDebugDiag::OnBnClickedTestpatterns)
	ON_BN_CLICKED(IDC_PLATENTOPRINTPOS, &CDebugDiag::OnBnClickedPlatentoprintpos)
	ON_BN_CLICKED(IDC_SERVO_OFFSETS, &CDebugDiag::OnBnClickedServoOffsets)
	ON_BN_CLICKED(IDC_HEADTOMAINT2, &CDebugDiag::OnBnClickedHeadtomaint2)
	ON_BN_CLICKED(IDC_HEADTOMAINT3, &CDebugDiag::OnBnClickedHeadtomaint3)
	ON_BN_CLICKED(IDC_SPECIAL_1, &CDebugDiag::OnBnClickedSpecial1)
	ON_BN_CLICKED(IDC_PIC_REFRESH, &CDebugDiag::OnBnClickedPicRefresh)
	ON_BN_CLICKED(IDC_PIC_CLEAR, &CDebugDiag::OnBnClickedPicClear)
	ON_BN_CLICKED(IDC_LOG, &CDebugDiag::OnBnClickedLog)
	ON_BN_CLICKED(IDC_PIC_CLEARLOG, &CDebugDiag::OnBnClickedPicClearlog)
	ON_BN_CLICKED(IDC_PLATEN_PIC_TEST, &CDebugDiag::OnBnClickedPlatenPicTest)
	ON_BN_CLICKED(IDC_FORCE_LOG, &CDebugDiag::OnBnClickedForceLog)
	ON_BN_CLICKED(IDC_SEND_DATA, &CDebugDiag::OnBnClickedSendData)
	ON_BN_CLICKED(IDC_WHITELEDON, &CDebugDiag::OnBnClickedWhiteledon)
	ON_BN_CLICKED(IDC_WHITELEDOFF, &CDebugDiag::OnBnClickedWhiteledoff)
	ON_BN_CLICKED(IDC_PLATENTO_PRINT_POS, &CDebugDiag::OnBnClickedPlatentoPrintPos)
	ON_BN_CLICKED(IDC_PLATENTO_FEED_POS, &CDebugDiag::OnBnClickedPlatentoFeedPos)
	ON_BN_CLICKED(IDC_TESTWAITDLG, &CDebugDiag::OnBnClickedTestwaitdlg)
	ON_BN_CLICKED(IDC_BAGTIMER_OFF, &CDebugDiag::OnBnClickedBagtimerOff)
	ON_BN_CLICKED(IDC_BAGTIMER_ON, &CDebugDiag::OnBnClickedBagtimerOn)
	ON_BN_CLICKED(IDOK, &CDebugDiag::OnBnClickedOk)
	ON_BN_CLICKED(IDC_AUTOCLOSECB, &CDebugDiag::OnBnClickedAutoclosecb)
	ON_BN_CLICKED(IDC_EDIT_INI, &CDebugDiag::OnBnClickedEditIni)
	ON_BN_CLICKED(IDC_SUPERVISOR_MODE, &CDebugDiag::OnBnClickedSupervisorMode)
	ON_BN_CLICKED(IDC_RESET_PUR, &CDebugDiag::OnBnClickedResetPur)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_WIPE2, &CDebugDiag::OnBnClickedWipe2)
	ON_BN_CLICKED(IDC_GENERICTEST, &CDebugDiag::OnBnClickedGenerictest)
END_MESSAGE_MAP()


// CDebugDiag message handlers

/*
#define USB_CMD_CAP                      0x12A
#define USB_CMD_RETRACT                  0x12B
#define USB_CMD_WIPER_RECTACT            0x12C
#define USB_CMD_VAC_SELECT               0x12D
#define USB_CMD_VAC_VALVE                0x12E
#define USB_CMD_VAC_PUMP                 0x12F
*/

void CDebugDiag::OnBnClickedCap() {
	
	USBPrinterCommand(USB_CMD_CAP, 1);

}


void CDebugDiag::OnBnClickedUncap() {
	USBPrinterCommand(USB_CMD_CAP, 0);
}


void CDebugDiag::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);

}


BOOL CDebugDiag::OnInitDialog()
{
	CDialogEx::OnInitDialog();
HANDLE fh;
char tmp_buff[100];

	SizeDialog();


	crv.channel = 2;
	crv.MaxVolt = 20.45;
	crv.Amplitude = 20.45;
	crv.AdjA = 5;
	crv.AdjB = 10;
	crv.AdjP = 66;


	unsigned long bytesread;

	fh = CreateFile("lastcurve.dat",
					GENERIC_READ,
					0, //FILE_SHARE_READ,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);

	if(fh != INVALID_HANDLE_VALUE) {

		ReadFile(fh, (char *)&crv, sizeof(curve), &bytesread, NULL);

		CloseHandle(fh);
	}

	m_SupervisorMode.SetCheck(theApp.GVars->bAdvancedFeaturesUnlocked);
	/*
	f = _open("lastcurve.dat", _O_RDONLY|_O_BINARY);
	if(f > 0) {
		_read(f, (char *)&crv, sizeof(curve));
		close(f);
	}
	*/

	/// always disabled while in diag mode
	USBPrinterCommand(USB_CMD_ENABLE_INKBAG_TIMER, 1);
	USBPrinterCommand(USB_CMD_SET_MAINT_MODE, 1);
	
	sprintf(tmp_buff, "%u", crv.channel);
	m_Channel.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%.04f", crv.MaxVolt);
	m_MaxV.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%.04f", crv.Amplitude);
	m_Amplitude.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%.04f", crv.AdjA);
	m_AdjA.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%.04f", crv.AdjB);
	m_AdjB.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%.04f", crv.AdjP);
	m_AdjP.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%.04f", crv.PltAmp);
	m_PltAmp.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%u", crv.PltTck);
	m_PltTck.SetWindowText(tmp_buff);


	theApp.ReadINI();
	sprintf(tmp_buff, "%d", theApp.GVars->iPlatenFeedHeight);
	m_FeedHeight.SetWindowText(tmp_buff);

	m_AutoCloseCB.SetCheck(theApp.GVars->bInkBagAutoOffDisabled);

	OnBnClickedPicRefresh();

	m_Log.SetCheck(theApp.GVars->bEventLog);

	sprintf(tmp_buff, "%d", theApp.GVars->iCapFillTimer);
	m_FillMinutes.SetWindowText(tmp_buff);

	if(theApp.GVars->bUnderFlow)
		m_Underruns.SetWindowText("Yes");
	else
		m_Underruns.SetWindowText("No");




	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


/*
void CDebugDiag::OnBnClickedBleedpeb()
{
	static BOOL state;

	unsigned char head = m_Head.GetCurSel();

	unsigned long param = 0;

	param |= GetChannels() << 24;

	param |= (head << 8);

	if(!state)
		param |= 1;

	USBPrinterCommand(USB_CMD_BLEED_PEB, param);

	state = !state;
}
*/




void CDebugDiag::OnBnClickedHvOn()
{
	USBPrinterCommand(USB_CMD_ENABLE_HV, 1);
}


void CDebugDiag::OnBnClickedHvOff()
{
	USBPrinterCommand(USB_CMD_ENABLE_HV, 0);

}


void CDebugDiag::OnBnClickedWipe()
{
	USBPrinterCommand(USB_CMD_WIPE, 2);
}


void CDebugDiag::OnBnClickedHomeplatenz()
{

	SendPlatenLevelingOffset();
	USBPrinterCommand(USB_CMD_HOME_PLATEN_Z, 1);

}


void CDebugDiag::OnBnClickedPlatenheight()
{
CPlatenControl dlg;

	dlg.DoModal();
	
	
	/*
char number[100];
int val;


	m_FeedAmount.GetWindowText(number,100);

	val = atoi(number);

	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, val);

	*/
}


void CDebugDiag::OnBnClickedHeadtomaint()
{
		USBPrinterCommand(USB_CMD_CAP, 0);
		USBPrinterCommand(USB_CMD_VAC_VALVE, 0);
		USBPrinterCommand(USB_HEAD_TO_MAINT, 0);
}


void CDebugDiag::OnBnClickedHomehead()
{
	USBPrinterCommand(USB_CMD_CAP, 0);
	USBPrinterCommand(USB_CMD_PARK, 0);
	//USBPrinterCommand(USB_CMD_CAP, 1);
}


void CDebugDiag::OnBnClickedNozzlecheck()
{

	SendPlatenHeightOffset();
	Sleep(500);
	USBPrinterCommand(USB_CMD_NOZZLE_CHECK, 1);
	Sleep(500);

}


void CDebugDiag::OnBnClickedAutolevel()
{
	USBPrinterCommand(USB_CMD_AUTO_PLATEN_HEIGHT, 1);
	Sleep(250);
}


void CDebugDiag::OnBnClickedMagenta()
{
	// TODO: Add your control notification handler code here
}


void CDebugDiag::OnBnClickedSendcurve() {
int Channel;
char tmp_buff[128];
unsigned long p1, p2, p3, p4, p5, chnl;


	m_Channel.GetWindowTextA(tmp_buff, 128);
	Channel = atoi(tmp_buff);
	if(Channel < 0 || Channel > 8) {
		MessageBox("INVALID CHANNEL", "GENESIS CONTROL", MB_OK|MB_SYSTEMMODAL);
		return;
	}

	crv.channel = Channel;
	
	m_MaxV.GetWindowTextA(tmp_buff, 128);
	crv.MaxVolt = atof(tmp_buff);
	if(crv.MaxVolt < 1 || crv.MaxVolt > 24) {
		MessageBox("INVALID MAX VOLTAGE", "GENESIS CONTROL", MB_OK|MB_SYSTEMMODAL);
		return;
	}

	m_Amplitude.GetWindowTextA(tmp_buff, 128);
	crv.Amplitude = atof(tmp_buff);
	if(crv.Amplitude < 1 || crv.Amplitude > crv.MaxVolt) {
		MessageBox("INVALID MAX AMPLITUDE", "GENESIS CONTROL", MB_OK|MB_SYSTEMMODAL);
		return;
	}
	
	m_AdjA.GetWindowTextA(tmp_buff, 128);
	crv.AdjA = atof(tmp_buff);
	if(crv.AdjA < 1 || crv.AdjA > 100) {
		MessageBox("INVALID MAX AdjA", "GENESIS CONTROL", MB_OK|MB_SYSTEMMODAL);
		return;
	}

	m_AdjB.GetWindowTextA(tmp_buff, 128);
	crv.AdjB = atof(tmp_buff);
	if(crv.AdjB < 1 || crv.AdjB > 100) {
		MessageBox("INVALID MAX AdjB", "GENESIS CONTROL", MB_OK|MB_SYSTEMMODAL);
		return;
	}

	m_AdjP.GetWindowTextA(tmp_buff, 128);
	crv.AdjP = atof(tmp_buff);
	if(crv.AdjA < 1 || crv.AdjP > 100) {
		MessageBox("INVALID MAX AdjA", "GENESIS CONTROL", MB_OK|MB_SYSTEMMODAL);
		return;
	}

	m_PltAmp.GetWindowTextA(tmp_buff, 128);
	crv.PltAmp = atof(tmp_buff);
	if(crv.PltAmp < 0 || crv.PltAmp > crv.Amplitude) {
		MessageBox("INVALID PLATEAU Amplitude", "GENESIS CONTROL", MB_OK|MB_SYSTEMMODAL);
		return;
	}

	m_PltTck.GetWindowTextA(tmp_buff, 128);
	crv.PltTck = atoi(tmp_buff);
	if(crv.PltTck < 0 || crv.PltTck > 20) {
		MessageBox("INVALID PLATEAU Length\r\n(must be 0 - 20)", "GENESIS CONTROL", MB_OK|MB_SYSTEMMODAL);
		return;
	}


	memcpy((char *)&p1, (char *)&crv.PltAmp,	4);
	memcpy((char *)&p2, (char *)&crv.PltTck, 4);

	USBPrinterCommandEx(USB_CMD_SET_COM_PLATEAU, p1, p2, 0, 0, 0, 0, 0);

	chnl = (unsigned long)Channel;

	memcpy((char *)&p1, (char *)&crv.MaxVolt,	4);
	memcpy((char *)&p2, (char *)&crv.Amplitude, 4);
	memcpy((char *)&p3, (char *)&crv.AdjA,		4);
	memcpy((char *)&p4, (char *)&crv.AdjB,		4);
	memcpy((char *)&p5, (char *)&crv.AdjP,		4);

	USBPrinterCommandEx(USB_CMD_SET_COM_WAVE, p1, p2, p3, p4, p5, chnl, 0);

	//int f;
	HANDLE fh;
	unsigned long written;

	DeleteFile("lastcurve.dat");

	fh = CreateFile(	"lastcurve.dat", 
						GENERIC_WRITE,
						0, //FILE_SHARE_READ|FILE_SHARE_WRITE,
						(LPSECURITY_ATTRIBUTES)NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE)NULL);
	if(fh != INVALID_HANDLE_VALUE) {

		WriteFile(fh, (char *)&crv, sizeof(curve), &written, NULL);

		CloseHandle(fh);
	}
	else {
		int error, foo;
		error = GetLastError();
		foo = 0;
	}

}


void CDebugDiag::OnBnClickedButton1()
{
int foo;

	foo = 1;
}


void CDebugDiag::OnBnClickedLaserson()
{
	USBPrinterCommand(USB_CMD_ENABLE_LASERS, 1);
}


void CDebugDiag::OnBnClickedLasersoff()
{
	USBPrinterCommand(USB_CMD_ENABLE_LASERS, 0);
}


void CDebugDiag::OnBnClickedClearfifos()
{
	USBPrinterCommand(USB_CMD_FLUSH_PIXEL_PIPELINES, 1);

}


void CDebugDiag::OnBnClickedResetxy()
{
	USBPrinterCommand(USB_CMD_RESET_MOVEMENT, 0);
	Sleep(200);
	USBPrinterCommand(USB_CMD_RESET_MOVEMENT, 1);
	Sleep(200);

}


void CDebugDiag::OnBnClickedPage2() {
	CDebugDiag2 dlg;

	dlg.DoModal();



}


void CDebugDiag::OnBnClickedButton3()
{
CDebugDiag3 dlg;

	if(!theApp.InksOKToPrint()) {
		theDlg->OKBox("*** WARNING ***", "ONE OR MORE INK", "CHANNEL EMPTY!");
	}


	dlg.DoModal();


}



void CDebugDiag::OnBnClickedPlatentotop()
{
	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 11);
}


void CDebugDiag::OnBnClickedPlatentobottom()
{
	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 0);
}

void CDebugDiag::OnBnClickedTestpatterns() {
CTestBands				cdlg;
CTestPatternSelector	dlg;
CYLinearization			yldlg;
int result;

	result = dlg.DoModal();

	switch(dlg.Result) {
		case 0:
			cdlg.DoModal();
			break;

		case 1:
			yldlg.DoModal();
			break;
	}
	

	
}


void CDebugDiag::OnBnClickedPlatentoprintpos()
{
	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 16000);
}


void CDebugDiag::OnBnClickedServoOffsets()
{
	CServoOffsets dlg;

	dlg.DoModal();

}


void CDebugDiag::OnBnClickedHeadtomaint2()
{
	USBPrinterCommand(USB_HEAD_TO_MAINT, 1);

}


void CDebugDiag::OnBnClickedHeadtomaint3()
{
	USBPrinterCommand(USB_HEAD_TO_MAINT, 2);
}


void CDebugDiag::OnBnClickedSpecial1()
{
	USBPrinterCommand(USB_CMD_SPECIAL_COMMAND, 1);
}


void CDebugDiag::OnBnClickedPicRefresh()
{

	sprintf(StateMessages, "Slave 1 Resets: %d\r\nSlave 1 Timeouts: %d\r\nSlave1 Bad Responses: %d\r\nSlave 1 Temp Failsafe: %d\r\nHead Temp Timeouts: %d\r\n",
		theApp.GVars->PICResets,
		theApp.GVars->PICUnresponsive,
		theApp.GVars->PICBadResponse,
		theApp.GVars->PICBadTemp,
		theApp.GVars->PICHeadTempTimeout);

	m_StateMessages.SetWindowText(StateMessages);

	sprintf(StateMessages2, "Amb Temp Timeouts: %d\r\nAmb Humidity Timeouts: %d\r\nAux Timeouts: %d\r\nAux Bad Resp: %d\r\nAux Ready Timeouts: %d\r\n",
		theApp.GVars->PICAmbTempTimeout,
		theApp.GVars->PICAmbHumTimeout,
		theApp.GVars->AUXTimeout,
		theApp.GVars->AUXBadResp,
		theApp.GVars->AUXReadyTimeout);

	m_StateMessages2.SetWindowText(StateMessages2);


}


void CDebugDiag::OnBnClickedPicClear()
{
	theApp.GVars->PICUnresponsive = 0;
	theApp.GVars->PICResets = 0;
	theApp.GVars->PICBadResponse = 0;
	theApp.GVars->PICBadTemp = 0;
	OnBnClickedPicRefresh();
}


void CDebugDiag::OnBnClickedLog()
{
	theApp.GVars->bEventLog = m_Log.GetCheck();
	if(theApp.GVars->bEventLog) {
		OpenLogFile();
	}
	else {
		CloseLogFile();
	}


}


void CDebugDiag::OnBnClickedPicClearlog()
{
	ClearLogFile(theApp.GVars->bEventLog);
}




void CDebugDiag::OnBnClickedPlatenPicTest() {

	USBPrinterCommandEx(USB_CMD_PLATEN_COMMAND, 10, 20, 30, 40, 50, 60, 70);

}


void CDebugDiag::OnBnClickedForceLog() {

	//theApp.NtwrkObj.ConnectToLogServer(theApp.GVars->szLogIP, theApp.GVars->LogPort);

#ifndef USE_MOE_LOGGING
	/// MOE - Get requirements for test functions and I will provide them in CNetworkDLLinterface
	if(theApp.ConnectedToLogServer) {
		MessageBox("ALREADY CONNECTED TO LOG SITE", "GENESIS CONTROLLER", MB_OK|MB_SYSTEMMODAL);
		return;
	}

	if(theApp.ConnectToLogServer(theApp.GVars->szLogIP, theApp.GVars->LogPort)) {
		MessageBox("CONNECTED TO LOG SITE!!", "GENESIS CONTROLLER", MB_OK|MB_SYSTEMMODAL);
	}
	else {
		MessageBox("ERROR CONNECTING TO LOG SITE", "GENESIS CONTROLLER", MB_OK|MB_SYSTEMMODAL);
	}
#endif

}


void CDebugDiag::OnBnClickedSendData() {

	//theApp.NtwrkObj.UploadString("Hello World");

#ifndef USE_MOE_LOGGING
	theApp.UploadLog();                            //MOE - I can provide a send data test function in CNetworkDLLinterface if necessary
#endif

}


void CDebugDiag::OnBnClickedWhiteledon()
{
		USBPrinterCommand(USB_CMD_LED_STRIP, 1);
}


void CDebugDiag::OnBnClickedWhiteledoff()
{
		USBPrinterCommand(USB_CMD_LED_STRIP, 0);
}


void CDebugDiag::OnBnClickedPlatentoPrintPos()
{
	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 15500); // special number. means go to saved print pos.

}


void CDebugDiag::OnBnClickedPlatentoFeedPos()
{
char tmp_buff[32];

	m_FeedHeight.GetWindowText(tmp_buff, 32);

	theApp.GVars->iPlatenFeedHeight = atoi(tmp_buff);

	USBPrinterCommandEx(USB_CMD_SAVE_PLATVARS, theApp.GVars->iPlatenFeedHeight, SAVE_POS_AS_FEEDINGPOS, 0, 0, 0, 0, 0);

	Sleep(500);

	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 15001); /// special number, means go to feed position

	theApp.WriteINI();

}


void CDebugDiag::OnBnClickedTestwaitdlg()
{

	USBPrinterCommand(USB_CMD_SET_HEADTEMPTIMEOUT, 60000);

	/*
	if(!theApp.RunWaitDialog) {
		theApp.StartWaitDialog("INITIALIZING...");
	}
	else {
		theApp.RunWaitDialog = FALSE;
	}
	*/


}


void CDebugDiag::OnBnClickedBagtimerOff() {

	//theApp.GVars->bInkBagAutoOffDisabled = TRUE;
	//USBPrinterCommand(USB_CMD_ENABLE_INKBAG_TIMER, 1);


}


void CDebugDiag::OnBnClickedBagtimerOn()
{
	//theApp.GVars->bInkBagAutoOffDisabled = FALSE;
	//USBPrinterCommand(USB_CMD_ENABLE_INKBAG_TIMER, 0);
}


void CDebugDiag::OnBnClickedOk()
{

	/*
	if(theApp.GVars->bInkBagAutoOffDisabled) {
		theDlg->OKBox("** WARNING **", "Ink Bag auto close", "DISABLED");
	}
	*/

	int fillInterval = 0;
	char tmp_buff[128];

	m_FillMinutes.GetWindowText(tmp_buff, 128);
	fillInterval = atoi(tmp_buff);

	if(fillInterval < 60) {
		theDlg->OKBox("IDLE TIME BEFORE CAP", "FILL TOO SHORT", "MIN. IS 60 MIN.");
	}
	else {
		theApp.GVars->iCapFillTimer = fillInterval;
	}

	theApp.WriteINI();
	CDialogEx::OnOK();
}


void CDebugDiag::OnBnClickedAutoclosecb() {
	theApp.GVars->bInkBagAutoOffDisabled = m_AutoCloseCB.GetCheck();
	//USBPrinterCommand(USB_CMD_ENABLE_INKBAG_TIMER, theApp.GVars->bInkBagAutoOffDisabled);
}


void CDebugDiag::OnBnClickedEditIni() {
CINIEditor dlg;

	dlg.DoModal();
	
	if(dlg.m_Entries.bChanged) {
		
		if(!theApp.GVars->bHideForceButton) {
			theDlg->m_ForceButton.ShowWindow(SW_SHOW);
		}
		else {
			theDlg->m_ForceButton.ShowWindow(SW_HIDE);
		}

		if(theApp.GVars->MaxJobs < 4 && !theApp.GVars->bDualCMYK) {
			theDlg->OKBox("MaxJobs cannot be less than 3 for","a CMYKW printer. That's only supported"," on CMYKx2 printers. Setting MaxJobs to 3");
			theApp.GVars->MaxJobs = 3;
		}

		theApp.WriteINI();
		SendSpeeds();
	}


}


void CDebugDiag::OnBnClickedSupervisorMode()
{
	theApp.GVars->bAdvancedFeaturesUnlocked = m_SupervisorMode.GetCheck();
}


void CDebugDiag::OnBnClickedResetPur()
{
	theApp.GVars->bUnderFlow = FALSE;
	m_Underruns.SetWindowText("No");
}


void CDebugDiag::OnDestroy()
{

	/// back to whatever mode it's supposed to be in
	USBPrinterCommand(USB_CMD_ENABLE_INKBAG_TIMER, theApp.GVars->bInkBagAutoOffDisabled);
	USBPrinterCommand(USB_CMD_SET_MAINT_MODE, 0);

	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}


void CDebugDiag::OnBnClickedWipe2() {

	USBPrinterCommand(USB_CMD_WIPE, 1);

}


void CDebugDiag::OnBnClickedGenerictest() {

	int ret;

	/*
	ret = theApp.ConfigurableUserDlg(0,
										"Test User Dialog",
									
										"Button 1\nTest",
										"Button 2\nTest",
										"Button 3\nTest",
										"Button 4\nTest",

										NULL,
										NULL,
										NULL,
										NULL,

										0);

	*/

	ret = theApp.ConfigurableUserDlg(0,
										"PLEASE CHECK THAT CAP IS EMPTY",
									
										"CAP IS EMPTY\nCONTINUE",
										"REPEAT DRAIN\nOPERATION",
										NULL,
										"CANCEL",

										"HELP_DOCS\\CAP_EMPTY_VERIFY_1.jpg",
										"HELP_DOCS\\CAP_EMPTY_VERIFY_2.jpg",
										NULL,
										NULL,

										0);

	char retbuff[32];
	sprintf(retbuff, "Returns: %d", ret);
	MessageBox(retbuff, APP_NAME, MB_OK|MB_SYSTEMMODAL);


}

// YLinearization.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "YLinearization.h"
#include "afxdialogex.h"
#include "TestBands.h"
#include "parser.h"
#include "HEAD_structs.h"
#include "Controller4Dlg1.h"

extern CController4Dlg1 *theDlg;


// CYLinearization dialog

extern void USBPrinterCommand(unsigned long , unsigned long);

extern BOOL OnFileOpenTestBand(char *, int);

extern BOOL ConvertPlane(int, BOOL);

extern void SendTestBand(testbandVars *, int);

extern void AddNumberToBand(int, int);

extern CController4App theApp;

extern void SendPlatenHeightOffset();

static int YReses[4] = {150, 300, 600, 1200};

static CYLinearization *thisDlg;

/*-------------------------------------------------------------------------------------*/
/*-                                                                                   -*/
/*-------------------------------------------------------------------------------------*/
void PrintAlignmentTest(void *p) {
int i, reps, c;
int timeout = 0;
YLVars *vars = (YLVars *)p;


	vars->SendThreadRunning = TRUE;

	reps = vars->Rep;


	vars->Progress->SetRange32(0, reps - 1);

	vars->Progress->SetPos(0);

	vars->Progress->ShowWindow(SW_SHOW);

	theApp.bPrinterPrepped = 0;
	theApp.bYPrintMovementSet = 0;

	SendPlatenHeightOffset();

	USBPrinterCommand(USB_CMD_PREPARE_FOR_JOB, 1);
	
	Sleep(500);

	if(theApp.GPlatenPosition != 1 && theApp.GPlatenPosition != 2) {
			
		USBPrinterCommand(USB_CMD_TRAY_POS_REPORT_REQ, 1);
		Sleep(0);
		//Sleep(1000);
		Sleep(500);

	}


	if(!theApp.NoUSB) {
		while(!(/*theApp.bPrinterPrepped && */theApp.GPlatenPosition == 2) || theApp.GMasterStatus == MASTER_STATUS_PAUSED) {
			Sleep(1);
			timeout++;
			if(timeout > 20000) {
				MessageBox(GetFocus(), "PrintTestBand Timeout", "GENAPP", MB_OK|MB_SYSTEMMODAL);
				goto abortsendtestband;
			}
		}
	}


	USBPrinterCommand(USB_SET_SOL, (13 - vars->LM) * 1200);

	USBPrinterCommand(USB_CMD_SET_LAYERTYPE, LAYER_TP_SINGLE_COLOR);

	USBPrinterCommand(USB_CMD_SET_Y_RESOLUTION, YReses[vars->YParams - 1]);

	USBPrinterCommand(USB_CMD_SET_X_RESOLUTION, X_RESOLUTION_600);

	//USBPrinterCommand(USB_CMD_FEED, (int)(vars->Top * (float)4000)); // initial position

	vars->iDirection = DIRECTION_AWAY;

	testbandVars aux_vars;

	aux_vars.bBiDirectional = FALSE;
	aux_vars.iDirection = vars->iDirection;
	aux_vars.bVerifyBands = vars->bVerifyBands;
	aux_vars.bForce1Bit = TRUE;
	aux_vars.ChannelToUse[1] = 1;
	aux_vars.Progress = vars->Progress;
	aux_vars.Rep = vars->Rep;
	aux_vars.Status = vars->Status;
	aux_vars.YAdvance = vars->YAdvance;
	aux_vars.XResolution = 600;
	aux_vars.Top = 0;
	aux_vars.InkOrder = vars->iCurInkSetup;

	int band_no = 0;

	if(thisDlg->m_TestPattern.GetCurSel() == 1) {
		band_no++;
	}

	for(i = 0; i < reps; i++) {

		if(vars->CancelRequest) break;

		//if(OnFileOpenTestBand(ALIGNMENT_BAND_FILE)) {
		if(OnFileOpenTestBand(thisDlg->TestPatterns[thisDlg->m_TestPattern.GetCurSel()].szFileName, vars->iCurInkSetup)) {
			
			if(vars->iCurInkSetup == 0) {// cmykw
				AddNumberToBand(1, band_no++);
				ConvertPlane(1, 1); // magenta
				Sleep(1500);
				SendTestBand(&aux_vars, 1);
			}
			else {
				AddNumberToBand(3, band_no++);
				ConvertPlane(3, 1); // black
				Sleep(1500);
				SendTestBand(&aux_vars, 3);
			}
			
			// TRACE("Sent Band %d\n", i);

		}
		else {
			theDlg->OKBox("ERROR Opening", thisDlg->TestPatterns[thisDlg->m_TestPattern.GetCurSel()].szFileName, NULL);
		}

	}

	USBPrinterCommand(USB_CMD_HOME_PLATEN, 0);
	USBPrinterCommand(USB_CMD_PARK, 0);

abortsendtestband:

	vars->Progress->SetPos(100);

	Sleep(1000);

	vars->Status->SetWindowText(" ");
	vars->Progress->ShowWindow(SW_HIDE);
	vars->SendThreadRunning = FALSE;

	
}





//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(CYLinearization, CDialogEx)

CYLinearization::CYLinearization(CWnd* pParent /*=NULL*/)
	: CDialogEx(CYLinearization::IDD, pParent)
{

}

CYLinearization::~CYLinearization()
{
}

void CYLinearization::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LMUD, m_LMUD);
	DDX_Control(pDX, IDC_STRIPESUD, m_StripesUD);
	DDX_Control(pDX, IDC_LM, m_LM);
	DDX_Control(pDX, IDC_STRIPES, m_Stripes);
	DDX_Control(pDX, IDC_VALUD, m_ValUD);
	DDX_Control(pDX, IDC_INDEXUD, m_IndexUD);
	DDX_Control(pDX, IDC_COR_VAL_PRE, m_CorValPre);
	DDX_Control(pDX, IDC_COR_VAL_CUR, m_CorValCur);
	DDX_Control(pDX, IDC_COR_VAL_POST, m_CorValPost);
	DDX_Control(pDX, IDC_INDEXL_PRE, m_IndexPre);
	DDX_Control(pDX, IDC_INDEXL_CUR, m_IndexCur);
	DDX_Control(pDX, IDC_INDEXL_POST, m_IndexPost);
	DDX_Control(pDX, IDC_UD_1000, m_Spin1000);
	DDX_Control(pDX, IDC_UD_100, m_Spin100);
	DDX_Control(pDX, IDC_UD_10, m_Spin10);
	DDX_Control(pDX, IDC_UD_1, m_Spin1);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_REPS2, m_Steps);
	DDX_Control(pDX, IDC_STATUS, m_Status);
	DDX_Control(pDX, IDC_COUNTER, m_Counter);
	DDX_Control(pDX, IDC_YPARAMS, m_YParams);
	DDX_Control(pDX, IDC_VERIFY_BANDS, m_VerifyBands);
	DDX_Control(pDX, IDC_TEST_PATTERN, m_TestPattern);
	DDX_Control(pDX, IDC_INKSETUP, m_InkOrder);
}


BEGIN_MESSAGE_MAP(CYLinearization, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_INDEXUD, &CYLinearization::OnDeltaposIndexud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FEEDUD, &CYLinearization::OnDeltaposFeedud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_1000, &CYLinearization::OnDeltaposUd1000)
	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_100, &CYLinearization::OnDeltaposUd100)
	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_10, &CYLinearization::OnDeltaposUd10)
	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_1, &CYLinearization::OnDeltaposUd1)
	ON_WM_DESTROY()
	ON_NOTIFY(UDN_DELTAPOS, IDC_VALUD, &CYLinearization::OnDeltaposValud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_LMUD, &CYLinearization::OnDeltaposLmud)
	ON_NOTIFY(UDN_DELTAPOS, IDC_STRIPESUD, &CYLinearization::OnDeltaposStripesud)
	ON_BN_CLICKED(IDC_CANCEL_PRINT, &CYLinearization::OnBnClickedCancelPrint)
	ON_BN_CLICKED(IDC_PRINT_TEST, &CYLinearization::OnBnClickedPrintTest)
	ON_BN_CLICKED(IDC_SENDVALUES, &CYLinearization::OnBnClickedSendvalues)
	ON_BN_CLICKED(IDC_RESET, &CYLinearization::OnBnClickedReset)
	ON_BN_CLICKED(IDC_GOTOZERO, &CYLinearization::OnBnClickedGotozero)
	ON_BN_CLICKED(IDC_GOTOSELECTED, &CYLinearization::OnBnClickedGotoselected)
	ON_CBN_SELCHANGE(IDC_YPARAMS, &CYLinearization::OnCbnSelchangeYparams)
	ON_BN_CLICKED(IDC_VERIFY_BANDS, &CYLinearization::OnBnClickedVerifyBands)
	ON_CBN_SELCHANGE(IDC_TEST_PATTERN, &CYLinearization::OnCbnSelchangeTestPattern)
	ON_EN_CHANGE(IDC_REPS2, &CYLinearization::OnEnChangeReps2)
	ON_EN_CHANGE(IDC_LM, &CYLinearization::OnEnChangeLm)
	ON_CBN_SELCHANGE(IDC_INKSETUP, &CYLinearization::OnCbnSelchangeInksetup)
END_MESSAGE_MAP()


// CYLinearization message handlers


void CYLinearization::PopulateSliders() {
int i, index;
char tmp_buf[1000], num[64];

	// List index, top area
	tmp_buf[0] = 0;
	for(i = 0; i < 8; i++) {
		strcat(tmp_buf, SliderDisplay[ylVars.CurrentEditVar + i + 1]);
		strcat(tmp_buf, "\r\n");
	}

	m_IndexPre.SetWindowText(tmp_buf);
	
	// Values, top area
	tmp_buf[0] = 0;

	 index = ylVars.CurrentEditVar - 8;

	for(i = 0; i < 9; i++, index++) {
		
		if(index >= 0) {
			sprintf(num, "%d\r\n", ylVars.CorVals[ylVars.iCurTestPattern][index]);
		}
		else {
			sprintf(num, " \r\n");
		}
		strcat(tmp_buf, num);
	}

	m_CorValPre.SetWindowText(tmp_buf);


	// Current Index
	sprintf(tmp_buf, "%d", ylVars.CurrentEditVar + 1);
	m_IndexCur.SetWindowText(tmp_buf);


	// Current Value
	sprintf(tmp_buf, "%d", ylVars.CorVals[ylVars.iCurTestPattern][ylVars.CurrentEditVar]);
	m_CorValCur.SetWindowText(tmp_buf);



	// List index, bottom area;
	tmp_buf[0] = 0;
	for(i = 0; i < 8; i++) {
		strcat(tmp_buf, SliderDisplay[ylVars.CurrentEditVar + i + 10]);
		strcat(tmp_buf, "\r\n");
	}

	m_IndexPost.SetWindowText(tmp_buf);


	// Values, bottom area;
	tmp_buf[0] = 0;
	index = ylVars.CurrentEditVar + 1;
	for(i = 0; i < 8; i++, index++) {

		if(index <= MAX_TABLE_SLOT) {
			sprintf(num, "%d\r\n", ylVars.CorVals[ylVars.iCurTestPattern][index]);
		}
		else {
			sprintf(num, " \r\n");
		}
		strcat(tmp_buf, num);
	}

	m_CorValPost.SetWindowText(tmp_buf);

}

void CYLinearization::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);

	//m_Home.SetWindowPlacement(&theApp.HomeWP);



}


BOOL CYLinearization::OnInitDialog()
{
int i;

	theApp.bBlockTempThread = TRUE;

	CDialogEx::OnInitDialog();

	thisDlg = this;

	SizeDialog();

	strcpy(TestPatterns[0].szName, "1200 DPI TEST PATTERN"); 
	strcpy(TestPatterns[0].szFileName, ALIGNMENT_1200DPI_BAND_FILE); 

	strcpy(TestPatterns[1].szName, "600 DPI TEST PATTERN"); 
	strcpy(TestPatterns[1].szFileName, ALIGNMENT_600DPI_BAND_FILE); 

	strcpy(TestPatterns[2].szName, "CUSTOM TEST PATTERN"); 
	strcpy(TestPatterns[2].szFileName, "** UNDEFINED **"); 


	for(i = 0; i < TOTAL_PATTERNS; i++) {
		m_TestPattern.AddString(TestPatterns[i].szName);
	}


	WINDOWPLACEMENT  wp1, wp2, wp3, wp4, wp5, wp6, wp7, wp8;

	m_LMUD.GetWindowPlacement(&wp1);
	m_StripesUD.GetWindowPlacement(&wp2);
	m_ValUD.GetWindowPlacement(&wp3);
	m_IndexUD.GetWindowPlacement(&wp4);

	m_Spin1000.GetWindowPlacement(&wp5);
	m_Spin100.GetWindowPlacement(&wp6);
	m_Spin10.GetWindowPlacement(&wp7);
	m_Spin1.GetWindowPlacement(&wp8);

	wp1.rcNormalPosition.right = wp1.rcNormalPosition.left + 90;
	wp2.rcNormalPosition.right = wp2.rcNormalPosition.left + 90;
	wp3.rcNormalPosition.right = wp3.rcNormalPosition.left + 90;
	wp4.rcNormalPosition.right = wp4.rcNormalPosition.left + 90;
	wp5.rcNormalPosition.right = wp5.rcNormalPosition.left + 90;
	wp6.rcNormalPosition.right = wp6.rcNormalPosition.left + 90;
	wp7.rcNormalPosition.right = wp7.rcNormalPosition.left + 90;
	wp8.rcNormalPosition.right = wp8.rcNormalPosition.left + 90;

	m_LMUD.SetWindowPlacement(&wp1);
	m_StripesUD.SetWindowPlacement(&wp2);
	m_ValUD.SetWindowPlacement(&wp3);
	m_IndexUD.SetWindowPlacement(&wp4);

	m_Spin1000.SetWindowPlacement(&wp5);
	m_Spin100.SetWindowPlacement(&wp6);
	m_Spin10.SetWindowPlacement(&wp7);
	m_Spin1.SetWindowPlacement(&wp8);


	ylVars.bExternalCall = FALSE;

	LoadVars(YL_VARS_FN);


	ylVars.Status = &m_Status;

	for(i = 0; i < 138; i++) {
		if(i < 9 || i > (YL_TABLE_LENGTH + 8)) {
			strcpy(SliderDisplay[i], " ");
		}
		else {
			sprintf(SliderDisplay[i], "%d", i - 8);
		}
	}

	//ylVars.CurrentEditVar = 0;

	PopulateSliders();

	DisplaySteps();

	DisplayStripes();

	DisplayLM();

	DisplayCounter();

	DisplayYParams();

	DisplayVerifyBands();

	m_InkOrder.SetCurSel(ylVars.iCurInkSetup);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CYLinearization::SaveVars(char *szFileName) {
HANDLE fh;
unsigned long bytes;
char tmp_buff[MAX_PATH];

	fh = CreateFile(	szFileName, 
						GENERIC_WRITE,
						0, //FILE_SHARE_READ|FILE_SHARE_WRITE,
						(LPSECURITY_ATTRIBUTES)NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE)NULL);


	if(fh != INVALID_HANDLE_VALUE) {

		m_Stripes.GetWindowText(tmp_buff, MAX_PATH);
		ylVars.Rep = atoi(tmp_buff);

		m_Steps.GetWindowText(tmp_buff, MAX_PATH);

		ylVars.YAdvance = atoi(tmp_buff);
		WriteFile(fh, (char *)&ylVars, sizeof(ylVars), &bytes, NULL);

		CloseHandle(fh);
	}


}


void CYLinearization::LoadVars(char *szFileName) {
HANDLE fh;
unsigned long bytes;
char tmp_buff[MAX_PATH];
BOOL external = ylVars.bExternalCall;
int i;

	memset((char *)&ylVars, 0, sizeof(YLVars));
	
	ylVars.bExternalCall = external;

	fh = CreateFile(szFileName,
					GENERIC_READ,
					0,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);

	if(fh != INVALID_HANDLE_VALUE) {

		ReadFile(fh, (char *)&ylVars, sizeof(YLVars), &bytes, NULL);
		ylVars.bExternalCall = external;


		// check to see if it's an old version
		for(i = 0; i < YL_TABLE_LENGTH; i++) {
			if(ylVars.CorVals[1][i] > 100) { // it's an old DAT file, wipe all tables but 1200
				int j;
				for(j = 0; j < YL_TABLE_LENGTH; j++) {
					ylVars.CorVals[1][j] = 0;
					ylVars.CorVals[2][j] = 0;
					ylVars.CorVals[3][j] = 0;
				}
				break;
			}
		}


		if(!ylVars.bExternalCall) {
			sprintf(tmp_buff, "%d", ylVars.Rep);
			m_Stripes.SetWindowText(tmp_buff);

			sprintf(tmp_buff, "%d", ylVars.YAdvance);
			m_Steps.SetWindowText(tmp_buff);

			if(ylVars.iCurTestPattern < 0 || ylVars.iCurTestPattern >= TOTAL_PATTERNS) {
				ylVars.iCurTestPattern = 0;
			}

		}

		CloseHandle(fh);
	
	}

	ylVars.SendThreadRunning = FALSE;
	
	if(!ylVars.bExternalCall) {
		m_TestPattern.SetCurSel(ylVars.iCurTestPattern);
		ylVars.Progress = &m_Progress;
	}
}

void CYLinearization::LoadVarsExt(char *szFileName) {
HANDLE fh;
unsigned long bytes;
char tmp_buff[MAX_PATH];
BOOL external = ylVars.bExternalCall;

	memset((char *)&ylVars, 0, sizeof(YLVars));
	
	ylVars.bExternalCall = external;

	fh = CreateFile(szFileName,
					GENERIC_READ,
					0,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);

	if(fh != INVALID_HANDLE_VALUE) {

		ReadFile(fh, (char *)&ylVars, sizeof(YLVars), &bytes, NULL);
		ylVars.bExternalCall = external;

		CloseHandle(fh);
	
	}

	ylVars.SendThreadRunning = FALSE;
	
}

void CYLinearization::OnDeltaposIndexud(NMHDR *pNMHDR, LRESULT *pResult)
{
int Delta;

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	Delta = pNMUpDown->iDelta;

	if(ylVars.CurrentEditVar == MAX_TABLE_SLOT && Delta > 0) return;

	if(Delta > 0) {
		if(ylVars.CurrentEditVar < MAX_TABLE_SLOT) {
			if(abs(Delta) < 5) {
				ylVars.CurrentEditVar++;
			}
			else if(abs(Delta) < 10) {
				ylVars.CurrentEditVar += 5;
			}
			else if(abs(Delta) < 20) {
				ylVars.CurrentEditVar += 10;
			}
			else {
				ylVars.CurrentEditVar += 20;
			}

		}
		if(ylVars.CurrentEditVar > MAX_TABLE_SLOT) ylVars.CurrentEditVar = MAX_TABLE_SLOT;
	}
	else {
		if( ylVars.CurrentEditVar > 0) {
			if(abs(Delta) < 5) {
				ylVars.CurrentEditVar--;
			}
			else if(abs(Delta) < 10) {
				ylVars.CurrentEditVar -= 5;
			}
			else if(abs(Delta) < 20) {
				ylVars.CurrentEditVar -= 10;
			}
			else {
				ylVars.CurrentEditVar -= 20;
			}
		}
		if(ylVars.CurrentEditVar < 0) ylVars.CurrentEditVar = 0;
	}

	PopulateSliders();

	*pResult = 0;
}

void CYLinearization::DisplayYParams() {

	if(ylVars.YParams < 1) ylVars.YParams = 4; // default to 1200

	m_YParams.SetCurSel(ylVars.YParams - 1);


}

void CYLinearization::DisplayStripes() {
char tmp[128];

	sprintf(tmp, "%d", ylVars.Rep);

	m_Stripes.SetWindowText(tmp);

	SaveVars(YL_VARS_FN);


}


void CYLinearization::DisplayLM() {
char tmp[128];

	sprintf(tmp, "%.02f", ylVars.LM);

	m_LM.SetWindowText(tmp);

	SaveVars(YL_VARS_FN);


}

void CYLinearization::DisplayCounter() {
char tmp[128];

	sprintf(tmp, "%d", ylVars.iCounter);

	m_Counter.SetWindowText(tmp);

	SaveVars(YL_VARS_FN);


}


void CYLinearization::DisplaySteps() {
char tmp[128];

	sprintf(tmp, "%d", ylVars.YAdvance);

	m_Steps.SetWindowText(tmp);

	SaveVars(YL_VARS_FN);

}

void CYLinearization::OnDeltaposFeedud(NMHDR *pNMHDR, LRESULT *pResult)
{
static int lastYParams;

	if(lastYParams != ylVars.YParams || theApp.GVars->bTrayPosChange) {
		USBPrinterCommand(USB_CMD_SET_Y_RESOLUTION, YReses[ylVars.YParams - 1]);
		USBPrinterCommand(USB_CMD_SET_Y_PRAMS, ylVars.YParams + 3);
		theApp.GVars->bTrayPosChange = FALSE;
	}
	
	lastYParams = ylVars.YParams;

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) {
		USBPrinterCommand(USB_CMD_FEED_FORWARD, ylVars.YAdvance);
		ylVars.iCounter--;
		if(	ylVars.CurrentEditVar > 0) {
			ylVars.CurrentEditVar--;
			PopulateSliders();
		}
	}
	else {
		USBPrinterCommand(USB_CMD_FEED_BACKWARDS, ylVars.YAdvance);
		ylVars.iCounter++;
		if(	ylVars.CurrentEditVar < 93) {
			ylVars.CurrentEditVar++;
			PopulateSliders();
		}

	}
	DisplayCounter();
	Sleep(500);
	*pResult = 0;
}


void CYLinearization::OnDeltaposUd1000(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) ylVars.YAdvance += 1000;
	else if(ylVars.YAdvance > 1000) ylVars.YAdvance -= 1000;

	DisplaySteps();


	*pResult = 0;
}


void CYLinearization::OnDeltaposUd100(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) ylVars.YAdvance += 100;
	else if(ylVars.YAdvance > 1000) ylVars.YAdvance -= 100;

	DisplaySteps();

	*pResult = 0;
}


void CYLinearization::OnDeltaposUd10(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) ylVars.YAdvance += 10;
	else if(ylVars.YAdvance > 1000) ylVars.YAdvance -= 10;

	DisplaySteps();

	*pResult = 0;
}


void CYLinearization::OnDeltaposUd1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) ylVars.YAdvance++;
	else if(ylVars.YAdvance > 1000) ylVars.YAdvance--;

	DisplaySteps();

	*pResult = 0;
}


void CYLinearization::OnDestroy()
{
	SaveVars(YL_VARS_FN);

	ylVars.bExternalCall = TRUE;

	CDialogEx::OnDestroy();

	theApp.bBlockTempThread = FALSE;
}


void CYLinearization::OnDeltaposValud(NMHDR *pNMHDR, LRESULT *pResult)
{
char tmp_buf[20];

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) ylVars.CorVals[ylVars.iCurTestPattern][ylVars.CurrentEditVar] ++;
	else ylVars.CorVals[ylVars.iCurTestPattern][ylVars.CurrentEditVar] --;

	// Current Value
	sprintf(tmp_buf, "%d", ylVars.CorVals[ylVars.iCurTestPattern][ylVars.CurrentEditVar]);
	m_CorValCur.SetWindowText(tmp_buf);


	SaveVars(YL_VARS_FN);

	*pResult = 0;
}


void CYLinearization::OnDeltaposLmud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) ylVars.LM += .25;
	else if(ylVars.LM > 0) ylVars.LM -= .25;

	DisplayLM();

	*pResult = 0;
}


void CYLinearization::OnDeltaposStripesud(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) ylVars.Rep ++;
	else if(ylVars.Rep > 0) ylVars.Rep --;

	DisplayStripes();

	*pResult = 0;
}


void CYLinearization::OnBnClickedCancelPrint() {

	ylVars.CancelRequest = TRUE;


}


void CYLinearization::OnBnClickedPrintTest() {

	ylVars.CancelRequest = FALSE;

	/*
	if(ylVars.SendThreadRunning) {
		theDlg->OKBox("Can't Send", "Sending Flag", "Still Set");
		return;
	}
	*/


	AlignementTestThread = AfxBeginThread((AFX_THREADPROC)PrintAlignmentTest,
														(void *)&ylVars,
														0,
														0,	
														0,
														&theApp.ThreadSA);
	Sleep(250);


}


void SendValuesThread(void *p) {
int i;
signed short index, value;
unsigned long comb_val;
CYLinearization *dlg = (CYLinearization *)p;

	dlg->ylVars.SendThreadRunning  = TRUE;

	if(!dlg->ylVars.bExternalCall) {

		dlg->m_Progress.SetRange32(0, MAX_TABLE_SLOT);
		dlg->m_Status.SetWindowText("Sending Values");
	}
	
	USBPrinterCommand(USB_CMD_SET_STRIPE_1200, dlg->ylVars.YAdvance);

	for(i = 0; i < YL_TABLE_LENGTH; i++) {
	
		comb_val = i << 16;

		comb_val |= (dlg->ylVars.CorVals[dlg->ylVars.iCurTestPattern][i] & 0xFFFF);

		USBPrinterCommand(USB_CMD_SET_YCOR_ELEMENT, comb_val);

		// TRACE("V: %d\n", comb_val);

		if(!dlg->ylVars.bExternalCall) {
			dlg->m_Progress.SetPos(i);
		}

		Sleep(25);
	
	}

	if(!dlg->ylVars.bExternalCall) {
		dlg->m_Progress.SetPos(0);
		dlg->m_Status.SetWindowTextA("");
		dlg->MessageBox("Value Send Complete", "Genesis Ctrl", MB_OK|MB_SYSTEMMODAL);
	}

	dlg->ylVars.SendThreadRunning = FALSE;

}

void SendValuesThreadExter(void *p) {
int i;
signed short index, value;
unsigned long comb_val;
CYLinearization *dlg = (CYLinearization *)p;

	dlg->ylVars.SendThreadRunning  = TRUE;

	if(dlg->ylVars.iCurTestPattern == 1) {
		dlg->ylVars.YAdvance = theApp.GVars->Advance[2];
		dlg->ylVars.YParams = 3;
	}
	else {
		dlg->ylVars.YParams = 4;
		dlg->ylVars.YAdvance = theApp.GVars->Advance[3];
	}

	USBPrinterCommand(USB_CMD_SET_STRIPE_1200, dlg->ylVars.YAdvance);

	for(i = 0; i < YL_TABLE_LENGTH; i++) {
	
		comb_val = i << 16;

		comb_val |= (dlg->ylVars.CorVals[dlg->ylVars.iCurTestPattern][i] & 0xFFFF);

		USBPrinterCommand(USB_CMD_SET_YCOR_ELEMENT, comb_val);

		Sleep(25);
	
	}

	dlg->ylVars.SendThreadRunning = FALSE;

}


void CYLinearization::SendValues(void *p) {
int timeout = 0;


	if(ylVars.SendThreadRunning) return;

	ylVars.CancelRequest = FALSE;

	AlignementTestThread = AfxBeginThread((AFX_THREADPROC)SendValuesThreadExter,
														(void *)p,
														0,
														0,	
														0,
														&theApp.ThreadSA);
	Sleep(250);

	while(ylVars.SendThreadRunning) {
		timeout++;
		Sleep(1);
		if(timeout > 30000) {
			theDlg->OKBox("TIMEOUT while", "sending Y correction", "values");
			break;
		}
	}

}


void CYLinearization::OnBnClickedSendvalues() {

	if(ylVars.SendThreadRunning) return;

	ylVars.bExternalCall = 0;

	ylVars.CancelRequest = FALSE;

	AlignementTestThread = AfxBeginThread((AFX_THREADPROC)SendValuesThread,
														(void *)this,
														0,
														0,	
														0,
														&theApp.ThreadSA);

	Sleep(250);

	//SendValues();

}


void CYLinearization::OnBnClickedReset()
{
	ylVars.iCounter = 0;
	DisplayCounter();
}


void CYLinearization::OnBnClickedGotozero() {

	USBPrinterCommand(USB_CMD_GOTO_STRIPE, 0);
	ylVars.CurrentEditVar = 0;
	PopulateSliders();


}


void CYLinearization::OnBnClickedGotoselected() {

	USBPrinterCommand(USB_CMD_GOTO_STRIPE, ylVars.CurrentEditVar);
	

}


void CYLinearization::OnCbnSelchangeYparams()
{
	ylVars.YParams = m_YParams.GetCurSel() + 1;
}


void CYLinearization::OnBnClickedVerifyBands()
{
	ylVars.bVerifyBands = m_VerifyBands.GetCheck();
}

void CYLinearization::DisplayVerifyBands() {
	m_VerifyBands.SetCheck(ylVars.bVerifyBands);
}

void CYLinearization::OnCbnSelchangeTestPattern()
{

	ylVars.iCurTestPattern = m_TestPattern.GetCurSel();

	SaveVars(YL_VARS_FN);

	if(ylVars.iCurTestPattern == 1) {
		ylVars.YAdvance = theApp.GVars->Advance[2];
		ylVars.YParams = 3;
	}
	else {
		ylVars.YParams = 4;
		ylVars.YAdvance = theApp.GVars->Advance[3];
	}
	DisplaySteps();
	DisplayYParams();
	PopulateSliders();



}


void CYLinearization::OnEnChangeReps2() {
 


}


void CYLinearization::OnEnChangeLm()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CYLinearization::OnCbnSelchangeInksetup()
{
	ylVars.iCurInkSetup = m_InkOrder.GetCurSel();
	SaveVars(YL_VARS_FN);
}

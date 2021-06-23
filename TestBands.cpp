// TestBands.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "TestBands.h"
#include "afxdialogex.h"
#include "parser.h"
#include "head_structs.h"

// CTestBands dialog

char szTestFiles[4][MAX_PATH];

extern BOOL SelectTestBandFile(char *);

extern void	PrintTestBand(void *);
extern void USBPrinterCommand(unsigned long , unsigned long);

BOOL CopyFlags[4];

IMPLEMENT_DYNAMIC(CTestBands, CDialogEx)

	CTestBands::CTestBands(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestBands::IDD, pParent)
{

}

CTestBands::~CTestBands()
{
}

void CTestBands::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UD_1000, m_Spin1000);
	DDX_Control(pDX, IDC_UD_100, m_Spin100);
	DDX_Control(pDX, IDC_UD_10, m_Spin10);
	DDX_Control(pDX, IDC_UD_1, m_Spin1);
	DDX_Control(pDX, IDC_REPS2, m_Steps);
	DDX_Control(pDX, IDC_BITMAP1, m_Bitmap1);
	DDX_Control(pDX, IDC_BITMAP2, m_Bitmap2);
	DDX_Control(pDX, IDC_BITMAP3, m_Bitmap3);
	DDX_Control(pDX, IDC_BITMAP4, m_Bitmap4);
	DDX_Control(pDX, IDC_CHANNEL_1, m_Channel1);
	DDX_Control(pDX, IDC_CHANNEL_2, m_Channel2);
	DDX_Control(pDX, IDC_CHANNEL_3, m_Channel3);
	DDX_Control(pDX, IDC_CHANNEL_4, m_Channel4);
	DDX_Control(pDX, IDC_REPS, m_Reps);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_STATUS, m_Status);
	DDX_Control(pDX, IDC_X_RESOLUTION, m_XResolution);
	DDX_Control(pDX, IDC_REP_UD, m_RepUD);
	DDX_Control(pDX, IDC_FORCE_ONEBIT, m_ForceOneBit);
	DDX_Control(pDX, IDC_VERIFY_BANDS, m_VerifyBands);
	DDX_Control(pDX, IDC_BIDIRECTIONAL, m_Bidirectional);
	DDX_Control(pDX, IDC_TOP_UD, m_TopUD);
	DDX_Control(pDX, IDC_TOP, m_Top);
	DDX_Control(pDX, IDC_YPARAMS, m_YParams);
	DDX_Control(pDX, IDC_LM2, m_LM);
	DDX_Control(pDX, IDC_LMUD2, m_LMUD);
	DDX_Control(pDX, IDC_COPY_TO_A, m_CopyToA);
	DDX_Control(pDX, IDC_COPY_TO_B, m_CopyToB);
	DDX_Control(pDX, IDC_COPY_TO_C, m_CopyToC);
	DDX_Control(pDX, IDC_COPY_TO_D, m_CopyToD);
}


BEGIN_MESSAGE_MAP(CTestBands, CDialogEx)

	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_1, &CTestBands::OnDeltaposUd1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_10, &CTestBands::OnDeltaposUd10)
	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_100, &CTestBands::OnDeltaposUd100)
	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_1000, &CTestBands::OnDeltaposUd1000)
	ON_BN_CLICKED(IDC_BROWSE1, &CTestBands::OnBnClickedBrowse1)
	ON_BN_CLICKED(IDC_BROWSE2, &CTestBands::OnBnClickedBrowse2)
	ON_BN_CLICKED(IDC_BROWSE3, &CTestBands::OnBnClickedBrowse3)
	ON_BN_CLICKED(IDC_BROWSE4, &CTestBands::OnBnClickedBrowse4)
	ON_BN_CLICKED(IDC_PRINT, &CTestBands::OnBnClickedPrint)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_BITMAP1, &CTestBands::OnEnChangeBitmap1)
	ON_EN_CHANGE(IDC_BITMAP2, &CTestBands::OnEnChangeBitmap2)
	ON_EN_CHANGE(IDC_BITMAP3, &CTestBands::OnEnChangeBitmap3)
	ON_EN_CHANGE(IDC_BITMAP4, &CTestBands::OnEnChangeBitmap4)
	ON_CBN_SELCHANGE(IDC_CHANNEL_1, &CTestBands::OnCbnSelchangeChannel1)
	ON_CBN_SELCHANGE(IDC_CHANNEL_2, &CTestBands::OnCbnSelchangeChannel2)
	ON_CBN_SELCHANGE(IDC_CHANNEL_3, &CTestBands::OnCbnSelchangeChannel3)
	ON_CBN_SELCHANGE(IDC_CHANNEL_4, &CTestBands::OnCbnSelchangeChannel4)
	ON_EN_CHANGE(IDC_REPS, &CTestBands::OnEnChangeReps)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestBands::OnBnClickedButton1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_REP_UD, &CTestBands::OnDeltaposRepUd)
	ON_BN_CLICKED(IDC_FORCE_ONEBIT, &CTestBands::OnBnClickedForceOnebit)
	ON_BN_CLICKED(IDC_VERIFY_BANDS, &CTestBands::OnBnClickedVerifyBands)
	ON_BN_CLICKED(IDC_BIDIRECTIONAL, &CTestBands::OnBnClickedBidirectional)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TOP_UD, &CTestBands::OnDeltaposTopUd)
	ON_CBN_SELCHANGE(IDC_YPARAMS, &CTestBands::OnCbnSelchangeYparams)
	ON_NOTIFY(UDN_DELTAPOS, IDC_LMUD2, &CTestBands::OnDeltaposLmud2)
END_MESSAGE_MAP()


// CTestBands message handlers

void CTestBands::SaveVars(char *szFileName) {
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

		m_Reps.GetWindowText(tmp_buff, MAX_PATH);
		tbVars.Rep = atoi(tmp_buff);
		tbVars.ChannelToUse[0] = m_Channel1.GetCurSel();
		tbVars.ChannelToUse[1] = m_Channel2.GetCurSel();
		tbVars.ChannelToUse[2] = m_Channel3.GetCurSel();
		tbVars.ChannelToUse[3] = m_Channel4.GetCurSel();
		tbVars.XResolution = m_XResolution.GetCurSel();

		m_Bitmap1.GetWindowText(tbVars.szTestFiles[0], MAX_PATH);
		m_Bitmap2.GetWindowText(tbVars.szTestFiles[1], MAX_PATH);
		m_Bitmap3.GetWindowText(tbVars.szTestFiles[2], MAX_PATH);
		m_Bitmap4.GetWindowText(tbVars.szTestFiles[3], MAX_PATH);

		m_Steps.GetWindowText(tmp_buff, MAX_PATH);

		tbVars.YAdvance = atoi(tmp_buff);

		WriteFile(fh, (char *)&tbVars, sizeof(tbVars), &bytes, NULL);

		CloseHandle(fh);
	}


}


void CTestBands::LoadVars(char *szFileName) {
HANDLE fh;
unsigned long bytes;
char tmp_buff[MAX_PATH];

	memset((char *)&tbVars, 0, sizeof(testbandVars));

	fh = CreateFile(szFileName,
					GENERIC_READ,
					0,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);

	if(fh != INVALID_HANDLE_VALUE) {

		ReadFile(fh, (char *)&tbVars, sizeof(tbVars), &bytes, NULL);

		sprintf(tmp_buff, "%d", tbVars.Rep);
		m_Reps.SetWindowText(tmp_buff);

		m_Channel1.SetCurSel(tbVars.ChannelToUse[0]);
		m_Channel2.SetCurSel(tbVars.ChannelToUse[1]);
		m_Channel3.SetCurSel(tbVars.ChannelToUse[2]);
		m_Channel4.SetCurSel(tbVars.ChannelToUse[3]);

		m_Bitmap1.SetWindowText(tbVars.szTestFiles[0]);
		m_Bitmap2.SetWindowText(tbVars.szTestFiles[1]);
		m_Bitmap3.SetWindowText(tbVars.szTestFiles[2]);
		m_Bitmap4.SetWindowText(tbVars.szTestFiles[3]);

		m_XResolution.SetCurSel(tbVars.XResolution);

		sprintf(tmp_buff, "%d", tbVars.YAdvance);
		m_Steps.SetWindowText(tmp_buff);

		CloseHandle(fh);
	
	}

	tbVars.SendThreadRunning = FALSE;
	tbVars.Progress = &m_Progress;
	tbVars.Status = &m_Status;

}

void CTestBands::DisplaySteps() {
char tmp[128];

	sprintf(tmp, "%d", tbVars.YAdvance);

	m_Steps.SetWindowText(tmp);

	SaveVars(TB_VARS_FN);

}

void CTestBands::DisplayReps() {
char tmp[128];

	sprintf(tmp, "%d", tbVars.Rep);

	m_Reps.SetWindowText(tmp);

	SaveVars(TB_VARS_FN);
}

void CTestBands::DisplayForceOneBit() {

	m_ForceOneBit.SetCheck(tbVars.bForce1Bit);

	SaveVars(TB_VARS_FN);
}

void CTestBands::DisplayVerifyBands() {

	m_VerifyBands.SetCheck(tbVars.bVerifyBands);

	SaveVars(TB_VARS_FN);
}

void CTestBands::DisplayBidirectional() {

	m_Bidirectional.SetCheck(tbVars.bBiDirectional);

	SaveVars(TB_VARS_FN);
}


void CTestBands::OnDeltaposUd1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	if(pNMUpDown->iDelta < 0) tbVars.YAdvance++;
	else if(tbVars.YAdvance > 0) tbVars.YAdvance--;

	DisplaySteps();


	*pResult = 0;
}

void CTestBands::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);

}


BOOL CTestBands::OnInitDialog()
{
	OldBHT = theApp.bBlockTempThread;

	theApp.bBlockTempThread = TRUE;

	CDialogEx::OnInitDialog();

	SizeDialog();
	


	WINDOWPLACEMENT  wp1, wp2, wp3, wp4, wp5, wp6, wp7;

	m_Spin1000.GetWindowPlacement(&wp1);
	m_Spin100.GetWindowPlacement(&wp2);
	m_Spin10.GetWindowPlacement(&wp3);
	m_Spin1.GetWindowPlacement(&wp4);
	m_RepUD.GetWindowPlacement(&wp5);
	m_TopUD.GetWindowPlacement(&wp6);
	m_LMUD.GetWindowPlacement(&wp7);

	wp1.rcNormalPosition.right = wp1.rcNormalPosition.left + 70;
	wp2.rcNormalPosition.right = wp2.rcNormalPosition.left + 70;
	wp3.rcNormalPosition.right = wp3.rcNormalPosition.left + 70;
	wp4.rcNormalPosition.right = wp4.rcNormalPosition.left + 70;
	wp5.rcNormalPosition.right = wp5.rcNormalPosition.left + 70;
	wp6.rcNormalPosition.right = wp6.rcNormalPosition.left + 70;
	wp7.rcNormalPosition.right = wp7.rcNormalPosition.left + 70;

	m_Spin1000.SetWindowPlacement(&wp1);
	m_Spin100.SetWindowPlacement(&wp2);
	m_Spin10.SetWindowPlacement(&wp3);
	m_Spin1.SetWindowPlacement(&wp4);
	m_RepUD.SetWindowPlacement(&wp5);
	m_TopUD.SetWindowPlacement(&wp6);
	m_LMUD.SetWindowPlacement(&wp7);

	m_Channel1.SetCurSel(0);
	m_Channel2.SetCurSel(0);
	m_Channel3.SetCurSel(0);
	m_Channel4.SetCurSel(0);

	

	LoadVars(TB_VARS_FN);

	DisplaySteps();

	DisplayVerifyBands();

	DisplayTop();

	DisplayYParams();

	DisplayLM();


	char tmp_buff[30];

	sprintf(tmp_buff, "%d", tbVars.Rep);
	m_Reps.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%d", tbVars.YAdvance);
	m_Steps.SetWindowText(tmp_buff);

	m_Progress.SetRange32(0, 100);

	m_ForceOneBit.SetCheck(tbVars.bForce1Bit);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTestBands::OnDeltaposUd10(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) tbVars.YAdvance += 10;
	else if(tbVars.YAdvance > 10) tbVars.YAdvance -= 10;

	DisplaySteps();

	*pResult = 0;
}


void CTestBands::OnDeltaposUd100(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) tbVars.YAdvance += 100;
	else if(tbVars.YAdvance > 100) tbVars.YAdvance -= 100;

	DisplaySteps();

	*pResult = 0;
}


void CTestBands::OnDeltaposUd1000(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) tbVars.YAdvance += 1000;
	else if(tbVars.YAdvance > 1000) tbVars.YAdvance -= 1000;

	DisplaySteps();
	*pResult = 0;
}


void CTestBands::OnBnClickedBrowse1() {
char szTmpFile[MAX_PATH];

	if(SelectTestBandFile(szTmpFile)) {
		strcpy(szTestFiles[0], szTmpFile);
		m_Bitmap1.SetWindowText(szTestFiles[0]);
	}

}


void CTestBands::OnBnClickedBrowse2()
{
char szTmpFile[MAX_PATH];

	if(SelectTestBandFile(szTmpFile)) {
		strcpy(szTestFiles[1], szTmpFile);
		m_Bitmap2.SetWindowText(szTestFiles[1]);
	}

}



void CTestBands::OnBnClickedBrowse3()
{
char szTmpFile[MAX_PATH];

	if(SelectTestBandFile(szTmpFile)) {
		strcpy(szTestFiles[2], szTmpFile);
		m_Bitmap3.SetWindowText(szTestFiles[2]);
	}

}


void CTestBands::OnBnClickedBrowse4()
{
char szTmpFile[MAX_PATH];

	if(SelectTestBandFile(szTmpFile)) {
		strcpy(szTestFiles[3], szTmpFile);
		m_Bitmap4.SetWindowText(szTestFiles[3]);
	}

}

void CTestBands::DisplayTop() {
char tmp_buff[100];

	sprintf(tmp_buff, "%.02f", tbVars.Top);

	m_Top.SetWindowText(tmp_buff);

	SaveVars(TB_VARS_FN);


}

void CTestBands::DisplayLM() {
char tmp[128];

	sprintf(tmp, "%.02f", tbVars.LM);

	m_LM.SetWindowText(tmp);

	SaveVars(TB_VARS_FN);


}


void CTestBands::OnBnClickedPrint() {

	if(tbVars.SendThreadRunning) return;

	//PrintTestBand((void *)&tbVars);

	CopyFlags[0] = m_CopyToA.GetCheck();
	CopyFlags[1] = m_CopyToB.GetCheck();
	CopyFlags[2] = m_CopyToC.GetCheck();
	CopyFlags[3] = m_CopyToD.GetCheck();

	TestBandThread = AfxBeginThread((AFX_THREADPROC)PrintTestBand,
														(void *)&tbVars,
														0,
														0,	
														0,
														&theApp.ThreadSA);
	Sleep(250);

}


void CTestBands::OnDestroy()
{
	
	SaveVars(TB_VARS_FN);

	theApp.bBlockTempThread = OldBHT;

	CDialogEx::OnDestroy();


}


void CTestBands::OnEnChangeBitmap1()
{
	SaveVars(TB_VARS_FN);
}


void CTestBands::OnEnChangeBitmap2()
{
	SaveVars(TB_VARS_FN);
}


void CTestBands::OnEnChangeBitmap3()
{
	SaveVars(TB_VARS_FN);
}


void CTestBands::OnEnChangeBitmap4()
{
	SaveVars(TB_VARS_FN);
}


void CTestBands::OnCbnSelchangeChannel1()
{
	SaveVars(TB_VARS_FN);
}


void CTestBands::OnCbnSelchangeChannel2()
{
	SaveVars(TB_VARS_FN);
}


void CTestBands::OnCbnSelchangeChannel3()
{
	SaveVars(TB_VARS_FN);
}


void CTestBands::OnCbnSelchangeChannel4()
{
	SaveVars(TB_VARS_FN);
}


void CTestBands::OnEnChangeReps()
{
	SaveVars(TB_VARS_FN);
}


void CTestBands::OnBnClickedButton1()
{
	USBPrinterCommand(USB_CMD_CAP, 0);
	USBPrinterCommand(USB_CMD_PARK, 0);
}


void CTestBands::OnDeltaposRepUd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) tbVars.Rep ++;
	else if(tbVars.YAdvance > 1) tbVars.Rep--;

	DisplayReps();


	*pResult = 0;
}


void CTestBands::OnBnClickedForceOnebit()
{

	tbVars.bForce1Bit = m_ForceOneBit.GetCheck();

	DisplayForceOneBit();

}


void CTestBands::OnBnClickedVerifyBands()
{
	tbVars.bVerifyBands = m_VerifyBands.GetCheck();

	DisplayVerifyBands();
}


void CTestBands::OnBnClickedBidirectional()
{
	tbVars.bBiDirectional = m_Bidirectional.GetCheck();

	DisplayBidirectional();
}


void CTestBands::OnDeltaposTopUd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) tbVars.Top += .25;
	else if(tbVars.YAdvance > 0) tbVars.Top -= .25;

	DisplayTop();


	*pResult = 0;
}


void CTestBands::OnCbnSelchangeYparams()
{
	tbVars.YParams = m_YParams.GetCurSel() + 1;

}

void CTestBands::DisplayYParams() {

	if(tbVars.YParams < 1) tbVars.YParams = 4; // default to 1200

	m_YParams.SetCurSel(tbVars.YParams - 1);


}


void CTestBands::OnDeltaposLmud2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) tbVars.LM += .25;
	else if(tbVars.LM > 0) tbVars.LM -= .25;

	DisplayLM();

	*pResult = 0;
}

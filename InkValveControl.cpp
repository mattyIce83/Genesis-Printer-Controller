// InkValveControl.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "InkValveControl.h"
#include "afxdialogex.h"
#include "parser.h"

#include "HEAD_structs.h"

#define STEP_DELAY	50

static 	CFont pNewFont;

extern void USBPrinterCommand(unsigned long , unsigned long);

// CInkValveControl dialog

IMPLEMENT_DYNAMIC(CInkValveControl, CDialogEx)

CInkValveControl::CInkValveControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInkValveControl::IDD, pParent)
{

}

CInkValveControl::~CInkValveControl()
{
}

void CInkValveControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAN_LAB_1, m_ChanLab1);
	DDX_Control(pDX, IDC_CHAN_LAB_2, m_ChanLab2);
	DDX_Control(pDX, IDC_CHAN_LAB_3, m_ChanLab3);
	DDX_Control(pDX, IDC_CHAN_LAB_4, m_ChanLab4);
	DDX_Control(pDX, IDC_CHAN_LAB_5, m_ChanLab5);
	DDX_Control(pDX, IDC_CHAN_LAB_6, m_ChanLab6);
	DDX_Control(pDX, IDC_CLOSE_INK_VALVE_1, m_Close1);
	DDX_Control(pDX, IDC_CLOSE_INK_VALVE_2, m_Close2);
	DDX_Control(pDX, IDC_CLOSE_INK_VALVE_3, m_Close3);
	DDX_Control(pDX, IDC_CLOSE_INK_VALVE_4, m_Close4);
	DDX_Control(pDX, IDC_CLOSE_INK_VALVE_5, m_Close5);
	DDX_Control(pDX, IDC_CLOSE_INK_VALVE_6, m_Close6);
	DDX_Control(pDX, IDC_FILL_CH_1, m_Fill1);
	DDX_Control(pDX, IDC_FILL_CH_2, m_Fill2);
	DDX_Control(pDX, IDC_FILL_CH_3, m_Fill3);
	DDX_Control(pDX, IDC_FILL_CH_4, m_Fill4);
	DDX_Control(pDX, IDC_FILL_CH_5, m_Fill5);
	DDX_Control(pDX, IDC_FILL_CH_6, m_Fill6);
	DDX_Control(pDX, IDC_OPEN_INK_VALVE_1, m_Open1);
	DDX_Control(pDX, IDC_OPEN_INK_VALVE_2, m_Open2);
	DDX_Control(pDX, IDC_OPEN_INK_VALVE_3, m_Open3);
	DDX_Control(pDX, IDC_OPEN_INK_VALVE_4, m_Open4);
	DDX_Control(pDX, IDC_OPEN_INK_VALVE_6, m_Open6);
	DDX_Control(pDX, IDC_OPEN_INK_VALVE_5, m_Open5);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_FILL_ALL, m_FillAll);
	DDX_Control(pDX, IDC_OPEN_ALL, m_OpenAll);
	DDX_Control(pDX, IDC_CLOSE_ALL, m_CloseAll);
	DDX_Control(pDX, IDC_POS_IND_1, m_PosInd1);
	DDX_Control(pDX, IDC_POS_IND_2, m_PosInd2);
	DDX_Control(pDX, IDC_POS_IND_3, m_PosInd3);
	DDX_Control(pDX, IDC_POS_IND_4, m_PosInd4);
	DDX_Control(pDX, IDC_POS_IND_5, m_PosInd5);
	DDX_Control(pDX, IDC_POS_IND_6, m_PosInd6);
}


BEGIN_MESSAGE_MAP(CInkValveControl, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CInkValveControl::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OPEN_INK_VALVE_1, &CInkValveControl::OnBnClickedOpenInkValve1)
	ON_BN_CLICKED(IDC_OPEN_INK_VALVE_2, &CInkValveControl::OnBnClickedOpenInkValve2)
	ON_BN_CLICKED(IDC_OPEN_INK_VALVE_3, &CInkValveControl::OnBnClickedOpenInkValve3)
	ON_BN_CLICKED(IDC_OPEN_INK_VALVE_4, &CInkValveControl::OnBnClickedOpenInkValve4)
	ON_BN_CLICKED(IDC_OPEN_INK_VALVE_5, &CInkValveControl::OnBnClickedOpenInkValve5)
	ON_BN_CLICKED(IDC_OPEN_INK_VALVE_6, &CInkValveControl::OnBnClickedOpenInkValve6)
	ON_BN_CLICKED(IDC_CLOSE_INK_VALVE_1, &CInkValveControl::OnBnClickedCloseInkValve1)
	ON_BN_CLICKED(IDC_CLOSE_INK_VALVE_2, &CInkValveControl::OnBnClickedCloseInkValve2)
	ON_BN_CLICKED(IDC_CLOSE_INK_VALVE_3, &CInkValveControl::OnBnClickedCloseInkValve3)
	ON_BN_CLICKED(IDC_CLOSE_INK_VALVE_4, &CInkValveControl::OnBnClickedCloseInkValve4)
	ON_BN_CLICKED(IDC_CLOSE_INK_VALVE_5, &CInkValveControl::OnBnClickedCloseInkValve5)
	ON_BN_CLICKED(IDC_CLOSE_INK_VALVE_6, &CInkValveControl::OnBnClickedCloseInkValve6)
	ON_BN_CLICKED(IDC_FILL_CH_1, &CInkValveControl::OnBnClickedFillCh1)
	ON_BN_CLICKED(IDC_FILL_CH_2, &CInkValveControl::OnBnClickedFillCh2)
	ON_BN_CLICKED(IDC_FILL_CH_3, &CInkValveControl::OnBnClickedFillCh3)
	ON_BN_CLICKED(IDC_FILL_CH_4, &CInkValveControl::OnBnClickedFillCh4)
	ON_BN_CLICKED(IDC_FILL_CH_5, &CInkValveControl::OnBnClickedFillCh5)
	ON_BN_CLICKED(IDC_FILL_CH_6, &CInkValveControl::OnBnClickedFillCh6)
	ON_BN_CLICKED(IDC_OPEN_ALL, &CInkValveControl::OnBnClickedOpenAll)
	ON_BN_CLICKED(IDC_CLOSE_ALL, &CInkValveControl::OnBnClickedCloseAll)
	ON_BN_CLICKED(IDC_FILL_ALL, &CInkValveControl::OnBnClickedFillAll)
END_MESSAGE_MAP()


// CInkValveControl message handlers


void CInkValveControl::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


BOOL CInkValveControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFont* pOldFont = m_ChanLab1.GetFont();

	LOGFONT logfont;

	pOldFont->GetObject(sizeof(logfont),&logfont);
 
	logfont.lfHeight = 35;
	logfont.lfWidth = 15;


	logfont.lfWeight = FW_HEAVY;
 
	logfont.lfPitchAndFamily = FF_DONTCARE;
 
	strcpy(logfont.lfFaceName, "Helvetica");  
 
	logfont.lfEscapement = logfont.lfOrientation = 0;
 
	pNewFont.CreateFontIndirect(&logfont);

	m_ChanLab1.SetFont(&pNewFont);
	m_ChanLab2.SetFont(&pNewFont);
	m_ChanLab3.SetFont(&pNewFont);
	m_ChanLab4.SetFont(&pNewFont);
	m_ChanLab5.SetFont(&pNewFont);
	m_ChanLab6.SetFont(&pNewFont);
	m_Close1.SetFont(&pNewFont);
	m_Close2.SetFont(&pNewFont);
	m_Close3.SetFont(&pNewFont);
	m_Close4.SetFont(&pNewFont);
	m_Close5.SetFont(&pNewFont);
	m_Close6.SetFont(&pNewFont);
	m_Fill1.SetFont(&pNewFont);
	m_Fill2.SetFont(&pNewFont);
	m_Fill3.SetFont(&pNewFont);
	m_Fill4.SetFont(&pNewFont);
	m_Fill5.SetFont(&pNewFont);
	m_Fill6.SetFont(&pNewFont);
	m_Open1.SetFont(&pNewFont);
	m_Open2.SetFont(&pNewFont);
	m_Open3.SetFont(&pNewFont);
	m_Open4.SetFont(&pNewFont);
	m_Open6.SetFont(&pNewFont);
	m_Open5.SetFont(&pNewFont);
	m_btnOK.SetFont(&pNewFont);
	m_OpenAll.SetFont(&pNewFont);
	m_CloseAll.SetFont(&pNewFont);
	m_FillAll.SetFont(&pNewFont);
	
	m_PosInd1.SetRange(0, 20);
	m_PosInd2.SetRange(0, 20);
	m_PosInd3.SetRange(0, 20);
	m_PosInd4.SetRange(0, 20);
	m_PosInd5.SetRange(0, 20);
	m_PosInd6.SetRange(0, 20);

	indicators[0] = &m_PosInd1;
	indicators[1] = &m_PosInd2;
	indicators[2] = &m_PosInd3;
	indicators[3] = &m_PosInd4;
	indicators[4] = &m_PosInd5;
	indicators[5] = &m_PosInd6;

	int i;
	
	for(i = 0; i < 6; i++) state[i]	= 0;

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CInkValveControl::OnDestroy()
{
	CDialogEx::OnDestroy();

	pNewFont.DeleteObject();
}

void CInkValveControl::CloseValve(int valve) {
int i, val = 20;


	USBPrinterCommand(USB_CMD_CLOSE_INK_VALVE, valve);

	if(!state[valve]) return;

	for(i = 0; i < 5; i++) {
		indicators[valve]->SetPos(val);
		val -= 5;
		Sleep(STEP_DELAY);
	}

	state[valve] = FALSE;
}

void CInkValveControl::OpenValve(int valve) {
int i, val = 0;

	USBPrinterCommand(USB_CMD_OPEN_INK_VALVE, valve);
	
	if(state[valve]) return;

	for(i = 0; i < 5; i++) {
		indicators[valve]->SetPos(val);
		val += 5;
		Sleep(STEP_DELAY);
	}

	state[valve] = TRUE;

}

void CInkValveControl::FillChannel(int channel) {
int i, val;

	USBPrinterCommand(USB_CMD_FILL_INK, channel);

	val = 0;
	for(i = 0; i < 5; i++) {
		indicators[channel]->SetPos(val);
		val += 5;
		Sleep(STEP_DELAY);
	}

	val = 20;
	for(i = 0; i < 5; i++) {
		indicators[channel]->SetPos(val);
		val -= 5;
		Sleep(STEP_DELAY);
	}

	state[channel] = FALSE;

}

void CInkValveControl::FillAllChannels() {
int c, i, val;

	USBPrinterCommand(USB_CMD_FILL_ALL_INKS, 0);

	for(c = 0; c < 6; c++) {
		val = 0;
		for(i = 0; i < 5; i++) {
			indicators[c]->SetPos(val);
			val += 5;
			Sleep(STEP_DELAY);
		}
	}

	for(c = 0; c < 6; c++) {
		val = 20;
		for(i = 0; i < 5; i++) {
			indicators[c]->SetPos(val);
			val -= 5;
			Sleep(STEP_DELAY);
		}
		state[c] = FALSE;
	}
}

void CInkValveControl::OnBnClickedOpenInkValve1() {
	OpenValve(0);
}


void CInkValveControl::OnBnClickedOpenInkValve2() {
	OpenValve(1);
}


void CInkValveControl::OnBnClickedOpenInkValve3() {
	OpenValve(2);
}


void CInkValveControl::OnBnClickedOpenInkValve4() {
	OpenValve(3);
}


void CInkValveControl::OnBnClickedOpenInkValve5() {
	OpenValve(4);
}


void CInkValveControl::OnBnClickedOpenInkValve6(){
	OpenValve(5);
}


void CInkValveControl::OnBnClickedCloseInkValve1() {
	CloseValve(0);
}

void CInkValveControl::OnBnClickedCloseInkValve2() {
	CloseValve(1);
}

void CInkValveControl::OnBnClickedCloseInkValve3() {
	CloseValve(2);
}

void CInkValveControl::OnBnClickedCloseInkValve4() {
	CloseValve(3);
}

void CInkValveControl::OnBnClickedCloseInkValve5() {
	CloseValve(4);
}


void CInkValveControl::OnBnClickedCloseInkValve6() {
	CloseValve(5);
}

void CInkValveControl::OnBnClickedFillCh1() {
	FillChannel(0);
}


void CInkValveControl::OnBnClickedFillCh2() {
	FillChannel(1);
}

void CInkValveControl::OnBnClickedFillCh3() {
	FillChannel(2);
}


void CInkValveControl::OnBnClickedFillCh4() {
	FillChannel(3);
}

void CInkValveControl::OnBnClickedFillCh5() {
	FillChannel(4);
}


void CInkValveControl::OnBnClickedFillCh6() {
	FillChannel(5);
}


void CInkValveControl::OnBnClickedOpenAll() {
int i, c, val;

	USBPrinterCommand(USB_CMD_OPEN_ALLINK_VALVES, 0);

	for(c = 0; c < 6; c++) {
		if(!state[c]) {
			val = 0;
			for(i = 0; i < 5; i++) {
				indicators[c]->SetPos(val);
				val += 5;
				Sleep(STEP_DELAY);
			}
			state[c] = TRUE;
		}
	}
}


void CInkValveControl::OnBnClickedCloseAll() {
int i, c, val;

	USBPrinterCommand(USB_CMD_CLOSE_ALLINK_VALVES, 0);

	for(c = 0; c < 6; c++) {
		if(state[c]) {
			val = 20;
			for(i = 0; i < 5; i++) {
				indicators[c]->SetPos(val);
				val -= 5;
				Sleep(STEP_DELAY);
			}
			state[c] = FALSE;
		}
	}
}

void CInkValveControl::OnBnClickedFillAll() {
int i;

	FillAllChannels();

}

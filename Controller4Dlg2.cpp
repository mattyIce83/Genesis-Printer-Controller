// Controller4Dlg2.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg2.h"
#include "Controller4Dlg3.h"
#include "afxdialogex.h"
#include "parser.h"
#include "HEAD_structs.h"


#ifdef ENABLE_NETWORK_STUFF
	#include <iphlpapi.h>
#endif

extern CController4App theApp;
extern CController4Dlg1 *theDlg;


extern void SendHeadTemp(int, int);
extern void DisableHeadHeater(BOOL, BOOL);
extern void USBPrinterCommand(unsigned long , unsigned long);
extern BOOL CheckAdvanced();
extern void SendHeaterTimeout();
extern BOOL UnlockAdvanced();

#define printf	TRACE


/*-------------------------------------------------------------------------
- Function to tokenize a line of text                                    -
-------------------------------------------------------------------------*/

short tokenize(char **token, char *string, char *tokstr) {

short  toknum = 0;

     *token = strtok(string, tokstr);

     while (*token != NULL) {
         toknum++;
         *++token = strtok(NULL, tokstr);

      }

      return (toknum);

}


// CController4Dlg2 dialog

IMPLEMENT_DYNAMIC(CController4Dlg2, CDialogEx)

CController4Dlg2::CController4Dlg2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CController4Dlg2::IDD, pParent)
{

}

CController4Dlg2::~CController4Dlg2()
{
}

void CController4Dlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOME, m_Home);
	DDX_Control(pDX, IDC_BIDI_COR, m_BiDiCor_Val);
	DDX_Control(pDX, IDC_BIDI_COR_PLUS, m_BidiCorUP);
	DDX_Control(pDX, IDC_BIDICOR_MINUS, m_BidiCorDN);
	DDX_Control(pDX, IDC_HEADSETUP, m_HeadSetup);
	DDX_Control(pDX, IDC_HEADCONFIG, m_HeadConfig);
	DDX_Control(pDX, IDC_BIDICOR_MINUS2, m_KeepJobsDN);
	DDX_Control(pDX, IDC_BIDI_COR_PLUS2, m_KeepJobsUP);
	DDX_Control(pDX, IDC_KEEPJOBS, m_KeepJobs);
	DDX_Control(pDX, IDC_AUTOHD, m_HDOnOff);
	DDX_Control(pDX, IDC_RAPIDRT, m_RTOnOff);
	DDX_Control(pDX, IDC_PRINTERNAME, m_PrinterName);
	DDX_Control(pDX, IDC_SERVER, m_Server);
	DDX_Control(pDX, IDC_IP1, m_IP1);
	DDX_Control(pDX, IDC_IP2, m_IP2);
	DDX_Control(pDX, IDC_IP3, m_IP3);
	DDX_Control(pDX, IDC_IP4, m_IP4);
	DDX_Control(pDX, IDC_COLORJOBSCLEAN, m_ColorJobsClean);
	DDX_Control(pDX, IDC_WHITEJOBSCLEAN, m_WhiteJobsClean);
	DDX_Control(pDX, IDC_COLOR_CLEAN_JOBS_PLUS, m_ColorJobsCleanUp);
	DDX_Control(pDX, IDC_COLOR_CLEAN_JOBS_PLUS, m_ColorJobsCleanUp);
	DDX_Control(pDX, IDC_COLOR_CLEAN_JOBS_MINUS, m_ColorJobsCleanDown);
	DDX_Control(pDX, IDC_WHITE_CLEAN_JOBS_PLUS, m_WhiteJobsCleanUp);
	DDX_Control(pDX, IDC_WHITE_CLEAN_JOBS_MINUS, m_WhiteJobsCleanDown);
	DDX_Control(pDX, IDC_COLORJOBSCLEAN, m_ColorJobsClean);
	DDX_Control(pDX, IDC_WHITEJOBSCLEAN, m_WhiteJobsClean);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Control(pDX, IDC_OBTAINIPAUTO, m_AutoIP);
	DDX_Control(pDX, IDC_OBTAINIPSTATIC, m_ManualIP);
	DDX_Control(pDX, IDC_SERVERNAME, m_ServerName);
	DDX_Control(pDX, IDC_CONROLLER_VER, m_ControllerVer);
	DDX_Control(pDX, IDC_MAIN_VER, m_MainVer);
	DDX_Control(pDX, IDC_AUX_VER, m_AuxVer);
	DDX_Control(pDX, IDC_SLAVE_VER, m_SlaveVer);
	DDX_Control(pDX, IDC_LAYERDELAY, m_LayerDelaySec);
	DDX_Control(pDX, IDC_CHANGE, m_EditIP);
	DDX_Control(pDX, IDC_ADAPTERS, m_AdapterList);
	DDX_Control(pDX, IDC_CONNECTION_TYPE, m_ConnectionType);
	DDX_Control(pDX, IDC_ONEBUTTON_OPERATION, m_OneButtonOperation);
	DDX_Control(pDX, IDC_DELETE_AFTER_PRINT, m_DeleteJobAfterPrint);
	DDX_Control(pDX, IDC_NETOWRKING_GROUP, m_NetworkingGroup);
	DDX_Control(pDX, IDC_PLT_OFFSET, m_PlatenGap);
	DDX_Control(pDX, IDC_IGNORE_OBSTRUCTION2, m_IgnoreLasers);
	DDX_Control(pDX, IDC_PLTN_SIZE, m_PlatenSize);
	DDX_Control(pDX, IDC_SN, m_SerialNumber);
	DDX_Control(pDX, IDC_PRINTS, m_TotalPrints);
	DDX_Control(pDX, IDC_PLATEN_GAP_PLUS, m_PlatenGapUp);
	DDX_Control(pDX, IDC_PLATEN_GAP_MINUS2, m_PlatenGapDown);
	DDX_Control(pDX, IDC_PLATEN_SIZE_UP, m_PlatenSizeUp);
	DDX_Control(pDX, IDC_PLATEN_SIZE_DOWN, m_PlatenSizeDown);
	DDX_Control(pDX, IDC_LAYERDELAY_UP, m_LayerDelayUp);
	DDX_Control(pDX, IDC_LAYERDELAY_DOWN, m_LayerDelayDown);
	DDX_Control(pDX, IDC_HEATER_TIMEOUT_UP, m_HeaterTimeoutUP);
	DDX_Control(pDX, IDC_HEATER_TIMEOUT_DOWN, m_HeaterTimeoutDown);
	DDX_Control(pDX, IDC_HEATER_TIMEOUT, m_HeaterTimeout);
}


BEGIN_MESSAGE_MAP(CController4Dlg2, CDialogEx)
	ON_BN_CLICKED(IDC_HOME, &CController4Dlg2::OnBnClickedHome)
	ON_BN_CLICKED(IDCANCEL, &CController4Dlg2::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RIGHT, &CController4Dlg2::OnBnClickedRight)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BIDI_COR_PLUS, &CController4Dlg2::OnBnClickedBidiCorPlus)
	ON_BN_CLICKED(IDC_BIDICOR_MINUS, &CController4Dlg2::OnBnClickedBidicorMinus)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_HEADSETUP, &CController4Dlg2::OnBnClickedHeadsetup)
	ON_BN_CLICKED(IDC_BIDI_COR_PLUS2, &CController4Dlg2::OnBnClickedBidiCorPlus2)
	ON_BN_CLICKED(IDC_BIDICOR_MINUS2, &CController4Dlg2::OnBnClickedBidicorMinus2)
	ON_BN_CLICKED(IDC_AUTOHD, &CController4Dlg2::OnBnClickedAutohd)
	ON_BN_CLICKED(IDC_RAPIDRT, &CController4Dlg2::OnBnClickedRapidrt)
	ON_BN_CLICKED(IDC_COLOR_CLEAN_JOBS_PLUS, &CController4Dlg2::OnBnClickedColorCleanJobsPlus)
	ON_BN_CLICKED(IDC_COLOR_CLEAN_JOBS_MINUS, &CController4Dlg2::OnBnClickedColorCleanJobsMinus)
	ON_BN_CLICKED(IDC_WHITE_CLEAN_JOBS_PLUS, &CController4Dlg2::OnBnClickedWhiteCleanJobsPlus)
	ON_BN_CLICKED(IDC_WHITE_CLEAN_JOBS_MINUS, &CController4Dlg2::OnBnClickedWhiteCleanJobsMinus)
	ON_BN_CLICKED(IDC_NETTEST, &CController4Dlg2::OnBnClickedNettest)
	ON_BN_CLICKED(IDC_GET_VERSIONS, &CController4Dlg2::OnBnClickedGetVersions)
	ON_CBN_SELCHANGE(IDC_LAYERDELAY, &CController4Dlg2::OnCbnSelchangeLayerdelaySec)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS, &CController4Dlg2::OnIpnFieldchangedIpaddress)
	ON_CBN_SELCHANGE(IDC_ADAPTERS, &CController4Dlg2::OnCbnSelchangeAdapters)
	ON_BN_CLICKED(IDC_CHANGE, &CController4Dlg2::OnBnClickedChange)
	ON_BN_CLICKED(IDC_DBG, &CController4Dlg2::OnBnClickedDbg)
	ON_BN_CLICKED(IDC_ONEBUTTON_OPERATION, &CController4Dlg2::OnBnClickedOnebuttonOperation)
	ON_BN_CLICKED(IDC_DELETE_AFTER_PRINT, &CController4Dlg2::OnBnClickedDeleteAfterPrint)
	ON_BN_CLICKED(IDC_IGNORE_OBSTRUCTION2, &CController4Dlg2::OnBnClickedIgnoreObstruction2)
	ON_CBN_SELCHANGE(IDC_PLT_OFFSET, &CController4Dlg2::OnCbnSelchangePtOffset)
	ON_CBN_SELCHANGE(IDC_PLTN_SIZE, &CController4Dlg2::OnCbnSelchangePlatenSize)
	ON_BN_CLICKED(IDC_PLATEN_GAP_PLUS, &CController4Dlg2::OnBnClickedPlatenGapPlus)
	ON_BN_CLICKED(IDC_PLATEN_GAP_MINUS2, &CController4Dlg2::OnBnClickedPlatenGapMinus2)
	ON_LBN_SELCHANGE(IDC_PLT_OFFSET, &CController4Dlg2::OnLbnSelchangePltOffset)
	ON_BN_CLICKED(IDC_PLATEN_SIZE_UP, &CController4Dlg2::OnBnClickedPlatenSizeUp)
	ON_BN_CLICKED(IDC_PLATEN_SIZE_DOWN, &CController4Dlg2::OnBnClickedPlatenSizeDown)
	ON_LBN_SELCHANGE(IDC_PLTN_SIZE, &CController4Dlg2::OnLbnSelchangePltnSize)
	ON_BN_CLICKED(IDC_LAYERDELAY_UP, &CController4Dlg2::OnBnClickedLayerdelayUp)
	ON_BN_CLICKED(IDC_LAYERDELAY_DOWN, &CController4Dlg2::OnBnClickedLayerdelayDown)
	ON_EN_CHANGE(IDC_HEATER_TIMEOUT, &CController4Dlg2::OnEnChangeHeaterTimeout)
	ON_BN_CLICKED(IDC_HEATER_TIMEOUT_UP, &CController4Dlg2::OnBnClickedHeaterTimeoutUp)
	ON_BN_CLICKED(IDC_HEATER_TIMEOUT_DOWN, &CController4Dlg2::OnBnClickedHeaterTimeoutDown)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


// CController4Dlg2 message handlers

void CController4Dlg2::SizeDialog() {

	SetWindowPlacement(&theApp.MainWP);


}

void CController4Dlg2::ShowVersions() {
char tmp_buff[128];

	sprintf(tmp_buff, "%d", BUILD_NUMBER);
	m_ControllerVer.SetWindowText(tmp_buff);

	if(theApp.GVars->Versions.values_present) {
	
		sprintf(tmp_buff, "%d", theApp.GVars->Versions.Main);
		m_MainVer.SetWindowText(tmp_buff);

		sprintf(tmp_buff, "%d", theApp.GVars->Versions.Aux);
		m_AuxVer.SetWindowText(tmp_buff);

		sprintf(tmp_buff, "%d.%d.%X", theApp.GVars->Versions.PIC_Maj, theApp.GVars->Versions.PIC_Min, theApp.GVars->Versions.PIC_Rev);
		m_SlaveVer.SetWindowText(tmp_buff);

	}

	else {
	
		m_MainVer.SetWindowText("???");
		m_AuxVer.SetWindowText("???");
		m_SlaveVer.SetWindowText("???");
	
	}



}

void CController4Dlg2::SplitIPAddy(DWORD *dest, IP_ADDRESS_STRING *strng) {
char *ptr = strng->String, fields[4];
int i = 0;

	fields[i] = (char)atoi(ptr);
	for(i = 1; i < 4; i++) {
		while(*ptr != '.') ptr++;
		ptr++;
		fields[i] = atoi(ptr);
	}

	ptr = (char *)dest;

	for(i = 0; i < 4; i++) {
		ptr[i] = fields[i];
	}
}

void CController4Dlg2::SplitIPAddyChr(unsigned char *dest, IP_ADDRESS_STRING *strng) {
char *ptr = strng->String, fields[4];
int i = 0;

	dest[i] = (char)atoi(ptr);
	for(i = 1; i < 4; i++) {
		while(*ptr != '.') ptr++;
		ptr++;
		dest[i] = atoi(ptr);
	}


}

void CController4Dlg2::ShowIPStuff(IP_ADAPTER_INFO *pAdapt) {
int j;
PIP_ADDR_STRING pAddrStr;
unsigned char ip[4];


    printf("%s\n", pAdapt->Description);

	pAddrStr = &(pAdapt->IpAddressList);

	while(pAddrStr)  {

		SplitIPAddyChr((unsigned char *)&ip[0], &pAddrStr->IpAddress); 

		m_IPAddress.SetAddress(ip[0], ip[1], ip[2], ip[3]);

		printf("%-10.d%-10.d%-20.20s%s\n", pAdapt->Index, pAddrStr->Context, pAddrStr->IpAddress.String, pAddrStr->IpMask.String);
        pAddrStr = pAddrStr->Next;
    }


}

/*---------------------------------------------------------------------------------*/
/*-                                                                               -*/
/*---------------------------------------------------------------------------------*/
void CController4Dlg2::GetAndDisplayAdapters() {

	bHasAdapterInfo = FALSE;

	int AdapterBufferSize = theApp.NtwrkObj.GetAdaptersBufferSize(), index, j;

	PIP_ADAPTER_INFO pAdapt;
    PIP_ADDR_STRING pAddrStr;

	if(pAdapterInfo != NULL) free(pAdapterInfo);

	pAdapterInfo = (PIP_ADAPTER_INFO)calloc(1, AdapterBufferSize);

	if(!theApp.NtwrkObj.GetAdapterRecords(pAdapterInfo, (DWORD *)&AdapterBufferSize)) {

		theDlg->OKBox("Error getting", "IP Addresses", NULL);

		m_IPAddress.EnableWindow(FALSE);

		m_AdapterList.EnableWindow(FALSE);

		m_EditIP.EnableWindow(FALSE);

		
	}
	else {

		pAdapt = pAdapterInfo;

		while (pAdapt) {

			index = m_AdapterList.AddString(pAdapt->Description);
			m_AdapterList.SetItemData(index, (DWORD)pAdapt);


			for (j=0; j<pAdapt->AddressLength; j++) {
				if (j == (pAdapt->AddressLength - 1))
					printf("%.2X - ",(int)pAdapt->Address[j]);
				else
					printf("%.2X-",(int)pAdapt->Address[j]);

			}
			printf("%s\n", pAdapt->Description);

			pAddrStr = &(pAdapt->IpAddressList);

			while(pAddrStr)  {
				printf("%-10.d%-10.d%-20.20s%s\n", pAdapt->Index, pAddrStr->Context, pAddrStr->IpAddress.String, pAddrStr->IpMask.String);
				pAddrStr = pAddrStr->Next;
			}

			pAdapt = pAdapt->Next;

		}
	
		if((iAdapterCount = m_AdapterList.GetCount())) {

			bHasAdapterInfo = TRUE;

			if(theApp.GVars->DisplayedAdaptor > (iAdapterCount - 1))
				theApp.GVars->DisplayedAdaptor = 0;

			m_AdapterList.SetCurSel(theApp.GVars->DisplayedAdaptor);
			OnCbnSelchangeAdapters();
	
	
		}

	}

}



BOOL CController4Dlg2::OnInitDialog()
{
int result, i; 
	
	//m_BiDiCor_Val.SetBackgroundBitmap(&theApp.hSettingsBackground);

	theApp.currentWindow = SETTINGS_WINDOW;

	Initializing = TRUE;

	pAdapterInfo = NULL;

	result = m_Home.AutoLoad(IDC_HOME, this);

	m_BidiCorUP.AutoLoad(IDC_BIDI_COR_PLUS, this);
	m_BidiCorDN.AutoLoad(IDC_BIDICOR_MINUS, this);
	m_HeadSetup.AutoLoad(IDC_HEADSETUP,this);
	m_KeepJobsDN.AutoLoad(IDC_BIDI_COR_PLUS2,this);
	m_KeepJobsUP.AutoLoad(IDC_BIDICOR_MINUS2,this);
	m_PlatenGapUp.AutoLoad(IDC_PLATEN_GAP_PLUS, this);
	m_PlatenGapDown.AutoLoad(IDC_PLATEN_GAP_MINUS2, this);

	m_LayerDelayUp.AutoLoad(IDC_LAYERDELAY_UP, this);
	m_LayerDelayDown.AutoLoad(IDC_LAYERDELAY_DOWN, this);

	m_PlatenSizeUp.AutoLoad(IDC_PLATEN_SIZE_UP, this);
	m_PlatenSizeDown.AutoLoad(IDC_PLATEN_SIZE_DOWN, this);

	m_HeaterTimeoutUP.AutoLoad(IDC_HEATER_TIMEOUT_UP, this);
	m_HeaterTimeoutDown.AutoLoad(IDC_HEATER_TIMEOUT_DOWN, this);

	m_HDOnOff.LoadBitmaps(IDB_ONBUTTON1, IDB_ONBUTTON1, IDB_ONBUTTON1, IDB_ONBUTTON1);
	m_RTOnOff.LoadBitmaps(IDB_ONBUTTON2, IDB_ONBUTTON2, IDB_ONBUTTON2, IDB_ONBUTTON2);

	m_ColorJobsCleanUp.AutoLoad(IDC_COLOR_CLEAN_JOBS_PLUS, this);
	m_ColorJobsCleanDown.AutoLoad(IDC_COLOR_CLEAN_JOBS_MINUS, this);
	m_WhiteJobsCleanUp.AutoLoad(IDC_WHITE_CLEAN_JOBS_PLUS, this);
	m_WhiteJobsCleanDown.AutoLoad(IDC_WHITE_CLEAN_JOBS_MINUS, this);

	CDialogEx::OnInitDialog();


	///////////////////////////////////////
	//////////////////////////////////////
	// 

	m_PlatenGap.AddString("Normal");
	m_PlatenGap.AddString("Narrow");
	m_PlatenGap.AddString("Wide");

	m_LayerDelaySec.AddString("Manual");
	m_LayerDelaySec.AddString("No Delay");
	m_LayerDelaySec.AddString("15 Seconds");
	m_LayerDelaySec.AddString("30 Seconds");
	m_LayerDelaySec.AddString("45 Seconds");


	m_BiDiCor_Val.SetFont(&theDlg->pMediumFont);
	m_HeadConfig.SetFont(&theDlg->pMediumFont);
	m_KeepJobs.SetFont(&theDlg->pMediumFont);
	m_PrinterName.SetFont(&theDlg->pMediumFont);
	m_Server.SetFont(&theDlg->pMediumFont);


	m_IP1.SetFont(&theDlg->pMediumFont);
	m_IP2.SetFont(&theDlg->pMediumFont);
	m_IP3.SetFont(&theDlg->pMediumFont);
	m_IP4.SetFont(&theDlg->pMediumFont);


	/*
	m_HeadConfig.SetBkColor(RGB(240,240,240));
	m_KeepJobs.SetBkColor(RGB(36,93,151));
	m_PrinterName.SetBkColor(RGB(36,93,151));
	m_Server.SetBkColor(RGB(36,93,151));
	m_IP1.SetBkColor(RGB(36,93,151));
	m_IP2.SetBkColor(RGB(36,93,151));
	m_IP3.SetBkColor(RGB(36,93,151));
	m_IP4.SetBkColor(RGB(36,93,151));
	*/

	m_ColorJobsClean.SetFont(&theDlg->pMediumFont);
	m_WhiteJobsClean.SetFont(&theDlg->pMediumFont);


	//m_BiDiCor_Val.SetBkColor(RGB(240,240,240));
	//m_KeepJobs.SetBkColor(RGB(240,240,240));


	m_ColorJobsClean.SetBkColor(RGB(240,240,240));
	m_WhiteJobsClean.SetBkColor(RGB(240,240,240));
	m_KeepJobs.SetBkColor(RGB(240,240,240));
	m_BiDiCor_Val.SetBkColor(RGB(240,240,240));


	SizeDialog();

	ShowWindow(SW_SHOW);

	char tmp_buff[100];

	sprintf(tmp_buff, "%d", theApp.GVars->CustBiDiCor);

	m_BiDiCor_Val.SetWindowText(tmp_buff);

	m_HeadConfig.SetWindowTextA("CMYK+W");


	sprintf(tmp_buff, "%d", theApp.GVars->MaxJobs);

	m_KeepJobs.SetWindowText(tmp_buff);


	sprintf(tmp_buff, "%d", theApp.GVars->iWhiteJobsBeforeClean);
	m_WhiteJobsClean.SetWindowText(tmp_buff);

	sprintf(tmp_buff, "%d", theApp.GVars->iColorJobsBeforeClean);
	m_ColorJobsClean.SetWindowText(tmp_buff);


	if(theApp.GVars->bRapidRT) {
		m_RTOnOff.LoadBitmaps(IDB_ONBUTTON2, IDB_ONBUTTON2, IDB_ONBUTTON2, IDB_ONBUTTON2);
	}
	else {
		m_RTOnOff.LoadBitmaps(IDB_OFFBUTTON2, IDB_OFFBUTTON2, IDB_OFFBUTTON2, IDB_OFFBUTTON2);\
	}
	m_RTOnOff.Invalidate();
	m_RTOnOff.UpdateWindow();

	if(theApp.GVars->bAutoHD) {
		m_HDOnOff.LoadBitmaps(IDB_ONBUTTON1, IDB_ONBUTTON1, IDB_ONBUTTON1, IDB_ONBUTTON1);
	}
	else {
		m_HDOnOff.LoadBitmaps(IDB_OFFBUTTON1, IDB_OFFBUTTON1, IDB_OFFBUTTON1, IDB_OFFBUTTON1);
	}


	theApp.NtwrkObj.GetMainParams(theApp.GVars->ServerName); /// MOE - retrieve network parameters for the local computer

	GetAndDisplayAdapters();

	m_HDOnOff.Invalidate();
	m_HDOnOff.UpdateWindow();

	if(!strlen(theApp.GVars->ServerName)) {
		m_ServerName.SetWindowText("GENESIS_PRINTER");
	}
	else {
		m_ServerName.SetWindowText(theApp.GVars->ServerName);
	}
	
	m_LayerDelaySec.SetCurSel(theApp.GVars->LayerDelaySec);

	m_OneButtonOperation.SetCheck(theApp.GVars->bOneButtonOperation);

	m_IgnoreLasers.SetCheck(theApp.GVars->bIgnoreObstruction);

	SetObstructionLaserButtonText();

	m_PlatenGap.SetCurSel(theApp.GVars->CurrentHeightOffset);

	ShowVersions();

	EnableDJAP();

	for(i = 0; i < 4; i++) {
		if(strlen(theApp.GVars->szPlatenName[i]) > 3) {
			m_PlatenSize.AddString(theApp.GVars->szPlatenName[i]);
		}
	}

	m_PlatenSize.SetCurSel(theApp.GVars->iSelectedPlaten);

	char tmp[16];
	sprintf(tmp, "%d", theApp.GVars->TotalJobs);
	m_SerialNumber.SetWindowText(theApp.GVars->szSN);
	m_TotalPrints.SetWindowText(tmp);

	OldHeaterTimeout = theApp.GVars->iHeaterTimeout;
	DisplayHeaterTimeout();



	//m_ControllerVer.SetFocus();
	//SetDialogFocus(this, m_ControllerVer.m_hWnd)
	HWND hWnd;
	GetDlgItem(IDC_CONROLLER_VER, &hWnd);
	::PostMessage(hWnd, WM_SETFOCUS, 0, 0);

	Initializing = FALSE;


	//return TRUE;
	return FALSE;

}

void CController4Dlg2::EnableDJAP() {

	if(theApp.GVars->MaxJobs == 1) {
		//m_DeleteJobAfterPrint.ShowWindow(SW_SHOW);
		m_DeleteJobAfterPrint.EnableWindow(TRUE);
		m_DeleteJobAfterPrint.SetCheck(theApp.GVars->bDeleteAfterPrinting);
	}

	else {
		m_DeleteJobAfterPrint.EnableWindow(FALSE);
		m_DeleteJobAfterPrint.SetCheck(FALSE);
		//m_DeleteJobAfterPrint.ShowWindow(SW_HIDE);
	}


}


void CController4Dlg2::OnBnClickedHome() {
	CDialogEx::OnCancel();
}


void CController4Dlg2::OnBnClickedCancel() {
	CDialogEx::OnCancel();
}


void CController4Dlg2::OnBnClickedRight() {
CController4Dlg3 dlg;

	dlg.DoModal();
}


HRESULT CController4Dlg2::get_accFocus(VARIANT *pvarChild) {
	


	return CDialogEx::get_accFocus(pvarChild);
}


void CController4Dlg2::OnSetFocus(CWnd* pOldWnd) {

	if(theApp.bHomePressed) {

		CDialogEx::OnCancel();

	}
	else {
	}


	CDialogEx::OnSetFocus(pOldWnd);

}


void CController4Dlg2::OnBnClickedBidiCorPlus() {
char tmp_buff[100];

	if(!Initializing && !CheckAdvanced())
		return;


	theApp.GVars->CustBiDiCor++;

	sprintf(tmp_buff, "%d", theApp.GVars->CustBiDiCor);

	m_BiDiCor_Val.SetWindowText(tmp_buff);

	//theApp.WriteINI();
}


void CController4Dlg2::OnBnClickedBidicorMinus() {
char tmp_buff[100];

	if(!Initializing && !CheckAdvanced())
		return;
		
	theApp.GVars->CustBiDiCor--;

	sprintf(tmp_buff, "%d", theApp.GVars->CustBiDiCor);

	m_BiDiCor_Val.SetWindowText(tmp_buff);
	//theApp.WriteINI();

}


void CController4Dlg2::OnDestroy()
{
	
	USBPrinterCommand(USB_CMD_SET_BUTTONMODE, theApp.GVars->bOneButtonOperation);

	USBPrinterCommand(USB_CMD_SET_CAPDRY_JOBS, theApp.GVars->JobsBeforeDry);

	if(OldHeaterTimeout != theApp.GVars->iHeaterTimeout)
		SendHeaterTimeout();

	if(pAdapterInfo != NULL) free(pAdapterInfo);

	theApp.WriteINI();

	CDialogEx::OnDestroy();


}


void CController4Dlg2::OnPaint()
{
		CPaintDC dc(this); // device context for painting
	
		CRect rect;
		GetClientRect(&rect);

		// FILL THE BACKGROUND
		//dc.FillRect(&rect, &theApp.hBGbrush);

		CDialog::OnPaint();
		theApp.DisplayBitmap(&dc, &theApp.hSettingsBackground, 0, 0, DB_SHOWUPPERLEFT);
}



void CController4Dlg2::OnBnClickedHeadsetup() {
char tmp_buff[128];

	sprintf(tmp_buff, "CMYK+W");
	m_HeadConfig.SetWindowTextA(tmp_buff);

	//theApp.WriteINI();

}


void CController4Dlg2::OnBnClickedBidiCorPlus2() {
char tmp_buff[128];

	if(theApp.GVars->MaxJobs >= 16) return;

	theApp.GVars->MaxJobs++;
	sprintf(tmp_buff, "%d", theApp.GVars->MaxJobs);

	m_KeepJobs.SetWindowText(tmp_buff);

	EnableDJAP();

	// theApp.WriteINI();

}


void CController4Dlg2::OnBnClickedBidicorMinus2() {
char tmp_buff[128];


	if(theApp.GVars->MaxJobs < 2) return;

	if(theApp.GVars->MaxJobs == 0) return;

	if(theApp.GVars->MaxJobs < 4 && !theApp.GVars->bDualCMYK) {
		return;
	}

	if(theApp.GVars->MaxJobs < 0) {
		theApp.GVars->MaxJobs = 0;
		return;
	}


	theApp.GVars->MaxJobs--;

	sprintf(tmp_buff, "%d", theApp.GVars->MaxJobs);
	m_KeepJobs.SetWindowText(tmp_buff);

	EnableDJAP();

	// theApp.WriteINI();

}


void CController4Dlg2::OnBnClickedAutohd() {

	/**/ TRACE("AUTOHD\n");


	if(theApp.GVars->bAutoHD) {
		theApp.GVars->bAutoHD = FALSE;
		m_HDOnOff.LoadBitmaps(IDB_OFFBUTTON1, IDB_OFFBUTTON1, IDB_OFFBUTTON1, IDB_OFFBUTTON1);
	}
	else {
		theApp.GVars->bAutoHD = TRUE;
		m_HDOnOff.LoadBitmaps(IDB_ONBUTTON1, IDB_ONBUTTON1, IDB_ONBUTTON1, IDB_ONBUTTON1);
	}
	m_HDOnOff.Invalidate();
	theApp.WriteINI();
}


void CController4Dlg2::OnBnClickedRapidrt() {

	/**/ TRACE("RAPIDRT\n");

	if(theApp.GVars->bRapidRT) {
		theApp.GVars->bRapidRT = FALSE;
		m_RTOnOff.LoadBitmaps(IDB_OFFBUTTON2, IDB_OFFBUTTON2, IDB_OFFBUTTON2, IDB_OFFBUTTON2);\
	}
	else {
		theApp.GVars->bRapidRT = TRUE;
		m_RTOnOff.LoadBitmaps(IDB_ONBUTTON2, IDB_ONBUTTON2, IDB_ONBUTTON2, IDB_ONBUTTON2);
	}
	m_RTOnOff.Invalidate();
	//theApp.WriteINI();

}


void CController4Dlg2::OnBnClickedColorCleanJobsPlus()
{
char tmp_buff[16];

	theApp.GVars->iColorJobsBeforeClean++;

	sprintf(tmp_buff, "%d", theApp.GVars->iColorJobsBeforeClean);

	m_ColorJobsClean.SetWindowText(tmp_buff);

}


void CController4Dlg2::OnBnClickedColorCleanJobsMinus()
{
char tmp_buff[16];

	if(theApp.GVars->iColorJobsBeforeClean < 1) return;

	theApp.GVars->iColorJobsBeforeClean--;

	sprintf(tmp_buff, "%d", theApp.GVars->iColorJobsBeforeClean);

	m_ColorJobsClean.SetWindowText(tmp_buff);


}


void CController4Dlg2::OnBnClickedWhiteCleanJobsPlus()
{
char tmp_buff[16];

	theApp.GVars->iWhiteJobsBeforeClean++;

	sprintf(tmp_buff, "%d", theApp.GVars->iWhiteJobsBeforeClean);

	m_WhiteJobsClean.SetWindowText(tmp_buff);

	//theApp.WriteINI();

}


void CController4Dlg2::OnBnClickedWhiteCleanJobsMinus()
{
char tmp_buff[16];

	if(theApp.GVars->iWhiteJobsBeforeClean < 1) return;

	theApp.GVars->iWhiteJobsBeforeClean--;

	sprintf(tmp_buff, "%d", theApp.GVars->iWhiteJobsBeforeClean);

	m_WhiteJobsClean.SetWindowText(tmp_buff);

	//theApp.WriteINI();

}


void CController4Dlg2::OnBnClickedNettest() {

	theApp.NtwrkObj.AdvancedSetup();   /// MOE - TODO

}


void CController4Dlg2::OnBnClickedGetVersions() {
	ShowVersions();
	USBPrinterCommand(USB_CMD_REPORT_VERSIONS, 0);

}


void CController4Dlg2::OnCbnSelchangeLayerdelaySec() {

	theApp.GVars->LayerDelaySec = m_LayerDelaySec.GetCurSel();

	if(theApp.GVars->LayerDelaySec < 0) theApp.GVars->LayerDelaySec = 0;

	//theApp.WriteINI();

}


void CController4Dlg2::SimulateKey(char k) {

	INPUT keyInput;
	keyInput.type = INPUT_KEYBOARD;
	
	KEYBDINPUT key;
	key.wVk = k;
	key.wScan = 0;//::VkKeyScan(49);
		
	key.dwFlags = 0;
	keyInput.ki = key;

	m_IPAddress.SetFocus();

	::SendInput(1,&keyInput,sizeof(INPUT));
	
	key.dwFlags = KEYEVENTF_KEYUP;
	keyInput.ki = key;

	::SendInput(1,&keyInput,sizeof(INPUT));
}



void CController4Dlg2::OnIpnFieldchangedIpaddress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CController4Dlg2::OnCbnSelchangeAdapters() {
IP_ADAPTER_INFO *ad;
int index;
char szString[128]; 

	if(!Initializing && !CheckAdvanced()) {

		m_AdapterList.SetCurSel(theApp.GVars->DisplayedAdaptor);
		return;

	}

	index = m_AdapterList.GetCurSel();
	if(index < 0) return;

	ad = (IP_ADAPTER_INFO *)m_AdapterList.GetItemData(index);

	if(ad != NULL) {

		switch(ad->Type) {
		
			case MIB_IF_TYPE_OTHER:
				strcpy(szString, "Unknown type");
				break;

 
			case MIB_IF_TYPE_ETHERNET:
				strcpy(szString, "Ethernet");
				break;

 
			case IF_TYPE_ISO88025_TOKENRING:
				strcpy(szString, "Token Ring");
				break;
 
			case MIB_IF_TYPE_PPP: 
				strcpy(szString, "PPP Connection");
				break;
 
			case MIB_IF_TYPE_LOOPBACK:
				strcpy(szString, "Software Loopback");
				break;

 
			case MIB_IF_TYPE_SLIP:
				strcpy(szString, "ATM Network");
				break;
 
			case IF_TYPE_IEEE80211:
				strcpy(szString, "IEEE 802.11");
				break;
		
		}

		if(strstr(ad->Description, "Wi-Fi") != NULL || strstr(ad->Description, "802") != NULL || ad->Type == IF_TYPE_IEEE80211) {
			strcat(szString," [WiFI]");
		}

		m_ConnectionType.SetWindowText(szString);

		ShowIPStuff(ad);

		theApp.GVars->DisplayedAdaptor = m_AdapterList.GetCurSel();

		//theApp.WriteINI();

	}

}


void CController4Dlg2::OnBnClickedChange() 
{
	unsigned long ip;

	if(!Initializing && !CheckAdvanced())
		return;


	char *tptr = (char *)&ip, t[4], m[32];

	m_IPAddress.GetAddress(ip);

	unsigned long origip = ip;

	t[0] = tptr[3];
	t[1] = tptr[2];
	t[2] = tptr[1];
	t[3] = tptr[0];

	memcpy(tptr, t, 4);

	BOOL bUseDHCP = FALSE;

	if(theApp.NtwrkObj.GetIPAddress(&ip, "Enter IP Address or click\n\"Get from DHCP Server\" button", &bUseDHCP)) {
		
		if(origip != ip) {

			// first delete the current addy

			IP_ADAPTER_INFO *pAdapt = (PIP_ADAPTER_INFO)m_AdapterList.GetItemData(m_AdapterList.GetCurSel());

			if(pAdapt != NULL) {

				PIP_ADDR_STRING pAddrStr = &(pAdapt->IpAddressList);

				if(theApp.NtwrkObj.DeleteIP(pAddrStr->Context)) {

					if(bUseDHCP) {
						BOOL *finishWait = theApp.NtwrkObj.ShowWaitWindow("Please Wait");
						if(theApp.NtwrkObj.GetDHCPIPAddress(pAdapt->Index)) {
							GetAndDisplayAdapters();
						}
						else {
							printf("Failed to get IP from DHCP\n");
							theDlg->OKBox("Could Not Change", "IP Address (dh)", NULL);
						}
						*finishWait = TRUE;
					}
					else {

						ULONG context, instance;

						t[0] = tptr[0];
						t[1] = tptr[1];
						t[2] = tptr[2];
						t[3] = tptr[3];

						char addychr[32];

						sprintf(addychr, "%d.%d.%d.%d", (unsigned char)t[3], (unsigned char)t[2], (unsigned char)t[1], (unsigned char)t[0]);
	
						strcpy(m, pAddrStr->IpMask.String);

						if(theApp.NtwrkObj.AddIP(pAdapt->Index, addychr, m, &context, &instance)) {
							GetAndDisplayAdapters();
						}
						else {
							printf("Failed to add IP\n");
							theDlg->OKBox("Could Not Change", "IP Address (ad)", NULL);
						}

					}
				}
				else {
					theDlg->OKBox("Could Not Change", "IP Address (dl)", "Insufficient Rights");
					printf("Failed to delete IP\n");
				}

			}
		}
	}
}



void CController4Dlg2::OnBnClickedDbg()
{
	BOOL *finishWait = theApp.NtwrkObj.ShowWaitWindow("Please Wait");
	Sleep(5000);
	*finishWait = TRUE;

}


void CController4Dlg2::OnBnClickedOnebuttonOperation()
{
	theApp.GVars->bOneButtonOperation = m_OneButtonOperation.GetCheck();

}


void CController4Dlg2::OnBnClickedDeleteAfterPrint() {
	theApp.GVars->bDeleteAfterPrinting = m_DeleteJobAfterPrint.GetCheck();
}

void CController4Dlg2::SetObstructionLaserButtonText() {


	if(!theApp.GVars->bIgnoreObstruction) {
		m_IgnoreLasers.SetWindowText("Using\nObstruction\nLasers");
	}
	else {
		m_IgnoreLasers.SetWindowText("Ignoring\nObstruction\nLasers");
	}



}

void CController4Dlg2::OnBnClickedIgnoreObstruction2()
{

	if(!Initializing && !CheckAdvanced()) {
		m_IgnoreLasers.SetCheck(theApp.GVars->bIgnoreObstruction);
		return;
	}

	theApp.GVars->bIgnoreObstruction = m_IgnoreLasers.GetCheck();
	SetObstructionLaserButtonText();
	//theApp.WriteINI();
	USBPrinterCommand(USB_CMD_IGNORE_OBSTRUCTION, theApp.GVars->bIgnoreObstruction);

}


void CController4Dlg2::OnCbnSelchangePtOffset()
{
	theApp.GVars->CurrentHeightOffset = m_PlatenGap.GetCurSel();
	//theApp.WriteINI();
}


void CController4Dlg2::OnCbnSelchangePlatenSize() {

	theApp.GVars->iSelectedPlaten = m_PlatenSize.GetCurSel();

	theApp.SendPlatenSelection();

	//theApp.WriteINI();

}


void CController4Dlg2::OnBnClickedPlatenGapPlus() {
int cursel = m_PlatenGap.GetCurSel();

	cursel--;

	if(cursel < 0)
		cursel = 2;

	m_PlatenGap.SetCurSel(cursel);

	OnCbnSelchangePtOffset();

}


void CController4Dlg2::OnBnClickedPlatenGapMinus2()
{
int cursel = m_PlatenGap.GetCurSel();

	cursel++;

	if(cursel > 2)
		cursel = 0;

	m_PlatenGap.SetCurSel(cursel);

	OnCbnSelchangePtOffset();

}


void CController4Dlg2::OnLbnSelchangePltOffset()
{
	// TODO: Add your control notification handler code here
}


void CController4Dlg2::OnBnClickedPlatenSizeUp()
{
int cursel = m_PlatenSize.GetCurSel();

	cursel--;

	if(cursel < 0)
		cursel = 2;

	m_PlatenSize.SetCurSel(cursel);
	OnCbnSelchangePlatenSize();

}


void CController4Dlg2::OnBnClickedPlatenSizeDown()
{
int cursel = m_PlatenSize.GetCurSel();

	cursel++;

	if(cursel > 3)
		cursel = 0;

	m_PlatenSize.SetCurSel(cursel);
	OnCbnSelchangePlatenSize();
}


void CController4Dlg2::OnLbnSelchangePltnSize()
{
	// TODO: Add your control notification handler code here
}


void CController4Dlg2::OnBnClickedLayerdelayUp()
{
int cursel = m_LayerDelaySec.GetCurSel();

	cursel--;

	if(cursel < 0)
		cursel = 4;

	m_LayerDelaySec.SetCurSel(cursel);

	OnCbnSelchangeLayerdelaySec();
}


void CController4Dlg2::OnBnClickedLayerdelayDown()
{
int cursel = m_LayerDelaySec.GetCurSel();

	cursel++;

	if(cursel > 4)
		cursel = 0;

	m_LayerDelaySec.SetCurSel(cursel);

	OnCbnSelchangeLayerdelaySec();
}


void CController4Dlg2::OnEnChangeHeaterTimeout()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CController4Dlg2::DisplayHeaterTimeout() {
char tmp_buff[16];

	sprintf(tmp_buff, "%d", theApp.GVars->iHeaterTimeout);
	m_HeaterTimeout.SetWindowText(tmp_buff);

}

void CController4Dlg2::OnBnClickedHeaterTimeoutUp() {

	if(!theApp.GVars->bAdvancedFeaturesUnlocked) {
		theApp.GVars->bAdvancedFeaturesUnlocked = UnlockAdvanced();
	}
	
	if(theApp.GVars->bAdvancedFeaturesUnlocked) {
		theApp.GVars->iHeaterTimeout++;

		DisplayHeaterTimeout();
	}

}


void CController4Dlg2::OnBnClickedHeaterTimeoutDown() {

	if(!theApp.GVars->bAdvancedFeaturesUnlocked) {
		theApp.GVars->bAdvancedFeaturesUnlocked = UnlockAdvanced();
	}
	
	if(theApp.GVars->bAdvancedFeaturesUnlocked) {

		if(theApp.GVars->iHeaterTimeout > 0)
			theApp.GVars->iHeaterTimeout--;

		DisplayHeaterTimeout();
	}

}

// PlatenLiftCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "PlatenLiftCtrl.h"
#include "afxdialogex.h"
#include "parser.h"
#define STEPS_DEFINED
#include "HEAD_structs.h"


///////////// EXTERNAL PLATEN CONTROL COMMANDS
#define PLP_EXTCMD_INIT                 0x00000001
#define PLP_EXTCMD_ENABLE               0x00000002
#define PLP_EXTCMD_MOVE                 0x00000003
#define PLP_EXTCMD_FINDLASER            0x00000004
#define PLP_EXTCMD_QUERYLASER           0x00000005



extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;


// CPlatenLiftCtrl dialog

IMPLEMENT_DYNAMIC(CPlatenLiftCtrl, CDialogEx)

CPlatenLiftCtrl::CPlatenLiftCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlatenLiftCtrl::IDD, pParent)
{

}

CPlatenLiftCtrl::~CPlatenLiftCtrl()
{
}

void CPlatenLiftCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STEPPER_1, m_Stepper1);
	DDX_Control(pDX, IDC_STEPPER_2, m_Stepper2);
	DDX_Control(pDX, IDC_STEPPER_3, m_Stepper3);
	DDX_Control(pDX, IDC_STEPPER_4, m_Stepper4);
	DDX_Control(pDX, IDC_TICKS, m_Ticks);
	DDX_Control(pDX, IDC_DIRECTION, m_Direction);
	DDX_Control(pDX, IDC_STEPPING, m_Stepping);
	DDX_Control(pDX, IDC_CURRENT, m_Current);
	DDX_Control(pDX, IDC_MOVEMODE, m_MoveMode);
}


BEGIN_MESSAGE_MAP(CPlatenLiftCtrl, CDialogEx)
	ON_BN_CLICKED(IDC_INITSTEPPERS, &CPlatenLiftCtrl::OnBnClickedInitsteppers)
	ON_BN_CLICKED(IDC_ENABLEALL, &CPlatenLiftCtrl::OnBnClickedEnableall)
	ON_BN_CLICKED(IDC_DISABLE_ALL, &CPlatenLiftCtrl::OnBnClickedDisableAll)
	ON_BN_CLICKED(IDC_ENABLE_SEL, &CPlatenLiftCtrl::OnBnClickedEnableSel)
	ON_BN_CLICKED(IDC_MOVETICKS, &CPlatenLiftCtrl::OnBnClickedMoveticks)
	ON_BN_CLICKED(IDC_TOBOTTOM, &CPlatenLiftCtrl::OnBnClickedTobottom)
	ON_BN_CLICKED(IDC_TOTOP, &CPlatenLiftCtrl::OnBnClickedTotop)
	ON_BN_CLICKED(IDC_SENDSETTINGS, &CPlatenLiftCtrl::OnBnClickedSendsettings)
	ON_BN_CLICKED(IDC_BUTTON1, &CPlatenLiftCtrl::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPlatenLiftCtrl message handlers


void CPlatenLiftCtrl::OnBnClickedInitsteppers()
{

	USBPrinterCommandEx(USB_CMD_PLATEN_COMMAND, PLP_EXTCMD_INIT, 0, 0, 0, 0, 0, 0);

	SaveLastSettings();
}


void CPlatenLiftCtrl::OnBnClickedEnableall()
{

	m_Stepper1.SetCheck(TRUE);
	m_Stepper2.SetCheck(TRUE);
	m_Stepper3.SetCheck(TRUE);
	m_Stepper4.SetCheck(TRUE);

	USBPrinterCommandEx(USB_CMD_PLATEN_COMMAND, PLP_EXTCMD_ENABLE, 0x01|0x02|0x04|0x08, 0, 0, 0, 0, 0);


	SaveLastSettings();
}


void CPlatenLiftCtrl::OnBnClickedDisableAll()
{

	m_Stepper1.SetCheck(FALSE);
	m_Stepper2.SetCheck(FALSE);
	m_Stepper3.SetCheck(FALSE);
	m_Stepper4.SetCheck(FALSE);

	USBPrinterCommandEx(USB_CMD_PLATEN_COMMAND, PLP_EXTCMD_ENABLE, 0, 0, 0, 0, 0, 0);

	SaveLastSettings();
}


void CPlatenLiftCtrl::OnBnClickedEnableSel()
{
	unsigned char enable_bits[4] = {0x01, 0x02, 0x04, 0x08}, bits = 0;

	if(m_Stepper1.GetCheck()) bits |= enable_bits[0];
	if(m_Stepper2.GetCheck()) bits |= enable_bits[1];
	if(m_Stepper3.GetCheck()) bits |= enable_bits[2];
	if(m_Stepper4.GetCheck()) bits |= enable_bits[3];

	USBPrinterCommandEx(USB_CMD_PLATEN_COMMAND, PLP_EXTCMD_ENABLE, bits, 0, 0, 0, 0, 0);


	SaveLastSettings();
}


void CPlatenLiftCtrl::OnBnClickedMoveticks()
{
	unsigned long p2, p3, p4, ticks = 0;
	char tmp_buff[32];

	unsigned char tH, tL;

	m_Ticks.GetWindowText(tmp_buff, 32);

	ticks = atol(tmp_buff);

	tL = atoi(tmp_buff) & 0x00FF;
	tH = (atoi(tmp_buff) & 0xFF00) >> 8;

	p2 = tL;
	p3 = tH;

	p4 = m_Direction.GetCurSel();

	p4 |= m_MoveMode.GetCurSel() << 1;

	USBPrinterCommandEx(USB_CMD_PLATEN_COMMAND, PLP_EXTCMD_MOVE, p2, p3, p4, 0, 0, 0);


	SaveLastSettings();
}


void CPlatenLiftCtrl::OnBnClickedTobottom()
{
	// TODO: Add your control notification handler code here
}


void CPlatenLiftCtrl::OnBnClickedTotop()
{



	SaveLastSettings();
}


void CPlatenLiftCtrl::OnBnClickedSendsettings()
{
	unsigned char enable_bits[4] = {0x01, 0x02, 0x04, 0x08}, bits = 0;

	if(m_Stepper1.GetCheck()) bits |= enable_bits[0];
	if(m_Stepper2.GetCheck()) bits |= enable_bits[1];
	if(m_Stepper3.GetCheck()) bits |= enable_bits[2];
	if(m_Stepper4.GetCheck()) bits |= enable_bits[3];

	unsigned long p3 = 0, p4 = 0;

	bits |= m_Stepping.GetCurSel() << 4;

	char tmp_buff[32];

	m_Current.GetWindowText(tmp_buff, 32);

	p3 = atoi(tmp_buff) & 0x00FF;
	p4 = (atoi(tmp_buff) & 0xFF00) >> 8;

	USBPrinterCommandEx(USB_CMD_PLATEN_COMMAND, PLP_EXTCMD_PARAMS, bits, p3, p4, 0, 0, 0);

	SaveLastSettings();


}


BOOL CPlatenLiftCtrl::OnInitDialog() {

	CDialogEx::OnInitDialog();

	m_Stepper1.SetCheck(TRUE);
	m_Stepper2.SetCheck(TRUE);
	m_Stepper3.SetCheck(TRUE);
	m_Stepper4.SetCheck(TRUE);
	m_Ticks.SetWindowText("1000");
	m_Direction.SetCurSel(0);
	m_Stepping.SetCurSel(3);
	m_Current.SetWindowText("400");
	m_MoveMode.SetCurSel(0);

	GetLastSettings();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPlatenLiftCtrl::GetLastSettings() {
FILE *f;
char linebuff[256];

	f = fopen("lastplatctrl.dat", "r");

	if(f != NULL) {

		if(fgets(linebuff, 256, f) != NULL) {
			m_Stepper1.SetCheck(atoi(linebuff));
		}
		if(fgets(linebuff, 256, f) != NULL) {
			m_Stepper2.SetCheck(atoi(linebuff));
		}
		if(fgets(linebuff, 256, f) != NULL) {
			m_Stepper3.SetCheck(atoi(linebuff));
		}
		if(fgets(linebuff, 256, f) != NULL) {
			m_Stepper4.SetCheck(atoi(linebuff));
		}

		if(fgets(linebuff, 256, f) != NULL) {

			while(linebuff[strlen(linebuff) - 1] == 0x0D || linebuff[strlen(linebuff) - 1] == 0x0A)
				linebuff[strlen(linebuff) - 1] = 0;

			m_Ticks.SetWindowText(linebuff);
		}

		if(fgets(linebuff, 256, f) != NULL) {
			m_Direction.SetCurSel(atoi(linebuff));
		}

		if(fgets(linebuff, 256, f) != NULL) {
			m_Stepping.SetCurSel(atoi(linebuff));
		}

		if(fgets(linebuff, 256, f) != NULL) {

			while(linebuff[strlen(linebuff) - 1] == 0x0D || linebuff[strlen(linebuff) - 1] == 0x0A)
				linebuff[strlen(linebuff) - 1] = 0;

			m_Current.SetWindowText(linebuff);
		}

		if(fgets(linebuff, 256, f) != NULL) {
			m_MoveMode.SetCurSel(atoi(linebuff));
		}


		fclose(f);

	}


}

void CPlatenLiftCtrl::SaveLastSettings() {
FILE *f;
char linebuff[256];


	DeleteFile("lastplatctrl.dat");

	f = fopen("lastplatctrl.dat", "w");

	if(f != NULL) {

		fprintf(f, "%d\n", m_Stepper1.GetCheck());
		fprintf(f, "%d\n", m_Stepper2.GetCheck());
		fprintf(f, "%d\n", m_Stepper3.GetCheck());
		fprintf(f, "%d\n", m_Stepper4.GetCheck());

		m_Ticks.GetWindowText(linebuff, 256);
		fprintf(f, "%s\n", linebuff);

		fprintf(f, "%d\n", m_Direction.GetCurSel());

		fprintf(f, "%d\n", m_Stepping.GetCurSel());

		m_Current.GetWindowText(linebuff, 256);
		fprintf(f, "%s\n", linebuff);

		fprintf(f, "%d\n", m_MoveMode.GetCurSel());

		fclose(f);
	}
}

void CPlatenLiftCtrl::OnBnClickedButton1()
{

	USBPrinterCommandEx(USB_CMD_PLATEN_COMMAND, PLP_EXTCMD_GET_STATE, 0, 0, 0, 0, 0, 0);

}

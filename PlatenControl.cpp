// PlatenControl.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "PlatenControl.h"
#include "afxdialogex.h"
#include "parser.h"
#define STEPS_DEFINED
#include "HEAD_structs.h"


extern void USBPrinterCommand(unsigned long , unsigned long);
extern void USBPrinterCommandEx(unsigned long , unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long ,
												unsigned long) ;


// CPlatenControl dialog

IMPLEMENT_DYNAMIC(CPlatenControl, CDialogEx)

CPlatenControl::CPlatenControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlatenControl::IDD, pParent)
{

}

CPlatenControl::~CPlatenControl()
{
}

void CPlatenControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RL_STEPPER, m_RearL);
	DDX_Control(pDX, IDC_RR_STEPPER, m_RearR);
	DDX_Control(pDX, IDC_FL_STEPPER, m_FrontL);
	DDX_Control(pDX, IDC_FR_STEPPER, m_FrontR);
	DDX_Control(pDX, IDC_REPS2, m_Steps);
	DDX_Control(pDX, IDC_UD_1000, m_Spin1000);
	DDX_Control(pDX, IDC_UD_100, m_Spin100);
	DDX_Control(pDX, IDC_UD_10, m_Spin10);
	DDX_Control(pDX, IDC_UD_1, m_Spin1);
	DDX_Control(pDX, IDC_DIR_UP, m_Up);
	DDX_Control(pDX, IDC_DIR_DOWN, m_Down);
}


BEGIN_MESSAGE_MAP(CPlatenControl, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_1000, &CPlatenControl::OnDeltaposUd1000)
	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_100, &CPlatenControl::OnDeltaposUd100)
	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_10, &CPlatenControl::OnDeltaposUd10)
	ON_NOTIFY(UDN_DELTAPOS, IDC_UD_1, &CPlatenControl::OnDeltaposUd1)
	ON_BN_CLICKED(IDC_MOVE, &CPlatenControl::OnBnClickedMove)
	ON_BN_CLICKED(IDC_DIR_UP, &CPlatenControl::OnBnClickedDirUp)
	ON_BN_CLICKED(IDC_DIR_DOWN, &CPlatenControl::OnBnClickedDirDown)
	ON_BN_CLICKED(IDC_CLEAR, &CPlatenControl::OnBnClickedClear)
	ON_BN_CLICKED(IDC_TOTOP, &CPlatenControl::OnBnClickedTotop)
	ON_BN_CLICKED(IDC_TOBOTTOM, &CPlatenControl::OnBnClickedTobottom)
	ON_BN_CLICKED(IDC_SETASDEFAULT, &CPlatenControl::OnBnClickedSetasdefault)
END_MESSAGE_MAP()


// CPlatenControl message handlers


BOOL CPlatenControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MoveVal = 0;

	m_Up.SetCheck(TRUE);

	m_RearL.SetCheck(TRUE);
	m_RearR.SetCheck(TRUE);
	m_FrontL.SetCheck(TRUE);
	m_FrontR.SetCheck(TRUE);

	WINDOWPLACEMENT  wp1, wp2, wp3, wp4, wp5, wp6, wp7;

	m_Spin1000.GetWindowPlacement(&wp1);
	m_Spin100.GetWindowPlacement(&wp2);
	m_Spin10.GetWindowPlacement(&wp3);
	m_Spin1.GetWindowPlacement(&wp4);

	wp1.rcNormalPosition.right = wp1.rcNormalPosition.left + 70;
	wp2.rcNormalPosition.right = wp2.rcNormalPosition.left + 70;
	wp3.rcNormalPosition.right = wp3.rcNormalPosition.left + 70;
	wp4.rcNormalPosition.right = wp4.rcNormalPosition.left + 70;

	m_Spin1000.SetWindowPlacement(&wp1);
	m_Spin100.SetWindowPlacement(&wp2);
	m_Spin10.SetWindowPlacement(&wp3);
	m_Spin1.SetWindowPlacement(&wp4);


	DisplaySteps();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CPlatenControl::DisplaySteps() {
char tmp[128];

	sprintf(tmp, "%d", MoveVal);

	m_Steps.SetWindowText(tmp);



}

void CPlatenControl::OnDeltaposUd1000(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) MoveVal += 1000;
	else if(MoveVal >= 1000) MoveVal -= 1000;

	DisplaySteps();


	*pResult = 0;
}


void CPlatenControl::OnDeltaposUd100(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) MoveVal += 100;
	else if(MoveVal >= 100) MoveVal -= 100;

	DisplaySteps();

	*pResult = 0;
}


void CPlatenControl::OnDeltaposUd10(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) MoveVal += 10;
	else if(MoveVal >= 10) MoveVal -= 10;

	DisplaySteps();

	*pResult = 0;
}


void CPlatenControl::OnDeltaposUd1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(pNMUpDown->iDelta < 0) MoveVal += 1;
	else if(MoveVal >= 1) MoveVal -= 1;

	DisplaySteps();


	*pResult = 0;
}


void CPlatenControl::OnBnClickedMove() {
unsigned char steppers = 0;
int direction;


	if(!MoveVal) return;

	if(m_RearL.GetCheck()) steppers |= 0x01;
	if(m_RearR.GetCheck()) steppers |= 0x08;
	if(m_FrontL.GetCheck()) steppers |= 0x02;
	if(m_FrontR.GetCheck()) steppers |= 0x04;


	direction = m_Up.GetCheck();

	unsigned long cmd = PLP_EXTCMD_MOVE << 4;
	cmd |= steppers;
	unsigned short move = (unsigned short)MoveVal;



	/**/USBPrinterCommandEx(USB_CMD_PLATEN_COMMAND, cmd, (move & 0xFF00) >> 8, move & 0x00FF, direction, 0, 0, 0);

	//USBPrinterCommandEx(USB_CMD_PLATEN_COMMAND, 9, 1, 2, 3, 0, 0, 0);


}


void CPlatenControl::OnBnClickedDirUp()
{

	m_Up.SetCheck(TRUE);
	m_Down.SetCheck(FALSE);


}


void CPlatenControl::OnBnClickedDirDown()
{
	m_Up.SetCheck(FALSE);
	m_Down.SetCheck(TRUE);
}


void CPlatenControl::OnBnClickedClear()
{
	MoveVal = 0;
	DisplaySteps();

}


void CPlatenControl::OnBnClickedTotop()
{
	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 11);
}



void CPlatenControl::OnBnClickedTobottom()
{
	USBPrinterCommand(USB_CMD_SET_PLATEN_HEIGHT, 10);
}


void CPlatenControl::OnBnClickedSetasdefault()
{
	theApp.GVars->iDefaultPlatenHeight = MoveVal;
	theApp.WriteINI();


}

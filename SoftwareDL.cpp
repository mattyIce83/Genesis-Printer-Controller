// SoftwareDL.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "Controller4Dlg1.h"
#include "SoftwareDL.h"
#include "afxdialogex.h"


// CSoftwareDL dialog

extern CController4Dlg1 *theDlg;

static BOOL foundsomething;


CSoftwareDL *thisDlg;

IMPLEMENT_DYNAMIC(CSoftwareDL, CDialogEx)

CSoftwareDL::CSoftwareDL(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSoftwareDL::IDD, pParent)
{

}

CSoftwareDL::~CSoftwareDL()
{
}

void CSoftwareDL::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DL_MESSAGES, m_Status1);
	DDX_Control(pDX, IDOK, m_OKSWDL);
}


BEGIN_MESSAGE_MAP(CSoftwareDL, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CSoftwareDL::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CMyEdit2, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

/*
G_Vars.szFPGAMain
G_Vars.szFPGAAux
G_Vars.szController
*/

/*------------------------------------------------------------*/
/*-                                                          -*/
/*------------------------------------------------------------*/
void SWLoader(void *v) {
WIN32_FIND_DATA  c_file;
HANDLE rc;
BOOL oktolook;
char filters[3][MAX_PATH];
char tmp_buff2[MAX_PATH];
char filter[MAX_PATH];
char szDestination[MAX_PATH];
char *dst_folders[3] = {theApp.GVars->szController, theApp.GVars->szFPGAMain, theApp.GVars->szFPGAAux};
int i;
PVOID OldValue = NULL;


	foundsomething = FALSE;
	theApp.CloseRequest = TRUE;
	theApp.bHomePressed = TRUE;


char *looking_messages[3] = {"..... Looking for controller software\r\n",
							"..... Looking for main system software\r\n",
							"..... Looking for auxiliary software\r\n"};

char *copying_messages[3] = {"Copying controller software..... \r\n",
							"Copying main system software..... \r\n",
							"Copying auxiliary software..... \r\n"};


	CreateDirectory(theApp.GVars->szFPGAMain, &theApp.ThreadSA);
	CreateDirectory(theApp.GVars->szFPGAAux, &theApp.ThreadSA);
	CreateDirectory(theApp.GVars->szController, &theApp.ThreadSA);

	sprintf(filters[0], "%s\\GENESIS_UPDATE\\CONTROLLER", theApp.GVars->szUpdateDrive);
	sprintf(filters[1], "%s\\GENESIS_UPDATE\\MAIN_FIRMWARE", theApp.GVars->szUpdateDrive);
	sprintf(filters[2], "%s\\GENESIS_UPDATE\\AUX_FIRMWARE", theApp.GVars->szUpdateDrive);


	for(i = 0; i < 3; i++) {
	
		thisDlg->StatusLine(looking_messages[i]);

		sprintf(filter, "%s\\*.*", filters[i]);

		Wow64DisableWow64FsRedirection(&OldValue);
		rc = FindFirstFile(filter, &c_file);
		Wow64RevertWow64FsRedirection(OldValue);

		if(rc != INVALID_HANDLE_VALUE) {
			oktolook = TRUE;
			//thisDlg->StatusLine(copying_messages[i]);
		}
		else {
			oktolook = FALSE;
		}

		while(oktolook) {

			if(strlen(c_file.cFileName) > 3) {
	
				foundsomething = TRUE;
				sprintf(tmp_buff2, "%s\\%s", filters[i], c_file.cFileName);
				thisDlg->StatusLine("Copying: %s\n", c_file.cFileName);
				if(!_strcmpi(c_file.cFileName, "genesis_launcher.exe")) {
					sprintf(szDestination, "%s\\%s", theApp.LaunchDir, c_file.cFileName);
				}
				else {
					sprintf(szDestination, "%s\\%s", dst_folders[i], c_file.cFileName);
				}
				DeleteFile(szDestination);
				/**/ TRACE("Copying %s to %s\n", tmp_buff2, szDestination);
				if(CopyFile(tmp_buff2,szDestination, FALSE)) {
				/**/ TRACE("Copy Success\n");
				}
				else {
				/**/ TRACE("Copy FAILED!!!\n");
				}


			}
			Wow64DisableWow64FsRedirection(&OldValue);
			oktolook = FindNextFile(rc, &c_file);
			Wow64RevertWow64FsRedirection(OldValue);
		}
		FindClose(rc);


	}
	if(foundsomething) {
		thisDlg->StatusLine(".... Software copying complete\r\nClick OK To Install\r\n");
		theApp.GVars->bPendingUpdate = TRUE;
		theApp.WriteINI();
	}
	else {
		thisDlg->StatusLine("\r\n\r\n**** NO SOFTWARE UPDATES FOUND *****\r\n");
	}


	thisDlg->m_OKSWDL.ShowWindow(SW_SHOW);

	if(foundsomething) {
		theApp.CloseRequest = TRUE;
		theApp.bHomePressed = TRUE;
	}


	Sleep(500);
	//thisDlg->OnBnClickedOk();
	//thisDlg->bTimeToExit = TRUE;

}



// CSoftwareDL message handlers
BOOL CSoftwareDL::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	thisDlg = this;
	
	bTimeToExit = FALSE;

	m_OKSWDL.SetFont(&theDlg->pMediumFont);

	m_OKSWDL.ShowWindow(SW_HIDE);

	m_CharsInWindow = 0;

	m_Status1.SetFont(&theDlg->pNewFont);
	m_Status1.SetBkColor(RGB(0,0,0));

	m_nTimer = SetTimer(7171, 1000, 0);

	DLLoaderThread = AfxBeginThread((AFX_THREADPROC)SWLoader,
													(void *)NULL,
													0,
													0,	
													0,
													&theApp.ThreadSA);




	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSoftwareDL::OnPaint()
{
		CPaintDC dc(this); // device context for painting
	
		CRect rect;
		GetClientRect(&rect);

		// FILL THE BACKGROUND
		//dc.FillRect(&rect, &theApp.hBGbrush);

		CDialog::OnPaint();
		theApp.DisplayBitmap(&dc, &theApp.hSWDLBackground, 0, 0, DB_SHOWUPPERLEFT);
}
/*----------------------------------------------------------------*/
/*-   Send status message to message window                      -*/
/*----------------------------------------------------------------*/
void CSoftwareDL::StatusLine(char *message,...) {
int 	len;
char	tmpbuf[1024];
va_list	args;
static unsigned char n_virgin;

	if(theApp.quit_threads)
		return;

	va_start (args, message);
	vsprintf (tmpbuf, message, args);
	va_end (args);

	len = strlen(tmpbuf);

	if(len + m_CharsInWindow >= 1000) {
		m_Status1.SetWindowText("");
		m_CharsInWindow = 0;
	}

	m_Status1.SetSel(m_CharsInWindow, m_CharsInWindow,FALSE);
	m_Status1.ReplaceSel(tmpbuf,FALSE);

	m_CharsInWindow += len;
	
}


void CSoftwareDL::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	KillTimer(m_nTimer);
	CDialogEx::OnOK();
}


void CSoftwareDL::OnTimer(UINT_PTR nIDEvent) {


	if(nIDEvent == 7171) {

		if(bTimeToExit) {
			KillTimer(nIDEvent);
			OnBnClickedOk();
		}

	}

	CDialogEx::OnTimer(nIDEvent);
}

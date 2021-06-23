#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define TOTAL_PATTERNS		3

#define YL_VARS_FN	"YL_VARS.DAT"

#define ALIGNMENT_1200DPI_BAND_FILE "1200DPI_Y_STEP_CALIBRATE.tif"
#define ALIGNMENT_600DPI_BAND_FILE "600DPI_Y_STEP_CALIBRATE.tif"

#define MAX_TABLE_SLOT	94

#define YL_TABLE_LENGTH	    MAX_TABLE_SLOT + 1

typedef struct _testpatterns {

	char szName[128];
	char szFileName[128];
} testpatterns;


typedef struct _YLVars {

	int Rep;
	int unused1;
	CProgressCtrl *Progress;
	int YAdvance;
	float Top;
	BOOL SendThreadRunning;
	int CorVals[4][YL_TABLE_LENGTH];
	int CurrentEditVar;
	float LM;
	BOOL CancelRequest;
	int iDirection;
	CEdit *Status;
	int iCounter;
	BOOL bExternalCall;
	int YParams;
	BOOL bVerifyBands;
	int iCurTestPattern;
	int iCurInkSetup;
} YLVars;

// CYLinearization dialog

class CYLinearization : public CDialogEx
{
	DECLARE_DYNAMIC(CYLinearization)

public:
	CYLinearization(CWnd* pParent = NULL);   // standard constructor
	virtual ~CYLinearization();


	YLVars ylVars;
	void SaveVars(char *);
	void LoadVars(char *);
	void LoadVarsExt(char *);

	char SliderDisplay[150][4];

	void DisplayStripes();

	void DisplayLM();

	void DisplaySteps();

	void PopulateSliders(void);

	void DisplayCounter(void);

	void DisplayYParams(void);

	void DisplayVerifyBands(void);

	void SizeDialog();

	void SendValues(void *);

	testpatterns TestPatterns[TOTAL_PATTERNS];

	CWinThread *AlignementTestThread;

// Dialog Data
	enum { IDD = IDD_YLINEARIZATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSpinButtonCtrl m_LMUD;
	CSpinButtonCtrl m_StripesUD;
	virtual BOOL OnInitDialog();
	CEdit m_LM;
	CEdit m_Stripes;
	CSpinButtonCtrl m_ValUD;
	CSpinButtonCtrl m_IndexUD;
	CEdit m_CorValPre;
	CEdit m_CorValCur;
	CEdit m_CorValPost;
	CEdit m_IndexPre;
	CEdit m_IndexCur;
	CEdit m_IndexPost;
	CSpinButtonCtrl m_Spin1000;
	CSpinButtonCtrl m_Spin100;
	CSpinButtonCtrl m_Spin10;
	CSpinButtonCtrl m_Spin1;
	CProgressCtrl m_Progress;
	CEdit m_Steps;
	afx_msg void OnDeltaposIndexud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposFeedud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposUd1000(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposUd100(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposUd10(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposUd1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnDeltaposValud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposLmud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposStripesud(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancelPrint();
	afx_msg void OnBnClickedPrintTest();
	CEdit m_Status;
	afx_msg void OnBnClickedSendvalues();
	afx_msg void OnBnClickedReset();
	CEdit m_Counter;
	afx_msg void OnBnClickedGotozero();
	afx_msg void OnBnClickedGotoselected();
	CComboBox m_YParams;
	afx_msg void OnCbnSelchangeYparams();
	CButton m_VerifyBands;
	afx_msg void OnBnClickedVerifyBands();
	CComboBox m_TestPattern;
	afx_msg void OnCbnSelchangeTestPattern();
	afx_msg void OnEnChangeReps2();
	afx_msg void OnEnChangeLm();
	CComboBox m_InkOrder;
	afx_msg void OnCbnSelchangeInksetup();
};

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define TB_VARS_FN	"TB_VARS.DAT"

typedef struct _testbandVars {

	char szTestFiles[4][MAX_PATH];
	int ChannelToUse[4];
	int Rep;
	CProgressCtrl *Progress;
	CEdit *Status;
	int XResolution;
	int YAdvance;
	BOOL bForce1Bit;
	BOOL bVerifyBands;
	BOOL bBiDirectional;
	float Top;
	int iDirection;
	BOOL SendThreadRunning;
	int YParams;
	float LM;
	int InkOrder;
} testbandVars;


// CTestBands dialog

class CTestBands : public CDialogEx
{
	DECLARE_DYNAMIC(CTestBands)

public:
	CTestBands(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestBands();

	void LoadVars(char *);

	void SaveVars(char *);

	void DisplaySteps();

	void DisplayReps();

	void DisplayTop();

	void DisplayBidirectional();

	void DisplayForceOneBit();

	void DisplayVerifyBands();

	void SizeDialog();
	
	void DisplayYParams();

	void 	DisplayLM();



	testbandVars tbVars;

	CWinThread *TestBandThread;

	BOOL OldBHT;

	// Dialog Data
	enum { IDD = IDD_TEST_BAND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSpinButtonCtrl m_Spin1000;
	CSpinButtonCtrl m_Spin100;
	CSpinButtonCtrl m_Spin10;
	CSpinButtonCtrl m_Spin1;
	afx_msg void OnDeltaposUd1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	CEdit m_Steps;
	afx_msg void OnDeltaposUd10(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposUd100(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposUd1000(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_Bitmap1;
	CEdit m_Bitmap2;
	CEdit m_Bitmap3;
	CEdit m_Bitmap4;
	CComboBox m_Channel1;
	CComboBox m_Channel2;
	CComboBox m_Channel3;
	CComboBox m_Channel4;
	afx_msg void OnBnClickedBrowse1();
	afx_msg void OnBnClickedBrowse2();
	afx_msg void OnBnClickedBrowse3();
	afx_msg void OnBnClickedBrowse4();
	afx_msg void OnBnClickedPrint();
	CEdit m_Reps;
	CProgressCtrl m_Progress;
	afx_msg void OnDestroy();
	CEdit m_Status;
	afx_msg void OnEnChangeBitmap1();
	afx_msg void OnEnChangeBitmap2();
	afx_msg void OnEnChangeBitmap3();
	afx_msg void OnEnChangeBitmap4();
	afx_msg void OnCbnSelchangeChannel1();
	afx_msg void OnCbnSelchangeChannel2();
	afx_msg void OnCbnSelchangeChannel3();
	afx_msg void OnCbnSelchangeChannel4();
	afx_msg void OnEnChangeReps();
	CComboBox m_XResolution;
	afx_msg void OnBnClickedButton1();
	CSpinButtonCtrl m_RepUD;
	afx_msg void OnDeltaposRepUd(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_ForceOneBit;
	afx_msg void OnBnClickedForceOnebit();
	CButton m_VerifyBands;
	afx_msg void OnBnClickedVerifyBands();
	CButton m_Bidirectional;
	afx_msg void OnBnClickedBidirectional();
	CSpinButtonCtrl m_TopUD;
	afx_msg void OnDeltaposTopUd(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_Top;
	CComboBox m_YParams;
	afx_msg void OnCbnSelchangeYparams();
	CEdit m_LM;
	CSpinButtonCtrl m_LMUD;
	afx_msg void OnDeltaposLmud2(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_CopyToA;
	CButton m_CopyToB;
	CButton m_CopyToC;
	CButton m_CopyToD;
};

#pragma once
#include "afxwin.h"


// CDebugDiag dialog

class CDebugDiag : public CDialogEx
{
	DECLARE_DYNAMIC(CDebugDiag)

public:
	
	CDebugDiag(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDebugDiag();

	void SizeDialog();
	char StateMessages[1024];
	char StateMessages2[1024];


// Dialog Data
	enum { IDD = IDD_DEBUG_DIAG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCap();
	afx_msg void OnBnClickedUncap();
//	afx_msg void OnBnClickedFillpeb();
	virtual BOOL OnInitDialog();
	//afx_msg void OnBnClickedBleedpeb();
	afx_msg void OnBnClickedHvOn();
	afx_msg void OnBnClickedHvOff();
	afx_msg void OnBnClickedWipe();
	afx_msg void OnBnClickedHomeplatenz();
	afx_msg void OnBnClickedPlatenheight();
	CEdit m_FeedAmount;
	afx_msg void OnBnClickedHeadtomaint();
	afx_msg void OnBnClickedHomehead();
	afx_msg void OnBnClickedNozzlecheck();
	afx_msg void OnBnClickedAutolevel();
	afx_msg void OnBnClickedMagenta();
	CEdit m_Channel;
	CEdit m_MaxV;
	CEdit m_Amplitude;
	CEdit m_AdjA;
	CEdit m_AdjB;
	CEdit m_AdjP;
	afx_msg void OnBnClickedSendcurve();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedLaserson();
	afx_msg void OnBnClickedLasersoff();
	afx_msg void OnBnClickedClearfifos();
	CEdit m_PltAmp;
	CEdit m_PltTck;
	afx_msg void OnBnClickedResetxy();
	afx_msg void OnBnClickedPage2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOpenink();
	afx_msg void OnBnClickedPlatentotop();
	afx_msg void OnBnClickedPlatentobottom();
	afx_msg void OnBnClickedTestpatterns();
	afx_msg void OnBnClickedPlatentoprintpos();
	afx_msg void OnBnClickedServoOffsets();
	afx_msg void OnBnClickedHeadtomaint2();
	afx_msg void OnBnClickedHeadtomaint3();
	afx_msg void OnBnClickedSpecial1();
	CEdit m_PicUnresp;
	CEdit m_PicReset;
	CEdit m_PicBadresp;
	afx_msg void OnBnClickedPicRefresh();
	afx_msg void OnBnClickedPicClear();
	CEdit m_PicBadTemp;
	CEdit m_StateMessages;
	CEdit m_StateMessages2;
	CButton m_Log;
	afx_msg void OnBnClickedLog();
	afx_msg void OnBnClickedPicClearlog();
	afx_msg void OnBnClickedPlatenPicTest();
	afx_msg void OnBnClickedForceLog();
	afx_msg void OnBnClickedSendData();
	afx_msg void OnBnClickedWhiteledon();
	afx_msg void OnBnClickedWhiteledoff();
	afx_msg void OnBnClickedPlatentoPrintPos();
	afx_msg void OnBnClickedPlatentoFeedPos();
	CEdit m_FeedHeight;
	afx_msg void OnBnClickedTestwaitdlg();
	afx_msg void OnBnClickedBagtimerOff();
	afx_msg void OnBnClickedBagtimerOn();
	afx_msg void OnBnClickedOk();
	CButton m_AutoCloseCB;
	afx_msg void OnBnClickedAutoclosecb();
	CEdit m_FillMinutes;
	afx_msg void OnBnClickedEditIni();
	CButton m_SupervisorMode;
	afx_msg void OnBnClickedSupervisorMode();
	CEdit m_Underruns;
	afx_msg void OnBnClickedResetPur();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedWipe2();
	afx_msg void OnBnClickedGenerictest();
};

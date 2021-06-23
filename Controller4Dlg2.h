#pragma once
#include "afxwin.h"

#include "transedit.h"
#include "afxcmn.h"
#include "MyIPAddressCtrl.h"

// CController4Dlg2 dialog


class CController4Dlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(CController4Dlg2)

public:
	CController4Dlg2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CController4Dlg2();

	void ShowVersions();

	void EnableDJAP();

	void ShowIPStuff(IP_ADAPTER_INFO *);

	BOOL bHasAdapterInfo;
	BOOL Initializing;

	PIP_ADAPTER_INFO pAdapterInfo;

	void SimulateKey(char);

	void SplitIPAddy(DWORD *, IP_ADDRESS_STRING *);
	void SplitIPAddyChr(unsigned char *, IP_ADDRESS_STRING *);

	void GetAndDisplayAdapters();

	void SetObstructionLaserButtonText();

	void DisplayHeaterTimeout();

	int iAdapterCount;

	int OldHeaterTimeout;

// Dialog Data
	enum { IDD = IDD_CONTROLLER4_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SizeDialog();
	int ButtonTop;
	int ButtonHeight;
	int ButtonWidth;
	int adapters;


	CBitmapButton m_Home;
	afx_msg void OnBnClickedHome();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRight();
	virtual HRESULT get_accFocus(VARIANT *pvarChild);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CButton m_CustBiDiLabel;
	CButton m_BiDiCor_Minus;
	CButton m_BiDiCor_Plus;
	CMyEdit m_BiDiCor_Val;
	//CEdit m_BiDiCor_Val;
	//TransEdit m_BiDiCor_Val;
	afx_msg void OnBnClickedBidiCorPlus();
	afx_msg void OnBnClickedBidicorMinus();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	CButton m_SetTemp;
	CBitmapButton m_BidiCorUP;
	CBitmapButton m_BidiCorDN;
	CBitmapButton m_HeadSetup;
	CMyEdit m_HeadConfig;
	//CEdit m_HeadConfig;
	afx_msg void OnBnClickedHeadsetup();
	CBitmapButton m_KeepJobsDN;
	CBitmapButton m_KeepJobsUP;
	CMyEdit m_KeepJobs;
	//CEdit m_KeepJobs;
	afx_msg void OnBnClickedBidiCorPlus2();
	afx_msg void OnBnClickedBidicorMinus2();
	CBitmapButton m_HDOnOff;
	CBitmapButton m_RTOnOff;
	afx_msg void OnBnClickedAutohd();
	afx_msg void OnBnClickedRapidrt();
	CMyEdit m_PrinterName;
	CMyEdit m_Server;
	CMyEdit m_IP1;
	CMyEdit m_IP2;
	CMyEdit m_IP3;
	CMyEdit m_IP4;
	CBitmapButton m_ColorJobsCleanUp;
	CBitmapButton m_ColorJobsCleanDown;
	CBitmapButton m_WhiteJobsCleanUp;
	CBitmapButton m_WhiteJobsCleanDown;
	CMyEdit m_ColorJobsClean;
	CMyEdit m_WhiteJobsClean;
	CButton m_AutoIP;
	CButton m_ManualIP;
	afx_msg void OnBnClickedColorCleanJobsPlus();
	afx_msg void OnBnClickedColorCleanJobsMinus();
	afx_msg void OnBnClickedWhiteCleanJobsPlus();
	afx_msg void OnBnClickedWhiteCleanJobsMinus();
	//CIPAddressCtrl m_IPAddress;
	CMyIPAddressCtrl m_IPAddress;
	afx_msg void OnBnClickedNettest();
	CEdit m_ServerName;
	CStatic m_ControllerVer;
	CStatic m_MainVer;
	CStatic m_AuxVer;
	CStatic m_SlaveVer;
	afx_msg void OnBnClickedGetVersions();
	afx_msg void OnCbnSelchangeLayerdelaySec();
	CListBox m_LayerDelaySec;

	afx_msg void OnIpnFieldchangedIpaddress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeAdapters();
	CButton m_EditIP;
	afx_msg void OnBnClickedChange();
	CComboBox m_AdapterList;
	CEdit m_ConnectionType;
	afx_msg void OnBnClickedDbg();
	CButton m_OneButtonOperation;
	afx_msg void OnBnClickedOnebuttonOperation();
	CButton m_DeleteJobAfterPrint;
	afx_msg void OnBnClickedDeleteAfterPrint();
	CStatic m_NetworkingGroup;
	CListBox m_PlatenGap;
	CButton m_IgnoreLasers;
	afx_msg void OnBnClickedIgnoreObstruction2();
	afx_msg void OnCbnSelchangePtOffset();
	CListBox m_PlatenSize;
	afx_msg void OnCbnSelchangePlatenSize();
	CStatic m_SerialNumber;
	CStatic m_TotalPrints;
	CBitmapButton m_PlatenGapUp;
	CBitmapButton m_PlatenGapDown;
	afx_msg void OnBnClickedPlatenGapPlus();
	afx_msg void OnBnClickedPlatenGapMinus2();
	afx_msg void OnLbnSelchangePltOffset();
	CBitmapButton m_PlatenSizeUp;
	CBitmapButton m_PlatenSizeDown;
	afx_msg void OnBnClickedPlatenSizeUp();
	afx_msg void OnBnClickedPlatenSizeDown();
	afx_msg void OnLbnSelchangePltnSize();
	CBitmapButton m_LayerDelayUp;
	CBitmapButton m_LayerDelayDown;
	afx_msg void OnBnClickedLayerdelayUp();
	afx_msg void OnBnClickedLayerdelayDown();
	CBitmapButton m_HeaterTimeoutUP;
	CBitmapButton m_HeaterTimeoutDown;
	afx_msg void OnEnChangeHeaterTimeout();
	CEdit m_HeaterTimeout;
	afx_msg void OnBnClickedHeaterTimeoutUp();
	afx_msg void OnBnClickedHeaterTimeoutDown();
};

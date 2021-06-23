#pragma once
#include "afxwin.h"
#include <time.h>

#define DB_SHOWATPOS		0
#define DB_SHOWUPPERRIGHT	1
#define DB_SHOWUPPERLEFT	2
#define DB_SHOWLOWERLEFT	3

#define MESSAGE_BUFFER_SIZE	3000000

#define	TEMP_DISPLAY_UPDATE2		7171
#define HALF_SECOND_TIMER			7172

#define TOOLS_BUTTON			1
#define MAINT_BUTTON			2
#define SETTINGS_BUTTON			3
#define CENTER_BUTTON			4
#define JOB_BUTTON				5
#define VANITY_SCREEN			6
#define MAIN_COUNTER_BUTTON		7
#define CURRENT_COUNTER_BUTTON	8


#define TOTAL_BUTTONS		9

#define CIRCLE_WIDTH		535
#define CIRCLE_HEIGHT		535
#define SLICE_HEIGHT		40

#define CIRCLE_LEFT_BOUNDARY	60
#define CIRCLE_RIGHT_BOUNDARY  (CIRCLE_LEFT_BOUNDARY + CIRCLE_WIDTH)
#define CIRCLE_TOP_BOUDARY		105
#define CIRCLE_BOTTOM_BOUDARY	(CIRCLE_TOP_BOUDARY + CIRCLE_HEIGHT)



typedef struct _polybuttonarea {

	int slices;

	RECT slice[40];

} polybuttonarea;




// CController4Dlg1 dialog

class CController4Dlg1 : public CDialogEx
{
	DECLARE_DYNAMIC(CController4Dlg1)

public:
	CController4Dlg1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CController4Dlg1();

	CBitmap hMainBackground;
	CBitmap hMainBackgroundSettings;
	CBitmap hMainBackgroundMaint;
	CBitmap hMainBackgroundTools;
	CBitmap hMainBackgroundCenter;
	//CBitmap hMainBackgroundPower;

	CBitmap hScanningGreen;
	CBitmap hScanningYellow;
	CBitmap hScanningRed;
	CBitmap hScanningBlack;
	CBitmap hScanningWhite;
	CBitmap hScanningBlue;
	CBitmap hScanningPurple;
	CBitmap hScanningPink;

	CBitmap hCartWarnF;
	CBitmap hCartWarnY;
	CBitmap hCartWarnM;
	CBitmap hCartWarnC;
	CBitmap hCartWarnK;
	CBitmap hCartWarnW;

	CBitmap *CartWarnImage[6];

	CRect InkMarkersDisplay[6];
	CRect InkMarkersOriginal[6];

	int iButtons;
	int iPrevButtons;
	void *currentDlg;

	int iBlinkObstruction;
	BOOL bObstructionMarkerState;

	time_t GetSystemSecs();


	polybuttonarea ButtonArea[TOTAL_BUTTONS];

	void DrawButtonSlices(CPaintDC *, int);
	void DefineButtonAreas();
	int FindPressedButton(CPoint *);
	void HandleButtonPress(int);
	void RedrawInkMarkers(int channel);
	void UpdatePRIcon();
	void UpdatePRIconNoBlink();

	BOOL TimeToFillCap();


	CCriticalSection		MainWindowCS;

	BOOL bButtonDown;

	WINDOWPLACEMENT prv_wp;
	BOOL GotPrevSize;
	RECT prv_rect;

	LOGPEN logPen;
	CPen	hCPen;

	LOGPEN logPenX;
	CPen	hCPenX;

	LOGPEN logPenX2;
	CPen	hCPenX2;

	CBrush	hBGbrushWhite;
	LOGBRUSH logBrushWhite;

	CBrush	hBGbrushGray;
	LOGBRUSH logBrushGray;


	WINDOWPLACEMENT pwp;

	CBrush	DrakGrayBackground;

	LOGBRUSH	lbDrakGrayBackground;


	int m_XOffset;
	int m_YOffset;
	int m_ResX;
	int m_ResY;

	time_t	time_at_click;
	time_t	time_at_release;
	time_t	button_duration;

	CDC *dcScreen;
	
	float m_AspectX;
	float m_AspectY;
	RECT rc;
	WINDOWPLACEMENT twp;
	CDC m_PicDC;
	float m_CanvasWidth;
	float m_CanvasHeight;

	void AdjustCanvas();

	void PreparePreview();

	void PreparePage();

	void SizeDialog();

	void Power();

	int m_CanvasPixelWidth;
	int m_CanvasPixelHeight;

	float m_FactorX;
	float m_FactorY;

	BITMAPHANDLE Bitmap;

	RECT crect;

	RECT inRect;

	RECT clrect;

	RECT job_name_rect;

	RECT size_rect;

	RECT print_mode_rect;

	RECT job_number_rect;

	RECT temp_rect;

	RECT counter_rect;

	RECT current_counter_rect;


	void DisplayPreview();

	void DisplayJobInfo();

	void ShowTemp(CPaintDC *);

	void ShowVersion(CPaintDC *);

	void ShowCount(CPaintDC *dc);

	void ShowCurrentCount(CPaintDC *);

	void LoadBackgroundBitmaps();
	
	void RefreshTemp();

	void StatusLine(char *,...);

	QUEUE_ITEM *CreateNewQueueItem(char *);

	void OKBox(char *, char *, char *);

	void ForcePreviewRefresh(BOOL );

	int m_CharsInWindow;

	int m_nTimer;

	int m_nTimer2;

	QUEUE_ITEM *queue;

	CFont pNewFont;
	CFont pMediumFont;
	CFont pSmallFont;
	CFont pSmallFontVert;
	CFont pSmallFont2;
	CFont pDigitalFont;
	CFont pLargeFont;

	HGLOBAL  mb_mem;

	BOOL bIsAReprint;

	int LastCurrentJob;

// Dialog Data
	enum { IDD = IDD_CONTROLLER4_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	CStatic m_Preview;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_Status1;
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedOk();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedSymPrint();
	CButton m_SymPrint;
	CStatic m_ScanIndicator;
	afx_msg void OnStnClickedScanMarker();
	afx_msg void OnBnClickedScannerDebug();
	CStatic m_BagMarker1;
	CStatic m_BagMarker2;
	CStatic m_BagMarker3;
	CStatic m_BagMarker4;
	CStatic m_BagMarker5;
	CStatic m_BagMarker0;
	CButton m_ForceButton;
	CStatic m_PRIcon;
	CStatic m_ObstructionSign;
};

#pragma once
#include "afxwin.h"
#include "MyCtrlList.h"
#include "MyScrollButton.h"

#define PREVIEW_MODE_SIMPLE		0
#define PREVIEW_MODE_ADVANCED	1


#define PREV_WIN_HOT_ULX	9
#define PREV_WIN_HOT_ULY	380
#define PREV_WIN_HOT_LRX	208
#define PREV_WIN_HOT_LRY	625

#define TEMP_DISPLAY_UPDATE	7070
#define SCROLL_UPDATE		7575



// CController4Dlg4 dialog

class CController4Dlg4 : public CDialogEx
{
	DECLARE_DYNAMIC(CController4Dlg4)

public:
	CController4Dlg4(CWnd* pParent = NULL);   // standard constructor
	virtual ~CController4Dlg4();

	//BOOL bIsAReprint;

	void SizeDialog();
	int ButtonTop;
	int ButtonHeight;
	int ButtonWidth;
	int iPreviewMode;

	void DisplayQueue();
	void DisplayQueueItem(QUEUE_ITEM *);
	void DeleteQueueItem(QUEUE_ITEM *);
	void DeleteQueueItem(QUEUE_ITEM *, BOOL);
	int SaveSelections();
	void OnBnClickedDelete();
	void RestoreSelections();
	void CreateNewQueueItem(char *);
	//void OnBnClickedReprint();
	void ProtectItems();
	void ProtectItem();
	QUEUE_ITEM *GetQItemInfo();
	int ProtectedItems();
	BOOL CheckForJobOverflow();
	void ReconcileIcons();
	void ShowTemp(CPaintDC *dc);
	void SetNewCurrentJob();
	void SetProductionModeButton(QUEUE_ITEM *);

	LOGBRUSH logBrush;
	LOGBRUSH logBrush2;
	CBrush	hBGbrush;
	CBrush	hBGbrush2;
	LOGPEN logPen;
	CPen	hCPen;

	WINDOWPLACEMENT pwp;
	
	BOOL bButtonDown;

	int				m_XOffset;
	int				m_YOffset;
	CDC				*dcScreen;
	CDC				m_PicDC;
	RECT			rc;

	CDC				*hdc;
	RECT			rDest;
	RECT			rSource;
	int				m_ResX;
	int				m_ResY;
	float			m_AspectX;
	float			m_AspectY;
	int				m_DrawingWindowWidth;
	int				m_DrawingWindowHeight;
	float			factor;
	float			m_CanvasWidth;
	float			m_CanvasHeight;

	int				m_nTimer;
	int				m_nScrollTimer;



	BOOL bInPreview;


// Dialog Data
	enum { IDD = IDD_CONTROLLER4_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CBitmapButton m_Home;
	afx_msg void OnBnClickedHome();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedHlp();
	afx_msg void OnBnClickedProtect();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	CButton m_ConfirmBands;
	//CListCtrl m_JobList;
	CMyCtrlList m_JobList;

	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnBnClickedReprint();
	afx_msg void OnNMClickJoblist(NMHDR *pNMHDR, LRESULT *pResult);
	//CStatic m_PreviewWindow;
	afx_msg void OnPaint();
	afx_msg void OnStnClickedPreview();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//CEdit m_Copies;
	//CStatic m_CopiesLabel;
	//CStatic m_JobLabel;
//	CStatic m_LayersLabel;
	//CStatic m_ResolutionLabel;
	//CStatic m_SizeLabel;
//	CStatic m_LayerLabel;
	//CStatic m_LayersLabel;
	afx_msg void OnBnClickedImportjobs();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedConfirmbands();
	afx_msg void OnLvnItemchangedJoblist(NMHDR *pNMHDR, LRESULT *pResult);
	CBitmapButton m_Reprint;
	CBitmapButton m_Protect;
	CButton m_Import;
	CBitmapButton m_Export;
	CBitmapButton m_Delete;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	afx_msg void OnKeydownJoblist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemactivateJoblist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnclickJoblist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	/*
	CBitmapButton m_LeftArrow;
	CBitmapButton m_RightArrow;
	*/
	CMyScrollButton m_LeftArrow;
	CMyScrollButton m_RightArrow;
	afx_msg void OnBnClickedScrollLeft();
	afx_msg void OnBnClickedScrollRight();
	afx_msg void OnBnDoubleclickedScrollRight();
	afx_msg void OnBnDoubleclickedScrollLeft();
	CEdit m_JobSpecs;
	CBitmapButton m_ProdRun;
	afx_msg void OnBnClickedProdrun();
};

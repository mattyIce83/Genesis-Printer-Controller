#pragma once

#define LCSB_CLIENTDATA 1
#define LCSB_NCOVERRIDE 2


#include "afxcmn.h"
class CMyCtrlList :
	public CListCtrl
{
public:

	WINDOWPLACEMENT pos;
	BOOL bButtonDown;
	int Anchor;

	void HideScrollBars(int Type, int Which=SB_BOTH);



	CMyCtrlList(void);
	~CMyCtrlList(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
private:
	BOOL NCOverride;
	int Who;

public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


#pragma once


// CEditableListCtrl

class CEditableListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEditableListCtrl)

public:
	CEditableListCtrl();
	virtual ~CEditableListCtrl();

	int HitTestEx(CPoint &, int *);

	CEdit *EditSubLabel(int, int);

	void OnEndLabelEdit(NMHDR*, LRESULT*);

	BOOL bChanged;

	BOOL *changeList;

	//virtual COLORREF OnGetCellTextColor(int,int ); 
	//virtual COLORREF OnGetCellTextColor(int nRow,int nColumn) ;



protected:
	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg COLORREF OnGetCellTextColor(int,int ); 


	afx_msg void OnLvnItemchangedEntries(NMHDR *pNMHDR, LRESULT *pResult);
};



#pragma once
#include "afxcmn.h"
#include "EditableListCtrl.h"

// CINIEditor dialog

class CINIEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CINIEditor)

public:
	CINIEditor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CINIEditor();
	void Populate();
	void SetItem(int, char *, char *, char *, char *);
	BOOL changeList[MAXINITKEYS];
	void GetVals();



// Dialog Data
	enum { IDD = IDD_INIEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//CListCtrl m_Entries;
	CEditableListCtrl m_Entries;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCustomdrawEntries(NMHDR *pNMHDR, LRESULT *pResult);
};

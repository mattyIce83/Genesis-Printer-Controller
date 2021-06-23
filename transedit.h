// The MFC header class --> transedit.h

#pragma once

 #pragma pack(push, 1)

class TransEdit : public CEdit
 {
     CBrush m_Brush;
     COLORREF m_clrText;
public:
     TransEdit();
     void SetTextColor(COLORREF clr);
	 void SetBackgroundBitmap(CBitmap *);
	 CBitmap *bkbitmap;
     DECLARE_DYNCREATE(TransEdit)

private:
     void PrepareBackground();
     DECLARE_MESSAGE_MAP()

protected:
     //{{AFX_MSG(TransEdit)
     afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
     //}}AFX_MSG
public:
     afx_msg void OnEnUpdate();
     afx_msg void OnSize(UINT nType, int cx, int cy);
 };

#pragma pack(pop)



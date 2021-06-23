// The TransEdit implementation --> transedit.cpp

#include "stdafx.h"
#include "transedit.h"

 BEGIN_MESSAGE_MAP(TransEdit, CEdit)
     ON_WM_CTLCOLOR_REFLECT()
     ON_CONTROL_REFLECT(EN_UPDATE, OnEnUpdate)
     ON_WM_SIZE()
 END_MESSAGE_MAP()

 IMPLEMENT_DYNCREATE(TransEdit, CEdit)


 TransEdit::TransEdit()
 {
     m_clrText = GetSysColor(COLOR_WINDOWTEXT);
 }

void TransEdit::SetTextColor(COLORREF clr)
 {
     m_clrText = clr;
 }

void TransEdit::SetBackgroundBitmap(CBitmap *bm)
 {
     bkbitmap = bm;
 }


HBRUSH TransEdit::CtlColor(CDC *pDC, UINT nCtlColor)
 {
     pDC->SetBkMode(TRANSPARENT);
     pDC->SetTextColor(m_clrText);
     return (HBRUSH)m_Brush;
 }

/*
void TransEdit::PrepareBackground()
 {
     if( m_Brush.m_hObject!=0 )
         return;

     CWnd * pParent = GetParent();
     CBitmap bkgnd;
     CClientDC dc(pParent);
     CDC thisMem;
     CRect pos, rc;
     GetWindowRect(pos);
     rc = pos;
     ScreenToClient(rc);
     pParent->ScreenToClient(pos);
     pos.InflateRect(rc.left, rc.top);

     CBitmap* pOldThisBmp;
     thisMem.CreateCompatibleDC( &dc );
     CSize sz = pos.Size();

     bkgnd.DeleteObject();
     bkgnd.CreateCompatibleBitmap(&dc, sz.cx, sz.cy );
     pOldThisBmp = thisMem.SelectObject(&bkgnd);
     thisMem.BitBlt(0, 0, sz.cx, sz.cy, &dc, pos.left, pos.top, SRCCOPY);
     thisMem.SelectObject(pOldThisBmp);
     m_Brush.DeleteObject();
     m_Brush.CreatePatternBrush(&bkgnd);
 }
*/

 void TransEdit::PrepareBackground()
 {
     if( m_Brush.m_hObject!=0 )
         return;

     CWnd * pParent = GetParent();
     CBitmap *bkgnd = bkbitmap;
     CClientDC dc(pParent);
     CDC thisMem;
     CRect pos, rc;
     GetWindowRect(pos);
     rc = pos;
     ScreenToClient(rc);
     pParent->ScreenToClient(pos);
     pos.InflateRect(rc.left, rc.top);

     CBitmap* pOldThisBmp;
     thisMem.CreateCompatibleDC( &dc );
     CSize sz = pos.Size();

     //bkgnd.DeleteObject();
     //bkgnd.CreateCompatibleBitmap(&dc, sz.cx, sz.cy );
     pOldThisBmp = thisMem.SelectObject(bkgnd);
     thisMem.BitBlt(0, 0, sz.cx, sz.cy, &dc, pos.left, pos.top, SRCCOPY);
     thisMem.SelectObject(pOldThisBmp);
     m_Brush.DeleteObject();
     m_Brush.CreatePatternBrush(bkgnd);
 }

void TransEdit::OnEnUpdate()
 {
     CRect rc;
     CWnd * pParent = GetParent();
     GetWindowRect(rc);
     pParent->ScreenToClient(rc);
     pParent->InvalidateRect(rc, 1);
     PrepareBackground();
 }

void TransEdit::OnSize(UINT nType, int cx, int cy)
 {
     m_Brush.DeleteObject();
     BOOL b = IsWindowVisible();
     CEdit::OnSize(nType, cx, cy);
     if( b )
         ShowWindow(SW_HIDE);
     OnEnUpdate();
     if( b )
         ShowWindow(SW_SHOW);
 }



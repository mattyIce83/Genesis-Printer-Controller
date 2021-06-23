// MyScrollButton.cpp : implementation file
//

#include "stdafx.h"
#include "Controller4.h"
#include "MyScrollButton.h"


// CMyScrollButton

IMPLEMENT_DYNAMIC(CMyScrollButton, CBitmapButton)

CMyScrollButton::CMyScrollButton()
{
	bButtonActive = FALSE;

}

CMyScrollButton::~CMyScrollButton()
{
}


BEGIN_MESSAGE_MAP(CMyScrollButton, CBitmapButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CMyScrollButton message handlers




void CMyScrollButton::OnLButtonDown(UINT nFlags, CPoint point)
{

	/**/ TRACE("Button down\n");
	bButtonActive = TRUE;
	ActiveCount = 0;
	CBitmapButton::OnLButtonDown(nFlags, point);
}


void CMyScrollButton::OnLButtonUp(UINT nFlags, CPoint point)
{

	/**/ TRACE("Button up\n");
	bButtonActive = FALSE;
	CBitmapButton::OnLButtonUp(nFlags, point);
}

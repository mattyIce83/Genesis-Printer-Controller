#include "StdAfx.h"
#include "MyCtrlList.h"


CMyCtrlList::CMyCtrlList(void)
{
	bButtonDown = FALSE;
	NCOverride=FALSE; //False as default...
	Who=SB_BOTH; //Default remove both...
}


CMyCtrlList::~CMyCtrlList(void)
{
}
BEGIN_MESSAGE_MAP(CMyCtrlList, CListCtrl)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


void CMyCtrlList::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
#ifdef DEBUG_MOUSE_DRAG
	TRACE("MyCtrlList LBUTTONDOWN\n");
#endif

	CListCtrl::OnNcLButtonDown(nHitTest, point);
}


void CMyCtrlList::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CListCtrl::OnNcLButtonUp(nHitTest, point);
}


void CMyCtrlList::OnMouseMove(UINT nFlags, CPoint point)
{
CSize s;

	if(bButtonDown && (GetKeyState(VK_LBUTTON) & 0x80) != 0) {
		if(point.x > Anchor) {
			if(point.x - Anchor > 40) {
				Anchor = point.x;
#ifdef DEBUG_MOUSE_DRAG
				TRACE("LEFT\n");
#endif
				s.cy = 0;
				s.cx = -60;
				Scroll(s);
			}
		}
		if(point.x < Anchor) {
			if(Anchor - point.x >  40) {
				Anchor = point.x;
#ifdef DEBUG_MOUSE_DRAG
				TRACE("LEFT\n");
#endif
				s.cy = 0;
				s.cx = 60;
				Scroll(s);
			}
		}
	}

	CListCtrl::OnMouseMove(nFlags, point);
}


void CMyCtrlList::OnLButtonDown(UINT nFlags, CPoint point)
{

#ifdef DEBUG_MOUSE_DRAG
	TRACE("CMyCtrlList BUTTON DOWN\n");
#endif

	bButtonDown = TRUE;
	Anchor = point.x;

	CListCtrl::OnLButtonDown(nFlags, point);

}


void CMyCtrlList::OnLButtonUp(UINT nFlags, CPoint point)
{

#ifdef DEBUG_MOUSE_DRAG
	TRACE("MyCtrlList BUTTON UP\n");
#endif
	bButtonDown = FALSE;


	CListCtrl::OnLButtonUp(nFlags, point);
}


BOOL CMyCtrlList::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{

	

	return CListCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMyCtrlList::HideScrollBars(int Type, int Which)
{
	if(Type==LCSB_CLIENTDATA) //This is the clientrect function
	{
		RECT ierect;
		int cxvs, cyvs;
		GetClientRect(&ierect); //Get client width and height

		cxvs = GetSystemMetrics (SM_CXVSCROLL); //Get the system metrics - VERT
		cyvs = GetSystemMetrics (SM_CYVSCROLL); //Get the system metrics - HORZ
		
		if(Which==SB_HORZ) cxvs=0; //Set VERT to zero when choosen HORZ
		if(Which==SB_VERT) cyvs=0; //Set HORZ to zero when choosen VERT

		//Here we set the position of the window to the clientrect + the size of the scrollbars
		SetWindowPos(NULL, ierect.left, ierect.top, ierect.right+cxvs, ierect.bottom+cyvs, SWP_NOMOVE | SWP_NOZORDER);

		//Her we modify the rect so the right part is subbed from the rect.
		if(Which==SB_BOTH||Which==SB_HORZ) ierect.bottom -= ierect.top;
		if(Which==SB_BOTH||Which==SB_VERT) ierect.right -= ierect.left;

		//Just to be safe that the left/top corner is 0...
		ierect.top = 0;
		ierect.left = 0;
		
		HRGN iehrgn = NULL; //This range is created base on which scrollbar that is going to be removed!

		//The -2 is probably a border of some kind that we also need to remove. I could not find any good
		//metrics that gave me an 2 as an answer. So insted we makes it static with -2.
		if(Which==SB_BOTH) iehrgn=CreateRectRgn (ierect.left, ierect.top, ierect.right-2, ierect.bottom-2);
		if(Which==SB_HORZ) iehrgn=CreateRectRgn (ierect.left, ierect.top, ierect.right, ierect.bottom-2);
		if(Which==SB_VERT) iehrgn=CreateRectRgn (ierect.left, ierect.top, ierect.right-2, ierect.bottom);
		
		//After the range has been made we add it...
		SetWindowRgn (iehrgn, TRUE);

		//Reset of NCOverride
		NCOverride=FALSE;
	}

	if(Type==LCSB_NCOVERRIDE) //This is the NcCalcSize override
	{
		NCOverride=TRUE; //Set to true, so we run the code on each OnNcCalcSize.
		Who=Which; //Selects which scrollbars to get hidden.
	}
}


void CMyCtrlList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
#ifdef DEBUG_MOUSE_DRAG
	TRACE("MyCtrl double-click\n");
#endif

	CListCtrl::OnLButtonDblClk(nFlags, point);
}

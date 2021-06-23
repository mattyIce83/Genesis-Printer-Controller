#pragma once


// CMyScrollButton

class CMyScrollButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CMyScrollButton)

public:
	CMyScrollButton();
	virtual ~CMyScrollButton();

	BOOL bButtonActive;
	int ActiveCount;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};



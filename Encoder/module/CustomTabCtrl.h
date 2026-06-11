#pragma once


// CCustomTabCtrl

class AFX_EXT_CLASS CCustomTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CCustomTabCtrl)

public:
	CCustomTabCtrl();
	virtual ~CCustomTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawTabBorder(CDC* pDC, CRect& TabRC);
public:
	void DrawItemFrame(DRAWITEMSTRUCT* lpDrawItem);
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
public:
	afx_msg void OnPaint();
protected:
	virtual void PreSubclassWindow();
};



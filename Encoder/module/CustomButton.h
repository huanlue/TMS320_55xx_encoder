#pragma once


// CCustomButton

class AFX_EXT_CLASS CCustomButton : public CButton
{
	DECLARE_DYNAMIC(CCustomButton)

public:
	CCustomButton();
	virtual ~CCustomButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void DrawBK(CDC * pDC, UINT ResID);
	void DrawBK(CDC* pDC, short r0, short g0, short b0, short rn, short gn, short bn, COLORREF crFrameColor);
public:
	BOOL	m_bIsInRect;

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void PreSubclassWindow();
public:
	int m_nStyle;	//表面样式，0为固定模式，图片按钮；其它为不同的热点按钮格式，为默认值为1。
	UINT m_uResId;
};



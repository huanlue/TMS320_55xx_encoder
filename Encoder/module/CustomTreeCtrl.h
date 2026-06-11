#pragma once

// CCustomTreeCtrl
/////////////////////////////////////////////////////////////////////////////
// CBitmapTree window
class CMemDCTree : public CDC 
{
private:
	CBitmap*	m_Bmp;
	CBitmap*	m_OldBmp;
	CDC*		m_pDC;
	CRect		m_Rect;
public:
	CMemDCTree(CDC* pDC, const CRect& rect) : CDC()
	{
		CreateCompatibleDC(pDC);
		m_Bmp = new CBitmap;
		m_Bmp->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		m_OldBmp = SelectObject(m_Bmp);
		m_pDC = pDC;
		m_Rect = rect;
	}
	~CMemDCTree() 
	{
		m_pDC->BitBlt(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height(), 
			this, m_Rect.left, m_Rect.top, SRCCOPY);
		SelectObject(m_OldBmp);
		if(m_Bmp != NULL) 
			delete m_Bmp;
	}
};

class AFX_EXT_CLASS CCustomTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CCustomTreeCtrl)

public:
	CCustomTreeCtrl();
	virtual ~CCustomTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};



// StatListCtrl.cpp : 实现文件
//

#include "pch.h"
#include "ColorListCtrl.h"

class CMemDCList : public CDC 
{
private:
	CBitmap*	m_Bmp;
	CBitmap*	m_OldBmp;
	CDC*		m_pDC;
	CRect		m_Rect;
public:
	CMemDCList(CDC* pDC, const CRect& rect) : CDC()
	{
		CreateCompatibleDC(pDC);
		m_Bmp = new CBitmap;
		m_Bmp->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		m_OldBmp = SelectObject(m_Bmp);
		m_pDC = pDC;
		m_Rect = rect;
	}
	~CMemDCList() 
	{
		m_pDC->BitBlt(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height(), 
			this, m_Rect.left, m_Rect.top, SRCCOPY);
		SelectObject(m_OldBmp);
		if(m_Bmp != NULL) 
			delete m_Bmp;
	}
};

// CStatListCtrl

IMPLEMENT_DYNAMIC(CColorListCtrl, CListCtrl)

CColorListCtrl::CColorListCtrl()
{

}

CColorListCtrl::~CColorListCtrl()
{

}


BEGIN_MESSAGE_MAP(CColorListCtrl, CListCtrl)
//	ON_WM_PAINT()
END_MESSAGE_MAP()

void CColorListCtrl::PreSubclassWindow()
{
	CHeaderCtrl* m_head = GetHeaderCtrl();
	m_ctlHeader.SubclassWindow(m_head->GetSafeHwnd());
	SetBkColor(RGB(204, 238, 251));
	SetTextBkColor(RGB(204, 238, 251));

	CListCtrl::PreSubclassWindow();
}

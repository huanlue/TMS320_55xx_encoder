#if !defined(AFX_MYSPLITTERWND_H__C53CF73B_FD68_46B2_9703_12D108B9D4DC__INCLUDED_)
#define AFX_MYSPLITTERWND_H__C53CF73B_FD68_46B2_9703_12D108B9D4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySplitterWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySplitterWnd window

class AFX_EXT_CLASS CMySplitterWnd : public CSplitterWnd
{
// Construction
public:
	CMySplitterWnd();

// Attributes
public:

// Operations
public:

	void  ShowRow(int nColumn, int nRow);
	void  HideRow(int nColumn, int nRow);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySplitterWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMySplitterWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMySplitterWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
public:
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
public:
	virtual void OnInvertTracker(const CRect& rect);
};

class AFX_EXT_CLASS CMyFixSplitterWnd :
	public CMySplitterWnd
{
public:
	CMyFixSplitterWnd(void);
public:
	~CMyFixSplitterWnd(void);
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPLITTERWND_H__C53CF73B_FD68_46B2_9703_12D108B9D4DC__INCLUDED_)

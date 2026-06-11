#pragma once


// CStaticEdit

class AFX_EXT_CLASS CStaticEdit : public CEdit
{
	DECLARE_DYNAMIC(CStaticEdit)

public:
	CStaticEdit();
	virtual ~CStaticEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnKillfocus();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};



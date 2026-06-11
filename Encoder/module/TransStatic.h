#pragma once


// CTransStatic

class AFX_EXT_CLASS CTransStatic : public CStatic
{
	DECLARE_DYNAMIC(CTransStatic)

public:
	CTransStatic();
	virtual ~CTransStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};



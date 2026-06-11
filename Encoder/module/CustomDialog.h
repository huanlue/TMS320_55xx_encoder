#pragma once


// CCustomDialog ¶Ô»°¿ò

class CCustomDialog : public CDialog
{

public:
	CCustomDialog(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CCustomDialog(void);


	DECLARE_MESSAGE_MAP()

protected:

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

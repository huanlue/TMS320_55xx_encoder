#pragma once


// CMyDialog 对话框

class AFX_EXT_CLASS CMyDialog : public CDialog
{

public:
	CMyDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDialog(void);

	// 对话框数据

protected:

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int ShowBack(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	int m_nBackNo;
};

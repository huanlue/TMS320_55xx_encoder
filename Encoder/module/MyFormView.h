#pragma once



// CMyFormView 窗体视图

class AFX_EXT_CLASS CMyFormView : public CFormView
{

protected:
	CMyFormView(UINT nIDTemplate, CWnd* pParent = NULL);           // 动态创建所使用的受保护的构造函数
	virtual ~CMyFormView();

public:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int ShowBack(CDC* pDC);

	int m_nBackNo;


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};



#pragma once

#include "module\\MyFormView.h"
#include "module\\custombutton.h"
#include "module\\levelstatic.h"
#include "module\\transstatic.h"

#include "CDialogSymTabDst.h"

// CViewFileDstFile 窗体视图

class CViewFileDstFile : public CMyFormView
{
	DECLARE_DYNCREATE(CViewFileDstFile)

protected:
	CViewFileDstFile();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewFileDstFile();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE_DSTFILE };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	bool m_bInit;
	CMFCTabCtrl	m_wndTab;
	int m_nCurSelTab;
	CDialogSymTabDst* m_pDialogSymTabDst;
	LRESULT OnChangeActiveTab(WPARAM wp, LPARAM /*lp*/);

	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};



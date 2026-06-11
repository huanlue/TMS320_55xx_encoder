#pragma once

#include "module\\MyFormView.h"
#include "module\\custombutton.h"
#include "module\\levelstatic.h"
#include "module\\transstatic.h"

#include "CDialogSymTab.h"
// CViewFileSrcFile 窗体视图

class CViewFileSrcFile : public CMyFormView
{
	DECLARE_DYNCREATE(CViewFileSrcFile)

protected:
	CViewFileSrcFile();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewFileSrcFile();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE_SRCFILE };
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

	bool m_bInit;
	CMFCTabCtrl	m_wndTab;
	int m_nCurSelTab;


public:
	CDialogSymTab* m_pDialogSymTab;

	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnChangeActiveTab(WPARAM wp, LPARAM /*lp*/);

};



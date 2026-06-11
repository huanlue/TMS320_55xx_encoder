#pragma once

#include "module\\MyDialog.h"
#include "module\\custombutton.h"
#include "module\\levelstatic.h"
#include "module\\transstatic.h"

#include "ColorListCtrl.h"
#include "elf.h"

// CDialogSectionTab 对话框

class CDialogSectionTab : public CMyDialog
{
	DECLARE_DYNAMIC(CDialogSectionTab)

public:
	CDialogSectionTab(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogSectionTab();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SECTION_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CColorListCtrl m_lstSections;
	bool m_bIsInit;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();

	int ShowSections(Elf32_Shdr* secs, CString* strnames, int num);
};

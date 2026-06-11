#pragma once

#include "module\\MyDialog.h"
#include "module\\custombutton.h"
#include "module\\levelstatic.h"
#include "module\\transstatic.h"

// CDialogHeadInfo 对话框

class CDialogHeadInfo : public CMyDialog
{
	DECLARE_DYNAMIC(CDialogHeadInfo)

public:
	CDialogHeadInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogHeadInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HEAD_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctlFileinfoResult;
	CString m_strEdtFileinfoResult;
	bool m_bIsInit;

	void AddString(CString str);
	void SetString(CString str);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

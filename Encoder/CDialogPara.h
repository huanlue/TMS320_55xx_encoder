#pragma once

#include "module\\MyDialog.h"
#include "module\\custombutton.h"
#include "module\\levelstatic.h"
#include "module\\transstatic.h"

// CDialogPara 对话框

class CDialogPara : public CMyDialog
{
	DECLARE_DYNAMIC(CDialogPara)

public:
	CDialogPara(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogPara();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PARA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CCustomButton m_btnOk;
	CCustomButton m_btnCancel;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComboBox m_cmbTransbit1;
	CComboBox m_cmbTransbit2;
	int m_nTransbit1;
	int m_nTransbit2;
	virtual BOOL OnInitDialog();
};

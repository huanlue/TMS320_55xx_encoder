#pragma once

#include "module\\MyDialog.h"
#include "module\\custombutton.h"
#include "module\\levelstatic.h"
#include "module\\transstatic.h"

// CDialogBasicInfo 对话框

class CDialogBasicInfo : public CMyDialog
{
	DECLARE_DYNAMIC(CDialogBasicInfo)

public:
	CDialogBasicInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogBasicInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BASIC_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctrEdtBaseinfoResult;
	CString m_strEdtBaseinfoResult;
	bool m_bIsInit;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int ShowBaseInfo(CStdioFile& file);

	//追加显示统计信息的函数声明
	void AddString(CString str);
	void SetString(CString str);
	void AppendStatisticsDisplay(int nTotal, int nChanged, double dScramble, int nValid, double dValid);
};

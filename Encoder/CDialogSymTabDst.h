#pragma once

#include "module\\MyDialog.h"
#include "module\\custombutton.h"
#include "module\\levelstatic.h"
#include "module\\transstatic.h"

#include "ColorListCtrl.h"
#include "elf.h"

// 💡 引入 vector 容器，用于支持我们新增的差异记录矩阵
#include <vector>

// CDialogSymTabDst 对话框

class CDialogSymTabDst : public CMyDialog
{
	DECLARE_DYNAMIC(CDialogSymTabDst)

public:
	CDialogSymTabDst(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogSymTabDst();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SYM_TAB_DST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CColorListCtrl m_ctlListSyms;
	bool m_bIsInit;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// 将原本的单文件 ShowInfo 修改为双文件对比版
	int ShowInfo(CStdioFile& fileSrc, CStdioFile& fileDst);

	// 列表自绘通知消息的响应函数，用来给不同的格子涂上红色
	afx_msg void OnNMCustomdrawListSyms(NMHDR* pNMHDR, LRESULT* pResult);

private:
	// 二维布尔矩阵，用来记录哪些行、哪些列的数据前后不一致
	std::vector<std::vector<bool>> m_diffMatrix;
};
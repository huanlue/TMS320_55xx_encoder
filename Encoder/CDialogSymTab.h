#pragma once
#include "module\\MyDialog.h"
#include "module\\custombutton.h"
#include "module\\levelstatic.h"
#include "module\\transstatic.h"

#include "ColorListCtrl.h"
#include "elf.h"
#include <vector>
#include <string>

struct InstructionRule {
	unsigned int mask;
	unsigned int value;
	std::string mnemonic;
};

// CDialogSymTab 对话框
class CDialogSymTab : public CMyDialog
{
	DECLARE_DYNAMIC(CDialogSymTab)

public:
	CDialogSymTab(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogSymTab();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SYM_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CColorListCtrl m_ctlListSyms;
	bool m_bIsInit;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int ShowInfo(CStdioFile& file);
	afx_msg void OnBnClickedButtonModify();
	CCustomButton m_btnModify;
	//void AddValidInstructionStatistics(int nTotal, int nValid, double dRate);
	std::vector<InstructionRule> g_instructionSetRules;
	bool LoadInstructionSet(const CString& strFilePath);
	bool IsInstructionValid(unsigned int cmd);

public:
	// 偏移量输入框
	CEdit m_editOffset;
	CStatic m_staticOffset;
	int m_nOffset;  // 偏移量（字节数）
protected:
	afx_msg void OnEnChangeEditOffset();  // 偏移量输入框内容变化消息


public:
	// 添加这两个新函数的声明
	bool ConvertHexTextToBinary(const CString& strInputPath, const CString& strOutputPath);
	bool ConvertBinaryToHexText(const CString& strInputPath, const CString& strOutputPath);



	// 👇 这是新添加的私有成员区域
private:
	bool ProcessBinaryCrypto(const CString& strInputPath, const CString& strOutputPath, int nOffset = 0);
	//bool CalculateInstructionStats(const CString& strInputPath, const CString& strOutputPath, int& nTotal, int& nChanged, double& dScrambleRate);
	void CalculateAndDisplayStatistics(const CString& strInputPath, const CString& strOutputPath);
};
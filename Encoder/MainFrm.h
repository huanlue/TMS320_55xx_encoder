// MainFrm.h: CMainFrame 类的接口
//

#pragma once
#include "ChildView.h"
#include "CFileInfo.h"
#include "CViewFileDstFile.h"
#include "CViewFileSrcFile.h"

#include "elf.h"

class CMainFrame : public CFrameWnd
{

public:
	CMainFrame() noexcept;
protected:
	DECLARE_DYNAMIC(CMainFrame)

	// 特性
public:

	// 操作
public:

	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CReBar		m_wndReBar;        //Rebar条

	CChildView    m_wndView;

	CSplitterWndEx m_wndSplitter;   // 三栏分割窗口

public:
	CFileInfo* m_pViewFileInfo;
	CViewFileSrcFile* m_pViewFileSrcFile;
	CViewFileDstFile* m_pViewFileDstFile;
	CString  m_strFilepathName;
	CString  m_strFilepath;
	CString  m_strResultFile;
	unsigned long	m_nFileLength;

	unsigned short m_nTransBit1;
	unsigned short m_nTransBit2;

	// 窗口默认尺寸
	static const int DEFAULT_WINDOW_WIDTH = 1400;
	static const int DEFAULT_WINDOW_HEIGHT = 800;
	static const int MIN_WINDOW_WIDTH = 900;
	static const int MIN_WINDOW_HEIGHT = 600;

	// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);

	afx_msg void OnFileOpen1(void);
	afx_msg void OnFilePara(void);
	afx_msg void OnFileClose(void);

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	BOOL CreateExToolBar(void);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	CString GetPath(CString fullpath);
};
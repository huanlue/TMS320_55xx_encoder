// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "Encoder.h"
#include <WinUser.h>
#include "MainFrm.h"

#include "CDialogPara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()

	ON_COMMAND(IDC_FILE_OPEN1, OnFileOpen1)
	ON_COMMAND(IDC_FILE_PARA, OnFilePara)
	ON_COMMAND(IDC_FILE_CLOSE, OnFileClose)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	m_pViewFileInfo = NULL;
	m_pViewFileSrcFile = NULL;
	m_pViewFileDstFile = NULL;

	m_nTransBit1 = 16;
	m_nTransBit2 = 17;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	AfxGetApp()->m_pMainWnd = this;

	if (!CreateExToolBar())
	{
		return (-1);
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// 设置窗口默认大小并居中显示
	int nScreenX = GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(NULL,
		(nScreenX - DEFAULT_WINDOW_WIDTH) / 2,
		(nScreenY - DEFAULT_WINDOW_HEIGHT) / 2,
		DEFAULT_WINDOW_WIDTH,
		DEFAULT_WINDOW_HEIGHT,
		SWP_NOZORDER);

	return 0;
}

BOOL CMainFrame::CreateExToolBar(void)
{
	CImageList img;
	CString str;

	if (!m_wndReBar.Create(this))
	{
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		return -1;
	}

	// 设置按钮的宽度和长度
	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(40, 80);

	// 设置"热"的位图
	img.Create(30, 30, ILC_COLOR8 | ILC_MASK, 2, 2);
	img.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	img.Add(AfxGetApp()->LoadIcon(IDI_ICON_OPEN1));  // 打开源文件
	img.Add(AfxGetApp()->LoadIcon(IDI_ICON_PARA));   // 打开加扰文件
	img.Add(AfxGetApp()->LoadIcon(IDI_ICON_CLOSE));  // 关闭文件
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&img);
	img.Detach();

	// 设置"冷"的位图
	img.Create(20, 20, ILC_COLOR8 | ILC_MASK, 2, 2);
	img.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	img.Add(AfxGetApp()->LoadIcon(IDI_ICON_OPEN1));  // 打开源文件
	img.Add(AfxGetApp()->LoadIcon(IDI_ICON_PARA));   // 设置参数
	img.Add(AfxGetApp()->LoadIcon(IDI_ICON_CLOSE));  // 关闭文件
	m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();

	// 改变属性
	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | CBRS_TOOLTIPS | TBSTYLE_TRANSPARENT | TBBS_CHECKBOX);
	m_wndToolBar.SetButtons(NULL, 2);

	m_wndToolBar.SetButtonInfo(0, IDC_FILE_OPEN1, TBSTYLE_BUTTON, 0);
	m_wndToolBar.SetButtonText(0, "打开源文件");

	m_wndToolBar.SetButtonInfo(1, IDC_FILE_CLOSE, TBSTYLE_BUTTON, 2);
	m_wndToolBar.SetButtonText(1, "关闭文件");

	CRect rectToolBar;
	m_wndToolBar.GetItemRect(0, &rectToolBar);
	m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(20, 20));

	// 在Rebar中加入ToolBar
	m_wndReBar.AddBar(&m_wndToolBar);

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE | RBBIM_BACKGROUND;
	rbbi.cxMinChild = rectToolBar.Width();
	rbbi.cyMinChild = rectToolBar.Height();
	rbbi.hbmBack = LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	rbbi.cx = rbbi.cxIdeal = rectToolBar.Width() * 10;

	m_wndReBar.GetReBarCtrl().SetBandInfo(0, &rbbi);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	// 允许调整大小、最大化、最小化
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	// 设置默认窗口大小
	cs.cx = DEFAULT_WINDOW_WIDTH;
	cs.cy = DEFAULT_WINDOW_HEIGHT;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}

// 设置窗口最小尺寸限制
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// 设置最小窗口大小
	lpMMI->ptMinTrackSize.x = MIN_WINDOW_WIDTH;
	lpMMI->ptMinTrackSize.y = MIN_WINDOW_HEIGHT;

	// 不限制最大尺寸，允许全屏
	// lpMMI->ptMaxTrackSize 保持默认

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CRect rectClient;
	GetClientRect(rectClient);

	// 计算三栏宽度：左边20%，中间40%，右边40%
	int nTotalWidth = rectClient.Width();
	int nLeftWidth = (int)(nTotalWidth * 0.1);
	int nMiddleWidth = (int)(nTotalWidth * 0.45);
	int nRightWidth = nTotalWidth - nLeftWidth - nMiddleWidth;

	// 创建三栏静态分割窗口
	if (!m_wndSplitter.CreateStatic(this, 1, 3))
	{
		TRACE0("未能创建分割窗口\n");
		return FALSE;
	}

	// 设置三栏的初始宽度和最小宽度
	m_wndSplitter.SetColumnInfo(0, nLeftWidth, 50);     // 左边最小50像素
	m_wndSplitter.SetColumnInfo(1, nMiddleWidth, 100);   // 中间最小100像素
	m_wndSplitter.SetColumnInfo(2, nRightWidth, 100);    // 右边最小100像素

	// 创建三个视图
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CFileInfo),
		CSize(nLeftWidth, rectClient.Height()), pContext))
	{
		TRACE0("未能创建左边视图\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CViewFileSrcFile),
		CSize(nMiddleWidth, rectClient.Height()), pContext))
	{
		TRACE0("未能创建中间视图\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0, 2, RUNTIME_CLASS(CViewFileDstFile),
		CSize(nRightWidth, rectClient.Height()), pContext))
	{
		TRACE0("未能创建右边视图\n");
		return FALSE;
	}

	// 获取视图指针
	m_pViewFileInfo = DYNAMIC_DOWNCAST(CFileInfo, m_wndSplitter.GetPane(0, 0));
	m_pViewFileSrcFile = DYNAMIC_DOWNCAST(CViewFileSrcFile, m_wndSplitter.GetPane(0, 1));
	m_pViewFileDstFile = DYNAMIC_DOWNCAST(CViewFileDstFile, m_wndSplitter.GetPane(0, 2));

	return TRUE;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// 窗口最小化时不处理
	if (nType == SIZE_MINIMIZED || m_pViewFileInfo == NULL)
	{
		return;
	}

	// 如果窗口被最大化，也会进入这里
	CRect rect;
	GetClientRect(rect);

	if (rect.Width() <= 0 || rect.Height() <= 0)
	{
		return;
	}

	// 保持三栏比例：20% - 40% - 40%
	int nTotalWidth = rect.Width();
	int nLeftWidth = (int)(nTotalWidth * 0.10);
	int nMiddleWidth = (int)(nTotalWidth * 0.45);
	int nRightWidth = nTotalWidth - nLeftWidth - nMiddleWidth;

	// 确保最小宽度
	if (nLeftWidth < 50) nLeftWidth = 50;
	if (nMiddleWidth < 100) nMiddleWidth = 100;
	if (nRightWidth < 100) nRightWidth = 100;

	// 设置行高（占满整个高度）
	m_wndSplitter.SetRowInfo(0, rect.Height(), 50);

	// 设置三栏宽度
	m_wndSplitter.SetColumnInfo(0, nLeftWidth, 50);
	m_wndSplitter.SetColumnInfo(1, nMiddleWidth, 100);
	m_wndSplitter.SetColumnInfo(2, nRightWidth, 100);

	// 重新布局
	m_wndSplitter.RecalcLayout();

	// 调试输出
	TRACE("OnSize: 总宽度=%d, 左=%d(%.1f%%), 中=%d(%.1f%%), 右=%d(%.1f%%)\n",
		nTotalWidth,
		nLeftWidth, (double)nLeftWidth / nTotalWidth * 100,
		nMiddleWidth, (double)nMiddleWidth / nTotalWidth * 100,
		nRightWidth, (double)nRightWidth / nTotalWidth * 100);
}

void CMainFrame::OnFileOpen1(void)
{
	CFileDialog dlg(TRUE, _T(".bin"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("(*.bin)|*.bin||"), this);

	if (dlg.DoModal() != IDOK)
	{
		AfxMessageBox(_T("无法找到资源文件"));
		return;
	}

	CStdioFile file;

	if (file.m_hFile != CFile::hFileNull)
	{
		file.Close();
	}

	m_strFilepathName = dlg.GetPathName();           // 获取文件路径
	file.Open(m_strFilepathName, CFile::typeBinary); // 打开文件
	m_nFileLength = file.GetLength();                // 获取文件长度

	m_strFilepath = GetPath(m_strFilepathName);

	if (file.m_hFile == CFile::hFileNull)
	{
		return;
	}

	// 显示文件信息
	if (m_pViewFileInfo && m_pViewFileInfo->m_pDialogBasicInfo)
	{
		m_pViewFileInfo->m_pDialogBasicInfo->ShowBaseInfo(file);
	}

	// 显示源文件内容
	if (m_pViewFileSrcFile && m_pViewFileSrcFile->m_pDialogSymTab)
	{
		m_pViewFileSrcFile->m_pDialogSymTab->ShowInfo(file);
	}

	file.Close();
	return;
}

void CMainFrame::OnFilePara(void)
{
	CDialogPara dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_nTransBit1 = dlg.m_nTransbit1;
		m_nTransBit2 = dlg.m_nTransbit2;
	}
	return;
}

void CMainFrame::OnFileClose(void)
{
	DestroyWindow();
	return;
}

CString CMainFrame::GetPath(CString fullpath)
{
	CString retpath = "";
	char buff[1024];
	strcpy_s(buff, fullpath);

	int len = strlen(buff);
	int i;
	for (i = len - 1; i >= 0; i--)
	{
		if (buff[i] == '\\')
		{
			buff[i] = 0;
			break;
		}
	}

	if (i != 0)
	{
		retpath = buff;
	}

	return retpath;
}
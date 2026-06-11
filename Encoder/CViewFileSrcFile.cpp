// CViewFileSrcFile.cpp: 实现文件
//

#include "pch.h"
#include "Encoder.h"
#include "CViewFileSrcFile.h"


// CViewFileSrcFile

IMPLEMENT_DYNCREATE(CViewFileSrcFile, CMyFormView)

CViewFileSrcFile::CViewFileSrcFile()
	: CMyFormView(IDD_DIALOG_FILE_SRCFILE)
{
	m_bInit = false;
	m_nCurSelTab = 0;

	m_pDialogSymTab = NULL;
}

CViewFileSrcFile::~CViewFileSrcFile()
{
}

void CViewFileSrcFile::DoDataExchange(CDataExchange* pDX)
{
	CMyFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewFileSrcFile, CMyFormView)
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)

END_MESSAGE_MAP()


// CViewFileSrcFile 诊断

#ifdef _DEBUG
void CViewFileSrcFile::AssertValid() const
{
	CMyFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewFileSrcFile::Dump(CDumpContext& dc) const
{
	CMyFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewFileSrcFile 消息处理程序


void CViewFileSrcFile::OnInitialUpdate()
{
	CMyFormView::OnInitialUpdate();

	if (m_bInit == true)
	{
		return;
	}
	m_bInit = true;


	CRect rctdlg;
	CRect rcttmp;

	CMFCTabCtrl::Style style;
	CMFCTabCtrl::Location location;
	CArray<COLORREF, COLORREF> arColors;
	int nTab = 0;

	GetClientRect(&rctdlg);
	rcttmp = rctdlg;

	m_wndTab.Create(CMFCTabCtrl::STYLE_3D_ROUNDED_SCROLL, rcttmp, this, 1, CMFCTabCtrl::LOCATION_TOP);
	//m_wndTab.SetImageList(IDB_BITMAP_TAB_ICONS, 16, RGB(255, 0, 255));

	m_pDialogSymTab = new CDialogSymTab;
	m_pDialogSymTab->Create(CDialogSymTab::IDD, &m_wndTab);
	m_wndTab.AddTab(m_pDialogSymTab, _T("文件内容"), 0, TRUE);

	style = CMFCTabCtrl::STYLE_3D_ROUNDED_SCROLL;
	m_wndTab.ModifyTabStyle(style);
	m_wndTab.RecalcLayout();
	m_wndTab.RedrawWindow();

	location = CMFCTabCtrl::LOCATION_TOP;
	m_wndTab.SetLocation(location);
	m_wndTab.RecalcLayout();
	m_wndTab.RedrawWindow();

	arColors.Add(RGB(121, 210, 231));  // '-'?????
	arColors.Add(RGB(190, 218, 153));
	arColors.Add(RGB(255, 170, 100));

	m_wndTab.EnableAutoColor(TRUE);
	m_wndTab.SetAutoColors(arColors);

	for (nTab = 0; nTab < m_wndTab.GetTabsNum(); nTab++)
	{
		m_wndTab.SetTabIcon(nTab, nTab);
	}

	m_wndTab.EnableTabSwap(TRUE);
	m_wndTab.SetResizeMode(CMFCTabCtrl::RESIZE_HORIZ);

	m_wndTab.MoveWindow(rcttmp);
}


void CViewFileSrcFile::OnSize(UINT nType, int cx, int cy)
{
	CMyFormView::OnSize(nType, cx, cy);

	if (m_bInit == false)
	{
		return;
	}

	CRect rctdlg;
	CRect rcttmp;

	GetClientRect(&rctdlg);

	rcttmp = rctdlg;
	m_wndTab.MoveWindow(rcttmp);

	m_pDialogSymTab->Invalidate();
}


LRESULT CViewFileSrcFile::OnChangeActiveTab(WPARAM wp, LPARAM /*lp*/)
{
	CString str;
	str.Format(_T("AFX_WM_CHANGE_ACTIVE_TAB: %d"), (int)wp);

	m_nCurSelTab = (int)wp;

	return 0;
}
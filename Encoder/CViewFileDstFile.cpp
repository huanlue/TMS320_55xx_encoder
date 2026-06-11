// CViewFileDstFile.cpp: 实现文件
//

#include "pch.h"
#include "Encoder.h"
#include "CViewFileDstFile.h"

// CViewFileDstFile

IMPLEMENT_DYNCREATE(CViewFileDstFile, CMyFormView)

CViewFileDstFile::CViewFileDstFile()
	: CMyFormView(IDD_DIALOG_FILE_DSTFILE)
{
	m_bInit = false;
	m_nCurSelTab = 0;

	m_pDialogSymTabDst = NULL;

}

CViewFileDstFile::~CViewFileDstFile()
{
}

void CViewFileDstFile::DoDataExchange(CDataExchange* pDX)
{
	CMyFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewFileDstFile, CMyFormView)
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)

END_MESSAGE_MAP()

// CViewFileDstFile 诊断

#ifdef _DEBUG
void CViewFileDstFile::AssertValid() const
{
	CMyFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewFileDstFile::Dump(CDumpContext& dc) const
{
	CMyFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// CViewFileDstFile 消息处理程序

void CViewFileDstFile::OnInitialUpdate()
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

	m_pDialogSymTabDst = new CDialogSymTabDst;
	m_pDialogSymTabDst->Create(CDialogSymTabDst::IDD, &m_wndTab);
	m_wndTab.AddTab(m_pDialogSymTabDst, _T("文件内容"), 0, TRUE);

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

void CViewFileDstFile::OnSize(UINT nType, int cx, int cy)
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

	m_pDialogSymTabDst->Invalidate();
}

LRESULT CViewFileDstFile::OnChangeActiveTab(WPARAM wp, LPARAM /*lp*/)
{
	CString str;
	str.Format(_T("AFX_WM_CHANGE_ACTIVE_TAB: %d"), (int)wp);

	m_nCurSelTab = (int)wp;

	return 0;
}
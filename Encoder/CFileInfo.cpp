// CFileInfo.cpp: 实现文件
//

#include "pch.h"
#include "Encoder.h"
#include "CFileInfo.h"


// CFileInfo

IMPLEMENT_DYNCREATE(CFileInfo, CMyFormView)

CFileInfo::CFileInfo()
	: CMyFormView(IDD_DIALOG_FILE_INFO)
{
	m_bInit = false;
	m_nCurSelTab = 0;

	m_pDialogBasicInfo = NULL;
	m_pDialogHeadInfo = NULL;
	m_pDialogSectionTab = NULL;

}

CFileInfo::~CFileInfo()
{
}

void CFileInfo::DoDataExchange(CDataExchange* pDX)
{
	CMyFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileInfo, CFormView)
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)

END_MESSAGE_MAP()


// CFileInfo 诊断

#ifdef _DEBUG
void CFileInfo::AssertValid() const
{
	CMyFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFileInfo::Dump(CDumpContext& dc) const
{
	CMyFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFileInfo 消息处理程序


void CFileInfo::OnInitialUpdate()
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

	m_pDialogBasicInfo = new CDialogBasicInfo;
	m_pDialogBasicInfo->Create(CDialogBasicInfo::IDD, &m_wndTab);
	m_wndTab.AddTab(m_pDialogBasicInfo, _T("基本信息"), 0, TRUE);

	m_pDialogHeadInfo = new CDialogHeadInfo;
	m_pDialogHeadInfo->Create(CDialogHeadInfo::IDD, &m_wndTab);
	m_wndTab.AddTab(m_pDialogHeadInfo, _T("头信息"), 0, TRUE);

	m_pDialogSectionTab = new CDialogSectionTab;
	m_pDialogSectionTab->Create(CDialogSectionTab::IDD, &m_wndTab);
	//m_wndTab.AddTab(m_pDialogSectionTab, _T("节区表"), 0, TRUE);

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

void CFileInfo::OnSize(UINT nType, int cx, int cy)
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

	m_pDialogBasicInfo->Invalidate();
	m_pDialogHeadInfo->Invalidate();
}

LRESULT CFileInfo::OnChangeActiveTab(WPARAM wp, LPARAM /*lp*/)
{
	CString str;
	str.Format(_T("AFX_WM_CHANGE_ACTIVE_TAB: %d"), (int)wp);

	m_nCurSelTab = (int)wp;

	return 0;
}

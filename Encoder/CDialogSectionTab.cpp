// CDialogSectionTab.cpp: 实现文件
//

#include "pch.h"
#include "Encoder.h"
#include "CDialogSectionTab.h"
#include "afxdialogex.h"


// CDialogSectionTab 对话框

IMPLEMENT_DYNAMIC(CDialogSectionTab, CMyDialog)

CDialogSectionTab::CDialogSectionTab(CWnd* pParent /*=nullptr*/)
	: CMyDialog(IDD_DIALOG_SECTION_TAB, pParent)
{
	m_bIsInit = false;

}

CDialogSectionTab::~CDialogSectionTab()
{
}

void CDialogSectionTab::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SECTIONS, m_lstSections);
}


BEGIN_MESSAGE_MAP(CDialogSectionTab, CMyDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDialogSectionTab 消息处理程序


void CDialogSectionTab::OnSize(UINT nType, int cx, int cy)
{
	CMyDialog::OnSize(nType, cx, cy);

	if (m_bIsInit == true)
	{
		CRect rcttmp;
		GetClientRect(&rcttmp);

		m_lstSections.MoveWindow(&rcttmp);
	}
}


BOOL CDialogSectionTab::OnInitDialog()
{
	CMyDialog::OnInitDialog();

	if (m_bIsInit == true)
	{
		return TRUE;
	}

	m_bIsInit = true;

	CRect rcttmp;
	GetClientRect(&rcttmp);

	m_lstSections.MoveWindow(&rcttmp);

	LV_COLUMN lvc;
	lvc.fmt = LVCFMT_LEFT;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_TEXT;

	lvc.iSubItem = 0;
	lvc.cx = 40;
	lvc.pszText = "序号";
	m_lstSections.InsertColumn(0, &lvc);

	lvc.iSubItem = 1;
	lvc.cx = 100;
	lvc.pszText = "name";
	m_lstSections.InsertColumn(1, &lvc);

	lvc.iSubItem = 2;
	lvc.cx = 40;
	lvc.pszText = "type";
	m_lstSections.InsertColumn(2, &lvc);

	lvc.iSubItem = 3;
	lvc.cx = 60;
	lvc.pszText = "flags";
	m_lstSections.InsertColumn(3, &lvc);

	lvc.iSubItem = 4;
	lvc.cx = 100;
	lvc.pszText = "addr";
	m_lstSections.InsertColumn(4, &lvc);

	lvc.iSubItem = 5;
	lvc.cx = 100;
	lvc.pszText = "offset";
	m_lstSections.InsertColumn(5, &lvc);

	lvc.iSubItem = 6;
	lvc.cx = 100;
	lvc.pszText = "size";
	m_lstSections.InsertColumn(6, &lvc);

	lvc.iSubItem = 7;
	lvc.cx = 60;
	lvc.pszText = "link";
	m_lstSections.InsertColumn(7, &lvc);

	lvc.iSubItem = 8;
	lvc.cx = 60;
	lvc.pszText = "info";
	m_lstSections.InsertColumn(8, &lvc);

	lvc.iSubItem = 9;
	lvc.cx = 60;
	lvc.pszText = "addralign";
	m_lstSections.InsertColumn(9, &lvc);

	lvc.iSubItem = 10;
	lvc.cx = 60;
	lvc.pszText = "entsize";
	m_lstSections.InsertColumn(10, &lvc);

	m_lstSections.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//CHeaderCtrl* m_head = m_lstSections.GetHeaderCtrl();
	//m_lstSections.m_ctlHeader.SubclassWindow(m_head->GetSafeHwnd());
	//m_lstSections.SetBkColor(RGB(204, 238, 251));
	//m_lstSections.SetTextBkColor(RGB(204, 238, 251));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

int CDialogSectionTab::ShowSections(Elf32_Shdr* secs, CString* strnames, int num)
{
	CString str;
	while (m_lstSections.GetItemCount())
	{
		m_lstSections.DeleteItem(0);
	}

	for (size_t i = 0; i < num; i++)
	{
		str.Format("%d", i+1);
		m_lstSections.InsertItem(i, str);

		str = strnames[i];
		m_lstSections.SetItemText(i, 1, str);

		str.Format("%d", TransferOrder32(secs[i].sh_type));
		m_lstSections.SetItemText(i, 2, str);

		str.Format("%d", TransferOrder32(secs[i].sh_flags));
		m_lstSections.SetItemText(i, 3, str);

		str.Format("0x%08X", TransferOrder32(secs[i].sh_addr));
		m_lstSections.SetItemText(i, 4, str);

		str.Format("0x%08X", TransferOrder32(secs[i].sh_offset));
		m_lstSections.SetItemText(i, 5, str);

		str.Format("0x%08X", TransferOrder32(secs[i].sh_size));
		m_lstSections.SetItemText(i, 6, str);

		str.Format("%d", TransferOrder32(secs[i].sh_link));
		m_lstSections.SetItemText(i, 7, str);

		str.Format("%d", TransferOrder32(secs[i].sh_info));
		m_lstSections.SetItemText(i, 8, str);

		str.Format("%d", TransferOrder32(secs[i].sh_addralign));
		m_lstSections.SetItemText(i, 9, str);

		str.Format("%d", TransferOrder32(secs[i].sh_entsize));
		m_lstSections.SetItemText(i, 10, str);
	}

	return 0;
}

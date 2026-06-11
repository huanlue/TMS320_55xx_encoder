// CDialogHeadInfo.cpp: 实现文件
//

#include "pch.h"
#include "Encoder.h"
#include "CDialogHeadInfo.h"
#include "afxdialogex.h"
#include "resource.h"

// CDialogHeadInfo 对话框

IMPLEMENT_DYNAMIC(CDialogHeadInfo, CMyDialog)

CDialogHeadInfo::CDialogHeadInfo(CWnd* pParent /*=nullptr*/)
	: CMyDialog(IDD_DIALOG_HEAD_INFO, pParent)
	, m_strEdtFileinfoResult(_T(""))
{
	m_bIsInit = false;
}

CDialogHeadInfo::~CDialogHeadInfo()
{
}

void CDialogHeadInfo::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILEINFO_RESULT, m_ctlFileinfoResult);
	DDX_Text(pDX, IDC_EDIT_FILEINFO_RESULT, m_strEdtFileinfoResult);
}


BEGIN_MESSAGE_MAP(CDialogHeadInfo, CMyDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDialogHeadInfo 消息处理程序
void CDialogHeadInfo::AddString(CString str)
{
	UpdateData(TRUE);
	m_strEdtFileinfoResult += str;
	m_strEdtFileinfoResult += "\r\n";

	UpdateData(FALSE);

	m_ctlFileinfoResult.LineScroll(m_ctlFileinfoResult.GetLineCount());

	return;
}


BOOL CDialogHeadInfo::OnInitDialog()
{
	CMyDialog::OnInitDialog();

	if (m_bIsInit == true)
	{
		return TRUE;
	}

	m_bIsInit = true;

	CRect rcttmp;
	GetClientRect(&rcttmp);

	m_ctlFileinfoResult.MoveWindow(&rcttmp);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDialogHeadInfo::OnSize(UINT nType, int cx, int cy)
{
	CMyDialog::OnSize(nType, cx, cy);

	if (m_bIsInit == true)
	{
		CRect rcttmp;
		GetClientRect(&rcttmp);
		//	rcttmp.right = rcttmp.right / 2;

		m_ctlFileinfoResult.MoveWindow(&rcttmp);
	}
}


HBRUSH CDialogHeadInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CMyDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetBkColor(RGB(204, 238, 251));
		pDC->SetTextColor(RGB(255, 0, 0));
		hbr = CreateSolidBrush(RGB(204, 238, 251));
	}

	return hbr;
}


void CDialogHeadInfo::SetString(CString str)
{
	m_strEdtFileinfoResult = str;
	m_strEdtFileinfoResult += "\n";
	UpdateData(FALSE);

	m_ctlFileinfoResult.LineScroll(m_ctlFileinfoResult.GetLineCount());

	return;
}

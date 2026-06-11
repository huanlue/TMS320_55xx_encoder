// CDialogBasicInfo.cpp: 实现文件
//

#include "pch.h"
#include "Encoder.h"
#include "CDialogBasicInfo.h"
#include "afxdialogex.h"


// CDialogBasicInfo 对话框

IMPLEMENT_DYNAMIC(CDialogBasicInfo, CMyDialog)

CDialogBasicInfo::CDialogBasicInfo(CWnd* pParent /*=nullptr*/)
	: CMyDialog(IDD_DIALOG_BASIC_INFO, pParent)
	, m_strEdtBaseinfoResult(_T(""))
{
	m_bIsInit = false;

}

CDialogBasicInfo::~CDialogBasicInfo()
{
}

void CDialogBasicInfo::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BASEINFO_RESULT, m_ctrEdtBaseinfoResult);
	DDX_Text(pDX, IDC_EDIT_BASEINFO_RESULT, m_strEdtBaseinfoResult);
}


BEGIN_MESSAGE_MAP(CDialogBasicInfo, CMyDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDialogBasicInfo 消息处理程序


BOOL CDialogBasicInfo::OnInitDialog()
{
	CMyDialog::OnInitDialog();

	if (m_bIsInit == true)
	{
		return TRUE;
	}

	m_bIsInit = true;

	CRect rcttmp;
	GetClientRect(&rcttmp);

	m_ctrEdtBaseinfoResult.MoveWindow(&rcttmp);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDialogBasicInfo::OnSize(UINT nType, int cx, int cy)
{
	CMyDialog::OnSize(nType, cx, cy);

	if (m_bIsInit == true)
	{
		CRect rcttmp;
		GetClientRect(&rcttmp);
		//	rcttmp.right = rcttmp.right / 2;

		m_ctrEdtBaseinfoResult.MoveWindow(&rcttmp);
	}
}

void CDialogBasicInfo::AddString(CString str)
{
	UpdateData(TRUE);
	m_strEdtBaseinfoResult += str;
	m_strEdtBaseinfoResult += "\r\n";

	UpdateData(FALSE);

	m_ctrEdtBaseinfoResult.LineScroll(m_ctrEdtBaseinfoResult.GetLineCount());

	return;
}

void CDialogBasicInfo::SetString(CString str)
{
	m_strEdtBaseinfoResult = str;
	m_strEdtBaseinfoResult += "\n";
	UpdateData(FALSE);

	m_ctrEdtBaseinfoResult.LineScroll(m_ctrEdtBaseinfoResult.GetLineCount());

	return;
}


HBRUSH CDialogBasicInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


int CDialogBasicInfo::ShowBaseInfo(CStdioFile& file)
{
	CString strTotal = "";
	CString str1;
	CFileStatus rStatus;
	CTime tt;
	char content[256];

	str1 = "文件路径:  \r\n" + file.GetFilePath();

	strTotal = strTotal + str1 + "\r\n\r\n";

	str1.Format("文件大小：\r\n%ld字节", file.GetLength());
	strTotal = strTotal + str1 + "\r\n\r\n";


	if (file.GetStatus(rStatus) == TRUE)
	{
		tt = rStatus.m_ctime;
		sprintf_s(content, "%d-%02d-%02d %02d:%02d:%02d", tt.GetYear(), tt.GetMonth(), \
			tt.GetDay(), tt.GetHour(), tt.GetMinute(), tt.GetSecond());

		str1.Format("文件创建时间： \r\n%s", content);
		strTotal = strTotal + str1 + "\r\n\r\n";

		tt = rStatus.m_mtime;
		sprintf_s(content, "%d-%02d-%02d %02d:%02d:%02d", tt.GetYear(), tt.GetMonth(), \
			tt.GetDay(), tt.GetHour(), tt.GetMinute(), tt.GetSecond());

		str1.Format("最后一次修改时间：\r\n%s", content);
		strTotal = strTotal + str1 + "\r\n\r\n";

		tt = rStatus.m_atime;
		sprintf_s(content, "%d-%02d-%02d %02d:%02d:%02d", tt.GetYear(), tt.GetMonth(), \
			tt.GetDay(), tt.GetHour(), tt.GetMinute(), tt.GetSecond());

		str1.Format("最后一次访问时间： \r\n%s", content);
		strTotal = strTotal + str1 + "\r\n\r\n";

	}

	SetString(strTotal);

	return 0;
}


void CDialogBasicInfo::AppendStatisticsDisplay(int nTotal, int nChanged, double dScramble, int nValid, double dValid)
{
	CString str;

	// 1. 写入加扰统计
	//AddString(_T("--------------------------------------------------"));
	AddString(_T(" "));
	AddString(_T("统计信息："));
	AddString(_T("--------------------------------------------------"));

	str.Format(_T("总指令数：%d"), nTotal);
	AddString(str);
	str.Format(_T("改变的指令数：%d"), nChanged);
	AddString(str);
	//str.Format(_T("加扰率：%.2f%%"), dScramble);
	str.Format(_T("加扰率：    "), dScramble);
	AddString(str);

	AddString(_T("--------------------------------------------------"));
	AddString(_T(""));

	// 2. 写入有效指令统计
	AddString(_T("有效指令统计:"));
	AddString(_T("--------------------------------------------------"));

	str.Format(_T("总指令数：%d"), nTotal);
	AddString(str);
	str.Format(_T("有效指令数：%d"), nValid);
	AddString(str);
	//str.Format(_T("有效指令率：%.2f%%"), dValid);
	str.Format(_T("有效指令率： "), dValid);
	AddString(str);

	AddString(_T("--------------------------------------------------"));
}
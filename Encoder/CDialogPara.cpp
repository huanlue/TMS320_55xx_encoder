// CDialogPara.cpp: 实现文件
//

#include "pch.h"
#include "Encoder.h"
#include "CDialogPara.h"
#include "afxdialogex.h"


// CDialogPara 对话框

IMPLEMENT_DYNAMIC(CDialogPara, CMyDialog)

CDialogPara::CDialogPara(CWnd* pParent /*=nullptr*/)
	: CMyDialog(IDD_DIALOG_PARA, pParent)
{

}

CDialogPara::~CDialogPara()
{
}

void CDialogPara::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_TRANSBIT1, m_cmbTransbit1);
	DDX_Control(pDX, IDC_COMBO_TRANSBIT2, m_cmbTransbit2);
}

BEGIN_MESSAGE_MAP(CDialogPara, CMyDialog)
	ON_BN_CLICKED(IDOK, &CDialogPara::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogPara::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialogPara 消息处理程序

void CDialogPara::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_cmbTransbit1.GetCurSel() == m_cmbTransbit2.GetCurSel())
	{
		MessageBox("两个比特位置不能相同！");
		return;
	}

	m_nTransbit1 = m_cmbTransbit1.GetCurSel();
	m_nTransbit2 = m_cmbTransbit2.GetCurSel();

	OnOK();
}

void CDialogPara::OnBnClickedCancel()
{
	OnCancel();
}


BOOL CDialogPara::OnInitDialog()
{
	CMyDialog::OnInitDialog();

	CString str;
	for (int i = 0; i < 32; i++)
	{
		str.Format("%d", i);
		m_cmbTransbit1.AddString(str);
		m_cmbTransbit2.AddString(str);
	}

	m_cmbTransbit1.SetCurSel(16);
	m_cmbTransbit2.SetCurSel(17);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

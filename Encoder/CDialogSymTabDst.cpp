// CDialogSymTabDst.cpp: 实现文件
//

#include "pch.h"
#include "Encoder.h"
#include "CDialogSymTabDst.h"
#include "afxdialogex.h"


// CDialogSymTabDst 对话框

IMPLEMENT_DYNAMIC(CDialogSymTabDst, CMyDialog)

CDialogSymTabDst::CDialogSymTabDst(CWnd* pParent /*=nullptr*/)
	: CMyDialog(IDD_DIALOG_SYM_TAB_DST, pParent)
{
	m_bIsInit = false;

}

CDialogSymTabDst::~CDialogSymTabDst()
{
}

void CDialogSymTabDst::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SYMS, m_ctlListSyms);
}


BEGIN_MESSAGE_MAP(CDialogSymTabDst, CMyDialog)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SYMS, &CDialogSymTabDst::OnNMCustomdrawListSyms)
END_MESSAGE_MAP()


// CDialogSymTabDst 消息处理程序
// 初始化 - 改为24列
BOOL CDialogSymTabDst::OnInitDialog()
{
	CMyDialog::OnInitDialog();

	if (m_bIsInit == true)
	{
		return TRUE;
	}

	m_bIsInit = true;

	CRect rcttmp;
	GetClientRect(&rcttmp);

	m_ctlListSyms.MoveWindow(&rcttmp);

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_TEXT;

	// 地址列
	lvc.fmt = LVCFMT_LEFT;
	lvc.iSubItem = 0;
	lvc.cx = 80;
	lvc.pszText = "地址";
	m_ctlListSyms.InsertColumn(0, &lvc);

	// 24个数据列
	const char* colHeaders[] = {
		"00", "01", "02", "03", "04", "05", "06", "07",
		"08", "09", "0a", "0b", "0c", "0d", "0e", "0f",
		"10", "11", "12", "13", "14", "15", "16", "17    "
	};

	// 计算列宽
	int nTotalWidth = rcttmp.Width();
	int nAddrColWidth = 78;
	int nRemainingWidth = nTotalWidth - nAddrColWidth;
	int nLastColReserve = 44;
	int nNormalColWidth = (nRemainingWidth - nLastColReserve) / 23;
	if (nNormalColWidth < 22) nNormalColWidth = 30;
	int nLastColWidth = nRemainingWidth - nNormalColWidth * 23;
	if (nLastColWidth < 35) nLastColWidth = 35;

	for (int i = 0; i < 24; i++)
	{
		lvc.iSubItem = i + 1;
		lvc.fmt = LVCFMT_LEFT;
		lvc.cx = (i == 23) ? nLastColWidth : nNormalColWidth;
		lvc.pszText = (LPSTR)colHeaders[i];
		m_ctlListSyms.InsertColumn(i + 1, &lvc);
	}

	m_ctlListSyms.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;
}

// 自适应
void CDialogSymTabDst::OnSize(UINT nType, int cx, int cy)
{
	CMyDialog::OnSize(nType, cx, cy);

	if (m_bIsInit == true)
	{
		CRect rcttmp;
		GetClientRect(&rcttmp);

		m_ctlListSyms.MoveWindow(&rcttmp);

		int nTotalWidth = rcttmp.Width();
		int nAddrColWidth = 78;
		if (nTotalWidth < 500) nAddrColWidth = 60;

		m_ctlListSyms.SetColumnWidth(0, nAddrColWidth);

		int nRemainingWidth = nTotalWidth - nAddrColWidth;
		int nLastColReserve = 43;
		int nNormalColWidth = (nRemainingWidth - nLastColReserve) / 23;
		if (nNormalColWidth < 22) nNormalColWidth = 30;
		int nLastColWidth = nRemainingWidth - nNormalColWidth * 23;
		if (nLastColWidth < 35) nLastColWidth = 35;

		for (int i = 1; i <= 23; i++)
		{
			m_ctlListSyms.SetColumnWidth(i, nNormalColWidth);
		}
		m_ctlListSyms.SetColumnWidth(24, nLastColWidth);
	}
}

// 全局差异矩阵：0-不变（浅蓝底），1-改变（白底红字）
std::vector<std::vector<int>> g_diffMatrix;

int CDialogSymTabDst::ShowInfo(CStdioFile& fileSrc, CStdioFile& fileDst)
{
	if (fileSrc.m_hFile == CFile::hFileNull || fileDst.m_hFile == CFile::hFileNull)
	{
		return -1;
	}

	// 清空旧数据
	while (m_ctlListSyms.GetItemCount())
	{
		m_ctlListSyms.DeleteItem(0);
	}
	g_diffMatrix.clear();

	ULONGLONG lenSrc = fileSrc.GetLength();
	ULONGLONG lenDst = fileDst.GetLength();

	if (lenSrc == 0 && lenDst == 0)
	{
		return -1;
	}

	char* pTextBufSrc = new char[(size_t)lenSrc + 1]{ 0 };
	char* pTextBufDst = new char[(size_t)lenDst + 1]{ 0 };

	if (!pTextBufSrc || !pTextBufDst)
	{
		MessageBox("分配内存失败！");
		if (pTextBufSrc) delete[] pTextBufSrc;
		if (pTextBufDst) delete[] pTextBufDst;
		return -1;
	}

	fileSrc.SeekToBegin();
	fileDst.SeekToBegin();
	fileSrc.Read(pTextBufSrc, (UINT)lenSrc);
	fileDst.Read(pTextBufDst, (UINT)lenDst);

	CString strTextSrc(pTextBufSrc);
	CString strTextDst(pTextBufDst);

	delete[] pTextBufSrc;
	delete[] pTextBufDst;

	// 解析十六进制文本，遇到ETX(0x03)停止
	auto ParseHexText = [](const CString& strText, std::vector<std::vector<unsigned char>>& rowsData) -> int
	{
		rowsData.clear();
		std::vector<unsigned char> currentRow;
		bool bRowHasData = false;
		bool bStopParsing = false;

		int nPos = 0;
		int nTextLength = strText.GetLength();

		while (nPos < nTextLength && !bStopParsing)
		{
			unsigned char ch = (unsigned char)strText[nPos];

			// 遇到 ETX(0x03) 立即停止
			if (ch == 0x03)
			{
				if (bRowHasData && !currentRow.empty())
				{
					rowsData.push_back(currentRow);
					currentRow.clear();
					bRowHasData = false;
				}
				bStopParsing = true;
				break;
			}

			// 过滤控制字符
			if (ch == 0x02 || ch == 0x00 || ch == 0x1A || ch == 0xFF)
			{
				nPos++;
				continue;
			}

			if (strText[nPos] == '\r' || strText[nPos] == '\n')
			{
				if (bRowHasData && !currentRow.empty())
				{
					rowsData.push_back(currentRow);
					currentRow.clear();
					bRowHasData = false;
				}
				if (strText[nPos] == '\r' && nPos + 1 < nTextLength && strText[nPos + 1] == '\n')
					nPos += 2;
				else
					nPos++;
				continue;
			}

			if (strText[nPos] == ' ' || strText[nPos] == '\t')
			{
				nPos++;
				continue;
			}

			if (!((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f')))
			{
				nPos++;
				continue;
			}

			int nStart = nPos;
			while (nPos < nTextLength &&
				strText[nPos] != ' ' && strText[nPos] != '\t' &&
				strText[nPos] != '\r' && strText[nPos] != '\n')
			{
				if ((unsigned char)strText[nPos] == 0x03)
					break;
				nPos++;
			}

			CString strHex = strText.Mid(nStart, nPos - nStart);
			strHex.Trim();

			if (strHex.GetLength() >= 1)
			{
				char* pEnd = NULL;
				unsigned char byteValue = (unsigned char)strtoul(strHex, &pEnd, 16);
				if (pEnd != NULL && (*pEnd == '\0' || *pEnd == ' ' || *pEnd == '\r' || *pEnd == '\n'))
				{
					currentRow.push_back(byteValue);
					bRowHasData = true;
				}
			}
		}

		if (!bStopParsing && bRowHasData && !currentRow.empty())
		{
			rowsData.push_back(currentRow);
		}

		return (int)rowsData.size();
	};

	std::vector<std::vector<unsigned char>> rowsSrcData;
	std::vector<std::vector<unsigned char>> rowsDstData;

	int nSrcRows = ParseHexText(strTextSrc, rowsSrcData);
	int nDstRows = ParseHexText(strTextDst, rowsDstData);

	// 使用较小的行数
	int maxRows = min(nSrcRows, nDstRows);

	if (maxRows == 0) return 0;

	g_diffMatrix.resize(maxRows, std::vector<int>(25, 0));

	CString str;
	int nDisplayRow = 0;

	for (int rowIdx = 0; rowIdx < maxRows; rowIdx++)
	{
		const auto& srcRow = (rowIdx < nSrcRows) ? rowsSrcData[rowIdx] : std::vector<unsigned char>();
		const auto& dstRow = (rowIdx < nDstRows) ? rowsDstData[rowIdx] : std::vector<unsigned char>();

		int maxBytes = max((int)srcRow.size(), (int)dstRow.size());
		maxBytes = min(maxBytes, 24);

		int rowStartOffset = nDisplayRow * 24;

		str.Format("%08XH", rowStartOffset);
		m_ctlListSyms.InsertItem(nDisplayRow, str);

		for (int byteIdx = 0; byteIdx < maxBytes; byteIdx++)
		{
			int nCol = byteIdx + 1;

			unsigned char srcByte = (byteIdx < (int)srcRow.size()) ? srcRow[byteIdx] : 0xFF;
			unsigned char dstByte = (byteIdx < (int)dstRow.size()) ? dstRow[byteIdx] : 0xFF;

			if (byteIdx < (int)dstRow.size())
				str.Format("%02X", dstByte);
			else
				str = "";

			m_ctlListSyms.SetItemText(nDisplayRow, nCol, str);

			if (byteIdx < (int)srcRow.size() && byteIdx < (int)dstRow.size())
			{
				g_diffMatrix[nDisplayRow][nCol] = (srcByte != dstByte) ? 1 : 0;
			}
			else
			{
				g_diffMatrix[nDisplayRow][nCol] = 1;
			}
		}

		for (int j = maxBytes; j < 24; j++)
		{
			m_ctlListSyms.SetItemText(nDisplayRow, j + 1, "");
			g_diffMatrix[nDisplayRow][j + 1] = 0;
		}

		nDisplayRow++;
	}

	m_ctlListSyms.Invalidate();
	return 0;
}

void CDialogSymTabDst::OnNMCustomdrawListSyms(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	*pResult = CDRF_DODEFAULT;

	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		int nRow = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		int nCol = pLVCD->iSubItem;

		COLORREF clrOriginalHtmlBlue = m_ctlListSyms.GetBkColor();
		COLORREF clrOriginalText = GetSysColor(COLOR_WINDOWTEXT);

		if (nRow >= 0 && nRow < (int)g_diffMatrix.size() && nCol >= 0 && nCol < 25)
		{
			if (nCol > 0)
			{
				int nStatus = g_diffMatrix[nRow][nCol];

				if (nStatus == 1)
				{
					// 发生改变的字节 -> 白底 + 红字
					//pLVCD->clrTextBk = RGB(255, 255, 255);
					pLVCD->clrText = RGB(255, 0, 0);
				}
				else
				{
					// 未改变的字节 -> 浅蓝底 + 黑字
					pLVCD->clrTextBk = clrOriginalHtmlBlue;
					pLVCD->clrText = clrOriginalText;
				}
			}
			else
			{
				pLVCD->clrTextBk = clrOriginalHtmlBlue;
				pLVCD->clrText = clrOriginalText;
			}
		}

		*pResult = CDRF_NEWFONT;
	}
}
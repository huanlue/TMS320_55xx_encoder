// CDialogSymTab.cpp: 实现文件
//

#include "pch.h"
#include "Encoder.h"
#include "CDialogSymTab.h"
#include "afxdialogex.h"
#include "CDialogBasicInfo.h" 
#include "MainFrm.h"
#include <fstream>
#include <iostream>  
using namespace std;

struct LineInfo {
	std::vector<int> lineLengths;  // 记录每行的字节数
};

// 全局变量，用于在转换函数间传递行结构信息
LineInfo g_lineInfo;

std::vector<InstructionRule> g_instructionSetRules;// 用 vector 代替普通的 map 装入所有的opcode

// CDialogSymTab 对话框

CDialogSymTab::CDialogSymTab(CWnd* pParent /*=nullptr*/)
	: CMyDialog(IDD_DIALOG_SYM_TAB, pParent)
{
	m_bIsInit = false;
}

CDialogSymTab::~CDialogSymTab()
{
}

void CDialogSymTab::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SYMS, m_ctlListSyms);
	DDX_Control(pDX, IDC_BUTTON_MODIFY, m_btnModify);
	// 不需要 DDX_Control 绑定动态创建的控件
}

// 界面初始化:构成标准的十六进制编辑器网格

BOOL CDialogSymTab::OnInitDialog()
{
	CMyDialog::OnInitDialog();

	if (m_bIsInit == true)
	{
		return TRUE;
	}

	m_bIsInit = true;

	// 初始化偏移量
	m_nOffset = 0;

	CRect rcttmp;
	GetClientRect(&rcttmp);

	// 列表占上方82%空间（给两行控件留更多空间）
	CRect rctList = rcttmp;
	rctList.bottom = (int)(rcttmp.Height() * 0.82);
	m_ctlListSyms.MoveWindow(&rctList);

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
		"10", "11", "12", "13", "14", "15", "16", "17   "
	};

	int nTotalWidth = rctList.Width();
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

	// ==========================================
	// 底部两行居中布局
	// ==========================================
	int nBottomAreaTop = rctList.bottom;
	int nBottomHeight = rcttmp.Height() - nBottomAreaTop;

	// 第一行和第二行的Y坐标
	int nRow1Y = nBottomAreaTop + nBottomHeight / 3;       // 第一行在上1/3处
	int nRow2Y = nBottomAreaTop + nBottomHeight * 2 / 3;   // 第二行在下1/3处

	// 控件尺寸
	int nLabelWidth = 55;
	int nEditWidth = 65;
	int nControlHeight = 23;
	int nGap = 6;

	// ===== 第一行：偏移量标签 + 输入框 =====
	int nRow1TotalWidth = nLabelWidth + nGap + nEditWidth;
	int nRow1StartX = (rcttmp.Width() - nRow1TotalWidth) / 2;

	// 创建偏移量标签
	if (!m_staticOffset.GetSafeHwnd())
	{
		CRect rctLabel(nRow1StartX, nRow1Y - nControlHeight / 2,
			nRow1StartX + nLabelWidth, nRow1Y + nControlHeight / 2);

		m_staticOffset.Create(_T("偏移量:"),
			WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,
			rctLabel, this, 2000);
		m_staticOffset.SetFont(GetFont());
	}

	// 创建偏移量输入框
	if (!m_editOffset.GetSafeHwnd())
	{
		int nEditX = nRow1StartX + nLabelWidth + nGap;
		CRect rctEdit(nEditX, nRow1Y - 22 / 2,
			nEditX + nEditWidth, nRow1Y + 22 / 2);

		m_editOffset.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,
			rctEdit, this, 2001);
		m_editOffset.SetFont(GetFont());
		m_editOffset.SetWindowText(_T("0"));
		m_editOffset.SetLimitText(8);
	}

	// ===== 第二行：加扰按钮 =====
	if (m_btnModify.GetSafeHwnd())
	{
		CRect rctBtn;
		m_btnModify.GetWindowRect(&rctBtn);
		ScreenToClient(&rctBtn);

		int nBtnWidth = rctBtn.Width();
		int nBtnHeight = rctBtn.Height();
		int nBtnX = (rcttmp.Width() - nBtnWidth) / 2;

		m_btnModify.MoveWindow(nBtnX, nRow2Y - nBtnHeight / 2,
			nBtnWidth, nBtnHeight);
	}

	return TRUE;
}

//界面自适应:当用户拉伸或缩放窗口时，动态调整列表控件和“修改”按钮（m_btnModify）的位置，确保列表占据上方 90% 的空间，按钮在下方居中
//界面自适应:当用户拉伸或缩放窗口时，动态调整列表控件和"修改"按钮的位置
void CDialogSymTab::OnSize(UINT nType, int cx, int cy)
{
	CMyDialog::OnSize(nType, cx, cy);

	if (m_bIsInit == true)
	{
		CRect rcttmp;
		GetClientRect(&rcttmp);

		// 列表占上方88%空间
		CRect rctList = rcttmp;
		rctList.bottom = (int)(rcttmp.Height() * 0.88);
		m_ctlListSyms.MoveWindow(&rctList);

		// 自适应列宽
		int nTotalWidth = rctList.Width();
		int nAddrColWidth = 78;
		if (nTotalWidth < 500) nAddrColWidth = 60;

		m_ctlListSyms.SetColumnWidth(0, nAddrColWidth);

		int nRemainingWidth = nTotalWidth - nAddrColWidth;
		int nLastColReserve = 44;
		int nNormalColWidth = (nRemainingWidth - nLastColReserve) / 23;
		if (nNormalColWidth < 22) nNormalColWidth = 30;
		int nLastColWidth = nRemainingWidth - nNormalColWidth * 23;
		if (nLastColWidth < 35) nLastColWidth = 35;

		for (int i = 1; i <= 23; i++)
			m_ctlListSyms.SetColumnWidth(i, nNormalColWidth);
		m_ctlListSyms.SetColumnWidth(24, nLastColWidth);

		// ==========================================
		// 底部两行居中布局
		// ==========================================
		int nBottomAreaTop = rctList.bottom;
		int nBottomHeight = rcttmp.Height() - nBottomAreaTop;

		int nRow1Y = nBottomAreaTop + nBottomHeight / 3;
		int nRow2Y = nBottomAreaTop + nBottomHeight * 2 / 3;

		int nLabelWidth = 55;
		int nEditWidth = 65;
		int nControlHeight = 23;
		int nGap = 6;

		// ===== 第一行：偏移量标签 + 输入框 =====
		int nRow1TotalWidth = nLabelWidth + nGap + nEditWidth;
		int nRow1StartX = (rcttmp.Width() - nRow1TotalWidth) / 2;

		if (m_staticOffset.GetSafeHwnd())
		{
			m_staticOffset.MoveWindow(nRow1StartX, nRow1Y - nControlHeight / 2,
				nLabelWidth, nControlHeight);
		}

		if (m_editOffset.GetSafeHwnd())
		{
			int nEditX = nRow1StartX + nLabelWidth + nGap;
			m_editOffset.MoveWindow(nEditX, nRow1Y - 22 / 2,
				nEditWidth, 22);
		}

		// ===== 第二行：加扰按钮 =====
		if (m_btnModify.GetSafeHwnd())
		{
			CRect rctBtn;
			m_btnModify.GetWindowRect(&rctBtn);
			ScreenToClient(&rctBtn);

			int nBtnWidth = rctBtn.Width();
			int nBtnHeight = rctBtn.Height();
			int nBtnX = (rcttmp.Width() - nBtnWidth) / 2;

			m_btnModify.MoveWindow(nBtnX, nRow2Y - nBtnHeight / 2,
				nBtnWidth, nBtnHeight);
		}
	}
}

int CDialogSymTab::ShowInfo(CStdioFile& file)
{
	if (file.m_hFile == CFile::hFileNull)
	{
		return -1;
	}

	while (m_ctlListSyms.GetItemCount())
	{
		m_ctlListSyms.DeleteItem(0);
	}

	CString str;
	ULONGLONG nFileLength = file.GetLength();
	if (nFileLength == 0) return -1;

	char* pTextBuffer = new char[(size_t)nFileLength + 1];
	memset(pTextBuffer, 0, (size_t)nFileLength + 1);
	file.SeekToBegin();
	file.Read(pTextBuffer, (UINT)nFileLength);

	// ★ 查找 STX 和 ETX 的位置
	int startPos = 0;
	int endPos = (int)nFileLength;

	for (int i = 0; i < (int)nFileLength; i++)
	{
		if ((unsigned char)pTextBuffer[i] == 0x02)
		{
			startPos = i + 1;
		}
		if ((unsigned char)pTextBuffer[i] == 0x03)
		{
			endPos = i;
			break;
		}
	}

	std::vector<std::vector<unsigned char>> rowsData;
	std::vector<unsigned char> currentRow;
	bool bRowHasData = false;

	int nPos = startPos;
	while (nPos < endPos)
	{
		unsigned char ch = (unsigned char)pTextBuffer[nPos];

		if (ch == ' ' || ch == '\r' || ch == '\t')
		{
			nPos++;
			continue;
		}

		if (ch == '\n')
		{
			if (bRowHasData && !currentRow.empty())
			{
				rowsData.push_back(currentRow);
				currentRow.clear();
				bRowHasData = false;
			}
			nPos++;
			continue;
		}

		if (!((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f')))
		{
			nPos++;
			continue;
		}

		// 提取十六进制字符串（与之前相同）
		CString strHex;
		int nStart = nPos;
		while (nPos < endPos &&
			pTextBuffer[nPos] != ' ' && pTextBuffer[nPos] != '\t' &&
			pTextBuffer[nPos] != '\r' && pTextBuffer[nPos] != '\n')
		{
			nPos++;
		}

		strHex = CString(&pTextBuffer[nStart], nPos - nStart);
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

	if (bRowHasData && !currentRow.empty())
	{
		rowsData.push_back(currentRow);
	}

	// 显示数据（与之前相同）
	int nDisplayRow = 0;
	for (size_t rowIdx = 0; rowIdx < rowsData.size(); rowIdx++)
	{
		const auto& rowBytes = rowsData[rowIdx];
		int nBytesToShow = (int)rowBytes.size();

		str.Format("%08XH", nDisplayRow * 24);
		m_ctlListSyms.InsertItem(nDisplayRow, str);

		for (int j = 0; j < 24; j++)
		{
			if (j < nBytesToShow)
				str.Format("%02X", rowBytes[j]);
			else
				str = "";
			m_ctlListSyms.SetItemText(nDisplayRow, j + 1, str);
		}
		nDisplayRow++;
	}

	delete[] pTextBuffer;
	return 0;
}

void CDialogSymTab::OnEnChangeEditOffset()
{
	CString strOffset;
	m_editOffset.GetWindowText(strOffset);

	if (strOffset.IsEmpty())
		m_nOffset = 0;
	else
		m_nOffset = _ttoi(strOffset);
}

//  !!! 核心逻辑实现  修改算法请使用此处 
// 这个函数用于处理4字节的完整命令
/*
bool CDialogSymTab::ProcessBinaryCrypto(const CString& strInputPath, const CString& strOutputPath)
{
	FILE* fi = nullptr;
	FILE* fo = nullptr;

	// 以二进制只读模式打开源文件
	if (fopen_s(&fi, strInputPath, "rb") != 0 || fi == nullptr)
	{
		return false;
	}

	// 以二进制只写模式打开目标文件
	if (fopen_s(&fo, strOutputPath, "wb") != 0 || fo == nullptr)
	{
		fclose(fi);
		return false;
	}

	int c = 0;

	// 循环按 4 字节块处理文件，直至文件末尾 (EOF)
	while (true)
	{
		// ----------------------------------------
		// 1. 处理每块的第 1 个字节：交换第 6 位和第 7 位
		// ----------------------------------------
		c = fgetc(fi);
		if (c == EOF) break;

		// 0x3F = 00111111 (保留0~5位)
		// 0x80 = 10000000 (第7位右移1位到第6位)
		// 0x40 = 01000000 (第6位左移1位到第7位)
		c = (c & 0x3F) | ((c & 0x80) >> 1) | ((c & 0x40) << 1);
		fputc(c, fo);

		// ----------------------------------------
		// 2. 处理每块的第 2 个字节：对第 1, 2, 3 位进行左循环移位
		// ----------------------------------------
		c = fgetc(fi);
		if (c == EOF) break;

		// 0xF1 = 11110001 (保留 7,6,5,4 和 0 位)
		// ((c >> 3) & 0x01) << 1 : 把原来的第3位移到第1位
		// (c & 0x06) << 1        : 把原来的第1、2位左移到第2、3位
		c = (c & 0xF1) | (((c >> 3) & 0x01) << 1) | ((c & 0x06) << 1);
		fputc(c, fo);

		// ----------------------------------------
		// 3. 处理每块的第 3、4 个字节：不作处理，直接复制
		// ----------------------------------------
		bool bEndOfFile = false;
		for (int m = 0; m < 2; m++)
		{
			c = fgetc(fi);
			if (c == EOF)
			{
				bEndOfFile = true;
				break;
			}
			fputc(c, fo);
		}

		if (bEndOfFile) break;
	}

	// 关闭文件句柄，释放资源
	fclose(fi);
	fclose(fo);
	return true;
}
*/

bool CDialogSymTab::ProcessBinaryCrypto(const CString& strInputPath, const CString& strOutputPath, int nOffset)
{
	FILE* fi = nullptr;
	FILE* fo = nullptr;

	if (fopen_s(&fi, strInputPath, "rb") != 0 || fi == nullptr)
		return false;

	if (fopen_s(&fo, strOutputPath, "wb") != 0 || fo == nullptr)
	{
		fclose(fi);
		return false;
	}

	int c = 0;
	int nByteIndex = 0;

	while ((c = fgetc(fi)) != EOF)
	{
		if (nByteIndex >= nOffset)
		{
			// 一行完成第0位和第1位交换
			c = (c & 0xFC) |                    // 保留第7-2位
				((c & 0x01) << 1) |            // 原第0位 -> 第1位
				((c >> 1) & 0x01);             // 原第1位 -> 第0位
		}

		fputc(c, fo);
		nByteIndex++;
	}

	fclose(fi);
	fclose(fo);
	return true;
}


// 将文本格式的十六进制文件转换为二进制文件
bool CDialogSymTab::ConvertHexTextToBinary(const CString& strInputPath, const CString& strOutputPath)
{
	FILE* fi = nullptr;
	FILE* fo = nullptr;

	if (fopen_s(&fi, strInputPath, "rb") != 0 || fi == nullptr)
	{
		return false;
	}

	if (fopen_s(&fo, strOutputPath, "wb") != 0 || fo == nullptr)
	{
		fclose(fi);
		return false;
	}

	g_lineInfo.lineLengths.clear();

	fseek(fi, 0, SEEK_END);
	long fileSize = ftell(fi);
	fseek(fi, 0, SEEK_SET);

	unsigned char* fileBuffer = new unsigned char[fileSize + 1];
	fread(fileBuffer, 1, fileSize, fi);
	fileBuffer[fileSize] = '\0';
	fclose(fi);

	// ★ 关键：查找 STX(0x02) 和 ETX(0x03) 的位置
	long startPos = 0;  // 有效数据开始位置
	long endPos = fileSize;  // 有效数据结束位置

	for (long i = 0; i < fileSize; i++)
	{
		if (fileBuffer[i] == 0x02)
		{
			startPos = i + 1;  // STX之后开始
		}
		if (fileBuffer[i] == 0x03)
		{
			endPos = i;  // ETX之前结束
			break;
		}
	}

	TRACE("文件大小=%ld, STX位置=%ld, ETX位置=%ld, 有效范围=[%ld, %ld)\n",
		fileSize, startPos > 0 ? startPos - 1 : -1, endPos, startPos, endPos);

	int currentLineBytes = 0;
	bool bLineHasData = false;
	int totalBytesWritten = 0;

	// ★ 只在有效范围内解析
	int pos = startPos;
	while (pos < endPos)
	{
		unsigned char ch = fileBuffer[pos];

		// 跳过空格、回车、制表符
		if (ch == ' ' || ch == '\r' || ch == '\t')
		{
			pos++;
			continue;
		}

		// 遇到换行符
		if (ch == '\n')
		{
			if (bLineHasData && currentLineBytes > 0)
			{
				g_lineInfo.lineLengths.push_back(currentLineBytes);
				currentLineBytes = 0;
				bLineHasData = false;
			}
			pos++;
			continue;
		}

		// 检查是否是有效的十六进制字符
		if (!((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f')))
		{
			pos++;
			continue;
		}

		// 处理十六进制字符
		char hexStr[3] = { 0 };
		hexStr[0] = (char)ch;
		pos++;

		if (pos >= endPos)
		{
			hexStr[1] = '\0';
			unsigned char byteValue = (unsigned char)strtoul(hexStr, NULL, 16);
			fputc(byteValue, fo);
			currentLineBytes++;
			bLineHasData = true;
			totalBytesWritten++;
			break;
		}

		unsigned char nextCh = fileBuffer[pos];

		if ((nextCh >= '0' && nextCh <= '9') ||
			(nextCh >= 'A' && nextCh <= 'F') ||
			(nextCh >= 'a' && nextCh <= 'f'))
		{
			hexStr[1] = (char)nextCh;
			hexStr[2] = '\0';
			unsigned char byteValue = (unsigned char)strtoul(hexStr, NULL, 16);
			fputc(byteValue, fo);
			currentLineBytes++;
			bLineHasData = true;
			totalBytesWritten++;
			pos++;
		}
		else if (nextCh == ' ' || nextCh == '\r' || nextCh == '\n' || nextCh == '\t')
		{
			hexStr[1] = '\0';
			unsigned char byteValue = (unsigned char)strtoul(hexStr, NULL, 16);
			fputc(byteValue, fo);
			currentLineBytes++;
			bLineHasData = true;
			totalBytesWritten++;

			if (nextCh == '\n')
			{
				g_lineInfo.lineLengths.push_back(currentLineBytes);
				currentLineBytes = 0;
				bLineHasData = false;
			}
			pos++;
		}
		else
		{
			hexStr[1] = '\0';
			unsigned char byteValue = (unsigned char)strtoul(hexStr, NULL, 16);
			fputc(byteValue, fo);
			currentLineBytes++;
			bLineHasData = true;
			totalBytesWritten++;
		}
	}

	// 记录最后一行
	if (bLineHasData && currentLineBytes > 0)
	{
		g_lineInfo.lineLengths.push_back(currentLineBytes);
	}

	delete[] fileBuffer;
	fclose(fo);

	TRACE("总写入字节=%d, 总行数=%zu\n", totalBytesWritten, g_lineInfo.lineLengths.size());
	return true;
}

// 将二进制文件转换为文本格式的十六进制文件

// 将二进制文件转换为文本格式的十六进制文件
bool CDialogSymTab::ConvertBinaryToHexText(const CString& strInputPath, const CString& strOutputPath)
{
	FILE* fi = nullptr;
	FILE* fo = nullptr;

	// 以二进制模式打开源文件
	if (fopen_s(&fi, strInputPath, "rb") != 0 || fi == nullptr)
	{
		return false;
	}

	// 以二进制模式打开输出文件（使用 "wb" 以支持写入控制字符）
	if (fopen_s(&fo, strOutputPath, "wb") != 0 || fo == nullptr)
	{
		fclose(fi);
		return false;
	}

	// ★ 写入 STX (0x02) 作为文件开始标记
	fputc(0x02, fo);
	fprintf(fo, "\n");  // STX 后换行

	int globalByteCount = 0;  // 全局字节计数
	int c;
	size_t lineIndex = 0;     // 当前处理到第几行
	int bytesInCurrentLine = 0;  // 当前行已输出的字节数

	while ((c = fgetc(fi)) != EOF)
	{
		// 检查是否需要换行
		if (lineIndex < g_lineInfo.lineLengths.size())
		{
			if (bytesInCurrentLine >= g_lineInfo.lineLengths[lineIndex])
			{
				// 当前行已满，换到下一行
				fprintf(fo, "\n");
				lineIndex++;
				bytesInCurrentLine = 0;

				// 跳过空行
				while (lineIndex < g_lineInfo.lineLengths.size() &&
					g_lineInfo.lineLengths[lineIndex] == 0)
				{
					fprintf(fo, "\n");
					lineIndex++;
				}
			}
		}

		// 在同一行内，字节之间加空格
		if (bytesInCurrentLine > 0)
		{
			fprintf(fo, " ");
		}

		fprintf(fo, "%02X", (unsigned char)c);
		bytesInCurrentLine++;
		globalByteCount++;
	}

	// 处理剩余的空行
	while (lineIndex < g_lineInfo.lineLengths.size())
	{
		if (g_lineInfo.lineLengths[lineIndex] == 0)
		{
			fprintf(fo, "\n");
		}
		lineIndex++;
	}

	// 确保最后一个数据行后有换行符
	if (bytesInCurrentLine > 0)
	{
		fprintf(fo, "\n");
	}

	// ★ 写入 ETX (0x03) 作为文件结束标记
	fputc(0x03, fo);
	fprintf(fo, "\n");  // ETX 后换行

	fclose(fi);
	fclose(fo);

	TRACE("ConvertBinaryToHexText: 输出%d字节, STX+ETX已添加\n", globalByteCount);
	return true;
}


// 统计指令变化效果, opcode修改率, opcode 有效率(在文件的基础上,会根据打开文件不同而动态变化)
void CDialogSymTab::CalculateAndDisplayStatistics(const CString& strInputPath, const CString& strOutputPath)
{
	FILE* fi = nullptr;
	FILE* fo = nullptr;

	// 导入助记符
	LoadInstructionSet(_T("..//Encoder\\Mnemonic-6678.txt"));

	// 1. 安全检查：以只读二进制模式打开两个文件
	if (fopen_s(&fi, strInputPath, "rb") != 0 || fi == nullptr) return;
	if (fopen_s(&fo, strOutputPath, "rb") != 0 || fo == nullptr)
	{
		fclose(fi);
		return;
	}

	int nTotalInstructions = 0;
	int nChangedInstructions = 0;
	int nValidInstructions = 0;

	// 4 字节（unsigned int）代表一条指令
	unsigned int cmdSrc = 0;
	unsigned int cmdDst = 0;

	// 2.核心流式比对：改为绝对安全的 4 字节块同步读取
	while (true)
	{
		cmdSrc = 0; // 每次读取前清零，防止文件末尾不足 4 字节时残留上一次的数据
		cmdDst = 0;

		size_t readSrc = fread(&cmdSrc, sizeof(unsigned int), 1, fi);
		size_t readDst = fread(&cmdDst, sizeof(unsigned int), 1, fo);

		// 如果两个文件都读取完毕，则退出比对
		if (readSrc == 0 && readDst == 0)
		{
			break;
		}

		// 只要有任意一个文件读到了数据，总指令数就加 1
		nTotalInstructions++;

		// 判定指令是否改变：
		// 1. 两个文件读取结果不一致（比如一个文件长，一个文件短）
		// 2. 两个文件都读到了 4 字节，但内容不相等
		if (readSrc != readDst || cmdSrc != cmdDst)
		{
			nChangedInstructions++;
		}

		// 检查修改后的指令是否为有效指令
		if (IsInstructionValid(cmdDst))
		{
			nValidInstructions++;
		}
	}

	// 彻底释放文件句柄
	fclose(fi);
	fclose(fo);

	// 计算百分比
	double dScrambleRate = 0.0;
	double dValidRate = 0.0;
	if (nTotalInstructions > 0)
	{
		dScrambleRate = ((double)nChangedInstructions / nTotalInstructions) * 100.0;
		dValidRate = ((double)nValidInstructions / nTotalInstructions) * 100.0;
	}

	// 跨窗口联动：推送到侧边栏
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if (pMain && pMain->m_pViewFileInfo && pMain->m_pViewFileInfo->m_pDialogBasicInfo)
	{
		// 体验优化：调用原有的 ShowBaseInfo 刷一下路径和时间
		CStdioFile fileOriginal;
		if (fileOriginal.Open(strInputPath, CFile::typeBinary | CFile::modeRead))
		{
			pMain->m_pViewFileInfo->m_pDialogBasicInfo->ShowBaseInfo(fileOriginal);
			fileOriginal.Close();
		}

		// 调用之前在 BasicInfo 里写好的渲染函数
		/*pMain->m_pViewFileInfo->m_pDialogBasicInfo->AppendStatisticsDisplay(
			nTotalInstructions,
			nChangedInstructions,
			dScrambleRate,
			nValidInstructions,
			dValidRate
		);*/
	}
}

// 调用流程,调用诸多函数
void CDialogSymTab::OnBnClickedButtonModify()
{
	// 1. 获取主框架指针以拿到当前操作的文件路径
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if (!pMain) return;

	// 2. 读取偏移量
	CString strOffset;
	if (m_editOffset.GetSafeHwnd())
	{
		m_editOffset.GetWindowText(strOffset);
	}
	int nOffset = strOffset.IsEmpty() ? 0 : _ttoi(strOffset);

	TRACE("加密偏移量: %d 字节\n", nOffset);

	// 3. 构建临时二进制文件路径和目标输出文件路径
	CString strTempBinFile = pMain->m_strFilepath + _T("\\temp_binary.bin");
	CString strResultFile = pMain->m_strFilepath + _T("\\result.bin");
	
	pMain->m_strResultFile = strResultFile;

	// 4. 将文本格式的十六进制文件转换为二进制文件
	if (!ConvertHexTextToBinary(pMain->m_strFilepathName, strTempBinFile))
	{
		MessageBox(_T("转换文本格式失败！"), _T("处理失败"), MB_ICONERROR);
		return;
	}

	// 5. 调用加密处理函数（传入偏移量）
	bool bSuccess = ProcessBinaryCrypto(strTempBinFile, strResultFile, nOffset);

	// 6. 删除临时二进制文件
	DeleteFile(strTempBinFile);

	if (!bSuccess)
	{
		MessageBox(_T("文件加密处理失败，请检查文件路径是否正确。"), _T("处理失败"), MB_ICONERROR);
		DeleteFile(strResultFile);
		return;
	}

	// 7. 截断result.bin到正确的大小（根据行结构记录）
	{
		int totalExpectedBytes = 0;
		for (size_t i = 0; i < g_lineInfo.lineLengths.size(); i++)
		{
			totalExpectedBytes += g_lineInfo.lineLengths[i];
		}

		if (totalExpectedBytes > 0)
		{
			HANDLE hFile = CreateFile(strResultFile, GENERIC_WRITE,
				0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				DWORD currentSize = GetFileSize(hFile, NULL);
				if (currentSize > (DWORD)totalExpectedBytes)
				{
					SetFilePointer(hFile, totalExpectedBytes, NULL, FILE_BEGIN);
					SetEndOfFile(hFile);
					TRACE("截断result.bin: %lu -> %d 字节\n", currentSize, totalExpectedBytes);
				}
				CloseHandle(hFile);
			}
		}
	}

	// 8. 将result.bin转换为文本格式
	CString strResultTextFile = pMain->m_strFilepath + _T("\\result_text.bin");
	if (!ConvertBinaryToHexText(strResultFile, strResultTextFile))
	{
		MessageBox(_T("转换结果文件格式失败！"), _T("处理失败"), MB_ICONERROR);
		DeleteFile(strResultFile);
		return;
	}

	// 更新结果文件路径为文本格式文件
	pMain->m_strResultFile = strResultTextFile;

	// 9. 统计指令变化效果
	CalculateAndDisplayStatistics(pMain->m_strFilepathName, strResultTextFile);

	// 10. 同时打开源文件和目标文件，传入 Dst 窗口进行颜色对比显示
	CStdioFile fileSrc;
	CStdioFile fileDst;

	if (fileSrc.Open(pMain->m_strFilepathName, CFile::typeText | CFile::modeRead) &&
		fileDst.Open(strResultTextFile, CFile::typeText | CFile::modeRead))
	{
		// 调用目标窗口的 ShowInfo 函数进行对比显示
		if (pMain->m_pViewFileDstFile && pMain->m_pViewFileDstFile->m_pDialogSymTabDst)
		{
			pMain->m_pViewFileDstFile->m_pDialogSymTabDst->ShowInfo(fileSrc, fileDst);
		}

		fileSrc.Close();
		fileDst.Close();
	}
	else
	{
		if (fileSrc.m_hFile != CFile::hFileNull) fileSrc.Close();
		if (fileDst.m_hFile != CFile::hFileNull) fileDst.Close();

		MessageBox(_T("无法读取源文件或生成的 result 文件，无法进行联动对比。"), _T("提示"), MB_ICONWARNING);
	}

	// 11. 删除临时 result.bin 文件
	DeleteFile(strResultFile);

	// 12. 提示处理完成

	//cstring strmsg;
	//strmsg.format(_t("文件加密处理完成！\r\n偏移量: %d 字节\r\n结果文件: %s"),
	//	noffset, strresulttextfile);
	//messagebox(strmsg, _t("处理成功"), mb_iconinformation);
}

/*
	用于统计指令有效率的两个函数, 在.bin文件的基础上进行修改
*/
bool CDialogSymTab::LoadInstructionSet(const CString& strFilePath)
{
	CStringA strPathAnsi(strFilePath);
	std::ifstream file(strPathAnsi.GetString());

	if (!file.is_open())
	{
		AfxMessageBox(_T("无法打开指令集配置文件！"));
		return false;
	}

	g_instructionSetRules.clear();
	std::string opcodeStr;
	std::string mnemonic;

	while (file >> opcodeStr >> mnemonic)
	{
		// 如果读出来的不是合法的特征码样式，或者是空行干扰，直接跳过
		if (opcodeStr.empty() || (opcodeStr[0] != '0' && opcodeStr[0] != '1' && opcodeStr[0] != '*'))
		{
			continue;
		}

		InstructionRule rule;
		rule.mnemonic = mnemonic;
		rule.mask = 0;
		rule.value = 0;
		int validCharCount = 0;

		// 严格将 10 位文本转换为二进制位
		for (char ch : opcodeStr)
		{
			rule.mask <<= 1;
			rule.value <<= 1;

			if (ch == '1')
			{
				rule.mask |= 1;
				rule.value |= 1;
				validCharCount++;
			}
			else if (ch == '0')
			{
				rule.mask |= 1;
				validCharCount++;
			}
			else if (ch == '*')
			{
				validCharCount++; // 通配符位对应的 mask 和 value 保持为 0
			}
		}

		// 如果长度不是 10 位，或者 mask 全为 0（万能钥匙漏洞），直接跳过
		if (validCharCount != 10 || rule.mask == 0)
		{
			continue;
		}

		g_instructionSetRules.push_back(rule);
	}

	file.close();
	return true;
}

bool CDialogSymTab::IsInstructionValid(unsigned int cmd)
{
	// 如果规则库因为路径不对或者全是空行导致没读到任何规则，直接判定为无效
	if (g_instructionSetRules.empty()) return false;

	// 1. 提取指令的 11 ~ 2 位
	unsigned int opcodeValue = (cmd >> 2) & 0x3FF;

	// 2. 遍历所有经过严格筛选的合法规则
	for (const auto& rule : g_instructionSetRules)
	{
		// 此时 rule.mask 绝对不为 0，只有当特征码完美吻合时才会返回 true
		if ((opcodeValue & rule.mask) == rule.value)
		{
			return true;
		}
	}
	return false;
}
/*
MFC 调用核心，用于实现运行时类型识别(RTTI)
*/ 
IMPLEMENT_DYNAMIC(CDialogSymTab, CMyDialog)
//将此 C++ 类与资源文件中的对话框模板（IDD_DIALOG_SYM_TAB，即画好的界面布局）进行绑定
BEGIN_MESSAGE_MAP(CDialogSymTab, CMyDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CDialogSymTab::OnBnClickedButtonModify)
	ON_EN_CHANGE(2001, &CDialogSymTab::OnEnChangeEditOffset)  // 使用固定ID 2001
END_MESSAGE_MAP()


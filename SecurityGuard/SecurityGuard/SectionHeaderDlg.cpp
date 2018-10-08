// SectionHeaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "SectionHeaderDlg.h"
#include "afxdialogex.h"


// CSectionHeaderDlg 对话框

IMPLEMENT_DYNAMIC(CSectionHeaderDlg, CDialogEx)

CSectionHeaderDlg::CSectionHeaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SectionHeader, pParent)
{

}

CSectionHeaderDlg::~CSectionHeaderDlg()
{
}

void CSectionHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SectionHeader, m_SectionHeaderList);
}


BEGIN_MESSAGE_MAP(CSectionHeaderDlg, CDialogEx)
END_MESSAGE_MAP()


// CSectionHeaderDlg 消息处理程序


BOOL CSectionHeaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_SectionHeaderList.InsertColumnCustom(6, 0.1, L"名称", 0.18, L"VRA", 0.18, L"VSize", 0.18, L"ROffset", 0.18, L"RSize", 0.18, L"标志");
	ShowSectionHeaderInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
	
void CSectionHeaderDlg::ShowSectionHeaderInfo()
{
	IMAGE_SECTION_HEADER* pSHeader = m_pPE->m_pSectionHeader;
	DWORD dwCount = m_pPE->m_pFileHeader->NumberOfSections;
	for (DWORD i = 0; i < dwCount ; i++) {
		BYTE name[10] = { 0 };
		memcpy_s(name, 8, pSHeader->Name, 8);
		//CStringW wName = CW2CW(name);
		WCHAR wchName[256] = { 0 };
		swprintf_s(wchName, L"%hs",name);
		CString VirtualSize, VirtualAddress, SizeOfRawData, PointerToRawData, Characteristic;
		VirtualSize.Format(L"%08X", pSHeader->Misc.VirtualSize);
		VirtualAddress.Format(L"%08X", pSHeader->VirtualAddress);
		SizeOfRawData.Format(L"%08X", pSHeader->SizeOfRawData);
		PointerToRawData.Format(L"%08X", pSHeader->PointerToRawData);
		Characteristic.Format(L"%08X", pSHeader->Characteristics);
		
		m_SectionHeaderList.InsertItemCustom(6, wchName, VirtualAddress, VirtualSize, PointerToRawData, SizeOfRawData, Characteristic);
		pSHeader++;

	}
}

// ExportDirDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ExportDirDlg.h"
#include "afxdialogex.h"


// CExportDirDlg 对话框

IMPLEMENT_DYNAMIC(CExportDirDlg, CDialogEx)

CExportDirDlg::CExportDirDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_EXPORT_DIR, pParent)
	, m_strOffset(_T(""))
	, m_strCharacteristic(_T(""))
	, m_strBase(_T(""))
	, m_strName(_T(""))
	, m_strNumOfFunc(_T(""))
	, m_strNumOfNames(_T(""))
	, m_strAddrOfFunc(_T(""))
	, m_strAddrOfNames(_T(""))
	, m_strAddrOfOrdinals(_T(""))
	, m_strNameString(_T(""))
{

}

CExportDirDlg::~CExportDirDlg()
{
}

void CExportDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_offset, m_strOffset);
	DDX_Text(pDX, IDC_EDIT_characteristic, m_strCharacteristic);
	DDX_Text(pDX, IDC_EDIT_base, m_strBase);
	DDX_Text(pDX, IDC_EDIT_name, m_strName);
	//  DDX_Control(pDX, IDC_EDIT_name_str, m_strNameString);
	DDX_Text(pDX, IDC_EDIT_NumOfFunc, m_strNumOfFunc);
	DDX_Text(pDX, IDC_EDIT_NumOfNames, m_strNumOfNames);
	DDX_Text(pDX, IDC_EDIT_AddrOfFunc, m_strAddrOfFunc);
	DDX_Text(pDX, IDC_EDIT_NumOfName, m_strAddrOfNames);
	DDX_Text(pDX, IDC_EDIT_NumOfOrdinal, m_strAddrOfOrdinals);
	DDX_Text(pDX, IDC_EDIT_name_str, m_strNameString);
	DDX_Control(pDX, IDC_LIST_EXPORT_DIR, m_ExportDirList);
}


BEGIN_MESSAGE_MAP(CExportDirDlg, CDialogEx)
END_MESSAGE_MAP()


// CExportDirDlg 消息处理程序


BOOL CExportDirDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowExportDirInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void CExportDirDlg::ShowExportDirInfo()
{
	if (m_pPE->m_pDataDir[0].Size == 0) {
		MessageBox(L"该文件没有导出表");
		return;
	}
	DWORD VirtualAddress = m_pPE->m_pDataDir[0].VirtualAddress;
	DWORD Offset = m_pPE->RVA2FOA(VirtualAddress);
	m_strOffset.Format(L"%08X", Offset);
	IMAGE_EXPORT_DIRECTORY*  pExportDir = (IMAGE_EXPORT_DIRECTORY*)(Offset + (DWORD)m_pPE->m_pFileBuffer);
	//显示整体信息
	m_strCharacteristic.Format(L"%08X", pExportDir->Characteristics);
	m_strBase.Format(L"%08X", pExportDir->Base);
	m_strName.Format(L"%08X", pExportDir->Name);
	DWORD NameFOA = m_pPE->RVA2FOA(pExportDir->Name);
	DWORD NameAddrOfFile = NameFOA + (DWORD)m_pPE->m_pFileBuffer;
	char* chNameString = (char*)NameAddrOfFile;
	WCHAR wName[100] = { 0 };
	swprintf_s(wName, L"%hs", chNameString);
	m_strNameString = wName;
	m_strNumOfFunc.Format(L"%08X", pExportDir->NumberOfFunctions);
	m_strNumOfNames.Format(L"%08X", pExportDir->NumberOfNames);
	m_strAddrOfFunc.Format(L"%08X", pExportDir->AddressOfFunctions);
	m_strAddrOfNames.Format(L"%08X", pExportDir->AddressOfNames);
	m_strAddrOfOrdinals.Format(L"%08X", pExportDir->AddressOfNameOrdinals);
	

	UpdateData(false);
	//详细的表格中的信息
	m_ExportDirList.InsertColumnCustom(4, 0.1, L"序号", 0.18, L"RVA", 0.18, L"Offset", 0.54, L"FunctionName");
	DWORD dwFuncAddrRVA = pExportDir->AddressOfFunctions;
	DWORD dwFuncAddrFoa = m_pPE->RVA2FOA(dwFuncAddrRVA);
	DWORD dwFuncAddrOfFile = dwFuncAddrFoa + (DWORD)m_pPE->m_pFileBuffer; //函数表的地址

	DWORD dwNameAddrRVA = pExportDir->AddressOfNames;
	DWORD dwNameAddrFoa = m_pPE->RVA2FOA(dwNameAddrRVA);
	DWORD dwNameAddrOfFile = dwNameAddrFoa + (DWORD)m_pPE->m_pFileBuffer;

	DWORD dwOrderAddrRVA = pExportDir->AddressOfNameOrdinals;
	DWORD dwOrderAddrFoa = m_pPE->RVA2FOA(dwOrderAddrRVA);
	DWORD dwOrderAddrOfFile = dwOrderAddrFoa + (DWORD)m_pPE->m_pFileBuffer;
	for (DWORD i = 0; i < pExportDir->NumberOfFunctions; i++) {
		CString index, funcRVA, funcOffset, FuncNameString;
		index.Format(L"%04X", i + pExportDir->Base);  // + 1还是base
		//RVA
		funcRVA.Format(L"%08X", *(DWORD*)(dwFuncAddrOfFile + 4* i));
		funcOffset.Format(L"%08X", m_pPE->RVA2FOA(*(DWORD*)(dwFuncAddrOfFile + 4 * i))); //表格需要显示
		DWORD dwOrdinalIndexNeed = 0;
		BOOL bFind = false;
		for (DWORD j = 0; j < pExportDir->NumberOfNames; j++) {
			WORD dwOrdinal = *(WORD*)(dwOrderAddrOfFile + 2 * j); // word类型
			if (dwOrdinal == i) {
				dwOrdinalIndexNeed = j;
				bFind = true;
				break;
			}
		}
		if (bFind) {
			//函数名导出的函数
			DWORD dwFuncAddrRVANeed = *(DWORD*)(dwNameAddrOfFile + 4 * dwOrdinalIndexNeed);
			DWORD dwFuncAddrFoaNeed = m_pPE->RVA2FOA(dwFuncAddrRVANeed);
			DWORD dwFuncAddrOfFileNeed = dwFuncAddrFoaNeed + (DWORD)m_pPE->m_pFileBuffer;
			char* pName = (char*)dwFuncAddrOfFileNeed;
			WCHAR wName[256] = { 0 };
			swprintf_s(wName, L"%hs", pName);
			FuncNameString.Format(L"%s", wName);
		}

		m_ExportDirList.InsertItemCustom(4, index, funcRVA, funcOffset, FuncNameString);

	}

}

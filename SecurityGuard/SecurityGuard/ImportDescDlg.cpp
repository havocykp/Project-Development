// ImportDescDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ImportDescDlg.h"
#include "afxdialogex.h"


// CImportDescDlg 对话框

IMPLEMENT_DYNAMIC(CImportDescDlg, CDialogEx)

CImportDescDlg::CImportDescDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_Import_desc, pParent)
{

}

CImportDescDlg::~CImportDescDlg()
{
}

void CImportDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DLL, m_ImportDllList);
	DDX_Control(pDX, IDC_LIST_Thunk, m_ImportThunkList);
}


BEGIN_MESSAGE_MAP(CImportDescDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DLL, &CImportDescDlg::OnClickListDll)
END_MESSAGE_MAP()

void CImportDescDlg::ShowImportDllList()
{
	//初始化表格标题
	m_ImportDllList.InsertColumnCustom(6, 0.16, L"DllName", 0.16, L"OriginalFirstThunk", 0.16, L"TimeDataStamp", 0.16, L"ForwardChain", 0.16, L"名称", 0.16, L"FirstThunk");
	DWORD dwImportDescRVA = m_pPE->m_pDataDir[1].VirtualAddress;
	DWORD dwImportDescFOA = m_pPE->RVA2FOA(dwImportDescRVA);
	DWORD dwImportDescAddrOfFile = dwImportDescFOA + (DWORD)m_pPE->m_pFileBuffer;
	IMAGE_IMPORT_DESCRIPTOR* pImportDesc =(IMAGE_IMPORT_DESCRIPTOR*)dwImportDescAddrOfFile;

	while(pImportDesc->Name) {
		CString DllNameString, OriginalFirstThunk, TimeDateStamp, ForwarderChain, NameRVA, FirstThunk;
		DWORD dwNameAddrRVA = pImportDesc->Name;
		DWORD dwNameAddrFOA = m_pPE->RVA2FOA(dwNameAddrRVA);
		DWORD dwNameAddrOfFile = dwNameAddrFOA + (DWORD)m_pPE->m_pFileBuffer;
		char* chName = (char*)dwNameAddrOfFile;
		WCHAR wName[256] = { 0 };
		swprintf_s(wName, L"%hs", chName);
		DllNameString.Format(L"%s", wName); // table
		NameRVA.Format(L"%08X", dwNameAddrRVA); // table
		OriginalFirstThunk.Format(L"%08X", pImportDesc->OriginalFirstThunk);
		TimeDateStamp.Format(L"%08X", pImportDesc->TimeDateStamp);
		ForwarderChain.Format(L"%08X", pImportDesc->ForwarderChain);
		FirstThunk.Format(L"%08X", pImportDesc->FirstThunk);
		m_ImportDllList.InsertItemCustom(6, DllNameString, OriginalFirstThunk, TimeDateStamp, ForwarderChain, NameRVA, FirstThunk);
		pImportDesc++;
	}
}

void CImportDescDlg::ShowImportThunkByDll()
{

}

// CImportDescDlg 消息处理程序


BOOL CImportDescDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowImportDllList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}




void CImportDescDlg::OnClickListDll(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//空白区不响应
	if (pNMItemActivate->iItem == -1) {
		return;
	}
	m_ImportThunkList.DeleteAllItems();
	CString strDllNameRVA = m_ImportDllList.GetItemText(pNMItemActivate->iItem, 4); // 用dll的name(RVA)作为行的识别对象
	//初始化表格标题
	m_ImportThunkList.InsertColumnCustom(5, 0.15, L"ThunkRVA", 0.15, L"ThunkOffset", 0.15, L"ThunkValue", 0.1, L"Hint", 0.1, L"ApiName");

	DWORD dwImportDescRVA = m_pPE->m_pDataDir[1].VirtualAddress;
	DWORD dwImportDescFOA = m_pPE->RVA2FOA(dwImportDescRVA);
	DWORD dwImportDescAddrOfFile = dwImportDescFOA + (DWORD)m_pPE->m_pFileBuffer;
	IMAGE_IMPORT_DESCRIPTOR* pImportDesc = (IMAGE_IMPORT_DESCRIPTOR*)dwImportDescAddrOfFile;

	while (pImportDesc->Name) {
		CString ThunkRVA, ThunkOffset, ThunkValue, Hint, ApiName;
		DWORD  dwDllNameRVA = pImportDesc->Name;
		CString tmpName;
		tmpName.Format(L"%08X", dwDllNameRVA);
		if (tmpName == strDllNameRVA) { // 找到了
			DWORD ThunkAddrRVA = pImportDesc->OriginalFirstThunk;
			DWORD ThunkAddrFOA = m_pPE->RVA2FOA(ThunkAddrRVA);
			DWORD ThunkAddrOfFile = ThunkAddrFOA + (DWORD)m_pPE->m_pFileBuffer;
			IMAGE_THUNK_DATA32* pThunkData = (IMAGE_THUNK_DATA32*)ThunkAddrOfFile;
			DWORD i = 0;
			while (pThunkData->u1.Ordinal) {
				ThunkRVA.Format(L"%08X", ThunkAddrRVA + i*4);
				ThunkOffset.Format(L"%08X", ThunkAddrFOA + i*4);
				DWORD dwNameRVA = pThunkData->u1.AddressOfData;
				ThunkValue.Format(L"%08X", dwNameRVA);
				if (!IMAGE_SNAP_BY_ORDINAL32(pThunkData->u1.Ordinal)) {
					//名称导入的
					DWORD  dwNameFOA = m_pPE->RVA2FOA(dwNameRVA);
					DWORD  dwNameAddrOfFile = dwNameFOA + (DWORD)m_pPE->m_pFileBuffer;
					IMAGE_IMPORT_BY_NAME* pByName = (IMAGE_IMPORT_BY_NAME*)dwNameAddrOfFile;
					Hint.Format(L"%04X", pByName->Hint);
					CHAR* chName = pByName->Name;
					WCHAR wName[256] = { 0 };
					swprintf_s(wName, L"%hs", chName);
					ApiName.Format(L"%s", wName);

				}
				else { //序号导入
					Hint.Format(L"%s", L"-");
					ApiName.Format(L"Ordinal : %02X", pThunkData->u1.Ordinal & 0xFFFF);
				}
				pThunkData++;
				i++;
				m_ImportThunkList.InsertItemCustom(5, ThunkRVA, ThunkOffset, ThunkValue, Hint, ApiName);
			}
			
			break;//终止
		}
		pImportDesc++;
	}
		
}

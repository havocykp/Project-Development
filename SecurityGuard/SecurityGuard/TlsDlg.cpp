// TlsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "TlsDlg.h"
#include "afxdialogex.h"


// CTlsDlg 对话框

IMPLEMENT_DYNAMIC(CTlsDlg, CDialogEx)

CTlsDlg::CTlsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_TLS, pParent)
	, m_StartAddressOfRawData(_T(""))
	, m_EndAddressOfRawData(_T(""))
	, m_AddressOfIndex(_T(""))
	, m_AddressOfCallBacks(_T(""))
	, m_SizeOfZeroFill(_T(""))
	, m_Characteristics(_T(""))
{

}

CTlsDlg::~CTlsDlg()

{
}

void CTlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_StartAddressOfRawData, m_StartAddressOfRawData);
	DDX_Text(pDX, IDC_EDIT_EndAddressOfRawData, m_EndAddressOfRawData);
	DDX_Text(pDX, IDC_EDIT_AddressOfIndex, m_AddressOfIndex);
	DDX_Text(pDX, IDC_EDIT_AddressOfCallBacks, m_AddressOfCallBacks);
	DDX_Text(pDX, IDC_EDIT_SizeOfZeroFill, m_SizeOfZeroFill);
	DDX_Text(pDX, IDC_EDIT_Characteristics, m_Characteristics);
}


BEGIN_MESSAGE_MAP(CTlsDlg, CDialogEx)
END_MESSAGE_MAP()


// CTlsDlg 消息处理程序


BOOL CTlsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwTlsRVA = m_pPE->m_pDataDir[9].VirtualAddress;
	DWORD dwTlsFOA = m_pPE->RVA2FOA(dwTlsRVA);
	DWORD dwTlsAddrOfFile = dwTlsFOA + (DWORD)m_pPE->m_pFileBuffer;
	IMAGE_TLS_DIRECTORY* pDirTLS = (IMAGE_TLS_DIRECTORY*)dwTlsAddrOfFile;

	m_StartAddressOfRawData.Format(L"%08X", pDirTLS->StartAddressOfRawData);
	m_EndAddressOfRawData.Format(L"%08X", pDirTLS->EndAddressOfRawData);
	m_AddressOfIndex.Format(L"%08X", pDirTLS->AddressOfIndex);
	m_AddressOfCallBacks.Format(L"%08X", pDirTLS->AddressOfCallBacks);
	m_SizeOfZeroFill.Format(L"%08X", pDirTLS->SizeOfZeroFill);
	m_Characteristics.Format(L"%08X", pDirTLS->Characteristics);

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
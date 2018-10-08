// DataDirDlb.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "DataDirDlb.h"
#include "afxdialogex.h"
#include "ExportDirDlg.h"
#include "ImportDescDlg.h"
#include "ResourceDirDlg.h"
#include "RelocationDlg.h"
#include "TlsDlg.h"


// CDataDirDlb 对话框

IMPLEMENT_DYNAMIC(CDataDirDlb, CDialogEx)

CDataDirDlb::CDataDirDlb(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_DATA_DIR, pParent)
	, m_strExportRva(_T(""))
	, m_strExportSize(_T(""))
	, m_strImportRVA(_T(""))
	, m_strImportSize(_T(""))
	, m_strResourceRVA(_T(""))
	, m_strResourceSize(_T(""))
	, m_strRelocRVA(_T(""))
	, m_strRelocSize(_T(""))
	, m_strDelayRVA(_T(""))
	, m_strDelaySize(_T(""))
	, m_strTLSRVA(_T(""))
	, m_strTLSSize(_T(""))
{

}

CDataDirDlb::~CDataDirDlb()
{
}

void CDataDirDlb::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RVA_Export, m_strExportRva);
	DDX_Text(pDX, IDC_EDIT_Size_Export, m_strExportSize);
	DDX_Text(pDX, IDC_EDIT_RVA_import, m_strImportRVA);
	DDX_Text(pDX, IDC_EDIT_Size_import, m_strImportSize);
	DDX_Text(pDX, IDC_EDIT_RVA_resource, m_strResourceRVA);
	DDX_Text(pDX, IDC_EDIT_Size_resource, m_strResourceSize);
	DDX_Text(pDX, IDC_EDIT_RVA_relocation, m_strRelocRVA);
	DDX_Text(pDX, IDC_EDIT_Size_relocation, m_strRelocSize);
	DDX_Text(pDX, IDC_EDIT_RVA_delay, m_strDelayRVA);
	DDX_Text(pDX, IDC_EDIT_Size_delay, m_strDelaySize);
	DDX_Text(pDX, IDC_EDIT_RVA_TLS, m_strTLSRVA);
	DDX_Text(pDX, IDC_EDIT1_Size_TLS, m_strTLSSize);
}


BEGIN_MESSAGE_MAP(CDataDirDlb, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_export, &CDataDirDlb::OnBnClickedBtnExportDir)
	ON_BN_CLICKED(IDC_BTN_import, &CDataDirDlb::OnBnClickedBtnImportDesc)
	ON_BN_CLICKED(IDC_BTN_resource, &CDataDirDlb::OnBnClickedBtnResourceDir)
	ON_BN_CLICKED(IDC_BTN_relocation, &CDataDirDlb::OnBnClickedBtnRelocation)
	ON_BN_CLICKED(IDC_BTN_Tls, &CDataDirDlb::OnBnClickedBtnTls)
END_MESSAGE_MAP()

void CDataDirDlb::ShowDataDirInfo()
{
	IMAGE_DATA_DIRECTORY*  pDataDir = m_pPE->m_pOptionalHeader->DataDirectory;
	//导出表数据目录
	m_strExportRva.Format(L"%08X", pDataDir[0].VirtualAddress);
	m_strExportSize.Format(L"%08X", pDataDir[0].Size);
	//导入表
	m_strImportRVA.Format(L"%08X", pDataDir[1].VirtualAddress);
	m_strImportSize.Format(L"%08X", pDataDir[1].Size);
	//资源表
	m_strResourceRVA.Format(L"%08X", pDataDir[2].VirtualAddress);
	m_strResourceSize.Format(L"%08X", pDataDir[2].Size);
	//重定位表
	m_strRelocRVA.Format(L"%08X", pDataDir[5].VirtualAddress);
	m_strRelocSize.Format(L"%08X", pDataDir[5].Size);
	//延迟加载表
	m_strDelayRVA.Format(L"%08X", pDataDir[13].VirtualAddress);
	m_strDelaySize.Format(L"%08X", pDataDir[13].Size);
	//TLS表
	m_strTLSRVA.Format(L"%08X", pDataDir[9].VirtualAddress);
	m_strTLSSize.Format(L"%08X", pDataDir[9].Size);

	UpdateData(false);
}

// CDataDirDlb 消息处理程序


BOOL CDataDirDlb::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowDataDirInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDataDirDlb::OnBnClickedBtnExportDir()
{
	// TODO: 在此添加控件通知处理程序代码
	CExportDirDlg* pExportDirDlg = new CExportDirDlg();
	pExportDirDlg->m_pPE = m_pPE;
	pExportDirDlg->DoModal();

}


void CDataDirDlb::OnBnClickedBtnImportDesc()
{
	// TODO: 在此添加控件通知处理程序代码
	CImportDescDlg* pImportDescDlg = new CImportDescDlg();
	pImportDescDlg->m_pPE = m_pPE;
	pImportDescDlg->DoModal();
}


void CDataDirDlb::OnBnClickedBtnResourceDir()
{
	// TODO: 在此添加控件通知处理程序代码
	CResourceDirDlg* pRescDirDlg = new CResourceDirDlg();
	pRescDirDlg->m_pPE = m_pPE;
	pRescDirDlg->DoModal();
}


void CDataDirDlb::OnBnClickedBtnRelocation()
{
	// TODO: 在此添加控件通知处理程序代码
	CRelocationDlg* pRelocDlg = new CRelocationDlg();
	pRelocDlg->m_pPE = m_pPE;
	pRelocDlg->DoModal();
}


void CDataDirDlb::OnBnClickedBtnTls()
{
	// TODO: 在此添加控件通知处理程序代码
	CTlsDlg* pTlsDlg = new CTlsDlg();
	pTlsDlg->m_pPE = m_pPE;
	pTlsDlg->DoModal();
}

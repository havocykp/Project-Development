// NtHeaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "NtHeaderDlg.h"
#include "afxdialogex.h"
#include "SectionHeaderDlg.h"
#include "DataDirDlb.h"

// CNtHeaderDlg 对话框

IMPLEMENT_DYNAMIC(CNtHeaderDlg, CDialogEx)

CNtHeaderDlg::CNtHeaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Dlg_NT_HEADER, pParent)
	, m_strOEP(_T(""))
	, m_strSizeOfImage(_T(""))
	, m_strImageBase(_T(""))
	, m_strBaseOfCode(_T(""))
	, m_strBaseOfData(_T(""))
	, m_strSectionAlignment(_T(""))
	, m_strFileAlignment(_T(""))
	, m_strMagic(_T(""))
	, m_strSubSystem(_T(""))
	, m_strNumOfSection(_T(""))
	, m_strTimeDateStamp(_T(""))
	, m_strSizeOfHeader(_T(""))
	, m_strDllCharacteristic(_T(""))
	, m_strCheckSum(_T(""))
	, m_strSizeOfOptionalHeader(_T(""))
	, m_strNumOfDataDir(_T(""))
{

}

CNtHeaderDlg::~CNtHeaderDlg()
{
}

void CNtHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OEP, m_strOEP);
	DDX_Text(pDX, IDC_EDIT_SizeOfImage, m_strSizeOfImage);
	DDX_Text(pDX, IDC_EDIT_ImageBase, m_strImageBase);
	DDX_Text(pDX, IDC_EDIT_BaseOfCode, m_strBaseOfCode);
	DDX_Text(pDX, IDC_EDIT_BaseOfData, m_strBaseOfData);
	DDX_Text(pDX, IDC_EDIT_SectionAlignment, m_strSectionAlignment);
	DDX_Text(pDX, IDC_EDIT_FileAlignment, m_strFileAlignment);
	DDX_Text(pDX, IDC_EDIT_Magic, m_strMagic);
	DDX_Text(pDX, IDC_EDIT_SubSystem, m_strSubSystem);
	DDX_Text(pDX, IDC_EDIT_NumOfSection, m_strNumOfSection);
	DDX_Text(pDX, IDC_EDIT_TimeDateStamp, m_strTimeDateStamp);
	DDX_Text(pDX, IDC_EDIT_SizeOfHeader, m_strSizeOfHeader);
	DDX_Text(pDX, IDC_EDIT_DllCharacteristic, m_strDllCharacteristic);
	DDX_Text(pDX, IDC_EDIT1_CheckSum, m_strCheckSum);
	DDX_Text(pDX, IDC_EDIT_SizeOfOptionalHeader, m_strSizeOfOptionalHeader);
	DDX_Text(pDX, IDC_EDIT_NumOfDataDir, m_strNumOfDataDir);
}


BEGIN_MESSAGE_MAP(CNtHeaderDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SECTION_HEADER, &CNtHeaderDlg::OnBnClickedBtnSectionHeader)
	ON_BN_CLICKED(IDC_BTN_DATADIR, &CNtHeaderDlg::OnBnClickedBtnDatadir)
END_MESSAGE_MAP()


// CNtHeaderDlg 消息处理程序


BOOL CNtHeaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowNtHeaderInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CNtHeaderDlg::ShowNtHeaderInfo()
{
	IMAGE_OPTIONAL_HEADER* pOptionalHeader = m_pPE->m_pOptionalHeader;
	m_strOEP.Format(L"%08X", pOptionalHeader->AddressOfEntryPoint);
	m_strImageBase.Format(L"%08X", pOptionalHeader->ImageBase);
	m_strSizeOfImage.Format(L"%08X", pOptionalHeader->SizeOfImage);
	m_strBaseOfCode.Format(L"%08X", pOptionalHeader->BaseOfCode);
	m_strBaseOfData.Format(L"%08X", pOptionalHeader->BaseOfData);
	m_strSectionAlignment.Format(L"%08X", pOptionalHeader->SectionAlignment);
	m_strFileAlignment.Format(L"%08X", pOptionalHeader->FileAlignment);
	m_strMagic.Format(L"%04X", pOptionalHeader->Magic);
	m_strSubSystem.Format(L"%04X", pOptionalHeader->Subsystem);
	m_strNumOfSection.Format(L"%04X", m_pPE->m_pFileHeader->NumberOfSections);
	m_strTimeDateStamp.Format(L"%08X", m_pPE->m_pFileHeader->TimeDateStamp);
	m_strSizeOfHeader.Format(L"%08X", pOptionalHeader->SizeOfHeaders);
	m_strDllCharacteristic.Format(L"%04X", m_pPE->m_pFileHeader->Characteristics);
	m_strCheckSum.Format(L"%08X", pOptionalHeader->CheckSum);
	m_strSizeOfOptionalHeader.Format(L"%04X", m_pPE->m_pFileHeader->SizeOfOptionalHeader);
	m_strNumOfDataDir.Format(L"%08X", pOptionalHeader->NumberOfRvaAndSizes);


	UpdateData(false);
}


void CNtHeaderDlg::OnBnClickedBtnSectionHeader()
{
	// TODO: 在此添加控件通知处理程序代码
	CSectionHeaderDlg* pSectionHeaderDlg = new CSectionHeaderDlg();
	pSectionHeaderDlg->m_pPE = m_pPE;
	pSectionHeaderDlg->DoModal();

}


void CNtHeaderDlg::OnBnClickedBtnDatadir()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataDirDlb* pDataDirDlb = new CDataDirDlb();
	pDataDirDlb->m_pPE = m_pPE;
	pDataDirDlb->DoModal();
	
}

// PeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "PeDlg.h"
#include "afxdialogex.h"
#include "NtHeaderDlg.h"


// CPeDlg 对话框

IMPLEMENT_DYNAMIC(CPeDlg, CDialogEx)

CPeDlg::CPeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOADPE, pParent)
	, m_strChoosedFile(_T(""))
{

}

CPeDlg::~CPeDlg()
{
}

void CPeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_PE_FILE, m_EditBrowseCtrl);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_PE_FILE, m_strChoosedFile);
}


BEGIN_MESSAGE_MAP(CPeDlg, CDialogEx)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_PE_FILE, &CPeDlg::OnEnChangeMfceditbrowsePeFile)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_PE, &CPeDlg::OnBnClickedButtonLoadPe)
END_MESSAGE_MAP()


// CPeDlg 消息处理程序


void CPeDlg::OnEnChangeMfceditbrowsePeFile()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_strChoosedFile.IsEmpty()) {
		MessageBox(L"请选择PE文件");
		return;
	}

}


void CPeDlg::OnBnClickedButtonLoadPe()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_strChoosedFile.IsEmpty()) {
		MessageBox(L"文件不能为空");
		return;
	}
	//开始加载;验证签名
	WCHAR  pathBuffer[256] = { 0 };
	CStringW  strWide = CT2CW(m_strChoosedFile);
	wcscpy_s(pathBuffer,wcslen(strWide)*2 + 1,  strWide);
	m_pPE = new CPe();
	if (!m_pPE->LoadFile(pathBuffer)) {
		MessageBox(L"无法加载此文件");
		return;
	}
	if (!m_pPE->IsPeFile()) {
		MessageBox(L"此文件不是PE格式");
		return;
	}
	//显示nt头信息Dlg
	CNtHeaderDlg* pNtHeaderDlg = new CNtHeaderDlg;
	pNtHeaderDlg->m_pPE = m_pPE;
	pNtHeaderDlg->DoModal();
	return;
}

// StartupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "StartupDlg.h"
#include "afxdialogex.h"


// CStartupDlg 对话框

IMPLEMENT_DYNAMIC(CStartupDlg, CDialogEx)

CStartupDlg::CStartupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_StartUp, pParent)
{
	m_SubKey1 = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	m_SubKey2 = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	m_hRootKey = HKEY_LOCAL_MACHINE;

	m_KeyName = L"DisplayName";
	m_KeyNameVer = L"DisplayVersion";
	m_KeyNameSize = L"EstimatedSize";
	m_KeyNameDate = L"InstallDate";
	m_KeyNamePub = L"Publisher";
	m_KeyNameInsPath = L"InstallLocation";
	m_KeyNameUniPath = L"UninstallString";
}

CStartupDlg::~CStartupDlg()
{
}

void CStartupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_StartUp, m_StartupList);
}


BEGIN_MESSAGE_MAP(CStartupDlg, CDialogEx)
	//ON_COMMAND(ID_UnLoad, &CStartupDlg::OnUnload)
	ON_COMMAND(ID_Brush, &CStartupDlg::OnBrush)
	ON_COMMAND(ID_OpenFile, &CStartupDlg::OnOpenfile)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_StartUp, &CStartupDlg::OnNMRClickListStartup)
END_MESSAGE_MAP()


// CStartupDlg 消息处理程序


BOOL CStartupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化表格
	//m_StartupList.InsertColumnCustom(3, 0.2, L"Index", 0.3, L"keyName", 0.5, L"keyValue");
	DWORD dwOldStyle = m_StartupList.GetExtendedStyle();

	m_StartupList.SetExtendedStyle(
		dwOldStyle | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES
	);
	CRect rc;
	m_StartupList.GetClientRect(rc);
	int nWidth = rc.Width();

	m_StartupList.InsertColumn(0, L"名称", LVCFMT_CENTER, 250, -1);
	m_StartupList.InsertColumn(1, L"版本", LVCFMT_CENTER, 50, -1);
	m_StartupList.InsertColumn(2, L"安装日期", LVCFMT_CENTER, 90, -1);
	m_StartupList.InsertColumn(3, L"大小", LVCFMT_CENTER, 70, -1);
	m_StartupList.InsertColumn(4, L"安装路径", LVCFMT_CENTER, 250, -1);
	m_StartupList.InsertColumn(5, L"卸载路径", LVCFMT_CENTER, 250, -1);


	//获取软件信息
	OnBrush();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// void CStartupDlg::UpdateStartupInfo()
// {
// 	m_StartupList.DeleteAllItems();
// 	
// }


void CStartupDlg::GetSoftwareInfo(CString SubKey)
{
	//定义相关信息
	HKEY hKey = NULL;
	HKEY hSubKey = NULL;
	LONG lRet;
	CString strBuffer;
	CString strMidReg;

	DWORD dwIndex = 0;
	DWORD dwSoftSize = 0;
	WCHAR szKeyName[MAX_PATH] = { 0 };
	WCHAR szBuffer[MAX_PATH] = { 0 };
	DWORD dwKeyLen = MAXBYTE;
	DWORD dwNameLen = MAXBYTE;
	DWORD dwType = REG_DWORD | REG_EXPAND_SZ | REG_SZ;

	//打开子键
	lRet = RegOpenKeyEx(m_hRootKey, SubKey, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);

	if (lRet == ERROR_SUCCESS)
	{
		//枚举软件目录子键
		while (ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hKey, dwIndex, szKeyName, &dwKeyLen, 0, NULL, NULL, NULL))
		{

			strBuffer.Format(L"%s", szKeyName);
			if (!strBuffer.IsEmpty())
			{
				//格式化拼接子键
				strMidReg = SubKey + L"\\" + strBuffer;
				//打开单个软件子键
				if (ERROR_SUCCESS == RegOpenKeyEx(m_hRootKey, strMidReg, 0, KEY_READ | KEY_WOW64_64KEY, &hSubKey))
				{
					//获取信息
					SOFTINFO1 SoftInfo = { 0 };

					//名称
					RegQueryValueEx(hSubKey, m_KeyName, 0, &dwType, (LPBYTE)SoftInfo.szSoftName, &dwNameLen);
					dwNameLen = MAXBYTE;	//需要恢复此值

											//厂商
					RegQueryValueEx(hSubKey, m_KeyNamePub, 0, &dwType, (LPBYTE)SoftInfo.szSoftPub, &dwNameLen);
					dwNameLen = MAXBYTE;

					//日期
					RegQueryValueEx(hSubKey, m_KeyNameDate, 0, &dwType, (LPBYTE)SoftInfo.szSoftDate, &dwNameLen);
					dwNameLen = MAXBYTE;

					//版本
					RegQueryValueEx(hSubKey, m_KeyNameVer, 0, &dwType, (LPBYTE)SoftInfo.szSoftVer, &dwNameLen);
					dwNameLen = MAXBYTE;

					//安装路径
					RegQueryValueEx(hSubKey, m_KeyNameInsPath, 0, &dwType, (LPBYTE)SoftInfo.strSoftInsPath, &dwNameLen);
					dwNameLen = MAXBYTE;

					//卸载路径
					RegQueryValueEx(hSubKey, m_KeyNameUniPath, 0, &dwType, (LPBYTE)SoftInfo.strSoftUniPath, &dwNameLen);
					dwNameLen = MAXBYTE;

					//大小为DWORD值
					RegQueryValueEx(hSubKey, m_KeyNameSize, 0, &dwType, (LPBYTE)&dwSoftSize, &dwNameLen);
					dwNameLen = MAXBYTE;

					//转换文件大小
					CString strSize;
					if (dwSoftSize > 0 && dwSoftSize < 1024)
					{
						strSize.Format(L"%dKB", dwSoftSize);
					}
					else if (dwSoftSize > 1024 && dwSoftSize < 1024 * 1024)
					{
						strSize.Format(L"%dMB", dwSoftSize / 1024);
					}
					else if (dwSoftSize > 1024 * 1024)
					{
						strSize.Format(L"%dGB", dwSoftSize / 1024 / 1024);
					}
					else
					{
						strSize = L" ";
					}
					memcpy_s(SoftInfo.szSoftSize, (strSize.GetLength() + 1) * sizeof(WCHAR),
						strSize.GetBuffer(), (strSize.GetLength() + 1) * sizeof(WCHAR));

					CString strName = SoftInfo.szSoftName;
					if (!strName.IsEmpty())
					{
						m_vecSoftInfo.push_back(SoftInfo);
					}
				}
				dwIndex++;
				dwKeyLen = MAXBYTE;
				dwNameLen = MAXBYTE;
				dwSoftSize = 0;
				ZeroMemory(szKeyName, MAX_PATH);
			}

		}

	}
}

void CStartupDlg::OnUnload()
{
	// TODO: 在此添加命令处理程序代码
	//获取选中项

	if (m_SoftUniPath.IsEmpty())
	{
		MessageBox(L"卸载失败！");
		return;
	}
	else
	{
		ShellExecute(NULL, _T("open"), m_SoftUniPath, NULL, NULL, SW_SHOW);
	}
	m_SoftUniPath = L"";
}


void CStartupDlg::OnBrush()
{
	// TODO: 在此添加命令处理程序代码
	m_StartupList.DeleteAllItems();
	m_vecSoftInfo.clear();

	GetSoftwareInfo(m_SubKey1);

	GetSoftwareInfo(m_SubKey2);


	for (size_t i = 0; i < m_vecSoftInfo.size(); i++)
	{
		m_SoftName = m_vecSoftInfo[i].szSoftName;
		m_SoftPub = m_vecSoftInfo[i].szSoftPub;
		m_SoftDate = m_vecSoftInfo[i].szSoftDate;
		m_SoftSize = m_vecSoftInfo[i].szSoftSize;
		m_SoftVer = m_vecSoftInfo[i].szSoftVer;
		m_SoftInsPath = m_vecSoftInfo[i].strSoftInsPath;
		m_SoftUniPath = m_vecSoftInfo[i].strSoftUniPath;


		m_StartupList.InsertItem(i, m_SoftName);
		m_StartupList.SetItemText(i, 1, m_SoftVer);
		m_StartupList.SetItemText(i, 2, m_SoftDate);
		m_StartupList.SetItemText(i, 3, m_SoftSize);
		m_StartupList.SetItemText(i, 4, m_SoftInsPath);
		m_StartupList.SetItemText(i, 5, m_SoftUniPath);

	}
}


void CStartupDlg::OnOpenfile()
{
	// TODO: 在此添加命令处理程序代码
	//获取路径
	if (m_SoftInsPath.IsEmpty())
	{
		MessageBox(L"打开路径失败！");
	}
	else
	{
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), m_SoftInsPath, NULL, SW_SHOWNORMAL);
	}
	m_SoftInsPath = L"";
}


void CStartupDlg::OnNMRClickListStartup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU4);
	pmenu = menu.GetSubMenu(0);//指定包含菜单中的弹出菜单的位置。 位置值从0开始第一个菜单项。 
	ClientToScreen(&pNMItemActivate->ptAction);//在客户端坐标中指示发生事件的位置
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);


	if (pNMItemActivate->iItem != -1)
	{
		m_SoftInsPath = m_StartupList.GetItemText(pNMItemActivate->iItem, 5);//GetItemText:1.索引， 2.检索对象。返回一个CString的文本
		m_SoftUniPath = m_StartupList.GetItemText(pNMItemActivate->iItem, 6);//GetItemText:1.索引， 2.检索对象。返回一个CString的文本
	}

	// 菜单项分支
	switch (Select)
	{
	case ID_UnLoad: // 卸载
		OnUnload();
		break;
	case ID_Brush: // 刷新
		OnBrush();
		break;
	case ID_OpenFile:
		OnOpenfile(); // 打开文件路径
		break;
	default:
		break;
	}
}

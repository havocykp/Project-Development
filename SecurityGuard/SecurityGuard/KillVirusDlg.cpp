// KillVirusDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "KillVirusDlg.h"
#include "afxdialogex.h"
#include "Md5.h"
#include <TlHelp32.h>


#define  WCHAR_TO_CHAR(lpW_Char, lpChar) \
    WideCharToMultiByte(CP_ACP, NULL, lpW_Char, -1,\
lpChar, _countof(lpChar), NULL, FALSE);

#define  CHAR_TO_WCHAR(lpChar, lpW_Char) \
    MultiByteToWideChar(CP_ACP, NULL, lpChar, -1,\
lpW_Char, _countof(lpW_Char));


// CKillVirusDlg 对话框

IMPLEMENT_DYNAMIC(CKillVirusDlg, CDialogEx)

CKillVirusDlg::CKillVirusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_KillVirus, pParent)
	, m_BrowserPath(_T(""))
	, m_BrowserFile(_T(""))
{

}

CKillVirusDlg::~CKillVirusDlg()
{
}

void CKillVirusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_MFCEDITBROWSE_PATH, m_BrowserPath);
	//  DDX_Control(pDX, IDC_MFCEDITBROWSE_FILE, m_BrowserFile);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_PATH, m_BrowserPath);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_FILE, m_BrowserFile);
}


BEGIN_MESSAGE_MAP(CKillVirusDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_KillVirus_online, &CKillVirusDlg::OnBnClickedBtnKillvirusonline)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_PATH, &CKillVirusDlg::OnEnChangeMfceditbrowsePath)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_FILE, &CKillVirusDlg::OnEnChangeMfceditbrowseFile)
	ON_BN_CLICKED(IDC_BUTTON_md5_save, &CKillVirusDlg::OnBnClickedButtonmd5save)
	ON_BN_CLICKED(IDC_BUTTON_kill_by_md5, &CKillVirusDlg::OnBnClickedButtonkillByMd5)
	ON_BN_CLICKED(IDC_BUTTON_kill_by_path, &CKillVirusDlg::OnBnClickedButtonkillByPath)
	ON_BN_CLICKED(IDC_BUTTON_kill_by_process, &CKillVirusDlg::OnBnClickedButtonkillByProcess)
END_MESSAGE_MAP()


// CKillVirusDlg 消息处理程序

//云查杀
void CKillVirusDlg::OnBnClickedBtnKillvirusonline()
{
	// TODO: 在此添加控件通知处理程序代码
	//连接服务器
	if (!m_client.ConnectServer(SERVERIP, SERVERPORT)) {
		MessageBox(L"连接服务器失败");
		return;
	}
	if (m_BrowserFile.IsEmpty() && m_BrowserPath.IsEmpty()) {
		MessageBox(L"请确认查杀路径或文件");
		return;
	}
	if (!m_BrowserFile.IsEmpty()) { // 文件查杀
		CheckSingleFileOnLine();
	}
	else if (!m_BrowserPath.IsEmpty()) { //路径所有文件查杀
		CheckPathOnLine();
	}

}



void CKillVirusDlg::OnEnChangeMfceditbrowsePath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	m_BrowserFile = L"";
	UpdateData(false);

}


void CKillVirusDlg::OnEnChangeMfceditbrowseFile()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	m_BrowserPath = L"";
	UpdateData(false);
}

void CKillVirusDlg::CheckSingleFileOnLine()
{
	if (!m_BrowserFile.IsEmpty()) { // 文件查杀
		char chFullPath[256] = { 0 };
		WCHAR_TO_CHAR(m_BrowserFile.GetBuffer(), chFullPath);
		char* md5 = md5FileValue(chFullPath);
		if (strlen(md5) == 0) {
			MessageBox(L"无法获取md5");
			return;
		}

		m_client.Send(eSAMPLE, md5, 33);
		char* ret = m_client.Recv();
		if (!strcmp(ret, "true")) {
			MessageBox(L"Warning : 此文件有毒");
			
			return;
		}
		else {
			MessageBox(L"此文件正常");
			return;
		}
		m_client.Close();
	}
}
void CKillVirusDlg::CheckPathOnLine()
{
	if (!m_BrowserPath.IsEmpty()) { //路径查杀
		TraverseCheckFilesOnLine(m_BrowserPath);
		MessageBox(L"检测完成");
	}
}

void CKillVirusDlg::TraverseCheckFilesOnLine(CString fullPath)
{

	CString strFileName;
	CString strFilePath;

	strFilePath = fullPath;
	strFilePath.Append(L"\\*");

	WIN32_FIND_DATA findFileData;
	//获取第一个文件/目录, 并获得查找句柄
	HANDLE hFileList = FindFirstFile(strFilePath, &findFileData);
	if (INVALID_HANDLE_VALUE == hFileList) {
		return;
	}
	do {
		strFileName = findFileData.cFileName;
		if (strFileName[0] == '.' || strFileName[0] == '..') {
			continue;
		}
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {// 目录
																	   //前端显示
			CString info;
			info.Format(L"%s", findFileData.cFileName);
			//获取完整路径
			CString strFullPath;
			strFilePath.TrimRight(L"\\*");
			strFullPath.Format(L"%s\\%s", strFilePath, strFileName);
			TraverseCheckFilesOnLine(strFullPath);
		}
		else { //文件
			//计算md5;
			CString strFullPath;
			strFullPath.Format(L"%s\\%s", fullPath, findFileData.cFileName);
			char chFullPath[256] = { 0 };
			WCHAR_TO_CHAR(strFullPath.GetBuffer(), chFullPath);
			char* md5 = md5FileValue(chFullPath);
			//服务器验证
			CString strTip;
			if (strlen(md5) == 0) {
				strTip = L"无法获取md5:";
				strTip.Append(strFullPath);
				MessageBox(strTip);
				continue;
			}

			m_client.Send(eSAMPLE, md5, 33);
			char* ret = m_client.Recv();
			if (!strcmp(ret, "是病毒!")) {
				strTip = L"发现病毒文件:";
				strTip.Append(strFullPath);
				MessageBox(strTip);
				continue;
				
			}
			else {
				//MessageBox(L"此文件正常");
				continue;
			}
			continue;
		}


	} while (FindNextFile(hFileList, &findFileData));
	//CloseHandle(hFileList);
	return;
}



void CKillVirusDlg::OnBnClickedButtonmd5save()
{
	WATCHINFO KillVirus[255] = { 0 };
	//md5;
	KillVirus[0].type = 1;
	strcpy_s(KillVirus[0].md5, "8fb950297242c65f4c9a3e79465efa16"); // 聊天客户端
	KillVirus[1].type = 1;
	strcpy_s(KillVirus[1].md5, "4916569400e1087f6da4560363695ea5"); // flv播放器
	KillVirus[2].type = 1;
	strcpy_s(KillVirus[2].md5, "04e0342a91c0bab33599fd1178c0d600"); // MD5本身
																	// path md5
	KillVirus[3].type = 2;
	strcpy_s(KillVirus[3].md5, "8fb950297242c65f4c9a3e79465efa16");
	strcpy_s(KillVirus[3].path, "C:\\Users\\yakun\\Desktop\\dll");
	KillVirus[4].type = 2;
	strcpy_s(KillVirus[4].md5, "8fb950297242c65f4c9a3e79465efa16");
	strcpy_s(KillVirus[4].path, "C:\\Users\\yakun\\Desktop");
	KillVirus[5].type = 2;
	strcpy_s(KillVirus[5].md5, "8fb950297242c65f4c9a3e79465efa16");
	strcpy_s(KillVirus[5].path, "C:\\Users\\yakun\\Desktop\\delete");
	//blacklist
	KillVirus[6].type = 3;
	strcpy_s(KillVirus[6].process, "Chatclient.exe");
	strcpy_s(KillVirus[7].process, "TIM.exe");
	strcpy_s(KillVirus[8].process, "WeChat.exe");


	
	FILE* pFile;
	errno_t ResRead = fopen_s(&pFile, "killVirus.txt", "wb");
	if (ResRead != 0) {
		MessageBox(L"打开文件失败");
		return;
	}
	//存入长度
	int nCount = _countof(KillVirus);
	fwrite(&nCount, sizeof(int),1, pFile);
	//存入md5数组信息
	fwrite(KillVirus, sizeof(WATCHINFO), nCount, pFile);
	fclose(pFile);
	
	//读取测试
	// FILE* pFile;
	errno_t Res2= fopen_s(&pFile, "killVirus.txt", "rb");
	if (Res2 != 0) {
		MessageBox(L"打开本地病毒库失败");
		return;
	}
	int nLength;
	fread_s(&nLength, sizeof(int), sizeof(int), 1, pFile);
	WATCHINFO *pInfo = new WATCHINFO[nLength];
	for (int i = 0; i < nLength; i++) {
		fread_s(&pInfo[i], sizeof(WATCHINFO), sizeof(WATCHINFO), 1, pFile);
	}
	fclose(pFile);

	

}



//
void CKillVirusDlg::OnBnClickedButtonkillByMd5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_BrowserFile.IsEmpty() && m_BrowserPath.IsEmpty()) {
		MessageBox(L"请确认查杀路径或文件");
		return;
	}
	if (!m_BrowserFile.IsEmpty()) { // 文件查杀
		CheckVirusOffLine(1);
	}
	else if (!m_BrowserPath.IsEmpty()) { //路径所有文件查杀
		CheckVirusOffLine(1);
	}
}


void CKillVirusDlg::OnBnClickedButtonkillByPath()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_BrowserFile.IsEmpty() && m_BrowserPath.IsEmpty()) {
		MessageBox(L"请确认查杀路径或文件");
		return;
	}
	if (!m_BrowserFile.IsEmpty()) { // 文件查杀
		CheckVirusOffLine(2);
	}
	else if (!m_BrowserPath.IsEmpty()) { //路径所有文件查杀
		CheckVirusOffLine(2);
	}
}


void CKillVirusDlg::OnBnClickedButtonkillByProcess()
{
	// TODO: 在此添加控件通知处理程序代码
	CheckVirusOffLine(3); //需要读取病毒库文件
	
}

// 1, md5 2. path, 3, process
void CKillVirusDlg::CheckVirusOffLine(int type)
{
	//读取文件
	FILE* pFile;
	errno_t Res2 = fopen_s(&pFile, "killVirus.txt", "rb");
	if (Res2 != 0) {
		MessageBox(L"打开本地病毒库失败");
		return;
	}
	int nLength;
	fread_s(&nLength, sizeof(int), sizeof(int), 1, pFile);
	WATCHINFO *pInfo = new WATCHINFO[nLength];
	for (int i = 0; i < nLength; i++) {
		fread_s(&pInfo[i], sizeof(WATCHINFO), sizeof(WATCHINFO), 1, pFile);
	}
	fclose(pFile);

	m_pVirusBase = pInfo;
	m_nVirtusBaseCount = nLength;   //病毒数量
	if (type == 3) {
		CheckProcessOffLine();
	}
	else if (!m_BrowserFile.IsEmpty()) { // 文件查杀

		CheckSingleFileOffLine(type);

	}
	else if (!m_BrowserPath.IsEmpty()) { //路径所有文件查杀
		CheckPathOffLine(type);
	}

}

void CKillVirusDlg::CheckSingleFileOffLine(int type)
{
		char chFullPath[256] = { 0 };
		WCHAR_TO_CHAR(m_BrowserFile.GetBuffer(), chFullPath);
		char* md5 = md5FileValue(chFullPath);
		if (strlen(md5) == 0) {
			MessageBox(L"无法获取md5");
			return;
		}
		//遍历病毒库
		for (int i = 0; i < m_nVirtusBaseCount; i++) {
			if (strcmp(m_pVirusBase[i].md5, md5) == 0) { //是病毒
				if (type == 2) { //全路径要检查路径
					//获取文件路径
					CString strPath;
					int n = m_BrowserFile.ReverseFind('\\');
					strPath = m_BrowserFile.Left(n);
					char chFilePath[256] = { 0 };
					WCHAR_TO_CHAR(strPath, chFilePath);
					if (m_pVirusBase[i].type == 2 && strcmp(m_pVirusBase[i].path, chFilePath)==0) {
						MessageBox(L"此文件是路径md5病毒");
						return;
					}
				}
				else { //仅仅md查杀
					MessageBox(L"此文件是md5病毒");
					return;
				}
			}

		}

		MessageBox(L"此文件正常");
		
}



void CKillVirusDlg::CheckPathOffLine(int type)
{
	TraverseCheckFilesOffLine(m_BrowserPath, type);
	MessageBox(L"检测完成");
}

void CKillVirusDlg::TraverseCheckFilesOffLine(CString fullPath, int type)
{
	CString strFileName;
	CString strFilePath;

	strFilePath = fullPath;
	strFilePath.Append(L"\\*");

	WIN32_FIND_DATA findFileData;
	//获取第一个文件/目录, 并获得查找句柄
	HANDLE hFileList = FindFirstFile(strFilePath, &findFileData);
	if (INVALID_HANDLE_VALUE == hFileList) {
		return;
	}
	do {
		strFileName = findFileData.cFileName;
		if (strFileName[0] == '.' || strFileName[0] == '..') {
			continue;
		}
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {// 目录
																	   //前端显示
			CString info;
			info.Format(L"%s", findFileData.cFileName);
			//获取完整路径
			CString strFullPath;
			strFilePath.TrimRight(L"\\*");
			strFullPath.Format(L"%s\\%s", strFilePath, strFileName);
			TraverseCheckFilesOffLine(strFullPath, type);
		}
		else { //文件
			   //计算md5;
			CString strFullPath;
			strFullPath.Format(L"%s\\%s", fullPath, findFileData.cFileName);
			char chFullPath[256] = { 0 };
			WCHAR_TO_CHAR(strFullPath.GetBuffer(), chFullPath);
			char* md5 = md5FileValue(chFullPath);
			//本地验证
			CString strTip;
			if (strlen(md5) == 0) {
				strTip = L"无法获取md5:";
				strTip.Append(strFullPath);
				MessageBox(strTip);
				continue;
			}
			//遍历病毒库
			for (int i = 0; i < m_nVirtusBaseCount; i++) {
				if (strcmp(m_pVirusBase[i].md5, md5) == 0) { //是病毒
					if (type == 2 && m_pVirusBase[i].type == 2) { //全路径要检查路径
									 //获取文件路径
						CString strPath;
						int n = strFullPath.ReverseFind('\\');
						strPath = strFullPath.Left(n);
						char chFilePath[256] = { 0 };
						WCHAR_TO_CHAR(strPath, chFilePath);
						if (m_pVirusBase[i].type == 2 && strcmp(m_pVirusBase[i].path, chFilePath) == 0) {
							strTip = L"发现路径病毒:";
							strTip.Append(strFullPath);
							MessageBox(strTip);
							break;
						}
					}
					else if(type == 1){ //仅仅md查杀
						strTip = "发现md5病毒:";
						strTip.Append(strFullPath);
						MessageBox(strTip);
						break;
					}
				}

			}
			
			continue;
		}


	} while (FindNextFile(hFileList, &findFileData));
	//CloseHandle(hFileList);
	return;

}
void CKillVirusDlg::CheckProcessOffLine()
{
	//添加进程遍历数据
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		MessageBox(L"无法获取进程快照");
		return ;
	}
	PROCESSENTRY32  pe32 = { sizeof(PROCESSENTRY32) };
	if (Process32First(hSnapshot, &pe32) != TRUE) {
		MessageBox(L"无法获取进程快照中的首个进程");
		CloseHandle(hSnapshot);
		return ;
	}
	do {
		CString  strExeFile;
		strExeFile = pe32.szExeFile;
		char chProcName[256] = { 0 };
		WCHAR_TO_CHAR(strExeFile, chProcName);
		//遍历病毒库
		for (int i = 0; i < m_nVirtusBaseCount; i++) {
			if (strcmp(chProcName, m_pVirusBase[i].process) == 0) {
				CString strTip = L"发现进程病毒:";
				strTip.Append(strExeFile);
				MessageBox(strTip);
			}
		}
			

	} while (Process32Next(hSnapshot, &pe32));


	CloseHandle(hSnapshot);
	MessageBox(L"检查完成");
}
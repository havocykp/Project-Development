
// SecurityGuard.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 关于权限的代码

BOOL    IsAdmin(HANDLE hProcess)
{
	HANDLE hToken = NULL;
	OpenProcessToken(hProcess, TOKEN_QUERY, &hToken);

	TOKEN_ELEVATION_TYPE tokenType = TokenElevationTypeDefault; // 用于接收令牌类型

	DWORD dwRetSize = 0; // 用于接收函数输出信息的字节数

						 // 2. 查询进程令牌中的权限提升值.( 这个值会记录当前的令牌是何种类型( 细节在17_权限管理_令牌的获取.cpp ) )
	GetTokenInformation(hToken,
		TokenElevationType,// 获取令牌的当前提升等级
		&tokenType,
		sizeof(tokenType),
		&dwRetSize // 所需缓冲区的字节数
	);


	// 根据令牌的类型来输出相应的信息
	if (TokenElevationTypeFull == tokenType) {
		// 3. 如果令牌是TokenElevationTypeFull , 则拥有至高无上的能力,可以给令牌添加任何特权
		printf("管理员账户,并拥有全部的权限,可以给令牌添加任何特权\n");
		return TRUE;
	}
	// 4. 如果是其他的, 则需要以管理员身份重新运行本进程. 这样就能以第三步的方法解决剩下的问题.
	else if (TokenElevationTypeDefault == tokenType) {
		printf("默认用户, 可能是一个普通用户, 可能是关闭UAC时登录的管理员用户\n");

		// 调用系统函数IsUserAnAdmin, 进一步确定是普通用户还是管理员用户
		return IsUserAnAdmin();
	}
	else if (TokenElevationTypeLimited == tokenType) {

		// 判断受限制的用户是管理员
		// 如果是管理员, 则这个令牌中会保存有管理员的SID

		// 1. 获取系统内键管理员用户的SID
		SID adminSid;
		DWORD dwSize = sizeof(adminSid);
		CreateWellKnownSid(WinBuiltinAdministratorsSid, // 获取SID的类型,这里是系统内键管理员
			NULL, // 传NULL,获取本地计算机的管理员
			&adminSid,// 函数输出的管理员SID
			&dwSize // 输入结构的大小,也作为输出
		);

		// 获取本令牌的连接令牌(受限制的令牌都会有一个连接的令牌,受限制的令牌正式由主令牌所创建的. )
		TOKEN_LINKED_TOKEN linkToken;
		GetTokenInformation(hToken,
			TokenLinkedToken, // 获取连接的令牌句柄
			&linkToken,
			sizeof(linkToken),
			&dwSize
		);

		// 在连接的令牌中查找是否具有管理员的SID
		BOOL    bIsContain = FALSE; // 用于保存是否包含.
		CheckTokenMembership(linkToken.LinkedToken, // 在这个令牌中检查
			&adminSid,             // 检查令牌中是否包含此SID
			&bIsContain);           // 输出TRUE则包含,反之不包含



		if (bIsContain) {
			printf("权限被阉割的受限制管理员账户, 部分权限被移处理\n");
		}


		return FALSE; // 不是以管理员权限运行
	}

	return FALSE;
}

// CApp

BEGIN_MESSAGE_MAP(CApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CApp 构造

CApp::CApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CApp 对象

CApp theApp;


// CApp 初始化

BOOL CApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//提权代码
	// 	HANDLE hToken;
	// 	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	// 	{
	// 		TOKEN_PRIVILEGES tp;
	// 		tp.PrivilegeCount = 1;
	// 		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	// 		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
	// 		{
	// 			AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	// 		}
	// 		CloseHandle(hToken);
	// 	}

	//另一个提权代码

// 	if (!IsAdmin(GetCurrentProcess())) {
// 
// 		// 以管理员身份运行本进程
// 		//  1 获取本进程的文件路径.
// 		TCHAR path[MAX_PATH] = { 0 }; // 需要初始化
// 		DWORD dwPathSize = MAX_PATH;
// 		QueryFullProcessImageName(GetCurrentProcess(), 0, path, &dwPathSize);
// 
// 		// 2 调用创建进程的API运行本进程.
// 		ShellExecute(NULL,            // 窗口句柄,没有则填NULL
// 			_T("runas"),   // 以管理员身份运行的重要参数
// 			path,            // 所有运行的程序的路径(这里是本进程)
// 			NULL,            // 命令行参数
// 			NULL,            // 新进程的工作目录的路径
// 			SW_SHOW           // 创建后的显示标志(最小化,最大化, 显示,隐藏等)
// 		);
// 
// 		// 退出本进程
// 		//exit(0);
// 		ExitProcess(0);
// 	}
// 
// 
// 	LUID privilegeLuid;
// 	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &privilegeLuid);
// 
// 	//获取本进程令牌
// 	HANDLE hToken = NULL;
// 	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
// 
// 	TOKEN_PRIVILEGES tokenPrivieges;
// 	// 使用特权的LUID来初始化结构体.
// 	tokenPrivieges.PrivilegeCount = 1; // 特权个数
// 	tokenPrivieges.Privileges[0].Luid = privilegeLuid; // 将特权LUID保存到数组中
// 	tokenPrivieges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;// 将属性值设为启用(有禁用,移除等其他状态)
// 
// 	AdjustTokenPrivileges(hToken,              // 要添加特权的令牌
// 		FALSE,               // TRUE是移除特权, FALSE是添加特权
// 		&tokenPrivieges,     // 要添加的特权数组
// 		sizeof(tokenPrivieges),// 整个特权数组的大小
// 		NULL,                // 旧的特权数组
// 		NULL                  // 旧的特权数组的长度
// 	);

	CMainDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


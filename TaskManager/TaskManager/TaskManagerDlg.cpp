
// TaskManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "TaskManagerDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ICON_NOTIFY WM_USER+100
BOOL g_bWillExit = FALSE;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTaskManagerDlg 对话框



CTaskManagerDlg::CTaskManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TASKMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTaskManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MANAGE, m_tabCtrl);
}

BEGIN_MESSAGE_MAP(CTaskManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MANAGE, &CTaskManagerDlg::OnTcnSelchangeTabManage)
END_MESSAGE_MAP()


// CTaskManagerDlg 消息处理程序


// 提取管理员权限
BOOL IsAdmin(HANDLE hProcess)
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

BOOL CTaskManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 托盘功能实现
	// 实现托盘功能：响应左键右键
	NOTIFYICONDATA m_tnid;

	m_tnid.cbSize = sizeof(NOTIFYICONDATA); // 设置结构大小
	m_tnid.hWnd = this->m_hWnd; // 设置图标对应的窗口
	m_tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; // 图标属性
	m_tnid.uCallbackMessage = WM_ICON_NOTIFY; // 应用程序定义的回调消息ID

	CString szToolTip;
	szToolTip = _T("Task 客户端程序");
	_tcscpy_s(m_tnid.szTip, szToolTip); // 帮助信息
	m_tnid.uID = IDR_MAINFRAME; // 应用程序图标
	m_tnid.hIcon = m_hIcon; // 图标句柄
	PNOTIFYICONDATA m_ptnid = &m_tnid;
	::Shell_NotifyIcon(NIM_ADD, m_ptnid); // 增加图标到系统盘

	//提权代码
// 	 	HANDLE hToken;
// 	 	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
// 	 	{
// 	 		TOKEN_PRIVILEGES tp;
// 	 		tp.PrivilegeCount = 1;
// 	 		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
// 	 		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
// 	 		{
// 	 			AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
// 	 		}
// 	 		CloseHandle(hToken);
// 	 	}

	//另一个提权代码

	if (!IsAdmin(GetCurrentProcess())) {

		// 以管理员身份运行本进程
		//  1 获取本进程的文件路径.
		TCHAR path[MAX_PATH] = { 0 }; // 需要初始化
		DWORD dwPathSize = MAX_PATH;
		QueryFullProcessImageName(GetCurrentProcess(), 0, path, &dwPathSize);

		// 2 调用创建进程的API运行本进程.
		ShellExecute(NULL,   // 窗口句柄,没有则填NULL
			_T("runas"),     // 以管理员身份运行的重要参数
			path,            // 所有运行的程序的路径(这里是本进程)
			NULL,            // 命令行参数
			NULL,            // 新进程的工作目录的路径
			SW_SHOW          // 创建后的显示标志(最小化,最大化, 显示,隐藏等)
		);

		// 退出本进程
		//exit(0);
		ExitProcess(0);
	}

	LUID privilegeLuid;
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &privilegeLuid);

	//获取本进程令牌
	HANDLE hToken = NULL;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	TOKEN_PRIVILEGES tokenPrivieges;
	// 使用特权的LUID来初始化结构体.
	tokenPrivieges.PrivilegeCount = 1; // 特权个数
	tokenPrivieges.Privileges[0].Luid = privilegeLuid; // 将特权LUID保存到数组中
	tokenPrivieges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;// 将属性值设为启用(有禁用,移除等其他状态)

	AdjustTokenPrivileges(hToken,              // 要添加特权的令牌
		FALSE,               // TRUE是移除特权, FALSE是添加特权
		&tokenPrivieges,     // 要添加的特权数组
		sizeof(tokenPrivieges),// 整个特权数组的大小
		NULL,                // 旧的特权数组
		NULL                  // 旧的特权数组的长度
	);


	// 设置Tab控件标签
	m_tabCtrl.InsertItem(0, _T("应用程序"));
	m_tabCtrl.InsertItem(1, _T("进程"));
	m_tabCtrl.InsertItem(2, _T("性能"));

	// 设置属性
	m_tabCtrl.SetPadding(19);
	m_tabCtrl.SetMinTabWidth(50);
	m_tabCtrl.SetItemSize(CSize(50, 25));

	// 将子对话框融合进Tab
	m_task.Create(IDD_PAGE_TASK, &m_tabCtrl);
	m_proc.Create(IDD_PAGE_PROCESS, &m_tabCtrl);
	m_chart.Create(IDD_PAGE_CHART, &m_tabCtrl);

	//	设定在Tab内显示的范围
	CRect rc;
	m_tabCtrl.GetClientRect(rc);
	rc.top += 25;
	// 	rc.bottom -= 8;
	// 	rc.left += 8;
	// 	rc.right -= 8;
	m_task.MoveWindow(&rc);
	m_proc.MoveWindow(&rc);
	m_chart.MoveWindow(&rc);

	// 把对话框对象指针保存起来
	m_pDialog[0] = &m_task;
	m_pDialog[1] = &m_proc;
	m_pDialog[2] = &m_chart;

	// 显示初始页面
	m_pDialog[0]->ShowWindow(SW_SHOW);
	m_pDialog[1]->ShowWindow(SW_HIDE);
	m_pDialog[2]->ShowWindow(SW_HIDE);

	// 保存当前选择
	m_CurSelTab = 0;

	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTaskManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTaskManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTaskManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 实现对话框的切换
void CTaskManagerDlg::OnTcnSelchangeTabManage(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	m_pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	m_CurSelTab = m_tabCtrl.GetCurSel();
	if (m_pDialog[m_CurSelTab]) {
		m_pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}


// 程序关闭后，自动移除托盘图标
void CTaskManagerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// 退出线程
	g_bWillExit = TRUE;

	NOTIFYICONDATA nd = { 0 };

	nd.cbSize = sizeof(NOTIFYICONDATA);
	nd.hWnd = m_hWnd;
	nd.uID = IDR_MAINFRAME;
	nd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nd.uCallbackMessage = WM_ICON_NOTIFY;
	nd.hIcon = m_hIcon;
	Shell_NotifyIcon(NIM_DELETE, &nd);

	CDialogEx::OnClose();
}

// 托盘响应功能实现
// 托盘：恢复界面
void CTaskManagerDlg::OnTrayRestore()
{
	// TODO: 在此添加命令处理程序代码

	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
	SetForegroundWindow();
}


void CTaskManagerDlg::OnTrayExit()
{
	// TODO: 在此添加命令处理程序代码
	OnClose();
}
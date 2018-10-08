
// ChatclientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Chatclient.h"
#include "ChatclientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatclientDlg 对话框
#define WM_ICON_NOTIFY WM_USER + 100


CChatclientDlg::CChatclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatclientDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strPass(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_strName, m_strName);
	DDX_Text(pDX, IDC_EDIT_strPass, m_strPass);
}

BEGIN_MESSAGE_MAP(CChatclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Anony, &CChatclientDlg::OnBtnProcAnony)
	ON_BN_CLICKED(IDC_BUTTON_Register, &CChatclientDlg::OnBtnProcRegister)
	ON_BN_CLICKED(IDC_BUTTON_Login, &CChatclientDlg::OnBtnProcLogin)
	ON_BN_CLICKED(IDC_CHECK_SAVEPWD, &CChatclientDlg::OnBnClickedCheckSavepwd)
	ON_BN_CLICKED(IDC_CHECK_AUTO, &CChatclientDlg::OnBnClickedCheckAuto)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_COMMAND(ID_TRAY_32777, &CChatclientDlg::OnTrayRestore)
	ON_COMMAND(ID_TRAY_32778, &CChatclientDlg::OnTrayExit)
	//ON_BN_CLICKED(IDC_BUTTON1, &CChatclientDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CChatclientDlg 消息处理程序

BOOL CChatclientDlg::OnInitDialog()
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

	// TODO:  在此添加额外的初始化代码
	NOTIFYICONDATA m_tnid;
	m_tnid.cbSize = sizeof(NOTIFYICONDATA);
	m_tnid.hWnd = this->m_hWnd;
	m_tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_tnid.uCallbackMessage = WM_ICON_NOTIFY;

	CString szToolTip;
	szToolTip = _T("聊天室 客户端程序");
	_tcscpy_s(m_tnid.szTip, szToolTip);
	m_tnid.uID = IDR_MAINFRAME;
	m_tnid.hIcon = m_hIcon;
	PNOTIFYICONDATA m_ptnid = &m_tnid;
	::Shell_NotifyIcon(NIM_ADD, m_ptnid); // 增加系统托盘

	// 背景图片
	m_BKGround.LoadBitmapW(IDB_BITMAP_WINMAIN);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
	if (nID == SC_MINIMIZE)
	{
		ShowWindow(FALSE); // 隐藏窗口
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChatclientDlg::OnPaint()
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

		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP_WINMAIN);
		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap *pbmpOld = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
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
HCURSOR CChatclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatclientDlg::OnBtnProcAnony()
{
	//TODO:  在此添加控件通知处理程序代码
	if (!m_client.ConnectSrver("127.0.0.1", 1234))
	{
		MessageBox(L"连接服务器失败！", L"Error!", MB_OK | MB_ICONWARNING);
		return;
	}

	// 隐藏登录窗口
	ShowWindow(SW_HIDE);
	// 显示匿名聊天窗口
	CDlgChatMain dlgChat(&m_client);   // 调用自定义的构造函数

	dlgChat.DoModal();
	m_client.Close();
	// 退出
	CChatclientDlg::OnClose();
}


void CChatclientDlg::OnBtnProcRegister()
{
	// TODO:  在此添加控件通知处理程序代码
	// 获取用户名和密码
	UpdateData(TRUE);

	if (m_strName.IsEmpty() || m_strPass.IsEmpty())
	{
		MessageBox(L"用户名/密码不能为空!");
		return;
	}

	if (!m_client.ConnectSrver("127.0.0.1", 1234))
	{
		MessageBox(L"连接服务器失败!", L"Error!", MB_OK | MB_ICONWARNING);
		return;
	}

	// 注册用户名:密码
	CString strSend = m_strName;
	strSend += L":" + m_strPass;

	CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	m_client.Send(REGISTER, str.GetBuffer(), str.GetLength() + 1);

	// 直接等待注册结果
	char* ret = m_client.Recv();
	if (ret == nullptr)
	{
		MessageBox(L"注册失败!");
	}
	else
	{
		MessageBox(L"注册成功！");
	}

	m_client.Close();

	return;
}


void CChatclientDlg::OnBtnProcLogin()
{
	// TODO:  在此添加控件通知处理程序代码
	// 获取用户名和密码
	UpdateData(TRUE);
	if (m_strName.IsEmpty() || m_strPass.IsEmpty())
	{
		MessageBox(L"用户名/密码不能为空!");
		return;
	}

	if (!m_client.ConnectSrver("127.0.0.1", 1234))
	{
		MessageBox(L"连接服务器失败!", L"Error", MB_OK | MB_ICONWARNING);
		return;
	}

	// 登录用户名: 密码
	CString strSend = m_strName;
	strSend += L":" + m_strPass;
	CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	m_client.Send(LOGIN, str.GetBuffer(), str.GetLength() + 1);
	char* ret = m_client.Recv();

	if (ret == nullptr)
	{
		MessageBox(L"登录失败!");
		m_client.Close();
		return;
	}

	// 登录成功
	// 设置当前登录用户名,否则显示的是个随机名称
	CStringA strShowName = CW2A(m_strName.GetBuffer(), CP_THREAD_ACP);
	strcpy_s(m_client.m_szName, strShowName.GetBuffer());
	// 隐藏登录窗口
	ShowWindow(SW_HIDE);

	// 显示聊天窗口
	CDlgChatMain dlgChat(&m_client);
	dlgChat.m_bLogin = TRUE;   // 非匿名模式
	dlgChat.DoModal();
	m_client.Close();

	CChatclientDlg::OnClose();
}


void CChatclientDlg::OnBnClickedCheckSavepwd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (!m_bSavePwd)
	{
		if (m_bAuto)
		{
			m_bAuto = FALSE;
			UpdateData(FALSE);
		}
	}
}


void CChatclientDlg::OnBnClickedCheckAuto()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bAuto)
	{
		if (!m_bSavePwd)
		{
			m_bSavePwd = TRUE;
			UpdateData(FALSE);
		}
	}
}


void CChatclientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

LRESULT CChatclientDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_LBUTTONDOWN:
	{
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
		SetForegroundWindow();
		break;
	}
	case WM_RBUTTONUP:
	{
		POINT point;
		HMENU hMenu, hSubMenu;
		GetCursorPos(&point); // 鼠标位置
		hMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU_TRAY));
		hSubMenu = GetSubMenu(hMenu, 0);
		SetForegroundWindow();

		TrackPopupMenu(hSubMenu, 0, point.x, point.y, 0, m_hWnd, NULL);
	}
	}
	return 1;
}


void CChatclientDlg::OnTrayRestore()
{
	// TODO: 在此添加命令处理程序代码
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
	SetForegroundWindow();
}


void CChatclientDlg::OnTrayExit()
{
	// TODO: 在此添加命令处理程序代码
	OnClose();
}


// void CChatclientDlg::OnBnClickedButton1()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	//ShellExecute(0, _T("open"), _T("Notepad++"), _T("D:\\Notepad++"), _T(""), SW_SHOW);
// }

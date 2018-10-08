// DlgChatMain.cpp : 实现文件
//

#include "stdafx.h"
#include "Chatclient.h"
#include "DlgChatMain.h"
#include "afxdialogex.h"


// CDlgChatMain 对话框

IMPLEMENT_DYNAMIC(CDlgChatMain, CDialogEx)

CDlgChatMain::CDlgChatMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgChatMain::IDD, pParent)
	, m_strShow(_T(""))
	, m_strSend(_T(""))
{

}

CDlgChatMain::~CDlgChatMain()
{
}

void CDlgChatMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_QULIAO, m_strShow);
	DDX_Text(pDX, IDC_EDIT_FASONG, m_strSend);
	DDX_Control(pDX, IDC_LIST_Anonymous, m_listName);
}

void CDlgChatMain::ChatForOne2One(CHATONE2ONE& objOne2One)
{
	CString strName(objOne2One.szName);
	CString strContent(objOne2One.szContent);

	if (m_map.find(strName) == m_map.end())
	{
		// 创建私聊窗口
		CDlgOne2One* pDlg = new CDlgOne2One;
		pDlg->Create(IDD_DLGONE2ONE, this);
		pDlg->SetWindowTextW(strName.GetBuffer());
		m_map[strName] = pDlg;
		pDlg->m_strShow += strName + L":" + strContent;
		pDlg->m_strShow += "\r\n";
		pDlg->UpdateData(FALSE);
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		CDlgOne2One* pDlg = (CDlgOne2One*)m_map[strName];
		pDlg->UpdateData(TRUE);
		pDlg->m_strShow += strName + L":" + strContent;
		pDlg->m_strShow += "\r\n";
		pDlg->UpdateData(FALSE);
		pDlg->ShowWindow(SW_SHOW);
	}

}

BEGIN_MESSAGE_MAP(CDlgChatMain, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CDlgChatMain::OnBtnProcSend)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_Anonymous, &CDlgChatMain::OnNMRDblclkList1)
	ON_COMMAND(ID_AddFriend, &CDlgChatMain::OnAddfriend)
	ON_COMMAND(ID_Search, &CDlgChatMain::OnSearch)
	ON_COMMAND(ID_GetChatRecord, &CDlgChatMain::OnGetchatrecord)
	ON_WM_TIMER()
	ON_MESSAGE(WM_MYSOCKET, &CDlgChatMain::OnMysocket)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_Anonymous, &CDlgChatMain::OnNMRClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_Anonymous, &CDlgChatMain::OnNMDblcList1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgChatMain 消息处理程序

CDlgChatMain::CDlgChatMain(CclientSocket* pClient, CWnd* pParent /*= NULL*/)
	:CDialogEx(IDD_DLGANONYMOUS, pParent), m_pClient(pClient)
{

}

BOOL CDlgChatMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 设置窗口标题为用户昵称
	// 匿名登录时m_pClient->m_szName是个随机值，构造函数内初始化
	// 登录的时候是登录用户名，点登录按钮的时候初始化
	CString strTitle(m_pClient->m_szName);
	SetWindowText(strTitle.GetBuffer());
	// 设置窗口任务栏可见
	ModifyStyleEx(0, WS_EX_APPWINDOW);
	// 在线用户列表
	m_listName.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listName.InsertColumn(0, L"在线列表", 0, 100);
	// 使用消息选择模型绑定socket和当前对话框
	// 1.define 自定义消息WM_SOCKET
	// 2.类向导添加自定义消息(WM_SOCKET)响应
	// 3.WSAAsynSelect绑定socket、窗口和消息
	WSAAsyncSelect(m_pClient->m_sClient, m_hWnd, WM_MYSOCKET, FD_READ | FD_CLOSE);
	// 告诉服务器自己的昵称
	m_pClient->Send(ANONYMOUS, m_pClient->m_szName, strlen(m_pClient->m_szName) + 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDlgChatMain::OnBtnProcSend()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strSend.IsEmpty())
	{
		// 如果发送的编辑框内是空,就不发送
		return;
	}
	CStringA str = CW2A(m_strSend.GetBuffer(), CP_THREAD_ACP);
	m_pClient->Send(CHAT, str.GetBuffer(), str.GetLength() + 1);
	// 显示在自己的对话框上
	m_strShow += "你说:";
	m_strShow += m_strSend;
	m_strShow += "\r\n";

	// 把发送内容置空
	m_strSend.Empty();
	UpdateData(FALSE);
}


void CDlgChatMain::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void CDlgChatMain::OnNMRDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	// 双击的空白区域
	if (pNMItemActivate->iItem == -1)
		return;
	// 新建1V1聊天对话框
	CDlgOne2One* pDlg = new CDlgOne2One;
	pDlg->Create(IDD_DLGONE2ONE, this);   //把当前窗口指定为私聊窗口的父窗口

	// 设置窗口标题为要聊天的目标用户名称
	CString strTitle = m_listName.GetItemText(pNMItemActivate->iItem, 0);
	pDlg->SetWindowTextW(strTitle.GetBuffer());

	// 把该私聊窗口添加到自己的私聊MAP里
	m_map[strTitle] = pDlg;

	pDlg->ShowWindow(SW_SHOW);
}


void CDlgChatMain::OnAddfriend()
{
	// TODO:  在此添加命令处理程序代码
	CString strFriend = m_listName.GetItemText(m_dwNameIndex, 0);

	// 当前用户:要添加用户
	CString strUserFrd;
	GetWindowText(strUserFrd);
	strUserFrd += L":";
	strUserFrd += strFriend;
	CStringA strSend = CW2A(strUserFrd.GetBuffer(), CP_THREAD_ACP);
	m_pClient->Send(ADDFRIEND, strSend.GetBuffer(), strSend.GetLength() + 1);
}


void CDlgChatMain::OnSearch()
{
	// TODO:  在此添加命令处理程序代码
	CDlgSearch dlgSearch;
	dlgSearch.DoModal();
	if (dlgSearch.m_strSearch.IsEmpty()) return;  // 搜索对话框的搜索编辑框的内容
	CStringA strSearch = CW2A(dlgSearch.m_strSearch.GetBuffer(), CP_THREAD_ACP);
	m_pClient->Send(SEARCHUSER, strSearch.GetBuffer(), strSearch.GetLength() + 1);
}


void CDlgChatMain::OnGetchatrecord()
{
	// TODO:  在此添加命令处理程序代码
	m_pClient->m_hEvent = CreateEventW(NULL, FALSE, TRUE, L"Event1");
	// 防止上次的获取消息记录还未返回就重新申请,设置事件对象同步
	DWORD dwRet = WaitForSingleObject(m_pClient->m_hEvent, 100);
	if (dwRet == WAIT_FAILED || dwRet == WAIT_TIMEOUT) return;
	m_pClient->Send(MSGRECORD, NULL, NULL);
	m_pClient->m_vecMsgRecord.clear();
	// 设置定时器等待消息返回
	SetTimer(0x1001, 1000, NULL);
}


void CDlgChatMain::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == 0x1001)
	{
		// 等待消息记录返回,消息记录会把事件对象置为空true
		DWORD dwRet = WaitForSingleObject(m_pClient->m_hEvent, 100);
		if (dwRet == WAIT_OBJECT_0)
		{
			// 显示新窗口
			if (m_pDlgRecord)
			{
				// 更新数据
				m_pDlgRecord->UpdateList();
				// 显示窗口
				m_pDlgRecord->ShowWindow(SW_SHOW);
				KillTimer(nIDEvent);
				SetEvent(m_pClient->m_hEvent);
			}
			else
			{
				m_pDlgRecord = new CDlgRecord;
				m_pDlgRecord->Create(IDD_DIALOGRECORD, this);
				m_pDlgRecord->ShowWindow(SW_SHOW);
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT CDlgChatMain::OnMysocket(WPARAM wParam, LPARAM lParam)
{
	// wparam 是socket
	// lparam 低word是事件
	// lparam 高word是错误码
	SOCKET s = wParam;
	WORD wEvent = WSAGETSELECTEVENT(lParam);
	WORD wErrorCode = WSAGETSELECTERROR(lParam);

	// 先判断是否有网络错误事件发生,有则跳过
	if (wErrorCode)
	{
		exit(0);
		MessageBox(L"网络错误! ");
		CDialogEx::OnClose();
	}

	switch (wEvent)
	{
	case FD_READ:
	{
		char* szRecv = m_pClient->Recv();
		if (szRecv == nullptr)
		{
			if (m_pClient->m_pObjUpdate)
			{
				// 更新用户列表用到
				// 判断用户是加入还是退出
				// RecvForUpdateUserlist函数返回处理
				InsertOrDeleteUser(*m_pClient->m_pObjUpdate);
				delete m_pClient->m_pObjUpdate;
				m_pClient->m_pObjUpdate = nullptr;
			}
			else if (m_pClient->m_pObjOne20ne)
			{
				// 1V1聊天用到
				// RecvForOne2One函数返回处理
				ChatForOne2One(*m_pClient->m_pObjOne20ne);
				delete m_pClient->m_pObjOne20ne;
				m_pClient->m_pObjOne20ne = nullptr;
			}
			return 0;
		}
		UpdateData(TRUE);
		m_strShow += szRecv;
		m_strShow += "\r\n";
		UpdateData(FALSE);
	}
	default:
		break;
	}

	return 0;
}

void CDlgChatMain::InsertOrDeleteUser(CHATUPDATEUSER& objUpdate)
{
	m_listName.InsertItem(0, CA2W(objUpdate.buf));
}

void CDlgChatMain::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	// 匿名聊天右键失败
	if (!m_bLogin) return;
	// 当前点击的项,用于获得添加的是哪个好友
	m_dwNameIndex = pNMItemActivate->iItem;
	// 弹出右键菜单
	CMenu mu;
	mu.LoadMenuW(IDR_MENU1);
	CMenu* pSub = mu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	pSub->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this, NULL);
}


void CDlgChatMain::OnNMDblcList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	// 双击的空白区域
	if (pNMItemActivate->iItem == -1)
		return;
	// 新建1V1聊天对话框
	CDlgOne2One* pDlg = new CDlgOne2One;
	pDlg->Create(IDD_DLGONE2ONE, this);   //把当前窗口指定为私聊窗口的父窗口

	// 设置窗口标题为要聊天的目标用户名称
	CString strTitle = m_listName.GetItemText(pNMItemActivate->iItem, 0);
	pDlg->SetWindowTextW(strTitle.GetBuffer());

	// 把该私聊窗口添加到自己的私聊MAP里
	m_map[strTitle] = pDlg;
	pDlg->ShowWindow(SW_SHOW);
}


void CDlgChatMain::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
	PostQuitMessage(WM_QUIT);
}


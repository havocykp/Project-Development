#pragma once
#include <vector>

#define BUFNAME 50 // 姓名
#define BUFMSG 2048 // 一次接收信息的最大字节数

using namespace std;

enum CHATPURPOSE
{
	ANNOYMOUS = 1,
	CHAT,
	ONE2ONE,
	REGISTER,
	LOGIN,
	ADDFRIEND,
	SEARCHUSER,
	FILETRANS,
	MSGRECORD,
	UPDATEUSER
};

// 聊天内容
// 匿名的结构体
typedef struct _CHATANNOYMOUS
{
	DWORD dwLen; // 匿名名称长度
	char buf[50]; // 匿名
}CHATANNOYMOUS;

// 聊天的结构体
typedef struct _CHATCHAT
{
	DWORD dwLen; // 聊天内容长度
	char buf[1024];
}CHATCHAT;

// 1v1结构体
typedef struct _CHATONE2ONE
{
	CHAR szName[50];
	CHAR szContent[1024];
}CHATONE2ONE;

// 注册结构体
typedef struct _CHATREGISTER
{
	CHAR szName[50];
	CHAR szPwd[50];
}CHATREGISTER;

// 登录结构体，服务器返回结果放在szName里
typedef struct _CHATLOGIN
{
	CHAR szName[50];
	CHAR szPwd[50];
}CHATLOGIN;

// 添加好友结构体
typedef struct _CHATADDFRIEND
{
	CHAR szName[50]; // 自己的姓名
	CHAR szFrd[50]; // 要添加好友姓名
}CHATADDFRIEND;

// 搜索用户结构体
typedef struct _CHATSEARCHUSER
{
	CHAR szName[50]; // 要搜索的用户名
}CHATSEARCHUSER;

// 文件传输结构体
typedef struct _CHATFILETRANS
{

}CHATFILETRANS;

// 查看聊天记录结构体
typedef struct _CHATMSGRECORD
{
	CHAR szFrom[50];
	CHAR szTo[50];
	CHAR szContent[1024];
}CHATMSGRECORD;

// 更新在线用户列表
typedef struct _CHATUPDATAUSER
{
	DWORD bAdd; // 是加入还是退出
	DWORD dwLen; // 一次只接收1个用户名
	CHAR buf[50]; 
}CHATUPDATAUSER;

// 发送内容
typedef struct _CHATSEND
{
	CHATPURPOSE m_type;
	union {
		char buf[BUFMSG];
		CHATANNOYMOUS any;
		CHATCHAT chat;
		CHATONE2ONE o2o;
		CHATREGISTER reg;
		CHATLOGIN log;
		CHATADDFRIEND adf;
		CHATSEARCHUSER seu;
		CHATFILETRANS trs;
		CHATMSGRECORD rec;
		CHATUPDATAUSER upd;
	}m_content;
}CHATSEND;


class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();
	// 初始化套接字，并尝试连接服务器
	bool ConnectServer(char* szIp, WORD port);
	// 负责创建线程用来接收服务器信息
	char* Recv();
	bool Send(CHATPURPOSE purpose, char* bufSend, DWORD dwLen);
	bool Close();

private:
	// 接收函数
	char* RecvForAnonumous();
	char* RecvForChat();
	char* RecvForUpdateUserlist();
	char* RecvForOne2One();
	char* RecvForRegister();
	char* RecvForLogin();
	char* RecvForAddFriend();
	char* RecvForSearchUser();
	char* RecvForGetMsgRecord();

	// 发送函数
	void SendForAnnoymous(char* bufSend, DWORD dwLen);
	void SendForChat(char* bufSend, DWORD dwLen);
	void SendForOne2One(char* bufSend, DWORD dwLen);
	void SendForRegister(char* bufSend, DWORD dwLen);
	void SendForLogin(char* bufSend, DWORD dwLen);
	void SendForAddFriend(char* bufSend, DWORD dwLen);
	void SendForSearchUser(char* bufSend, DWORD dwLen);
	void SendForGetMsgRecord(char* bufSend, DWORD dwLen);

public:
	// 保存聊天记录
	HANDLE m_hEvent = 0;
	vector<CHATMSGRECORD> m_vecMsgRecord;
	// 客户端套接字
	SOCKET m_sClient = NULL; 
	// 新用户加入或退出聊天室的时候用到 
	CHATUPDATAUSER *m_pObjUpdate = nullptr;
	// 1v1聊天的时候
	CHATONE2ONE *m_pObjOne2One = nullptr;
	// 当前客户端的用户名
	char m_szName[BUFNAME] = {};
	// 收消息的时候用到
	CHATSEND * m_pObjChatRecv;

private:
	char m_bufRecv[BUFMSG] = {};
};


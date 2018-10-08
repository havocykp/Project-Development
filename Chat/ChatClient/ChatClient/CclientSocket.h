#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

#define  _WINSOCK_DEPRECATED_NO_WARNINGS
// 协议设计
#define BUFMSG 2048  // 一次接受消息的最大字节数
#define BUFCHAT 1024//聊天内容最大长度
#define BUFNAME 50 //名字最大字节数
// 头部:通讯目的,包括匿名登录,聊天、1V1聊天、注册、登录、添加好友、搜索好友、文件传输、查看聊天记录

enum CHATPURPOSE{
	ANONYMOUS = 1,
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
typedef struct _CHATANONYMOUS{
	DWORD dwLen;    // 匿名名称长度
	char buf[50];   // 匿名
}CHATANONYMOUS;

//聊天的结构体
typedef struct _CHATCHAT{

	DWORD dwLen;   // 聊天内容长度
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
	char szPwd[50];
}CHATREGISTER;

//登录结构体，服务器返回结果放在szName里
typedef struct _CHATLOGIN
{
	CHAR szName[50];
	CHAR szPwd[50];
}CHATLOGIN;

// 添加好友结构体
typedef struct _CHATADDFRIEND
{
	CHAR szName[50];   // 自己姓名
	CHAR szFrd[50];    // 要添加姓名好友
}CHATADDFRIEND;

// 搜索用户结构体
typedef struct _CHATSEARCHUSER
{
	CHAR szName[50];   // 要搜索的用户名

}CHATSEARCHUSER;


//文件传输结构体
typedef struct _CHATFILETRANS
{
	CHAR szName[BUFNAME];
	CHAR szContent[BUFCHAT];
}CHATFILETRANS;

// 查看聊天记录结构体
typedef struct _CHATMSGRECORD
{
	CHAR szFrom[50];
	CHAR szTo[50];
	CHAR szContent[1024];
}CHATMSGRECORD;

// 更新在线用户列表
typedef struct _CHATUPDATEUSER
{
	DWORD bAdd;   // 是加入还是退出
	DWORD dwLen;  // 一次只接收1个用户名
	CHAR buf[50];
}CHATUPDATEUSER;

// 发送内容
typedef struct _CHATSEND
{
	CHATPURPOSE m_type;

	union{
		char buf[BUFMSG];
		CHATANONYMOUS any;    // 匿名
		CHATCHAT chat;        // 聊天
		CHATONE2ONE o2o;     // 1V1
		CHATREGISTER reg;     // 注册
		CHATLOGIN log;        // 登录
		CHATADDFRIEND adf;    // 添加好友
		CHATSEARCHUSER seu;   // 搜索好友
		CHATFILETRANS  trs;   // 文件传输
		CHATMSGRECORD  rec;   // 聊天记录
		CHATUPDATEUSER upd;   // 更新用户列表
	}m_content;
}CHATSEND;


class CclientSocket
{
#define BUFNAME 50 // 姓名
public:
	CclientSocket();
	~CclientSocket();
	// 初始化客户端套接字，并尝试连接服务器
	bool ConnectSrver(char* szIp, WORD port);

	// 负责创建线程用来接收服务器的信息
	char* Recv();
	bool Send(CHATPURPOSE purpose, char* bufSend, DWORD dwLen, SOCKET client = NULL);
	bool Close();
	fstream ReceiveFile;
private:
	// recv funciton
	char* RecvForAnonumous();
	char* RecvForChat();
	char* RecvForUpdateUserlist();
	char* RecvForOne2One();
	char* RecvForRegister();
	char* RecvForLogin();
	char* RecvForAddFriend();
	char* RecvForSearchUser();
	char* RecvForGetMsgRecord();
	char* RecvForFiletrans();

	// send function
	void SendForAnonymous(char* bufSend, DWORD dwLen);
	void SendForChat(char* bufSend, DWORD dwLen);
	void SendForOne2One(char* bufSend, DWORD dwLen);
	void SendForRegister(char* bufSend, DWORD dwLen);
	void SendForLogin(char* bufSend, DWORD dwLen);
	void SendForAddFriend(char* bufSend, DWORD dwLen);
	void SendForSearchUser(char* bufSend, DWORD dwlen);
	void SendForGetMsgRecord(char* bufSend, DWORD dwLen);
	void SendForFiletrans(char* bufSend, DWORD dwLen, SOCKET client);
	

public:
	// 保存聊天记录用
	HANDLE m_hEvent = 0;
	vector<CHATMSGRECORD> m_vecMsgRecord;
	// 客户端套接字
	SOCKET m_sClient = NULL;
	// 新用户加入或退出聊天室的时候用到
	CHATUPDATEUSER* m_pObjUpdate = nullptr;
	// 1V1 聊天的时候
	CHATONE2ONE* m_pObjOne20ne = nullptr;
	// 当前客户端的用户名
	char m_szName[BUFNAME] = {};
	// 收信息的时候用到
	CHATSEND* m_pObjChatRecv;

private:
	char m_bufRecv[BUFMSG];
};




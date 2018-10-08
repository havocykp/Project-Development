#pragma once
#include "Data.h"
#include <vector>
using std::vector;
#define BUFNAME 50


class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();
public:
	//初始化客户端套接字, 并尝试连接服务器
	bool ConnectServer(char* szIp, WORD port);
	//负责创建线程用来接收服务器信息
	char* Recv();
	bool Send(INTERACTPURPOSE purpose, char* bufSend, DWORD dwLen);
	bool Close();

private:
	// send
	void SendForSampleMd5(char* bufSend, DWORD dwLen);
	// recv
	char* RecvForSampleMd5();
public:
	//客户端套接字
	SOCKET m_sClient = NULL;
	//收信息的时候用
	CONTENTSEND* m_pObjRecv;

private:

	char m_bufRecv[BUFMSG] = {};


};


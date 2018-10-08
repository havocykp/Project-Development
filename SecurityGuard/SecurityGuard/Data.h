#pragma once
//协议
#define BUFMSG 2048 //一次性接收的最大字节数
#define SERVERIP  "127.0.0.1"
#define SERVERPORT  1234

//头部: 交互通讯目的, 包括 样本信息md5信息
enum INTERACTPURPOSE {
	eSAMPLE = 1
};


//样本信息MD5的结构体
typedef struct _SAMPLEMD5 {
	char szMD5[32]; // md5值
} SAMPLEMD5;

//发送内容
typedef struct _CONTENTSEND {
	INTERACTPURPOSE  m_type;
	union {
		char buf[BUFMSG];
		SAMPLEMD5  sample;
	} m_content;
} CONTENTSEND;


typedef struct _WATCHINFO {
	int type; // 1 md5  ; 2  path md5;  3; process;
	char md5[33];
	char path[256];
	char process[256];
} WATCHINFO;
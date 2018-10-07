#pragma once
//自定义数据类型
//定义所有的用户操作为枚举
#define 查看所有    0
#define 新增        1
#define 删除        2
#define 修改        3
#define 查询        4
#define 保存文本    5
#define 退出        6

//键盘按键
enum 
{
	UP = 72,
	DOWN = 80,
	ENTER = 13,
	ESC = 27

};

//定义存储账号密码的结构体
typedef struct _PASSWORD
{
	char webSite[100]; //站点名
	char userName[100]; //帐户名
	char passWord[100]; //密码
	char note[100]; //备注
}PASSWORD, *PPASSWORD;

//全局变量，常量的声明
#define NUM 100   //初始密码本容量
extern int g_nNum;  //当前密码本容量
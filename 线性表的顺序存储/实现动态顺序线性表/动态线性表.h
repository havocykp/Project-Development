#pragma once
#include <sal.h>

typedef int Opercode;  // 定义操作类型
#define OR_SUCCESS  1  // 操作成功
#define OR_BUFFER_IS_FULL -1   // 缓冲区已经满了
#define OR_BUFFER_IS_EMPTY -2  // 缓冲区为空
#define OR_ERROR           -3  // 未知错误
#define OR_INDEX_IS_WRONG  -4  // 索引值有误
#define OR_MEM_ERROR       -5  // 分配内存失败
#define INIT_SIZE 100  // 线性表初始长度
#define MAX_SIZE  9999 // 线性表最大长度

class CMyVector
{
public:
	CMyVector();  // 构造函数
	~CMyVector(); // 析构函数

	// 初始化，建立线性表
	bool initVector(_Out_ int& nError);
	// 判断线性表是否为空
	bool isEmpty(_Out_ int& nError);
	// 判断线性表是否满
	bool isFull(_Out_ int& nError);
	// 清空线性表
	bool clearVector(_Out_ int& nError);
	// 按下标获取元素
	bool getEleByIndex(_In_ int nPos, _Out_ int& nEle, _Out_ int& nError);
	// 按元素值获取下标
	bool getEleByValue(_Out_ int& nPos, _In_ int& nEle, _Out_ int& nError);
	// 插入元素
	bool insertEleByIndex(_In_ int nPos, _In_ int nEle, _Out_ int& nError);
	// 按位置删除元素
	bool delEleByIndex(_In_ int nPos, _Out_ int& nEle, _Out_ int& nError);
	// 按元素值删除元素
	bool delEleByValue(_In_ int nEle, _Out_ int& nError);
	// 返回当前长度
	bool getLength(_Out_ int& nLenth, _Out_ int& nError);
	// 返回最大长度
	bool max_size(_Out_ int& nLenth, _Out_ int& nError);
	// 在尾部插入
	bool push_back(_In_ int nEle, _Out_ int& nError);
	// 弹出尾部元素
	bool pop_back(_Out_ int& nEle, _Out_ int& nError);
	// 在下标处插入元素
	bool insert(_In_ int pos, _In_ int elem, _Out_ int& nError);
	// 在下标处插入多个元素
	bool insert(_In_ int pos, _In_ int n, _In_ int elem, _Out_ int& nError);
	// 在开始坐标与终点坐标之间插入多个元素
	bool insertEx(_In_ int beg, _In_ int end, _In_ int elem, _Out_ int& nError);
	// 返回指定位置的元素
	bool at(_In_ int idx, _Out_ int& elem, _Out_ int& nError);
	// 重载[]符号
	int operator[](_In_ int pos);

private:
	// 重新设置vector的大小
	bool reSize(_In_ int& nError);


private:
	int *m_nBuf;
	int m_nLenth;
	int m_nSize;
};
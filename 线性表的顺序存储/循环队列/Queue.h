#pragma once

typedef int ElemType; //数据元素的类型
const int MAXSIZE = 100; //数组的容量

// 循环队列
class Queue
{
public:
	Queue(); // 构造函数
	~Queue(); // 析构函数
	int Empty(); // 判断循环队列是否为空
	void Display(); // 输出队列
	void Add(ElemType x); // 进队
	ElemType Del();  // 出队
	ElemType GetFront(); // 取队首元素

private:
	ElemType elem[MAXSIZE];
	int front; // 头指针
	int rear; // 尾指针
};


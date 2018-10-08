#pragma once

typedef int ElemType; //数据元素的类型
const int MAXSIZE = 100; //数组的容量

class Stack
{
public:
	Stack(); // 构造函数
	~Stack(); // 析构函数
	void SetEmpty(); // 置已有的栈为空栈
	int IsEmpty();  // 判断栈是否为空
	void push(ElemType e); // 进栈
	ElemType pop();  // 出栈
	void PrintOut(); // 输出栈中数据元素
	//ElemType Getpop(); // 取栈顶元素数据

private:
	ElemType elem[MAXSIZE];
	int top;
};


#include "stdafx.h"
#include "Stack.h"
#include <iostream>
#include <iomanip>

using namespace std;

/* 构造函数 */
Stack::Stack()
{
	top = 0; // 初始化一个空栈
}

/* 析构函数 */
Stack::~Stack()
{

}

/* 置已有的栈为空栈 */
void Stack::SetEmpty()
{
	top = 0;
}

/* 判断栈是否为空 */
int Stack::IsEmpty()
{
	if (top == 0)
		return 1;
	else
		return 0;
}

/* 入栈 */
void Stack::push(ElemType e)
{
	if (top == MAXSIZE - 1) {
		cout << "\n栈满溢出" << endl;
	}
	else{
		top++;
		elem[top] = e; //数据元素e进栈
	}
}

/* 出栈 */
ElemType Stack::pop()
{
	ElemType x;
	if (top == 0) {
		cout << "\n 栈为空，不能进行出栈操作" << endl;
		x = 0; //表示未曾出栈
	}
	else {
		x = elem[top]; // 出栈
		top--;
	}
	return x;
}

/* 输出栈中数据元素 */
void Stack::PrintOut()
{
	// 输出函数不可改变top
	cout << "\n PrintOut Data:\n";
	if (top == 0) {
		cout << "\n已是空栈！";
		if (IsEmpty()) {
			cout << "\n已是空栈";
		}
	}
	else {
		for (int k = top; k >= 1; --k) {
			cout << setw(6) << elem[k];
			cout << endl;
		}
	}
}

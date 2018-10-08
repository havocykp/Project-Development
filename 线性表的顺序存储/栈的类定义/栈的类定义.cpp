// 栈的类定义.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Stack.h"
#include <iostream>

using namespace std;

int main()
{
	ElemType e, x;
	Stack s; //声明和创建对象s，初始化空栈
	cout << "\n  顺序表存储结构演示";
	cin >> e;
	s.push(e);  //输入数据，将其入栈

	s.PrintOut(); //输出栈当前内容
	x = s.pop(); //出栈
	s.PrintOut(); //输出栈当前的内容


    return 0;
}


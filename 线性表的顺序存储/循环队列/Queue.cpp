#include "stdafx.h"
#include "Queue.h"
#include <iostream>

using namespace std;

/* 构造函数 */

Queue::Queue()
{
	front = 0;
	rear = 0; // 初始化空队列
}

/* 析构函数 */
Queue::~Queue()
{
}

/* 判断循环队列是否为空 */
int Queue::Empty()
{
	if (rear == front)
		return 1;
	else
		return 0;
}

/* 输出队列 */
void Queue::Display()
{

}

/* 进队 */
void Queue::Add(ElemType x)
{
	if ((rear + 1) % MAXSIZE == front) { // 判断是否满队
		cout << "\n Queu is Full!" << endl;
	}
	else {
		rear = (rear + 1) % MAXSIZE; // 尾指针加1
		elem[rear] = x;  // x进队
	}

}

/* 出队 */
ElemType Queue::Del()
{
	if (front == rear) { // 判断队列是否为空
		cout << "\n Queue is Empty!" << endl;
		return -1; // 表明队空，未曾出队
	}
	else {
		front = (front + 1) % MAXSIZE; // 出队操作
		return elem[front];
	}
}

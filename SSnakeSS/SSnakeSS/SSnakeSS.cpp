#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "control.h"
#include "interface.h"
#include <windows.h>


/**
* 主函数
*/
int main()
{
	system("mode con cols=100 lines=30");   //设置控制台的宽高
	system("title Snake online"); 

	printsnake();
	welcometogame();
	File_out();
	keyboardControl();
	endgame();



	return 0;
}
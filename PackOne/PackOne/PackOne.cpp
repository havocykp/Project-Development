// PackOne.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Shell_Base.h"
#include <iostream>

using namespace std;

int main()
{
	CShell_Base obj;
	TCHAR szPath[MAX_PATH];

	memset(szPath, 0, MAX_PATH);
	cout << "\n请输入要加壳的程序的路径 > ";
	wscanf_s(L"%s", szPath, MAX_PATH - 2);
	if (!obj.LoadFile(szPath))
	{
		cout << "无法加载文件，程序即将退出!\n";
		system("pause");
	}

	obj.LoadStubDll();

	cout << "\n请输入要程序的加密密码 > ";
	char szPSW[512];
	if (1 != scanf_s("%s", szPSW, MAX_PATH - 2))
	{
		cout << "该密码无效，程序即将退出!\n";
		system("pause");
		obj.ReleaseAll();
	}
	DWORD dwHash = obj.HashPassWord(szPSW);
	if (dwHash == 0)
	{
		cout << "该密码无效，程序即将退出!\n";
		system("pause");
		obj.ReleaseAll();
	}

	obj.AddStub();

	obj.Encode(dwHash);

	cout << "\n请输入要保存程序的路径 > ";
	memset(szPath, 0, MAX_PATH);
	wscanf_s(L"%s", szPath, MAX_PATH - 2);
	if (!obj.SaveFile(szPath))
	{
		cout << "无法保存文件，程序即将退出!\n";
		system("pause");
		obj.ReleaseAll();
	}

	cout << "\n加壳成功，程序即将退出！\n";
	obj.ReleaseAll();

	system("pause");
    return 0;
}


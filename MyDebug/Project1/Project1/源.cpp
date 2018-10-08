#include<Windows.h>
#include<iostream>

using namespace std;

int main()
{
	HANDLE hToken;
	bool flag = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	cout << "¹þ¹þ£¬Äã±»½Ù³ÖÁË!" << endl;
	cin.get();
}
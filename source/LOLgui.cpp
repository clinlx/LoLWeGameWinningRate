// LOLgui.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <tchar.h>
#include <string.h>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
using namespace std;

wstring getLnkFormPath(wchar_t* lnkPath)
{
	// 初始化
	wstring sRet;
	wchar_t wRet[MAX_PATH];

	// 初始化 COM 库
	CoInitialize(NULL);
	IPersistFile* pPF = NULL;

	// 创建 COM 对象
	HRESULT hr = CoCreateInstance(
		CLSID_ShellLink,			// CLSID
		NULL,						// IUnknown 接口指针
		CLSCTX_INPROC_SERVER,		// CLSCTX_INPROC_SERVER：以 Dll 的方式操作类对象 
		IID_IPersistFile,			// COM 对象接口标识符
		(void**)(&pPF)				// 接收 COM 对象的指针
	); if (FAILED(hr)) { cout << "CoCreateInstance failed." << endl; }

	// 判断是否支持接口
	IShellLink* pSL = NULL;
	hr = pPF->QueryInterface(
		IID_IShellLink,				// 接口 IID
		(void**)(&pSL)				// 接收指向这个接口函数虚标的指针
	); if (FAILED(hr)) { cout << "QueryInterface failed." << endl; }

	// 打开文件
	hr = pPF->Load(
		lnkPath,					// 文件全路径
		STGM_READ					// 访问模式：只读
	); if (FAILED(hr)) { cout << "Load failed ：" << GetLastError() << endl; }

	// 获取 Shell 链接来源
	hr = pSL->GetPath(wRet, MAX_PATH, NULL, 0);
	sRet = wRet;

	// 关闭 COM 库
	pPF->Release();
	CoUninitialize();

	return sRet;
}
void TcharToChar(const TCHAR* tchar, char* _char)
{
	int iLength;
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
void CharToTchar(const char* _char, TCHAR* tchar)
{
	int iLength;
	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}
int main()
{
	TCHAR szPath[MAX_PATH];
	setlocale(LC_ALL, "chs");
	if (!SUCCEEDED(SHGetSpecialFolderPath(NULL, szPath, 23, FALSE)))
	{
		MessageBox(NULL, L"获取windows路径失败！", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	char str[300];
	TcharToChar(szPath, str);
	sprintf_s(str,"%s\\腾讯游戏\\WeGame\\WeGame.lnk",str);
	FILE *fp;
	fopen_s(&fp, str, "r");
	if (fp == NULL)
	{
		MessageBox(NULL, L"获取WeGame路径失败！", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	else fclose(fp);
	//system(str);
	CharToTchar(str, szPath);
	wstring realStr = getLnkFormPath(szPath);
	const TCHAR exe[] = L"exe.";
	for (int i = 0; i < 4; i++)
	{
		if(realStr.back()!=exe[i])
		{
			MessageBox(NULL, L"访问WeGame目录失败！", L"Error", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
		realStr.pop_back();
	}
	fopen_s(&fp, "C:\\power.test", "w");
	if (fp != NULL)fclose(fp);
	fopen_s(&fp, "C:\\power.test", "r");
	if (fp == NULL)
	{
		MessageBox(NULL, L"请右键管理员身份运行此程序！", L"Error", MB_OK | MB_ICONEXCLAMATION);
		if(1)return 0;
	}
	else
	{
		fclose(fp);
		system("del C:\\power.test");
	}
	system("taskkill /f /im wegame.exe");
	Sleep(1000);
	system("taskkill /f /im wegame.exe");
	Sleep(1000);

	for (int i = 0; i < 6; i++)realStr.pop_back();
	realStr += L"apps\\Pallas\\lolguide\\";
	wstring cmd = L"copy /y";
	wstring file1 = L"lol_helper.dll", file2 = L"LOLRes.vfs";
	wstring cmd1 = cmd + L" \".\\" + file1 + L"\" \"" + realStr + L"\"";// +L" > nul";
	wstring cmd2 = cmd + L" \".\\" + file2 + L"\" \"" + realStr + L"\"";// +L" > nul";

	wprintf(L"%s\n", realStr.data());
	char v1[300], v2[300];
	TcharToChar(cmd1.data(), v1);
	TcharToChar(cmd2.data(), v2);
	system(v1);
	system(v2);
	MessageBox(NULL, L"修改成功！", L"成功", MB_OK);
	return 0;
}

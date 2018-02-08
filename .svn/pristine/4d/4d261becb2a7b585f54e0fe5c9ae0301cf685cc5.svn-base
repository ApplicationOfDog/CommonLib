// stdafx.cpp : 只包括标准包含文件的源文件
// TestCorelib.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
String g_programPath;
String g_programDir;

void printHeader()
{
	bool is64Bit = false;
	Environment::GetOsArchitecture(&is64Bit);
	Console::m_tabChar = _T(' ');
	Console::WriteLine("*****************************************");
	Console::WriteLine(1,"OS:%s",is64Bit?"x64":"win32");
	Console::WriteLine("*****************************************");
}

int _tmain(int argc, _TCHAR* argv[])
{
	printHeader();	

	g_programPath = Environment::GetExecutablePath();
	g_programDir = Path::GetDirectoryName(g_programPath);

	extern void Program(int argc, _TCHAR* argv[]);
	Program(argc,argv);

	Console::Pause();
	return 0;
}
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// TestCorelib.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������
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
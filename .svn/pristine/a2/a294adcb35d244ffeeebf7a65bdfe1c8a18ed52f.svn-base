#include "stdafx.h"
#include <System.Compressor.hpp>

void Compressor_Test()
{
	String path = "C:\\Users\\jsb\\Desktop\\更新程序疑问.txt";
	String content = File::ReadAllText(path);
	PBYTE pContent = (PBYTE)content.c_str();
	size_t szContent = content.GetBytes();

	extern void Test(PBYTE pContent,size_t szContent,String& content);
	Test(pContent,szContent,content);

	string contentA = Convert::ToStringA(content,IO::EC_UTF8);
	size_t szContentA = contentA.size()+1;
	PBYTE pContentA = (PBYTE)contentA.c_str();

	extern void TestA(PBYTE pContent,size_t szContent,string& content);
	TestA(pContentA,szContentA,contentA);

}

void Test(PBYTE pContent,size_t szContent,String& content)
{
	SevenZ sz;
	size_t szDest = 0;//  szContent;
	PBYTE output = NULL;//  new BYTE[szDest];
	//ZeroMemory(output,szDest);
	bool isSuc = sz.CompressBytes(pContent,szContent,&output,&szDest,5);
	if(!isSuc)
	{
		Console::WriteLine(_T("compress failed,%s,%d"),(LPCTSTR)sz.m_sLastError,sz.m_nLastError);
		return;
	}
	Console::WriteLine(_T("Compress suc,rate:%u/%u=%.2f%%"),szDest,szContent,(double)szDest/szContent*100);

	size_t szDest2 = 0;// szContent+100;
	PBYTE output2 = NULL;//  new BYTE[szDest2];
	//ZeroMemory(output2,szDest2);
	isSuc = sz.ExtractBytes(output,szDest,&output2,&szDest2);
	if(!isSuc)
	{
		Console::WriteLine(_T("extract failed,%s,%d"),(LPCTSTR)sz.m_sLastError,sz.m_nLastError);
		return;
	}
	String content2 = (LPCTSTR)output2;
	if(content != content2)
	{
		Console::WriteLine("error");
		return;
	}
	sz.FreeBuffer(output);
	sz.FreeBuffer(output2);
}
void TestA(PBYTE pContent,size_t szContent,string& content)
{
	SevenZ sz;
	size_t szDest = 0;//  szContent;
	PBYTE output = NULL;//  new BYTE[szDest];
	//ZeroMemory(output,szDest);
	bool isSuc = sz.CompressBytes(pContent,szContent,&output,&szDest,5);
	if(!isSuc)
	{
		Console::WriteLine(_T("compress failed,%s,%d"),(LPCTSTR)sz.m_sLastError,sz.m_nLastError);
		return;
	}
	Console::WriteLine(_T("Compress suc,rate:%u/%u=%.2f%%"),szDest,szContent,(double)szDest/szContent*100);

	size_t szDest2 = 0;// szContent+100;
	PBYTE output2 = NULL;//  new BYTE[szDest2];
	//ZeroMemory(output2,szDest2);
	isSuc = sz.ExtractBytes(output,szDest,&output2,&szDest2);
	if(!isSuc)
	{
		Console::WriteLine(_T("extract failed,%s,%d"),(LPCTSTR)sz.m_sLastError,sz.m_nLastError);
		return;
	}
	string content2 = (LPCSTR)output2;
	if(content != content2)
	{
		Console::WriteLine("error");
		return;
	}
	sz.FreeBuffer(output);
	sz.FreeBuffer(output2);
}
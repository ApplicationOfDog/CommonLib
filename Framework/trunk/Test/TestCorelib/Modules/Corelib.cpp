#include "stdafx.h"
#include <Core.hpp>
using namespace System::Text;

void Corelib_Test()
{
	int count = 10010;
	String strTest0 = _T("字符串测试0");
	String strTest1 = "字符串测试1";
	String strTest2 = strTest0 + _T(",")+strTest1;
	String strTest3 = String::Format(_T("格式化测试:%s,%s,%d"),(LPCTSTR)strTest0,(LPCTSTR)strTest1,count);

	Console::WriteLine(strTest0);
	Console::WriteLine(strTest1);
	Console::WriteLine(strTest2);
	Console::WriteLine(strTest3);


	StringBuilder builder0;
	builder0.Append(_T("0123456"));
	builder0.AppendFormat(_T("abc-%d"),1024);
	builder0.Append(_T('A'),3);
	builder0.AppendLine();
	Console::WriteLine(builder0.ToString());
}

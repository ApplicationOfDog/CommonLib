#include <stdio.h>
#include <tchar.h>
#include "AmLog.h"
#ifdef _DEBUG
#pragma comment(lib, "AmLogD.lib")
#else
#pragma comment(lib, "AmLog.lib")
#endif
int main()
{
	
	CHAR *pszwTmep = "E:\\workspace\\AmLog\\testLog\\新建文件夹\\test.log";

	//初始化日志组件
	AmLog::InitLog(pszwTmep, "test");

	AmLog::InitLog(pszwTmep, "test2", LEVEL_WARN);

	//由于前面模块对应的都是pszwTmep，则最终生效的级别为级别高的等级LEVEL_WARN

	AmLog::SetLevel("test", LEVEL_INFO);//按照最高等级生效，所以本次设置不生效

	//如果需要控制自己的日志等级，应该把日志写到独立的日志文件里面
	AmLog::InitLog("E:\\workspace\\AmLog\\testLog\\新建文件夹\\test1.log", 
		"test3", LEVEL_INFO);

	int i =0;
	for (i=0; i<100; i++)
	{
		AmLog::Log("test", LEVEL_FATAL, "test");
		AM_LOG("test", LEVEL_TRACE, "中文%S", L"测试");
		AM_TRACE( "test", "test%s", "test");
		AM_TRACE("test2", "中文%S", L"测试");
		AM_WARN("test3", "中文%S", L"测试");
		AM_ERROR("test2", "中文%S", L"测试");
		AM_INFO("test3", "中文%S", L"测试");
	}
	

	return 0;
}
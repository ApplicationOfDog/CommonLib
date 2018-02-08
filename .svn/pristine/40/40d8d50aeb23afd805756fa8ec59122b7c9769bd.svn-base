#pragma once
#define SYSTEM_DB_NAME "System.DB"
#define SYSTEM_DB_VERSION "v2.0.1"

#include "Platform.hpp"

#if IS_WINDOWS_OS //Windows
#	ifdef DB_EXPORTS //源代码,PS.如果在dll中使用该lib，请请定义CORE_EXPORTS宏
#		define API_DB __declspec(dllexport)
#	elif defined(DB_IMPORTS) 
#		define API_DB  __declspec(dllimport) //如果工程是dll不是lib,那么目标工程(即引用工程)就需要dllimport才能找到类的static成员
#	elif defined(DB_NOPORTS)
#		define API_DB  //使用静态lib的时候不能__declspec(dllimport)，这个修饰符是用作dll导出的
#else
#		define API_DB
#endif

#	include <assert.h>
#	include <tchar.h>
#	include <WTypes.h>
#else //其他OS
#	define API_DB  ///Windows 库导出宏定义，OSX不需要所以定义为空的
#	include "OS/OSX.hpp"
#endif //end IS_WINDOWS_OS


#include "..\System.DB\core\TableStructure.hpp"
#include "..\System.DB\core\ISqlStatement.hpp"
#include "..\System.DB\core\IDataReader.hpp"
#include "..\System.DB\core\IDB.hpp"

using namespace System::DB;

#ifndef _NO_IMPORT_LIB_
#	if IS_WINDOWS_OS //如果是window平台则在这里导入lib库	
#		pragma comment(lib,LIB_FULL_NAME(SYSTEM_DB_NAME))
#		pragma message(LIB_COMPILE_MESSAGE(SYSTEM_DB_NAME,SYSTEM_DB_VERSION))
#	endif //IS_WINDOWS_OS
//#	pragma message(SYSTEM_DB_NAME":"PRINT_MACRO_1(API_DB)) //编译输出预定义宏的值
#endif
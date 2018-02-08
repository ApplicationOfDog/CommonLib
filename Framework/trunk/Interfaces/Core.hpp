#pragma once
#define CORE_LIB_NAME "corelib"
#define CORE_VERSION "v2.0.170413"

#include "Platform.hpp"

#if IS_WINDOWS_OS //Windows
#	ifdef CORE_EXPORTS //源代码,PS.如果在dll中使用该lib，请请定义CORE_EXPORTS宏
#		define API_CORE __declspec(dllexport)
#	elif defined(CORE_IMPORTS) 
#		define API_CORE  __declspec(dllimport) //如果工程是dll不是lib,那么目标工程(即引用工程)就需要dllimport才能找到类的static成员
#	elif defined(CORE_NOPORTS)
#		define API_CORE  //使用静态lib的时候不能__declspec(dllimport)，这个修饰符是用作dll导出的
#else
#		define API_CORE
#endif

#	include <assert.h>
#	include <tchar.h>
#	include <WTypes.h>
#else //其他OS
#	define API_CORE  ///Windows 库导出宏定义，OSX不需要所以定义为空的
#	include "OS/OSX.hpp"
#endif //end IS_WINDOWS_OS


///<remark>Common and Define</remark>
#include "../CoreLib/Core/Com.hpp"
#include "../CoreLib/Core/Helper/CoreTypes.hpp"
#include "../CoreLib/IO/Helper/IOTypes.hpp"

///<remark>System</remark>
#include "../CoreLib/Core/String.hpp"
#include "../CoreLib/Core/Convert.hpp"
#include "../CoreLib/Core/Char.hpp"
#include "../CoreLib/Core/Environment.hpp"
#include "../CoreLib/Core/TimeSpan.hpp"
#include "../CoreLib/Core/DateTime.hpp"
#include "../CoreLib/Core/Console.hpp"

///<remark>System::Data</remark>
#include "../CoreLib/Data/Memory.hpp"

///<remark>System::IO</remark>
#include "../CoreLib/IO/Path.hpp"
#include "../CoreLib/IO/File.hpp"
#include "../CoreLib/IO/Directory.hpp"
#include "../CoreLib/IO/TextLog.hpp"
#include "../CoreLib/IO/TimeLog.hpp"
#include "../CoreLib/IO/Debuger.hpp"

///<remark>System::Text</remark>
#include "../CoreLib/Text/StringBuilder.hpp"

///<remark>System::Pattern</remark>
#include "../CoreLib/Pattern/Singleton.hpp"
#include "../CoreLib/Pattern/IError.hpp"
#include "../CoreLib/Pattern/ILock.hpp"
#include "../CoreLib/Pattern/Reflect.hpp"

typedef System::String stringEx;
using namespace System;

#ifndef _NO_IMPORT_LIB_
#	if IS_WINDOWS_OS //如果是window平台则在这里导入lib库	
#		pragma comment(lib,LIB_FULL_NAME(CORE_LIB_NAME))
#		pragma message(LIB_COMPILE_MESSAGE(CORE_LIB_NAME,CORE_VERSION))
#	endif //IS_WINDOWS_OS
//#	pragma message(CORE_LIB_NAME":"PRINT_MACRO_1(API_CORE)) //编译输出预定义宏的值
#endif
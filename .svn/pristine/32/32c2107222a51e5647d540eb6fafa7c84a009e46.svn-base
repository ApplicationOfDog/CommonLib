#pragma once
#define SYSTEM_LIB_NAME "System"
#define SYSTEM_VERSION "v2.0.1"

#include "Platform.hpp"

#ifdef IS_WINDOWS_OS //Windows

#	ifdef SYSTEM_EXPORTS //源代码,PS.如果在dll中使用该lib，请请定义CORE_EXPORTS宏
#		define API_SYSTEM __declspec(dllexport)
#	elif defined(SYSTEM_IMPORTS) 
#		define API_SYSTEM  __declspec(dllimport)
#	elif defined(SYSTEM_NOPORTS)
#		define API_SYSTEM  //使用静态lib的时候不能__declspec(dllimport)，这个修饰符是用作dll导出的
#else
#		define API_SYSTEM
#endif

#	include <assert.h>
#	include <tchar.h>
#	include <WTypes.h>
#	include <WinDef.h>
#else //其他OS
#	define API_SYSTEM  ///Windows 库导出宏定义，OSX不需要所以定义为空的
#endif //end IS_WINDOWS_OS


///<remark>System::Diagnostics</remark>
#include "../System/Diagnostics/Machine.hpp"
#include "../System/Diagnostics/Runtime.hpp"
#include "../System/Diagnostics/MiniDump.hpp"
#include "../System/Diagnostics/Registry.hpp"

///<remark>System::Task</remark>
#include "../System/Task/AsynTask.hpp"

///<remark>System::Data</remark>
#include "../System/Data/GlobalUniqueID.hpp"

///<remark>System::Communication</remark>
#include "../System/Communication/DIOC.h"

using namespace System;

#ifndef _NO_IMPORT_LIB_
#	if IS_WINDOWS_OS //如果是window平台则在这里导入lib库	
#		pragma comment(lib,LIB_FULL_NAME(SYSTEM_LIB_NAME))
#		pragma message(LIB_COMPILE_MESSAGE(SYSTEM_LIB_NAME,SYSTEM_VERSION))
#	endif //IS_WINDOWS_OS
//#	pragma message(SYSTEM_LIB_NAME":"PRINT_MACRO_1(API_SYSTEM)) //编译输出预定义宏的值
#endif
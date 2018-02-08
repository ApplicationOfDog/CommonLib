#pragma once
#define SYSTEM_JSON_NAME "System.Json"
#define SYSTEM_JSON_VERSION "v2.0.1"

#include "..\System.Json\interface\version.hpp"
#include "..\System.Json\interface\config.hpp"

#include "..\System.Json\core\value.hpp"
#include "..\System.Json\core\reader.hpp"
#include "..\System.Json\core\writer.hpp"
#include "..\System.Json\core\features.hpp"

using namespace System::Json;

#ifndef _NO_IMPORT_LIB_
#	if IS_WINDOWS_OS //如果是window平台则在这里导入lib库	
#		pragma comment(lib,LIB_FULL_NAME(SYSTEM_JSON_NAME))
#		pragma message(LIB_COMPILE_MESSAGE(SYSTEM_JSON_NAME,SYSTEM_JSON_VERSION))
#	endif //IS_WINDOWS_OS
//#	pragma message(SYSTEM_JSON_NAME":"PRINT_MACRO_1(JSON_API)) //编译输出预定义宏的值
#endif
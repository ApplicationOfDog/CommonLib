// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

#ifdef _LIB
	#define SYSTEM_NOPORTS 1 	//lib工程不使用__declspec修饰，因为该修饰符只针对dll
#elif defined(_USRDLL)
	#define SYSTEM_EXPORTS 1 //dll工程使用__declspec(dllexport)修饰
#endif

#include "../../Interfaces/Core.hpp"
#include "../../Interfaces/System.hpp"
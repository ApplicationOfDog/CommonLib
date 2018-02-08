// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

#ifdef _LIB	
	#define CORE_NOPORTS 1 //lib工程不使用__declspec修饰，因为该修饰符只针对dll
#elif defined(_USRDLL)
	#define CORE_EXPORTS 1 //dll工程使用__declspec(dllexport)修饰
	//warning C4275: 非 dll 接口 struct“std::_Container_base12”用作 dll 接口 class“std::_String_val<_Elem,_Alloc>”的基
	#pragma warning(disable:4275)
	// warning C4251: “std::_String_val<_Elem,_Alloc>::_Alval”: class“std::allocator<_Ty>”需要有 dll 接口由 class“std::_String_val<_Elem,_Alloc>”的客户端使用
	#pragma warning(disable:4251)
#endif

#include "../../Interfaces/Core.hpp"

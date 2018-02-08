#pragma once

/*
静态lib注意点:
1.静态lib是代码嵌入,所以并没有导入导出的概念.
2.静态lib中的导出函数或类不能使用dllexport或dllimport描述
3.宿主程序在引用静态lib时,lib对应的头文件中不要使用dllexport或dllimport描述
4.dll使用dumpbin -exports ".lib/.dll"可以看到导出函数,但是静态lib是看不到内容的(可通过dumpbin -linkermember "静态.lib").
总之:dll才有dllexport或dllimport问题,静态lib是没有的,使用时只需要将.h和.lib引入即可
*/

///<summary>
/// 通过如下两个宏可以打印出宏名称和宏内容,实例如下:
/// #define PI 3.1415926
/// #define MAX(a,b) (a)>(b) ? (a) :(b)
/// #pragma message(PRINT_MACRO(PI))		输出:PI=3.1415926
/// #pragma message(PRINT_MACRO(PI2))		输出:PI2=PI2
/// #pragma message(PRINT_MACRO(MAX(x,y)))	输出:MAX(x,y)=(x)>(y) ? (x) :(y)
///</summary>
#define PRINT_MACRO_VALUE(x)	#x //打印宏的值内容
#define PRINT_MACRO_0(x)		PRINT_MACRO_VALUE(x) //打印:宏值内容
#define PRINT_MACRO_1(x)		#x"="PRINT_MACRO_VALUE(x) //打印:宏名称=宏值内容

//os types
#define _OS_WINDOWS_	0
#define _OS_MAC_		1
#define _OS_LINUX_		2

//current os type define
#ifndef _OS_TYPE_
#	if defined(WIN32)||defined(_WIN32)||defined(_WIN64) //如果定义WIN32表示是windows系统
#		define _OS_TYPE_  _OS_WINDOWS_
#		define IS_WINDOWS_OS	1
#	else
#		define _OS_TYPE_  _OS_MAC_ //其他平台请在这边定义
#		define IS_MAC_OS	1
#	endif
#endif
#pragma message("Platform: os type is "PRINT_MACRO_1(_OS_TYPE_))

//judge current os'type
#define IS_OS_TYPE(os) (_OS_TYPE_==os)

#ifndef IS_WINDOWS_OS
#define IS_WINDOWS_OS	IS_OS_TYPE(_OS_WINDOWS_)
#endif

#ifndef IS_MAC_OS
#define IS_MAC_OS		IS_OS_TYPE(_OS_MAC_)
#endif

#ifndef IS_LINUX_OS
#define IS_LINUX_OS		IS_OS_TYPE(_OS_LINUX_)
#endif

#if IS_WINDOWS_OS
#pragma message("Platform: IS_WINDOWS_OS")
#endif

#if IS_MAC_OS
#pragma message("Platform: IS_MAC_OS")
#endif

#if IS_LINUX_OS
#pragma message("Platform: IS_LINUX_OS")
#endif

//定义运行库
#undef _RUNTIME_MSG_HEADER_
#define _RUNTIME_MSG_HEADER_ "Platform: Run-time library is "

#define RT_LIB_MDd		1
#define RT_LIB_MD		2
#define RT_LIB_MTd		3
#define RT_LIB_MT		4
#undef _RUNTIME_LIBRARY_

#ifdef _UNICODE //unicode编码

#	ifdef _DLL //Defined when /MD or /MDd (Multithread DLL) is specified
#		ifdef _DEBUG //debug版本
#			define _RUNTIME_LIBRARY_ RT_LIB_MDd
#			define LIB_NAME_SUFFIX "-mdd.lib"
#			pragma message(_RUNTIME_MSG_HEADER_"/MDd & Unicode")
#		else //release版本
#			define _RUNTIME_LIBRARY_ RT_LIB_MD
#			define LIB_NAME_SUFFIX "-md.lib"
#			pragma message(_RUNTIME_MSG_HEADER_"/MD & Unicode")
#		endif
#	else //_DLL
#		ifdef _DEBUG //debug版本
#			define _RUNTIME_LIBRARY_ RT_LIB_MTd
#			define LIB_NAME_SUFFIX "-mtd.lib"
#			pragma message(_RUNTIME_MSG_HEADER_"/MTd & Unicode")
#		else //release版本
#			define _RUNTIME_LIBRARY_ RT_LIB_MT
#			define LIB_NAME_SUFFIX "-mt.lib"
#			pragma message(_RUNTIME_MSG_HEADER_"/MT & Unicode")
#		endif
#	endif

#else //ansi编码
	
#	ifdef _DLL //Defined when /MD or /MDd (Multithread DLL) is specified
#		ifdef _DEBUG //debug版本
#			define _RUNTIME_LIBRARY_ RT_LIB_MDd
#			define LIB_NAME_SUFFIX "-a-mdd.lib"
#			pragma message(_RUNTIME_MSG_HEADER_"/MDd & Ansi")
#		else //release版本
#			define LIB_NAME_SUFFIX "-a-md.lib"
#			define _RUNTIME_LIBRARY_ RT_LIB_MD
#			pragma message(_RUNTIME_MSG_HEADER_"/MD & Ansi")
#		endif
#	else //_DLL
#		ifdef _DEBUG //debug版本
#			define _RUNTIME_LIBRARY_ RT_LIB_MTd
#			define LIB_NAME_SUFFIX "-a-mtd.lib"
#			pragma message(_RUNTIME_MSG_HEADER_"/MTd & Ansi")
#		else //release版本
#			define _RUNTIME_LIBRARY_ RT_LIB_MT
#			define LIB_NAME_SUFFIX "-a-mt.lib"
#			pragma message(_RUNTIME_MSG_HEADER_"/MT & Ansi")
#		endif
#	endif

#endif//_UNICODE

#ifdef IS_DYNAMIC_LINK_LIB //如果是引用动态链接库
	#define LIB_MID_NAME "-d"
	#define CORE_IMPORTS 1 //corelib中所有static变量就需要使用dllimport导入
#else //如果是引用静态链接库
	#define LIB_MID_NAME ""
#endif

#undef LIB_FULL_NAME
#define LIB_FULL_NAME(lib) lib""LIB_MID_NAME""LIB_NAME_SUFFIX
#define LIB_COMPILE_MESSAGE(lib,ver) "include " LIB_FULL_NAME(lib) " ("ver")"


#ifndef IN
#	define IN
#endif

#ifndef OUT
#	define OUT
#endif

#ifndef IOUT
#	define IOUT
#endif

#ifndef FALSE
#	define FALSE 0
#	define TRUE 1
#endif // !FALSE
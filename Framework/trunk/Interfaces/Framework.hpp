#pragma once
/*
静态lib注意点:
1.静态lib是代码嵌入,所以并没有导入导出的概念.
2.静态lib中的导出函数或类不能使用dllexport或dllimport描述
3.宿主程序在引用静态lib时,lib对应的头文件中不要使用dllexport或dllimport描述
4.dll使用dumpbin -exports ".lib/.dll"可以看到导出函数,但是静态lib是看不到内容的(可通过dumpbin -linkermember "静态.lib").
总之:dll才有dllexport或dllimport问题,静态lib是没有的,使用时只需要将.h和.lib引入即可
*/

#if defined(NET_LIB_EXPORT_STATIC)
	#undef NET_LIB_EXPORT
#endif

//后三个库是依赖第一个库的(CORE_LIB_EXPORT)
#if defined(SYSTEM_LIB_EXPORT)||defined(NET_LIB_EXPORT_STATIC)||defined(NET_LIB_EXPORT)
	#ifndef CORE_LIB_EXPORT 
	#define CORE_LIB_EXPORT 1
	#endif
#endif
#if defined(XML_LIB_EXPORT)||(DB_LIB_EXPORT)||defined(PLUGGABLE_LIB_EXPORT)||defined(JSON_LIB_EXPORT)
	#ifndef CORE_LIB_EXPORT 
	#define CORE_LIB_EXPORT 1
	#endif
#endif
#if defined(COMPRESSOR_LIB_EXPORT)
	#ifndef CORE_LIB_EXPORT 
	#define CORE_LIB_EXPORT 1
	#endif
#endif

#pragma message("************** Framework library  **********************")
#define IS_FRAMEWORK_INCLUDE

#ifdef CORE_LIB_EXPORT
	#include "Core.hpp"
#endif

#ifdef SYSTEM_LIB_EXPORT
	#include "System.hpp"
#endif

#if defined(NET_LIB_EXPORT_STATIC)||defined(NET_LIB_EXPORT)
	#include "System.Net.hpp"
#endif

#ifdef DB_LIB_EXPORT
	#include "System.DB.hpp"
#endif

#ifdef XML_LIB_EXPORT
	#include "System.Xml.hpp"
#endif

#ifdef JSON_LIB_EXPORT
	#include "System.Json.hpp"
#endif



//warning LNK4099: 未找到 PDB“vc90.pdb”(使用“xx.lib”或在“...\vc90.pdb”中寻找)；正在链接对象，如同没有调试信息一样
#pragma warning(disable:4099)
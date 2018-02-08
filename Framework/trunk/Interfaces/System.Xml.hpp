#pragma once
#define SYSTEM_XML_NAME "System.Xml"
#define SYSTEM_XML_VERSION "v2.0.0"

#include "Platform.hpp"

#if IS_WINDOWS_OS //Windows
#	ifdef XML_EXPORTS //源代码,PS.如果在dll中使用该lib，请请定义CORE_EXPORTS宏
#		define API_Xml __declspec(dllexport)
#	elif defined(XML_IMPORTS) 
#		define API_Xml  __declspec(dllimport) //如果工程是dll不是lib,那么目标工程(即引用工程)就需要dllimport才能找到类的static成员
#	elif defined(XML_NOPORTS)
#		define API_Xml  //使用静态lib的时候不能__declspec(dllimport)，这个修饰符是用作dll导出的
#else
#		define API_Xml
#endif

#	include <assert.h>
#	include <tchar.h>
#	include <WTypes.h>
#else //其他OS
#	define API_Xml  ///Windows 库导出宏定义，OSX不需要所以定义为空的
#	include "OS/OSX.hpp"
#endif //end IS_WINDOWS_OS

///<remark>reference</remark>
#ifdef _UNICODE
#	define PUGIXML_WCHAR_MODE 1
#else
#	undef PUGIXML_WCHAR_MODE
#endif
#include "../System.Xml/Pugixml/pugixml.hpp"
using namespace pugi;

///<remark>Core</remark>
#include "../System.Xml/Core/XmlDocument.hpp"
#include "../System.Xml/Core/XmlNode.hpp"
#include "../System.Xml/Core/XmlElement.hpp"
#include "../System.Xml/Core/XmlAttribute.hpp"
#include "../System.Xml/Core/XmlNodeList.hpp"

///<remark>Extension</remark>
#include "../System.Xml/Extension/IDataNode.hpp"

using namespace System::Xml;

#ifndef _NO_IMPORT_LIB_
#	if IS_WINDOWS_OS //如果是window平台则在这里导入lib库	
#		pragma comment(lib,LIB_FULL_NAME(SYSTEM_XML_NAME))
#		pragma message(LIB_COMPILE_MESSAGE(SYSTEM_XML_NAME,SYSTEM_XML_VERSION))
#	endif //IS_WINDOWS_OS
//#	pragma message(SYSTEM_XML_NAME":"PRINT_MACRO_1(API_Xml)) //编译输出预定义宏的值
#endif
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����


#ifdef _LIB	
	#define JSON_NOPORTS 1 //lib���̲�ʹ��__declspec���Σ���Ϊ�����η�ֻ���dll
#elif defined(_USRDLL)
	#define JSON_EXPORTS 1 //dll����ʹ��__declspec(dllexport)����
#endif

#define PRINT_MACRO_VALUE(x)   #x //��ӡ���ֵ����
#define PRINT_MACRO(x)   #x"="PRINT_MACRO_VALUE(x)

#include <string>
#include <vector>
#include <exception>
#include <map>

#include <utility>
#include <cstdio>
#include <cassert>
#include <cstring>

#include "..\interface\config.hpp"

#include "..\..\Interfaces\Core.hpp"
#include "..\core\value.hpp"
#include "..\core\reader.hpp"
#include "..\core\writer.hpp"
#include "..\core\path.hpp"

//Conditional NORETURN attribute on the throw functions would:
// a) suppress false positives from static code analysis 
// b) possibly improve optimization opportunities.
#if !defined(JSONCPP_NORETURN)
#  if defined(_MSC_VER)
#    define JSONCPP_NORETURN __declspec(noreturn)
#  elif defined(__GNUC__)
#    define JSONCPP_NORETURN __attribute__ ((__noreturn__))
#  else
#    define JSONCPP_NORETURN
#  endif
#endif

#pragma message(PRINT_MACRO(JSONCPP_NORETURN))

// Disable warning C4251: <data member>: <type> needs to have dll-interface to
// be used by...
#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)

namespace System{
	namespace Json {

	/// used internally
	JSONCPP_NORETURN void throwRuntimeError(JSONCPP_STRING const& msg);
	/// used internally
	JSONCPP_NORETURN void throwLogicError(JSONCPP_STRING const& msg);

}}

namespace std {
	/// Specialize std::swap() for Json::Value.
	template<>
	inline void swap(System::Json::Value& a,System::Json::Value& b) { a.swap(b); }
}

#define IS_LETTER(c) (( c>='a' && c<='z' ) || ( c>='A' && c<='Z' ))
#define IS_LETTER_OR_DIGIT(c) (( c>='a' && c<='z' ) || ( c>='A' && c<='Z' ) || (c>='0' && c<='9'))
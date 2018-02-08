﻿#pragma once

//dll不能使用std:string作为接口参数,因为string在dll和exe中分配内存堆不同,如果生命周期结束交叉释放会有问题
#undef IS_DLL_EXPORT	//dll导出
#undef IS_DLL_IMPORT	//dll导入

#ifdef _MSC_VER //VC编译器

	#ifdef JSON_EXPORTS //源代码,PS.如果在dll中使用该lib，请请定义CORE_EXPORTS宏
		#define JSON_API __declspec(dllexport)
		#define JSONCPP_DISABLE_DLL_INTERFACE_WARNING
		#define IS_DLL_EXPORT
	#elif defined(JSON_IMPORTS) 
		#define JSON_API  __declspec(dllimport)
		#define JSONCPP_DISABLE_DLL_INTERFACE_WARNING
		#define IS_DLL_IMPORT
	#elif defined(JSON_NOPORTS)
		#define JSON_API  //使用静态lib的时候不能__declspec(dllimport)，这个修饰符是用作dll导出的
	#else
		#define JSON_API
	#endif

	#include <assert.h>
	#include <tchar.h>
	#include <WTypes.h>

#else //其他编译器，ps.GCC

	#define JSON_API  ///Windows 库导出宏定义，OSX不需要所以定义为空的

#endif //end _MSC_VER

#include <stddef.h>
#include <string> //typdef String

/// If defined, indicates that json library is embedded in CppTL library.
//# define JSON_IN_CPPTL 1

/// If defined, indicates that json may leverage CppTL library
//#  define JSON_USE_CPPTL 1
/// If defined, indicates that cpptl vector based map should be used instead of
/// std::map
/// as Value container.
//#  define JSON_USE_CPPTL_SMALLMAP 1

// If non-zero, the library uses exceptions to report bad input instead of C
// assertion macros. The default is to use exceptions.
#ifndef JSON_USE_EXCEPTION
#define JSON_USE_EXCEPTION 1
#endif

/// If defined, indicates that the source file is amalgated
/// to prevent private header inclusion.
/// Remarks: it is automatically defined in the generated amalgated header.
// #define JSON_IS_AMALGAMATION

#ifdef JSON_IN_CPPTL
#include <cpptl/config.h>
#ifndef JSON_USE_CPPTL
#define JSON_USE_CPPTL 1
#endif
#endif

// If JSON_NO_INT64 is defined, then Json only support C++ "int" type for
// integer
// Storages, and 64 bits integer support is disabled.
// #define JSON_NO_INT64 1

#if defined(_MSC_VER) // MSVC
#  if _MSC_VER <= 1200 // MSVC 6
// Microsoft Visual Studio 6 only support conversion from __int64 to double
// (no conversion from unsigned __int64).
#    define JSON_USE_INT64_DOUBLE_CONVERSION 1
// Disable warning 4786 for VS6 caused by STL (identifier was truncated to '255'
// characters in the debug information)
// All projects I've ever seen with VS6 were using this globally (not bothering
// with pragma push/pop).
#    pragma warning(disable : 4786)
#  endif // MSVC 6

#  if _MSC_VER >= 1500 // MSVC 2008
/// Indicates that the following function is deprecated.
#    define JSONCPP_DEPRECATED(message) __declspec(deprecated(message))
#  endif

#endif // defined(_MSC_VER)

// In c++11 the override keyword allows you to explicity define that a function
// is intended to override the base-class version.  This makes the code more
// managable and fixes a set of common hard-to-find bugs.
#if __cplusplus >= 201103L
# define JSONCPP_OVERRIDE override
#elif defined(_MSC_VER) && _MSC_VER > 1600
# define JSONCPP_OVERRIDE override
#else
# define JSONCPP_OVERRIDE
#endif

#ifndef JSON_HAS_RVALUE_REFERENCES

#if defined(_MSC_VER) && _MSC_VER >= 1600 // MSVC >= 2010
#define JSON_HAS_RVALUE_REFERENCES 1
#endif // MSVC >= 2010

#ifdef __clang__
#if __has_feature(cxx_rvalue_references)
#define JSON_HAS_RVALUE_REFERENCES 1
#endif  // has_feature

#elif defined __GNUC__ // not clang (gcc comes later since clang emulates gcc)
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103L)
#define JSON_HAS_RVALUE_REFERENCES 1
#endif  // GXX_EXPERIMENTAL

#endif // __clang__ || __GNUC__

#endif // not defined JSON_HAS_RVALUE_REFERENCES

#ifndef JSON_HAS_RVALUE_REFERENCES
#define JSON_HAS_RVALUE_REFERENCES 0
#endif

#ifdef __clang__
#elif defined __GNUC__ // not clang (gcc comes later since clang emulates gcc)
#  if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))
#    define JSONCPP_DEPRECATED(message)  __attribute__ ((deprecated(message)))
#  elif (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#    define JSONCPP_DEPRECATED(message)  __attribute__((__deprecated__))
#  endif  // GNUC version
#endif // __clang__ || __GNUC__

#if !defined(JSONCPP_DEPRECATED)
#define JSONCPP_DEPRECATED(message)
#endif // if !defined(JSONCPP_DEPRECATED)

#if __GNUC__ >= 6
#  define JSON_USE_INT64_DOUBLE_CONVERSION 1
#endif

namespace System{
	namespace Json {

	typedef int Int;
	typedef unsigned int UInt;
	#if defined(JSON_NO_INT64)
		typedef int LargestInt;
		typedef unsigned int LargestUInt;
		#undef JSON_HAS_INT64
	#else                 // if defined(JSON_NO_INT64)
	// For Microsoft Visual use specific types as long long is not supported
	#if defined(_MSC_VER) // Microsoft Visual Studio
	typedef __int64 Int64;
	typedef unsigned __int64 UInt64;
	#else                 // if defined(_MSC_VER) // Other platforms, use long long
	typedef long long int Int64;
	typedef unsigned long long int UInt64;
	#endif // if defined(_MSC_VER)
	typedef Int64 LargestInt;
	typedef UInt64 LargestUInt;
	#define JSON_HAS_INT64
	#endif // if defined(JSON_NO_INT64)

	#define JSONCPP_STRING        std::string
	#define JSONCPP_OSTRINGSTREAM std::ostringstream
	#define JSONCPP_OSTREAM       std::ostream
	#define JSONCPP_ISTRINGSTREAM std::istringstream
	#define JSONCPP_ISTREAM       std::istream

	typedef unsigned int ArrayIndex;

}} // end namespace Json

///<summary>
/// 属性名称是否为复杂表达式
/// 如果定义了该宏,则属性名称可以有三种形式
/// 1.{ xx: 100 } => 名称不需要其他符号
/// 2.{ "xx": 100 } => 名称需要用双引号引用
/// 3.{ 'xx': 100 } => 名称需要用单引号引用
/// 如果没有定义这个宏,则只能使用第二种
///</summary>
#define IS_COMPLEX_NAME_REPRESSION 1
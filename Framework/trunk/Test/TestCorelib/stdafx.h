// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <Assert.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

//#define CORE_LIB_EXPORT 1
//#define JSON_LIB_EXPORT 1
//#define SYSTEM_LIB_EXPORT 1
//#define NET_LIB_EXPORT 1
//#define XML_LIB_EXPORT 1
//#define DB_LIB_EXPORT 1
//#define COMPRESSOR_LIB_EXPORT 1
//#define SECURITY_LIB_EXPORT 1
//#define PLUGGABLE_LIB_EXPORT 1
//#include <Framework.hpp>
#include <Core.hpp>
using namespace System::IO;

extern String g_programPath;
extern String g_programDir;

#define PRINT_SIZE(t) Console::WriteLine("sizeof(%s) = %d bytes",#t,sizeof(t))
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <Core.hpp>
using namespace System::Pattern;
using namespace System::IO;

#include <UIlib.h>
using namespace DuiLib;

extern PDebuger g_pDebuger;
#undef GDebuger
#define GDebuger g_pDebuger

#include "..\res\Resource.h"
#include "..\program\Program.h"
extern Context*	GContext;
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#if !defined(AFX_STDAFX_H__A9DB82DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB82DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)
#pragma warning(disable : 4996)
#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <rpcsal.h>
#include <Windows.h>
#include <COMMDLG.H>
#include <objbase.h>
#include <shlobj.h>
#include <Shellapi.h>
#include <exdisp.h>
#include <comdef.h>
#include <STDIO.H>
#include <map>
#include <WINSOCK2.H>
#include <iphlpapi.h>
#include <Shlwapi.h>
#include <xstring>
#include <list>
#include "resource.h"

#include <Core.hpp>
#include <System.hpp>

#include "UIlib.h"
using namespace DuiLib;
#pragma comment(lib, "DuiLib.lib")

// ======================================== DUILIB �ؼ�����Լ�� ==============================
//�򵼿ؼ�
#define DN_TAB_PAGE				 _T("tab_wizard")
//��װ������ؿؼ�
#define DN_BAR_PROGRASS			 _T("sld_percent")
#define DN_TXT_PERCENT			 _T("txt_percent")
#define DN_TXT_PROGINFO			 _T("txt_install")
#define DN_GIF_BG				 _T("gif_bg")
#define DN_ERROR				 _T("dn_error")
//ҳ�泣�ð�ť
#define DN_BTN_CLOSE			 _T("btn_close")
#define DN_BTN_MIN				 _T("btn_min")
#define DN_BTN_NEXT				 _T("btn_next")
#define DN_BTN_BACK				 _T("btn_back")
//��װ���ð�ť
#define DN_OPT_AGREE			 _T("opt_agree")
#define DN_BTN_CUSTOM			 _T("btn_custom")
#define DN_BTN_FASTINST			 _T("btn_fastinst")
#define DN_BTN_INSTALL			 _T("btn_install")
#define DN_BTN_FINISH			 _T("btn_finish")
//����Ŀ¼���
#define DN_BTN_CHGDIR			 _T("btn_dir")
#define DN_EDT_CHGDIR			 _T("edt_dir")

//============================================================================================
#endif // !defined(AFX_STDAFX_H__A9DB82DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
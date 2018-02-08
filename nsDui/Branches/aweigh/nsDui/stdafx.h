// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#if !defined(AFX_STDAFX_H__A9DB82DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB82DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)
#pragma warning(disable : 4996)
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
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

// ======================================== DUILIB 控件名称约定 ==============================
//向导控件
#define DN_TAB_PAGE				 _T("tab_wizard")
//安装进度相关控件
#define DN_BAR_PROGRASS			 _T("sld_percent")
#define DN_TXT_PERCENT			 _T("txt_percent")
#define DN_TXT_PROGINFO			 _T("txt_install")
#define DN_GIF_BG				 _T("gif_bg")
#define DN_ERROR				 _T("dn_error")
//页面常用按钮
#define DN_BTN_CLOSE			 _T("btn_close")
#define DN_BTN_MIN				 _T("btn_min")
#define DN_BTN_NEXT				 _T("btn_next")
#define DN_BTN_BACK				 _T("btn_back")
//安装常用按钮
#define DN_OPT_AGREE			 _T("opt_agree")
#define DN_BTN_CUSTOM			 _T("btn_custom")
#define DN_BTN_FASTINST			 _T("btn_fastinst")
#define DN_BTN_INSTALL			 _T("btn_install")
#define DN_BTN_FINISH			 _T("btn_finish")
//更改目录相关
#define DN_BTN_CHGDIR			 _T("btn_dir")
#define DN_EDT_CHGDIR			 _T("edt_dir")

//============================================================================================
#endif // !defined(AFX_STDAFX_H__A9DB82DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
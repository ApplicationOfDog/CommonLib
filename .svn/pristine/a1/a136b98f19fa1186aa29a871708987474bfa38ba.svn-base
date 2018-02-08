#include "stdafx.h"
#include "plugin-common.h"
#include "DlgMain.h"

extra_parameters *g_pluginParms = NULL;
std::map<HWND, WNDPROC>g_windowInfoMap;
bool g_bMSGLoopFlag = true;

HINSTANCE g_hInstance;
HWND g_hwndParent;
HANDLE g_hMutex;

CDlgMain *g_pMainDlg = NULL;
CDlgMain *g_pSubDlg = NULL;
CTabLayoutUI *g_pTabLayout = NULL;

#define NSMETHOD_INIT(parent) {\
        g_pluginParms = extra; \
        g_hwndParent = parent; \
        EXDLL_INIT(); }

BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	g_hInstance = (HINSTANCE)hInst;
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		//do what you want at init time.
	}
	if (ul_reason_for_call == DLL_THREAD_DETACH || ul_reason_for_call == DLL_PROCESS_DETACH) {
		//clean up code.
	}
	return TRUE;
}

//回调
BOOL CALLBACK NSISWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//必须添加，否则显示#32770
	ShowWindow(hwnd, SW_HIDE);
	BOOL res = 0;
	std::map<HWND, WNDPROC>::iterator iter = g_windowInfoMap.find(hwnd);
	if (iter != g_windowInfoMap.end())
	{
		if (message == WM_PAINT)
		{
			ShowWindow(hwnd, SW_HIDE);
		}
		else if (message == LVM_INSERTITEM)
		{
			//源代码使用ListView_InsertItem插入数据
			//(int)SNDMSG((hwnd), LVM_INSERTITEM, 0, (LPARAM)(const LV_ITEM *)(pitem))
			const LV_ITEM * item = (const LV_ITEM *)lParam;
			CTextUI *pInfo = static_cast<CTextUI *>(g_pMainDlg->GetPM()->FindControl(DN_TXT_PROGINFO));
			if (pInfo) pInfo->SetText(item->pszText);
		}
		else if (message == PBM_SETPOS)
		{
			int iValue = (int)wParam;
			CSliderUI *pProgress = static_cast<CSliderUI *>(g_pMainDlg->GetPM()->FindControl(DN_BAR_PROGRASS));
			if (pProgress) pProgress->SetValue(iValue);

			CDuiString szValue;
			szValue.Format(_T("%d%%"), iValue / 300);
			CTextUI *pInfo = static_cast<CTextUI *>(g_pMainDlg->GetPM()->FindControl(DN_TXT_PERCENT));
			if (pInfo) pInfo->SetText(szValue);

			if (30000 == iValue)
			{
				if (g_pTabLayout)
				{
					int currentIndex = g_pTabLayout->GetCurSel();
					g_pTabLayout->SelectItem(currentIndex + 1);
				}
			}
		}
		else if (message == DM_SETDEFID)
		{
			CControlUI *pUI1 = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl(DN_BTN_CLOSE));
			CControlUI *pUI2 = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl(DN_BTN_MIN));
			if (pUI1) pUI1->SetVisible(true);
			if (pUI2) pUI2->SetVisible(true);
			std::map<CDuiString, int >::iterator iter = g_pMainDlg->GetCBMap().find(DN_ERROR);
			if (iter != g_pMainDlg->GetCBMap().end())
				g_pluginParms->ExecuteCodeSegment(iter->second - 1, 0);
		}
		else
		{
			res = CallWindowProc(iter->second, hwnd, message, wParam, lParam);
		}
	}
	return res;
}

// NSIS插件导出函数,NSIS规定函数声明格式如下：
extern "C" __declspec(dllexport) void __cdecl
add(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	// == 添加自己代码
	int i = popint();
	int j = popint();
	int k = i + j;
	pushint(k);
	// ==
}

// =========================================== DUILIB =============================================
static UINT_PTR PluginCallback(enum NSPIM msg)
{
	return 0;
}
///<summary>结束安装进程
/// 输入：空
/// 输出：空
///</summary>
NSISAPI ExitDUISetup(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	CloseHandle(g_hMutex);
	ExitProcess(0);
}
///<summary>创建窗口,存在则窗口置顶
/// 输入：	【字符】窗口标题
/// 		【字符】资源xml文件名	
/// 输出：	【整型】窗口句柄
///</summary>
NSISAPI NewDUISetup(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	extra->RegisterPluginCallback(g_hInstance, PluginCallback);

	CPaintManagerUI::SetInstance(g_hInstance);
	CWndShadow::Initialize(g_hInstance);
	char buf[2][512] = { 0 };
	popstring(buf[0]);
	popstring(buf[1]);
	g_hMutex = CreateMutexA(NULL, TRUE, buf[0]);
	DWORD dwRet = GetLastError();
	if (g_hMutex && ERROR_ALREADY_EXISTS == dwRet)
	{
		CloseHandle(g_hMutex);
		HWND hPre = ::FindWindow(_T("NsInstall"), 0);
		if (hPre)
		{
			SetForegroundWindow(hPre);
			::ShowWindow(hPre, SW_SHOWNORMAL);
		}
		ExitProcess(0);
	}
	else
	{
		String StrBuf0 = Convert::ToString(buf[0]);  
		String StrBuf1 = Convert::ToString(buf[1]);  
		g_pMainDlg = new CDlgMain();
		g_pMainDlg->SetXmlName((TCHAR *)StrBuf1.GetBuffer());
		g_pMainDlg->Create(NULL, (LPCTSTR)StrBuf0, UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX & ~WS_VISIBLE & ~WS_CAPTION, 0, 0, 600, 400);
		g_pMainDlg->CenterWindow();
		g_pTabLayout = static_cast<CTabLayoutUI *>(g_pMainDlg->GetPM()->FindControl(DN_TAB_PAGE));
		pushint(int(g_pMainDlg->GetHWND()));
	}
}
///<summary>显示窗口,并阻塞主线程
/// 输入：	空
/// 输出：	空
///</summary>
NSISAPI ShowPage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);

	g_pMainDlg->ShowWindow();
	MSG msg = { 0 };
	while (::GetMessage(&msg, NULL, 0, 0) && g_bMSGLoopFlag)
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}
///<summary>停止阻塞主线程，进入安装页面
///	输入：	【整型】0为停止阻塞
/// 输出：	空
///</summary>
NSISAPI InstPage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	int bLoop = popint();
	g_bMSGLoopFlag = (bLoop != 0);
}
///<summary>查找控件是否存在
///	输入：	【字符】控件标识名
///	输出：	【整型】0存在，-1不存在
///</summary>
NSISAPI FindControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	char controlName[MAX_PATH] = { 0 };
	String StrControlName = Convert::ToString(controlName);
	popstring(controlName);
	CControlUI *pControl = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl((LPCTSTR)StrControlName));
	if (pControl == NULL)
		pushint(-1);
	else
		pushint(0);
}
///<summary>绑定事件
/// GetFunctionAddress $0 FunctionName
/// nsDui::BindNSIS "控件名" $0
/// NSIS使用实例如下:
/// GetFunctionAddress $0 BtnDir 
/// nsDui::BindNSIS "btn_dir" $0	#绑定安装路径
/// Function BtnDir #更换路径函数定义
/// FunctionEnd
///</summary>
NSISAPI BindNSIS(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	char controlName[MAX_PATH] = { 0 };
	popstring(controlName);

	String wControlName = Convert::ToString(controlName);
	int callbackID = popint();
	g_pMainDlg->SaveToControlCallbackMap((LPCTSTR)wControlName, callbackID);
}

//进度条控制及安装过程信息获取
NSISAPI InstBindNSIS(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);

	CSliderUI *pProgress = static_cast<CSliderUI *>(g_pMainDlg->GetPM()->FindControl(DN_BAR_PROGRASS));
	if (pProgress)
	{
		pProgress->SetMaxValue(30000);
		pProgress->SetValue(0);
	}
	CTextUI *pText = static_cast<CTextUI *>(g_pMainDlg->GetPM()->FindControl(DN_TXT_PERCENT));
	if (pText)pText->SetText(_T("0%"));

	// 接管page instfiles的消息
	g_windowInfoMap[hwndParent] = (WNDPROC)SetWindowLong(hwndParent, GWL_WNDPROC, (long)NSISWindowProc);
	HWND hProgressHWND = FindWindowEx(FindWindowEx(hwndParent, NULL, _T("#32770"), NULL), NULL, _T("msctls_progress32"), NULL);
	g_windowInfoMap[hProgressHWND] = (WNDPROC)SetWindowLong(hProgressHWND, GWL_WNDPROC, (long)NSISWindowProc);

	// NSIS源代码中找到 g_hwndList = FindWindowEx(FindWindowEx(g_hwndParent,NULL,"#32770",NULL),NULL,"SysListView32",NULL);
	HWND g_hwndList = FindWindowEx(FindWindowEx(hwndParent, NULL, _T("#32770"), NULL), NULL, _T("SysListView32"), NULL);
	if (g_hwndList)	g_windowInfoMap[g_hwndList] = (WNDPROC)SetWindowLong(g_hwndList, GWL_WNDPROC, (long)NSISWindowProc);
}
///<summary>进入下N页,NSIS使用实例如下:
/// nsDui::NextPage 1
///</summary>
NSISAPI NextPage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	int diff = popint();
	diff = (diff > 0) ? diff : 1;
	if (g_pTabLayout)
	{
		int currentIndex = g_pTabLayout->GetCurSel();
		g_pTabLayout->SelectItem(currentIndex + diff);
	}
}
///<summary>进入上N页,NSIS使用实例如下:
/// nsDui::PrePage 1  #第二个页面的点自定义返回
///</summary>
NSISAPI PrePage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	int diff = popint();
	diff = (diff > 0) ? diff : 1;
	if (g_pTabLayout)
	{
		int currentIndex = g_pTabLayout->GetCurSel();
		g_pTabLayout->SelectItem(currentIndex - diff);
	}
}
///<summary>重新调整窗口大小</summary>
NSISAPI ReSize(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	int cx = popint();
	int cy = popint();
	g_pMainDlg->ResizeClient(cx-15, cy-15);
}
///<summary>获取勾选状态
/// 输入：【字符】控件标识名
/// 输出：【整型】1-勾选，0-未勾选
/// nsDui::GetChecked "edt_dir"
/// Pop $0
///</summary>
NSISAPI GetChecked(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	String buf = PopTString();
	COptionUI *pChbAgree = static_cast<COptionUI *>(g_pMainDlg->GetPM()->FindControl((LPCTSTR)buf));
	if (!pChbAgree) {
		pushint(-1);
		return;
	}
	pushint(pChbAgree->IsSelected() ? 1 : 0);
}
///<summary>设置勾选状态
/// 输入：【字符】控件标识名
///		  【整型】1-勾选，0-未勾选
/// 输出： 空
/// IntOp $bInstall360 $bInstall360 ^ 1
/// nsDui::SetChecked "opt_isInstall360_1" $bInstall360
///</summary>
NSISAPI SetChecked(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	int iChecked = 0;
	String buf = PopTString();
	iChecked = popint();
	COptionUI *pChbAgree = static_cast<COptionUI *>(g_pMainDlg->GetPM()->FindControl((LPCTSTR)buf));
	if (pChbAgree) pChbAgree->Selected(iChecked != 0);
}
///<summary>设置可用状态
/// 输入：【字符】控件标识名
///		  【整型】1-可用，0-禁用
/// 输出： 空
/// nsDui::SetEnabled "btn_install_one" 1
/// nsDui::SetEnabled "btn_licence_one" 0
///</summary>
NSISAPI SetEnabled(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	String buf = PopTString();
	int bEnabled = popint();
	CControlUI *pUI = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl((LPCTSTR)buf));
	if (pUI == NULL)pushint(-1);
	pUI->SetEnabled(0 != bEnabled);
}
///<summary>设置可视状态
/// 输入：【字符】控件标识名
///		  【整型】1-显示，0-隐藏
/// 输出： 空
///</summary>
NSISAPI SetVisible(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	String buf = PopTString();
	int bShow = popint();
	CControlUI *pUI = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl((LPCTSTR)buf));
	if (pUI == NULL)pushint(-1);
	pUI->SetVisible(0 != bShow);
}
///<summary>设置控件显示文字
/// 输入：【字符】控件标识名
///		  【字符】要显示的文字
/// 输出： 空
///</summary>
NSISAPI SetText(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	String buf = PopTString();
	String text = PopTString();
	CControlUI *pText = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl((LPCTSTR)buf));
	if (pText)pText->SetText((LPCTSTR)text);
}
///<summary>获取控件显示文字
/// 输入：【字符】控件标识名
/// 输出：【字符】控件显示的文字
/// nsDui::GetText "edt_dir"
/// Pop $0
///</summary>
NSISAPI GetText(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char text[1024] = { 0 };
	String buf = PopTString();
	CControlUI *pText = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl((LPCTSTR)buf));
	if (pText) {
		CDuiString StrText = pText->GetText();
		string strText = Convert::ToStringA((LPCTSTR)StrText);
		strcpy(text,strText.c_str());
	}
	pushstring(text);
}
///<summary>选择安装路径
/// 输入：空
/// 输出：【字符】选择的路径
///</summary>
NSISAPI SelectInstallDir(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);

	BROWSEINFO bi;
	memset(&bi, 0, sizeof(BROWSEINFO));

	bi.hwndOwner = g_pMainDlg->GetHWND();
	bi.lpszTitle = _T("选择安装路径");
	bi.ulFlags = 0x0040;

	char szFolderPath[MAX_PATH] = { 0 };
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl == NULL)
	{
		pushstring(szFolderPath);
		return;
	}
	String StrSzFolderPath = Convert::ToString(szFolderPath);
	SHGetPathFromIDList(idl, StrSzFolderPath);
	CEditUI *pEdit = static_cast<CEditUI *>(g_pMainDlg->GetPM()->FindControl(DN_EDT_CHGDIR));
	if (pEdit)pEdit->SetText(StrSzFolderPath);
	pushstring(szFolderPath);
}
///<summary>访问URL地址
/// 输入：【字符】URL
///		  【整型】超时时间（单位：秒）
/// 输出：空
///</summary>
NSISAPI NaviUrl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	String buf = PopTString();
	int timeout = popint();
	g_pMainDlg->NaviUrl((LPCTSTR)buf, timeout);
}
///<summary>创建子窗口
/// 输入：【字符】窗口标题
///		  【字符】资源xml文件名	
/// 输出：【整型】窗口句柄
///</summary>
NSISAPI NewSubDlg(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char buf[2][512] = { 0 };
	popstring(buf[0]);
	popstring(buf[1]);
	String StrBuf0 = Convert::ToString(buf[0]);
	String StrBuf1 = Convert::ToString(buf[1]);
	g_pSubDlg = new CDlgMain();
	g_pSubDlg->SetXmlName((TCHAR *)StrBuf1.GetBuffer());
	g_pSubDlg->Create(g_pMainDlg->GetHWND(), (LPCTSTR)StrBuf0, UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX & ~WS_VISIBLE & ~WS_CAPTION, 0, 0, 600, 400);
	g_pSubDlg->CenterWindow();
	pushint(int(g_pSubDlg->GetHWND()));
}
///<summary>将子窗口控件响应绑定到NSIS脚本的Function
/// 输入：【字符】控件标识名
/// 	  【整型】Function ID
/// 输出： 空
///</summary>
NSISAPI BindSubNSIS(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char controlName[MAX_PATH] = { 0 };
	popstring(controlName);
	int callbackID = popint();
	String StrControlName = Convert::ToString(controlName);
	g_pSubDlg->SaveToControlCallbackMap((LPCTSTR)StrControlName, callbackID);
}
///<summary>显示子窗口，并阻塞主线程
/// 输入：空
/// 输出：空
///</summary>
NSISAPI ShowSubDlg(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	g_pSubDlg->ShowModal();
}
///<summary>关闭子窗口，停止阻塞主线程
/// 输入：空
/// 输出：空
///</summary>
NSISAPI ExitSubDlg(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	g_pSubDlg->Close();
}
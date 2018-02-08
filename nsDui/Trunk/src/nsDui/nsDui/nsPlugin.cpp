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

NSISAPI ExitDUISetup(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	CloseHandle(g_hMutex);
	ExitProcess(0);
}

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
		g_pMainDlg = new CDlgMain();
		g_pMainDlg->SetXmlName(buf[1]);
		g_pMainDlg->Create(NULL, buf[0], UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX & ~WS_VISIBLE & ~WS_CAPTION, 0, 0, 600, 400);
		g_pMainDlg->CenterWindow();
		g_pTabLayout = static_cast<CTabLayoutUI *>(g_pMainDlg->GetPM()->FindControl(DN_TAB_PAGE));
		pushint(int(g_pMainDlg->GetHWND()));
	}
}

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

NSISAPI InstPage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	int bLoop = popint();
	g_bMSGLoopFlag = (bLoop != 0);
}

NSISAPI FindControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	char controlName[MAX_PATH] = { 0 };
	popstring(controlName);
	CControlUI *pControl = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl(controlName));
	if (pControl == NULL)
		pushint(-1);
	else
		pushint(0);
}

NSISAPI BindNSIS(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	char controlName[MAX_PATH] = { 0 };
	popstring(controlName);
	int callbackID = popint();
	g_pMainDlg->SaveToControlCallbackMap(controlName, callbackID);
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

NSISAPI ReSize(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	int cx = popint();
	int cy = popint();
	g_pMainDlg->ResizeClient(cx-15, cy-15);
}

NSISAPI GetChecked(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char buf[512] = { 0 };
	popstring(buf);
	COptionUI *pChbAgree = static_cast<COptionUI *>(g_pMainDlg->GetPM()->FindControl(buf));
	if (!pChbAgree) {
		pushint(-1);
		return;
	}
	pushint(pChbAgree->IsSelected() ? 1 : 0);
}

NSISAPI SetChecked(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char buf[512] = { 0 };
	int iChecked = 0;
	popstring(buf);
	iChecked = popint();
	COptionUI *pChbAgree = static_cast<COptionUI *>(g_pMainDlg->GetPM()->FindControl(buf));
	if (pChbAgree) pChbAgree->Selected(iChecked != 0);
}

NSISAPI SetEnabled(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char buf[512] = { 0 };
	popstring(buf);
	int bEnabled = popint();
	CControlUI *pUI = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl(buf));
	if (pUI == NULL)pushint(-1);
	pUI->SetEnabled(0 != bEnabled);
}

NSISAPI SetVisible(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char buf[512] = { 0 };
	popstring(buf);
	int bShow = popint();
	CControlUI *pUI = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl(buf));
	if (pUI == NULL)pushint(-1);
	pUI->SetVisible(0 != bShow);
}

NSISAPI SetText(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char buf[512] = { 0 };
	char text[512] = { 0 };
	popstring(buf);
	popstring(text);
	CControlUI *pText = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl(buf));
	if (pText)pText->SetText(text);
}

NSISAPI GetText(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char buf[512] = { 0 }, text[1024] = { 0 };
	popstring(buf);
	CControlUI *pText = static_cast<CControlUI *>(g_pMainDlg->GetPM()->FindControl(buf));
	if (pText) strcpy(text, pText->GetText().GetData());
	pushstring(text);
}


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
	SHGetPathFromIDList(idl, szFolderPath);
	CEditUI *pEdit = static_cast<CEditUI *>(g_pMainDlg->GetPM()->FindControl(DN_EDT_CHGDIR));
	if (pEdit)pEdit->SetText(szFolderPath);
	pushstring(szFolderPath);
}

NSISAPI NaviUrl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char buf[512] = { 0 };
	popstring(buf);
	int timeout = popint();
	g_pMainDlg->NaviUrl(buf, timeout);
}

NSISAPI NewSubDlg(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char buf[2][512] = { 0 };
	popstring(buf[0]);
	popstring(buf[1]);
	g_pSubDlg = new CDlgMain();
	g_pSubDlg->SetXmlName(buf[1]);
	g_pSubDlg->Create(g_pMainDlg->GetHWND(), buf[0], UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX & ~WS_VISIBLE & ~WS_CAPTION, 0, 0, 600, 400);
	g_pSubDlg->CenterWindow();
	pushint(int(g_pSubDlg->GetHWND()));
}

NSISAPI BindSubNSIS(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	char controlName[MAX_PATH] = { 0 };
	popstring(controlName);
	int callbackID = popint();
	g_pSubDlg->SaveToControlCallbackMap(controlName, callbackID);
}

NSISAPI ShowSubDlg(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	g_pSubDlg->ShowModal();
}

NSISAPI ExitSubDlg(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT(hwndParent);
	g_pSubDlg->Close();
}
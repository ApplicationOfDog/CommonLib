// SeeFileCert.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MainDlg.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("Skin"));

	CMainDlg dlg;
	dlg.Create(NULL, _T("查看文件签名"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE|WS_EX_ACCEPTFILES);
	dlg.CenterWindow();
	dlg.ShowModal();

	return 0;
}

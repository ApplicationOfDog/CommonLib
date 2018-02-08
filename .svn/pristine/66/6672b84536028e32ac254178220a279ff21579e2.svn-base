#include "stdafx.h"
#include "MainWin.h"

MainWin::MainWin():IWindow(IDR_MAINUI,_T("MainWin"))
{
	REGISTER_EVENT("enterBtn",DUI_MSGTYPE_CLICK,&MainWin::enterBtn_click);
	REGISTER_EVENT("closeBtn",DUI_MSGTYPE_CLICK,&MainWin::closeBtn_click);
}


/////////////////////// event //////////////////////////////////////////////////////
void MainWin::enterBtn_click(IN TNotifyUI& msg)
{
	::ShellExecute(NULL,TEXT("open"),_T("www.baidu.com"),NULL,NULL,SW_NORMAL);
	exit(0);
}
void MainWin::closeBtn_click(IN TNotifyUI& msg)
{
	exit(0);
}


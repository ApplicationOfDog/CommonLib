#include "stdafx.h"
#include "DlgMain.h"
#include "AmHttpSocket.h"

CAmHttpSocket g_httpSock;

struct PARA
{
	string url;
	string ret;
};

static DWORD WINAPI thread_access(LPVOID args)
{
	PARA* p = (PARA*)args;
	String StrUrl = Convert::ToString(p->url);
	g_httpSock.GetPage((LPCTSTR)StrUrl);
	return 0;
}

static int urlAccess(const TCHAR* url, int timeOutSecs)
{
	DWORD id;
	PARA p1;
	string strUrl = Convert::ToStringA(url);
	p1.url = strUrl;
	HANDLE hThread = CreateThread(NULL, 0, thread_access, &p1, 0, &id);

	WaitForSingleObject(hThread, timeOutSecs * 1000);

	if (hThread)
	{
		CloseHandle(hThread);
	}
	return 0;
}

CDlgMain::CDlgMain()
{
	m_winClsName = _T("NsInstall");
	m_xmlName = _T("install.xml");
}

CDlgMain::~CDlgMain()
{	
}

void CDlgMain::NaviUrl(CDuiString szUrl, int timeOutSecs)
{
	urlAccess(szUrl.GetData(), timeOutSecs);
}

void CDlgMain::Notify(TNotifyUI& msg)
{	
	if (_tcsicmp(msg.sType, DUI_MSGTYPE_CLICK) == 0 ||
		 _tcsicmp(msg.sType, DUI_MSGTYPE_TEXTCHANGED) == 0 ||
		 _tcsicmp(msg.sType, DUI_MSGTYPE_LINK) == 0	 ) 
	{		
		std::map<CDuiString, int >::iterator iter = m_controlCallbackMap.find(msg.pSender->GetName());
		if (iter != m_controlCallbackMap.end())
			g_pluginParms->ExecuteCodeSegment( iter->second - 1, 0 );
	}

	if (_tcsicmp(msg.sType, DUI_MSGTYPE_CLICK) == 0)
	{
		OnClick(msg);
	}
}

void CDlgMain::OnClick(TNotifyUI & msg)
{
	if (_tcsicmp(msg.pSender->GetName(), DN_BTN_FASTINST) == 0)
	{

	}
	else if (_tcsicmp(msg.pSender->GetName(), DN_OPT_AGREE) == 0)
	{
		COptionUI* pOption = static_cast<COptionUI*>(m_PaintManager.FindControl(DN_OPT_AGREE));
		bool bChecked = pOption->IsSelected();
		
		CControlUI* pControl1 = static_cast<CControlUI*>(m_PaintManager.FindControl(DN_BTN_FASTINST));
		CControlUI* pControl2 = static_cast<CControlUI*>(m_PaintManager.FindControl(DN_BTN_INSTALL));
		CControlUI* pControl3 = static_cast<CControlUI*>(m_PaintManager.FindControl(DN_BTN_NEXT));
		CControlUI* pControl4 = static_cast<CControlUI*>(m_PaintManager.FindControl(DN_BTN_CUSTOM));

		if (pControl1) pControl1->SetEnabled(!bChecked);
		if (pControl2) pControl2->SetEnabled(!bChecked);
		if (pControl3) pControl3->SetEnabled(!bChecked);
		if (pControl4) pControl4->SetEnabled(!bChecked);
	}
	else if (_tcsicmp(msg.pSender->GetName(), DN_BTN_MIN) == 0)
	{
		::ShowWindow(this->GetHWND(), SW_MINIMIZE);
	}
}

LRESULT CDlgMain::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left
		&& pt.x < rcClient.right - rcCaption.right
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom)
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_VERTICALLAYOUT) == 0)
			return HTCAPTION;
		if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_TEXT) == 0)
			return HTCAPTION;
		if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_GIFANIM) == 0)
			return HTCAPTION;
	}
	return HTCLIENT;
}

LRESULT CDlgMain::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = FALSE;
	switch (uMsg)
	{
	case WM_CLOSE:
		if (m_PaintManager.FindControl(DN_TAB_PAGE))
		{
			ExitProcess(0);
		}
		break;
	default:
		break;
	}
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDlgMain::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT lRet = WindowImplBase::OnCreate(uMsg, wParam, lParam, bHandled);

	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(3);
	m_WndShadow.SetPosition(0, 0);

	return lRet;
}
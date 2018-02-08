#include "StdAfx.h"
#include "IWindow.h"

IWindow::IWindow(IN LPCTSTR skinFile,IN LPCTSTR winClassName,IN LPCTSTR skinFolder,IN UILIB_RESOURCETYPE restype)
{
	if(skinFolder!=NULL)
		_skinFileDir = skinFolder;
	else
		_skinFileDir = _T("skin\\");
	if(skinFile!=NULL)
		_skinFileName = skinFile;
	if(winClassName!=NULL)
		_winClassName = winClassName;
	_resType = restype;
}
IWindow::IWindow(IN UINT skinFile,IN LPCTSTR winClassName,IN UILIB_RESOURCETYPE restype)
{
	_skinFileName.Format(_T("%d"),skinFile); 
	if(winClassName!=NULL)
		_winClassName = winClassName;
	_resType = restype;
}
CDuiString IWindow::GetSkinFolder()
{
	return _skinFileDir;
}
CDuiString IWindow::GetSkinFile()
{
	return _skinFileName;
}
LPCTSTR IWindow::GetWindowClassName() const
{
	return _winClassName;
}
UILIB_RESOURCETYPE IWindow::GetResourceType() const
{
	return _resType;
}
bool IWindow::IsVisible()
{
	if(m_hWnd==NULL||m_hWnd==INVALID_HANDLE_VALUE) return false;
	return ::IsWindowVisible(m_hWnd)==TRUE;
}
void IWindow::CreateAndShow(IN LPCTSTR winName)
{
	if(m_hWnd==NULL)
	{
		Create(NULL,winName, UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		CenterWindow();
	}
	__super::ShowWindow();
}
void IWindow::MoveToRightBottomCorner(IN HWND hWnd,IN int winWidth,IN int winHeight,IN int offsetX,IN int offsetY)
{
	RECT rc;
	GetClientRect(hWnd,&rc);
	int cx = rc.right-rc.left+1;//width
	int cy = rc.bottom-rc.top+1;//height
	UINT uFlags = SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW;
	if(winWidth>0 ||winHeight>0)
	{
		ClearFlag(uFlags,SWP_NOSIZE);	
		if(winWidth>0) cx = winWidth;
		if(winHeight>0) cy = winHeight;
	}
	//取出桌面工作区
	SystemParametersInfo(SPI_GETWORKAREA,NULL,&rc,NULL);
	SetWindowPos(hWnd,NULL,rc.right-cx+offsetX,rc.bottom-cy+offsetY,cx,cy,uFlags);
}
////////////////////////////////////////////////////////////////////////////////
void IWindow::RegisterEvent(IN CDuiString ctlName,IN CDuiString eventName,IN LPEVENT_HANDLER_ROUTINE lpCallback)
{
	CDuiString sKey = ctlName + _T(".") + eventName;
	_eventMap[sKey] = lpCallback;
}
void IWindow::Notify(TNotifyUI& msg)
{
	CDuiString sName = msg.pSender->GetName();
	CDuiString sType = msg.sType;
	CDuiString sKey = sName + _T(".") + sType;

	std::map<CDuiString,LPEVENT_HANDLER_ROUTINE>::iterator ptr;
	ptr = _eventMap.find(sKey);
	if(ptr != _eventMap.end())
	{
		LPEVENT_HANDLER_ROUTINE pRoutine = ptr->second;
		(this->*pRoutine)(msg);
	}
}
LRESULT IWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg,wParam,lParam);
}
void IWindow::PostMessageWithBuffer(IN UINT msgID,IN String& buffer)
{
	send_msg_with_buffer(true,m_hWnd,msgID,buffer);
}
void IWindow::SendMessageWithBuffer(IN UINT msgID,IN String& buffer)
{
	send_msg_with_buffer(false,m_hWnd,msgID,buffer);
}
void IWindow::send_msg_with_buffer(IN bool isPost,IN HWND hWnd,IN UINT msgID,IN String& buffer)
{
	send_msg_with_buffer(isPost,hWnd,msgID,(PBYTE)buffer.c_str(),buffer.GetBytes());
}
void IWindow::send_msg_with_buffer(IN bool isPost,IN HWND hWnd,IN UINT msgID,IN PBYTE buffer,IN int size)
{
	BYTE* pBuffer = NULL;
	bool isNeedFreeBufferInReceived = false;//接收方是否需要释放内存	
	if(buffer!=NULL && size>0)
	{
		pBuffer = new BYTE[size];//这里申请的空间,由接收者释放
		if(pBuffer!=NULL)
		{
			memcpy(pBuffer,buffer,size);
			isNeedFreeBufferInReceived = true;
		}
	}
	else
		pBuffer = buffer;

	if(isPost)
	{
		if(!::PostMessage(hWnd,msgID,(WPARAM)isNeedFreeBufferInReceived,(LPARAM)pBuffer)){
			DEBUG_STRING_ERROR("PostMessage failed,error:%d",GetLastError());
		}
		return;
	}
	::SendMessage(hWnd,msgID,(WPARAM)isNeedFreeBufferInReceived,(LPARAM)pBuffer);
}
void IWindow::FreeBuffer(IN PVOID pBuffer)
{
	if(pBuffer==NULL) return;
	DELETE_MEM(pBuffer);
}
String IWindow::GetStringBuffer(IN LPARAM lParam,IN bool isFree)
{
	if(lParam==NULL) return String::Empty;
	
	String text = (LPCTSTR)lParam;
	if(isFree) FreeBuffer((PVOID)lParam);
	return text;
}
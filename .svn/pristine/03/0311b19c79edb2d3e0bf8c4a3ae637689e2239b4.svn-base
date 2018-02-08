#pragma once

class IWindow: public WindowImplBase
{
protected:
	typedef void (IWindow::*LPEVENT_HANDLER_ROUTINE)(IN TNotifyUI& msg);

	CDuiString			_skinFileDir;	//皮肤文件所在目录,例如:"skin\\"
	CDuiString			_skinFileName;	//当前窗口的皮肤文件名称,例如:"main.xml"
	CDuiString			_winClassName;	//当前窗口类名称,例如:"CMainWin"
	UILIB_RESOURCETYPE	_resType;		//皮肤文件资源类型,默认为:UILIB_FILE,例如:UILIB_RESOURCE
	std::map<CDuiString,LPEVENT_HANDLER_ROUTINE> _eventMap;	


public:
	///<summary>指定xml皮肤文件的形式</summary>
	///<param name="skinFile">皮肤文件名称,例如:"main.xml"</param>
	///<param name="winClassName">窗口类名称,例如:"CMainWin"</param>
	///<param name="skinFolder">皮肤文件所在目录,例如:"skin\\"</param>
	///<param name="restype">皮肤文件资源类型,值为:UILIB_FILE或UILIB_ZIP</param>
	IWindow(IN LPCTSTR skinFile,IN LPCTSTR winClassName,IN LPCTSTR skinFolder=_T("skin\\"),IN UILIB_RESOURCETYPE restype = UILIB_FILE);
	
	///<summary>指定内嵌资源的形式</summary>
	///<param name="skinFile">皮肤文件的资源ID</param>
	///<param name="restype">皮肤文件资源类型,值为:UILIB_RESOURCE或UILIB_ZIPRESOURCE</param>
	IWindow(IN UINT skinFile,IN LPCTSTR winClassName,IN UILIB_RESOURCETYPE restype = UILIB_RESOURCE);

	///<summary>当前窗口是否处于显示状态</summary>
	bool IsVisible();

	///<summary>创建窗口,并在视屏中间ShowModal</summary>
	void CreateAndShow(IN LPCTSTR winName);

	///<summary>
	/// 发送消息到指定窗口,常用于辅助线程通知窗口线程
	/// 注意:Post或Send的消息中的参数由lParam参数携带的,而wParam参数则固定是TRUE值
	/// 注意:消息参数将会被复制到新的内存空间中,接收方需要调用FreeBuffer释放lParam内存空间
	///</summary>
	void PostMessageWithBuffer(IN UINT msgID,IN String& buffer);
	void SendMessageWithBuffer(IN UINT msgID,IN String& buffer);
	///释放参数内存空间
	static void FreeBuffer(IN PVOID pBuffer);//释放缓冲区
	static String GetStringBuffer(IN LPARAM lParam,IN bool isFree=true);

	///<summary>将窗口移动到桌面右下角</summary>
	static void MoveToRightBottomCorner(IN HWND hWnd,IN int winWidth,IN int winHeight,IN int offsetX,IN int offsetY);

protected:
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName() const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual void RegisterEvent(IN CDuiString ctlName,IN CDuiString eventName,IN LPEVENT_HANDLER_ROUTINE lpCallback);
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	static void send_msg_with_buffer(IN bool isPost,IN HWND hWnd,IN UINT msgID,IN String& buffer);
	static void send_msg_with_buffer(IN bool isPost,IN HWND hWnd,IN UINT msgID,IN PBYTE buffer,IN int size);
};

///<summary>控件事件注册,一般在IWindow子类的InitWindow或构造函数函数中注册
/// 例如:REGISTER_EVENT("closeBtn",DUI_MSGTYPE_CLICK,&MainWin::closeBtn_click);
///		 表示将名称为closeBtn的控件的鼠标单击事件绑定到closeBtn_click函数中
///</summary>
#define REGISTER_EVENT(ctlName,eventName,lpCallback) \
	RegisterEvent(_T(ctlName),eventName,(LPEVENT_HANDLER_ROUTINE)lpCallback);
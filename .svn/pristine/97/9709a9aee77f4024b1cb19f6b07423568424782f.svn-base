#include "StdAfx.h"
#include "NetUseConnection.h"
#include <WinNetWk.h>
#pragma comment(lib, "Mpr.lib")

namespace System
{
	namespace Net
	{
		NetUseConnection::NetUseConnection()
		{
			_initializeCode = -111;
			_status = 0;
			ZeroMemory(_deviceName,sizeof(_deviceName));
			ZeroMemory(_userName,sizeof(_userName));
			ZeroMemory(_remoteName,sizeof(_remoteName));
			ZeroMemory(_localName,sizeof(_localName));
		}
		/// <summary>
		/// 用于将计算机与共享的资源相连接（建立磁盘映射）
		/// 相当于DOS命令 net use \\F250\bnd_dat\bndmap PassWord /user:UserName
		/// NetworkConnection.Connect(@"\\F250\bnd_dat\bndmap", null, @"xm\weizhiwei", null);
		/// </summary>
		/// <param name="remotePath">远程地址</param>
		/// <param name="localPath">本地映射磁盘</param>
		/// <param name="username">访问的用户名</param>
		/// <param name="password">密码</param>
		/// <returns>TRUE:连接成功，FALSE：连接失败</returns>
		bool NetUseConnection::Connect(LPCTSTR remoteName, LPCTSTR username, LPCTSTR password)
		{
			return Connect(remoteName,_T(""), username,password);
		}
		bool NetUseConnection::Connect(LPCTSTR remoteName, LPCTSTR localName, LPCTSTR username, LPCTSTR password)
		{
			int len =0;
			if(remoteName!=NULL){
				len = (int)_tcslen(remoteName);
				memcpy(_remoteName,remoteName,min((len+1)*sizeof(TCHAR),sizeof(_remoteName)));
				memcpy(_deviceName,_remoteName,sizeof(_deviceName));
			}
			if(localName!=NULL){
				len = (int)_tcslen(localName);
				memcpy(_localName,localName,min((len+1)*sizeof(TCHAR),sizeof(_localName)));
				memcpy(_deviceName,_remoteName,sizeof(_deviceName));
			}
			if(username!=NULL){
				len = (int)_tcslen(username);
				memcpy(_userName,username,min((len+1)*sizeof(TCHAR),sizeof(_userName)));
			}			

			NETRESOURCE share_driver;
			share_driver.dwScope = RESOURCE_GLOBALNET;
			share_driver.dwType = RESOURCETYPE_DISK;
			share_driver.dwDisplayType = RESOURCEDISPLAYTYPE_SHARE;
			share_driver.dwUsage = RESOURCEUSAGE_CONNECTABLE;
			share_driver.lpLocalName = (LPTSTR)localName;
			share_driver.lpRemoteName =_remoteName;
			share_driver.lpComment=NULL;
			share_driver.lpProvider=NULL;

			Disconnect();	
			_status = ::WNetAddConnection2(&share_driver, password, username, CONNECT_UPDATE_PROFILE);
			return _status == NO_ERROR;
		}
		/// <summary>
		/// 获取连接状态
		/// </summary>
		/// <param name="outStatus">连接错误码</param>
		/// <param name="message">连接消息</param>
		/// <returns>是否连接成功，true成功，false失败</returns>
		int NetUseConnection::Status(){
			return _status;
		}
		//String NetUseConnection::StatusText()
		//{
		//	String message;
		//	if (_status == _initializeCode){
		//		message = _T("none connection.");
		//		return message;
		//	}

		//	message=String::Format(_T("Status:%d,"),_status);

		//	if (_status == ERROR_INVALID_PASSWORD)
		//		message += String::Format(_T("账号%s密码错误"),(LPCTSTR)_userName);
		//	else if(_status==ERROR_ACCESS_DENIED)
		//		message += String::Format(_T("账号%s的权限不足"),(LPCTSTR)_userName);
		//	else if (_status == ERROR_BAD_NETPATH)
		//		message += String::Format(_T("路径(%s)错误"),(LPCTSTR) _remoteName);
		//	else if (_status == ERROR_LOGON_FAILURE)//Logon failure: unknown user name or bad password
		//		message += String::Format(_T("unknown user name(%s) or bad password"),(LPCTSTR) _userName);
		//	else if(_status==NO_ERROR)
		//		message+=_T("connected succeed.");
		//	else
		//		message+=Environment::LastErrorText(_status);

		//	return message;
		//}
		LPCTSTR NetUseConnection::DeviceName(){
			return _deviceName;
		}
		LPCTSTR NetUseConnection::RemoteName(){
			return _remoteName;
		}
		LPCTSTR NetUseConnection::LocalName(){
			return _localName;
		}
		/// <summary>
		/// 断开共享连接
		/// 相当于net use {0} /delete /yes 删除与共享资源连接
		/// </summary>
		/// <param name="remotePath"></param>
		/// <returns></returns>
		bool NetUseConnection::Disconnect()
		{
			if (_deviceName[0]==0) return false;
			DWORD result =  ::WNetCancelConnection2(_deviceName,CONNECT_UPDATE_PROFILE,TRUE);//返回0表示执行成功，否则为发生错误
			return result==ERROR_SUCCESS;
		}
		//bool NetUseConnection::Upload(LPCTSTR localFilePath,LPCTSTR remoteDir)
		//{
		//	ifstream ifile(localFilePath);
		//	if(!ifile.is_open()){
		//		::SetLastError(ERROR_FILE_NOT_FOUND);
		//		return false;
		//	}
		//	String fileName=Path::GetFileName(localFilePath);
		//	String destFilePath;
		//	if(!remoteDir.IsEmptyOrWhiteSpace())
		//		destFilePath = Path::Combine(_deviceName,remoteDir,fileName);
		//	else
		//		destFilePath = Path::Combine(_deviceName,fileName);

		//	return File::Copy(localFilePath,destFilePath,TRUE);
		//}
	}
}
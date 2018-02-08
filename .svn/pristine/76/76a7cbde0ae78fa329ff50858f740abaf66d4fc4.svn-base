#pragma once
namespace System
{
	namespace Net
	{
		/// <summary>
		/// 网络共享连接
		/// 用于将计算机与共享的资源相连接（建立磁盘映射）
		/// 相当于DOS命令 net use \\F250\bnd_dat\bndmap PassWord /user:UserName		
		/// </summary>
		class API_SYSTEM NetUseConnection
		{
		private:
			int _initializeCode;
			int _status;
			TCHAR _deviceName[MAX_PATH];
			TCHAR _userName[MAX_PATH];
			TCHAR _remoteName[MAX_PATH];
			TCHAR _localName[MAX_PATH];

		public:
			///获取连接状态
			int Status();
			//String StatusText();
			///获取设备名称，如果有本地磁盘映射就是本地映射名称，否则为共享路径
			LPCTSTR DeviceName();
			///远程连接地址
			LPCTSTR RemoteName();
			///本地映射磁盘
			LPCTSTR LocalName();

		public:
			NetUseConnection();
			/// <summary>
			/// 用于将计算机与共享的资源相连接（建立磁盘映射）
			/// 相当于DOS命令 net use \\F250\bnd_dat\bndmap PassWord /user:UserName
			/// NetworkConnection.Connect(@"\\F250\bnd_dat\bndmap", null, @"xm\weizhiwei", null);
			/// </summary>
			/// <param name="remotePath">远程地址</param>
			/// <param name="localPath">本地映射磁盘,ps."X:"</param>
			/// <param name="username">访问的用户名</param>
			/// <param name="password">访问的用户密码</param>
			/// <returns>TRUE:连接成功，FALSE：连接失败（用GetLastError查看错误码）</returns>
			bool Connect(LPCTSTR remotePath, LPCTSTR username, LPCTSTR password);
			bool Connect(LPCTSTR remotepath, LPCTSTR localPath, LPCTSTR username, LPCTSTR password);
			/// <summary>
			/// 断开共享连接
			/// 相当于net use {0} /delete /yes 删除与共享资源连接
			/// </summary>
			bool Disconnect();
			/// <summary>
			/// 上传本地文件到共享
			/// </summary>
			/// <param name="localFilePath">本地文件地址</localFilePath>
			/// <param name="remoteDir">远程目录</shareDir>
			/// <returns>TRUE:成功，FALSE：失败（用GetLastError查看错误码）</returns>
			//bool Upload(LPCTSTR localFilePath,LPCTSTR remoteDir=_T(""));
		};

		/// 常用的net use 命令:
		/// 查看当前计算机所有共享连接: net use
		/// 删除所有的共享连接: net use /delete * 
	}
}
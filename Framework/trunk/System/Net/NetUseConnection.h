#pragma once
namespace System
{
	namespace Net
	{
		/// <summary>
		/// ���繲������
		/// ���ڽ�������빲�����Դ�����ӣ���������ӳ�䣩
		/// �൱��DOS���� net use \\F250\bnd_dat\bndmap PassWord /user:UserName		
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
			///��ȡ����״̬
			int Status();
			//String StatusText();
			///��ȡ�豸���ƣ�����б��ش���ӳ����Ǳ���ӳ�����ƣ�����Ϊ����·��
			LPCTSTR DeviceName();
			///Զ�����ӵ�ַ
			LPCTSTR RemoteName();
			///����ӳ�����
			LPCTSTR LocalName();

		public:
			NetUseConnection();
			/// <summary>
			/// ���ڽ�������빲�����Դ�����ӣ���������ӳ�䣩
			/// �൱��DOS���� net use \\F250\bnd_dat\bndmap PassWord /user:UserName
			/// NetworkConnection.Connect(@"\\F250\bnd_dat\bndmap", null, @"xm\weizhiwei", null);
			/// </summary>
			/// <param name="remotePath">Զ�̵�ַ</param>
			/// <param name="localPath">����ӳ�����,ps."X:"</param>
			/// <param name="username">���ʵ��û���</param>
			/// <param name="password">���ʵ��û�����</param>
			/// <returns>TRUE:���ӳɹ���FALSE������ʧ�ܣ���GetLastError�鿴�����룩</returns>
			bool Connect(LPCTSTR remotePath, LPCTSTR username, LPCTSTR password);
			bool Connect(LPCTSTR remotepath, LPCTSTR localPath, LPCTSTR username, LPCTSTR password);
			/// <summary>
			/// �Ͽ���������
			/// �൱��net use {0} /delete /yes ɾ���빲����Դ����
			/// </summary>
			bool Disconnect();
			/// <summary>
			/// �ϴ������ļ�������
			/// </summary>
			/// <param name="localFilePath">�����ļ���ַ</localFilePath>
			/// <param name="remoteDir">Զ��Ŀ¼</shareDir>
			/// <returns>TRUE:�ɹ���FALSE��ʧ�ܣ���GetLastError�鿴�����룩</returns>
			//bool Upload(LPCTSTR localFilePath,LPCTSTR remoteDir=_T(""));
		};

		/// ���õ�net use ����:
		/// �鿴��ǰ��������й�������: net use
		/// ɾ�����еĹ�������: net use /delete * 
	}
}
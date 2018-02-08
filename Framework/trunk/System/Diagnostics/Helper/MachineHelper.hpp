#pragma once

namespace System
{
	namespace Diagnostics
	{		
		class MachineHelper
		{
		public:
			/// <summary>
			/// 获取MAC地址字符串（字符串,长度最大为17个字符,传入的缓冲区应该至少有18个字符长度）
			/// </summary>
			/// <param name="pBuffer">返回MAC地址字符串的缓冲区</param>
			/// <param name="separator">中间的分隔符,0:表示不需要分隔符</param>
			/// <returns>0:成功,其他值为:错误码</returns>
			static int GetMacAddr(OUT char pMacAddr[MACADDR_BUFFER_LEN],IN char separator='-');
			/// <summary>
			/// 获取本地计算机当前使用的IP地址，例如:192.168.1.248
			/// </summary>
			static bool GetIPAddress(OUT char ipAddr[IPADDR_BUFFER_LEN]);
#if IS_WINDOWS_OS
			static void GetCPUID(OUT DWORD* id,OUT DWORD* name,OUT DWORD* reserved1,OUT DWORD* reserved2);			
#endif
		};
	}	
}
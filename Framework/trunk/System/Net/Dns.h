#pragma once
#include <string>
#include <vector>
using namespace std;

namespace System
{
	namespace Net
	{
		// 摘要:
		//     提供简单的域名解析功能。
		class API_SYSTEM Dns
		{
		public:
			/// <summary>
			/// 获取本地计算机的主机名
			/// </summary>
			/// <returns>包含本地计算机的 DNS 主机名的字符串</returns>
			static bool GetHostName(OUT char* pBuffer,IN int szBuffer);
			static vector<std::string> GetHostIPAddressList();
		};
	}	
}
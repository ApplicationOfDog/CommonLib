#pragma once
#include <string>
#include <vector>
using namespace std;

namespace System
{
	namespace Net
	{
		// ժҪ:
		//     �ṩ�򵥵������������ܡ�
		class API_SYSTEM Dns
		{
		public:
			/// <summary>
			/// ��ȡ���ؼ������������
			/// </summary>
			/// <returns>�������ؼ������ DNS ���������ַ���</returns>
			static bool GetHostName(OUT char* pBuffer,IN int szBuffer);
			static vector<std::string> GetHostIPAddressList();
		};
	}	
}
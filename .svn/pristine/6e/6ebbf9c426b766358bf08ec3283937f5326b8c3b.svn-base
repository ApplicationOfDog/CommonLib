#pragma once
#include <string>
#include <vector>
using namespace std;

//网卡地址的最大长度,例如:D4-BE-D9-D4-53-3E
#define MACADDR_BUFFER_LEN 18
//IP地址缓冲区大小,例如:192.168.201.222
#define IPADDR_BUFFER_LEN 16

namespace System{
	namespace Diagnostics{		
		// 摘要:
		//     获取机器信息
		class API_SYSTEM Machine
		{
		public:
			/// <summary>
			/// 判断宿主主机是否为低位优先（little endian）的字节存储机制
			/// </summary>
			/// <returns>true:little endian,false:big endian</returns>
			static bool IsLittleEndian();
			/// <summary>
			/// 获取MAC地址字符串,例如:D4-BE-D9-D4-53-3E
			/// </summary>
			/// <param name="macAddr">返回MAC地址字符串的缓冲区,至少有18个字符长度</param>
			/// <param name="separator">中间的分隔符,0:表示不需要分隔符</param>
			/// <returns>0:成功,其他值为:错误码</returns>
			static int GetMacAddr(OUT char macAddr[MACADDR_BUFFER_LEN],IN char separator='-');
			/// <summary>
			/// 获取本地计算机当前使用的IP地址，例如:192.168.1.248
			/// </summary>
			/// <returns>true:成功,false:获取失败</returns>
			static bool GetIPAddress(OUT char ipAddr[IPADDR_BUFFER_LEN]);
#if IS_WINDOWS_OS
		public:
			/// <summary>
			/// 获取本地计算机的主机名
			/// </summary>
			static std::string GetHostName();
			/// <summary>
			/// 获取本地计算机的IP地址列表
			/// </summary>
			static vector<std::string> GetHostIPAddressList();
			
			/// <summary>
			/// 生成机器的CPUID
			/// </summary>
			/// <returns>
			/// 返回35个字符长度的cpuid字符串,
			/// ps.BFEBFBFF-000206A7-00100800-1D9AE3BF
			/// </returns>
			static std::string GetCPUID();
			/// <summary>
			/// 生成机器的CPUID
			/// </summary>
			/// <param name="id">cpuid</param>
			/// <param name="name">cpuName</param>
			/// <param name="reserved1">CPUReserved1</param>
			/// <param name="reserved2">CPUReserved2</param>
			static void GetCPUID(OUT DWORD* id,OUT DWORD* name,OUT DWORD* reserved1,OUT DWORD* reserved2);
			/// <summary>
			/// 获取分区序列号
			/// </summary>
			/// <param name="driver">系统盘符,空表示默认获取系统盘，例如：C,D</param>
			/// <returns>分区序列号,0:表示获取失败</returns>
			static DWORD GetPartitionSN(IN LPCTSTR driver=NULL);
#endif
		};

}}
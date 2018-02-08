#include "stdafx.h"
#include "Machine.hpp"
#include "Helper/MachineHelper.hpp"
#if IS_WINDOWS_OS
#include "../Net/Dns.h"
using namespace System::Net;
#endif

namespace System{
	namespace Diagnostics{	

		bool Machine::IsLittleEndian()
		{
			union {
				char c;
				int i;
			} u;
			//宿主主机如果为高位优先big endian的机器利用u.i的高位定义u.c，所以对于lcc的32位目标机，在将u.c赋值为1的同时u.i被设置成0x01000000
			//宿主主机如果为低位优先little endian的机器u.i的低位定义u.c，所以下面代码将u.c赋值为1的同时也将u.i设置成1
            u.i = 0;
            u.c = 1;
			/*对于上面的union u变量在内存中的分布情况都一样,不管是big endian还是Little endian。如下（假设u的起始地址是0x0012fefc）
											big endian 			little endian
							0x0012fefc		  01						01
							0x0012fefd		  00						00
							0x0012fefe		  00						00
							0x0012feff		  00						00
							u.i的值		0x01 00 00 00			0x00 00 00 00 01	
							上面是u变量在内存中的存储结构，但这种存储结构对于big endian和Little endian却是不同的值。
			*/
			return u.i==1;
		}
		int Machine::GetMacAddr(OUT char macAddr[MACADDR_BUFFER_LEN],IN char separator)
		{
			return MachineHelper::GetMacAddr(macAddr,separator);
		}
		bool Machine::GetIPAddress(OUT char ipAddr[IPADDR_BUFFER_LEN])
		{
			return MachineHelper::GetIPAddress(ipAddr);
		}
#if IS_WINDOWS_OS
		std::string Machine::GetHostName()
		{
			std::string result;
			char name[_MAX_FNAME+1]={0};
			if(Dns::GetHostName(name,sizeof(name))) result = name;
			return result;
		}
		vector<std::string> Machine::GetHostIPAddressList()
		{
			return Dns::GetHostIPAddressList();
		}
		/// <summary>
		/// 生成机器的CPUID
		/// </summary>
		/// <param name="id">cpuid</param>
		/// <param name="name">cpuName</param>
		/// <param name="reserved1">CPUReserved1</param>
		/// <param name="reserved2">CPUReserved2</param>
		void Machine::GetCPUID(OUT DWORD* id,OUT DWORD* name,OUT DWORD* reserved1,OUT DWORD* reserved2)
		{
			MachineHelper::GetCPUID(id,name,reserved1,reserved2);
		}
		/// <summary>
		/// 生成机器的CPUID
		/// </summary>
		/// <returns>
		/// 返回35个字符长度的cpuid字符串,
		/// ps.BFEBFBFF-000206A7-00100800-1D9AE3BF
		/// </returns>
		std::string Machine::GetCPUID()
		{
			//用于输出 CPU的名称、CPU的Reserved1、CPU的Reserved2、CPU的ID
			DWORD name, reserved1,reserved2,id;
			MachineHelper::GetCPUID(&id,&name,&reserved1,&reserved2);
			
			char buffer[100]={0};
			sprintf_s(buffer,sizeof(buffer),"%08X-%08X-%08X-%08X",id,name,reserved1,reserved2);
			std::string result = buffer;
			return result;
		}
		/// <summary>
		/// 获取分区序列号
		/// </summary>
		/// <param name="driver">系统盘符,空表示默认获取系统盘，例如：C,D</param>
		/// <returns>分区序列号,0:表示获取失败</returns>
		DWORD Machine::GetPartitionSN(IN LPCTSTR driver)
		{
			TCHAR sysDir[MAX_PATH]={0};
			if(driver==NULL){				
				::GetSystemDirectory(sysDir,MAX_PATH-1);
				driver=sysDir;
			}
			
			TCHAR drv[]=_T("X:\\");
			drv[0]=driver[0];
			DWORD dwSerialNum = 0;
			if (::GetVolumeInformation(drv,NULL,NULL,&dwSerialNum,NULL,NULL,NULL,NULL))
				return dwSerialNum;

			return 0;
		}
#endif
	}
}
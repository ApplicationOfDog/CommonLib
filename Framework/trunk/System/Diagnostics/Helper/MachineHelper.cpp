#include "stdafx.h"
#include "MachineHelper.hpp"

#if IS_WINDOWS_OS
#include <IPHlpApi.h>
#include <Winsock2.h>
#pragma comment(lib,"IPHlpApi.lib")
#pragma comment(lib,"Ws2_32.lib")
#include <malloc.h>
#include <stdio.h>
#endif

namespace System
{
	namespace Diagnostics
	{
		int MachineHelper::GetMacAddr(OUT char macAddr[MACADDR_BUFFER_LEN],IN char separator)
		{
			ZeroMemory(macAddr,MACADDR_BUFFER_LEN);
#if IS_WINDOWS_OS
			int szBuffer=MACADDR_BUFFER_LEN;
			int iRet = 0;
			PIP_ADAPTER_INFO pIPinfo = NULL;	
			bool bIsFind = false;
			unsigned long ulIp = 0, ulMask = 0;
			unsigned long nError;	

			PIP_ADAPTER_INFO pAdapterInfo = NULL;
			PIP_ADAPTER_INFO pAdapter = NULL;
			DWORD dwRetVal = 0;
			ULONG ulOutBufLen = 0;

			if (::GetAdaptersInfo( NULL,&ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
			{
				pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen); 
				if (pAdapterInfo == NULL) return 1;
			}
			pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen); 
			nError = ::GetAdaptersInfo( pAdapterInfo, &ulOutBufLen);
			switch(nError) 
			{
			case 0:			
				break;
			case ERROR_NO_DATA:
				iRet = 1;
				break;
			case ERROR_NOT_SUPPORTED:
				iRet = 2;
				break;
			default:
				iRet = 4;
				break;
			}
			if (0 != iRet) {
				free(pAdapterInfo);
				return iRet;
			}

			char ip[32]={0};
			GetIPAddress(ip);
			int m_ulSerIP= inet_addr(ip);

			//////////////////////////////////////////////////////////////////////////
			char format[32]={0};
			if(separator!=0)
				sprintf_s(format,sizeof(format),"%s%c%s%c%s%c%s%c%s%c%s",
				"%02X",separator,"%02X",separator,"%02X",separator,"%02X",separator,"%02X",separator,"%02X");
			else
				strcpy_s(format,sizeof(format),"%02X%02X%02X%02X%02X%02X");//char* format="%02X-%02X-%02X-%02X-%02X-%02X";//%02X:%02X:%02X:%02X:%02X:%02X
			pIPinfo = pAdapterInfo;
			do 
			{
				if (0 != strcmp ("0.0.0.0", pIPinfo->IpAddressList.IpAddress.String)){
					ulIp = inet_addr (pIPinfo->IpAddressList.IpAddress.String);
					ulMask = inet_addr (pIPinfo->IpAddressList.IpMask.String);

					if ((m_ulSerIP & ulMask) == (ulIp & ulMask)) {
						sprintf_s (macAddr, szBuffer,format,
							pIPinfo->Address[0], pIPinfo->Address[1],
							pIPinfo->Address[2], pIPinfo->Address[3],
							pIPinfo->Address[4], pIPinfo->Address[5]);				
						bIsFind = true;
					}
				}
				pIPinfo = pIPinfo->Next;
			} while(pIPinfo);

			if (!bIsFind){
				pIPinfo = pAdapterInfo;
				do {
					if (0 != strcmp ("", pIPinfo->GatewayList.IpAddress.String)) {
						sprintf_s (macAddr, szBuffer, format,
							pIPinfo->Address[0], pIPinfo->Address[1],
							pIPinfo->Address[2], pIPinfo->Address[3],
							pIPinfo->Address[4], pIPinfo->Address[5]);
						bIsFind = true;
					}
					pIPinfo = pIPinfo->Next;
				} while (pIPinfo);
			}  	

			free(pAdapterInfo);
			if (bIsFind) return ERROR_SUCCESS;
			return -1;
#elif IS_MAC_OS
			FILE *fp = popen("ifconfig en0 | awk '/ether/{print $2}'", "r");
			if(fp==NULL) return ERROR_PIPE_NOT_CONNECTED;
			fscanf(fp, "%s",macAddr);//得到mac地址,PS."00:0c:29:e7:68:63"
			pclose(fp);
			for (char* p = macAddr;separator!=':' &&  *p;p++){ //分隔符替换
				if(*p!=':') continue;
				*p = separator;
			}			
			return ERROR_SUCCESS;
#endif
		}
		////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 获取IP地址字符串（字符串,长度为8个字符,传入的缓冲区应该至少有9个字符长度）
		/// </summary>
		/// <param name="ip">返回的IP地址字符串</param>
		/// <returns>TRUE:成功,false:失败</returns>
		bool MachineHelper::GetIPAddress(OUT char ipAddr[IPADDR_BUFFER_LEN])
		{
			ZeroMemory(ipAddr,IPADDR_BUFFER_LEN);
#if IS_WINDOWS_OS
			WORD wVersionRequested = MAKEWORD(1, 1);
			WSADATA wsaData;
			if (WSAStartup(wVersionRequested, &wsaData))  return false;
			char *hostname = new char[256];
			int res = ::gethostname(hostname, 256);
			if (res != 0) return false;

			hostent* pHostent = ::gethostbyname(hostname);
			delete []hostname;
			if (pHostent==NULL) return false;
			hostent& he = *pHostent;
			sockaddr_in sa;
			memcpy ( &sa.sin_addr.s_addr, he.h_addr_list[0],he.h_length);
			WSACleanup();
			char* _ip = inet_ntoa(sa.sin_addr);
			//strcpy_s(ipAddr,IPADDR_BUFFER_LEN-1,_ip);
			strncpy(ipAddr,_ip,IPADDR_BUFFER_LEN-1);
			return true;
#elif IS_MAC_OS
			FILE *fp = popen("ifconfig  | grep -E 'inet.[0-9]' | grep -v '127.0.0.1' | awk '{ print $2}'", "r");
			if(fp==NULL) return false;
			fscanf(fp, "%s",ipAddr);//得到mac地址,PS."192.168.111.112"
			pclose(fp);		
			return true;
#endif
		}
#if IS_WINDOWS_OS
		void MachineHelper::GetCPUID(OUT DWORD* id,OUT DWORD* name,OUT DWORD* reserved1,OUT DWORD* reserved2)
		{
#ifndef X64  //x64不支持汇编编译
			//用于输出 CPU的名称、CPU的Reserved1、CPU的Reserved2、CPU的ID
			DWORD dwCPUName, dwCPUReserved1,dwCPUReserved2,dwCPUID;
			__asm
			{
				PUSHAD//压栈(把前面进行的东西的信息先收集起来)
					MOV EAX, 1//这两行是规定用法，是Intel的CPU的标准取CPUID的命令，没什么可说。
					cpuid//这两行用完后，EAX、EBX、ECX、EDX中就有你想要的信息了。
					MOV dwCPUName, EAX//将EAX中的值放到dwCPUName中
					MOV dwCPUReserved1, EBX//将EBX中的值放到dwCPUReserved1中
					MOV dwCPUReserved2, ECX//将ECX中的值放到dwCPUReserved2中
					MOV dwCPUID, EDX//将EDX中的值放到dwCPUID中
					POPAD//出栈//（把前面进行的东西的信息释放出来）
			}

			if(id!=NULL) *id=dwCPUID;
			if(name!=NULL) *name=dwCPUName;
			if(reserved1!=NULL) *reserved1=dwCPUReserved1;
			if(reserved2!=NULL) *reserved2=dwCPUReserved2;
#endif			
		}
#endif
	}
}
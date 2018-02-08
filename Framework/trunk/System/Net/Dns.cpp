#include "stdafx.h"
#include <WinSock.h>
#include "Sockets/SocketError.h"
#include "Dns.h"
#pragma comment( lib, "ws2_32.lib") 

namespace System
{
	namespace Net
	{
		bool Dns::GetHostName(OUT char* pBuffer,IN int szBuffer)
		{
			WSAData lpWSAData;
			Sockets::SocketError socketError = (Sockets::SocketError)::WSAStartup(0x202,&lpWSAData);
			if (socketError != Sockets::Success){
				throw runtime_error("SocketException");
				return false;
			}
			

			char hostName[0x100]={0};
			int result = ::gethostname(hostName,sizeof(hostName)/sizeof(char));
			::WSACleanup();
			if(result!=SOCKET_ERROR)
			{
				int len = (int)strlen(hostName);
				memcpy(pBuffer,hostName,min(len,szBuffer));
				return true;
			}
			int error = WSAGetLastError();
			assert(result != SOCKET_ERROR);
			return false;
		}

		vector<string> Dns::GetHostIPAddressList()
		{
			vector<string> result;

			//1.��ʼ��wsa
			WSAData lpWSAData;
			Sockets::SocketError socketError = (Sockets::SocketError)::WSAStartup(0x202,&lpWSAData);
			if (socketError != Sockets::Success){
				throw runtime_error("SocketException");
				return result;
			}

			//2.��ȡ������
			char hostName[0x100]={0};
			int ret = ::gethostname(hostName,sizeof(hostName)/sizeof(char));
			if(ret==SOCKET_ERROR){
				int error = WSAGetLastError();
				assert(ret != SOCKET_ERROR);
				::WSACleanup();
				return result;
			}

			//3.��ȡ����ip
			HOSTENT* host=gethostbyname(hostName);
			if (host==NULL){
				int error = WSAGetLastError();
				assert(host != NULL);
				::WSACleanup();
				return result;
			}

			//4.���ת��Ϊchar*����������
			std::string ipItem;
			for (int i=0;i<host->h_length;i++)
			{
				in_addr* addr=(in_addr*)*host->h_addr_list;
				ipItem = inet_ntoa(addr[i]);
				result.push_back(ipItem);
			}
			::WSACleanup();
			return result;
		}
	}
}
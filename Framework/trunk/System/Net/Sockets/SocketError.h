#pragma once

namespace System
{
	namespace Net
	{
		namespace Sockets
		{
			// ժҪ:
			//     ���� System.Net.Sockets.Socket ��Ĵ�����롣
			enum SocketError
			{
				// ժҪ:
				//     ������δָ���� System.Net.Sockets.Socket ����
				Unkown = -1,
				//
				// ժҪ:
				//     System.Net.Sockets.Socket �����ɹ���
				Success = 0,
				//
				// ժҪ:
				//     ���� System.Net.Sockets.Socket �ѹرգ��ص��Ĳ�������ֹ��
				OperationAborted = 995,
				//
				// ժҪ:
				//     Ӧ�ó���������һ���޷�������ɵ��ص�������
				IOPending = 997,
				//
				// ժҪ:
				//     ��ȡ����ֹ System.Net.Sockets.Socket ���õĲ�����
				Interrupted = 10004,
				//
				// ժҪ:
				//     ����ͼͨ���������Ȩ�޽�ֹ�ķ�ʽ���� System.Net.Sockets.Socket��
				AccessDenied = 10013,
				//
				// ժҪ:
				//     �����׽����ṩ�����⵽��Ч��ָ���ַ��
				Fault = 10014,
				//
				// ժҪ:
				//     �� System.Net.Sockets.Socket ��Ա�ṩ��һ����Ч������
				InvalidArgument = 10022,
				//
				// ժҪ:
				//     �����׽����ṩ�����д򿪵��׽���̫�ࡣ
				TooManyOpenSockets = 10024,
				//
				// ժҪ:
				//     �Է���ֹ���׽��ֵĲ�������������ɡ�
				WouldBlock = 10035,
				//
				// ժҪ:
				//     ��ֹ�������ڽ����С�
				InProgress = 10036,
				//
				// ժҪ:
				//     ����ֹ�� System.Net.Sockets.Socket ����һ���������ڽ����С�
				AlreadyInProgress = 10037,
				//
				// ժҪ:
				//     �Է��׽��ֳ��� System.Net.Sockets.Socket ������
				NotSocket = 10038,
				//
				// ժҪ:
				//     �ڶ� System.Net.Sockets.Socket �Ĳ�����ʡ���˱���ĵ�ַ��
				DestinationAddressRequired = 10039,
				//
				// ժҪ:
				//     ���ݱ�̫����
				MessageSize = 10040,
				//
				// ժҪ:
				//     �� System.Net.Sockets.Socket ��Э�����Ͳ���ȷ��
				ProtocolType = 10041,
				//
				// ժҪ:
				//     �� System.Net.Sockets.Socket ʹ����δ֪����Ч����֧�ֵ�ѡ��򼶱�
				ProtocolOption = 10042,
				//
				// ժҪ:
				//     δʵ�ֻ�δ����Э�顣
				ProtocolNotSupported = 10043,
				//
				// ժҪ:
				//     �ڴ˵�ַ���в����ڶ�ָ�����׽������͵�֧�֡�
				SocketNotSupported = 10044,
				//
				// ժҪ:
				//     Э���岻֧�ֵ�ַ�塣
				OperationNotSupported = 10045,
				//
				// ժҪ:
				//     δʵ�ֻ�δ����Э���塣
				ProtocolFamilyNotSupported = 10046,
				//
				// ժҪ:
				//     ��֧��ָ���ĵ�ַ�塣 ���ָ���� IPv6 ��ַ���δ�ڱ��ؼ�����ϰ�װ IPv6 ��ջ����᷵�ش˴��� ���ָ���� IPv4 ��ַ���δ�ڱ��ؼ�����ϰ�װ
				//     IPv4 ��ջ����᷵�ش˴���
				AddressFamilyNotSupported = 10047,
				//
				// ժҪ:
				//     ͨ����ֻ����ʹ�õ�ַһ�Ρ�
				AddressAlreadyInUse = 10048,
				//
				// ժҪ:
				//     ѡ���� IP ��ַ�ڴ�����������Ч��
				AddressNotAvailable = 10049,
				//
				// ժҪ:
				//     ���粻���á�
				NetworkDown = 10050,
				//
				// ժҪ:
				//     �����ڵ�Զ��������·�ɡ�
				NetworkUnreachable = 10051,
				//
				// ժҪ:
				//     Ӧ�ó��������ѳ�ʱ������������ System.Net.Sockets.SocketOptionName.KeepAlive��
				NetworkReset = 10052,
				//
				// ժҪ:
				//     �������� .NET Framework ������׽����ṩ������ֹ��
				ConnectionAborted = 10053,
				//
				// ժҪ:
				//     ��������Զ�̶Եȼ�������á�
				ConnectionReset = 10054,
				//
				// ժҪ:
				//     û�п����� System.Net.Sockets.Socket �����Ŀ��û������ռ䡣
				NoBufferSpaceAvailable = 10055,
				//
				// ժҪ:
				//     System.Net.Sockets.Socket �����ӡ�
				IsConnected = 10056,
				//
				// ժҪ:
				//     Ӧ�ó�����ͼ���ͻ�������ݣ����� System.Net.Sockets.Socket δ���ӡ�
				NotConnected = 10057,
				//
				// ժҪ:
				//     ���ͻ�������ݵ�����δ�õ�������Ϊ System.Net.Sockets.Socket �ѱ��رա�
				Shutdown = 10058,
				//
				// ժҪ:
				//     ���ӳ��Գ�ʱ���������ӵ�����û����Ӧ��
				TimedOut = 10060,
				//
				// ժҪ:
				//     Զ���������������ܾ����ӡ�
				ConnectionRefused = 10061,
				//
				// ժҪ:
				//     ����Զ���������رգ�����ʧ�ܡ�
				HostDown = 10064,
				//
				// ժҪ:
				//     û�е�ָ������������·�ɡ�
				HostUnreachable = 10065,
				//
				// ժҪ:
				//     ����ʹ�û����׽����ṩ����Ľ��̹��ࡣ
				ProcessLimit = 10067,
				//
				// ժҪ:
				//     ������ϵͳ�����á�
				SystemNotReady = 10091,
				//
				// ժҪ:
				//     �����׽����ṩ����İ汾������Χ��
				VersionNotSupported = 10092,
				//
				// ժҪ:
				//     ��δ��ʼ�������׽����ṩ����
				NotInitialized = 10093,
				//
				// ժҪ:
				//     �����ػ����ڽ����С�
				Disconnecting = 10101,
				//
				// ժҪ:
				//     δ�ҵ�ָ�����ࡣ
				TypeNotFound = 10109,
				//
				// ժҪ:
				//     �޷�ʶ������������ �����Ʋ�����ʽ���������������
				HostNotFound = 11001,
				//
				// ժҪ:
				//     �޷������������� ���Ժ����ԡ�
				TryAgain = 11002,
				//
				// ժҪ:
				//     ���󲻿ɻָ����Ҳ�����������ݿ⡣
				NoRecovery = 11003,
				//
				// ժҪ:
				//     �����Ʒ��������Ҳ�����������ƻ� IP ��ַ��
				NoData = 11004,
			};			
		}		
	}	
}
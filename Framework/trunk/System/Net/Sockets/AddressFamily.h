#pragma once

namespace System
{
	namespace Net
	{
		namespace Sockets
		{
			// ժҪ:
			//     ָ�� System.Net.Sockets.Socket ���ʵ������ʹ�õ�Ѱַ������
			enum AddressFamily
			{
				// ժҪ:
				//     δ֪�ĵ�ַ�塣
				Unknown = -1,
				//
				// ժҪ:
				//     δָ���ĵ�ַ�塣
				Unspecified = 0,
				//
				// ժҪ:
				//     Unix ���ص�������ַ��
				Unix = 1,
				//
				// ժҪ:
				//     IP �汾 4 �ĵ�ַ��
				InterNetwork = 2,
				//
				// ժҪ:
				//     ARPANET IMP ��ַ��
				ImpLink = 3,
				//
				// ժҪ:
				//     PUP Э��ĵ�ַ��
				Pup = 4,
				//
				// ժҪ:
				//     MIT CHAOS Э��ĵ�ַ��
				Chaos = 5,
				//
				// ժҪ:
				//     IPX �� SPX ��ַ��
				Ipx = 6,
				//
				// ժҪ:
				//     Xerox NS Э��ĵ�ַ��
				NS = 6,
				//
				// ժҪ:
				//     OSI Э��ĵ�ַ��
				Osi = 7,
				//
				// ժҪ:
				//     ISO Э��ĵ�ַ��
				Iso = 7,
				//
				// ժҪ:
				//     ŷ�޼����������Э�� (ECMA) ��ַ��
				Ecma = 8,
				//
				// ժҪ:
				//     Datakit Э��ĵ�ַ��
				DataKit = 9,
				//
				// ժҪ:
				//     CCITT Э�飨�� X.25���ĵ�ַ��
				Ccitt = 10,
				//
				// ժҪ:
				//     IBM SNA ��ַ��
				Sna = 11,
				//
				// ժҪ:
				//     DECnet ��ַ��
				DecNet = 12,
				//
				// ժҪ:
				//     ֱ���������ӽӿڵ�ַ��
				DataLink = 13,
				//
				// ժҪ:
				//     LAT ��ַ��
				Lat = 14,
				//
				// ժҪ:
				//     NSC Hyperchannel ��ַ��
				HyperChannel = 15,
				//
				// ժҪ:
				//     AppleTalk ��ַ��
				AppleTalk = 16,
				//
				// ժҪ:
				//     NetBios ��ַ��
				NetBios = 17,
				//
				// ժҪ:
				//     VoiceView ��ַ��
				VoiceView = 18,
				//
				// ժҪ:
				//     FireFox ��ַ��
				FireFox = 19,
				//
				// ժҪ:
				//     Banyan ��ַ��
				Banyan = 21,
				//
				// ժҪ:
				//     ���� ATM �����ַ��
				Atm = 22,
				//
				// ժҪ:
				//     IP �汾 6 �ĵ�ַ��
				InterNetworkV6 = 23,
				//
				// ժҪ:
				//     Microsoft Ⱥ����Ʒ�ĵ�ַ��
				Cluster = 24,
				//
				// ժҪ:
				//     IEEE 1284.4 �������ַ��
				Ieee12844 = 25,
				//
				// ժҪ:
				//     IrDA ��ַ��
				Irda = 26,
				//
				// ժҪ:
				//     ֧����������� OSI ���ص�Э��ĵ�ַ��
				NetworkDesigners = 28,
				//
				// ժҪ:
				//     MAX ��ַ��
				Max = 29,
			};
		}		
	}	
}
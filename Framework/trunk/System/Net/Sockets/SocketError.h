#pragma once

namespace System
{
	namespace Net
	{
		namespace Sockets
		{
			// 摘要:
			//     定义 System.Net.Sockets.Socket 类的错误代码。
			enum SocketError
			{
				// 摘要:
				//     发生了未指定的 System.Net.Sockets.Socket 错误。
				Unkown = -1,
				//
				// 摘要:
				//     System.Net.Sockets.Socket 操作成功。
				Success = 0,
				//
				// 摘要:
				//     由于 System.Net.Sockets.Socket 已关闭，重叠的操作被中止。
				OperationAborted = 995,
				//
				// 摘要:
				//     应用程序已启动一个无法立即完成的重叠操作。
				IOPending = 997,
				//
				// 摘要:
				//     已取消阻止 System.Net.Sockets.Socket 调用的操作。
				Interrupted = 10004,
				//
				// 摘要:
				//     已试图通过被其访问权限禁止的方式访问 System.Net.Sockets.Socket。
				AccessDenied = 10013,
				//
				// 摘要:
				//     基础套接字提供程序检测到无效的指针地址。
				Fault = 10014,
				//
				// 摘要:
				//     给 System.Net.Sockets.Socket 成员提供了一个无效参数。
				InvalidArgument = 10022,
				//
				// 摘要:
				//     基础套接字提供程序中打开的套接字太多。
				TooManyOpenSockets = 10024,
				//
				// 摘要:
				//     对非阻止性套接字的操作不能立即完成。
				WouldBlock = 10035,
				//
				// 摘要:
				//     阻止操作正在进行中。
				InProgress = 10036,
				//
				// 摘要:
				//     非阻止性 System.Net.Sockets.Socket 已有一个操作正在进行中。
				AlreadyInProgress = 10037,
				//
				// 摘要:
				//     对非套接字尝试 System.Net.Sockets.Socket 操作。
				NotSocket = 10038,
				//
				// 摘要:
				//     在对 System.Net.Sockets.Socket 的操作中省略了必需的地址。
				DestinationAddressRequired = 10039,
				//
				// 摘要:
				//     数据报太长。
				MessageSize = 10040,
				//
				// 摘要:
				//     此 System.Net.Sockets.Socket 的协议类型不正确。
				ProtocolType = 10041,
				//
				// 摘要:
				//     对 System.Net.Sockets.Socket 使用了未知、无效或不受支持的选项或级别。
				ProtocolOption = 10042,
				//
				// 摘要:
				//     未实现或未配置协议。
				ProtocolNotSupported = 10043,
				//
				// 摘要:
				//     在此地址族中不存在对指定的套接字类型的支持。
				SocketNotSupported = 10044,
				//
				// 摘要:
				//     协议族不支持地址族。
				OperationNotSupported = 10045,
				//
				// 摘要:
				//     未实现或未配置协议族。
				ProtocolFamilyNotSupported = 10046,
				//
				// 摘要:
				//     不支持指定的地址族。 如果指定了 IPv6 地址族而未在本地计算机上安装 IPv6 堆栈，则会返回此错误。 如果指定了 IPv4 地址族而未在本地计算机上安装
				//     IPv4 堆栈，则会返回此错误。
				AddressFamilyNotSupported = 10047,
				//
				// 摘要:
				//     通常，只允许使用地址一次。
				AddressAlreadyInUse = 10048,
				//
				// 摘要:
				//     选定的 IP 地址在此上下文中无效。
				AddressNotAvailable = 10049,
				//
				// 摘要:
				//     网络不可用。
				NetworkDown = 10050,
				//
				// 摘要:
				//     不存在到远程主机的路由。
				NetworkUnreachable = 10051,
				//
				// 摘要:
				//     应用程序尝试在已超时的连接上设置 System.Net.Sockets.SocketOptionName.KeepAlive。
				NetworkReset = 10052,
				//
				// 摘要:
				//     此连接由 .NET Framework 或基础套接字提供程序中止。
				ConnectionAborted = 10053,
				//
				// 摘要:
				//     此连接由远程对等计算机重置。
				ConnectionReset = 10054,
				//
				// 摘要:
				//     没有可用于 System.Net.Sockets.Socket 操作的可用缓冲区空间。
				NoBufferSpaceAvailable = 10055,
				//
				// 摘要:
				//     System.Net.Sockets.Socket 已连接。
				IsConnected = 10056,
				//
				// 摘要:
				//     应用程序试图发送或接收数据，但是 System.Net.Sockets.Socket 未连接。
				NotConnected = 10057,
				//
				// 摘要:
				//     发送或接收数据的请求未得到允许，因为 System.Net.Sockets.Socket 已被关闭。
				Shutdown = 10058,
				//
				// 摘要:
				//     连接尝试超时，或者连接的主机没有响应。
				TimedOut = 10060,
				//
				// 摘要:
				//     远程主机正在主动拒绝连接。
				ConnectionRefused = 10061,
				//
				// 摘要:
				//     由于远程主机被关闭，操作失败。
				HostDown = 10064,
				//
				// 摘要:
				//     没有到指定主机的网络路由。
				HostUnreachable = 10065,
				//
				// 摘要:
				//     正在使用基础套接字提供程序的进程过多。
				ProcessLimit = 10067,
				//
				// 摘要:
				//     网络子系统不可用。
				SystemNotReady = 10091,
				//
				// 摘要:
				//     基础套接字提供程序的版本超出范围。
				VersionNotSupported = 10092,
				//
				// 摘要:
				//     尚未初始化基础套接字提供程序。
				NotInitialized = 10093,
				//
				// 摘要:
				//     正常关机正在进行中。
				Disconnecting = 10101,
				//
				// 摘要:
				//     未找到指定的类。
				TypeNotFound = 10109,
				//
				// 摘要:
				//     无法识别这种主机。 该名称不是正式的主机名或别名。
				HostNotFound = 11001,
				//
				// 摘要:
				//     无法解析主机名。 请稍后重试。
				TryAgain = 11002,
				//
				// 摘要:
				//     错误不可恢复或找不到请求的数据库。
				NoRecovery = 11003,
				//
				// 摘要:
				//     在名称服务器上找不到请求的名称或 IP 地址。
				NoData = 11004,
			};			
		}		
	}	
}
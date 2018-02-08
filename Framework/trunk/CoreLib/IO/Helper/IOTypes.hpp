﻿#pragma once

namespace System
{
	namespace IO
	{
		// 摘要:
		//     目前BOM的最大长度
		#define BOM_MAX_LEN 4 
		// 摘要:
		//     文本编码。
		enum Encoding{
			EC_ANSI=0,
			EC_UNICODE,
			EC_UTF8,
			EC_UTF16LE,
			EC_UTF16BE,
			EC_UTF32LE,
			EC_UTF32BE,
			EC_UNKNOWN
		};
		// 摘要:
		//     指定操作系统打开文件的方式。
		enum FileMode
		{
			// 摘要:
			//     指定操作系统应创建新文件。 这需要 System.Security.Permissions.FileIOPermissionAccess.Write
			//     权限。 如果文件已存在，则将引发 System.IO.IOException异常。
			FM_CreateNew = 1,
			//
			// 摘要:
			//     指定操作系统应创建新文件。 如果文件已存在，它将被覆盖。 这需要 System.Security.Permissions.FileIOPermissionAccess.Write
			//     权限。 FileMode.Create 等效于这样的请求：如果文件不存在，则使用 System.IO.FileMode.CreateNew；否则使用
			//     System.IO.FileMode.Truncate。 如果该文件已存在但为隐藏文件，则将引发 System.UnauthorizedAccessException异常。
			FM_Create = 2,
			//
			// 摘要:
			//     指定操作系统应打开现有文件。 打开文件的能力取决于 System.IO.FileAccess 枚举所指定的值。 如果文件不存在，引发一个 System.IO.FileNotFoundException
			//     异常。
			FM_Open = 3,
			//
			// 摘要:
			//     指定操作系统应打开文件（如果文件存在）；否则，应创建新文件。 如果用 FileAccess.Read 打开文件，则需要 System.Security.Permissions.FileIOPermissionAccess.Read权限。
			//     如果文件访问为 FileAccess.Write，则需要 System.Security.Permissions.FileIOPermissionAccess.Write权限。
			//     如果用 FileAccess.ReadWrite 打开文件，则同时需要 System.Security.Permissions.FileIOPermissionAccess.Read
			//     和 System.Security.Permissions.FileIOPermissionAccess.Write权限。
			FM_OpenOrCreate = 4,
			//
			// 摘要:
			//     指定操作系统应打开现有文件。 该文件被打开时，将被截断为零字节大小。 这需要 System.Security.Permissions.FileIOPermissionAccess.Write
			//     权限。 尝试从使用 FileMode.Truncate 打开的文件中进行读取将导致 System.ArgumentException 异常。
			FM_Truncate = 5,
			//
			// 摘要:
			//     若存在文件，则打开该文件并查找到文件尾，或者创建一个新文件。 这需要 System.Security.Permissions.FileIOPermissionAccess.Append
			//     权限。 FileMode.Append 只能与 FileAccess.Write 一起使用。 试图查找文件尾之前的位置时会引发 System.IO.IOException
			//     异常，并且任何试图读取的操作都会失败并引发 System.NotSupportedException 异常。
			FM_Append = 6
		};
		// 摘要:
		//     定义用于文件读取、写入或读取/写入访问权限的常数。
		enum FileAccess
		{
			// 摘要:
			//     对文件的读访问。 可从文件中读取数据。 与 Write 组合以进行读写访问。
			FA_Read = 1,
			//
			// 摘要:
			//     文件的写访问。 可将数据写入文件。 同 Read 组合即构成读/写访问权。
			FA_Write = 2,
			//
			// 摘要:
			//     对文件的读访问和写访问。 可从文件读取数据和将数据写入文件。
			FA_ReadWrite = 3
		};
		// 摘要:
		//     包含用于控制其他 System.IO.FileStream 对象对同一文件可以具有的访问类型的常数。
		//	   代表当前文件句柄没有关闭的同时，如果再打开同一个文件，那个新文件句柄可做的操作
		enum FileShare
		{
			// 摘要:
			//     谢绝共享当前文件。 文件关闭前，打开该文件的任何请求（由此进程或另一进程发出的请求）都将失败。
			FS_None = 0,
			//
			// 摘要:
			//     允许随后打开文件读取。 如果未指定此标志，则文件关闭前，任何打开该文件以进行读取的请求（由此进程或另一进程发出的请求）都将失败。 但是，即使指定了此标志，仍可能需要附加权限才能够访问该文件。
			FS_Read = 1,
			//
			// 摘要:
			//     允许随后打开文件写入。 如果未指定此标志，则文件关闭前，任何打开该文件以进行写入的请求（由此进程或另一进过程发出的请求）都将失败。 但是，即使指定了此标志，仍可能需要附加权限才能够访问该文件。
			FS_Write = 2,
			//
			// 摘要:
			//     允许随后打开文件读取或写入。 如果未指定此标志，则文件关闭前，任何打开该文件以进行读取或写入的请求（由此进程或另一进程发出）都将失败。 但是，即使指定了此标志，仍可能需要附加权限才能够访问该文件。
			FS_ReadWrite = 3,
			//
			// 摘要:
			//     允许随后删除文件。
			FS_Delete = 4,
			//
			// 摘要:
			//     使文件句柄可由子进程继承。 Win32 不直接支持此功能。
			FS_Inheritable = 16
		};

		// 摘要:
		//     提供文件和目录的属性。
		enum FileAttributes
		{
			// 摘要:
			//     此文件是只读的。
			FA_ReadOnly = 1,//#define FILE_ATTRIBUTE_READONLY             0x00000001  
			//
			// 摘要:
			//     文件是隐藏的，因此没有包括在普通的目录列表中。
			FA_Hidden = 2,//#define FILE_ATTRIBUTE_HIDDEN               0x00000002
			//
			// 摘要:
			//     此文件是系统文件。 即，该文件是操作系统的一部分或者由操作系统以独占方式使用。
			FA_System = 4,//#define FILE_ATTRIBUTE_SYSTEM               0x00000004 
			//
			// 摘要:
			//     此文件是一个目录。
			FA_Directory = 16,//#define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
			//
			// 摘要:
			//     该文件是备份或移除的候选文件。
			FA_Archive = 32,//#define FILE_ATTRIBUTE_ARCHIVE              0x00000020 
			//
			// 摘要:
			//     保留供将来使用。
			FA_Device = 64,//#define FILE_ATTRIBUTE_DEVICE               0x00000040 
			//
			// 摘要:
			//     该文件是没有特殊属性的标准文件。 仅当其单独使用时，此特性才有效。
			FA_Normal = 128,//#define FILE_ATTRIBUTE_NORMAL               0x00000080  
			//
			// 摘要:
			//     文件是临时文件。 临时文件包含当执行应用程序时需要的，但当应用程序完成后不需要的数据。 文件系统尝试将所有数据保存在内存中，而不是将数据刷新回大容量存储，以便可以快速访问。
			//     当临时文件不再需要时，应用程序应立即删除它。
			FA_Temporary = 256,//#define FILE_ATTRIBUTE_TEMPORARY            0x00000100
			//
			// 摘要:
			//     此文件是稀疏文件。 稀疏文件一般是数据通常为零的大文件。
			FA_SparseFile = 512,//#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200
			//
			// 摘要:
			//     文件包含一个重新分析点，它是一个与文件或目录关联的用户定义的数据块。
			FA_ReparsePoint = 1024,//#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400
			//
			// 摘要:
			//     此文件是压缩文件。
			FA_Compressed = 2048,//#define FILE_ATTRIBUTE_COMPRESSED           0x00000800
			//
			// 摘要:
			//     此文件处于脱机状态， 文件数据不能立即供使用。
			FA_Offline = 4096,//#define FILE_ATTRIBUTE_OFFLINE              0x00001000
			//
			// 摘要:
			//     将不会通过操作系统的内容索引服务来索引此文件。
			FA_NotContentIndexed = 8192,//#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000
			//
			// 摘要:
			//     此文件或目录已加密。 对于文件来说，表示文件中的所有数据都是加密的。 对于目录来说，表示新创建的文件和目录在默认情况下是加密的。
			FA_Encrypted = 16384,//#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000
		};
	}
}
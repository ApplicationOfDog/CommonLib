#pragma once

namespace System
{
	// 摘要:
	//     指定适用的 Overload:System.String.Split 方法重载包含还是省略返回值中的空子字符串。
	enum StringSplitOptions
	{
		// 摘要:
		//     返回值包括含有空字符串的数组元素
		SSO_None = 0,
		//
		// 摘要:
		//     返回值不包括含有空字符串的数组元素
		SSO_RemoveEmptyEntries = 1
	};

	// 摘要:window版本
	enum WinOSType
	{
		WOS_None=0,
		WOS_Win32s,   
		WOS_WinNT3,   
		WOS_Win95,   
		WOS_Win98,   
		WOS_WinME,   
		WOS_WinNT4,   
		WOS_Win2000,   
		WOS_WinXP,		//8
		WOS_Win2003,	//9
		WOS_VISTA,		//10
		WOS_WINDOWS7,	//11
		WOS_WINDOWS8,	//12
		WOS_WINDOWS81,	//13
		WOS_WINDOWS10	//14
	};
	// 摘要:
	//     指定用于检索系统特殊文件夹的目录路径的枚举常数。
	enum SpecialFolder
	{
		// 摘要:
		//     逻辑桌面，而不是物理文件系统位置。
		SF_Desktop = 0,
		//
		// 摘要:
		//     包含用户程序组的目录。
		SF_Programs = 2,
		//
		// 摘要:
		//     用作文档的公共储存库的目录。
		SF_Personal = 5,
		//
		// 摘要:
		//     “我的文档”文件夹。
		SF_MyDocuments = 5,
		//
		// 摘要:
		//     用作用户收藏夹项的公共储存库的目录。
		SF_Favorites = 6,
		//
		// 摘要:
		//     对应于用户的“启动”程序组的目录。
		SF_Startup = 7,
		//
		// 摘要:
		//     包含用户最近使用过的文档的目录。
		SF_Recent = 8,
		//
		// 摘要:
		//     包含“发送”菜单项的目录。
		SF_SendTo = 9,
		//
		// 摘要:
		//     包含“开始”菜单项的目录。
		SF_StartMenu = 11,
		//
		// 摘要:
		//     “我的音乐”文件夹。
		SF_MyMusic = 13,
		//
		// 摘要:
		//     文件系统目录，充当属于某个用户的视频的存储库。
		SF_MyVideos = 14,
		//
		// 摘要:
		//     用于物理上存储桌面上的文件对象的目录。
		SF_DesktopDirectory = 16,
		//
		// 摘要:
		//     “我的电脑”文件夹。
		SF_MyComputer = 17,
		//
		// 摘要:
		//     文件系统目录，包含“网上邻居”虚拟文件夹中可能存在的链接对象。
		SF_NetworkShortcuts = 19,
		//
		// 摘要:
		//     包含字体的虚拟文件夹。
		SF_Fonts = 20,
		//
		// 摘要:
		//     用作文档模板的公共储存库的目录。
		SF_Templates = 21,
		//
		// 摘要:
		//     文件系统目录，包含在所有用户的“开始”菜单上都出现的程序和文件夹。 此特殊文件夹仅对 Windows NT 系统有效。
		SF_CommonStartMenu = 22,
		//
		// 摘要:
		//     跨应用程序共享的组件的文件夹。 此特殊文件夹仅对 Windows NT、Windows 2000 和 Windows XP 系统有效。
		SF_CommonPrograms = 23,
		//
		// 摘要:
		//     文件系统目录，包含在所有用户的“启动”文件夹中都出现的程序。 此特殊文件夹仅对 Windows NT 系统有效。
		SF_CommonStartup = 24,
		//
		// 摘要:
		//     文件系统目录，包含在所有用户桌面上出现的文件和文件夹。 此特殊文件夹仅对 Windows NT 系统有效。
		SF_CommonDesktopDirectory = 25,

		///<summary>目录，它用作当前漫游用户的应用程序特定数据的公共储存库
		/// XP: C:\Documents and Settings\XXXX\Application Data 
		/// WIN7: C:\Users\XXXX\AppData\Roaming
		///</summary>
		SF_ApplicationData = 26,
		//
		// 摘要:
		//     文件系统目录，包含“打印机”虚拟文件夹中可能存在的链接对象。
		SF_PrinterShortcuts = 27,

		///<summary>目录，它用作当前非漫游用户使用的应用程序特定数据的公共储存库
		/// XP: C:\Documents and Settings\XXX\Local Settings\Application Data 
		/// WIN7: C:\Users\XXXX\AppData\Local
		///</summary>		
		SF_LocalApplicationData = 28,
		//
		// 摘要:
		//     用作 Internet 临时文件的公共储存库的目录。
		SF_InternetCache = 32,
		//
		// 摘要:
		//     用作 Internet Cookie 的公共储存库的目录。
		SF_Cookies = 33,
		//
		// 摘要:
		//     用作 Internet 历史记录项的公共储存库的目录。
		SF_History = 34,

		///<summary>目录，它用作所有用户使用的应用程序特定数据的公共储存库。
		/// WIN7: C:\ProgramData
		/// XP: C:\Documents and Settings\All Users\Application Data
		///</summary>
		SF_CommonApplicationData = 35,
		//
		// 摘要:
		//     Windows 目录或 SYSROOT。 它与 %windir% 或 %SYSTEMROOT% 环境变量相对应。
		SF_Windows = 36,
		//
		// 摘要:
		//     “System”目录。
		SF_System = 37,
		//
		// 摘要:
		//     “Program files”目录。
		SF_ProgramFiles = 38,
		//
		// 摘要:
		//     “我的图片”文件夹。
		SF_MyPictures = 39,
		//
		// 摘要:
		//     用户的配置文件文件夹。 应用程序不应在此级别上创建文件或文件夹；它们应将其数据放在 System.Environment.SpecialFolder.ApplicationData
		//     所引用的位置之下。
		SF_UserProfile = 40,
		//
		// 摘要:
		//     Windows“System”文件夹。
		SF_SystemX86 = 41,
		//
		// 摘要:
		//     “Program Files”文件夹。
		SF_ProgramFilesX86 = 42,
		//
		// 摘要:
		//     用于应用程序间共享的组件的目录。
		SF_CommonProgramFiles = 43,
		//
		// 摘要:
		//     “Program Files”文件夹。
		SF_CommonProgramFilesX86 = 44,
		//
		// 摘要:
		//     文件系统目录，包含所有用户都可以使用的模板。 此特殊文件夹仅对 Windows NT 系统有效。
		SF_CommonTemplates = 45,
		//
		// 摘要:
		//     文件系统目录，包含所有用户共有的文档。 此特殊文件夹仅对装有 Shfolder.dll 的 Windows NT 系统、Windows 95 和
		//     Windows 98 系统有效。
		SF_CommonDocuments = 46,
		//
		// 摘要:
		//     文件系统目录，包含计算机所有用户的管理工具。
		SF_CommonAdminTools = 47,
		//
		// 摘要:
		//     文件系统目录，用于存储各个用户的管理工具。 Microsoft Management Console (MMC) 会将自定义的控制台保存在此目录中，并且此目录将随用户一起漫游。
		SF_AdminTools = 48,
		//
		// 摘要:
		//     文件系统目录，充当所有用户共有的音乐文件的存储库。
		SF_CommonMusic = 53,
		//
		// 摘要:
		//     文件系统目录，充当所有用户共有的图像文件的存储库。
		SF_CommonPictures = 54,
		//
		// 摘要:
		//     文件系统目录，充当所有用户共有的视频文件的存储库。
		SF_CommonVideos = 55,
		//
		// 摘要:
		//     文件系统目录，包含资源数据。
		SF_Resources = 56,
		//
		// 摘要:
		//     文件系统目录，包含本地化资源数据。
		SF_LocalizedResources = 57,
		//
		// 摘要:
		//     为了实现向后兼容，Windows Vista 中可以识别此值，但该特殊文件夹本身已不再使用。
		SF_CommonOemLinks = 58,
		//
		// 摘要:
		//     文件系统目录，充当等待写入 CD 的文件的临时区域。
		SF_CDBurning = 59
	};
	// 摘要:
	//     指定用于获取特殊文件夹路径的选项。
	enum SpecialFolderOption
	{
		// 摘要:
		//     已验证指向文件夹的路径。 如果该文件夹存在，则返回路径。 如果文件夹不存在，则返回空字符串。 这是默认行为。
		SFO_None = 0,
		//
		// 摘要:
		//     指向文件夹的路径在返回时不会验证路径是否存在。 如果文件夹位于网络上，则指定此选项可以缩短延隔时间。
		SFO_DoNotVerify = 16384,
		//
		// 摘要:
		//     如果文件夹尚未存在，则创建该文件夹的路径。
		SFO_Create = 32768,
	};	
}
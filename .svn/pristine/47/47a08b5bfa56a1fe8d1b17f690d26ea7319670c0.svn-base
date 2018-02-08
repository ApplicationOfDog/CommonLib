#pragma once
#include <map>

/// <summary>
/// Unicode means Unicode16
/// </summary>

namespace System
{
	class API_CORE Environment
	{
	public:
		static LPCTSTR	NewLine;
		static DWORD	LastErrorCode;

	public:
#if IS_WINDOWS_OS
		///<summary>获取包含当前操作系统版本类型</summary>		
		///<returns>系统版本类型</returns>
		static WinOSType GetOsVersion();

		///<summary>获取操作系统架构</summary>
		///<param name="is64Bit">返回是否为64位系统</param>
		///<returns>是否获取成功</returns>
		static bool GetOsArchitecture(OUT bool* is64Bit);

		///<summary>从当前进程检索环境变量的值</summary>
		///<param name="variable">环境变量名</param>
		///<returns>variable 指定的环境变量的值；或者如果找不到环境变量，则返回 null</returns>
		static String GetEnvironmentVariable(IN LPCTSTR variable);

		///<summary>创建、修改或删除当前进程中存储的环境变量</summary>
		///<param name="variable">环境变量名</param>
		///<param name="value">要分配给 variable 的值</param>
		static bool SetEnvironmentVariable(IN LPCTSTR variable, IN LPCTSTR value);

		///<summary>获取由指定枚举标识的系统特殊文件夹的路径</summary>
		///<param name="folder">标识系统特殊文件夹的枚举常数</param>
		///<returns>
		/// 如果指定的系统特殊文件夹实际存在于您的计算机上，则为到该文件夹的路径；否则为空字符串 ("")。
		/// 如果系统未创建文件夹、已删除现有文件夹，或者文件夹是不对应物理路径的虚拟目录（例如"我的电脑"）,
		/// 则该文件夹不会实际存在
		/// </returns>
		static String GetFolderPath(IN SpecialFolder folder,SpecialFolderOption option=SFO_None);

		///<summary>获取系统目录</summary>
		///<param name="isWindowsDir">true:获取windows目录,false:获取system32目录</param>
		///<returns>返回的目录取决于isWindowsDir的值
		/// true:C:\WINDOWS 
		/// false:C:\WINDOWS\system32
		///</returns>
		static String GetSystemFolder(IN bool isWindowsDir);

		///<summary>返回包含当前计算机中的逻辑驱动器名称的字符串数组</summary>
		///<returns>字符串数组，其中的每个元素都包含逻辑驱动器名称。 例如，如果计算机的硬盘是第一个逻辑驱动器，则返回的第一个元素是“C:\”</returns>
		static vector<String> GetLogicalDrives();

		///<summary>返回当前模块。即调用该函数所在的模块</summary>
		///<param name="bProcess">是否返回进程句柄，TRUE表示返回执行进程的句柄。FALSE:返回PE句柄(有可能是当exe进程，或是调用该函数的dll)</param>
		///<returns>当前模块句柄</returns>
		static HMODULE GetCurrentModule(bool bProcess=false);
#endif

		///<summary>返回当前PE载入路径.即返回当前exe或dll所在的全路径</summary>
		///<param name="bProcess">是否返回进程可执行文件路径，TRUE表示返回执行进程的可执行文件路径。FALSE:返回PE的可执行文件路径(有可能是当exe进程，或是调用该函数的dll)</param>
		///<returns>PE文件全路径</returns>
		static String GetExecutablePath(IN bool bProcess=false);

		///<summary>
		///返回由上一个函数返回的错误代码或错误信息,例如"5-Access is denied."
		///</summary>
		///<param name="code">错误码(GetLastError的值)，-1表示内部自行获取无需参数</param>
		///<param name="textMode">输出模式,bit0:错误码,bit1:错误文本</param>
		///<returns>错误信息</returns>
		static String LastErrorText(IN DWORD code=-1,IN byte textMode=0x3);
		static DWORD LastError();
		static void LastError(IN DWORD val);
		
		///<summary>
		///返回由上一个函数返回的系统错误代码或错误信息,例如"2-No such file or directory"
		///</summary>
		///<param name="no">错误码(Errno的值)，-1表示内部自行获取无需参数</param>
		///<param name="textMode">输出模式,bit0:错误码,bit1:错误文本</param>
		///<returns>错误信息</returns>
		static String ErrnoText(IN int no=-1,IN byte textMode=0x3);
		static int Errno();
		

		/// <summary>
		/// 获取当前登录的windows账户名
		/// </summary>
		static String GetLoginName();
		/// <summary>
		/// 获取当前进程所处环境的LOGONID
		/// </summary>
		static ULONGLONG GetCurrentLUID();

		///<summary>获取或设置当前工作目录的完全限定路径</summary>
		///<returns>包含目录路径的字符串</returns>
		static String CurrentDirectory();

#if IS_WINDOWS_OS		
		static void CurrentDirectory(IN LPCTSTR path);
		/// <summary>
		/// 判断当前进程是否是已经打开的唯一实例
		/// </summary>
		/// <param name="instance">实例名称</param>
		/// <returns>TRUE:当前是唯一实例，并向系统注册该实例名称,FALSE:已经存在一个实例</returns>
		static bool IsSingleInstance(IN LPCTSTR instance);
#endif
		///<summary>获取当前进程的命令行参数,即去除可行文件的全路径之后的纯参数</summary>
		static String GetProcessCommandLine();
		///<summary>
		/// 分析程序命令行参数,通过cmd.exe或其他方式创建进程时传入
		/// 参数由多个部分(称为参数项)组成,不同的参数使用','隔开,示例如下
		/// -xx:aa,-yy:bb,-zz:cc //说明:-参数名称:参数值
		///</summary>
		/// <param name="commandLine">程序命令行,NULL表示内部获取当前进程的命令行</param>
		/// <param name="item_separator">参数项之间的分隔符,如上面例子的','</param>
		/// <param name="item_prefix">合法参数前缀,如上面例子的'-'</param>
		/// <param name="keyVal_separator">参数项内部键-值分隔符,如上面例子的':'</param>
		/// <returns>返回参数项键值对,PS.key是去除item_prefix前缀
		/// 例如上面实例:[<"xx":"aa">,<"yy":"bb">,<"zz":"cc">]
		///</returns>
		static std::map<String,String> AnayzeProgramCommandLine(IN LPCTSTR commandLine,IN LPCTSTR item_separator=_T(","),IN LPCTSTR item_prefix=_T("-"),IN TCHAR keyVal_separator=_T(':'));

	private:
#if IS_WINDOWS_OS	
		static bool CheckEnvironmentVariableName(String variable);
#endif
	};
}
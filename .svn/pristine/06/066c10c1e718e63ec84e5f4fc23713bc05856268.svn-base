#pragma once

namespace System
{
	namespace IO
	{
		class API_CORE Path
		{
		public:
			static const TCHAR DirectorySeparatorChar;//路径分隔符 '\'
			static const TCHAR AltDirectorySeparatorChar;//相对路径分隔符 '/'
			static const TCHAR VolumeSeparatorChar;//盘符分隔符 ':'
			static const TCHAR ExtensionSeparatorChar;//扩展名分隔符 '.'

			//路径组成部分
			//ps.C:\Users\Aweigh\Desktop\new.txt
			enum TPart
			{ 
				TVolume=0x1,//盘符 "C:"
				TDirectory=0x2, //文件夹(不包含盘符) "Users\Aweigh\Desktop"
				TFileName=0x4,//文件名称(不包含扩展名)  "new"
				TExtension=0x8, //扩展名 ".txt"
				TFileWithExt=TFileName|TExtension,//文件名(不包含扩展名)  "new.txt"
				TNoFile=TVolume|TDirectory,//除文件名之外的其余部分 "C:\Users\Aweigh\Desktop\"
				TNoExt=TVolume|TDirectory|TFileName //除扩展名之外的其余部分 "C:\Users\Aweigh\Desktop\new"
			};

		public:
			///<summary>
			/// 判断文件或目录是否存在磁盘中,注意:对文件或文件夹都适用
			///</summary>
			///<param name="path">文件或目录的路径</param>
			///<returns>true:存在,false:不存在</returns>
			static bool Exists(IN String path);

			///<summary>
			/// 判断给定字符串是否为一个有效文件路径
			///</summary>
			///<param name="path">文件或目录的路径</param>
			///<param name="pType">返回路径类型,
			/// 0表示普通文件路径,例如:C:\Users\Aweigh\Desktop\new.txt
			/// 1表示共享文件路径,例如:\\192.168.1.100\VM-Share\files\2.txt
			/// -1表示无法识别
			///</param>
			///<returns>true:表示合法路径,false:是非法路径</returns>
			static bool IsValid(IN LPCTSTR path,OUT int* pType=NULL);

			///<summary>将字符串数组组合成一个路径</summary>
			///<param name="count">参数个数</param>
			///<param name="...">参数类型必须都为LPTSTR类型</param>
			///<returns>组合后的路径</returns>
			static String Combine(IN int count,...);
			static String Combine(IN String path0,IN String path1);
			static String Combine(IN String path0,IN String path1,IN String path2);
			
			///<summary>
			/// 获取指定路径字符串的目录信息
			/// 原理是判断最后一个'\\'或':'字符的位置并返回之前的位置,如果不存在'\\'或':'字符则返回String::Empty
			///</summary>
			///<param name="path">文件或目录的路径</param>
			///<param name="isEndDirSeparatorChar">文件或目录的路径</param>
			///<returns>
			/// 文件路径的目录信息,如果path没有包含目录信息,则返回String::Empty
			/// isEndDirSeparatorChar=false: "f:\CloudDisk\Aw.Core\IO\Path.hpp" => "f:\CloudDisk\Aw.Core\IO"
			/// isEndDirSeparatorChar=true : "f:\CloudDisk\Aw.Core\IO\Path.hpp" => "f:\CloudDisk\Aw.Core\IO\"
			///</returns>
			static String GetDirectoryName(IN String& path,IN bool isEndDirSeparatorChar=false);
			static String GetDirectoryName(IN LPCTSTR path,IN bool isEndDirSeparatorChar=false);
			
			///<summary>返回指定的路径字符串的扩展名</summary>
			///<param name="path">从其获取扩展名的路径字符串</param>
			///<returns>返回路径的扩展名称
			///	例如.源字符串:"f:\CloudDisk\Aw.Core\IO\Path.hpp" 输出:".hpp"
			///</returns>
			static String GetExtension(IN String& path);

			///<summary>返回指定路径字符串的文件名和扩展名,对文件夹路径同样适用</summary>
			///<param name="path">从其获取文件名和扩展名的路径字符串</param>
			///<returns>
			/// path 中最后的目录字符后的字符。 如果 path 的最后一个字符是目录或卷分隔符，则此方法返回 System.Empty。 如果
			/// path 为 null，则此方法返回 null。
			///	eg. "f:\CloudDisk\Aw.Core\IO\Path.hpp" => "Path.hpp"
			///	eg. "f:\CloudDisk\Aw.Core\IO" => "IO"
			///</returns>
			static String GetFileName(IN String& path);
			static String GetFileName(IN LPCTSTR path);

			///<summary>返回不具有扩展名的指定路径字符串的文件名</summary>
			///<param name="path">path</param>
			///<returns>返回的字符串，但不包括最后的句点 (.) 以及之后的所有字符</returns>
			///<example>"f:\CloudDisk\Aw.Core\IO\Path.hpp" => "Path"</example>
			static String GetFileNameWithoutExtension(IN String& path);

			///<summary>获取指定路径的根目录信息</summary>
			///<param name="path">从其获取根目录信息的路径</param>
			///<returns>
			/// path 的根目录，例如“C:\”；如果 path 为 null，则为 null；如果 path 不包含根目录信息，则为空字符串。
			///	 eg. "f:\CloudDisk\Aw.Core\IO\Path.hpp" => "f:\"
			///</returns>
			static String GetRoot(IN String& path);

			///<summary>确定路径是否包括文件扩展名</summary>
			///<param name="path">用于搜索扩展名的路径</param>
			///<returns>
			// 如果路径中最后的目录分隔符（\\ 或 /）或卷分隔符 (:) 之后的字符包括句点 (.)，并且后面跟有一个或多个字符，则为 true；否则为 false。
			///</returns>
			static bool HasExtension(IN String& path);

#if IS_WINDOWS_OS
			///<summary>获取路径的某个组成部分</summary>
			///<param name="path">用于搜索扩展名的路径</param>
			///<param name="bPart">指定要获取的组成部分</param>
			///<returns>获取路径的某个组成部分</returns>
			static String GetPart(IN String& path,IN BYTE parts);

			///<summary>替换路径中的一部分</summary>
			///<param name="path">用于搜索扩展名的路径</param>
			///<param name="part">要替换哪一部分</param>
			///<param name="src">替换字符串。(如果part=TDirectory,src前后‘\’路径分隔符可要可不要)</param>
			///<returns>返回被替换后的路径字符串</returns>
			static String Replace(IN String& path,IN TPart part,IN String src);
#endif
		public:
			///<summary>获取包含不允许在路径名中使用的字符的数组</summary>
			///<returns>包含不允许在路径名中使用的字符的数组</returns>
			static String& GetInvalidPathChars();

			///<summary>参数路径是否含有非法字符</summary>
			///<param name="path">判断路径</param>
			///<returns>包含不允许在路径名中使用的字符的数组</returns>
			static bool HasIllegalCharacters(IN String& path);

			///<summary>是否为非法文件名称字符字符</summary>
			///<returns>true:表示非法字符,false:不是非法字符</returns>
			static bool IsInvalidFileNameChar(IN TCHAR c);
		};
	}	
}
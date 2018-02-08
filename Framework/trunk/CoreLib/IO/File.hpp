#pragma once

namespace System
{
	namespace IO
	{
		// 摘要:
		//     提供用于创建、复制、删除、移动和打开文件的静态方法，并协助创建 System.IO.FileStream 对象。
		class API_CORE File
		{
		public:
			/// <summary>
			/// 将现有文件复制到新文件。 允许覆盖同名的文件。
			/// </summary>
			/// <param name="sourceFileName"> 要复制的文件</param>
			/// <param name="destFileName">目标文件的名称。 不能是目录。</param>
			/// <param name="overwrite">是否可以覆盖目标文件:
			///	true:如果目标文件存在则覆盖,false:如果目标文件存在则返回false即执行失败
			/// </param>
			/// <returns>是否成功</returns>
			static bool Copy(IN LPCTSTR sourceFileName,IN LPCTSTR destFileName, bool overwrite);
			static bool CopyToDir(IN LPCTSTR sourceFileName,IN LPCTSTR destDir, bool overwrite);

			///<summary>删除指定的文件</summary>
			///<param name="path">要删除的文件的名称。 该指令不支持通配符</param>
			///<returns>是否成功</returns>
			static bool Delete(IN LPCTSTR path);

			///<summary>确定指定的文件是否存在</summary>
			///<param name="path">要检查的文件</param>
			///<returns>如果调用方具有要求的权限并且 path 包含现有文件的名称，则为 true；否则为 false。 如果 path 为 null、无效路径或零长度字符串，则此方法也将返回
			//     false。 如果调用方不具有读取指定文件所需的足够权限，则不引发异常并且该方法返回 false，这与 path 是否存在无关。
			///</returns>
			static bool Exists(IN LPCTSTR path);
			static bool ExistsA(IN char* path);

			///<summary>将指定文件移到新位置，并提供指定新文件名的选项</summary>
			///<param name="sourceFileName">要移动的文件的名称</param>
			///<param name="destFileName">文件的新路径</param>
			///<param name="isDeleteIfFileExist">如果目标文件已经存在是否删除</param>
			///<returns>是否成功</returns>
			static bool Move(IN LPCTSTR sourceFileName,IN LPCTSTR destFileName,bool isDeleteIfFileExist=FALSE);

			///<summary>将指定文件移到新位置，并提供指定新文件名的选项</summary>
			///<param name="sourceFileName">源文件</param>
			///<param name="destDir">目标目录</param>
			///<param name="isDeleteIfFileExist">如果目标文件已经存在是否删除</param>
			///<returns>是否成功</returns>
			static bool MoveToDirectory(IN LPCTSTR sourceFileName,IN LPCTSTR destDir,bool isDeleteIfFileExist=FALSE);

			/// <summary>
			/// 打开一个文件，将文件的所有内容读入一个缓冲区中，然后关闭该文件。
			/// </summary>
			/// <param name="path">文件路径</param>
			/// <param name="buffer">返回文件内容的缓冲区</param>
			/// <returns>读取的字节数</returns>
			static size_t ReadAllBytes(IN LPCTSTR path,IN byte* buffer);
			static size_t ReadAllBytes2(IN LPCTSTR path,OUT byte** pBuffer);
			/// <summary>
			/// 打开一个文件，从文件中读取一定长度到缓冲区中，然后关闭该文件。
			/// </summary>
			/// <param name="path">文件路径</param>
			/// <param name="buffer">返回文件内容的缓冲区</param>
			/// <param name="szBuffer">缓冲区大小(0表示读取整个文件)</param>
			/// <returns>读取的字节数</returns>
			static size_t ReadBytes(IN LPCTSTR path,IN byte* buffer,IN unsigned szBuffer);
			/// <summary>
			/// 获取文件长度
			/// </summary>
			/// <param name="path">文件路径</param>
			/// <param name="isText">是否为文本文件</param>
			static ULONGLONG GetLength(IN LPCTSTR path,IN bool isText=false);

			///<summary>打开一个文本文件，读取文件的所有行，然后关闭该文件</summary>
			///<param name="path">要打开以进行读取的文件</param>
			///<returns>包含文件所有行的字符串数组</returns>
			static vector<String> ReadAllLines(IN LPCTSTR path);

			///<summary>打开一个文本文件，读取文件的所有行，然后关闭该文件</summary>
			///<param name="path">要打开以进行读取的文件路径</param>
			///<param name="code">目标文件编码</param>
			///<returns>包含文件所有行的字符串</returns>
			static String ReadAllText(IN LPCTSTR path,IN Encoding code = EC_ANSI);
			static bool ReadAllText(IN LPCTSTR path,OUT char* pBuffer,OUT long szBuffer=-1);

			///<summary>创建一个新文件，在其中写入指定的字节数组，然后关闭该文件。 如果目标文件已存在，则覆盖该文件</summary>
			///<param name="path">要写入的文件</param>
			///<param name="buffer">要写入的数据缓冲区</param>
			///<param name="size">数据缓冲区大小</param>
			///<returns>返回写入的字节数,0表示错误</returns>
			static size_t WriteAllBytes(IN LPCTSTR path,const byte* buffer,size_t size);
			static size_t WriteAllBytes(FILE* file,const byte* buffer,size_t size);

			/// <summary>
			/// 创建一个新文件，在其中写入指定的字符串，然后关闭文件。 如果目标文件已存在，则覆盖该文件。
			/// 备注:目标文件的字符编码与contents字符编码相同,即contents是utf-8或ansi编码那生成的文件也是utf-8或ansi编码
			/// </summary>
			/// <param name="path">要写入的文件</param>
			/// <param name="contents">要写入文件的字符串</param>
			static bool WriteAllText(IN LPCTSTR path,IN string& contents);

			///<summary>获取在此路径上的文件的 System.IO.FileAttributes</summary>
			///<param name="path">该文件的路径</param>
			///<returns>路径上文件的 System.IO.FileAttributes</returns>
			static DWORD GetAttributes(IN String path);
			/// <summary>
			/// 设置指定路径上文件的指定的 System.IO.FileAttributes
			/// </summary>
			/// <param name="path">该文件的路径</param>
			/// <param name="fileAttributes">枚举值的按位组合,System.IO.FileAttributes</param>
			/// <returns>是否设置成功</returns>
			static bool SetAttributes(IN String path,IN DWORD fileAttributes);

#if IS_WINDOWS_OS //window系统才有下面的接口
			/// <summary>
			/// 获取文件内容头部的BOM字节数
			/// </summary>
			/// <param name="buffer">文件内容</param>
			/// <returns>BOM的字节数</returns>
			static int GetBomCount(IN byte* buffer);
			/// <summary>
			/// 获取文件内容头部的BOM编码格式
			/// </summary>
			/// <param name="buffer">文件内容</param>
			static IO::Encoding GetBOM(IN byte* buffer);
#endif

		private:
			static size_t InnerReadBytes(IN String path,IN byte* buffer,IN unsigned szBuffer,IN bool isNewBuffer);
		};
	}	
}


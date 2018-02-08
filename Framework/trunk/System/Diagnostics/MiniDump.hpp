#pragma once
#include "../ref/dbghelp.h"

namespace System{
	namespace Diagnostics{

		typedef API_SYSTEM void(*LPCALLBACK_PREEXCEPTION_HANDLER)();
		class API_SYSTEM MiniDump:public Pattern::Singleton_ptr<MiniDump>
		{
			friend Pattern::Singleton_ptr<MiniDump>;
		public:
			///dump类型
			MINIDUMP_TYPE Type();
			void Type(MINIDUMP_TYPE type);
			///<summary>设置或获取dump文件名前缀</summary>
			String PrefixName();
			void PrefixName(IN String prefix);

			///<summary>固定文件名称,这样可以防止dump文件过多,因为会前后覆盖</summary>
			String FileName();
			void FileName(IN String fileName);//如果为空表示使用进程名称

			///dump文件的保存路径,这里是目录，不是全路径
			String PathDir();
			///<summary>设置或获取dump文件的保存路径</summary>
			///<param name="path">目录路径(这里是目录路径,将与FileName合成一个完整路径)
			///例如:d:\aaa\bb\cc
			///</param>
			void PathDir(IN String path);//不能为空
			///生成的dump文件全路径
			String FilePath();
			//发生异常时调用
			void PreviewExceptionHandler(LPCALLBACK_PREEXCEPTION_HANDLER handler);
			
			///参数设置,ps.该函数必须在Install之前调用
			//type:dump类型
			//prefix:设置dump文件名前缀
			//pathDir:dump文件所要保存的路径目录（非全路径）
			//preview:发生异常时调用
			static void Set(IN MINIDUMP_TYPE type,
							IN String prefix,IN String fileName,IN String pathDir,
							IN LPCALLBACK_PREEXCEPTION_HANDLER preview);
			//安装异常处理过程
			static bool Install(IN LPCALLBACK_PREEXCEPTION_HANDLER preview = NULL);
			
		protected:
			static bool				_isInstalled;
			MINIDUMP_TYPE			_type;
			String					_prefixName;
			String					_pathDir;
			String					_fileName;
			String					_filePath;//生成的dump文件全路径
			LPCALLBACK_PREEXCEPTION_HANDLER	_previewExceptionHandler;

			MiniDump();
			void InnerSet();
			//内部函数,作为未处理异常的处理函数
			static LONG WINAPI ExceptionFilter(PEXCEPTION_POINTERS exceptionInfo);
		};
}}
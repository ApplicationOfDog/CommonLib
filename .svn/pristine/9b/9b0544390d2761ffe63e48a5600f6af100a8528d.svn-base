#pragma once
#include <time.h>
#include <map>
#include "../../Text/StringBuilder.hpp"
using namespace System::Text;

namespace System
{
	namespace IO
	{
		class LogHelper
		{
		public:
			bool			_isShowTime;//是否记录日期
			String			_headText;//日志头部
			bool			_isEndEnterChar;//是否输出换行符
			bool			_isDebugStatus;//调用模块是否处于Debug版本
			bool			_isClearFirst;//在第一次写日志之前是否先清空日志
			bool			_isStartupCounter;//是否启用日志文件计数.
			DWORD			_maxFileSize;//日志文件大小上限
			DWORD			_nFileIndex;//日志文件下标

			String			_fileName;//日志文件名称(文件名称+后缀,请自行指定文件后缀)
			String			_fileDir;//日志文件所在目录（非全路径）
			String			_defaultExt;//日志文件的默认后缀(当不指定文件名时有效，默认为_T(".g"))
		
			TLogSave			_saveType;//存储类型
			String				_path;//日志全路径
			int					_nFirstTime;//记录第几次调用写文件
			CRITICAL_SECTION	_hCritialSec;
			StringBuilder		_collection;

		public:
			LogHelper();
			~LogHelper();
			
			void Init();			
			String FormatText(LPCTSTR text);

			void InnerOutput(LPCTSTR text);
			void InnerOutputToFile();
			void InnerOutputToFile(String text);
			void InnerOutputToFile(BYTE* buffer,int size);
			void Clear();

			void Lock();
			void Unlock();

			String FindUsableFileName();
		};
	}	
}
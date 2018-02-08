#include "AmLog.h"
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h> 
#include <log4cplus/loggingmacros.h>
#include <Poco/MemoryPool.h>
#include <memory>
#include <tchar.h>
#include <xlocale>
using namespace log4cplus; 
using namespace log4cplus::helpers; 


// Link Lib 
#ifndef _DEBUG  
#pragma comment(lib,"log4cplusS.lib") 
#else 
#pragma comment(lib,"log4cplusSD.lib") 
#endif 
#pragma comment(lib, "WS2_32.lib")

#define MAX_FILE_SIZE 20*1024*1024   //最大文件大小为20m

#define DEFAULT_BUFFER_SIZE  8192    //内存块大小
#define BUFFER_POOL_SIZE     100     //内存池大小


namespace AmLog{

	static  bool isInit = false;
	Poco::MemoryPool pBufferPool(DEFAULT_BUFFER_SIZE*sizeof(CHAR), 0, BUFFER_POOL_SIZE);

	//Poco::MemoryPool pModulePool(MODULE_SIZE*sizeof(WCHAR), 0, BUFFER_POOL_SIZE);

	//初始化日志组件
	bool InitLog(const CHAR *v_pFileName, const CHAR *v_pModuleName, LOG_LEVEL v_level)
	{
		bool bRet = false;

		do 
		{
			if(!v_pFileName || !v_pModuleName)
			{
				break;
			}

			std::locale::global(std::locale("chs"));

			if (!isInit)
			{
				log4cplus::initialize();
				isInit = true;
			}

			SharedAppenderPtr pFileAppender(
				new RollingFileAppender(v_pFileName, 
				MAX_FILE_SIZE, 
				1, 
				true, 
				true)); 

			// 定义一个PatternLayout,并绑定到 pFileAppender 
			std::auto_ptr<Layout> pPatternLayout(new PatternLayout(_T("%D [%-4t] (%-5p) [%-6c] %m%n"))); 
			pFileAppender->setLayout(pPatternLayout); 
			Logger pLogger = Logger::getInstance(v_pModuleName);  // 定义Logger 
			pLogger.setLogLevel(v_level);               //设置日志级别

			// 将需要关联Logger的Appender添加到Logger上 
			pLogger.addAppender(pFileAppender); 

			bRet = true;

		} while (FALSE);

		return bRet;
	}


	//设置日志等级
	bool SetLevel(const CHAR *v_pModuleName,
		LOG_LEVEL v_level)
	{

		bool bRet = false;

		if (v_pModuleName)
		{
			Logger::getInstance(v_pModuleName).setLogLevel(v_level);
			bRet = true;
		}

		return bRet;
	}


	//设置日志等级
	bool Log(const CHAR *v_pModuleName,
		LOG_LEVEL v_level, CHAR *v_pFormat, ...)
	{
		bool bRet = false;


		if (!v_pModuleName || !v_pFormat)
		{
			return bRet;
		}

		CHAR *pszBuffer = (CHAR *)pBufferPool.get();

		if (pszBuffer)
		{
			va_list args;
			int len = 0;

			va_start( args, v_pFormat );
			len = _vsctprintf( v_pFormat, args ) // _vscprintf doesn't count
				+ 1; // terminating '\0'

			if (len > DEFAULT_BUFFER_SIZE)
			{
				len = DEFAULT_BUFFER_SIZE-1;
				pszBuffer[len] = '\0';
			}

			_vstprintf_s( pszBuffer, len, v_pFormat, args );

			va_end(args);//va_end宏将我们传递给它的实参--va_list类型的指针复位成空值。


			switch(v_level)
			{
			case LEVEL_TRACE:
				LOG4CPLUS_TRACE(Logger::getInstance(v_pModuleName), pszBuffer);
				break;
			case LEVEL_DEBUG:
				LOG4CPLUS_DEBUG(Logger::getInstance(v_pModuleName), pszBuffer);
				break;
			case LEVEL_INFO:
				LOG4CPLUS_INFO(Logger::getInstance(v_pModuleName), pszBuffer);
				break;
			case LEVEL_WARN:
				LOG4CPLUS_WARN(Logger::getInstance(v_pModuleName), pszBuffer);
				break;
			case LEVEL_ERROR:
				LOG4CPLUS_ERROR(Logger::getInstance(v_pModuleName), pszBuffer);
				break;
			case LEVEL_FATAL:
				LOG4CPLUS_FATAL(Logger::getInstance(v_pModuleName), pszBuffer);
				break;
			default:
				break;
			}

			pBufferPool.release(pszBuffer);

			bRet = true;
		}



		return bRet;
	}



	//打印普通信息
	bool Trace(const CHAR *v_pModuleName,
		CHAR *v_pFormat,...)
	{

		bool bRet = false;

		if (!v_pModuleName || !v_pFormat)
		{
			return bRet;
		}

		CHAR *pszBuffer = (CHAR *)pBufferPool.get();

		if (pszBuffer)
		{
			va_list args;
			int len = 0;

			va_start( args, v_pFormat );
			len = _vsctprintf( v_pFormat, args ) // _vscprintf doesn't count
				+ 1; // terminating '\0'

			if (len > DEFAULT_BUFFER_SIZE)
			{
				len = DEFAULT_BUFFER_SIZE-1;
				pszBuffer[len] = '\0';
			}

			_vstprintf_s( pszBuffer, len, v_pFormat, args );
	
			va_end(args);//va_end宏将我们传递给它的实参--va_list类型的指针复位成空值。

			LOG4CPLUS_TRACE(Logger::getInstance(v_pModuleName), pszBuffer);

			pBufferPool.release(pszBuffer);


			bRet = true;
		}


		return bRet;
	}


	//打印调试信息
	bool Debug(const CHAR *v_pModuleName,
		CHAR *v_pFormat,...)
	{
		bool bRet = false;

		if (!v_pModuleName || !v_pFormat)
		{
			return bRet;
		}

		CHAR *pszBuffer = (CHAR *)pBufferPool.get();

		if (pszBuffer)
		{
			va_list args;
			int len = 0;

			va_start( args, v_pFormat );
			len = _vsctprintf( v_pFormat, args ) // _vscprintf doesn't count
				+ 1; // terminating '\0'

			if (len > DEFAULT_BUFFER_SIZE)
			{
				len = DEFAULT_BUFFER_SIZE-1;
				pszBuffer[len] = '\0';
			}

			_vstprintf_s( pszBuffer, len, v_pFormat, args );
			LOG4CPLUS_DEBUG(Logger::getInstance(v_pModuleName), pszBuffer);

			va_end(args);//va_end宏将我们传递给它的实参--va_list类型的指针复位成空值。

			pBufferPool.release(pszBuffer);

			bRet = true;
		}

		return bRet;
	}


	//打印提示信息
	bool Info(const CHAR *v_pModuleName,
		CHAR *v_pFormat,...)
	{
		bool bRet = false;

		if (!v_pModuleName || !v_pFormat)
		{
			return bRet;
		}

		CHAR *pszBuffer = (CHAR *)pBufferPool.get();

		if (pszBuffer)
		{
			va_list args;
			int len = 0;

			va_start( args, v_pFormat );
			len = _vsctprintf( v_pFormat, args ) // _vscprintf doesn't count
				+ 1; // terminating '\0'

			if (len > DEFAULT_BUFFER_SIZE)
			{
				len = DEFAULT_BUFFER_SIZE-1;
				pszBuffer[len] = '\0';
			}

			_vstprintf_s( pszBuffer, len, v_pFormat, args );
			LOG4CPLUS_INFO(Logger::getInstance(v_pModuleName), pszBuffer);

			va_end(args);//va_end宏将我们传递给它的实参--va_list类型的指针复位成空值。

			pBufferPool.release(pszBuffer);

			bRet = true;
		}

		return bRet;
	}


	//打印警告信息
	bool Warn(const CHAR *v_pModuleName,
		CHAR *v_pFormat,...)
	{
		bool bRet = false;

		if (!v_pModuleName || !v_pFormat)
		{
			return bRet;
		}

		CHAR *pszBuffer = (CHAR *)pBufferPool.get();

		if (pszBuffer)
		{
			va_list args;
			int len = 0;

			va_start( args, v_pFormat );
			len = _vsctprintf( v_pFormat, args ) // _vscprintf doesn't count
				+ 1; // terminating '\0'

			if (len > DEFAULT_BUFFER_SIZE)
			{
				len = DEFAULT_BUFFER_SIZE-1;
				pszBuffer[len] = '\0';
			}

			_vstprintf_s( pszBuffer, len, v_pFormat, args );
			LOG4CPLUS_WARN(Logger::getInstance(v_pModuleName), pszBuffer);

			va_end(args);//va_end宏将我们传递给它的实参--va_list类型的指针复位成空值。

			pBufferPool.release(pszBuffer);

			bRet = true;
		}
		return bRet;
	}


	//打印普通错误
	bool Error(const CHAR *v_pModuleName,
		CHAR *v_pFormat,...)
	{
		bool bRet = false;

		if (!v_pModuleName || !v_pFormat)
		{
			return bRet;
		}

		CHAR *pszBuffer = (CHAR *)pBufferPool.get();

		if (pszBuffer)
		{
			va_list args;
			int len = 0;

			va_start( args, v_pFormat );
			len = _vsctprintf( v_pFormat, args ) // _vscprintf doesn't count
				+ 1; // terminating '\0'

			if (len > DEFAULT_BUFFER_SIZE)
			{
				len = DEFAULT_BUFFER_SIZE-1;
				pszBuffer[len] = '\0';
			}

			_vstprintf_s( pszBuffer, len, v_pFormat, args );
			LOG4CPLUS_ERROR(Logger::getInstance(v_pModuleName), pszBuffer);

			va_end(args);//va_end宏将我们传递给它的实参--va_list类型的指针复位成空值。

			pBufferPool.release(pszBuffer);

			bRet = true;
		}

		return bRet;
	}


	//打印致命错误
	bool Fatal(const CHAR *v_pModuleName,
		CHAR *v_pFormat,...)
	{
		bool bRet = false;


		if (!v_pModuleName || !v_pFormat)
		{
			return bRet;
		}

		CHAR *pszBuffer = (CHAR *)pBufferPool.get();

		if (pszBuffer)
		{
			va_list args;
			int len = 0;

			va_start( args, v_pFormat );
			len = _vsctprintf( v_pFormat, args ) // _vscprintf doesn't count
				+ 1; // terminating '\0'

			if (len > DEFAULT_BUFFER_SIZE)
			{
				len = DEFAULT_BUFFER_SIZE-1;
				pszBuffer[len] = '\0';
			}

			_vstprintf_s( pszBuffer, len, v_pFormat, args );
			LOG4CPLUS_FATAL(Logger::getInstance(v_pModuleName), pszBuffer);

			va_end(args);//va_end宏将我们传递给它的实参--va_list类型的指针复位成空值。

			pBufferPool.release(pszBuffer);

			bRet = true;
		}

		return bRet;
	}

}


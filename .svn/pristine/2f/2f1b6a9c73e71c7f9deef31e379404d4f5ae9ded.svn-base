
#ifndef _AM_LOG_H
#define _AM_LOG_H
#include <wtypes.h>

#ifdef AMLOG_EXPORTS
#define AMLOG_API __declspec(dllexport)
#else
#define AMLOG_API __declspec(dllimport)
#endif

enum LOG_LEVEL{
	LEVEL_TRACE = 0,
	LEVEL_DEBUG = 10000,
	LEVEL_INFO  = 20000,
	LEVEL_WARN  = 30000,
	LEVEL_ERROR = 40000,
	LEVEL_FATAL = 50000,
	LEVEL_OFF   = 60000
};


#define AM_LOG(moduleName,level, format, ...) \
	AmLog::Log( moduleName, level, "[%s(%d)] "format, __FILE__, __LINE__ , ##__VA_ARGS__);

#define AM_TRACE(moduleName, format, ...) \
	AmLog::Trace( moduleName, "[%s(%d)] "format, __FILE__, __LINE__ , ##__VA_ARGS__);

#define AM_DEBUG(moduleName, format, ...) \
	AmLog::Debug( moduleName, "[%s(%d)] "format, __FILE__, __LINE__ , ##__VA_ARGS__);

#define AM_INFO(moduleName, format, ...) \
	AmLog::Info( moduleName, "[%s(%d)] "format, __FILE__, __LINE__ , ##__VA_ARGS__);

#define AM_WARN(moduleName, format, ...) \
	AmLog::Warn( moduleName, "[%s(%d)] "format, __FILE__, __LINE__ , ##__VA_ARGS__);

#define AM_ERROR(moduleName, format, ...) \
	AmLog::Error( moduleName, "[%s(%d)] "format, __FILE__, __LINE__ , ##__VA_ARGS__);

#define AM_FATAL(moduleName, format, ...) \
	AmLog::Fatal( moduleName, "[%s(%d)] "format, __FILE__, __LINE__ , ##__VA_ARGS__);



namespace AmLog{

	extern "C" {

	
		/************************************************************************/
		/* 函数名称：InitLog
		 * 功能说明：初始化日志组件
		 * 输入参数：v_pFileName：  日志文件名
		             v_pModuleName：模块名称
					 v_level：      日志等级
		 * 输出参数：无
		 * 返 回 值：bool：tru成功，false失败
		 * 备注信息：
		*/
		/************************************************************************/
		AMLOG_API bool  __cdecl InitLog(const char *v_pFileName, 
			const char *v_pModuleName,
			LOG_LEVEL v_level = LEVEL_TRACE);


		/************************************************************************/
		/* 函数名称：SetLevel
		 * 功能说明：设置日志级别
		 * 输入参数：v_pModuleName：模块名称
					 v_level：      日志等级
		 * 输出参数：无
		 * 返 回 值：bool：tru成功，false失败
		 * 备注信息：
		*/
		/************************************************************************/
		AMLOG_API bool  __cdecl SetLevel(const char *v_pModuleName,
			LOG_LEVEL v_level);

		/************************************************************************/
		/* 函数名称：Log
		 * 功能说明：打印日志公共接口
		 * 输入参数：v_pModuleName：模块名称
					 v_level：      日志等级
					 v_pFormat：    格式化输出日志
		 * 输出参数：无
		 * 返 回 值：bool：tru成功，false失败
		 * 备注信息：
		*/
		/************************************************************************/
		AMLOG_API bool  __cdecl Log(const char *v_pModuleName,
			LOG_LEVEL v_level, char *v_pFormat, ...);


		/************************************************************************/
		/* 函数名称：Trace
		 * 功能说明：打印普通日志
		 * 输入参数：v_pModuleName：模块名称
					 v_pFormat：    格式化输出日志
		 * 输出参数：无
		 * 返 回 值：bool：tru成功，false失败
		 * 备注信息：
		*/
		/************************************************************************/
		AMLOG_API bool  __cdecl Trace(const char *v_pModuleName,
			char *v_pFormat,...);

		//打印调试信息
		AMLOG_API bool  __cdecl Debug(const char *v_pModuleName,
			char *v_pFormat,...);

		//打印提示信息
		AMLOG_API bool  __cdecl Info(const char *v_pModuleName,
			char *v_pFormat,...);

		//打印警告信息
		AMLOG_API bool  __cdecl Warn(const char *v_pModuleName,
			char *v_pFormat,...);

		//打印普通错误
		AMLOG_API bool  __cdecl Error(const char *v_pModuleName,
			char *v_pFormat,...);

		//打印致命错误
		AMLOG_API bool  __cdecl Fatal(const char *v_pModuleName,
			char *v_pFormat,...);
	}
}
#endif
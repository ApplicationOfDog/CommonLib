#pragma once

namespace System{
	namespace IO{

//定义调试的级别
enum DebugLevel{
	DEBUG_LEVEL_NULL=TL_None,//关闭调试输出
	DEBUG_LEVEL_DBG=TL_Debug,
	DEBUG_LEVEL_INFO=TL_Normal,
	DEBUG_LEVEL_WARNING=TL_Warning,
	DEBUG_LEVEL_ERROR=TL_Error,
	DEBUG_LEVEL_ALL=TL_All //认打开所有消息
};

/// <summary>
/// 调试器分两部分:1.运行时输出调试,2.内存数据输出调试
/// </summary>
class API_CORE Debuger
{
private:
	TextLog _innerLog;
	BOOL _isOpenMessageBox;//是否打开弹出MessageBox标识,用户命令行调试(目的是让程序弹出窗口停止运行等待调试)

public:
	Debuger(IN LPCTSTR header=NULL);
	LPCTSTR Header();
	void Header(IN LPCTSTR value);//调试信息的头部,即前缀
	void IsOutputEnter(IN bool value);//是否在末尾输出换行符
	void SaveType(IN TLogSave type);//数据存储类型

	///<summary>参数设置</summary>
	///<param name="isShowTime">获取或设置时间标识,即是否记录时间</param>
	///<param name="isClearFirst">获取或设置清空标识,即在第一次写日志之前是否先清空日志</param>
	///<param name="isEndEnterChar">获取或设置是否输出换行符标识</param>
	void Parameters(IN bool isShowTime,IN bool isClearFirst,IN bool isEndEnterChar);

	///<summary>获取或设置level值
	/// 记录的级别，可以是多个DebugLevel值的组合
	///</summary>
	void RecordLevel(DWORD levels);
	DWORD RecordLevel();

	void FileDir(LPCTSTR fileDir);//日志文件的存储位置(文件目录),LS_File_XX是有效
	void IsOpenMessageBox(BOOL val);

	///<summary>输出调试信息</summary>
	///<param name="level">调试信息的级别</param>
	///<param name="format">调试信息的格式化字符串</param>
	void Output(IN DebugLevel level,IN LPCTSTR format,...);
	void Output(IN DebugLevel level,IN String& text);
	void Output(LPCTSTR format,...);
#ifdef _UNICODE
	void OutputA(IN DebugLevel level,IN LPCSTR format,...);
	void OutputA(IN DebugLevel level,IN string& text);
#endif

	void Error(LPCTSTR format,...);
	void Warning(LPCTSTR format,...);
	void Debug(LPCTSTR format,...);
	void MessageBox(DebugLevel level,LPCTSTR format,...);
};

typedef Debuger* PDebuger;

#define GDebuger //由各个模块自己定义GDebuger

#define DEBUG_STRING(_string,...) GDebuger->Output(DEBUG_LEVEL_INFO,_T(_string),__VA_ARGS__)	//只有当前的调试级别是DEBUG_LEVEL_INFO才输出信息
#define DEBUG_STRING_WARNING(_string,...)	GDebuger->Warning(_T(_string),__VA_ARGS__)			//只有当前的调试级别是DEBUG_LEVEL_WARNING才输出信息
#define DEBUG_STRING_ERROR(_string,...)		GDebuger->Error(_T(_string),__VA_ARGS__)			//只有当前的调试级别是DEBUG_LEVEL_ERROR才输出信息
#define DEBUG_STRING_DBG(_string,...)	GDebuger->Debug(_T(_string),__VA_ARGS__)				//只有当前的调试级别是DEBUG_LEVEL_DBG才输出信息
#define OUTPUT_STRING(_string,...) GDebuger->Output(DEBUG_LEVEL_ALL,_T(_string),__VA_ARGS__)	//不管当前的调试级别都会输出信息
#define DEBUG_STRING_LEVEL(_level,_string,...)	GDebuger->Output(_level,_T(_string),__VA_ARGS__)

#define DEBUG_MESSAGEBOX(_string,...) GDebuger->MessageBox(DEBUG_LEVEL_DBG,_T(_string),__VA_ARGS__)

//所有版本均可输出
#define OUTPUT_DEBUG_STRING(fmt,...)	TextLog::DebugString(_T(fmt),__VA_ARGS__)

#ifdef _DEBUG
	#define LOG_DEBUG_STRING(fmt,...)	TextLog::DebugString(_T(fmt),__VA_ARGS__)
#else 
	#define LOG_DEBUG_STRING(fmt,...) 
#endif

}}
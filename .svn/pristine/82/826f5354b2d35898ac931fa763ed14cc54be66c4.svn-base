#pragma once

namespace System{
	namespace IO{

enum TLog{
	TL_None=0x0,	//无消息
	TL_Debug=0x1,	//调试消息(与普通信息类似，但是该类型针对非常频繁的消息)
	TL_Normal=0x2,	//普通消息
	TL_Warning=0x4,	//警告消息
	TL_Error=0x8,	//错误消息
	TL_Failed=0x10,	//失败消息
	TL_Fatal=0x20,  //严重错误消息
	TL_All=0xff		//所有消息
};
///日志保存方式
enum TLogSave{
	LS_File_Immediate=0, //立即输出到文件
	LS_File_LastOut,//即先将日志保存在内存，然后一次性写入文件。减少IO读写
	LS_DbgView_Always, //使用OutputDebugString输出日志内容,不管Debug或Release版本都输出
	LS_DbgView_Debug, //使用OutputDebugString输出日志内容,Debug版输出 而Release版不输出
	LS_Console_Always, //输出到当前控制台,不管Debug或Release版本都输出
	LS_Console_Debug //输出到当前控制台,Debug版输出 而Release版不输出
};

///<summary>文本日志</summary>
class API_CORE TextLog
{
protected:
	void*	_pHelper;
	DWORD	_recordType;//记录类型,是TLog类型值的组合,例如TL_Normal|TL_Warning

public:
	TextLog();
	TextLog(IN TLogSave type,IN String path);
	virtual ~TextLog();

	///<summary>获取或设置日志全路径</summary>
	String Path();
	void Path(IN String val);

	///<summary>获取或设置日志文件名称(例如:121.txt)</summary>
	String FileName();
	void FileName(IN String val);

	///<summary>日志文件所在目录（非全路径）,支持设置绝对路径和相对路径（相对当前程序路径）</summary>
	///<example>
	/// 例如相对路径:dir="AA\\BB",就是将日志保存到当前路径下的AA文件下的BB文件夹下
	/// 例如绝对路径:dir="c:\\AA",将日志生成到c盘下的AA文件夹下
	///</example>
	void FileDir(IN String val);

	///<summary>获取或设置日志记录类型</summary>
	DWORD RecordType();
	void RecordType(IN DWORD val);

	///<summary>获取或设置日志的存储类型</summary>
	TLogSave SaveType();
	void SaveType(IN TLogSave val);

	///<summary>获取或设置head文本</summary>
	LPCTSTR HeadText();
	void HeadText(IN String val);

	///<summary>获取或设置时间标识,即是否记录时间</summary>
	bool IsShowTime();
	void IsShowTime(IN bool val);

	///<summary>获取或设置清空标识,即在第一次写日志之前是否先清空日志</summary>
	bool IsClearFirst();
	void IsClearFirst(IN bool val);

	///<summary>获取或设置Debug标识,即调用者版本是否为Debug版本。这在LS_DbgView_Debug模式下会影响输出</summary>
	bool IsDebugStatus();
	void IsDebugStatus(bool val);

	///<summary>获取或设置是否启用日志文件计数</summary>
	bool IsStartupCounter();
	void IsStartupCounter(IN bool val);

	///<summary>获取或设置日志文件大小上限</summary>
	DWORD MaxFileSize();
	void MaxFileSize(IN DWORD val);

	///<summary>获取或设置是否输出换行符标识</summary>
	DWORD IsEndEnterChar();
	void IsEndEnterChar(IN bool value);

	///<summary>格式化文本</summary>
	String FormatText(IN LPCTSTR text);

	///<summary>是否会记录该类型</summary>
	bool IsRecord(IN TLog type);

	///<summary>输出日志:普通日志</summary>
	virtual void Output(String text);
	virtual void Output(LPCTSTR format,...);
	virtual void Output(BYTE* buffer,int size);

	///<summary>输出日志:错误日志</summary>
	virtual void Error(String text);
	virtual void Error(LPCTSTR format,...);
	virtual void Error(BYTE* buffer,int size);

	///<summary>输出日志:警告日志</summary>
	virtual void Warning(String text);
	virtual void Warning(LPCTSTR format,...);
	virtual void Warning(BYTE* buffer,int size);

	///<summary>输出日志:警告日志</summary>
	virtual void Failed(String text);
	virtual void Failed(LPCTSTR format,...);
	virtual void Failed(BYTE* buffer,int size);

	///<summary>输出日志:指定类型日志</summary>
	virtual void Output(TLog type,String text);
	virtual void Output(TLog type,LPCTSTR format,...);
	virtual void Output(TLog type,BYTE* buffer,int size);
	virtual void Output(IN TLog type,IN LPCTSTR format,IN va_list args);
#ifdef _UNICODE
	virtual void OutputA(IN TLog type,IN LPCSTR format,IN va_list args);
	virtual void OutputA(IN TLog type,IN string& text);
#endif

	///<summary>对LS_File_LastOut模式有效,将当前内存中的日志输出到文件</summary>
	virtual void OutputToFile();

	///<summary>清空日志</summary>
	virtual void Clear();
	static void DebugString(String text);
	static void DebugString(LPCTSTR format,...);
};

}}
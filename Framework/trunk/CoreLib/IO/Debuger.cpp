#include "stdafx.h"
#include "Debuger.hpp"

namespace System{
	namespace IO{

		Debuger::Debuger(IN LPCTSTR header)
		{
			_innerLog.IsEndEnterChar(TRUE);
			_innerLog.IsShowTime(FALSE);
			_isOpenMessageBox=FALSE;//MessageBox标识默认是不打开的，除非是调试状态手动打开
#ifdef _DEBUG
			_innerLog.RecordType(DEBUG_LEVEL_ALL); //debug版默认打开所有消息
#else
			_innerLog.RecordType(DEBUG_LEVEL_WARNING|DEBUG_LEVEL_ERROR); //release版默认只显示警告信息和错误信息
#endif
			_innerLog.SaveType(LS_DbgView_Always);//默认使用debugview调试

			if(header!=NULL && *header!=0)
				_innerLog.HeadText(header);
		}

		void Debuger::Header(IN LPCTSTR value){
			_innerLog.HeadText(value);
		}
		LPCTSTR Debuger::Header(){
			return _innerLog.HeadText();
		}
		void Debuger::IsOutputEnter(bool value){
			_innerLog.IsEndEnterChar(value);
		}
		void Debuger::SaveType(TLogSave type){
			_innerLog.SaveType(type);
		}
		void Debuger::RecordLevel(DWORD levels){
			_innerLog.RecordType(levels);
		}
		DWORD Debuger::RecordLevel()
		{
			return _innerLog.RecordType();
		}
		void Debuger::FileDir(LPCTSTR fileDir){
			_innerLog.FileDir(fileDir);
		}
		void Debuger::IsOpenMessageBox(BOOL val){
			_isOpenMessageBox=val;
		}
		void Debuger::Parameters(IN bool isShowTime,IN bool isClearFirst,IN bool isEndEnterChar)
		{
			_innerLog.IsShowTime(isShowTime);
			_innerLog.IsClearFirst(isClearFirst);
			_innerLog.IsEndEnterChar(isEndEnterChar);
		}
		void Debuger::Output(IN DebugLevel level,IN LPCTSTR format,...)
		{
			va_list ap;
			va_start(ap,format);
			_innerLog.Output((TLog)level,format,ap);
			va_end(ap);
		}
#ifdef _UNICODE
		void Debuger::OutputA(IN DebugLevel level,IN LPCSTR format,...)
		{
			va_list ap;
			va_start(ap,format);
			_innerLog.OutputA((TLog)level,format,ap);
			va_end(ap);
		}
		void Debuger::OutputA(IN DebugLevel level,IN string& text)
		{
			_innerLog.OutputA((TLog)level,text);
		}
#endif
		void Debuger::Output(IN DebugLevel level,IN String& text)
		{
			_innerLog.Output((TLog)level,text);
		}
		void Debuger::Output(LPCTSTR format,...)
		{
			va_list ap;
			va_start(ap,format);
			_innerLog.Output(TL_Normal,format,ap);
			va_end(ap);
		}
		void Debuger::Error(LPCTSTR format,...)
		{
			va_list ap;
			va_start(ap,format);
			_innerLog.Output(TL_Error,format,ap);
			va_end(ap);
		}
		void Debuger::Warning(LPCTSTR format,...)
		{
			va_list ap;
			va_start(ap,format);
			_innerLog.Output(TL_Warning,format,ap);
			va_end(ap);
		}
		void Debuger::Debug(LPCTSTR format,...)
		{
			va_list ap;
			va_start(ap,format);
			_innerLog.Output(TL_Debug,format,ap);
			va_end(ap);
		}
		void Debuger::MessageBox(DebugLevel level,LPCTSTR format,...)
		{
			if(!_isOpenMessageBox) return;
			if(!_innerLog.IsRecord((TLog)level)) return;

			va_list ap;
			va_start(ap,format);
			String message = String::Format(format,ap);	
			::MessageBox(NULL,message,_innerLog.HeadText(),MB_OK);
			va_end(ap);
		}


}}
#include "stdafx.h"
#include "Helper/LogHelper.hpp"
#include "TextLog.hpp"

#define PHELPER ((LogHelper*)_pHelper)

namespace System
{
	namespace IO
	{
		TextLog::TextLog()
		{
			_recordType=TL_All;
			_pHelper = new LogHelper();
		}
		TextLog::TextLog(TLogSave type,String path)
		{
			_recordType=TL_All;
			_pHelper = new LogHelper();
			PHELPER->_saveType=type;
			PHELPER->_path=path;
		}
		TextLog::~TextLog()
		{
			DELETE_OBJECT(_pHelper);
		}

		//// method ////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		String TextLog::Path(){ return PHELPER->_path; }
		void TextLog::Path(String val){ PHELPER->_path=val; }

		String TextLog::FileName(){ return PHELPER->_fileName; }
		void TextLog::FileName(String val){ PHELPER->_fileName=val; }

		void TextLog::FileDir(IN String val){ PHELPER->_fileDir=val; }

		DWORD TextLog::RecordType(){ return _recordType; }
		void TextLog::RecordType(IN DWORD val){ _recordType=val; }

		TLogSave TextLog::SaveType(){ return PHELPER->_saveType; }
		void TextLog::SaveType(IN TLogSave val){ PHELPER->_saveType=val; }

		LPCTSTR TextLog::HeadText(){ return PHELPER->_headText.c_str(); }
		void TextLog::HeadText(IN String val){ PHELPER->_headText=val; }

		bool TextLog::IsShowTime(){ return PHELPER->_isShowTime; }
		void TextLog::IsShowTime(IN bool val){ PHELPER->_isShowTime=val; }

		bool TextLog::IsClearFirst(){ return PHELPER->_isClearFirst; }
		void TextLog::IsClearFirst(IN bool val){ PHELPER->_isClearFirst=val; }

		bool TextLog::IsDebugStatus(){ return PHELPER->_isDebugStatus; }
		void TextLog::IsDebugStatus(bool val){ PHELPER->_isDebugStatus=val; }

		bool TextLog::IsStartupCounter(){ return PHELPER->_isStartupCounter; }
		void TextLog::IsStartupCounter(IN bool val){ PHELPER->_isStartupCounter=val; }

		DWORD TextLog::MaxFileSize(){ return PHELPER->_maxFileSize; }
		void TextLog::MaxFileSize(IN DWORD val){ PHELPER->_maxFileSize=val; }

		DWORD TextLog::IsEndEnterChar(){ return PHELPER->_isEndEnterChar; }
		void TextLog::IsEndEnterChar(IN bool value){ PHELPER->_isEndEnterChar=value; }

		bool TextLog::IsRecord(IN TLog type){ return IS_FLAG_ON(_recordType,type); }

		void TextLog::Output(TLog type,String text)
		{
			if(!FLAG_ON(type,_recordType)) return;//不是需要记录的类型，跳过

			PHELPER->InnerOutput(text);
		}
		void TextLog::Output(TLog type,LPCTSTR format,...)
		{
			if(!FLAG_ON(type,_recordType)) return;
			if(format==NULL||*format==0) return;

			va_list ap;
			va_start(ap,format);
			String text=String::Format(format,ap);
			va_end(ap);
			PHELPER->InnerOutput(text);
		}
		void TextLog::Output(IN TLog type,IN LPCTSTR format,IN va_list args)
		{
			if(!FLAG_ON(type,_recordType)) return;
			if(format==NULL||*format==0) return;

			String text=String::Format(format,args);
			PHELPER->InnerOutput(text);
		}
#ifdef _UNICODE
		void TextLog::OutputA(IN TLog type,IN LPCSTR format,IN va_list args)
		{
			if(!FLAG_ON(type,_recordType)) return;
			if(format==NULL||*format==0) return;

			String text = String::Format(format,args);
			PHELPER->InnerOutput(text);
		}
		void TextLog::OutputA(IN TLog type,IN string& text)
		{
			if(!FLAG_ON(type,_recordType)) return;
			if(text.empty()) return;

			String text_out = Convert::ToString(text);
			PHELPER->InnerOutput(text_out);
		}
#endif
		void TextLog::Output(TLog type,BYTE* buffer,int size)
		{
			assert(PHELPER->_saveType==LS_File_Immediate);
			if(!FLAG_ON(type,_recordType)) return;

			PHELPER->InnerOutputToFile(buffer,size);
		}
		void TextLog::Output(String text)
		{
			Output(TL_Normal,text);
		}
		void TextLog::Error(String text)
		{
			Output(TL_Error,text);
		}
		void TextLog::Warning(String text)
		{
			Output(TL_Warning,text);
		}
		void TextLog::Failed(String text)
		{
			Output(TL_Failed,text);
		}
		void TextLog::Output(LPCTSTR format,...)
		{
			if(!FLAG_ON(_recordType,TL_Normal)) return;
			if(format==NULL) return;

			va_list ap;
			va_start(ap,format);
			String text=String::Format(format,ap);
			va_end(ap);
			PHELPER->InnerOutput(text);
		}
		void TextLog::Error(LPCTSTR format,...)
		{
			if(!FLAG_ON(_recordType,TL_Error)) return;
			if(format==NULL) return;

			va_list ap;
			va_start(ap,format);
			String text=String::Format(format,ap);
			va_end(ap);
			PHELPER->InnerOutput(text);
		}
		void TextLog::Warning(LPCTSTR format,...)
		{
			if(!FLAG_ON(_recordType,TL_Warning)) return;
			if(format==NULL) return;

			va_list ap;
			va_start(ap,format);
			String text=String::Format(format,ap);
			va_end(ap);
			PHELPER->InnerOutput(text);
		}
		void TextLog::Failed(LPCTSTR format,...)
		{
			if(!FLAG_ON(_recordType,TL_Failed)) return;
			if(format==NULL) return;

			va_list ap;
			va_start(ap,format);
			String text=String::Format(format,ap);
			va_end(ap);
			PHELPER->InnerOutput(text);
		}
		void TextLog::Output(BYTE* buffer,int size)
		{
			Output(TL_Normal,buffer,size);
		}
		void TextLog::Error(BYTE* buffer,int size)
		{
			Output(TL_Error,buffer,size);
		}
		void TextLog::Warning(BYTE* buffer,int size)
		{
			Output(TL_Warning,buffer,size);
		}
		void TextLog::Failed(BYTE* buffer,int size)
		{
			Output(TL_Failed,buffer,size);
		}
		void TextLog::OutputToFile()
		{
			if(PHELPER->_saveType!=LS_File_LastOut) return;
			PHELPER->InnerOutputToFile();
		}
		void TextLog::Clear(){
			PHELPER->Clear();
		}
		String TextLog::FormatText(LPCTSTR text){
			return PHELPER->FormatText(text);
		}
		void TextLog::DebugString(String text){
			::OutputDebugString(text.c_str());
		}
		void TextLog::DebugString(LPCTSTR format,...)
		{
			if(format==NULL) return;
			va_list ap;
			va_start(ap,format);
			String text=String::Format(format,ap);
			va_end(ap);

			::OutputDebugString(text.c_str());
		}
	}
}
#include "stdafx.h"
#include "MiniDump.hpp"
using namespace System::IO;

namespace System{
	namespace Diagnostics{

		bool MiniDump::_isInstalled=false;
		MINIDUMP_TYPE MiniDump::Type(){
			return _type;
		}
		void MiniDump::Type(MINIDUMP_TYPE type){
			_type=type;
		}
		String MiniDump::PrefixName(){
			return _prefixName;
		}
		void MiniDump::PrefixName(String prefix){
			_prefixName=prefix;
		}
		String MiniDump::FileName(){
			return _fileName;
		}
		void MiniDump::FileName(String fileName){
			if(fileName.IsEmptyOrWhiteSpace()){
				String pePath=Environment::GetExecutablePath(FALSE);
				fileName=Path::GetFileNameWithoutExtension(pePath);
			}
			_fileName=fileName;
		}
		String MiniDump::PathDir(){
			return _pathDir;
		}
		void MiniDump::PathDir(String path){
			_pathDir=path;
		}
		String MiniDump::FilePath(){
			return _filePath;
		}
		void MiniDump::PreviewExceptionHandler(LPCALLBACK_PREEXCEPTION_HANDLER handler)
		{
			_previewExceptionHandler=handler;
		}
		void MiniDump::Set(IN MINIDUMP_TYPE type,
			IN String prefix,IN String fileName,IN String pathDir,
			IN LPCALLBACK_PREEXCEPTION_HANDLER preview)
		{
			MiniDump* gp = MiniDump::Instance();
			gp->_type=type;
			gp->_prefixName=prefix;
			gp->_fileName=fileName;
			gp->_pathDir=pathDir;
			gp->_previewExceptionHandler=preview;
		}
		bool MiniDump::Install(IN LPCALLBACK_PREEXCEPTION_HANDLER preview)
		{
			if(_isInstalled) return true;
			MiniDump* instancePtr = MiniDump::Instance();

			if(preview!=NULL) 
				instancePtr->PreviewExceptionHandler(preview);

			instancePtr->InnerSet();
			if(instancePtr->_pathDir.IsEmptyOrWhiteSpace()) return false;
			if(!Directory::Exists(instancePtr->_pathDir))
				if(!Directory::CreateInLayers(instancePtr->_pathDir)) return false;

			::SetUnhandledExceptionFilter(ExceptionFilter);
			_isInstalled=true;
			return true;
		}


		////////////////////////// inner process /////////////////////////////
		MiniDump::MiniDump()
		{
			DateTime dt=DateTime::Now();

			_type=MiniDumpWithFullMemory;			
			_previewExceptionHandler=NULL;
		}
		void MiniDump::InnerSet()
		{
			DateTime dt=DateTime::Now();
			String pePath=Environment::GetExecutablePath(FALSE);
			if(_prefixName.IsEmptyOrWhiteSpace())
				_prefixName=IO::Path::GetFileNameWithoutExtension(pePath);
			if(_pathDir.IsEmptyOrWhiteSpace())
				_pathDir=IO::Path::GetDirectoryName(pePath)+_T("\\Dump");

			if(_fileName.IsEmptyOrWhiteSpace())
				_fileName=_prefixName+_T("_")+dt.ToString(_T("yyyyMMdd"))+_T(".dmp");
			
			_filePath=IO::Path::Combine(_pathDir,_fileName);
		}
		//////////////////////////////////////////////////////////////////////////
		//内部函数,作为未处理异常的处理函数
		//ExceptionInfo 有操作系统传入的未处理异常的信息
		//////////////////////////////////////////////////////////////////////////

		LONG WINAPI MiniDump::ExceptionFilter(PEXCEPTION_POINTERS exceptionInfo)
		{
			MiniDump* instancePtr = MiniDump::Instance();
			if(instancePtr->_previewExceptionHandler!=NULL)
				instancePtr->_previewExceptionHandler();

			//instancePtr->InnerSet();
			if(File::Exists(instancePtr->_filePath))
				File::Delete(instancePtr->_filePath);

			//创建dump文件
			HANDLE hFile=::CreateFile((LPCTSTR)instancePtr->_filePath,FILE_ALL_ACCESS, FILE_SHARE_READ, NULL,
				OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			MINIDUMP_EXCEPTION_INFORMATION miniExcInfo;
			//存储异常信息
			miniExcInfo.ThreadId = ::GetCurrentThreadId();
			miniExcInfo.ExceptionPointers = exceptionInfo;
			miniExcInfo.ClientPointers = TRUE;

			//写dump文件
			::MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, 
				instancePtr->_type, &miniExcInfo, NULL, NULL);

			CloseHandle(hFile);
			return EXCEPTION_CONTINUE_SEARCH;
		}

}}
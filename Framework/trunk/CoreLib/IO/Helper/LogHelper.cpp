#include "stdafx.h"
#include "LogHelper.hpp"
#define LOGFILELEN_MAX 5242880

namespace System
{
	namespace IO
	{
		LogHelper::LogHelper()
		{
			Init();
			InitializeCriticalSection(&_hCritialSec);
		}
		LogHelper::~LogHelper()
		{			
			DeleteCriticalSection(&_hCritialSec);
		}
		void LogHelper::Lock()
		{
			EnterCriticalSection(&_hCritialSec);
		}
		void LogHelper::Unlock()
		{
			LeaveCriticalSection(&_hCritialSec);
		}

		void LogHelper::Init()
		{			
			_saveType=LS_File_Immediate;			
			_path=STRING_EMPTY;
			_nFirstTime=0;

			_isShowTime=TRUE;
			_headText=STRING_EMPTY;
			_isClearFirst=TRUE;
			_isDebugStatus=TRUE;
			_isEndEnterChar=TRUE;

			_isStartupCounter=FALSE;
			_maxFileSize=LOGFILELEN_MAX;
			_nFileIndex=0;
			_defaultExt=_T(".g");
		}


		//11/01/13 14:54:12 - error(2):Open key failed(key=SYSTEM\CurrentControlSet\services\tsfs)
		String LogHelper::FormatText(LPCTSTR text)
		{
			String _text=STRING_EMPTY;

			if(_isShowTime)
			{
				DateTime now=DateTime::Now();
				String sdate=now.ToString(_T("yy/MM/dd HH:mm:ss"));
				_text.append(sdate);
				_text.append(_T(" - "));
			}
			if(!_headText.IsEmptyOrWhiteSpace())
				_text.append(_headText);

			_text.append(text);

			if(_isEndEnterChar)
				_text.append(_T("\n"));//"\r\n"

			return _text;
		}
		void LogHelper::InnerOutput(LPCTSTR text)
		{
			String _text = FormatText(text);
			
			switch(_saveType)
			{
			case LS_Console_Debug: 
				if(!_isDebugStatus) return;
			case LS_Console_Always:
				Console::Write(_text);//Console::WriteLine(_text);
				break;

			case LS_DbgView_Debug:
				if(!_isDebugStatus) return;
			case LS_DbgView_Always:
				::OutputDebugString(_text);
				break;

			case LS_File_LastOut:
				_collection.AppendLine(text);
				break;
			case LS_File_Immediate:
				InnerOutputToFile(_text);
				break;
			}
		}
		void LogHelper::InnerOutputToFile()
		{
			assert(_saveType==LS_File_LastOut);
			InnerOutputToFile(_collection.ToString());
			_collection.Clear();
		}
		void LogHelper::InnerOutputToFile(String text)
		{
			Lock();

			String path=FindUsableFileName();

			FILE* hfile = _tfsopen((LPCTSTR)path,_T("a+"),_SH_DENYNO);
			assert(hfile!=NULL);
			if(hfile==NULL)
			{
				errno_t err=0;
				_get_errno(&err);
				Unlock();
				return;
			}
			/*errno_t err = _tfopen_s(&hfile,path,_T("a+"));// _tfopen_s打开的文件不可共享(在安全软件打开时可能导致该函数打开文件失败)
			assert(err==0 && hfile!=NULL);
			if(err!=0||hfile==NULL){ 
				Unlock();
				return;
			}*/

#ifdef UNICODE
			_tsetlocale ( LC_ALL, _T("chs"));
#endif
			_ftprintf(hfile,text);
			fclose(hfile);
			hfile=NULL;

			_nFirstTime++;
			Unlock();
		}
		void LogHelper::InnerOutputToFile(BYTE* buffer,int size)
		{
			Lock();
			String path=FindUsableFileName();

			HANDLE hFile = CreateFile(path.c_str(),FILE_ALL_ACCESS, FILE_SHARE_READ, NULL,
				OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				//文件打开成功或者创建成功，将新的日志文件数据写入文件尾部
				//将加密后的数据流写入文件尾部
				::SetFilePointer(hFile,0,NULL,FILE_END);

				DWORD nReal;
				::WriteFile(hFile,buffer,size,&nReal,NULL); 
				::CloseHandle(hFile);
			}

			_nFirstTime++;
			Unlock();
		}
		void LogHelper::Clear()
		{
			Lock();
			switch(_saveType)
			{
			case LS_File_LastOut:
				_collection.Clear();

			case LS_File_Immediate:
				if(_path.IsEmptyOrWhiteSpace()) goto BACK;
				if(_isStartupCounter)//删除计数日志文件
				{
					String sFilePathTemp,sFileNameTemp;
					String fileName=Path::GetFileNameWithoutExtension(_path);
					for (int i=_nFileIndex;i>=0;i--)
					{
						sFileNameTemp=String::Format(_T("%s%d"),fileName.c_str(),i);
						sFilePathTemp=Path::Replace(_path,Path::TFileName,sFileNameTemp);
						if(!File::Exists(sFilePathTemp)) continue;
						File::Delete(sFilePathTemp);
					}
					_nFileIndex=0;					
					goto BACK;
				}				
				if(!File::Exists(_path)) goto BACK;
				File::Delete(_path);//删除日志文件
				break;
			}
BACK:
			Unlock();
		}

		//说明：在客户端本地日志缓存文件夹中查找一个可以用的文件名
		//入参：strDBPath,本地日志数据库保存的位置
		//出参：
		//备注：查找可以用的文件名的规则是：日志文件的名字为符合xxx？的一个文件，其中?是一个从0开始递增的序数，当查找到该文件
		//      还未创建或者文件已存在，但文件大小未超过长度限制时，则说明该文件名是一个可以直接使用的文件名
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		String LogHelper::FindUsableFileName()
		{
			if(_path.empty())
			{				
				String excPath=Environment::GetExecutablePath();
				String fDir=Path::GetDirectoryName(excPath);
				String fName=Path::GetFileNameWithoutExtension(excPath);
				String fExt=_defaultExt;
				if(!_fileDir.empty()){
					String drive = Path::GetRoot(_fileDir);
					if(!drive.empty())
						fDir=_fileDir;
					else
						fDir=Path::Combine(fDir,_fileDir);
				}
				if(!_fileName.empty())
				{
					fName=Path::GetFileNameWithoutExtension(_fileName);
					fExt=Path::GetExtension(_fileName);
				}
				_path=Path::Combine(fDir,fName+fExt);
			}
			
			String fileName=Path::GetFileNameWithoutExtension(_path);
			String fileDir=Path::GetDirectoryName(_path);
			//判断当前存储日志文件的文件夹是否存在
			if(!Directory::Exists(fileDir))
				Directory::CreateInLayers(fileDir);

			if(!_isStartupCounter){ //不启用文件计数
				if(_isClearFirst && _nFirstTime==0 && File::Exists(_path))
					File::Delete(_path);
				return _path;
			}
			
			WIN32_FIND_DATA wfd;
			HANDLE hFind = NULL;
			String sFilePathTemp,sFileNameTemp;		
			
			while (TRUE){ //通过序号基循环查找
				sFileNameTemp=String::Format(_T("%s%d"),fileName.c_str(),_nFileIndex);
				sFilePathTemp=Path::Replace(_path,Path::TFileName,sFileNameTemp);

				hFind=::FindFirstFile(sFilePathTemp.c_str(), &wfd);
				if (hFind == INVALID_HANDLE_VALUE){
					//目标文件未查找到，说明该文件名还未使用，可返回给外部程序使用
					break;
				}

				::FindClose(hFind);
				if(wfd.nFileSizeLow <= _maxFileSize){
					//目标文件存在，但文件长度小于日志文件长度限制，可继续使用
					break;
				}
				//目标文件存在，且文件长度已超过日志文件长度限制，则说明该文件名已是一个等待上传的日志文件，无法使用
				_nFileIndex++;
			}
			
			return sFilePathTemp;
		}
	}	
}
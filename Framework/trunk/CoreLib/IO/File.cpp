#include "stdafx.h"
#if IS_WINDOWS_OS	
	#include <io.h>
	#include <errno.h>
#else
	#include <copyfile.h>
	#include <unistd.h>
	#include <sys/stat.h>
#endif

#include <fstream>
#include "File.hpp"
#include "Path.hpp"
#include "../Core/Convert.hpp"
#include "../Core/Environment.hpp"

namespace System
{
	namespace IO
	{
		bool File::Copy(IN LPCTSTR sourceFileName, IN LPCTSTR destFileName, bool overwrite)
		{
			assert("invalid parameter" && !String::IsNullOrWhiteSpace(sourceFileName));
			assert("source file is not exist" && Exists(sourceFileName));

			assert("invalid parameter" && !String::IsNullOrWhiteSpace(destFileName));

#if IS_WINDOWS_OS
			return ::CopyFile(sourceFileName,destFileName,!overwrite)==TRUE;
#else
			ifstream fsrc(sourceFileName,ios::in|ios::binary);
			if(!fsrc)
			{
				Environment::LastError(ERROR_FILE_NOT_FOUND);
				return false;
			}
			if(!overwrite && File::Exists(destFileName)){//不覆盖目标文件
				Environment::LastError(ERROR_FILE_EXISTS);
				return false;
			}
			ofstream fdest(destFileName.c_str(),ios::out|ios::trunc|ios::binary);//如果目标文件存在则情况目标文件
			if(!fdest) return false;
			char buffer[BUFFERSIZE]={0};
			while(!fsrc.eof())
			{
				fsrc.read(buffer,sizeof(buffer));
				fdest.write(buffer,fsrc.gcount());//gcount:返回最后一个非格式化的抽取方法读取的字符数
			}
			fsrc.close();
			fdest.close();
			return true;
#endif
			
		}
		bool File::CopyToDir(IN LPCTSTR sourceFileName, IN LPCTSTR destDir, bool overwrite)
		{
			assert("invalid parameter" && !String::IsNullOrWhiteSpace(sourceFileName));
			assert("invalid parameter" && !String::IsNullOrWhiteSpace(destDir));
			
			String fileName=Path::GetFileName(sourceFileName);
			String destFilePath=Path::Combine(destDir,fileName);
			return File::Copy(sourceFileName,destFilePath,overwrite);
		}
		bool File::Delete(IN LPCTSTR path)
		{
			assert("invalid parameter" && !String::IsNullOrWhiteSpace(path));
			assert("source file is not exist" && Exists(path));
			return ::_tremove(path)==0;
		}
		/// <summary>
		/// 确定指定的文件是否存在
		/// </summary>
		bool File::Exists(IN LPCTSTR path)
		{
			if(path==NULL||*path==0) return false;

			ifstream ifile(path);
			return ifile.is_open();
		}
		bool File::ExistsA(IN char* path)
		{
			assert(path!=NULL);
#if IS_WINDOWS_OS
			return _access(path,0)==0;
#else
			return access(path,0)==0;
#endif
		}
		bool File::Move(IN LPCTSTR sourceFileName,IN LPCTSTR destFileName,bool isDeleteIfFileExist)
		{
			assert("invalid parameter" && !String::IsNullOrWhiteSpace(sourceFileName));
			assert("invalid parameter" && !String::IsNullOrWhiteSpace(destFileName));

			if(!File::Exists(sourceFileName)) throw invalid_argument("source file is not exist.");
			if(isDeleteIfFileExist && File::Exists(destFileName)) File::Delete(destFileName);
#if IS_WINDOWS_OS
			return ::MoveFile(sourceFileName,destFileName)==TRUE;
#else
			if(File::Copy(sourceFileName,destFileName,TRUE))
				return File::Delete(sourceFileName)==TRUE;
			return false;
#endif
			
		}
		bool File::MoveToDirectory(IN LPCTSTR sourceFileName,IN LPCTSTR destDir,bool isDeleteIfFileExist)
		{
			assert("invalid parameter" && !String::IsNullOrWhiteSpace(sourceFileName));
			assert("invalid parameter" && !String::IsNullOrWhiteSpace(destDir));

			if(!File::Exists(sourceFileName)) throw invalid_argument("source file is not exist.");

			String fileName=Path::GetFileName(sourceFileName);
			String destFileName=Path::Combine(destDir,fileName);

			return File::Move(sourceFileName,destFileName,isDeleteIfFileExist);
		}
		String File::ReadAllText(IN LPCTSTR path,IN Encoding code)
		{
			if(path==NULL||*path==0) return String::Empty;

			String result;
			ifstream file(path);
			if(!file)
			{
				String errtext = Environment::ErrnoText();
				assert("file is not exist!" && false);
				return result;
			}
			file.seekg(0,ios::end);
			long fsize = (long)file.tellg();
			file.seekg(0,ios::beg);
			char* buffer = new char[fsize+sizeof(wchar_t)];			
			if(buffer==NULL)
			{
				String lastError = Environment::LastErrorText();
				assert("alloc memory failed!" && false);
				return result;
			}
			ZeroMemory(buffer,fsize+sizeof(wchar_t));
			file.read(buffer,fsize); 

			switch(code)
			{
			case EC_ANSI://目标文件是ansi编码,即读取到的缓冲区是ansi编码的
				{
#ifdef _UNICODE
					Convert::AnsiToWideChar((byte*)buffer,(int)fsize,result);
#else
					result.assign((char*)buffer,fsize/sizeof(char));
#endif
				}
				break;
			case EC_UNICODE:
			case EC_UTF16LE:
			case EC_UTF16BE:
				{
#ifdef _UNICODE
					result.assign((wchar_t*)buffer);
#else
					wstring temp = (wchar_t*)buffer;
					Convert::WideCharToAnsi(temp,result);
#endif
				}
				break;
			case EC_UTF8:
				{
#ifdef _UNICODE
					Convert::Utf8ToWideChar((byte*)buffer,(int)fsize,result);
#else
					std::string temp = (char*)buffer;
					Convert::Utf8ToAnsi(temp,result);
#endif
				}
				break;
			}

			DELETE_ARRAY(buffer);
			return result;
		}
		bool File::ReadAllText(IN LPCTSTR path,OUT char* pBuffer,OUT long szBuffer)
		{
			if(pBuffer==NULL||path==NULL||*path==0) return false;

			ifstream file(path);
			if(!file)
			{
				String errtext = Environment::ErrnoText();
				assert("file is not exist!" && false);
				return false;
			}
			file.seekg(0,ios::end);
			long fsize = (long)file.tellg();
			file.seekg(0,ios::beg);
			if(szBuffer>fsize) szBuffer = fsize;
			file.read(pBuffer,szBuffer);
			file.close();
			return true;
		}
		ULONGLONG File::GetLength(IN LPCTSTR path,IN bool isText)
		{
			assert(path!=NULL && *path!=0);

			//打开文件并定位到文件末尾
			ifstream file(path,ios::in|ios::ate|(isText?0:ios::binary));
			if(!file)
			{
				String errtext = Environment::ErrnoText();
				return 0;
			}

			ifstream::pos_type size = file.tellg();
			return (ULONGLONG)size;
		}
		size_t File::ReadAllBytes(IN LPCTSTR path,IN byte* buffer)
		{
			return InnerReadBytes(path,buffer,0,FALSE);
		}
		size_t File::ReadAllBytes2(IN LPCTSTR path,OUT byte** pBuffer)
		{
			return InnerReadBytes(path,(byte*)pBuffer,0,TRUE);
		}
		/// <summary>
		/// 从文件中读取一定字节数到buffer中
		/// </summary>
		/// <param name="path">文件路径</param>
		/// <param name="buffer">返回文件内容的缓冲区</param>
		/// <param name="szBuffer">缓冲区大小(0表示读取整个文件)</param>
		/// <returns>读取的字节数</returns>
		size_t File::ReadBytes(IN LPCTSTR path,IN byte* buffer,IN unsigned szBuffer)
		{
			return InnerReadBytes(path,buffer,szBuffer,FALSE);
		}
		vector<String> File::ReadAllLines(IN LPCTSTR path)
		{
			vector<String> lineCollection;
			ifstream file(path);
			if(!file)
			{
				String errtext = Environment::ErrnoText();
				assert("file is not exist!" && false);
				return lineCollection;
			}
			String line;
			string aline;			
			while( getline(file,aline) )
			{
				line = Convert::ToString(aline);
				lineCollection.push_back(line);
			}
			file.close();
			return lineCollection;
		}
		size_t File::WriteAllBytes(IN LPCTSTR path,const byte* buffer,size_t size)
		{
			assert(size>0 && buffer!=NULL);
			if(path==NULL||*path==0) return 0;

			ofstream file(path,ios::out|ios::binary);
			if(!file)
			{
				String errtext = Environment::ErrnoText();
				assert("file is not exist!" && false);
				return 0;
			}
			file.write((char*)buffer,size);
			file.close();
			return size;
		}
		size_t File::WriteAllBytes(FILE* file,const byte* buffer,size_t size)
		{
			assert(size>0 && buffer!=NULL);
			return fwrite(buffer,1,size,file);
		}
		bool File::WriteAllText(IN LPCTSTR path,IN string& contents)
		{
			if(path==NULL||*path==0) return false;

			ofstream file(path,ios::out|ios::trunc);
			if(!file)
			{
				String errtext = Environment::ErrnoText();
				return false;
			}

			char* text = (char*)contents.c_str();
			file.write(text,contents.size());//contents.GetBytes());
			return !file.bad();
		}
		DWORD File::GetAttributes(IN String path)
		{
#if IS_WINDOWS_OS
			return GetFileAttributes(path);
#else
			struct stat statbuf;
			if(stat(path.c_str(),&statbuf)<0){
				return 0;
			}
			return  (DWORD)statbuf.st_mode;
#endif
		}
		bool File::SetAttributes(IN String path,IN DWORD fileAttributes)
		{
#if IS_WINDOWS_OS
			return SetFileAttributes(path,fileAttributes)==TRUE;
#else
			return chmod(path.c_str(),(mode_t)fileAttributes)==0;
#endif
		}
#if IS_WINDOWS_OS
		/// <summary>
		/// 获取文件内容头部的BOM字节数
		/// </summary>
		/// <param name="buffer">文件内容</param>
		/// <returns>BOM的字节数</returns>
		int File::GetBomCount(IN byte* buffer)
		{
			Encoding encode = Convert::BomToEncode(buffer);
			return Convert::EncodeToBom(encode,NULL);
		}
		/// <summary>
		/// 获取文件内容头部的BOM编码格式
		/// </summary>
		/// <param name="buffer">文件内容</param>
		Encoding File::GetBOM(IN byte* buffer)
		{
			return Convert::BomToEncode(buffer);;
		}
#endif
		//////////////////////////////////////////////////////////////////////////
		/// private method
		//////////////////////////////////////////////////////////////////////////
		size_t File::InnerReadBytes(IN String path,IN byte* buffer,IN unsigned szBuffer,IN bool isNewBuffer)
		{
			byte* pBuffer=buffer;
			if(path.IsEmptyOrWhiteSpace()) throw invalid_argument("path");
			if(pBuffer==NULL) throw invalid_argument("buffer");

			String errtext;
			ifstream file(path.c_str(),ios::in|ios::binary);
			if(!file)
			{
				errtext = Environment::ErrnoText();
				if(Environment::Errno()==EACCES) return -4;
				//assert("file is not exist" && false);
				return 0;
			}
			file.seekg(0,ios::end);
			ifstream::pos_type fsize = file.tellg();
			if(file.fail())
			{
				errtext = Environment::ErrnoText();
				assert("get size of file failed" && false);
				return -1;
			}
			file.seekg(0,ios::beg);
			if(szBuffer==0||szBuffer>(unsigned)fsize) szBuffer=(unsigned)fsize;
			if(isNewBuffer)//内部申请空间
			{
				pBuffer=new byte[szBuffer];
				if(pBuffer==NULL)
				{
					assert("alloc memory failed" && false);
					return -2;
				} 
				ZeroMemory(pBuffer,szBuffer);
				*((byte**)buffer)=pBuffer;
			}
			file.read((char*)pBuffer,szBuffer);
			if(file.bad())
			{
				errtext = Environment::ErrnoText();
				assert("read file failed" && false);
				return -3;
			}
			return szBuffer;
		}		
	}
}


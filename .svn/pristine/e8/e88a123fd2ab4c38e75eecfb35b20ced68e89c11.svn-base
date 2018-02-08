#include "stdafx.h"
#if IS_WINDOWS_OS
#include <io.h>
#else
#include <unistd.h>
#endif
#include "Path.hpp"
#include "../Core/Char.hpp"

namespace System
{
	namespace IO
	{
#if IS_WINDOWS_OS
		const TCHAR Path::DirectorySeparatorChar=_T('\\');
		const TCHAR Path::AltDirectorySeparatorChar=_T('/');
#else
		const TCHAR Path::DirectorySeparatorChar=_T('/');
		const TCHAR Path::AltDirectorySeparatorChar=_T('\\');
#endif
		const TCHAR Path::VolumeSeparatorChar=_T(':');
		const TCHAR Path::ExtensionSeparatorChar=_T('.');

		/// <summary>
		/// 判断给定的文件或文件夹路径是否存在
		/// </summary>
		bool Path::Exists(IN String path)
		{
			if(String::IsEmptyOrWhiteSpace(path)) return FALSE;
#if IS_WINDOWS_OS
			return _taccess(path.c_str(),0)==0;
#else
			return access(path.c_str(),F_OK) == 0;
#endif
		}
		
		//C:\Users\Aweigh\Desktop\new.txt
		//\\192.168.1.100\VM-Share\files\2.txt
		bool Path::IsValid(IN LPCTSTR path,OUT int* pType)
		{
			int status = 0,type=-1;
			bool isValid = false;
			const TCHAR* p = path,* pExt=NULL,*pSeparator=NULL;			
			String invalidChars = GetInvalidPathChars();

			for (;path!=NULL && *p!=0;p++)
			{
				switch(status)
				{
				case 0://初始状态
					if(Char::IsWhiteSpace(*p)) continue;//跳过开头空格
					if(Char::IsLetter(*p))//字母开头
					{
						status = 1;//开始分析状态
						type = 0;//一般文件路径
						break;
					}
					if(*p==DirectorySeparatorChar)
					{
						status = 1;//开始分析状态
						type = 1;//共享文件路径
						break;
					}
					goto _BACK_;
				
				case 1://开始分析路径状态
					if(invalidChars.IndexOf(*p,0)>=0) goto _BACK_;//非法字符
					if(type==0)
					{
						if(*p!=VolumeSeparatorChar) goto _BACK_;
						if(*(p+1)!=0 && *(p+1)==DirectorySeparatorChar) p++;
						pSeparator = p;//设置分隔符位置
						status =2;//分析文件夹路径	
						break;
					}					
					if(type==1 && status==1)
					{
						if(*p!=DirectorySeparatorChar) goto _BACK_;
						pSeparator = p;//设置分隔符位置
						status =2;//分析文件夹路径
						break;
					}
					goto _BACK_;

				case 2://分析"文件夹\文件名称"部分
					if(*p==DirectorySeparatorChar)
					{
						if(p==pSeparator+1) goto _BACK_;//连续分隔符
						pSeparator = p;//记录分隔符位置
						break;
					}
					if(invalidChars.IndexOf(*p,0)>=0) goto _BACK_;//非法字符
					if(IsInvalidFileNameChar(*p)) goto _BACK_;//非法文件夹名称或文件名称

					if(type==0 && *p == ExtensionSeparatorChar) pExt = p;//指向拓展名起始位置(最后一个'.')
					break;
				}
				
			}
			if(status<2) goto _BACK_;
			if(type==0 && pExt!=NULL)//包含文件名的路径要进行文件名称合法性检测
			{
				if(*(pExt-1)==DirectorySeparatorChar) goto _BACK_;//空的文件名称
				for (;pExt>path && *pExt!=DirectorySeparatorChar;pExt--)
				{
					if(IsInvalidFileNameChar(*pExt)) goto _BACK_;
				}
			}
			isValid = true;
_BACK_:
			if(pType!=NULL) *pType = type;
			return isValid;
		}

		String Path::Combine(IN int count,...)
		{
			assert(count>0);
			
			String result,item;
			LPTSTR pArg=NULL;

			String::size_type ppos,spos;
			va_list ap;
			va_start(ap,count);
			for (int i=0;i<count;i++)
			{
				pArg = va_arg(ap,LPTSTR);
				if(i==0)
				{
					result=pArg;
					continue;
				}
				item=pArg;
				if(item.IsEmptyOrWhiteSpace()) continue;

				ppos=result.rfind(DirectorySeparatorChar);
				spos=item.find(DirectorySeparatorChar);
				if(spos==0) item.TrimStart(DirectorySeparatorChar);					
				if(ppos==String::npos||ppos!=result.length()-1)
					result.append(1,DirectorySeparatorChar);

				//result+=item;
				result.append(item);
			}
			va_end(ap);
			return result;
		}
		String Path::Combine(IN String path0,IN String path1)
		{
			assert(!String::IsEmptyOrWhiteSpace(path0));
			assert(!String::IsEmptyOrWhiteSpace(path1));
			return Combine(2,path0.c_str(),path1.c_str());
		}
		String Path::Combine(IN String path0,IN String path1,IN String path2)
		{
			assert(!String::IsEmptyOrWhiteSpace(path0));
			assert(!String::IsEmptyOrWhiteSpace(path1));
			assert(!String::IsEmptyOrWhiteSpace(path2));
			return Combine(3,path0.c_str(),path1.c_str(),path2.c_str());
		}

		String Path::GetDirectoryName(IN String& path,IN bool isEndDirSeparatorChar)
		{
			return GetDirectoryName((LPCTSTR)path,isEndDirSeparatorChar);
		}
		String Path::GetDirectoryName(IN LPCTSTR path,IN bool isEndDirSeparatorChar)
		{
			assert(path!=NULL && *path!=0);

			// Find last occurring '\\' or ':' in the string.
			// That is where the filename begins.
			LPCTSTR pWork = _tcsrchr(path,DirectorySeparatorChar);//查找最后一个'\\'
			if (pWork==NULL) pWork = _tcsrchr(path,VolumeSeparatorChar);//查找最后一个':'
			if(pWork==NULL) return String::Empty;//两个分隔符都找不到,返回空

			String result = path;
			int len = (int)(pWork-path);
			if(isEndDirSeparatorChar) len++;
			return result.Substring(0,len);
		}
		String Path::GetExtension(IN String& path)
		{
			assert(!HasIllegalCharacters(path));
			if(String::IsEmptyOrWhiteSpace(path)) return String::Empty;

			String::const_iterator ep=path.end();
			String::const_iterator sp=ep-1;
			for (;sp!=path.begin();sp--)
			{
				if(*sp==ExtensionSeparatorChar)
				{
					if (sp != ep) return String(sp,ep);
					return String::Empty;
				}
				if (((*sp == DirectorySeparatorChar) || (*sp == AltDirectorySeparatorChar)) || (*sp == VolumeSeparatorChar))
				{
					break;
				}
			}
			return String::Empty;
		}
		/*
		// GetFilename:
		// Returns a read-only character pointer into the path at the first character
		// of the base filename portion.  Note that this does not validate the
		// filename (or check if it is a file or directory).  For example, if the
		// string is "c:foo\bar.baz\baz.foo\blah.txt", then the returned pointer will
		// be pointing at the "blah.txt" portion.
		*/
		String Path::GetFileName(IN String& path)
		{
			assert("invalid parameter" && !HasIllegalCharacters(path));
			if(String::IsEmptyOrWhiteSpace(path)) return String::Empty;

			return GetFileName((LPCTSTR)path);
		}
		String Path::GetFileName(IN LPCTSTR path)
		{
			if(path==NULL||*path==0) return String::Empty;

			LPCTSTR p = path;
			// Find last occurring '\\' or ':' in the string.
			// That is where the filename begins.
			LPCTSTR pWork = _tcsrchr(p,DirectorySeparatorChar);
			if (pWork==NULL) pWork = _tcsrchr(p,VolumeSeparatorChar);
			if (pWork) pWork++;
			return pWork==NULL ? path : pWork;
		}
		String Path::GetRoot(IN String& path)
		{		
			assert(!HasIllegalCharacters(path));
			if(String::IsEmptyOrWhiteSpace(path)) return STRING_EMPTY;

			String result=String::Trim(path);
			String::size_type pos=path.find(VolumeSeparatorChar);
			if(pos!=String::npos)
			{
				result=path.substr(0,pos+1);
				result.append(1,DirectorySeparatorChar);
				return result;//+DirectorySeparatorChar;
			}
			if(Char::Compare(result[0],DirectorySeparatorChar,TRUE)) return _T("\\");
			return STRING_EMPTY;
		}
		String Path::GetFileNameWithoutExtension(IN String& path)
		{
			String _path = GetFileName(path);
			if (_path.IsEmptyOrWhiteSpace()){
				return STRING_EMPTY;
			}
			String::size_type pos=_path.rfind(ExtensionSeparatorChar);
			if(pos==String::npos) return _path;
			return _path.substr(0,pos);
		}
		bool Path::HasExtension(IN String& path)
		{
			assert(!HasIllegalCharacters(path));
			if(String::IsEmptyOrWhiteSpace(path)) return FALSE;
			
			String::const_reverse_iterator sp=path.rbegin();
			String::const_reverse_iterator ecp=sp;
			for (;sp!=path.rend();sp++)
			{
				if(*sp==ExtensionSeparatorChar) return sp!=ecp;
				if (((*sp == DirectorySeparatorChar) || (*sp == AltDirectorySeparatorChar)) || (*sp == VolumeSeparatorChar))
				{
					break;
				}
			}
			return FALSE;
		}
#if IS_WINDOWS_OS
		String Path::GetPart(IN String& path,IN BYTE parts)
		{
			assert(!String::IsEmptyOrWhiteSpace(path));
			assert(!HasIllegalCharacters(path));

			TCHAR drive[_MAX_DRIVE]={0};
			TCHAR dir[_MAX_DIR]={0};
			TCHAR fname[_MAX_FNAME]={0};
			TCHAR ext[_MAX_EXT]={0};

			_tsplitpath_s(path.c_str(), 
					drive,_MAX_DRIVE, dir,_MAX_DIR, 
					fname,_MAX_FNAME, ext,_MAX_EXT);
			
			String result=STRING_EMPTY;
			if(parts & TVolume){
				result=drive;
			}
			if(parts & TDirectory){
				String _dir=dir;
				_dir.Trim(SPACE_PATH_CHARS);
				if(!result.empty() && !_dir.IsEmptyOrWhiteSpace())
					result.append(1,DirectorySeparatorChar);//result+=_T("\\");

				result.append(_dir);//result+=_dir;
			}
			if(parts & TFileName){
				if(!result.empty() && fname[0]!=_T('\0'))
					result.append(1,DirectorySeparatorChar);//result+=_T("\\");
				result.append(fname);//result+=fname;
			}
			if(parts & TExtension){
				if(!result.empty() && ext[0]!=_T('\0'))
					result.append(1,DirectorySeparatorChar);//result+=_T("\\");
				result.append(ext);//result+=ext;
			}
			return result;
		}
		String Path::Replace(IN String& path,IN TPart part,IN String src)
		{
			TCHAR pathBuf[_MAX_PATH]={0};
			TCHAR drive[_MAX_DRIVE]={0};
			TCHAR dir[_MAX_DIR]={0};
			TCHAR fname[_MAX_FNAME]={0};
			TCHAR ext[_MAX_EXT]={0};

			_tsplitpath_s(path.c_str(), 
				drive,_MAX_DRIVE, dir,_MAX_DIR, 
				fname,_MAX_FNAME, ext,_MAX_EXT);
			
			String temp;
			src.Trim();
			switch(part)
			{
				case TVolume:
					_tcsncpy_s(drive,_MAX_DRIVE,src.c_str(),src.length());
					break;
				case TDirectory:
					{						
						if(!src.empty())
						{
							if(src.find(DirectorySeparatorChar)!=0)
								temp=_T("\\")+src;
							else
								temp=src;

							if(src.rfind(DirectorySeparatorChar)!=src.length()-1)
								temp+=_T("\\");
						}

						_tcsncpy_s(dir,_MAX_DIR,temp.c_str(),temp.length());
						break;
					}					
				case TFileName:
					_tcsncpy_s(fname,_MAX_FNAME,src.c_str(),src.length());
					break;
				case TExtension:
					_tcsncpy_s(ext,_MAX_EXT,src.c_str(),src.length());
					break;
				case TFileWithExt:
					{
						temp=Path::GetFileNameWithoutExtension(src);
						_tcsncpy_s(fname,_MAX_FNAME,temp.c_str(),temp.length());
						temp=Path::GetExtension(src);
						_tcsncpy_s(ext,_MAX_EXT,temp.c_str(),temp.length());
					}
					break;
				default:assert(FALSE);
					throw invalid_argument("invalid argument of 'part'");
					break;
			}
			
			_tmakepath_s(pathBuf,MAX_PATH,drive,dir,fname,ext);
			return pathBuf;
		}
#endif

		String& Path::GetInvalidPathChars()
		{
			static String invalids;
			if(invalids.IsEmpty())
			{
				for (int i=0;i<=31;i++){
					invalids.append(1,(TCHAR)i);
				}
				invalids.append(1,_T('"'));//34
				invalids.append(1,_T('<'));//60
				invalids.append(1,_T('>'));//62
				invalids.append(1,_T('|'));//124
			}			
			return invalids;
		}
		bool Path::HasIllegalCharacters(String& path)
		{
			String& invalids = GetInvalidPathChars();
			return path.find_first_of(invalids)!=String::npos;
		}
		
		bool Path::IsInvalidFileNameChar(IN TCHAR c)
		{
			static String invalids;
			if(invalids.IsEmpty())
			{
				invalids.append(1,_T('\\'));//92
				invalids.append(1,_T('/'));//47
				invalids.append(1,_T(':'));//58
				invalids.append(1,_T('*'));//42
				invalids.append(1,_T('?'));//63

				invalids.append(1,_T('"'));//34
				invalids.append(1,_T('<'));//60
				invalids.append(1,_T('>'));//62
				invalids.append(1,_T('|'));//124
			}	
			return invalids.IndexOf(c,0)>=0;
		}
	}	
}
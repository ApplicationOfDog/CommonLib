#include "stdafx.h"
#include "Directory.hpp"
#include "../Core/Environment.hpp"
#include "Path.hpp"
#include "File.hpp"
#include <sys/stat.h>
#include <errno.h>
#if IS_WINDOWS_OS
	#include <ShellAPI.h>
	#define S_ISREG(m) (((m) & 0170000) == (0100000))   
	#define S_ISDIR(m) (((m) & 0170000) == (0040000))
#elif IS_MAC_OS 
    #include <unistd.h>
	#include <sys/types.h>
	#include <dirent.h>
#endif


namespace System
{
	namespace IO
	{
		bool Directory::Exists(IN String path)
		{
			if(String::IsEmptyOrWhiteSpace(path)) return false;
			path.TrimEnd(_T("\\/"));

			struct _stat buffer;
			if(_tstat(path.c_str(),&buffer)!=0){
				//DWORD error = errno;
				return false;
			}
			return S_ISDIR(buffer.st_mode);;
		}

		bool Directory::Create(IN String path)
		{
			if(Exists(path)) return true;

#if IS_WINDOWS_OS
			SECURITY_ATTRIBUTES attrib;
			attrib.bInheritHandle = FALSE;
			//赋予空的DACL，允许所有用户访问该文件夹
			attrib.lpSecurityDescriptor = NULL;
			attrib.nLength =sizeof(SECURITY_ATTRIBUTES);
			//上面定义的属性可以省略。 直接return ::CreateDirectory( path, NULL); 即可
			return ::CreateDirectory( path.c_str(), &attrib)==TRUE;
#else
			return mkdir(path.c_str(),S_IREAD|S_IWRITE)==0;
#endif
			
		}
		bool Directory::CreateInLayers(IN String sDirPath)
		{
			if(Directory::Exists(sDirPath)) return true;
			if(sDirPath.empty())
			{
				Environment::LastError(ERROR_BAD_PATHNAME);
				return false;
			}			

			String::size_type pos = sDirPath.find(_T('\\'));
			if(pos==String::npos)
			{
				Environment::LastError(ERROR_BAD_PATHNAME);
				return false;
			}

			String subPath;
			while((pos=sDirPath.find(_T('\\'),pos+1))!=String::npos)
			{
				subPath=sDirPath.substr(0,pos);
				if(Directory::Exists(subPath)) continue;
				if(!Directory::Create(subPath)) return false;
			}

			if(Directory::Exists(sDirPath)) return true;
			return Directory::Create(sDirPath);

			//使用下面这个函数一样可以实现上面的功能
			//int ret = SHCreateDirectoryEx( NULL, path.c_str(), NULL );
		}
		bool Directory::Delete(IN String path)
		{
			if(!Exists(path)) return true;
			if(_trmdir(path.c_str())!=0){
				assert(false);
				return false;
			}
			return true;
		}
		bool Directory::Delete(IN String path,bool recursive)
		{
			assert(!path.IsEmptyOrWhiteSpace());

			path.TrimEnd(SPACE_PATH_CHARS);
			if(!Exists(path)) return true;//文件夹不存在,返回true

			if(recursive){//删除文件夹内容
				int visitLevel=-1;
				InnerSearch(path,InnerDeleteHandler,NULL,&visitLevel,HFFile|HFDirLast);
			}

			return Delete(path);//删除自身
		}
		bool Directory::DeleteContent(IN String path)
		{
			assert(!path.IsEmptyOrWhiteSpace());

			path.TrimEnd(SPACE_PATH_CHARS);
			if(!Exists(path)) return true;
			
			int visitLevel=-1;
			return InnerSearch(path,InnerDeleteHandler,NULL,&visitLevel,HFFile|HFDirLast);//删除文件夹内容
		}
		String Directory::GetCurrentDirectory()
		{
			String path;
			TCHAR lpBuffer[MAX_PATH+1]={0};
#if IS_WINDOWS_OS			
			if(::GetCurrentDirectory(MAX_PATH+1,lpBuffer)==0){//获取当前工作目录
				throw runtime_error("GetCurrentDirectory failed");
			}
			path = lpBuffer;
			if (path.IndexOf(_T('~'),TRUE) >= 0)
			{
				int num = ::GetLongPathName(path, lpBuffer, sizeof(lpBuffer)/sizeof(TCHAR));
				if ((num == 0) || (num >= MAX_PATH)){
					throw runtime_error("GetLongPathName failed");			
				}
				path = lpBuffer;
			}
#else
			getcwd(lpBuffer,MAX_PATH);//获取当前工作目录
			path = lpBuffer;
#endif			
			return path;
		}
#if IS_WINDOWS_OS		
		void Directory::SetCurrentDirectory(IN String path)
		{
			if(path.IsEmptyOrWhiteSpace()) throw invalid_argument("empty path");
			if (path.length() >= MAX_PATH) throw invalid_argument("path is too long");

			if (!::SetCurrentDirectory(path))
				throw runtime_error("SetCurrentDirectory failed");		
		}		
		
		DWORD Directory::SearchDisk(IN DiskHandler diskHandler,IN LPVOID lpParameter)
		{
			if(diskHandler==NULL){
				::SetLastError(ERROR_INVALID_FUNCTION);
				return ERROR_INVALID_FUNCTION;
			}
			TCHAR sysDrive[MAX_PATH]={0};
			::GetWindowsDirectory(sysDrive,MAX_PATH);

			TCHAR szBuf[MAX_PATH]={0};
			ZeroMemory(szBuf,sizeof(szBuf));
			String drivePath;
			DWORD len = ::GetLogicalDriveStrings(sizeof(szBuf)/sizeof(TCHAR),szBuf);  
			for (TCHAR* pDrive= szBuf;  *pDrive;  pDrive+=_tcslen(pDrive)+1){  
				UINT type = ::GetDriveType(pDrive);
				drivePath=pDrive;
				if(!diskHandler(drivePath,type,pDrive[0]==sysDrive[0],lpParameter)) break;
			}
			return ERROR_SUCCESS;
		}				
#endif
		bool Directory::GetFiles(IN String path,OUT vector<String>& result,IN SearchOption* searchOpts,IN BYTE backOpts)
		{
			if(!Directory::Exists(path))
			{
				Environment::LastError(ERROR_PATH_NOT_FOUND);
				return false;
			}
			int visitLevel=-1,position=0;
			SearchOption inner_options;
			if(searchOpts==NULL)
			{
				inner_options.maxVisitLevel = 0;//默认只访问最顶层
				searchOpts = &inner_options;
			}
			vector<PVOID> parameters;
			parameters.push_back(&result);
			parameters.push_back((PVOID)backOpts);
			searchOpts->lpParameter = &parameters;

			bool status = InnerSearch(path,InnerGetHandler,searchOpts,&visitLevel,HFFile|HFDirFirst);
			return status;
		}
		bool Directory::Copy(IN String src,IN String dest,IN int overwrite,IN SearchOption* options)
		{
			if(!Directory::Exists(src))
			{
				Environment::LastError(ERROR_PATH_NOT_FOUND);
				return false;
			}
			if(Directory::Exists(dest))
			{
				if(overwrite==1) Directory::DeleteContent(dest);
				else if(overwrite!=0)
				{
					Environment::LastError(ERROR_FILE_EXISTS);
					return false;
				}
			}
			else if(!Directory::CreateInLayers(dest))
			{
				return false;
			}

			LPCTSTR params[3]={
				src.c_str(),
				dest.c_str(),
				(LPCTSTR)((void*)overwrite)
			};
			SearchOption innerOption;
			if(options==NULL) options=&innerOption;
			options->lpParameter=params;

			Directory::Search(src,Directory::InnerCopyFileHandler,options);
			Environment::LastError(ERROR_SUCCESS);
			return true;
		}
        bool Directory::Search(IN String dir,IN FileHandler fileHandler,IN SearchOption* options)
		{
			int currentVisitLevel=-1;
			int flag = HFFile|HFDirFirst;//启用文件遍历回调和文件夹先序回调
			return Directory::InnerSearch(dir,fileHandler,options,&currentVisitLevel,flag);
		}
		////////////////////////////////////////////////////////////////////////
		////inner process
		bool Directory::InnerSearch(IN String dir,IN FileHandler fileHandler,IN SearchOption* options,IN int* pCurrentVisitLevel,IN int flag)
		{
			if(fileHandler==NULL) return false;			

			if(String::IsEmptyOrWhiteSpace(dir))
			{
				Environment::LastError(ERROR_BAD_PATHNAME);
				return false;
			}

			*pCurrentVisitLevel=*pCurrentVisitLevel+1;
			if(options!=NULL && options->maxVisitLevel>=0){				
				if(*pCurrentVisitLevel>options->maxVisitLevel){
					*pCurrentVisitLevel=*pCurrentVisitLevel-1;
					return true;
				}
			}
#if IS_WINDOWS_OS
			WIN32_FIND_DATA wfdata;
			String sdir=Path::Combine(dir,_T("*"));
			HANDLE hfile=::FindFirstFile((LPCTSTR)sdir,&wfdata);
			if(hfile==INVALID_HANDLE_VALUE){
#else
			struct stat wfdata;
			struct dirent *hFile=NULL;
			DIR* hDir = opendir(dir);
			if(hDir!=NULL) hFile = readdir(hDir);			
			if(hDir==NULL||hFile==NULL){
#endif
			
				if(options==NULL){
					*pCurrentVisitLevel=*pCurrentVisitLevel-1;
					return true;
				}
				options->failedCount++;
				return IS_BIT_0(options->flag,3);//是否遇到访问失败就立即返回，不再遍历文件树	
			}

			bool result = true,attrOper=false;
			String fileName,filePath;
			DSearchOption status= SearchOption::TContinue;
			bool isFilterNamesEmpty=true;
			LPVOID lpParameter=NULL;
			BYTE fileType= 0x3;
			DWORD filterMaxFileSize=0,dwFileAttributes=0,nFileSize=0;
			if(options!=NULL)
			{
				isFilterNamesEmpty = options->filterNames.IsEmptyOrWhiteSpace()||options->filterNames==_T("*");
				lpParameter = options->lpParameter;
				if((options->flag & 0x3) >0) fileType=options->flag&0x3;
				filterMaxFileSize=options->filterMaxFileSize;
			}
			
			do 
			{
#if IS_WINDOWS_OS
				fileName=wfdata.cFileName;
				filePath=Path::Combine(dir,fileName);
				dwFileAttributes = wfdata.dwFileAttributes;
				nFileSize = wfdata.nFileSizeLow;
#else
				fileName = hFile->d_name;
				filePath=Path::Combine(dir,fileName);
				if(stat(filePath.c_str(), &wfdata)!=0) continue;
				dwFileAttributes = (DWORD)wfdata.st_mode;
				nFileSize = wfdata.st_size;
#endif
				if(options!=NULL)
				{
					if(IS_FLAG_ON(options->flag,SEARCH_FILTERATTR_AND))//and操作:同时满足所有条件才是过滤对象
						attrOper=FLAG_ON(dwFileAttributes,options->filterAttrs)==options->filterAttrs;
					else //or操作:只要满足一个条件就是过滤对象
						attrOper=IS_FLAG_ON(dwFileAttributes,options->filterAttrs);

					//filters,多条件的,例如FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_READONLY
					if(options->filterAttrs>0 && attrOper) continue;
					if(!isFilterNamesEmpty)//是否有设置过滤列表
					{
						bool isMatch = String::MultiMatch(fileName,options->filterNames,0,true);
						if(IS_FLAG_ON(options->flag,SEARCH_IS_INCLUDE_FILTER))//如果是包含过滤
						{
							if(!isMatch) continue;//不匹配文件直接跳过
						}
						else//如果是排除过滤
						{
							if(isMatch) continue;//如果是匹配的则直接跳过
						}
					}				
				}

				//directory 
#if IS_WINDOWS_OS
				if (FLAG_ON(wfdata.dwFileAttributes,FILE_ATTRIBUTE_DIRECTORY))
#else
				if ( S_ISDIR(dwFileAttributes))
#endif
				{
					if(!FLAG_ON(fileType,SEARCH_DIRECTORY)) continue;//不遍历文件夹
					if(!IS_FLAG_ON(flag,HFDirFirst) && !IS_FLAG_ON(flag,HFDirLast) ) continue;//不启用任何文件夹访问回调
					if(_tcscmp(fileName.c_str(),_T("."))==0) continue;//不是有效文件夹
					if(_tcscmp(fileName.c_str(),_T(".."))==0) continue;//不是有效文件夹
					
					//先序遍历中访问文件夹
					if(IS_FLAG_ON(flag,HFDirFirst)){
						status = fileHandler(filePath,true,lpParameter,*pCurrentVisitLevel,&wfdata);
						if(status!=SearchOption::TContinue) goto _STATUS_;
					}					
					//递归访问文件夹内部
					if(!InnerSearch(filePath,fileHandler,options,pCurrentVisitLevel)){
						result=false;
						goto _BACK_;
					}
					//后序遍历中访问文件夹
					if(IS_FLAG_ON(flag,HFDirLast)){
						status = fileHandler(filePath,true,lpParameter,*pCurrentVisitLevel,&wfdata);
						if(status!=SearchOption::TContinue) goto _STATUS_;
					}					
					continue;
				}

				//file
				if(!FLAG_ON(fileType,SEARCH_FILE)||!IS_FLAG_ON(flag,HFFile)) continue;//不遍历文件或不启用文件访问回调
				if(filterMaxFileSize>0 && nFileSize>filterMaxFileSize) continue;//过滤掉太大的文件
				//文件访问
				status = fileHandler(filePath,false,lpParameter,*pCurrentVisitLevel,&wfdata);
_STATUS_:
				if(status==SearchOption::TStopInDir) continue;//结束对该文件夹遍历即不遍历该文件夹内部
				if(status==SearchOption::TStopInLayer){
					result = true;//整个文件树遍历还是在继续，只是结束当前层的遍历
					goto _BACK_;
				}
				if(status==SearchOption::TExit){
					result = false;//停止整个文件树遍历
					goto _BACK_;
				}
#if IS_WINDOWS_OS
			} while (::FindNextFile(hfile,&wfdata)!=NULL);
#else
			} while ((hFile=readdir(hDir))!=NULL);
#endif
_BACK_:
#if IS_WINDOWS_OS
			::FindClose(hfile);
#else
			closedir(hDir);
#endif
			*pCurrentVisitLevel=*pCurrentVisitLevel-1;
			return result;
		}       
		DSearchOption Directory::InnerCopyFileHandler(IN String& path,IN bool isDir,IN PVOID lpParameter,IN int level,IN PVOID pFindData)
		{
			LPCTSTR* params=(LPCTSTR*)lpParameter;
			String src=params[0];
			String dest=params[1];
			bool isOverwrite = params[2] ==NULL ;//重写类型
            
			String tempPath=path;
			tempPath.Replace(src,dest,TRUE);
			if(!isDir)//file
			{ 
                File::Copy(path.c_str(),tempPath.c_str(),isOverwrite);
				return SearchOption::TContinue;
			}
			
			if(Directory::Exists(tempPath)) return SearchOption::TContinue;

			Directory::Create(tempPath);
			return SearchOption::TContinue;
		}
		DSearchOption Directory::InnerDeleteHandler(IN String& path,IN bool isDir,IN PVOID lpParameter,IN int level,IN PVOID pFindData)
		{
			if(!isDir) //文件
				File::Delete(path);
			else //文件夹,必须是后序遍历才行
				Directory::Delete(path);

			return SearchOption::TContinue;
		}
		DSearchOption Directory::InnerGetHandler(IN String& path,IN bool isDir,IN PVOID lpParameter,IN int level,IN PVOID pFindData)
		{
			vector<PVOID>* parameters = (vector<PVOID>*)lpParameter;
			vector<String>* pResult = (vector<String>*)parameters->at(0);
			BYTE backOpts = (BYTE)parameters->at(1);

			if(!isDir && !FLAG_ON(backOpts,BACKOPT_FILE)) goto _BACK_;
			if(isDir && !FLAG_ON(backOpts,BACKOPT_DIRECTORY)) goto _BACK_;

			if(!FLAG_ON(backOpts,BACKOPT_FULL_PATH))
			{
				String fileName = Path::GetFileName(path);
				pResult->push_back(fileName);
			}
			else
				pResult->push_back(path);
		_BACK_:
			return SearchOption::TContinue;
		}
	}
}


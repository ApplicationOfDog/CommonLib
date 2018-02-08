#include "stdafx.h"
#if IS_WINDOWS_OS
	#include <ShlObj.h>
#elif IS_MAC_OS
	#include <mach-o/dyld.h>
	#include <unistd.h>
#endif
#include <errno.h>
#include "Environment.hpp"
#include "../IO/Directory.hpp"

namespace System
{
	LPCTSTR Environment::NewLine=_T("\r\n");
	DWORD Environment::LastErrorCode = 0;

#if IS_WINDOWS_OS

	WinOSType Environment::GetOsVersion()
	{
		WinOSType osType = WOS_None;
		DWORD dwMajorVer=0,dwMinorVer=0,dwBuildNumber=0;
		HMODULE hModNtdll = ::LoadLibrary(_T("ntdll.dll"));
		if(hModNtdll!=NULL)
		{
			typedef void(WINAPI* pfRTLGETNTVERSIONNUMBERS)(DWORD*,DWORD*,DWORD*);
			pfRTLGETNTVERSIONNUMBERS pfRtlGetNtVersionNumbers;
			pfRtlGetNtVersionNumbers = (pfRTLGETNTVERSIONNUMBERS)::GetProcAddress(hModNtdll, "RtlGetNtVersionNumbers");
			if (pfRtlGetNtVersionNumbers)
			{

				pfRtlGetNtVersionNumbers(&dwMajorVer, &dwMinorVer,&dwBuildNumber);
				dwBuildNumber&= 0x0ffff;

				if(dwMajorVer==4L) osType=WOS_WinNT4;   
				else if(dwMajorVer==5 && dwMinorVer==0) osType=WOS_Win2000;//5.0
				else if(dwMajorVer==5 && dwMinorVer==1) osType=WOS_WinXP;//5.1
				else if(dwMajorVer==5 && dwMinorVer==2) osType=WOS_Win2003;//5.2
				else if(dwMajorVer==6 && dwMinorVer==0) osType=WOS_VISTA;//6.0
				else if(dwMajorVer==6 && dwMinorVer==1) osType=WOS_WINDOWS7;//6.1
				else if(dwMajorVer==6 && dwMinorVer==2) osType=WOS_WINDOWS8;//6.2
				else if(dwMajorVer==6 && dwMinorVer==3) osType=WOS_WINDOWS81;//6.3
				else if(dwMajorVer==10 && dwMinorVer==0) osType = WOS_WINDOWS10;
				else osType = WOS_None;
			}
			::FreeLibrary(hModNtdll);
			hModNtdll=NULL;
		}
		else
		{
			DWORD winVer =::GetVersion();//该函数从win8之后都返回6.2,微软已经宣称放弃这个API了
			dwMajorVer = (DWORD)(LOBYTE(LOWORD(winVer)));
			dwMinorVer = (DWORD)(HIBYTE(LOWORD(winVer)));

			if(winVer < 0x80000000)//NT
			{   
				if(dwMajorVer==4L) osType=WOS_WinNT4;   
				else if(dwMajorVer==5 && dwMinorVer==0) osType=WOS_Win2000;//5.0
				else if(dwMajorVer==5 && dwMinorVer==1) osType=WOS_WinXP;//5.1
				else if(dwMajorVer==5 && dwMinorVer==2) osType=WOS_Win2003;//5.2
				else if(dwMajorVer==6 && dwMinorVer==0) osType=WOS_VISTA;//6.0
				else if(dwMajorVer==6 && dwMinorVer==1) osType=WOS_WINDOWS7;//6.1
				else if(dwMajorVer==6 && dwMinorVer==2) osType=WOS_WINDOWS8;//6.2
				else osType = WOS_None;
			}   
			else if(dwMajorVer<4)
				osType=WOS_Win32s;   
			else
			{   
				osType=WOS_Win95;
				OSVERSIONINFO* osvi=(OSVERSIONINFO*)malloc(sizeof(OSVERSIONINFO));   
				if(osvi!=NULL)
				{   
					memset(osvi,0,sizeof(OSVERSIONINFO));   
					osvi->dwOSVersionInfoSize=sizeof(OSVERSIONINFO);   
					GetVersionEx(osvi);   
					if(osvi->dwMajorVersion==4 && osvi->dwMinorVersion==10) osType=WOS_Win98;   
					else if(osvi->dwMajorVersion==4 && osvi->dwMinorVersion==90) osType=WOS_WinME;
					free(osvi);   
				}   
			}
		}
		return   osType;
	}
	bool Environment::GetOsArchitecture(OUT bool* is64Bit)
	{
		if(is64Bit==NULL) return false;

		*is64Bit = false;
		SYSTEM_INFO si; 
		typedef VOID(__stdcall*GETNATIVESYSTEMINFO)(LPSYSTEM_INFO lpSystemInfo);

		HMODULE hModKernel32dll = ::GetModuleHandle(_T("kernel32.dll"));
		if(hModKernel32dll!=NULL)
		{
			GETNATIVESYSTEMINFO fnGetNativeSystemInfo=(GETNATIVESYSTEMINFO)GetProcAddress(hModKernel32dll,"GetNativeSystemInfo");
			if (fnGetNativeSystemInfo!=NULL)
			{
				fnGetNativeSystemInfo(&si);
				if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
					si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ) 
				{
					*is64Bit = true;
				}
				return true;
			}
		}
		return false;
	}
	String Environment::GetEnvironmentVariable(IN LPCTSTR variable)
	{
		assert(variable!=NULL && *variable!=0);
		TCHAR pBuffer[BUFFERSIZE]={0};
		int szRet=::GetEnvironmentVariable(variable,pBuffer,BUFFERSIZE);
		if(szRet==0 && GetLastError()==ERROR_ENVVAR_NOT_FOUND) return STRING_EMPTY;
		return pBuffer;
	}
	bool Environment::SetEnvironmentVariable(IN LPCTSTR variable, IN LPCTSTR value)
	{
		assert(CheckEnvironmentVariableName(variable));
		return ::SetEnvironmentVariable(variable,value)==TRUE;
	}
	String Environment::GetFolderPath(SpecialFolder folder,SpecialFolderOption option)
	{
		assert(folder>=SF_Desktop && folder<=SF_CDBurning);

		String str;
		TCHAR lpszPath[MAX_PATH]={0};
		int num = SHGetFolderPath(NULL, (int) (folder | ((SpecialFolder)((int)option))),NULL, 0, lpszPath);
		if (num < 0)
		{
			switch (num)
			{
			case -2146233031:
				throw exception("PlatformNotSupportedException");
			}
			str = STRING_EMPTY;
		}
		else
		{
			str = lpszPath;
		}
		return str;
	}
	String Environment::GetSystemFolder(IN bool isWindowsDir)
	{
		TCHAR lpszPath[MAX_PATH]={0};
		if(isWindowsDir)
			::GetWindowsDirectory(lpszPath,MAX_PATH);
		else
			::GetSystemDirectory(lpszPath,MAX_PATH);
		return lpszPath;
	}
	vector<String> Environment::GetLogicalDrives()
	{
		int logicalDrives = ::GetLogicalDrives();
		if (logicalDrives == 0){			
			throw exception("not logical drives:__Error.WinIOError();");
		}
		unsigned num2 = (unsigned) logicalDrives;
		int num3 = 0;
		while (num2 != 0)
		{
			if ((num2 & 1) != 0)
			{
				num3++;
			}
			num2 = num2 >> 1;
		}
		String item;
		vector<String> strArray(num3);
		TCHAR chArray[] = { _T('A'), _T(':'), _T('\\') ,0};
		num2 = (unsigned) logicalDrives;
		num3 = 0;
		while (num2 != 0)
		{
			if ((num2 & 1) != 0)
			{
				item=chArray;
				strArray[num3++]=item;
			}
			num2 = num2 >> 1;
			chArray[0] = (TCHAR) (chArray[0] + _T('\x0001'));
		}
		return strArray;
	}


#if _MSC_VER >= 1300    // for VC 7.0
	// from ATL 7.0 sources
	#ifndef _delayimp_h
		extern "C" IMAGE_DOS_HEADER __ImageBase;
	#endif
#endif
	HMODULE Environment::GetCurrentModule(bool bProcess)
	{
		if(bProcess) return ::GetModuleHandle(NULL);//返回当前进程句柄

		//返回PE句柄，如果是在dll中调用该函数则返回dll的句柄
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)

		// Here's a trick that will get you the handle of the module
		// you're running in without any a-priori knowledge:
		// http://www.dotnet247.com/247reference/msgs/13/65259.aspx

		MEMORY_BASIC_INFORMATION mbi;
		static int dummy;
		VirtualQuery( &dummy, &mbi, sizeof(mbi) );
		return reinterpret_cast<HMODULE>(mbi.AllocationBase);
#else    // VC 7.0
		// from ATL 7.0 sources
		return reinterpret_cast<HMODULE>(&__ImageBase);
#endif
	}

#endif //IS_WINDOWS_OS

	String Environment::GetExecutablePath(bool bProcess)
	{
		TCHAR appFullPath[MAX_PATH+1]={0};
#if IS_WINDOWS_OS
		TCHAR appFileName[MAX_PATH+1]={0};
		HMODULE hModule=GetCurrentModule(bProcess);
		::GetModuleFileName(hModule,appFileName,sizeof(appFileName));
		::GetLongPathName(appFileName,appFullPath,MAX_PATH+1);
#elif IS_MAC_OS
		uint32_t len = sizeof(appFullPath);
		if(bProcess)//真正的可执行进程文件在xx.app中的/Contents/MacOS下
			_NSGetExecutablePath(appFullPath,&len);
		else{//获取.app包所在路径
			getcwd(appFullPath, len);
		}
#endif
		return appFullPath;
	}
	String Environment::LastErrorText(DWORD code,byte textMode)
	{
		if(code==-1) code=Environment::LastError();
		String result;

#if IS_WINDOWS_OS
		LPTSTR lpMsgBuf=NULL;
		::FormatMessage(
			  FORMAT_MESSAGE_ALLOCATE_BUFFER| //自动分配消息缓冲区
					FORMAT_MESSAGE_FROM_SYSTEM| //从系统获取信息
					FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK, 
			  NULL,
			  code, //获取错误信息标识
			  MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),//使用系统缺省语言
			  (LPTSTR)&lpMsgBuf, //消息缓冲区
			  0,NULL);
		
		if(IS_BIT_1(textMode,0)) result.AppendFormat(_T("0x%x-"),code);
		if(IS_BIT_1(textMode,1)) result.Append(lpMsgBuf);
		LocalFree(lpMsgBuf);
#else
		if(IS_BIT_1(textMode,0)) result.AppendFormat(_T("0x%x-"),code);
		if(IS_BIT_1(textMode,1)) result.Append(_tcserror((int)code));
#endif
		return result;
	}
	DWORD Environment::LastError()
	{
#if IS_WINDOWS_OS
		LastErrorCode = ::GetLastError();
#else
		LastErrorCode = errno;
#endif
		return LastErrorCode;
	}
	void Environment::LastError(IN DWORD val)
	{
		LastErrorCode = val;
#if IS_WINDOWS_OS		
		::SetLastError(LastErrorCode);
#endif
	}
	int Environment::Errno()
	{
		return errno;
	}
	String Environment::ErrnoText(IN int no,IN byte textMode)
	{
		if(no==-1) no = errno;

		String text;
		if(IS_BIT_1(textMode,0)) text.AppendFormat(_T("%d-"),no);
		if(IS_BIT_1(textMode,1)) text.Append(_tcserror(no));
		return text;
	}
	String Environment::GetLoginName()
	{
		String userName;
#if IS_WINDOWS_OS
		DWORD szBuffer = MAX_PATH;
		TCHAR buffer[MAX_PATH]={0};
		if (::GetUserName(buffer,&szBuffer))
			userName = buffer;
#else
		userName = getlogin();
#endif
		
		return userName;
	}
	ULONGLONG Environment::GetCurrentLUID()
	{
		ULONGLONG cluid=0;
#if IS_WINDOWS_OS		
		HANDLE hToken=INVALID_HANDLE_VALUE;
		if ( ::OpenProcessToken(::GetCurrentProcess(),TOKEN_QUERY,&hToken) )
		{
			TOKEN_STATISTICS stat;
			DWORD ReturnLength=0;
			::GetTokenInformation(hToken,TokenStatistics,&stat,sizeof(stat),&ReturnLength);
			memcpy(&cluid,&stat.AuthenticationId,sizeof(LUID));
			::CloseHandle(hToken);
		}
#else
		cluid = (ULONGLONG)getuid();
#endif
		return cluid;
	}
	String Environment::CurrentDirectory()
	{
		return IO::Directory::GetCurrentDirectory();
	}
#if IS_WINDOWS_OS	
	void Environment::CurrentDirectory(IN LPCTSTR path)
	{
		IO::Directory::SetCurrentDirectory(path);
	}
	
	bool Environment::IsSingleInstance(IN LPCTSTR instance)
	{
		//保证程序只有一个实例运行
		//使用自己创建的安全描述符创建事件量，使得该事件量可在不同的帐户里面全局可见
		SECURITY_ATTRIBUTES sa;
		SECURITY_DESCRIPTOR sd; 
		InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE);
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = &sd;

		HANDLE honlyone=::CreateMutex(&sa,FALSE,instance);
		return GetLastError()!=ERROR_ALREADY_EXISTS;
	}
#endif
	String Environment::GetProcessCommandLine()
	{
		String commandLineTemp = ::GetCommandLine();
		String programPath = Environment::GetExecutablePath(true);
		int index = commandLineTemp.IndexOf(programPath,0,true);
		if(index>=0)//如果包含exe路径则先删除exe路径
			commandLineTemp = commandLineTemp.Substring(index + programPath.GetLength());
		commandLineTemp.Trim(_T("\" \t\v\r\n\f\a\b"));//删除头尾的引号和空格
		return commandLineTemp;
	}
	std::map<String,String> Environment::AnayzeProgramCommandLine(IN LPCTSTR commandLine,IN LPCTSTR item_separator,IN LPCTSTR item_prefix,IN TCHAR keyVal_separator)
	{
		if(item_separator==NULL||*item_separator==0)
			throw invalid_argument("invalid argument 'item_separator'");

		String commandLineTemp;
		std::map<String,String> params;

		if(commandLine!=NULL)
			commandLineTemp = commandLine;
		else
			commandLineTemp = GetProcessCommandLine();

		int index = 0;
		std::map<String,String>::iterator pit;
		String arg,argName,argValue,prefixTemp = item_prefix;
		vector<String> arguments = commandLineTemp.Split(item_separator,SSO_RemoveEmptyEntries);
		for (vector<String>::iterator it = arguments.begin();it!=arguments.end();it++)
		{
			arg = String::Trim(*it);
			if(!prefixTemp.IsEmptyOrWhiteSpace())
			{
				index = arg.IndexOf(prefixTemp);
				if(index<0) continue;
				arg = arg.Substring(index);
			}
			index = arg.IndexOf(keyVal_separator,0);
			if(index<=0) continue;

			argName = arg.Substring(0,index);
			argValue = arg.Substring(index+1);

			if(!prefixTemp.IsEmpty())
			{
				index = argName.IndexOf(prefixTemp,false);
				if(index<0) continue;
				argName = argName.Substring(index+prefixTemp.GetLength());
			}			
			if(argName.IsEmptyOrWhiteSpace()) continue;

			pit = params.find(argName);
			if(pit==params.end())
			{
				params.insert(make_pair(argName,argValue));
				continue;
			}
			pit->second = argValue;
		}
		return params;
	}
	//////////////////////////////////////////////////////////////////////////
	///private method
#if IS_WINDOWS_OS	
	bool Environment::CheckEnvironmentVariableName(String variable)
	{
		if (variable.IsEmptyOrWhiteSpace())
		{
			throw exception("ArgumentNullException:variable");
			return FALSE;
		}
		if (variable.length() >= 0x7fff)
		{
			throw exception("ArgumentException:environment variable name is too long");
			return FALSE;
		}
		if (variable.IndexOf(_T('='),0) != -1)
		{
			throw exception("ArgumentException:illegal environment variable name");
			return FALSE;
		}
		return TRUE;
	}
#endif
}
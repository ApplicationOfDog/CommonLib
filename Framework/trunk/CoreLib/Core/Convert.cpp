#include "stdafx.h"
#include "Convert.hpp"
#include "Environment.hpp"

#ifndef _CONVERT_NO_IO_
#include "../IO/Path.hpp"
#endif

#include "Com.hpp"
#include "Char.hpp"
using namespace System::IO;

#if IS_WINDOWS_OS
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")
#include <strsafe.h>
#else
#include <stdlib.h>
#include <locale.h>
#endif

namespace System
{
	/// <summary>
	/// 对所有数制都通用（2进制、8进制、10进制、16进制）的字符串转数字的函数
	/// ps.从字符串开始位置开始转换直到一个非法字符（包括'\0'）结束。如果全部都是非法字符，返回0
	/// </summary>
	/// <param name="base">
	/// 数值格式，2:2进制，8：8进制...,目前支持2-9进制，和16进制。
	/// </param>
	/// <param name="pEnd">转换结束位置的字符下标,如果pEnd==0表示第一个字符就是非法字符</param>
	LONG64 Convert::ToInt64(IN LPCTSTR s,IN int base,OUT int* pEnd)
	{
		if(base<=1||(base>=11 && base<=15)||base>16)
			throw invalid_argument("base");

		LPCTSTR pc = s;
		LONG64 result = 0;
		if(pc==NULL||*pc==0)
		{
			if(pEnd!=NULL) *pEnd=0;
			return result;
		}

		if(base!=16)//非16进制
		{
			int isNeg=1;
			TCHAR max=_T('0')+base-1;
			if(base==10 && (*pc==_T('-')||*pc==_T('+')))
			{
				isNeg= *pc ==_T('-') ? -1 : 1;
				pc++;
			}

			for (;*pc>=_T('0') && *pc<=max; pc++)//value指向数字字符串
				result = base*result + *pc - _T('0');

			if(pEnd!=NULL) *pEnd=(int)(pc-s);
			return result*isNeg;
		}
		
		//转成16进制
		for (;*pc!=0;pc++)//value指向数字字符串
		{
			if(Char::IsWhiteSpace(*pc)) continue;
			if(Char::Compare(*pc,_T('x'),true)){//从'x'或'X'开始计算
				result = 0;
				continue;
			}
			if(!Char::IsHex(*pc)) break;
			result = base*result;
			if(Char::IsDigit(*pc))
				result+=*pc-_T('0');
			else if(*pc>=_T('a') && *pc<=_T('f'))
				result+=*pc-_T('a')+10;
			else
				result+=*pc-_T('A')+10;
		}
		if(pEnd!=NULL) *pEnd=(int)(pc-s);
		return result;
	}
	/// <summary>
	/// 对所有数制都通用（2进制、8进制、10进制、16进制）的字符串转数字的函数
	/// ps.从字符串开始位置开始转换直到一个非法字符（包括'\0'）结束。如果全部都是非法字符，返回0
	/// </summary>
	/// <param name="base">
	/// 数值格式，2:2进制，8：8进制...,目前支持2-9进制，和16进制。
	/// </param>
	/// <param name="pEnd">转换结束位置,如果pEnd==value表示第一个字符就是非法字符</param>
	LONG64 Convert::ToInt64(IN LPCSTR s,IN int base,OUT int* pEnd)
	{
		if(base<=1||(base>=11 && base<=15)||base>16)
			throw invalid_argument("base");

		LONG64 result = 0;
		const char* pc = s;
		if(pc==NULL||*pc==0)
		{
			if(pEnd!=NULL) *pEnd=0;
			return result;
		}
		if(base!=16)
		{
			int isNeg=1;
			char max='0'+base-1;
			if(base==10 && (*pc=='-'||*pc=='+')){
				isNeg=*pc=='-'?-1:1;
				pc++;
			}

			for (;*pc>='0' && *pc<=max; pc++)//pc指向数字字符串
				result = base*result + *pc - '0';

			if(pEnd!=NULL) *pEnd=(int)(pc-s);
			return result*isNeg;
		}
		//hex
		for (;*pc!=0;pc++)//pc指向数字字符串
		{
			if(!Char::IsWhiteSpace(*pc)) break;
			result = base*result;
			if(Char::IsDigit(*pc))
				result+=*pc-'0';
			else if(*pc>='a' && *pc<='f')
				result+=*pc-'a'+10;
			else
				result+=*pc-'A'+10;
		}
		if(pEnd!=NULL) *pEnd=(int)(pc-s);
		return result;
	}

	int Convert::ToInt(IN LPCTSTR s,IN int base,OUT int* pEnd)
	{
		int result = (int)ToInt64(s,base,pEnd);
		return result;
	}
	int Convert::ToInt(IN LPCSTR s,IN int base,OUT int* pEnd)
	{
		int result = (int)ToInt64(s,base,pEnd);
		return result;
	}
	int Convert::ToIntByDef(IN LPCTSTR s,OUT int def)
	{
		int end = 0;
		int result = ToInt(s,10,&end);
		return end>0 ? result : def;
	}
	int Convert::ToIntByDef(IN LPCSTR s,OUT int def)
	{
		int end = 0;
		int result = ToInt(s,10,&end);
		return end>0 ? result : def;
	}
	LONG64 Convert::ToInt64ByDef(IN LPCTSTR s,OUT LONG64 def)
	{
		int end = 0;
		LONG64 result = ToInt64(s,10,&end);
		return end>0 ? result : def;
	}
	LONG64 Convert::ToInt64ByDef(IN LPCSTR s,OUT LONG64 def)
	{
		int end = 0;
		LONG64 result = ToInt64(s,10,&end);
		return end>0 ? result : def;
	}

	double Convert::ToDouble(IN LPCTSTR val,IN double def)
	{
		if(String::IsNullOrWhiteSpace(val)) return def;
		return _tstof(val);
	}
	String Convert::ToString(int val)
	{
		return ToString((LONG64)val,10);
	}
	String Convert::ToString(IN LPCSTR str,IN Encoding code)
	{
		String result;
#ifdef _UNICODE
		string astr;
		if(code==EC_UTF8){
			astr = str;//如果目标字符串为utf8编码
			Utf8ToWideChar(astr,result);
		}
		else if(code==EC_UNICODE){
			result = (wchar_t*)str;
		}
		else{
			astr = str;//默认为EC_ANSI编码
			AnsiToWideChar(astr,result);
		}
#else
		if(code==EC_UTF8)//如果目标字符串为utf8编码
			Utf8ToAnsi(str,result);
		else if(code==EC_UNICODE)
			WideCharToAnsi((wchar_t*)str,result);
		else(code==EC_ANSI)//默认为EC_ANSI编码
			result.append(str);		 
#endif
		return result;
	}
	String Convert::ToString(IN std::string& str)
	{
		String result;
#ifdef _UNICODE
		AnsiToWideChar(str,result);
#else
		result.append(str);
#endif
		return result;
	}
	string Convert::ToStringA(IN LPCTSTR str,IN IO::Encoding code)
	{
		string result;
		if(str==NULL||*str==0) return result;

#ifdef _UNICODE
		wstring wstr = str;
		if(code == EC_UTF8)
			WideCharToUtf8(wstr,result);
		else
			WideCharToAnsi(wstr,result);
#else
		result.append(str);
#endif
		return result;
	}
	wstring Convert::ToStringW(IN LPCTSTR str,IN IO::Encoding code)
	{
		wstring result;
#ifdef _UNICODE
		result.append(str);		
#else
		string temp = str;
		if(code==EC_UTF8)
			Utf8ToWideChar(temp,result);
		else
			AnsiToWideChar(temp,result);
#endif
		return result;
	}

	/// <summary>
	/// 对所有数制都通用（2进制、8进制、10进制、16进制）的数字转字符串的函数
	/// </summary>
	/// <param name="base">数值格式，2:2进制，8：8进制...</param>
	String Convert::ToString(IN LONG64 val,IN int base)
	{
		TCHAR buf[25]={0};
		TCHAR* s = (TCHAR*)((byte*)buf + sizeof(buf));
		
		ULONG64 _val=val>=0?val:(val*-1);
		
		*--s = _T('\0');
		do
			*--s = _T("0123456789abcdef")[_val%base];
		while ((_val /= base) != 0);

		if(base==10 && val<0)  *--s = _T('-');

		return s;
	}
	string Convert::ToStringA(IN LONG64 val,IN int base)
	{
		char text[25]={0};
		char* s = (char*)((byte*)text + sizeof(text));

		ULONG64 _val=val>0?val:(val*-1);

		*--s = '\0';
		do
		*--s = "0123456789abcdef"[_val%base];
		while ((_val /= base) != 0);

		if(base==10 && val<0)  *--s = '-';

		return s;
	}
	
	String Convert::ToString(double value,int count)
	{
		assert(count>0);
		String format;
		format.FormatStr(_T("%%.%df"),count);
		return String::Format(format,value);
	}
	bool Convert::ToByteArray(IN LPCTSTR text,OUT BYTE* output,IN int szOutput)
	{
		assert(text!=NULL && output!=NULL);
		std::string atext = Convert::ToStringA(text);
		return ToByteArray(atext.c_str(),(int)atext.size(),output,szOutput);
	}
	bool Convert::ToByteArray(IN LPCSTR text,IN int len,OUT BYTE* output,IN int szOutput)
	{
		assert(text!=NULL && *text!=0 && output!=NULL);

		//翻译数据
		int nTarLen=0;
		bool bHigh,isHasInvliadChar=false;//当前处理的这个字符是对应目标的高4位么?
		BYTE temp=0;
		LPCSTR pSrc = text;
		BYTE* pDest = output;
		if(len<0) len = (int)strlen(text);

		bHigh=true;//一开始当然是处理高4位
		for (int i=0;i<len;i++)
		{
			Char c(pSrc[i]);
			if(szOutput>0 && nTarLen>=szOutput) break;//超出缓冲区了
			if(!c.IsHex()){
				if(!isHasInvliadChar) isHasInvliadChar= true;
				continue;
			}

			if (c.IsDigit())
				temp = (TCHAR)c - _T('0');
			else if (c.IsLower())
				temp = (TCHAR)c - _T('a') + 0xa;
			else if (c.IsUpper())
				temp = (TCHAR)c -_T('A') + 0xa;

			if (bHigh)
			{
				pDest[nTarLen]=temp<<4;
				pDest[nTarLen]=pDest[nTarLen] & 0xF0;//清掉低4位
				bHigh=false;//既然处理了高位,下面就该是低位了
			}
			else
			{
				pDest[nTarLen]=pDest[nTarLen] | temp;
				bHigh=true;//既然处理了低位,下面就是新的数字的高位了
				nTarLen++;
			}
		}
		return !isHasInvliadChar;
	}
	string Convert::ToTextFromArray(IN BYTE* arr,IN int len,IN char separator)
	{
		if(arr==NULL||len<=0) return "";
		
		string text;
		char item[4]={0},*p=NULL;
		for (int i = 0;i<len;i++)
		{
			p = item;
			ZeroMemory(item,sizeof(item));

			if(separator!=0) *p++ = separator;
			sprintf_s(p,sizeof(item)-(p-item),"%02x",arr[i],separator);
			text+=item;
		}
		return text;
	}
	int Convert::Utf8ToWideChar(IN const string& src,OUT wstring& dest) 
	{
		wchar_t* dest_buffer=NULL;
		int result = Utf8ToWideChar(src.c_str(),&dest_buffer);
		dest = dest_buffer;
		DELETE_STRING(dest_buffer);
		return result;
	}
	int Convert::Utf8ToWideChar(IN LPCSTR src,OUT wchar_t** dest) 
	{
#if IS_WINDOWS_OS
		int len = ::MultiByteToWideChar(CP_UTF8, 0,src, -1, NULL, 0)+1;
		if(len<=1) return -1;
		wchar_t* pWidechar = new wchar_t[len];
		ZeroMemory(pWidechar, sizeof(wchar_t)*len);
		::MultiByteToWideChar(CP_UTF8, 0, src, -1, pWidechar, len-1);
		*dest=pWidechar;
		return sizeof(wchar_t)*len;
#elif IS_MAC_OS
		setlocale(LC_CTYPE, "UTF-8");
		size_t dest_len = mbstowcs(NULL, src,0)+1;		
		wchar_t *dest_temp = new wchar_t[dest_len];
		ZeroMemory(dest_temp,dest_len*sizeof(wchar_t));
		mbstowcs(dest_temp, src, dest_len-1);
		*dest = dest_temp;
		return (int)dest_len*sizeof(wchar_t);
#endif
	}
	int Convert::WideCharToUtf8(IN const wstring& src,OUT string& dest)
	{
		char* dest_buffer = NULL;
		int result = WideCharToUtf8(src.c_str(),&dest_buffer);
		dest = dest_buffer;
		DELETE_STRING(dest_buffer);
		return result;
	}
	int Convert::WideCharToUtf8(IN LPCWSTR src,OUT char** dest)
	{
#if IS_WINDOWS_OS
		int dest_len = ::WideCharToMultiByte(CP_UTF8, 0, src, -1, NULL, 0, NULL, NULL)+1;
		if(dest_len<=1) return -1;
		char* pUTF8 = new char[dest_len];
		ZeroMemory(pUTF8,dest_len);
		::WideCharToMultiByte(CP_UTF8, 0, src, -1, pUTF8, dest_len-1, NULL, NULL);
		*dest = pUTF8;
		return dest_len;
#elif IS_MAC_OS
		setlocale(LC_CTYPE, "UTF-8");
		size_t src_len = wcslen(src);
		size_t bufferSize = (src_len+1)*4;
		char *dest_temp = new char[bufferSize];
		ZeroMemory(dest_temp,bufferSize);
		wcstombs(dest_temp, src, bufferSize);
		*dest = dest_temp;
		return (int)bufferSize;
#endif
	}

	#if IS_WINDOWS_OS	
	void Convert::Utf8ToWideChar(IN byte* pBuffer,int szBuf,OUT wstring& dest)
	{
		assert(pBuffer!=NULL && szBuf>0);
		int len = ::MultiByteToWideChar(CP_UTF8,0,(LPCSTR)pBuffer,szBuf,NULL, 0);
		if(len<=0) return;

		wchar_t* pWidechar = new wchar_t[len +1];
		ZeroMemory(pWidechar, sizeof(wchar_t)*(len+1));
		::MultiByteToWideChar(CP_UTF8, 0,(LPCSTR)pBuffer,szBuf, pWidechar, len);
		
		dest=pWidechar;
		delete[] pWidechar;
	}
	void Convert::Utf8ToAnsi(IN const string &src,OUT string &dest)
	{
		std::wstring tmp;
		Convert::Utf8ToWideChar(src, tmp);
		Convert::WideCharToAnsi(tmp,dest);
	}
	/*
	 * AnsiToUnicode converts the ANSI string pszA to a Unicode string
	 * and returns the Unicode string through ppszW. Space for the
	 * the converted string is allocated by AnsiToUnicode.
	 */ 
	void Convert::AnsiToWideChar(byte* buffer,int szBuf,OUT wstring& dest)
	{
		assert(buffer!=NULL);
		int len=::MultiByteToWideChar(CP_ACP,0,(LPCSTR)buffer,szBuf,NULL,0);
		if(len<=0) return;
		
		wchar_t* pWidechar = new wchar_t[len +1];
		ZeroMemory(pWidechar, sizeof(wchar_t)*(len+1));
		::MultiByteToWideChar(CP_ACP,0,(LPCSTR)buffer,szBuf,pWidechar,len);
		dest=pWidechar;
		delete[] pWidechar;
	}
	void Convert::AnsiToWideChar(IN const string& src,OUT wstring& dest)
	{
		//_bstr_t t = src.c_str();
		//wchar_t *pwchar = (wchar_t *) t;
		//dest = pwchar;
		AnsiToWideChar((byte*)src.c_str(),src.size(),dest);
	}
	void Convert::AnsiToUtf8(IN const std::string &src,OUT string &dest)
	{
		std::wstring wstr;
		Convert::AnsiToWideChar(src,wstr);
		Convert::WideCharToUtf8(wstr,dest);
	}
	int Convert::WideCharToAnsi(IN const wstring& src,OUT string& dest)
	{
		//_bstr_t t = src.c_str ();
		//char *pchar = (char *) t;
		//dest = pchar;

		int szDest = ::WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL)+1;
		if(szDest<=1) return -1;

		char* pDest = new char[szDest];
		if(pDest==NULL) return -2;

		ZeroMemory(pDest,szDest);
		::WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, pDest, szDest-1, NULL, NULL);
		dest = pDest;
		delete[] pDest;
		return szDest;
	}
#ifndef _CONVERT_NO_IO_
	String Convert::ToVolumeDevicePath(IN String path)
	{
		if(String::IsEmptyOrWhiteSpace(path)) return String::Empty;

		String result = path;
		String::size_type nPos = result.find(System::IO::Path::DirectorySeparatorChar);
		if (nPos==String::npos)
		{
			result.Replace(_T("\\\\"),_T("\\"),TRUE);

			if (Environment::GetOsVersion()>=WOS_VISTA)
				result=_T("\\Device\\Mup")+result;
			else
				result=_T("\\Device\\LanmanRedirector")+result;

			return result;
		}

		TCHAR ptemp[MAX_PATH]={0};
		String::size_type nSize=result.size();
		String strTemp=result.substr(0,nPos);
		if (::QueryDosDevice(strTemp.c_str(),ptemp,MAX_PATH)!=0)
		{
			String strTarget=ptemp;
			strTemp=result.substr(nPos);
			result=strTarget+strTemp;
		}
		return result;
	}
	String Convert::ToNtPath(IN String path)
	{
		const int BUFSIZE = 512;
		wchar_t lpszFilePath[_MAX_PATH] ={0};
		LPCTSTR lpszDevicePath = path; 
		// Translate path with device name to drive letters.
		TCHAR szTemp[BUFSIZE]={0};

		if (!GetLogicalDriveStrings(BUFSIZE-1, szTemp)) return String::Empty;

		TCHAR szName[_MAX_PATH]={0};
		TCHAR szDrive[3] = TEXT(" :");
		BOOL bFound = FALSE;
		TCHAR* p = szTemp;

		do 
		{
			// Copy the drive letter to the template string
			*szDrive = *p;

			// Look up each device name
			if (QueryDosDevice(szDrive, szName, MAX_PATH))
			{
				size_t uNameLen = _tcslen(szName);

				if (uNameLen < MAX_PATH) 
				{
					bFound = _tcsnicmp(lpszDevicePath, szName, uNameLen) == 0
						&& *(lpszDevicePath + uNameLen) == _T('\\');

					if (bFound) 
					{
						// Reconstruct pszFilename using szTempFile
						// Replace device path with DOS path
						TCHAR szTempFile[MAX_PATH];
						StringCchPrintf(szTempFile,
							MAX_PATH,
							TEXT("%s%s"),
							szDrive,
							lpszDevicePath+uNameLen);
						StringCchCopyN(lpszFilePath, MAX_PATH+1, szTempFile, _tcslen(szTempFile));
						goto _BACK_;
					}
				}
			}

			// Go to the next NULL character.
			while (*p++);
		} while (!bFound && *p); // end of string
_BACK_:
		return Convert::ToString((char*)lpszFilePath,EC_UNICODE);
	}

	/// <summary>
	/// 获取文件内容头部的BOM编码格式
	/// </summary>
	/// <param name="buffer">文件内容</param>
	Encoding Convert::BomToEncode(IN byte* buffer)
	{
		/*
		ANSI：没有BOM，直接是内容。
		UTF-8: EF BB BF
		UTF-16 little endian: FF FE
		UTF-16 big endian: FE FF
		UTF-32 little endian: FF FE 00 00
		UTF-32 big endian: 00 00 FE FF			
		*/
		byte* p=buffer;
		if(p[0]==0xEF && p[1]==0xBB && p[2]==0xBF) return EC_UTF8;
		if(p[0]==0xFF && p[1]==0xFE) return EC_UTF16LE;
		if(p[0]==0xFE && p[1]==0xFF) return EC_UTF16BE;
		if(p[0]==0xFF && p[1]==0xFE && p[2]==0x0 && p[3]==0x0) return EC_UTF32LE;
		if(p[0]==0x0 && p[1]==0x0 && p[2]==0xFE && p[2]==0xFF) return EC_UTF32BE;
		return EC_UNKNOWN;
	}
	/// <summary>
	/// 获取编码格式对应的BOM数据，返回BOM的字节数
	/// </summary>
	/// <param name="encode">编码</param>
	/// <param name="buffer">返回BOM数据,可以为null</param>
	/// <returns>BOM的字节数</returns>
	int Convert::EncodeToBom(IN IO::Encoding encode,OUT byte* buffer)
	{
		switch(encode)
		{
		case EC_UTF8://UTF-8: EF BB BF
			if(buffer!=NULL){
				buffer[0]=0xEE;
				buffer[1]=0xBB;
				buffer[2]=0xBF;
			}			
			return 3;
		case EC_UNICODE://unicode is same as EC_UTF16LE in window os
		case EC_UTF16LE://UTF-16 little endian: FF FE
			if(buffer!=NULL){
				buffer[0]=0xFF;
				buffer[1]=0xFE;
			}			
			return 2;
		case EC_UTF16BE://UTF-16 big endian: FE FF
			if(buffer!=NULL){
				buffer[0]=0xFE;
				buffer[1]=0xFF;
			}
			return 2;
		case EC_UTF32LE://UTF-32 little endian: FF FE 00 00
			if(buffer!=NULL){
				buffer[0]=0xFF;
				buffer[1]=0xFE;
				buffer[2]=0x00;
				buffer[3]=0x00;
			}
			return 4;
		case EC_UTF32BE://UTF-32 big endian: 00 00 FE FF	
			if(buffer!=NULL){
				buffer[0]=0x00;
				buffer[1]=0x00;
				buffer[2]=0xFE;
				buffer[3]=0xFF;
			}
			return 4;
		}
		return 0;
	}
#endif //_CONVERT_NO_IO_
#endif //IS_WINDOWS_OS
}
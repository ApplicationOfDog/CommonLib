#include "stdafx.h"
#ifndef _STRING_NO_STRINGHELPER_
#include "Helper/StringHelper.hpp"
#endif
#include "Char.hpp"
#ifndef _STRING_NO_CONVERT_
	#include "Convert.hpp"
#endif
#include "String.hpp"

namespace System
{
	String String::Empty = STRING_EMPTY;

#ifndef _STRING_NO_CONVERT_
	String::String(IN int num)
	{
		String text = Convert::ToString(num);
		assign(text);
	}
	String::String(IN LONG64 num,IN int base)
	{
		String text = Convert::ToString(num,base);
		assign(text);
	}
	String::String(IN LPCSTR s)
	{
		append(Convert::ToString(s));
	}
#endif
	TCHAR String::operator[](int i) 
	{
		return stdString::operator[](i);
	}
	String::operator LPCTSTR() const 
	{ 
		return c_str(); 
	}
	String::operator LPTSTR() const 
	{ 
		return (LPTSTR)c_str(); 
	}
#ifndef _STRING_NO_CONVERT_
	String& String::operator =(IN LONG64 num)
	{
		assign(Convert::ToString(num,10));
		return *this;
	}
#endif
	String& String::operator =(IN TCHAR c)
	{
		assign(1,c);
		return *this;
	}
#ifndef _STRING_NO_CONVERT_
	String& String::operator +=(IN LONG64 num)
	{
		String snum=Convert::ToString(num,10);
		append(snum);
		return *this;
	}
#endif
	String& String::operator +=(IN TCHAR c)
	{
		append(1,c);
		return *this;
	}
	String& String::operator +=(IN String& val)
	{
		append(val);
		return *this;
	}
	String& String::operator +=(IN LPCTSTR val)
	{
		append(val);
		return *this;
	}
#ifndef _STRING_NO_CONVERT_
	String String::operator +(IN LONG64 num)
	{
		String result=*this;
		String snum=Convert::ToString(num,10);
		result.append(snum);
		return result;
	}
#endif
	String String::operator +(TCHAR c)
	{
		String result=*this;
		result.append((size_type)1,c);
		return result;
	}
#if IS_WINDOWS_OS
	String& String::operator+=(const _Myt& val)
	{
		append(val);
		return *this;
	}
#endif
	bool String::operator ==(IN LPCSTR astr)
	{
		if(astr==NULL) return false;
		String bstr = Convert::ToString(astr,IO::EC_ANSI);
		return *this == bstr;
	}

	//////////////////////////////////////////////////////////////////////////
	bool String::IsEmptyOrWhiteSpace()
	{
		if(empty()) return true;

		const_iterator i = begin();
		while(i!=end() && Char::IsWhiteSpace(*i)) i++;
		return i==end();
	}
	bool String::IsNullOrWhiteSpace(IN LPCTSTR value)
	{
		if(value==NULL) return true;
		const TCHAR* p = value;
		while(*p!=0 && Char::IsWhiteSpace(*p)) p++;
		return *p==0;
	}
	bool String::IsEmpty(){
		return empty();
	}
	bool String::IsEmptyOrWhiteSpace(IN String& value)
	{
		return value.IsEmptyOrWhiteSpace();
	}
	bool String::IsEmpty(IN String& value){
		return value.empty();
	}

	void String::ToLower()
	{
		for (iterator i = begin(); i != end(); i++)
			if (*i >= _T('A') && *i <= _T('Z'))
				*i = (*i) + (_T('a') - _T('A'));
	}
	void String::ToUpper()
	{
		for (iterator i = begin(); i != end(); i++)
			if (*i >= _T('a') && *i <= _T('z'))
				*i = (*i) - (_T('a') - _T('A'));
	}
	
	vector<String> String::Split(IN TCHAR separator,IN StringSplitOptions options,IN size_type limit) const
	{
		String sitem;
		vector<String> v;
		const_iterator i = begin(),	last = i;
		for (; i != end(); i++)
		{
			if (*i == separator)
			{
				sitem.assign(last, i);
				if(options!=SSO_RemoveEmptyEntries|| !sitem.IsEmptyOrWhiteSpace())
					v.push_back(sitem);

				last = i + 1;
				if (v.size() >= limit - 1)
				{
					sitem.assign(last, end());
					if(options!=SSO_RemoveEmptyEntries|| !sitem.IsEmptyOrWhiteSpace())
						v.push_back(sitem);
					return v;
				}
			}
		}

		if (last != i){
			sitem.assign(last, i);
			if(options!=SSO_RemoveEmptyEntries|| !sitem.IsEmptyOrWhiteSpace())
				v.push_back(sitem);
		}
		return v;
	}
	vector<String> String::Split(IN LPCTSTR separator,IN StringSplitOptions options,IN size_type limit) const
	{
		String stemp,sitem;
		String separatorTemp = separator;

		vector<String> v;
		int separatorLen = separatorTemp.length();
		const_iterator i = begin(),last = i;
		for (; i != end(); i++)
		{
			if(end()-i<separatorLen)
			{//剩余的字符串长度不够就直接跳过
				i= end();
				break;
			}
			stemp.assign(i,i + separatorLen);				
			if (stemp != separatorTemp) continue;

			sitem.assign(last, i);
			if(options!=SSO_RemoveEmptyEntries|| !sitem.IsEmptyOrWhiteSpace())
				v.push_back(sitem);

			last = i + separatorLen;
			if (v.size() >= limit - 1) 
			{
				sitem.assign(last, end());
				if(options!=SSO_RemoveEmptyEntries|| !sitem.IsEmptyOrWhiteSpace())
					v.push_back(sitem);

				return v;
			}
			if(separatorLen>1)//直接跳过分隔断,因为for循环还会i++,所以这里是separatorLen-1
				i+=separatorLen-1;
		}

		if (last != i){
			sitem.assign(last,i);
			if(options!=SSO_RemoveEmptyEntries|| !sitem.IsEmptyOrWhiteSpace())
				v.push_back(sitem);
		}

		return v;
	}		
	vector<String> String::Split(size_type chunklen) const
	{
		vector<String> retval;
		retval.reserve(size() / chunklen + 1);

		size_type count = 0;
		const_iterator i = begin(),last = i;
		for (; i != end(); i++, count++)
		{
			if (count == chunklen)
			{
				count = 0;
				retval.push_back(String(last, i));
				last = i;
			}
		}
		
		if (last != i)
			retval.push_back(String(last, i));

		return retval;
	}

	int String::Compare(IN const String& strA,IN String strB,IN bool ignoreCase)
	{
		if(strA.empty()) return strB.empty()?0:-1;
		if(strB.empty()) return 1;

		const_iterator ap=strA.begin();
		const_iterator bp=strB.begin();
		while(ap!=strA.end() && bp!=strB.end() && Char::Compare(*ap,*bp,ignoreCase)){
			ap++;
			bp++;
		}
		if(ap==strA.end()) return bp==strB.end()?0:-1;
		if(bp==strB.end()) return 1;
		return *ap-*bp;
	}
	int String::Compare(IN const_iterator ap,IN size_type len,IN String& strB,IN bool ignoreCase)
	{
		assert(len!=String::npos && len>0);
		if(strB.empty()) return 1;

		const_iterator bp=strB.begin();
		while(len>0 && bp!=strB.end() && Char::Compare(*ap,*bp,ignoreCase))
		{
			len--;
			ap++;
			bp++;
		}
		if(len==0) return bp==strB.end()?0:-1;
		if(bp==strB.end()) return 1;
		return *ap-*bp;
	}
	int String::Compare(reverse_iterator ap,size_type len,String& strB, bool ignoreCase)
	{
		assert(len!=String::npos && len>0);
		if(strB.empty()) return 1;

		reverse_iterator bp=strB.rbegin();
		while(len>0 && bp!=strB.rend() && Char::Compare(*ap,*bp,ignoreCase))
		{
			len--;
			ap++;
			bp++;
		}
		if(len==0) return bp==strB.rend()?0:-1;
		if(bp==strB.rend()) return 1;
		return *ap-*bp;
	}

	int String::Compare(IN String str,IN bool ignoreCase)
	{
		return Compare(*this,str,ignoreCase);
	}
	bool String::IsUpper() const
	{
		if (length() == 0) return false;
		const_iterator i = begin();
		while(i != end() && Char::IsUpper(*i)) i++;
		return i == end();
	}
	bool String::IsLower() const
	{
		if (length() == 0) return false;
		const_iterator i = begin();
		while(i != end() && Char::IsLower(*i)) i++;
		return i == end();
	}
	bool String::IsAlnum() const
	{
		if (length() == 0) return false;

		for (const_iterator i = begin(); i != end(); i++)
		{
			//if (*i < 'A' || *i > 'Z')
			//	if (*i < '0' || *i > '9')
			//		if (*i < 'a' || *i > 'z')
			if(!Char::IsLetterOrDigit(*i))
				return false;
		}

		return true;
	}
	bool String::IsAlpha() const
	{
		if (length() == 0) return false;

		for (const_iterator i = begin(); i != end(); i++)
			if(!Char::IsLetter(*i))// (*i < 'A' || (*i > 'Z' && (*i < 'a' || *i > 'z')))
				return false;

		return true;
	}
	bool String::IsNumeric() const
	{
		if (length() == 0) return false;

		for (const_iterator i = begin(); i != end(); i++)
			if(!Char::IsDigit(*i))
				return false;

		return true;
	}

	String& String::Replace(IN String oldValue, IN String newValue)
	{
		size_type lastpos = 0,thispos;
		while ((thispos = find(oldValue, lastpos)) != npos)
		{
			String::replace(thispos, oldValue.length(), newValue);
			lastpos = thispos + newValue.length();
		}
		return *this;
	}
	String& String::Replace(IN String oldValue,IN String newValue,IN bool ignoreCase)
	{
		int index=0;
		while((index = IndexOf(oldValue,index,ignoreCase))>=0)
		{  
			String::replace(index,oldValue.length(),newValue);
			index += (int)newValue.length();
		}
		return *this;
	}
	String& String::Replace(IN TCHAR oldChar,IN TCHAR newChar)
	{
		for (iterator i = begin(); i != end(); i++)
			if (*i == oldChar)
				*i = newChar;
		return *this;
	}
	void String::FormatStr(IN LPCTSTR format, ...)  
	{
		if(format==NULL) return;

		va_list ap;
		va_start(ap,format);
		String result=String::Format(format,ap);
		va_end(ap);
		assign(result);
	}
	void String::FormatStr(IN LPCTSTR format,IN va_list args)  
	{
		if(format==NULL) return;
		String text = String::Format(format,args);
		assign(text);
	}
	void String::FormatStrA(IN LPCSTR format, ...)  
	{
		if(format==NULL) return;

		va_list ap;
		va_start(ap,format);
		string textA = String::FormatA(format,ap);
		String text = Convert::ToString(textA);
		va_end(ap);
		assign(text);
	}
	void String::FormatStrA(IN LPCSTR format,IN va_list args)  
	{
		if(format==NULL) return;
		string textA = String::FormatA(format,args);
		String text = Convert::ToString(textA);
		assign(text);
	}
	String String::Format(IN LPCTSTR format, ...)  
	{
		if(format==NULL) return Empty;

		va_list ap;
		va_start(ap,format);
		String text = String::Format(format,ap);
		va_end(ap);
		return text;
	}  
	String String::Format(IN LPCTSTR format,va_list args)
	{
		if(format==NULL||*format==0) return String::Empty;

		String result;
        int nBuffer= 0;
        TCHAR *pBuffer = NULL;
#if IS_WINDOWS_OS
        nBuffer=_vsctprintf(format,args)+1;
		pBuffer=new TCHAR[nBuffer];
		if(pBuffer==NULL) return result;
		int szBufferBytes=nBuffer*sizeof(*pBuffer);
		ZeroMemory(pBuffer,szBufferBytes);
		_vsntprintf_s(pBuffer,nBuffer,_TRUNCATE,format,args);
#else
        //for mac osx
        //int vprintf(const char * restrict format, va_list ap);
        //int vsprintf(char * restrict str, const char * restrict format, va_list ap);
        //int vsnprintf(char * restrict str, size_t size, const char * restrict format, va_list ap);
        nBuffer = vasprintf(&pBuffer,format,args)+1;//vasprintf函数返回格式化之后的字符数(不包括'\0')
		if(pBuffer==NULL) return result;
#endif       
		//for mac osx
        //int vsnprintf(char * restrict str, size_t size, const char * restrict format, va_list ap);
        //int vsprintf(char * restrict str, const char * restrict format, va_list ap);
        //int vswprintf(wchar_t *restrict ws, size_t n, const wchar_t *restrict format, va_list arg);
        //int vwprintf(const wchar_t *restrict format, va_list arg);
		result=pBuffer;
        
		if(pBuffer!=NULL){
			delete[] pBuffer;
			pBuffer=NULL;
		}
        
		return result;
	}

#ifdef _UNICODE
	String String::Format(IN LPCSTR format, ...)  
	{
		if(format==NULL) return String::Empty;

		va_list ap;
		va_start(ap,format);
		String result = String::Format(format,ap);
		va_end(ap);
		return result;
	}
	String String::Format(IN LPCSTR format,va_list args)
	{
		std::string a_result = String::FormatA(format,args);
		return Convert::ToString(a_result);
	}
	std::string String::FormatA(IN LPCSTR format, ...)  
	{
		if(format==NULL||*format==0) return "";

		va_list ap;
		va_start(ap,format);
		std::string result=String::FormatA(format,ap);
		va_end(ap);
		return result;
	}
	std::string String::FormatA(IN LPCSTR format,va_list args)
	{
		if(format==NULL||*format==0) return "";

		int nBuffer= 0;
		char* pBuffer = NULL;

#if IS_WINDOWS_OS
		nBuffer = _vscprintf(format,args)+1;
		pBuffer = new char[nBuffer];
		if(pBuffer==NULL) return "";

		int szBufferBytes = nBuffer*sizeof(*pBuffer);
		ZeroMemory(pBuffer,szBufferBytes);
		_vsnprintf_s(pBuffer,nBuffer,_TRUNCATE,format,args);
#endif 
		std::string a_result = pBuffer;
		DELETE_MEM(pBuffer);
		return a_result;
	}
#endif

	int String::IndexOf(IN TCHAR value,IN int startIndex)
	{
		if(startIndex>=(int)length()) return -1;
		if(startIndex<0) startIndex = 0;

		return (int)find(value,startIndex);
	}
	int String::IndexOf(IN LPCTSTR value,IN bool ignoreCase)
	{					
		return IndexOf(value,0,ignoreCase);
	}
	int String::IndexOf(IN LPCTSTR value,IN int startIndex,IN bool ignoreCase)
	{
		assert(value!=NULL && *value!=0);
		String valTemp = value;
		if(startIndex + valTemp.length()>length()) return -1;
		if(valTemp.empty()) return startIndex;

		//ignore case		
		int len=(int)valTemp.length();
		const_iterator ap=begin()+startIndex;
		const_iterator ep=end();
		for (;ap!=ep;ap++)
		{
			if(!Char::Compare(*ap,valTemp[0],ignoreCase)) continue;
			if(ep-ap<len) break;
			
			if(Compare(ap,len,valTemp,ignoreCase)!=0) continue;
			return (int)(ap-begin());
		}

		return -1;
	}
	
	int String::LastIndexOf(IN LPCTSTR value,IN bool ignoreCase)
	{
		assert(value!=NULL);
		if(*value == 0) return (int)length()-1;

		String valTemp = value;
		size_type len = valTemp.length();
		if(length()<len) return -1;

		Char lc(valTemp[(int)len-1]);
		reverse_iterator ap=rbegin();		
		for (;ap!=rend();ap++)
		{
			if(!Char::Compare(*ap,lc,ignoreCase)) continue;
			if(Compare(ap,len,valTemp,ignoreCase)==0)
			{
				return (int)(rend()-ap-len);
			}
		}
		
		return -1;
	}
	int String::LastIndexOf(IN TCHAR value)
	{
		return (int)rfind(value);
	}

	String& String::Trim()
	{
		 Trim(SPACE_CHARS);
		 return *this;
	}
	String String::Trim(IN String& src)
	{
		String result = src;
		result.erase(0,result.find_first_not_of(SPACE_CHARS));  
		result.erase(result.find_last_not_of(SPACE_CHARS)+1);
		return result;
	}
	String& String::RemoveUnprintableChars()
	{
		iterator ap = begin();
		while(ap!=end())
		{
			if(Char::IsPrintable(*ap))
			{
				ap++;
				continue;
			}
			ap = erase(ap);
		}
		return *this;
	}

	String& String::Trim(IN LPCTSTR trimChars)
	{
		if(trimChars==NULL) trimChars=SPACE_CHARS;
		this->erase(0,this->find_first_not_of(trimChars));  
		this->erase(this->find_last_not_of(trimChars)+1);
		return *this;
	}
	String String::Trim(IN String& src,IN LPCTSTR trimChars)
	{
		String result = src;
		result.Trim(trimChars);
		return result;
	}
#ifdef _UNICODE
	string String::TrimA(IN string& src,IN LPCSTR trimChars)
	{
		if(trimChars==NULL) trimChars=SPACE_CHARS_A;

		string result = src;
		result.erase(0,result.find_first_not_of(trimChars));  
		result.erase(result.find_last_not_of(trimChars)+1);
		return result;
	}
	string String::TrimA(IN LPCSTR src,IN LPCSTR trimChars)
	{
		if(src==NULL||*src==0) return "";

		string temp = src;
		return TrimA(temp,trimChars);
	}
#endif
	String& String::TrimEnd(IN LPCTSTR trimChars)
	{
		if(trimChars==NULL) trimChars=SPACE_CHARS;
		this->erase(this->find_last_not_of(trimChars)+1); 
		return *this;
	}
	String& String::TrimEnd(IN TCHAR c)
	{
		this->erase(this->find_last_not_of(c)+1); 
		return *this;
	}
	String String::TrimEnd(IN String& src,IN LPCTSTR trimChars)
	{
		String result = src;
		result.TrimEnd(trimChars);
		return result;
	}
	String String::TrimEnd(IN String& src,IN TCHAR c)
	{
		String result = src;
		result.TrimEnd(c);
		return result;
	}
	String& String::TrimStart(IN LPCTSTR trimChars)
	{
		if(trimChars==NULL) trimChars=SPACE_CHARS;
		this->erase(0,this->find_first_not_of(trimChars));
		return *this;
	}
	String& String::TrimStart(IN TCHAR c)
	{
		this->erase(0,this->find_first_not_of(c));
		return *this;
	}
	String String::TrimStart(IN String& src,IN LPCTSTR trimChars)
	{
		String result = src;
		result.TrimStart(trimChars);
		return result;
	}
	String String::TrimStart(IN String& src,IN TCHAR c)
	{
		String result = src;
		result.TrimStart(c);
		return result;
	}

	String String::Substring(int startIndex)
	{
		if(startIndex<0 || startIndex>(int)this->size())
			throw out_of_range("invalid parameter,overflow index");

		return substr(startIndex,String::npos);
	}
	String String::Substring(int startIndex, int length)
	{
		if(startIndex<0 || startIndex>(int)this->size())
			throw out_of_range("invalid parameter,overflow index");

		return substr(startIndex,length);
	}
	String String::Maxstring(IN int startIndex,IN int length,IN LPCTSTR suffix)
	{
		if(String::IsNullOrWhiteSpace(suffix))
			suffix = NULL;

		String tempStr;
		int startTemp = startIndex;

		if(startIndex>=(int)size()||length==0) return String::Empty;
		if(startTemp<0 && length<0) return *this;
		if(length>=(int)size()) return *this;
		if(length<0)
			return substr(startIndex,String::npos);

		if(startTemp==-1)
		{
			startIndex = (int)size() - length;
			if(suffix!=NULL && startIndex>0) 
				tempStr.append(suffix);
			tempStr.append(begin()+startIndex,end());
			return tempStr;
		}
		if(startTemp < -1)
		{
			int subfix_len = length/2,posfix_len = length - subfix_len;
			tempStr.append(begin(),begin()+subfix_len);
			tempStr.append(suffix);
			tempStr.append(end()-posfix_len,end());
			return tempStr;
		}
		
		tempStr = substr(startIndex,length);
		if(suffix!=NULL && startIndex+length<(int)size()) 
			tempStr.append(suffix);		
		return tempStr;
	}

#ifndef _STRING_NO_STRINGHELPER_
	bool String::Match(LPCTSTR source,LPCTSTR pattern, bool ignoreCase)
	{
		return StringHelper::Match(source,pattern,!ignoreCase);
	}
	bool String::MultiMatch(LPCTSTR source,LPCTSTR pattern, int nMatchLogic, bool ignoreCase)
	{
		return StringHelper::MultiMatch(source,pattern,nMatchLogic,false,!ignoreCase);
	}
#endif

	void String::AppendFormat(LPCTSTR format,...)
	{
		if(format==NULL) return;

		va_list ap;
		va_start(ap,format);
		String item = String::Format(format,ap);
		va_end(ap);
		append(item);
	}
#ifdef _UNICODE
	void String::AppendFormatA(LPCSTR format,...)
	{
		if(format==NULL) return;

		va_list ap;
		va_start(ap,format);
		String item = String::Format(format,ap);
		va_end(ap);
		append(item);
	}
#endif
#ifndef _STRING_NO_CONVERT_
	void String::Append(IN long value)
	{
		String item=Convert::ToString((int)value);
		append(item);
	}
	void String::Append(IN double value,IN int count)
	{
		String item=Convert::ToString(value,count);
		append(item);
	}
#endif
	void String::Append(IN LPCTSTR text)
	{
		if(text==NULL||*text==0) return;
		append(text);
	}
	void String::Append(IN TCHAR c,IN int count)
	{
		if(count<=0) throw invalid_argument("count");
		append(count,c);
	}
	void String::Append(IN LPCTSTR format,IN va_list args)
	{
		String item = String::Format(format,args);
		append(item);
	}
	void String::Append(IN LPCSTR text,IO::Encoding code)
	{
		if(text==NULL||*text==0) return;
		String item = Convert::ToString(text,code);
		append(item);
	}
	void String::Assign(IN TCHAR c,IN int count)
	{
		if(count<=0) throw invalid_argument("count");
		assign(count,c);
	}
	bool String::StartsWith(IN String value,IN bool ignoreCase)
	{
		assert(!value.empty());
		return IndexOf(value,0,ignoreCase)==0;
	}
	bool String::StartsWith(IN TCHAR value,IN bool ignoreCase)
	{
		if(length()<=0) return false;
		return Char::Compare(at(0),value,ignoreCase);
	}
	bool String::EndsWith(IN String value,IN bool ignoreCase)
	{
		assert(!value.empty());
		int index = LastIndexOf(value,ignoreCase);
		return index==length()-value.length();
	}
	bool String::EndsWith(IN TCHAR value,IN bool ignoreCase)
	{
		if(length()<=0) return false;
		return Char::Compare(at(length()-1),value,ignoreCase);
	}

	int String::GetBytes()
	{
		return ((int)length()+1)*sizeof(value_type);
	}
	int String::GetLength()
	{
		return (int)length();
	}
	LPCTSTR String::GetBuffer()
	{
		return c_str();
	}
#ifndef _STRING_NO_CONVERT_
	int String::ToInt(IN int def,IN int base)
	{
		String temp = String::Trim(*this);
		if(base<=0)//自动识别数值进制
		{
			if(temp.StartsWith(_T("0x"),true)){
				base = 16;
				temp.TrimStart(_T("0x"));
			}
			else if(temp.StartsWith(_T("0n"),true)){
				base = 10;
				temp.TrimStart(_T("0n"));
			}
			else if(temp.StartsWith(_T("0t"),true)){
				base = 8;
				temp.TrimStart(_T("0t"));
			}
			else if(temp.StartsWith(_T("0y"),true)){
				base = 2;
				temp.TrimStart(_T("0y"));
			}
			else
				base = 10;
		}

		int endIdx = -1;
		int result = Convert::ToInt(temp,base,&endIdx);
		if(endIdx<=0) return def;
		return result;
	}
#endif
	void String::Clear()
	{
		this->clear();
	}
}
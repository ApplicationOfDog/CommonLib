#include "stdafx.h"
#include "StringBuilder.hpp"
#include "Helper/StringBuilderHelper.hpp"
#define PDATA ((StringBuilderHelper*)_pData)

namespace System
{
	namespace Text
	{
		StringBuilder::StringBuilder()
		{
			_pData = new StringBuilderHelper();			
		}
		StringBuilder::StringBuilder(IN StringBuilder& builder)
		{
			_pData = new StringBuilderHelper((StringBuilderHelper*)builder._pData);
		}
		StringBuilder::StringBuilder(int capacity, int maxCapacity)
		{
			_pData = new StringBuilderHelper(capacity,maxCapacity);
		}
		StringBuilder::~StringBuilder()
		{
			DELETE_OBJECT(_pData);
		}
		int StringBuilder::Capacity()
		{
			return PDATA->m_Capacity;
		}
		int StringBuilder::Length()
		{
			return PDATA->m_ChunkLength;
		}
		int StringBuilder::MaxCapacity()
		{
			return PDATA->m_MaxCapacity;
		}
		void StringBuilder::NewLine(IN LPCTSTR chars)
		{
			PDATA->m_newLine = chars;
		}
		void StringBuilder::TabChar(IN TCHAR tab)
		{
			PDATA->m_tabChar = tab;
		}
		void StringBuilder::Append(IN const String& v)
		{
			PDATA->Append(v);
		}
		void StringBuilder::Append(int v)
		{
			String valStr;
			valStr.FormatStr(_T("%d"),v);
			PDATA->Append(valStr);
		}
		void StringBuilder::Append(IN UINT64 v)
		{
			String valStr;
			valStr.FormatStr(_T("%I64u"),v);
			PDATA->Append(valStr);
		}
		void StringBuilder::Append(LONG64 v,int base)
		{
			String valStr=Convert::ToString(v,base);
			PDATA->Append(valStr);
		}
		void StringBuilder::Append(double v,int count)
		{
			String valStr=Convert::ToString(v,count);
			PDATA->Append(valStr);
		}
		void StringBuilder::Append(IN TCHAR c,int count)
		{
			String str;
			str.Append(c,count);
			PDATA->Append(str);
		}
		void StringBuilder::Append(IN LPCTSTR format,IN va_list args)
		{
			PDATA->Append(String::Format(format,args));
		}
		void StringBuilder::AppendFormat(LPCTSTR format,...)
		{
			va_list ap;
			va_start(ap,format);
			String value=String::Format(format,ap);
			va_end(ap);

			PDATA->Append(value);
		}
		void StringBuilder::AppendLine()
		{
			PDATA->Append(PDATA->m_newLine);
		}
		void StringBuilder::AppendLine(IN LPCTSTR format,...)
		{
			va_list ap;
			va_start(ap,format);
			PDATA->Append(String::Format(format,ap));
			PDATA->Append(PDATA->m_newLine);
			va_end(ap);
		}
		void StringBuilder::Append(int nTab,IN LPCTSTR format,...)
		{
			String stab(nTab,PDATA->m_tabChar);
			PDATA->Append(stab);

			va_list ap;
			va_start(ap,format);
			PDATA->Append(String::Format(format,ap));
			va_end(ap);
		}
		void StringBuilder::AppendLine(int nTab,IN LPCTSTR format,...)
		{
			String stab(nTab,PDATA->m_tabChar);
			PDATA->Append(stab);

			va_list ap;
			va_start(ap,format);
			PDATA->Append(String::Format(format,ap));
			PDATA->Append(PDATA->m_newLine);
			va_end(ap);
		}
		void StringBuilder::TrimEnd(IN LPCTSTR trims)
		{
			PDATA->TrimEnd(trims);
		}
		void StringBuilder::TrimEnd(IN TCHAR c)
		{
			PDATA->TrimEnd(c);
		}
		void StringBuilder::Clear()
		{
			PDATA->Clear();
		}
		bool StringBuilder::IsEmpty()
		{
			return PDATA->m_ChunkLength<=0;
		}		
		void StringBuilder::Assign(IN StringBuilder& builder)
		{
			PDATA->Assign((StringBuilderHelper*)builder._pData);
		}
		String StringBuilder::ToString()
		{
			return PDATA->m_ChunkChars;
		}
		string StringBuilder::ToStringA(IN IO::Encoding code)
		{
			return Convert::ToStringA(PDATA->m_ChunkChars,code);
		}
	}	
}
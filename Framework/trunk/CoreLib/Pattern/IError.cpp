#include "stdafx.h"
#include "IError.hpp"

#ifndef _IERROR_NO_ENVIRONMENT_
#include "..\Core\Environment.hpp"
#endif

namespace System{
	namespace Pattern{

		IError::IError(){
			Clear();
		}
		IError& IError::operator =(IError& o)
		{
			Assign(o);	
			return *this;
		}

#ifndef _IERROR_NO_ENVIRONMENT_
		void IError::Assign()
		{
			m_nLastError = Environment::LastError();
			m_sLastError = Environment::LastErrorText();
		}
#endif
		void IError::Assign(IN IError& o)
		{
			m_nLastError = o.m_nLastError;
			m_sLastError = o.m_sLastError;
		}
		void IError::Assign(IN IError* p)
		{
			m_nLastError = p->m_nLastError;
			m_sLastError = p->m_sLastError;
		}

#ifndef _IERROR_NO_ENVIRONMENT_
		void IError::Assign(IN LPCTSTR format,...)
		{
			m_nLastError = Environment::LastError();
			if(format==NULL||*format==0)
			{
				m_sLastError = Environment::LastErrorText();
				return;
			}
			va_list ap;
			va_start(ap,format);
			m_sLastError = String::Format(format,ap);
			va_end(ap);
		}
#endif
		void IError::Clear()
		{
			m_nLastError=ERROR_SUCCESS;
			m_sLastError.clear();
		}
		void IError::Message(IN LPCTSTR format,...)
		{
			if(format==NULL) return;
			va_list ap;
			va_start(ap,format);
			m_sLastError = String::Format(format,ap);
			va_end(ap);
		}
		void IError::Set(IN DWORD code,IN LPCTSTR format,...)
		{
			m_nLastError = code;
			if(format==NULL)
			{
				m_sLastError.Clear();
				return;
			}
			va_list ap;
			va_start(ap,format);
			m_sLastError = String::Format(format,ap);
			va_end(ap);
		}
		void IError::Success(IN LPCTSTR format,...)
		{
			m_nLastError = ERROR_SUCCESS;
			if(format==NULL)
			{
				m_sLastError=_T("成功");
				return;
			}
			va_list ap;
			va_start(ap,format);
			m_sLastError = String::Format(format,ap);
			va_end(ap);
		}
		void IError::Error(IN LPCTSTR format,...)
		{
			m_nLastError = ERROR_ERROR;
			if(format==NULL)
			{
				m_sLastError=_T("发生错误");
				return;
			}
			va_list ap;
			va_start(ap,format);
			m_sLastError = String::Format(format,ap);
			va_end(ap);
		}

		void IError::Failed(IN LPCTSTR format,...)
		{
			m_nLastError = ERROR_FAIL;
			if(format==NULL)
			{
				m_sLastError=_T("执行失败");
				return;
			}
			va_list ap;
			va_start(ap,format);
			m_sLastError = String::Format(format,ap);
			va_end(ap);
		}

		void IError::Unkown(IN LPCTSTR format,...)
		{
			m_nLastError = ERROR_UNKOWN;
			if(format==NULL)
			{
				m_sLastError=_T("未知错误");
				return;
			}
			va_list ap;
			va_start(ap,format);
			m_sLastError = String::Format(format,ap);
			va_end(ap);
		}
		void IError::InvalidParameter(IN LPCTSTR name)
		{
			m_nLastError = ERROR_INVALID_PARAMETER;
			if(name!=NULL && *name!=0)
				m_sLastError.FormatStr(_T("invalid parameter '%s'"),name);
			else
				m_sLastError = _T("invalid parameter");
		}
		void IError::InvalidParameter(IN LPCSTR name)
		{
			m_nLastError = ERROR_INVALID_PARAMETER;
			if(name!=NULL && *name!=0)
			{
				String nameW = Convert::ToString(name,IO::EC_ANSI);
				m_sLastError.FormatStr(_T("invalid parameter '%s'"),(LPCTSTR)nameW);
				return;
			}
			m_sLastError = _T("invalid parameter");
		}
}}


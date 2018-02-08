#pragma once

//#define _IERROR_NO_ENVIRONMENT_

namespace System
{
	namespace Pattern
	{
		#define ERROR_UNKOWN (ERROR_SUCCESS-1)
		#define ERROR_ERROR (ERROR_SUCCESS-2)
		#define ERROR_FAIL  (ERROR_SUCCESS-3)

		/// <summary>
		/// 错误信息接口
		/// </summary>
		class API_CORE IError
		{
		public:
			DWORD m_nLastError;//错误码
			String m_sLastError;//错误信息    

		public:
			IError();
			IError& operator =(IError& o);

#ifndef _IERROR_NO_ENVIRONMENT_
			/// <summary>
			/// 对象赋值,使用::GetLastError对应数值和文本赋值当前对象
			/// </summary>
			void Assign();
#endif
			/// <summary>
			/// 对象之间赋值
			/// </summary>
			void Assign(IN IError& o);
			void Assign(IN IError* p);

#ifndef _IERROR_NO_ENVIRONMENT_
			void Assign(IN LPCTSTR format,...);
#endif
			void Clear();//清空错误信息
			virtual void Message(IN LPCTSTR format,...);
			virtual void Set(IN DWORD code,IN LPCTSTR format,...);//设置错误信息
			virtual void Success(IN LPCTSTR format,...);//设置成功信息
			virtual void Error(IN LPCTSTR format,...);
			virtual void Failed(IN LPCTSTR format,...);
			virtual void Unkown(IN LPCTSTR format,...);
			virtual void InvalidParameter(IN LPCTSTR name);
#ifdef _UNICODE
			virtual void InvalidParameter(IN LPCSTR name);
#endif
			bool IsSuccess(){ return m_nLastError == ERROR_SUCCESS; }	
		};
	}
}

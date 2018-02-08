#pragma once
#include <string>
#include <map>
using namespace std;

namespace System{
	namespace Diagnostics{

		///<summary>参数值类型</summary>
		enum VAR_TYPE
		{
			AVT_NULL,
			AVT_BOOL,
			AVT_INT32,
			AVT_INT64,
			AVT_UINT32,
			AVT_UINT64,
			AVT_FLOAT,
			AVT_STRING,
			AVT_POINTER,
		};

		///<summary>参数值对象</summary>
		class API_SYSTEM VAR_VALUE
		{
		protected:
			VAR_TYPE		m_type;//值类型
			UINT64			m_data;//值数据

		public:
			VAR_VALUE();
			VAR_VALUE(IN bool val);
			VAR_VALUE(IN INT32 val);
			VAR_VALUE(IN UINT32 val);
			VAR_VALUE(IN INT64 val);
			VAR_VALUE(IN UINT64 val);
			VAR_VALUE(IN double val);
			VAR_VALUE(IN LPCTSTR val);
			VAR_VALUE(IN PVOID val);
			VAR_VALUE(IN VAR_VALUE& val);
			virtual ~VAR_VALUE();

			VAR_VALUE& operator =(IN bool val);
			VAR_VALUE& operator =(IN INT32 val);
			VAR_VALUE& operator =(IN UINT32 val);
			VAR_VALUE& operator =(IN INT64 val);
			VAR_VALUE& operator =(IN UINT64 val);
			VAR_VALUE& operator =(IN double val);
			VAR_VALUE& operator =(IN LPCTSTR val);
			VAR_VALUE& operator =(IN PVOID val);
			VAR_VALUE& operator =(IN VAR_VALUE& val);

			void Assign(IN bool val);
			void Assign(IN INT32 val);
			void Assign(IN UINT32 val);
			void Assign(IN INT64 val);
			void Assign(IN UINT64 val);
			void Assign(IN double val);
			bool Assign(IN LPCTSTR val);
			bool Assign(IN PVOID val);
			bool Assign(IN VAR_VALUE& dest);
			bool Assign(IN VAR_VALUE* pDest);

			bool Is(IN VAR_TYPE type){ return m_type == type; }

			bool GetBool(){ assert(m_type == AVT_BOOL); return m_data == TRUE; }
			INT32 GetInt32(){ assert(m_type == AVT_INT32); return (INT32)m_data; }
			INT64 GetInt64(){ assert(m_type == AVT_INT64); return (INT64)m_data; }
			UINT32 GetUint32(){ assert(m_type == AVT_UINT32); return (UINT32)m_data; }
			UINT64 GetUint64(){ assert(m_type == AVT_UINT64); return (UINT64)m_data; }
			double GetFloat(){ assert(m_type == AVT_FLOAT); return (double)m_data; }
			LPCTSTR GetString(){ assert(m_type == AVT_STRING); return (LPCTSTR)m_data; }
			PVOID GetPointer(){ assert(m_type == AVT_POINTER); return (PVOID)m_data; }

		private:
			void do_free();
			void do_clear();
		};

		///<summary>任务参数</summary>
		class API_SYSTEM TaskArgs
		{
		private:
			map<string,VAR_VALUE> _datas;

		public:
			VAR_VALUE& operator[](IN LPCSTR key);
			void Assign(IN TaskArgs& dest);
			
			bool GetBool(IN LPCSTR key,IN bool def = false);
			INT32 GetInt32(IN LPCSTR key,IN INT32 def = 0);
			INT64 GetInt64(IN LPCSTR key,IN INT64 def = 0);
			UINT32 GetUint32(IN LPCSTR key,IN UINT32 def = 0);
			UINT64 GetUint64(IN LPCSTR key,IN UINT64 def = 0);
			double GetFloat(IN LPCSTR key,IN double def = 0.0);
			LPCTSTR GetString(IN LPCSTR key,IN LPCTSTR def = NULL);
			PVOID GetPointer(IN LPCSTR key,IN PVOID def = NULL);
		};

		///<summary>任务的回调函数,也是真正执行任务的函数</summary>
		///<param name="args">任务参数</param>
		///<param name="lpParameter">附加参数,一般是操作对象的this指针</param>
		///<result>异步线程退出码,即ExitCode</result>
		typedef DWORD (*TaskWorkingCallBack)(IN TaskArgs& args,IN PVOID lpParameter);

		/// <summary>
		/// 异步任务抽象类
		/// </summary>
		class API_SYSTEM AsynTask
		{
		private:
			TaskArgs		_parameters;//异步任务携带的参数

		public:
			AsynTask();
			AsynTask(IN TaskArgs& args);
			void Parameter(IN TaskArgs& args);
			void Parameter(IN LPCSTR key,IN bool val){ _parameters[key] = val; }
			void Parameter(IN LPCSTR key,IN INT32 val){ _parameters[key] = val; }
			void Parameter(IN LPCSTR key,IN INT64 val){ _parameters[key] = val; }
			void Parameter(IN LPCSTR key,IN UINT32 val){ _parameters[key] = val; }
			void Parameter(IN LPCSTR key,IN UINT64 val){ _parameters[key] = val; }
			void Parameter(IN LPCSTR key,IN double val){ _parameters[key] = val; }
			void Parameter(IN LPCSTR key,IN LPCTSTR val){ _parameters[key] = val; }
			void Parameter(IN LPCSTR key,IN PVOID val){ _parameters[key] = val; }

			///<summary>任务启动</summary>
			///<param name="args">任务参数</param>
			///<param name="pfnCallback">任务的回调函数,也是真正执行任务的函数</param>
			///<param name="lpParameter">附加参数,一般是操作对象的this指针</param>
			///<result>0表示成功,其他值表示错误码
			/// -1:pfnCallback为NULL错误；1:申请任务参数内存失败,2:创建线程失败
			///</result>
			int Start(IN TaskArgs& args,IN TaskWorkingCallBack pfnCallback,IN LPVOID lpParameter=NULL);
			int Start(IN TaskWorkingCallBack pfnCallback,IN LPVOID lpParameter=NULL);

		protected:
			static DWORD WINAPI OnWorkingProc(IN LPVOID lpParameter);
		};

}}//end namespace
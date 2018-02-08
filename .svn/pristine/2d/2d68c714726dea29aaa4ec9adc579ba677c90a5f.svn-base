#include "stdafx.h"
#include "AsynTask.hpp"

#define ARG_VALUE_CONSTRUCT(T) ArgVALUE::ArgVALUE(IN T val){ do_clear();Assign(val); }
#define ARG_VALUE_EQUAL_OPERATOR(T) ArgVALUE& ArgVALUE::operator =(IN T val){ do_clear();Assign(val); return *this; }

namespace System{
	namespace Diagnostics{	

		VAR_VALUE::VAR_VALUE(){ do_clear(); }
		VAR_VALUE::~VAR_VALUE(){ do_free(); }

		ARG_VALUE_CONSTRUCT(bool)
		ARG_VALUE_CONSTRUCT(INT32)
		ARG_VALUE_CONSTRUCT(UINT32)
		ARG_VALUE_CONSTRUCT(INT64)
		ARG_VALUE_CONSTRUCT(UINT64)
		ARG_VALUE_CONSTRUCT(double)
		ARG_VALUE_CONSTRUCT(LPCTSTR)
		ARG_VALUE_CONSTRUCT(PVOID)
		ARG_VALUE_CONSTRUCT(VAR_VALUE&)

		ARG_VALUE_EQUAL_OPERATOR(bool)
		ARG_VALUE_EQUAL_OPERATOR(INT32)
		ARG_VALUE_EQUAL_OPERATOR(UINT32)
		ARG_VALUE_EQUAL_OPERATOR(INT64)
		ARG_VALUE_EQUAL_OPERATOR(UINT64)
		ARG_VALUE_EQUAL_OPERATOR(double)
		ARG_VALUE_EQUAL_OPERATOR(LPCTSTR)
		ARG_VALUE_EQUAL_OPERATOR(PVOID)
		ARG_VALUE_EQUAL_OPERATOR(VAR_VALUE&)
		
		void VAR_VALUE::Assign(IN bool val)
		{
			do_free();
			m_type = AVT_BOOL;
			m_data = val?TRUE:FALSE;
		}
		void VAR_VALUE::Assign(IN INT32 val)
		{
			do_free();
			m_type = AVT_INT32;
			m_data = (UINT64)val;
		}
		void VAR_VALUE::Assign(IN UINT32 val)
		{
			do_free();
			m_type = AVT_UINT32;
			m_data = (UINT64)val;
		}
		void VAR_VALUE::Assign(IN INT64 val)
		{
			do_free();
			m_type = AVT_INT64;
			m_data = (UINT64)val;
		}
		void VAR_VALUE::Assign(IN UINT64 val)
		{
			do_free();
			m_type = AVT_UINT64;
			m_data = (UINT64)val;
		}
		void VAR_VALUE::Assign(IN double val)
		{
			do_free();
			m_type = AVT_FLOAT;
			memcpy(&m_data,&val,sizeof(val));
		}
		bool VAR_VALUE::Assign(IN LPCTSTR val)
		{
			do_free();
			m_type = AVT_STRING;
			if(val==NULL) return true;

			int len = _tclen(val)+1;
			TCHAR* pDataTemp = new TCHAR[len];
			if(pDataTemp==NULL) return false;

			memcpy(pDataTemp,val,len*sizeof(TCHAR));
			m_data = (UINT64)pDataTemp;
			return true;
		}
		bool VAR_VALUE::Assign(IN PVOID val)
		{
			do_free();
			m_type = AVT_POINTER;
			m_data = (UINT64)val;
			return true;
		}
		bool VAR_VALUE::Assign(IN VAR_VALUE& dest)
		{
			return Assign(&dest);
		}
		bool VAR_VALUE::Assign(IN VAR_VALUE* pDest)
		{
			if(pDest==NULL) return false;
			
			m_type = pDest->m_type;
			if(m_type == AVT_STRING)
				Assign((LPCTSTR)pDest->m_data);
			else
				m_data = pDest->m_data;
					
			return true;
		}
		void VAR_VALUE::do_free()
		{			
			PVOID pDataTemp = NULL;
			if(m_type==AVT_STRING)
				pDataTemp = (PVOID)m_data;

			DELETE_OBJECT(pDataTemp);
			m_type = AVT_NULL;
			m_data = 0x0;			
		}
		void VAR_VALUE::do_clear()
		{
			m_type = AVT_NULL;
			m_data = 0x0;
		}

		/////////////////////////// TaskArgs //////////////////////////////////////////////////////////////////////
		VAR_VALUE& TaskArgs::operator [](IN LPCSTR key)
		{
			assert("invalid parameter 'key'" && key!=NULL && *key!=0);
			map<string,VAR_VALUE>::iterator it = _datas.find(key);
			if(it == _datas.end())
			{
				VAR_VALUE argVal;
				_datas.insert(make_pair(key,argVal));
			}
			return _datas[key];
		}
		void TaskArgs::Assign(IN TaskArgs& dest)
		{
			_datas.clear();
			map<string,VAR_VALUE>::iterator it = dest._datas.begin();
			for (;it!=dest._datas.end();it++)
			{
				_datas.insert(make_pair(it->first,it->second));
			}
		}
		bool TaskArgs::GetBool(IN LPCSTR key,IN bool def)
		{
			map<string,VAR_VALUE>::iterator it = _datas.find(key);
			if(it == _datas.end()) return def;
			return it->second.GetBool();
		}
		INT32 TaskArgs::GetInt32(IN LPCSTR key,IN INT32 def)
		{
			map<string,VAR_VALUE>::iterator it = _datas.find(key);
			if(it == _datas.end()) return def;
			return it->second.GetInt32();
		}
		INT64 TaskArgs::GetInt64(IN LPCSTR key,IN INT64 def)
		{
			map<string,VAR_VALUE>::iterator it = _datas.find(key);
			if(it == _datas.end()) return def;
			return it->second.GetInt64();
		}
		UINT32 TaskArgs::GetUint32(IN LPCSTR key,IN UINT32 def)
		{
			map<string,VAR_VALUE>::iterator it = _datas.find(key);
			if(it == _datas.end()) return def;
			return it->second.GetUint32();
		}
		UINT64 TaskArgs::GetUint64(IN LPCSTR key,IN UINT64 def)
		{
			map<string,VAR_VALUE>::iterator it = _datas.find(key);
			if(it == _datas.end()) return def;
			return it->second.GetUint64();
		}
		double TaskArgs::GetFloat(IN LPCSTR key,IN double def)
		{
			map<string,VAR_VALUE>::iterator it = _datas.find(key);
			if(it == _datas.end()) return def;
			return it->second.GetFloat();
		}
		LPCTSTR TaskArgs::GetString(IN LPCSTR key,IN LPCTSTR def)
		{
			map<string,VAR_VALUE>::iterator it = _datas.find(key);
			if(it == _datas.end()) return def;
			return it->second.GetString();
		}
		PVOID TaskArgs::GetPointer(IN LPCSTR key,IN PVOID def)
		{
			map<string,VAR_VALUE>::iterator it = _datas.find(key);
			if(it == _datas.end()) return def;
			return it->second.GetPointer();
		}

		/////////////////////////// AsynTaskParams //////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 异步任务参数
		/// </summary>
		class AsynTaskParams
		{
		public:
			TaskArgs			m_workArgs;//任务参数
			TaskWorkingCallBack	m_pfnCallback;//任务线程的回调函数
			PVOID				m_this;//附件的this指针

		private:
			PVOID				_lpTaskObject;//任务对象指针,只给AsynTask使用

		public:
			AsynTaskParams(IN PVOID lpTask=NULL)
			{
				m_pfnCallback = NULL;
				m_this = NULL;
				_lpTaskObject = lpTask;
			}
			PVOID TaskObject()
			{
				return _lpTaskObject;
			}
		};


		/////////////////////////// AsynTask //////////////////////////////////////////////////////////////////////
		AsynTask::AsynTask(){}
		AsynTask::AsynTask(IN TaskArgs& args)
		{
			Parameter(args);
		}
		void AsynTask::Parameter(IN TaskArgs& args)
		{
			_parameters.Assign(args);
		}
		int AsynTask::Start(IN TaskWorkingCallBack pfnCallback,IN LPVOID lpParameter)
		{
			return Start(_parameters,pfnCallback,lpParameter);
		}
		int AsynTask::Start(IN TaskArgs& args,IN TaskWorkingCallBack pfnCallback,IN LPVOID lpParameter)
		{
			assert("invalid parameter 'pfnCallback'" && pfnCallback!=NULL);
			if(pfnCallback==NULL) return -1;
	
			//申请任务参数内存
			AsynTaskParams* pTaskArgs = new AsynTaskParams(this);
			if(pTaskArgs==NULL) return 1;

			DWORD hThreadID=0;
			pTaskArgs->m_pfnCallback = pfnCallback;
			pTaskArgs->m_this = lpParameter;
			pTaskArgs->m_workArgs = args;

			HANDLE hWorkingProc = ::CreateThread(NULL,0,OnWorkingProc,pTaskArgs,0,&hThreadID);
			if(hWorkingProc==NULL) return 2;
			::CloseHandle(hWorkingProc);
			return 0;
		}

		DWORD AsynTask::OnWorkingProc(IN LPVOID lpParameter)
		{
			AsynTaskParams* pArgs = (AsynTaskParams*)lpParameter;

			DWORD result = 0;
			if(pArgs->m_pfnCallback!=NULL){
				result = pArgs->m_pfnCallback(pArgs->m_workArgs,pArgs->m_this);
			}

			DELETE_OBJECT(pArgs);//释放任务参数内存
			return result;
		}

}}//end namespace
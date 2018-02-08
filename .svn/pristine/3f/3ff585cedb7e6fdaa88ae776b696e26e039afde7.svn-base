#include "stdafx.h"
#include "AsynTask.hpp"

namespace System{
	namespace Task{	

		/////////////////////////// AsynTaskParams //////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 异步任务参数
		/// </summary>
		class AsynTaskParams
		{
		public:
			String				m_workArgs;//任务参数
			TaskWorkingCallBack	m_pfnCallback;//任务线程的回调函数
			PVOID				m_this;//附件的this指针
			DWORD				m_workThrId;

		private:
			PVOID				_lpTaskObject;//任务对象指针,只给AsynTask使用

		public:
			AsynTaskParams(IN PVOID lpTask=NULL)
			{
				m_pfnCallback = NULL;
				m_this = NULL;
				_lpTaskObject = lpTask;
				m_workThrId = 0x0;
			}
			PVOID TaskObject()
			{
				return _lpTaskObject;
			}
		};


		/////////////////////////// AsynTask //////////////////////////////////////////////////////////////////////
		AsynTask::AsynTask()
		{
			_hWorkThr = NULL;
		}
		AsynTask::AsynTask(IN JsonText args)
		{
			Parameter(args);
			_hWorkThr = NULL;
		}
		AsynTask::~AsynTask()
		{
			CLOSE_HANDLE(_hWorkThr);
		}
		void AsynTask::Parameter(IN JsonText args)
		{
			_parameters = args;
			_paramItems.clear();
		}
		void AsynTask::Parameter(IN LPCSTR key,IN LPCTSTR value)
		{
			if(key == NULL||*key==0) return;
			if(value==NULL) value = _T("NULL");

			String valStr;
			valStr.FormatStr(_T("\"%s\""),value);
			_paramItems.insert(make_pair(key,valStr));
		}
		void AsynTask::Parameter(IN LPCSTR key,IN INT32 value)
		{
			if(key == NULL||*key==0) return;

			String valStr;
			valStr.FormatStr(_T("%d"),value);
			_paramItems.insert(make_pair(key,valStr));
		}
		void AsynTask::Parameter(IN LPCSTR key,IN UINT32 value)
		{
			if(key == NULL||*key==0) return;

			String valStr;
			valStr.FormatStr(_T("%u"),value);
			_paramItems.insert(make_pair(key,valStr));
		}
		void AsynTask::Parameter(IN LPCSTR key,IN INT64 value)
		{
			if(key == NULL||*key==0) return;

			String valStr;
			valStr.FormatStr(_T("%I64d"),value);
			_paramItems.insert(make_pair(key,valStr));
		}
		void AsynTask::Parameter(IN LPCSTR key,IN UINT64 value)
		{
			if(key == NULL||*key==0) return;

			String valStr;
			valStr.FormatStr(_T("%I64u"),value);
			_paramItems.insert(make_pair(key,valStr));
		}

		DWORD AsynTask::Start(IN TaskWorkingCallBack pfnCallback,IN LPVOID lpParameter)
		{
			if(_parameters.IsEmptyOrWhiteSpace() && !_paramItems.empty())
			{
				_parameters.Clear();
				_parameters.Append(_T("{"));
				map<string,String>::iterator it = _paramItems.begin();
				for (;it!=_paramItems.end();it++)
				{
					if(it!=_paramItems.begin()) _parameters.Append(_T(","));

					String keyStr = Convert::ToString(it->first.c_str(),IO::EC_ANSI);
					_parameters.AppendFormat(_T("\"%s\":%s"),(LPCTSTR)keyStr,it->second.c_str());
				}
				_parameters.Append(_T("}"));
			}
			return Start(_parameters,pfnCallback,lpParameter);
		}
		DWORD AsynTask::Start(IN LPCTSTR args,IN TaskWorkingCallBack pfnCallback,IN LPVOID lpParameter)
		{
			assert("invalid parameter 'pfnCallback'" && pfnCallback!=NULL);
			if(pfnCallback==NULL) return -1;
	
			//申请任务参数内存
			AsynTaskParams* pTaskArgs = new AsynTaskParams(this);
			if(pTaskArgs==NULL) return 1;

			_parameters = args;
			pTaskArgs->m_pfnCallback = pfnCallback;
			pTaskArgs->m_this = lpParameter;
			pTaskArgs->m_workArgs = _parameters;
			_hWorkThr = ::CreateThread(NULL,0,on_working_proc,pTaskArgs,0,&pTaskArgs->m_workThrId);

			if(_hWorkThr==NULL) return 2;
			return 0;
		}
		bool AsynTask::Wait(IN DWORD dwMilliseconds,OUT DWORD* lastError)
		{
			if(lastError!=NULL) *lastError = ERROR_SUCCESS;

			DWORD nBack = ::WaitForSingleObject(_hWorkThr,dwMilliseconds);
			if(nBack == WAIT_OBJECT_0)
			{
				CLOSE_HANDLE(_hWorkThr);
				return true;
			}
			
			if(nBack == WAIT_TIMEOUT) return false;
			//WAIT_FAILED
			if(lastError!=NULL) *lastError = ::GetLastError();
			return false;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DWORD AsynTask::on_working_proc(IN LPVOID lpParameter)
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
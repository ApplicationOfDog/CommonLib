#pragma once
#include <string>
#include <map>
using namespace std;

namespace System{
	namespace Task{

		///<summary>json对象的字符串类型</summary>
		typedef LPCTSTR JsonText;

		///<summary>任务的回调函数,也是真正执行任务的函数</summary>
		///<param name="args">任务参数,可能是普通字符串,也可能是json字符串</param>
		///<param name="lpParameter">附加参数,一般是操作对象的this指针</param>
		///<returns>异步线程退出码,即ExitCode</returns>
		typedef DWORD (*TaskWorkingCallBack)(IN LPCTSTR args,IN PVOID lpParameter);

		/// <summary>
		/// 异步任务抽象类
		/// </summary>
		class API_SYSTEM AsynTask
		{
		protected:
			String				_parameters;//异步任务携带的参数(可以是一般字符串也可以是Json字符串)
			map<string,String>	_paramItems;//参数项集合,最后会被转化为Json字符串保存到_parameters中.
			HANDLE				_hWorkThr;	//内部参数

		public:
			AsynTask();
			///<summary>构造函数</summary>
			///<param name="args">要传递的参数,可以是一般字符串也可以是Json字符串</param>
			AsynTask(IN JsonText args);
			~AsynTask();

			///<summary>设置参数</summary>
			///<param name="args">要传递的参数,可以是一般字符串也可以是Json字符串</param>
			void Parameter(IN JsonText args);
			///<summary>设置参数,键值对</summary>
			///<param name="key">要传递的参数Key</param>
			///<param name="value">要传递的参数Value</param>
			void Parameter(IN LPCSTR key,IN LPCTSTR value);
			void Parameter(IN LPCSTR key,IN INT32 value);
			void Parameter(IN LPCSTR key,IN UINT32 value);
			void Parameter(IN LPCSTR key,IN INT64 value);
			void Parameter(IN LPCSTR key,IN UINT64 value);

			///<summary>任务启动</summary>
			///<param name="args">任务参数</param>
			///<param name="pfnCallback">任务的回调函数,也是真正执行任务的函数</param>
			///<param name="lpParameter">附加参数,一般是操作对象的this指针</param>
			///<returns>0表示成功,其他值表示错误码
			/// -1:pfnCallback为NULL错误；1:申请任务参数内存失败,2:创建线程失败
			///</returns>
			DWORD Start(IN JsonText args,IN TaskWorkingCallBack pfnCallback,IN LPVOID lpParameter=NULL);
			DWORD Start(IN TaskWorkingCallBack pfnCallback,IN LPVOID lpParameter=NULL);
			///<summary>同步等待任务</summary>
			///<param name="dwMilliseconds">等待时间(ms),INFINITE:表示永久等待</param>
			///<param name="lastError">函数调用失败时返回的错误码,0表示成功没有错误</param>
			///<returns>true:任务结束返回,false:超时或发生错误</returns>
			bool Wait(IN DWORD dwMilliseconds = INFINITE,OUT DWORD* lastError = NULL);

		protected:
			static DWORD WINAPI on_working_proc(IN LPVOID lpParameter);
		};

}}//end namespace
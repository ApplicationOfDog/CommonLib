#pragma once
#include <time.h>

namespace System
{
	namespace IO
	{
		enum CalcMethod { CM_CLOCK,CM_COUNTER };

		///<summary>
		/// 执行时间日志（主要用于测试程序或算法的执行时间效率）
		///</summary>
		class API_CORE TimeLog
		{
		protected:
			void*		_pHelper;
			CalcMethod	_method;
			
			//程序执行时间日志
			clock_t				_tBegin;
			clock_t				_tLast;
			clock_t				_tNext;

			LARGE_INTEGER		_perfFreq;
			LARGE_INTEGER		_perfBegin;
			LARGE_INTEGER		_perfLast;
			LARGE_INTEGER		_perfNext;

		public:
			TimeLog();
			TimeLog(TLogSave type,String path,CalcMethod cm);
			~TimeLog();

			// 摘要:
			//     开始计时
			virtual void Start();
			// 摘要:
			//     产生时间记录,参数是记录前缀文本
			//     记录格式:%s-duration:%.2lf(s)
			virtual void Record(LPCTSTR format,...);
			// 摘要:
			//     结束时间计时,并将所有的记录输出到存储介质
			//     text:记录前缀文本
			//     bOutDuration:是否产生于上一次的时间间隔记录(TRUE:产生,FALSE:不产生)
			//     最后的记录格式:%s total running times:%.2lf(s)
			virtual void End(LPCTSTR text,bool bOutDuration);

		protected:
			void Init();
			virtual void InnerRecord(LPCTSTR text,bool bOutText);
		};
	}	
}
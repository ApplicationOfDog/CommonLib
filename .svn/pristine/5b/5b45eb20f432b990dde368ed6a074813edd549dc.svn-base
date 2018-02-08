#pragma once

namespace System{
	namespace Pattern{


		/// <summary>
		/// 同步锁接口(临界区,用于应用层线程同步)
		/// 如下特殊用法说明:
		/// 1.同一线程两次Lock会不会阻塞? 答案:不会
		/// 2.同一线程两次Lock，调用一次Unlock 另一个线程是否可以Lock? 答案:不可,两次Lock必须调用Unlock两次
		/// </summary>
		class API_CORE ILock
		{
		protected:
			CRITICAL_SECTION _hGuard;  

		public:
			ILock(){ InitializeCriticalSection(&_hGuard); }
			virtual ~ILock(){ DeleteCriticalSection(&_hGuard); }

		protected:
			virtual void Lock(){ EnterCriticalSection(&_hGuard); }
			virtual void Unlock(){ LeaveCriticalSection(&_hGuard); }
		};


}}//end namespace

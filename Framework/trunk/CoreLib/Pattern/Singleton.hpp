#pragma once
#include <memory>

namespace System
{
	/// <summary>
	/// 设计模式-单实例模式
	/// </summary>
	namespace Pattern{

		///<summary>设计模式-单实例
		/// 	class CTest : public Singleton_ptr<CTest>{
		/// 		friend class Singleton_ptr<CTest>;//让protect的构造函数被Singleton_ptr<>使用
		/// 		protected:
		/// 			CTest();//私有化构造函数,即不允许通过构造函数生成新实例对象
		/// 	}
		///</summary>
		template <class T>
		class API_CORE Singleton
		{
		public:
			~Singleton () {}
			static T& Instance()
			{
				static T _instance;
				return _instance;
			}

		protected:
			Singleton () {}

		private:
			Singleton ( const Singleton& );
			const Singleton& operator=( const Singleton& );
		};

		template <class T>
		class API_CORE Singleton_ptr
		{
		public:
			~Singleton_ptr () {}

			static T* Instance ()
			{
				static std::auto_ptr<T> _pInstance;
				if (!_pInstance.get ()) {
					_pInstance = std::auto_ptr<T>(new T);
				}
				return _pInstance.get();
			}
		protected:
			Singleton_ptr () {}
		};

	}
}
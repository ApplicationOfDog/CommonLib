#pragma once
#include <map>
using namespace std;

///<summary>实现反射机制
/// 
/// #region 反射使用说明
///	class SubA:public IReflect<SubA>
///	{
///	}
///	REFLECT_REGISTER(SubA);//向反射管理器注册SubA
/// #endregion
///</summary>

namespace System{
	namespace Pattern{

		///<summary>对象指针</summary>
		typedef void* PObject;
		///<summary>类实例化函数指针</summary>
		typedef PObject (*InstantiationMethod)();

		///<summary>数据类型信息</summary>
		class API_CORE TypeInfo
		{
		public:
			int		m_group;				//目标类型所属的组,用于类型进行分组
			string	m_name;					//类型名称,一般是类名称即className
			InstantiationMethod m_pfnInstantiation;//创建类型实例的函数指针

		public:
			TypeInfo(IN LPCSTR name,IN InstantiationMethod method);
			TypeInfo(IN LPCSTR name,IN InstantiationMethod method,IN int group=0,IN bool isRegister=true);
			void Clear();
		};
		typedef map<string,TypeInfo*> TypesMap;

		///<summary>类型反射管理器</summary>
		class API_CORE ReflectManager
		{
		public:
			///<summary>类型名称-类型信息映射</summary>
			static TypesMap* g_pTypeInfoMap;

		public:
			///<summary>根据类型名称创建类型实例对象,NULL表示创建失败</summary>
			static PObject CreateObject(IN LPCSTR typeName);

			///<summary>向管理器注册给定类型</summary>
			static void Register(IN TypeInfo* pType);
			///<summary>向管理器反注册给定类型</summary>
			static bool Unregister(IN TypeInfo* pType);
		};

		template<class T>
		class API_CORE IReflect
		{
		protected:
			static TypeInfo* g_pTypeInfo;

		public:
			///<summary>类型实例化函数,即创建实例对象函数</summary>
			static PObject Instantiation(){ return new T(); }
		};
		///<summary>注册反射向反射管理器注册T类型</summary>
		#define REFLECT_REGISTER(T) TypeInfo* IReflect<T>:: g_pTypeInfo = new TypeInfo(#T,&T::Instantiation,0,true)
		#define REFLECT_REGISTER_BY_GROUP(T,Group) TypeInfo* IReflect<T>:: g_pTypeInfo = new TypeInfo(#T,&T::Instantiation,Group,true)
	
}}//end namespace
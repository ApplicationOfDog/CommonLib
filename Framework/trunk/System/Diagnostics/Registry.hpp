#pragma once

namespace System{
	namespace Diagnostics{

	///<summary>
	/// 注册表操作
	///</summary>
		class API_SYSTEM Registry:public Pattern::IError
		{
		private:
			HKEY	_hRootKey;
			HKEY	_hSubKey;
			String	_sSubKey;
			REGSAM	_desired;

		public:
			Registry(IN HKEY rootKey =NULL,IN LPCTSTR subKey=NULL,IN REGSAM desired=KEY_QUERY_VALUE);
			~Registry();

			///<summary>打开注册表</summary>
			///<param name="rootKey">根键,PS:HKEY_CURRENT_USER,HKEY_LOCAL_MACHINE等</param>
			///<param name="subKey">子健路径字符串</param>
			///<param name="desired">访问权限,PS:KEY_READ,KEY_ALL_ACCESS等.
			/// 如果32位程序需要在x64系统中进行Wow64重定向(即写入非Wow6432Node下)请加入KEY_WOW64_64KEY属性,
			/// PS.x64系统中HKEY_CURRENT_USER是没有重定向问题的
			///</param>
			///<param name="isCreateIfWithout">当键不存在时是否创建</param>
			///<result>是否打开成功</result>
			bool Open(IN HKEY rootKey,IN LPCTSTR subKey,IN REGSAM desired=KEY_QUERY_VALUE,IN bool isCreateIfWithout=false);
			///<summary>关闭注册表项</summary>
			void Close();

			///<summary>查询键值</summary>
			///<param name="name">值名称</param>
			String QueryString(IN LPCTSTR name,IN LPCTSTR def=NULL);
			UINT32 QueryUInt32(IN LPCTSTR name,IN UINT32 def=0);
			UINT64 QueryUInt64(IN LPCTSTR name,IN UINT64 def=0);

			///<summary>删除键下的值</summary>
			///<param name="name">值名称,NULL表示将整个键删除</param>
			///<param name="isRemoveKeyNotEmpty">原则上是不能删除非空键的,true表示强制删除非空键,false:不删除非空键</param>
			bool Delete(IN LPCTSTR name,IN bool isRemoveKeyNotEmpty = false);

			///<summary>将字符串写入注册表</summary>
			///<param name="name">要设置的值名称</param>
			///<param name="val">要设置的值数据</param>
			///<param name="isMulti">false表示REG_SZ类型,true表示REG_MULTI_SZ类型</param>
			///<returns>是否成功</returns>
			bool SetString(IN LPCTSTR name,IN LPCTSTR data,IN bool isMulti=false);

			///<summary>将字符串写入注册表</summary>
			///<param name="name">要设置的值名称</param>
			///<param name="val">要设置的值数据</param>
			///<returns>是否成功</returns>
			bool SetUInt32(IN LPCTSTR name,IN UINT32 data);
			bool SetUInt64(IN LPCTSTR name,IN UINT64 data);

		private:
			void Clear();
		};
}}
#pragma once

/*
namespace System{
	namespace JSON{

		class API_Json JsonValue:public Json::Value
		{
		public:
			JsonValue();
			JsonValue(IN const Json::Value& v);

			///<summary>将字符串转为json对象</summary>
			///<param name="isParse">true表示字符串是json字符串要进行解析,false:表示普通字符串</param>
			JsonValue(IN LPCTSTR text,IN bool isParse = false);
			JsonValue(IN LPCSTR text,IN bool isParse = false);

		public:
			///<summary>将json字符串转为json对象</summary>
			bool FromString(IN LPCTSTR text);
			bool FromString(IN LPCSTR text);

			///<summary>将json对象转为json字符串</summary>
			String ToString(IN bool isFormat = false);

		public:
			bool IsNull();
			bool IsArray();
			bool IsObject();
			UINT Size();

			JsonValue GetMember(IN UINT index);
			bool HasMember(IN LPCSTR name);

			///<summary>选择匹配XPath表达式的第一个成员</summary>
			///<param name="xpath">XPath 表达式
			/// 例如:"User.item"表示获取User对象下的item成员
			///</param>
			JsonValue GetMemberByXPath(IN LPCSTR xpath);

		public:
			///<summary>获取字符串值或成员的字符串值</summary>
			///<param name="key">
			/// NULL表示当前对象是字符串的JSON对象;
			/// XPATH表达式(例如:"AA.BB.CC...")则表示要获取的对象链
			///</param>
			///<param name="def">获取失败时返回的默认值</param>
			string GetStringA(IN LPCSTR key,IN LPCSTR def=NULL);
			///<param name="code">json中字符串的编码</param>
			String GetString(IN LPCSTR key,IN LPCTSTR def=NULL,IN IO::Encoding code = IO::EC_ANSI);

			///<summary>获取布尔值或成员的布尔值</summary>
			///<param name="key">
			/// NULL表示当前对象是字符串的JSON对象;
			/// XPATH表达式(例如:"AA.BB.CC...")则表示要获取的对象链
			///</param>
			bool GetBool(IN LPCSTR key,IN bool def=false);

			///<summary>获取整数值或成员的整数值</summary>
			///<param name="key">
			/// NULL表示当前对象是字符串的JSON对象;
			/// XPATH表达式(例如:"AA.BB.CC...")则表示要获取的对象链
			///</param>
			INT32 GetInt32(IN LPCSTR key,IN INT32 def=0);
			UINT32 GetUInt32(IN LPCSTR key,IN UINT32 def=0);

			///<param name="key">
			/// NULL表示当前对象是字符串的JSON对象;
			/// XPATH表达式(例如:"AA.BB.CC...")则表示要获取的对象链
			///</param>
			INT64 GetInt64(IN LPCSTR key,IN INT64 def=0);
			UINT64 GetUInt64(IN LPCSTR key,IN UINT64 def=0);

			///<param name="key">
			/// NULL表示当前对象是字符串的JSON对象;
			/// XPATH表达式(例如:"AA.BB.CC...")则表示要获取的对象链
			///</param>
			double GetDouble(IN LPCSTR key,IN double def=0.0);

		public:
			bool Set(IN LPCSTR name,IN JsonValue val);

		private:
			Json::Value* getMemberBy(IN LPCSTR name);
			Json::Value* getMemberByXPath(IN LPCSTR xpath);

		};
		
	}
}
*/
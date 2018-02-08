#pragma once

namespace System
{
	namespace Text
	{		
		class API_CORE StringBuilder
		{
		public:
			StringBuilder();
			StringBuilder(IN StringBuilder& builder);
			~StringBuilder();
			/// <summary>
			/// 初始化类的新实例，该类起始于指定容量并且可增长到指定的最大容量
			/// </summary>
			/// <param name="capacity">建议起始大小</param>
			/// <param name="maxCapacity">当前字符串可包含的最大字符数</param>
			StringBuilder(int capacity, int maxCapacity);
			/// <summary>获取可包含在当前实例所分配的内存中的最大字符数</summary>
			int Capacity();
			/// <summary>获取当前对象的长度,此实例的长度</summary>
			int Length();
			/// <summary>
			/// 获取此实例的最大容量,此实例可容纳的最大字符数
			/// </summary>
			int MaxCapacity();
			/// <summary>
			/// 设置换行符，例如_T("\r\n")或_T("<br/>")
			/// </summary>
			void NewLine(IN LPCTSTR chars);
			/// <summary>
			/// 设置制表符,例如替换成空格_T(' ')
			/// </summary>
			void TabChar(IN TCHAR tab);

			/// <summary>
			/// 在此实例追加指定字符串
			/// </summary>
			void Append(IN const String& v);
			void Append(IN int v);
			void Append(IN UINT64 v);
			void Append(IN LONG64 v,IN int base);
			/// <param name="count">保留的小数位数</param>
			void Append(IN double v,IN int count);
			///<summary>追加字符</summary>
			/// <param name="count">追加次数</param>
			void Append(IN TCHAR c,IN int count);
			void Append(IN LPCTSTR format,IN va_list args);
			void AppendFormat(IN LPCTSTR format,...);
			void AppendLine();
			void AppendLine(IN LPCTSTR format,...);
			void Append(IN int nTab,IN LPCTSTR format,...);
			void AppendLine(IN int nTab,IN LPCTSTR format,...);
			
			/// <summary>
			/// 删除尾部指定的字符或字符串
			/// </summary>
			void TrimEnd(IN LPCTSTR trims);
			void TrimEnd(IN TCHAR c); 

			/// <summary>
			/// 从当前实例中移除所有字符
			/// </summary>
			void Clear();
			bool IsEmpty();
			void Assign(IN StringBuilder& builder);

			///<summary>转为字符串对象</summary>
			String ToString();

			///<summary>转为字符串对象-ansi版本</summary>
			///<param name="code">目标文件编码</param>
			string ToStringA(IN IO::Encoding code);
			
		protected:
			PVOID _pData;			
		};
	}	
}
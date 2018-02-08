#pragma once

namespace System
{
	namespace Text
	{		
		class StringBuilderHelper
		{
		public:
			TCHAR*	m_ChunkChars;//字符缓冲区
			int		m_ChunkLength;//当前字符个数(不包含结束字符\0)
			int		m_MaxCapacity;//最大容量(即当前对象最多允许的加入的字符数)
			int		m_Capacity;//当前容量，即当前缓冲区可以容纳的字符数(其实就是当前缓冲区内存的字符数)
			int		m_BlockSize;//默认申请块大小

			String	m_newLine;//换行符
			TCHAR	m_tabChar;//tab符号

		public:			
			StringBuilderHelper();
			StringBuilderHelper(int capacity,int maxCapacity);
			StringBuilderHelper(IN StringBuilderHelper* pData);
			~StringBuilderHelper();

			void Initialize(int capacity,int maxCapacity);
			void Append(IN const String& value);
			void Append(IN TCHAR* str,IN int length);

			void TrimEnd(IN LPCTSTR trims);
			void TrimEnd(IN TCHAR c); 

			///<summary>重新分配空间</summary>
			void ExpandByABlock(int minBlockCharCount);
			void Clear();
			void Assign(IN StringBuilderHelper* pData);
		};
	}	
}
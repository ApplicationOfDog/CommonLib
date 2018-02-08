#include "stdafx.h"
#include <stdio.h>
#include "StringBuilderHelper.hpp"
#include "../../Core/Environment.hpp"

#define sizeOfChars(n) ((n)*sizeof(TCHAR))

namespace System
{
	namespace Text
	{
		StringBuilderHelper::StringBuilderHelper(){
			Initialize(16,8000);
		}
		StringBuilderHelper::StringBuilderHelper(int capacity, int maxCapacity){
			Initialize(capacity,maxCapacity);
		}
		StringBuilderHelper::StringBuilderHelper(IN StringBuilderHelper* pData)
		{
			assert(pData!=NULL);
			m_Capacity = 0;
			m_ChunkChars = NULL;
			Assign(pData);
		}
		void StringBuilderHelper::Initialize(int capacity, int maxCapacity)
		{
			m_ChunkChars=NULL;
			m_BlockSize=1024;
			m_newLine = Environment::NewLine;
			m_tabChar = _T('\t');

			if (capacity > maxCapacity)
				throw exception("capacity:ArgumentOutOfRange_Capacity");

			if (maxCapacity < 1)
				throw exception("maxCapacity:ArgumentOutOfRange_SmallMaxCapacity");

			if (capacity < 0)
				throw exception("capacity:ArgumentOutOfRange_MustBePositive");

			if (capacity == 0)
				capacity = min(0x10,maxCapacity);
			
			m_Capacity = capacity;
			m_MaxCapacity = maxCapacity;
			m_ChunkChars = new TCHAR[m_Capacity];
			m_ChunkLength=0;
			if(m_ChunkChars==NULL)
			{
				assert(m_ChunkChars!=NULL);
				throw runtime_error("alloc failed,Out of memory");
			}
			ZeroMemory(m_ChunkChars,sizeOfChars(m_Capacity));
		}
		StringBuilderHelper::~StringBuilderHelper()
		{
			free(m_ChunkChars);
			m_ChunkChars=NULL;

			m_ChunkLength=0;
			m_MaxCapacity=0;
			m_Capacity=0;
			m_BlockSize=0;
		}
		void StringBuilderHelper::Append(IN const String& value)
		{
			if(value.empty()) return;

			TCHAR* str = (TCHAR*)value.c_str();
			int length = (int)value.length();
			int newChunkLen = m_ChunkLength + length;
			if (newChunkLen < m_Capacity)
			{
				memcpy(m_ChunkChars+m_ChunkLength,str,sizeOfChars(length));
				m_ChunkLength = newChunkLen;
				return;
			}
			Append(str,length);
		}
		void StringBuilderHelper::Append(IN TCHAR* val,IN int length)
		{
			assert(length>0);

			errno_t ret = 0;
			int num = length + m_ChunkLength;
			if (num < m_Capacity)
			{
				memcpy(m_ChunkChars+m_ChunkLength,val,sizeOfChars(length));
				m_ChunkLength = num;
				return;
			}
			int count = m_Capacity - m_ChunkLength;
			int minBlockCharCount = length - count+1;//more than 1 char for '\0'
			ExpandByABlock(minBlockCharCount);
			memcpy(m_ChunkChars+m_ChunkLength,val,sizeOfChars(length));
			m_ChunkLength = num;
		}
		void StringBuilderHelper::TrimEnd(IN LPCTSTR trims)
		{
			if(trims==NULL||*trims==0) return;
			int szTrim = (int)_tcslen(trims);//目标字符串长度(不包含结束字符'\0')
			if(m_ChunkLength<szTrim) return;//当前实例字符串长度比目标字符串长度短

			//从实例字符串尾部开始定位
			TCHAR* p = m_ChunkChars + (m_ChunkLength-szTrim);//指向尾部段(与目标字符长度相同)
			while(p>=m_ChunkChars)
			{
				if(memcmp(p,trims,sizeOfChars(szTrim))!=0) break;
				ZeroMemory(p,sizeOfChars(szTrim));
				m_ChunkLength-=szTrim;
				p-=szTrim;
			}
		}
		void StringBuilderHelper::TrimEnd(IN TCHAR c)
		{
			if(m_ChunkLength<=0||c==0) return;

			//从尾部开始删除指定的字符
			TCHAR* p = m_ChunkChars + (m_ChunkLength-1);//指向最后一个字符
			while(p>=m_ChunkChars && *p==c){
				*p--=0;//清空字符
				m_ChunkLength--;//减少当前字符数
			}
		}
		void StringBuilderHelper::ExpandByABlock(int minBlockCharCount)
		{			
			if (minBlockCharCount + m_Capacity > m_MaxCapacity)
			{
				assert(FALSE);
				throw exception("requiredLength:ArgumentOutOfRange_SmallCapacity");
			}

			int szBlock = max(minBlockCharCount,m_BlockSize);
			if (szBlock + m_Capacity > m_MaxCapacity)
				szBlock = m_MaxCapacity - m_Capacity;

			int newCapacity = m_Capacity + szBlock;
			TCHAR* pBuffer = (TCHAR*)realloc(m_ChunkChars,sizeOfChars(newCapacity));
			if(pBuffer==NULL)
			{
				assert(FALSE);
				throw runtime_error("realloc memory failed.");
			}			
			ZeroMemory(pBuffer+m_Capacity,sizeOfChars(szBlock));
			m_ChunkChars = pBuffer;
			m_Capacity = newCapacity;
		}
		void StringBuilderHelper::Clear()
		{
			if(m_ChunkLength<=0) return;
			ZeroMemory(m_ChunkChars,sizeOfChars(m_ChunkLength));
			m_ChunkLength=0;
		}
		void StringBuilderHelper::Assign(IN StringBuilderHelper* pData)
		{
			assert(pData!=NULL);
			m_newLine = pData->m_newLine;
			m_tabChar = pData->m_tabChar;

			if(m_Capacity<pData->m_Capacity)
			{
				DELETE_MEM(m_ChunkChars);
				m_Capacity = pData->m_Capacity;
				m_ChunkChars = new TCHAR[m_Capacity];
				ZeroMemory(m_ChunkChars,sizeOfChars(m_Capacity));
				memcpy(m_ChunkChars,pData->m_ChunkChars,sizeOfChars(m_Capacity));
				m_ChunkLength = pData->m_ChunkLength;
				m_MaxCapacity = pData->m_MaxCapacity;
				m_BlockSize = pData->m_BlockSize;
			}
			else
			{
				memcpy(m_ChunkChars,pData->m_ChunkChars,sizeOfChars(pData->m_ChunkLength));
				m_ChunkLength = pData->m_ChunkLength;
				ZeroMemory(m_ChunkChars+m_ChunkLength,sizeOfChars(m_Capacity-m_ChunkLength));
			}
		}
	}	
}
#include "stdafx.h"
#include "Memory.hpp"

namespace System{
	namespace Data{
	
		MemoryBlock::MemoryBlock()
		{
			m_address = NULL;
			m_size=0;
		}
		MemoryBlock::MemoryBlock(IN PBYTE addr,IN int size)
		{
			m_address = addr;
			m_size = size;
		}
		

		///////////////////////////// MemBuffer /////////////////////////////////////
		MemBuffer::MemBuffer()
		{
			_szBuffer = 0;
			_pBuffer = NULL;
		}
		MemBuffer::~MemBuffer()
		{
			FreeBuffer();
		}
		int MemBuffer::Size()
		{
			return _szBuffer;
		}
		int MemBuffer::Push(IN PBYTE addr,IN int size)
		{
			if(addr==NULL||size<=0) return _szBuffer;

			MemoryBlock mb(addr,size);
			_blocks.push_back(mb);
			_szBuffer += size;
			return _szBuffer;
		}
		int MemBuffer::PushText(IN LPCTSTR str,IN bool isAddEndChar)
		{
			if(str==NULL||*str==0) return _szBuffer;

			int size = (int)(_tcslen(str)+(int)isAddEndChar)*sizeof(*str);
			return Push((PBYTE)str,size);
		}
		int MemBuffer::PushPlaceHolder(IN int size)
		{
			MemoryBlock mb(NULL,size);
			_blocks.push_back(mb);
			_szBuffer += size;
			return _szBuffer;
		}
		void MemBuffer::Clear()
		{
			_blocks.clear();
			_szBuffer = 0;
		}
		int MemBuffer::CopyTo(IN PBYTE pBuffer)
		{
			int position = 0;
			MemoryBlock mb;
			BlockCollection::iterator it = _blocks.begin();
			for (;it!=_blocks.end();it++)
			{
				mb = *it;
				assert(mb.m_address!=NULL && mb.m_size>0);
				memcpy(pBuffer+position,mb.m_address,mb.m_size);
				position+=mb.m_size;
			}
			return position;
		}
		int MemBuffer::CopyFrom(IN PBYTE pBuffer)
		{
			int position = 0;
			MemoryBlock mb;
			BlockCollection::iterator it = _blocks.begin();
			for (;it!=_blocks.end();it++)
			{
				mb = *it;
				assert(mb.m_address!=NULL && mb.m_size>0);
				memcpy(mb.m_address,pBuffer+position,mb.m_size);
				position+=mb.m_size;
			}
			return position;
		}
		PBYTE MemBuffer::GetBuffer()
		{
			FreeBuffer();
			if(_szBuffer<=0) return NULL;

			_pBuffer = new BYTE[_szBuffer];
			if(_pBuffer==NULL) return NULL;

			ZeroMemory(_pBuffer,_szBuffer);
			CopyTo(_pBuffer);
			return _pBuffer;
		}
		void MemBuffer::FreeBuffer()
		{
			if(_pBuffer==NULL) return;

			delete[] _pBuffer;
			_pBuffer = NULL;
		}
		int MemBuffer::ParseBufferToVar()
		{
			if(_pBuffer==NULL) return 0;
			return CopyFrom(_pBuffer);
		}
}}
#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include "Runtime.hpp"

#define TIME_CLOCK_NULL 0x0

namespace System{
	namespace Diagnostics{

		RTDuration::RTDuration(IN LPCSTR name,IN bool isBegin,IN LPCSTR description)
		{
			m_id = -1;
			m_end = TIME_CLOCK_NULL;
			m_begin = !isBegin ? TIME_CLOCK_NULL : ::clock();

			if(name!=NULL && *name!=0)
				m_name = name;
			if(description!=NULL && *description!=0)
				m_description = description;
		}

		void RTDuration::Clear()
		{
			m_id = -1;
			m_name.clear();
			m_description.clear();
			m_begin = TIME_CLOCK_NULL;
			m_end = TIME_CLOCK_NULL;
		}
		void RTDuration::Name(IN LPCSTR text)
		{
			if(text!=NULL && *text!=0)
				m_name = text;
			else
				m_name.clear();
		}
		void RTDuration::Description(IN LPCSTR text)
		{
			if(text!=NULL && *text!=0)
				m_description = text;
			else
				m_description.clear();
		}
		void RTDuration::Start(IN LPCSTR name)
		{
			m_begin = ::clock();
			if(name!=NULL && *name!=0) m_name = name;
		}
		void RTDuration::End()
		{ 
			m_end = ::clock(); 
		}
		long RTDuration::Duration()
		{ 
			return m_end - m_begin; 
		}


		////////////////////////////////////////////////////////////////////////////////////////
		////Runtime
		Runtime::Runtime(IN bool enabled,IN bool isOutputDebugView)
		{
			_isEnabled = enabled;
			_isOutputDebugView = isOutputDebugView;
		}
		Runtime::~Runtime(){ Clear(); }

		void Runtime::Set(IN bool enabled,IN bool isOutputToDebugView)
		{ 
			_isEnabled = enabled; 
			_isOutputDebugView = isOutputToDebugView;
		}
		void Runtime::Clear()
		{
			RTDurations::iterator it = _durationMr.begin();
			for (;it!=_durationMr.end();it++)
			{
				if(it->second==NULL) continue;
				delete it->second;
				it->second = NULL;
			}
			_durationMr.clear();
		}
		int Runtime::Start(IN LPCSTR name,IN LPCSTR description,OUT clock_t* pBegin)
		{
			if(pBegin!=NULL) *pBegin = TIME_CLOCK_NULL;
			if(!_isEnabled) return -1;

			string nameStr;
			if(name==NULL||*name==0)
				nameStr = String::FormatA("_internal_%d_",_durationMr.size()+1);
			else
				nameStr = name;

			RTDuration* pItem = find(nameStr.c_str());
			if(pItem==NULL)
			{
				pItem = new RTDuration(nameStr.c_str(),false,description);				
				_durationMr.insert(make_pair(pItem->m_name,pItem));
				pItem->m_id = _durationMr.size();
				pItem->Start();
			}
			else
			{
				pItem->Description(description);
				pItem->Start();				
			}
			if(pBegin!=NULL) *pBegin = pItem->m_begin;
			return pItem->m_id;
		}
		long Runtime::End(IN LPCSTR name)
		{
			if(!_isEnabled) return -1;

			RTDuration* pItem = find(name);
			if(pItem == NULL) return -2;

			pItem->End();
			return pItem->Duration();
		}
		long Runtime::End(IN int id)
		{
			if(!_isEnabled) return -1;

			RTDuration* pItem = find(id);
			if(pItem == NULL) return -2;

			pItem->End();
			return pItem->Duration();
		}
		void Runtime::OutputDebugText(IN int nTab,IN String* containter)
		{
			if(!_isEnabled) return;

			vector<RTDuration*> sortList;
			int tempID = 0x0;
			while(true)
			{
				tempID = 0x0fffffff;
				RTDurations::iterator it,foundIt = _durationMr.end();
				for (it = _durationMr.begin();it!=_durationMr.end();it++)
				{
					vector<RTDuration*>::iterator vit = sortList.begin();
					while(vit!=sortList.end() && *vit!=it->second) vit++;

					if(vit!=sortList.end()) continue;//已经被加入顺序列表了
					if(it->second->m_id>tempID) continue;

					tempID = it->second->m_id;
					foundIt = it;
				}
				if(foundIt == _durationMr.end()) break;
				sortList.push_back(foundIt->second);//按照ID从小到大顺序加入顺序列表
			}			

			String line,tagName;
			double duration = 0.0;
			vector<RTDuration*>::iterator vit = sortList.begin();
			for (;vit!=sortList.end();vit++)
			{
				RTDuration* pItem = *vit;
				if(pItem->m_end == TIME_CLOCK_NULL) pItem->End();

				duration = pItem->Duration();
				tagName = Convert::ToString(pItem->m_name.c_str());
				
				line.Clear();
				if(nTab>0) line.Append(_T(' '),nTab);
				line.AppendFormat(_T("%d:\"%s\"=>duration:%.1lf(ms)=%.2f(s)"),pItem->m_id,(LPCTSTR)tagName,duration,duration/CLOCKS_PER_SEC);

				if(containter!=NULL)
				{
					containter->Append(line);
					continue;
				}

				if(_isOutputDebugView)
					::OutputDebugString(line);
				else
					Console::WriteLine(line);
			}
		}
		long Runtime::Duration(IN LPCSTR name,OUT clock_t* pBegin)
		{
			if(pBegin!=NULL) *pBegin = TIME_CLOCK_NULL;
			if(!_isEnabled) return -1;

			RTDuration* pItem = find(name);
			if(pItem==NULL) return -2;

			if(pBegin!=NULL) *pBegin = pItem->m_begin;
			return pItem->Duration();
		}
		long Runtime::Duration(IN int id,OUT clock_t* pBegin)
		{
			if(pBegin!=NULL) *pBegin = TIME_CLOCK_NULL;
			if(!_isEnabled) return -1;

			RTDuration* pItem = find(id);
			if(pItem==NULL) return -2;

			if(pBegin!=NULL) *pBegin = pItem->m_begin;
			return pItem->Duration();
		}
		/////////////////////////////////////////////////////////////////////////////////////////
		RTDuration* Runtime::find(IN int id)
		{
			if(id <=0 && _durationMr.size()==1)
				return _durationMr.begin()->second;

			RTDurations::iterator it = _durationMr.begin();
			for (;it!=_durationMr.end();it++)
			{
				if(it->second->m_id != id) continue;
				return it->second;
			}
			return NULL;
		}
		RTDuration* Runtime::find(IN LPCSTR name)
		{
			if(name==NULL && _durationMr.size()==1)
				return _durationMr.begin()->second;

			RTDurations::iterator it = _durationMr.find(name);
			if(it == _durationMr.end()) return NULL;
			return it->second;
		}
}}
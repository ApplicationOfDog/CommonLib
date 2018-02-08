#include "stdafx.h"
#include "Reflect.hpp"

namespace System{
	namespace Pattern{

		TypeInfo::TypeInfo(IN LPCSTR name,IN InstantiationMethod method)
		{
			assert(name!=NULL && *name!=0 && method!=NULL);
			
			m_group = 0;
			m_name = name;
			m_pfnInstantiation = method;
			ReflectManager::Register(this);
		}
		TypeInfo::TypeInfo(IN LPCSTR name,IN InstantiationMethod method,IN int group,IN bool isRegister)
		{
			assert(name!=NULL && *name!=0 && method!=NULL);
			m_group = group;
			m_name = name;
			m_pfnInstantiation = method;

			if(isRegister) ReflectManager::Register(this);
		}
		void TypeInfo::Clear()
		{
			m_group = 0;
			m_name.clear();
			m_pfnInstantiation = NULL;
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		////// ReflectManager

		map<string,TypeInfo*>* ReflectManager::g_pTypeInfoMap = NULL;

		PObject ReflectManager::CreateObject(IN LPCSTR typeName)
		{
			if(typeName==NULL||*typeName==0) return NULL;

			TypesMap::iterator it = g_pTypeInfoMap->find(typeName);
			if(it==g_pTypeInfoMap->end()) return NULL;

			return it->second->m_pfnInstantiation();
		}

		void ReflectManager::Register(IN TypeInfo* pType)
		{
			assert(pType!=NULL && !pType->m_name.empty());

			if(g_pTypeInfoMap==NULL)
				g_pTypeInfoMap = new TypesMap();

			TypesMap::iterator it = g_pTypeInfoMap->find(pType->m_name);
			if(it!=g_pTypeInfoMap->end())
			{
				throw runtime_error("register same class in one more times");
				return;
			}
			
			g_pTypeInfoMap->insert(make_pair(pType->m_name,pType));
		}

		bool ReflectManager::Unregister(IN TypeInfo* pType)
		{
			if(pType==NULL||pType->m_name.empty()||g_pTypeInfoMap==NULL) return false;

			TypesMap::iterator it = g_pTypeInfoMap->find(pType->m_name);
			if(it==g_pTypeInfoMap->end()) return false;

			g_pTypeInfoMap->erase(it);
			return true;
		}
		

}}//end namespace
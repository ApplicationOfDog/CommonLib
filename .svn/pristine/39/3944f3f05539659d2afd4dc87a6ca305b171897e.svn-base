#include "stdafx.h"
#include "Registry.hpp"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace System{
	namespace Diagnostics{	

		Registry::Registry(IN HKEY rootKey,IN LPCTSTR subKey,IN REGSAM desired)
		{
			Clear();
			if(desired!=_desired) _desired = desired;

			if(rootKey!=NULL||(subKey!=NULL && *subKey!=0))
				Open(rootKey,subKey,desired);
		}
		Registry::~Registry()
		{
			Close();
		}
		bool Registry::Open(IN HKEY rootKey,IN LPCTSTR subKey,IN REGSAM desired,IN bool isCreateIfWithout)
		{
			if(rootKey!=NULL) _hRootKey = rootKey;
			if(subKey!=NULL && *subKey!=0){
				_sSubKey = subKey;
				_sSubKey.Trim();
			}

			_desired = desired;
			
			IError::Clear();
			Close();
			//PS.subKey结尾不能待任何空格否则会返回lastError=6
			m_nLastError = (DWORD)::RegOpenKeyEx(_hRootKey,_sSubKey.c_str(),0,desired,&_hSubKey);
			if(m_nLastError != ERROR_SUCCESS)
			{
				_hSubKey = NULL;
				m_sLastError = Environment::LastErrorText(m_nLastError);
				if(m_nLastError == ERROR_FILE_NOT_FOUND && isCreateIfWithout)
				{//键不存在时创建它
					DWORD dwDisposition = REG_CREATED_NEW_KEY;
					m_nLastError = (DWORD)::RegCreateKeyEx(_hRootKey,subKey,0,NULL,REG_OPTION_NON_VOLATILE,desired,NULL,&_hSubKey,&dwDisposition);
					if(m_nLastError != ERROR_SUCCESS)
					{
						m_sLastError = Environment::LastErrorText(m_nLastError);
						return false;
					}
				}
			}
			return m_nLastError == ERROR_SUCCESS;
		}

		void Registry::Close()
		{
			if(_hSubKey==NULL) return;

			::RegCloseKey(_hSubKey);
			_hSubKey = NULL;
			_sSubKey.Clear();
		}
		String Registry::QueryString(IN LPCTSTR name,IN LPCTSTR def)
		{
			assert(name!=NULL && *name!=0);
			IError::Clear();

			TCHAR dwValue[_MAX_PATH]={0};
			DWORD dwSize = sizeof(dwValue);
			DWORD dwType = REG_SZ;
			ZeroMemory(dwValue,dwSize);
			m_nLastError = (DWORD)::RegQueryValueEx(_hSubKey,name,NULL, &dwType, (LPBYTE)dwValue, &dwSize);
			if(m_nLastError != ERROR_SUCCESS)
			{
				m_sLastError = Environment::LastErrorText(m_nLastError);
				return def;
			}

			return dwValue;
		}
		UINT32 Registry::QueryUInt32(IN LPCTSTR name,IN UINT32 def)
		{
			assert(name!=NULL && *name!=0);
			IError::Clear();

			UINT32 dwValue=0;
			DWORD dwSize = sizeof(dwValue);
			DWORD dwType = REG_DWORD;
			m_nLastError = (DWORD)::RegQueryValueEx(_hSubKey,name,NULL, &dwType, (LPBYTE)&dwValue, &dwSize);
			if(m_nLastError != ERROR_SUCCESS)
			{
				m_sLastError = Environment::LastErrorText(m_nLastError);
				return def;
			}

			return dwValue;
		}
		UINT64 Registry::QueryUInt64(IN LPCTSTR name,IN UINT64 def)
		{
			assert(name!=NULL && *name!=0);
			IError::Clear();

			UINT64 dwValue=0;
			DWORD dwSize = sizeof(dwValue);
			DWORD dwType = REG_QWORD;
			m_nLastError = (DWORD)::RegQueryValueEx(_hSubKey,name,NULL, &dwType, (LPBYTE)&dwValue, &dwSize);
			if(m_nLastError != ERROR_SUCCESS)
			{
				m_sLastError = Environment::LastErrorText(m_nLastError);
				return def;
			}

			return dwValue;
		}
		bool Registry::Delete(IN LPCTSTR name,IN bool isRemoveKeyNotEmpty)
		{
			IError::Clear();
			if(name!=NULL)
				m_nLastError = (DWORD)::RegDeleteValue(_hSubKey,name);
			else if(!isRemoveKeyNotEmpty)
				m_nLastError = (DWORD)::RegDeleteKey(_hRootKey,_sSubKey);
			else
				m_nLastError = (DWORD)SHDeleteKey(_hRootKey,_sSubKey);

			if (m_nLastError!= ERROR_SUCCESS)
				m_sLastError = Environment::LastErrorText(m_nLastError);
			return m_nLastError == ERROR_SUCCESS;
		}
		bool Registry::SetString(IN LPCTSTR name,IN LPCTSTR data,IN bool isMulti)
		{
			IError::Clear();
			if(String::IsNullOrWhiteSpace(name))
			{
				IError::InvalidParameter("name");
				return false;
			}
			int nBytes = (_tcslen(data)+1)*sizeof(TCHAR);
			m_nLastError = (DWORD)::RegSetValueEx(_hSubKey,name,0,isMulti?REG_MULTI_SZ:REG_SZ,(BYTE*)data,nBytes);
			if (m_nLastError!= ERROR_SUCCESS)
				m_sLastError = Environment::LastErrorText(m_nLastError);

			return m_nLastError == ERROR_SUCCESS;
		}
		bool Registry::SetUInt32(IN LPCTSTR name,IN UINT32 data)
		{
			IError::Clear();
			if(String::IsNullOrWhiteSpace(name))
			{
				IError::InvalidParameter("name");
				return false;
			}

			m_nLastError = (DWORD)::RegSetValueEx(_hSubKey,name,0,REG_DWORD,(BYTE*)&data,sizeof(data));
			if (m_nLastError!= ERROR_SUCCESS)
				m_sLastError = Environment::LastErrorText(m_nLastError);

			return m_nLastError == ERROR_SUCCESS;
		}
		bool Registry::SetUInt64(IN LPCTSTR name,IN UINT64 data)
		{
			IError::Clear();
			if(String::IsNullOrWhiteSpace(name))
			{
				IError::InvalidParameter("name");
				return false;
			}

			m_nLastError = (DWORD)::RegSetValueEx(_hSubKey,name,0,REG_QWORD,(BYTE*)&data,sizeof(data));
			if (m_nLastError!= ERROR_SUCCESS)
				m_sLastError = Environment::LastErrorText(m_nLastError);

			return m_nLastError == ERROR_SUCCESS;
		}
		//////////////////////////////////////////////////////////////////////////////////////
		/////// internal
		void Registry::Clear()
		{
			_hRootKey = NULL;
			_hSubKey = NULL;
			_desired = KEY_QUERY_VALUE;
		}

	}
}
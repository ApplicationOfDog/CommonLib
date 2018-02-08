#include "stdafx.h"
#include "Ini.h"


namespace System
{
	namespace IO
	{
		Ini::Ini(String path)
		{
			Test::NotEmpty(path);
			Test::Exist(path,FALSE);
			_path=path;
		}
		
		String Ini::ReadString(IN String secName,IN String keyName,IN String def)
		{
			Test::NotEmpty(secName);
			Test::NotEmpty(keyName);

			TCHAR buffer[INI_MAX_BUFFER]={0};
			DWORD val = ::GetPrivateProfileString((LPCTSTR)secName,(LPCTSTR)keyName,(LPCTSTR)def,buffer,INI_MAX_BUFFER,(LPCTSTR)_path);
			return buffer;
		}
		String Ini::ReadString(IN String xpath,IN String def)
		{
			Test::NotEmpty(xpath);
			vector<String> parts=xpath.Split(_T('/'),SSO_RemoveEmptyEntries);
			String secName=parts[0];
			String keyName;
			if(parts.size()>1) keyName=parts[1];
			return ReadString(secName,keyName,def);
		}
		int Ini::ReadInt(IN String secName,IN String keyName,IN int def)
		{
			Test::NotEmpty(secName);
			Test::NotEmpty(keyName);
			return ::GetPrivateProfileInt((LPCTSTR)secName,(LPCTSTR)keyName,def,(LPCTSTR)_path);
		}
		int Ini::ReadInt(IN String xpath,IN int def)
		{
			Test::NotEmpty(xpath);
			vector<String> parts=xpath.Split(_T('/'),SSO_RemoveEmptyEntries);
			String secName=parts[0];
			String keyName;
			if(parts.size()>1) keyName=parts[1];
			return ReadInt(secName,keyName,def);
		}
	}
}


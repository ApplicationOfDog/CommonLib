#include "stdafx.h"
#include <objbase.h>
#include "GlobalUniqueID.hpp"

#define GUID_SEPARATOR_CH _T('-')
#define GUID_FORMAT_0 _T("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X")
#define GUID_FORMAT_1 _T("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X")


namespace System{
	namespace Data{

		GUID GlobalUniqueID::Null = {0x0};

		GlobalUniqueID::GlobalUniqueID(IN bool isCreate)
		{
			if(!isCreate ||!Create())
			{
				ZeroMemory(&_data,sizeof(_data));
			}
		}
		GlobalUniqueID::GlobalUniqueID(IN GUID& value)
		{
			_data = value;
		}
		GlobalUniqueID::operator GUID() const
		{
			return _data;
		}
		GlobalUniqueID& GlobalUniqueID::operator =(IN GUID& value)
		{
			_data = value;
			return *this;
		}
		bool GlobalUniqueID::operator ==(IN GUID& value)
		{
			return memcmp(&_data,&value,sizeof(_data)) == 0;
		}
		bool GlobalUniqueID::operator ==(IN GlobalUniqueID& value)
		{
			return memcmp(&_data,&(value._data),sizeof(_data)) == 0;
		}
		bool GlobalUniqueID::Create()
		{
			return CoCreateGuid(&_data) == S_OK;
		}
		String GlobalUniqueID::ToString()
		{
			return ConvertTo(_data,true);
		}

		bool GlobalUniqueID::FromString(IN LPCTSTR value)
		{
			return ConvertTo(value,_data);
		}
		bool GlobalUniqueID::IsNull()
		{
			return IsNull(_data);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// static

		String GlobalUniqueID::ConvertTo(IN GUID& source,IN bool isSeparator)
		{
			TCHAR strBuffer[65] = {0};
			LPCTSTR format = GUID_FORMAT_0;
			if(isSeparator) format = GUID_FORMAT_1;//有分隔符
			
			_sntprintf_s(strBuffer,64,format,  
				source.Data1,
				source.Data2,
				source.Data3,
				source.Data4[0], source.Data4[1],
				source.Data4[2], source.Data4[3], source.Data4[4], source.Data4[5], source.Data4[6], source.Data4[7]  
			); 
			return strBuffer;
		}
		bool GlobalUniqueID::ConvertTo(IN LPCTSTR source,OUT GUID& dest)
		{
			assert(source!=NULL && *source!=0);
			return System::Convert::ToByteArray(source,(PBYTE)&dest,sizeof(dest));
		}

		bool GlobalUniqueID::IsNull(IN GUID& source)
		{
			return memcmp(&source,&Null,sizeof(Null)) == 0;
		}
}}
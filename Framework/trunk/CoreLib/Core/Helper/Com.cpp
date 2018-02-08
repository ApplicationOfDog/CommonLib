#include "stdafx.h"
#include "../String.hpp"

namespace System
{
	void Output_Debug_String(LPCTSTR format,...)
	{
		va_list ap;
		va_start(ap,format);
		String content = String::Format(format,ap);		
		::OutputDebugString(content.c_str());
		va_end(ap);
	}
}
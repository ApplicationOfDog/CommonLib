#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用

namespace System{
	namespace Json {
	
		JSONCPP_NORETURN void throwRuntimeError(JSONCPP_STRING const& msg)
		{
			throw std::runtime_error(msg);
		}
		JSONCPP_NORETURN void throwLogicError(JSONCPP_STRING const& msg)
		{
			throw std::logic_error(msg);
		}

}}
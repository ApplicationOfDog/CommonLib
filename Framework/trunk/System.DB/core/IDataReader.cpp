#include "stdafx.h"
#include "IDataReader.hpp"

namespace System{
	namespace DB{

	IDataReader::IDataReader(IN void* pStmt)
	{
		_pStmt = pStmt;
	}


}}//end namespace System.DB
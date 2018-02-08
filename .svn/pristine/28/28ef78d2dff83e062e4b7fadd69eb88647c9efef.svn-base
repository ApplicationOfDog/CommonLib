#include "stdafx.h"
#include "SqliteStatement.hpp"

namespace System{
	namespace DB{

	SqliteStatement::SqliteStatement(IN TableStructure* structure,IN StatementType type,IN int multi_max)
		:ISqlStatement(structure,type)
	{
		_multi_max = multi_max;
	}
	SqliteStatement::SqliteStatement(IN LPCTSTR commandText)
		:ISqlStatement(commandText)
	{
	}
	////////////////////////////////////////////////////////////////////////////////////
	String SqliteStatement::convert_filedName(IN LPCTSTR fieldName,IN StatementPart statPart)
	{
		if(fieldName==NULL||*fieldName==0) return String::Empty;

		String actualName;
		if(statPart == STATEMENT_PART_CREATE_TABLE_FIELD)
			actualName.FormatStr(_T("\"%s\""),fieldName);
		else
			actualName.FormatStr(_T("[%s]"),fieldName);

		return actualName;
	}

}}
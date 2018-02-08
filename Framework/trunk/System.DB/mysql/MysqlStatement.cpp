#include "stdafx.h"
#include "MysqlStatement.hpp"

namespace System{
	namespace DB{

		MysqlStatement::MysqlStatement(IN TableStructure* structure,IN StatementType type,IN int multi_max)
			:ISqlStatement(structure,type)
		{
			_multi_max = multi_max;
		}
		MysqlStatement::MysqlStatement(IN LPCTSTR commandText)
			:ISqlStatement(commandText)
		{
		}

	////////////////////////////////////////////////////////////////////////////////////
	String MysqlStatement::convert_filedName(IN LPCTSTR fieldName,IN StatementPart statPart)
	{
		if(fieldName==NULL||*fieldName==0) return String::Empty;

		String actualName;
		if(statPart == STATEMENT_PART_CREATE_TABLE_FIELD)
		{
			actualName.FormatStr(_T("`%s`"),fieldName);
			return actualName;
		}
		else
		{
			bool isKeyword = false;
			LPCTSTR keywordArr[]={_T("Name"),_T("Group"),_T("Value")};
			for (int i=0;i<sizeof(keywordArr)/sizeof(keywordArr[0]);i++)
			{
				if(String::Compare(keywordArr[i],fieldName,true)!=0) continue;
				isKeyword = true;
				break;
			}
			if(isKeyword) actualName.FormatStr(_T("`%s`"),fieldName);
			else actualName = fieldName;
		}		
		return actualName;
	}

}}
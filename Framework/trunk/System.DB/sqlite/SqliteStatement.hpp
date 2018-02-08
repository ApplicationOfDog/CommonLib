#pragma once

namespace System{
	namespace DB{

	class SqliteStatement:public ISqlStatement
	{
	public:
		SqliteStatement(IN TableStructure* structure=NULL,IN StatementType type = ST_UNKNOWN,IN int multi_max = 0);
		SqliteStatement(IN LPCTSTR commandText);

	protected:
		///<summary>将目标字段转换成sql语句中使用的名称</summary>
		///<example>convert_filedName("OrderID") => "[OrderID]"</example>
		String convert_filedName(IN LPCTSTR fieldName,IN StatementPart statPart);
	};

}}
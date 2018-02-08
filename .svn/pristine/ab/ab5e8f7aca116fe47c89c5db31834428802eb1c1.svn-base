#pragma once

namespace System{
	namespace DB{

	class MysqlStatement:public ISqlStatement
	{
	public:
		MysqlStatement(IN TableStructure* structure=NULL,IN StatementType type = ST_UNKNOWN,IN int multi_max = 0);
		MysqlStatement(IN LPCTSTR commandText);

	protected:
		String convert_filedName(IN LPCTSTR filedName,IN StatementPart statPart);
	};

}}
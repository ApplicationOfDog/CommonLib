#include "stdafx.h"
#include "ISqlStatement.hpp"

namespace System{
	namespace DB{

		#define LINK_OP_AND _T("AND")
		#define LINK_OP_OR	_T("OR")
		#define SEPARATOR_STR _T(",")
		typedef vector<String> StringCollection;

		FieldValue::FieldValue(IN StatementPart part)
		{
			m_part = part;
			m_flags = 0x0;
		}
		FieldValue::FieldValue(IN LPCTSTR name,IN LPCTSTR value,IN StatementPart part)
		{
			m_name = name;
			m_value = value;
			m_part = part;
			m_flags = 0x0;
		}
		FieldValue& FieldValue::operator =(IN const FieldValue& v)
		{
			Copy(v);
			return *this;
		}
		void FieldValue::Clear()
		{
			m_name.Clear();
			m_value.Clear();
			m_part = STATEMENT_PART_AUTO;
			m_flags = 0x0;
		}
		void FieldValue::Copy(IN const FieldValue& v)
		{
			m_name = v.m_name;
			m_value = v.m_value;
			m_part = v.m_part;
		}

		/////////////////////////////////////////////////////////////////////////////////////


		ISqlStatement::ISqlStatement(IN TableStructure* structure,IN StatementType type)
		{
			Clear();
			_type = type;
			_pStructure = structure;
			_databaseType = DBT_SQLITE;
			_multi_max = 0;
		}
		ISqlStatement::ISqlStatement(IN LPCTSTR commandText)
		{
			Clear();
			_type = ST_COMMAND_TEXT;
			_pStructure = NULL;
			_commandText = commandText;
			_databaseType = DBT_SQLITE;
			_multi_max = 0;

		}
		ISqlStatement::~ISqlStatement()
		{
			Clear();
		}
		void ISqlStatement::Clear()
		{
			_type = ST_UNKNOWN;
			_properties = 0x0;
			_commandText.Clear();
			FieldColletion::iterator it = _fieldValues.begin();
			for(;it!=_fieldValues.end();it++)
			{
				if(*it==NULL) continue;
				delete *it;
			}
			_fieldValues.clear();
		}
		void ISqlStatement::Copy(IN ISqlStatement& sql,IN bool isStructure)
		{
			if(isStructure) _pStructure = sql._pStructure;
			
			_type = sql._type;
			_properties = sql._properties;
			_commandText = sql._commandText;

			FieldColletion::iterator it = _fieldValues.begin();
			for(;it!=_fieldValues.end();it++){
				if(*it==NULL) continue;
				delete *it;
			}
			_fieldValues.clear();
			_fieldValues.insert(_fieldValues.begin(),sql._fieldValues.begin(),sql._fieldValues.end());
		}
		void ISqlStatement::Structure(IN TableStructure* val)
		{ 
			_pStructure = val;
		}
		void ISqlStatement::Type(IN StatementType val)
		{ 
			_type = val;
		}
		void ISqlStatement::MultiMax(IN int val)
		{ 
			_multi_max = val;
		}
		int ISqlStatement::MultiMax()
		{
			return _multi_max;
		}
		bool ISqlStatement::IsEmpty()
		{ 
			return _fieldValues.size()==0; 
		}
		bool ISqlStatement::Is(IN StatementType type)
		{
			return _type == type; 
		}
		DWORD ISqlStatement::Properties()
		{ 
			return _properties; 
		}
		bool ISqlStatement::IsProperty(IN DWORD val)
		{ 
			return IS_FLAG_ON(_properties,val); 
		}
		LPCTSTR ISqlStatement::CommandText()
		{
			return _commandText;
		}
		ISqlStatement& ISqlStatement::Select(IN LPCTSTR fields)
		{
			Clear();
			_type = ST_SELECT;
			if(fields!=NULL && *fields!=0)
			{
				String fieldsTemp = fields;
				StringCollection fieldCollection = fieldsTemp.Split(SEPARATOR_STR,SSO_RemoveEmptyEntries);
				StringCollection::iterator it = fieldCollection.begin();
				for (;it!=fieldCollection.end();it++)
				{
					Field(*it,(LPCTSTR)NULL,STATEMENT_PART_SELECT_BACK);
				}
			}
			return *this;
		}
		ISqlStatement& ISqlStatement::Count(IN LPCTSTR whereFormat,...)
		{
			Clear();
			_type = ST_COUNT;
			if(whereFormat!=NULL && *whereFormat!=0)
			{
				va_list ap;
				va_start(ap,whereFormat);
				String whereTemp = String::Format(whereFormat,ap);
				va_end(ap);
				Where(whereTemp,true);
			}
			return *this;
		}
		ISqlStatement& ISqlStatement::Update(IN LPCTSTR setFormat,...)
		{
			Clear();
			_type = ST_UPDATE;
			if(setFormat!=NULL && *setFormat!=0)
			{
				FieldValue* fv = new FieldValue(STATEMENT_PART_UPDATE_SET_WHOLE);
				if(fv!=NULL)
				{
					va_list ap;
					va_start(ap,setFormat);
					fv->m_name.FormatStr(setFormat,ap);
					va_end(ap);
					_fieldValues.push_back(fv);
				}				
			}
			return *this;
		}
		ISqlStatement& ISqlStatement::Insert(IN bool isBackID,IN LPCTSTR fields,IN LPCTSTR valuesFormat,...)
		{
			Clear();
			_type = ST_INSERT;
			if(isBackID) SET_VAR_FLAG(_properties,STATEMENT_PROPERTY_INSERT_IS_BACK_AUTO_ID);
			if(fields!=NULL && *fields!=0 && valuesFormat!=NULL && *valuesFormat!=0)
			{
				va_list ap;
				va_start(ap,valuesFormat);
				String fieldsTemp = fields;
				String valuesTemp = String::Format(valuesFormat,ap);
				va_end(ap);

				StringCollection fCollection = fieldsTemp.Split(SEPARATOR_STR,SSO_RemoveEmptyEntries);
				StringCollection vCollection = valuesTemp.Split(SEPARATOR_STR,SSO_RemoveEmptyEntries);
				if(fCollection.size()!=vCollection.size())
				{
					assert("fields is not match values" && false);
					throw invalid_argument("valuesFormat");
				}

				StringCollection::iterator fit = fCollection.begin();
				StringCollection::iterator vit = vCollection.begin();
				for (;fit!=fCollection.end() && vit!=vCollection.end();fit++,vit++)
				{
					Field(*fit,vit->c_str(),STATEMENT_PART_INSERT_SINGLE);
				}
			}
			return *this;
		}
		ISqlStatement& ISqlStatement::Delete(IN LPCTSTR whereFormat,...)
		{
			Clear();
			_type = ST_DELETE;
			if(whereFormat!=NULL && *whereFormat!=0)
			{
				va_list ap;
				va_start(ap,whereFormat);
				String whereTemp = String::Format(whereFormat,ap);
				va_end(ap);
				Where(whereTemp,true);
			}
			return *this;
		}
		ISqlStatement& ISqlStatement::CreateTable(IN LPCTSTR tableName)
		{
			assert(tableName!=NULL && *tableName!=0);
			Clear();
			_type = ST_CREATE_TABLE;

			Field(tableName,(LPCTSTR)NULL,STATEMENT_PART_CREATE_TABLE);
			return *this;
		}

		ISqlStatement& ISqlStatement::WhereItem(IN LPCTSTR name,IN LPCTSTR value,IN bool isAndWithFront)
		{
			if(name!=NULL && *name!=0 && value!=NULL && *value!=0)
			{
				insert_field_value(name,value,isAndWithFront?STATEMENT_PART_WHERE_AND:STATEMENT_PART_WHERE_OR);
			}
			return *this;
		}
		ISqlStatement& ISqlStatement::WhereItem(IN LPCTSTR name,IN int value,IN bool isAndWithFront)
		{
			if(name!=NULL && *name!=0)
			{
				String valStr;
				valStr.FormatStr(_T("%d"),value);
				FieldValue* fv = new FieldValue(name,valStr,isAndWithFront?STATEMENT_PART_WHERE_AND:STATEMENT_PART_WHERE_OR);
				if(fv!=NULL) _fieldValues.push_back(fv);
			}			
			return *this;
		}		
		ISqlStatement& ISqlStatement::WhereItem(IN LPCTSTR name,IN UINT32 value,IN bool isAndWithFront)
		{
			if(name!=NULL && *name!=0)
			{
				String valStr;
				valStr.FormatStr(_T("%u"),value);
				FieldValue* fv = new FieldValue(name,valStr,isAndWithFront?STATEMENT_PART_WHERE_AND:STATEMENT_PART_WHERE_OR);
				if(fv!=NULL) _fieldValues.push_back(fv);
			}			
			return *this;
		}
		ISqlStatement& ISqlStatement::WhereItem(IN LPCTSTR name,IN INT64 value,IN bool isAndWithFront)
		{
			if(name!=NULL && *name!=0)
			{
				String valStr;
				valStr.FormatStr(_T("%I64d"),value);
				FieldValue* fv = new FieldValue(name,valStr,isAndWithFront?STATEMENT_PART_WHERE_AND:STATEMENT_PART_WHERE_OR);
				if(fv!=NULL) _fieldValues.push_back(fv);
			}			
			return *this;
		}
		ISqlStatement& ISqlStatement::WhereItem(IN LPCTSTR name,IN UINT64 value,IN bool isAndWithFront)
		{
			if(name!=NULL && *name!=0)
			{
				String valStr;
				valStr.FormatStr(_T("%I64u"),value);
				FieldValue* fv = new FieldValue(name,valStr,isAndWithFront?STATEMENT_PART_WHERE_AND:STATEMENT_PART_WHERE_OR);
				if(fv!=NULL) _fieldValues.push_back(fv);
			}			
			return *this;
		}
		ISqlStatement& ISqlStatement::WhereIn(IN LPCTSTR name,IN LPCTSTR values,IN bool isAndWithFront,IN bool isNot)
		{
			if(name!=NULL && *name!=0 && values!=NULL && *values!=0)
			{
				String actualVal;
				if(isNot) actualVal.Append(_T("NOT "));
				actualVal.AppendFormat(_T("IN (%s)"),values);
				insert_field_value(name,actualVal,isAndWithFront?STATEMENT_PART_WHERE_AND:STATEMENT_PART_WHERE_OR,STATEMENT_FIELD_INC_OPERATION);
			}
			return *this;
		}

		ISqlStatement& ISqlStatement::Where(IN LPCTSTR format,...)
		{
			if(format!=NULL && *format!=0)
			{
				va_list ap;
				va_start(ap,format);
				String whereTemp = String::Format(format,ap);
				va_end(ap);

				//去除where关键字
				whereTemp = remove_keyword(whereTemp,_T("WHERE"));
				FieldValue* fv = new FieldValue(whereTemp,LINK_OP_AND,STATEMENT_PART_WHERE_WHOLE);
				if(fv!=NULL) _fieldValues.push_back(fv);
			}
			return *this;
		}
		ISqlStatement& ISqlStatement::WhereBy(IN LPCTSTR text,IN bool isAndWithFront)
		{
			if(text!=NULL && *text!=0)
			{
				FieldValue* fv = new FieldValue(text,isAndWithFront?LINK_OP_AND:LINK_OP_OR,STATEMENT_PART_WHERE_WHOLE);
				if(fv!=NULL) _fieldValues.push_back(fv);
			}
			return *this;
		}		
		ISqlStatement& ISqlStatement::OrderBy(IN LPCTSTR field,IN bool isAsc)
		{
			if(field!=NULL && *field!=0)
			{
				Field(field,isAsc?_T("ASC"):_T("DESC"),STATEMENT_PART_SELECT_ORDER);
			}
			return *this;
		}
		ISqlStatement& ISqlStatement::OrderBy(IN LPCTSTR fields)
		{
			if(fields!=NULL && *fields!=0)
			{
				//去除ORDER BY关键字
				String orderTemp = remove_keyword(fields,_T("ORDER BY"));
				Field(orderTemp,NULL,STATEMENT_PART_SELECT_ORDER_WHOLE);
			}
			return *this;
		}
		ISqlStatement& ISqlStatement::Limit(IN int skip,IN int count)
		{
			String valStr;
			if(count>0) valStr.AppendFormat(_T("%d "),count);
			if(skip>0) valStr.AppendFormat(_T("offset %d "),skip);
			if(!valStr.IsEmpty())
			{
				Field(_T("limit"),(LPCTSTR)valStr,STATEMENT_PART_SELECT_LIMIT);
			}
			return *this;
		}
		String ISqlStatement::ToString(IN LPCTSTR tableName,OUT int* multi_left)
		{
			switch(_type)
			{
				case ST_COMMAND_TEXT:
					return _commandText;
				case ST_SELECT:
					_commandText = get_select(tableName,false);
					break;
				case ST_UPDATE:
					_commandText = get_update(tableName);
					break;
				case ST_INSERT:
					_commandText = get_insert(tableName,_multi_max,multi_left);
					break;
				case ST_DELETE:
					_commandText = get_delete(tableName);
					break;
				case ST_COUNT:
					_commandText = get_select(tableName,true);
					break;
				case ST_CREATE_TABLE:
					_commandText = get_create_table(tableName);
					break;
			}
			return _commandText;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		bool ISqlStatement::Field(IN String name,IN LPCTSTR value,IN StatementPart part)
		{
			return insert_field_value(name,value,part);
		}
		bool ISqlStatement::Field(IN String name,IN int value,IN StatementPart part)
		{
			String valStr;
			valStr.FormatStr(_T("%d"),value);
			return Field(name,valStr,part);
		}
		bool ISqlStatement::Field(IN String name,IN UINT32 value,IN StatementPart part)
		{
			String valStr;
			valStr.FormatStr(_T("%u"),value);
			return Field(name,valStr,part);
		}
		bool ISqlStatement::Field(IN String name,IN INT64 value,IN StatementPart part)
		{
			String valStr;
			valStr.FormatStr(_T("%I64d"),value);
			return Field(name,valStr,part);
		}
		bool ISqlStatement::Field(IN String name,IN UINT64 value,IN StatementPart part)
		{
			String valStr;
			valStr.FormatStr(_T("%I64u"),value);
			return Field(name,valStr,part);
		}
		bool ISqlStatement::Field(IN String name,IN double value,IN StatementPart part)
		{
			String valStr;
			valStr.FormatStr(_T("%lf"),value);
			return Field(name,valStr,part);
		}
		bool ISqlStatement::Field(IN String name,IN bool value,IN StatementPart part)
		{
			String valStr = value ? _T("1") :_T("0");
			return Field(name,valStr,part);
		}
		bool ISqlStatement::Field(IN String name,IN unsigned char* value,IN int len,IN StatementPart part)
		{
			if(value==NULL||len<=0) return false;
			String valStr=_T("X'");
			for (int i=0;i<len;i++)
			{
				valStr.AppendFormat(_T("%02x"),value[i]);
			}
			valStr.Append(_T("'"));
			return Field(name,valStr,part);
		}
		bool ISqlStatement::Field(IN String name,IN FieldDataType type,
			IN bool isNotNull,IN bool isPrimaryKey,IN bool isAutoIncrement,
			IN int length,IN int decimals)
		{
			//\"Cookie\"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL
			String valStr;
			String fieldDataType = TableField::Convert(type,length,decimals);
			valStr.AppendFormat(_T("%s "),(LPCTSTR)fieldDataType);
			if(isPrimaryKey) valStr.Append(_T("PRIMARY KEY "));
			if(isAutoIncrement) valStr.Append(_T("AUTOINCREMENT "));
			if(isNotNull) valStr.Append(_T("NOT NULL "));

			return Field(name,valStr,STATEMENT_PART_CREATE_TABLE_FIELD);
		}
		void ISqlStatement::CombineField()
		{
			assert(_type == ST_INSERT);
			if(_fieldValues.size()==0) return;

			FieldValue* pf = NULL;
			FieldValue* npf = new FieldValue(STATEMENT_PART_INSERT_MULTI);
			if(npf==NULL)
			{
				assert("alloc memory failed" && false);
				return;
			}

			String lastMultiName;			
			FieldColletion::iterator it = _fieldValues.begin();
			while(it != _fieldValues.end())
			{
				pf = *it;
				if(pf->m_part == STATEMENT_PART_INSERT_SINGLE)
				{
					if(!npf->m_name.IsEmpty()) npf->m_name.Append(SEPARATOR_STR);
					String actualFieldName = convert_filedName(pf->m_name,pf->m_part);
					npf->m_name.Append(actualFieldName);

					if(!npf->m_value.IsEmpty()) npf->m_value.Append(SEPARATOR_STR);
					npf->m_value.Append(pf->m_value);

					delete pf;
					it=_fieldValues.erase(it);//删除STATEMENT_PART_INSERT_SINGLE项
					continue;
				}
				if(pf->m_part == STATEMENT_PART_INSERT_MULTI && lastMultiName.IsEmpty()){
					lastMultiName = pf->m_name;
				}
				it++;
			}
			if(npf->m_value.IsEmptyOrWhiteSpace()||npf->m_name.IsEmptyOrWhiteSpace()){
				throw invalid_argument("value or field list is empty");
			}
			if(!lastMultiName.IsEmpty() && lastMultiName.Compare(npf->m_name,true)!=0){//之前已经存在合并记录
				throw logic_error("current fields is different from the combine records");//之前的合并记录中的字段列表与当前字段列表不同
			}
			if(!npf->m_name.IsEmpty())
				_fieldValues.push_back(npf);
			else
			{
				delete npf;
				npf = NULL;
			}
			//Field(fields,values,STATEMENT_PART_INSERT_MULTI);
			SET_VAR_FLAG(_properties,STATEMENT_PROPERTY_INSERT_IS_MULTI);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		String ISqlStatement::get_select(IN LPCTSTR tableName,IN bool isCount)
		{
			String tableNameActual = get_table_name(tableName);
			if(tableNameActual.IsEmpty()) return String::Empty;

			String backFields,limits,actualFieldName;
			if(!isCount)
			{
				FieldValue* pf = NULL;
				FieldColletion::iterator it = _fieldValues.begin();
				for (;it!=_fieldValues.end();it++)
				{
					pf = *it;
					switch(pf->m_part)
					{
					case STATEMENT_PART_SELECT_BACK:
						if(pf->m_name.Compare(_T("*"),false)==0) continue;
						if(!backFields.IsEmpty()) backFields.Append(SEPARATOR_STR);

						actualFieldName = convert_filedName(pf->m_name,pf->m_part);
						backFields.Append(actualFieldName);
						break;

					case STATEMENT_PART_SELECT_LIMIT:
						limits = pf->m_value;
						break;
					}
				}
				if(backFields.IsEmptyOrWhiteSpace())
					backFields = _T("*");
			}
			else
			{
				backFields = _T("COUNT(*)");
			}
			
			String commandText,whereFields = get_where_part(),orderBy = get_order_part();
			commandText.FormatStr(_T("SELECT %s FROM %s "),(LPCTSTR)backFields,(LPCTSTR)tableNameActual);
			if(!whereFields.IsEmptyOrWhiteSpace())
				commandText.AppendFormat(_T("WHERE %s "),(LPCTSTR)whereFields);
			if(!orderBy.IsEmptyOrWhiteSpace())
				commandText.AppendFormat(_T("ORDER BY %s "),(LPCTSTR)orderBy);
			if(!limits.IsEmptyOrWhiteSpace())
				commandText.AppendFormat(_T("LIMIT %s "),(LPCTSTR)limits);
			return commandText;
		}

		String ISqlStatement::get_update(IN LPCTSTR tableName)
		{
			String tableNameActual = get_table_name(tableName);
			if(tableNameActual.IsEmpty()) return String::Empty;

			FieldValue* pf = NULL;
			String setFields,actualFieldName;
			FieldColletion::iterator it = _fieldValues.begin();
			for (;it!=_fieldValues.end();it++)
			{
				pf = *it;
				if(pf->m_part == STATEMENT_PART_UPDATE_SET)
				{
					if(!setFields.IsEmpty()) setFields.Append(SEPARATOR_STR);

					actualFieldName = convert_filedName(pf->m_name,pf->m_part);
					setFields.AppendFormat(_T("%s=%s"),(LPCTSTR)actualFieldName,(LPCTSTR)pf->m_value);
					continue;
				}
				if(pf->m_part == STATEMENT_PART_UPDATE_INCREAMENT)
				{
					if(!setFields.IsEmpty()) setFields.Append(SEPARATOR_STR);

					actualFieldName = convert_filedName(pf->m_name,pf->m_part);
					setFields.AppendFormat(_T("%s = %s + (%s)"),(LPCTSTR)actualFieldName,(LPCTSTR)actualFieldName,(LPCTSTR)pf->m_value);
					continue;
				}
				if(pf->m_part == STATEMENT_PART_UPDATE_SET_WHOLE)
				{
					setFields = pf->m_name;
					break;
				}
			}
			
			String commandText,whereFields = get_where_part();
			commandText.FormatStr(_T("UPDATE %s SET %s "),(LPCTSTR)tableNameActual,(LPCTSTR)setFields);
			if(!whereFields.IsEmptyOrWhiteSpace())
				commandText.AppendFormat(_T("WHERE %s "),(LPCTSTR)whereFields);
			return commandText;
		}
		String ISqlStatement::get_insert(IN LPCTSTR tableName,IN int multi_max,IN int* multi_left)
		{
			if(multi_left!=NULL) *multi_left = 0;

			int multi_count = 0;
			String tableNameActual = get_table_name(tableName);
			if(tableNameActual.IsEmpty()) return String::Empty;

			FieldValue* pf = NULL;
			String fields,values,commandText,actualFieldName;
			FieldColletion::iterator it = _fieldValues.begin();
			for (;it!=_fieldValues.end();it++)
			{
				pf = *it;
				if(pf->m_part == STATEMENT_PART_INSERT_SINGLE && !IsProperty(STATEMENT_PROPERTY_INSERT_IS_MULTI))
				{
					if(!fields.IsEmpty()) fields.Append(SEPARATOR_STR);

					actualFieldName = convert_filedName(pf->m_name,pf->m_part);
					fields.Append(actualFieldName);

					if(!values.IsEmpty()) values.Append(SEPARATOR_STR);
					values.Append(pf->m_value);
					continue;
				}
				if (pf->m_part == STATEMENT_PART_INSERT_MULTI && IsProperty(STATEMENT_PROPERTY_INSERT_IS_MULTI))
				{
					if(multi_max>0 && IS_FLAG_ON(pf->m_flags,STATEMENT_FIELD_USEABLE)) continue;
					if(multi_max<=0 || multi_max>0 && multi_count<multi_max)
					{
						if(fields.IsEmptyOrWhiteSpace()) fields = pf->m_name;

						if(!values.IsEmpty()) values.AppendFormat(_T(",\n"));
							values.AppendFormat(_T("(%s)"),(LPCTSTR)pf->m_value);

						if(multi_max>0)
						{
							SET_VAR_FLAG(pf->m_flags,STATEMENT_FIELD_USEABLE);
							multi_count++;
						}
						continue;
					}
					if(multi_left!=NULL && multi_max>0)
						*multi_left = *multi_left+1;
				}
			}

			commandText.FormatStr(_T("INSERT INTO %s (%s) VALUES "),(LPCTSTR)tableNameActual,(LPCTSTR)fields);

			if(!IsProperty(STATEMENT_PROPERTY_INSERT_IS_MULTI))
				commandText.AppendFormat(_T("(%s)"),(LPCTSTR)values);
			else
				commandText.Append(values);

			return commandText;
		}
		String ISqlStatement::get_delete(IN LPCTSTR tableName)
		{
			String tableNameActual = get_table_name(tableName);
			if(tableNameActual.IsEmpty()) return String::Empty;

			String commandText,whereFields = get_where_part();
			commandText.FormatStr(_T("DELETE FROM %s "),(LPCTSTR)tableNameActual);
			if(!whereFields.IsEmptyOrWhiteSpace())
				commandText.AppendFormat(_T("WHERE %s "),(LPCTSTR)whereFields);
			return commandText;
		}		
		String ISqlStatement::get_create_table(IN LPCTSTR tableName)
		{
			String tableNameActual;
			if(tableName!=NULL && *tableName!=0) tableNameActual = tableName;
			
			String commandText;
			FieldValue* pf = NULL;
			String fields,actualFieldName;
			FieldColletion::iterator it = _fieldValues.begin();
			for (;it!=_fieldValues.end();it++)
			{
				pf = *it;
				if(pf->m_part == STATEMENT_PART_CREATE_TABLE)
				{
					if(tableNameActual.IsEmptyOrWhiteSpace())
						tableNameActual = pf->m_name;
					continue;
				}
				if(pf->m_part == STATEMENT_PART_CREATE_TABLE_FIELD)
				{
					actualFieldName = convert_filedName(pf->m_name,pf->m_part);
					fields.AppendFormat(_T(" %s %s,\n"),(LPCTSTR)actualFieldName,(LPCTSTR)pf->m_value);
				}
			}
			fields.TrimEnd(_T("\n,"));

			commandText.AppendFormat(_T("CREATE TABLE IF NOT EXISTS \"%s\"( \n"),(LPCTSTR)tableNameActual);
			commandText.Append(fields);
			commandText.Append(_T("\n"));
			commandText.Append(_T(")"));
			return commandText;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		String ISqlStatement::get_table_name(IN LPCTSTR tableName)
		{
			if(tableName!=NULL && *tableName!=0) return tableName;
			if(_pStructure!=NULL && _pStructure->IsValid()) return _pStructure->m_name;

			assert("empty table name" && false);
			return String::Empty;
		}
		String ISqlStatement::get_where_part()
		{
			FieldValue* pf = NULL;
			String whereFields,actualFileName;
			FieldColletion::iterator it = _fieldValues.begin();
			for (;it!=_fieldValues.end();it++)
			{
				pf = *it;
				if(pf->m_part == STATEMENT_PART_WHERE_AND || pf->m_part == STATEMENT_PART_WHERE_OR)
				{
					bool isAnd = pf->m_part == STATEMENT_PART_WHERE_AND;
					if(!whereFields.IsEmpty()) whereFields.AppendFormat(_T(" %s "),isAnd?LINK_OP_AND:LINK_OP_OR);

					actualFileName = convert_filedName(pf->m_name,pf->m_part);
					if(FLAG_ON(pf->m_flags,STATEMENT_FIELD_INC_OPERATION))//值当前已经包含操作符了
						whereFields.AppendFormat(_T("%s %s"),(LPCTSTR)actualFileName,(LPCTSTR)pf->m_value);
					else
						whereFields.AppendFormat(_T("%s = %s"),(LPCTSTR)actualFileName,(LPCTSTR)pf->m_value);
					continue;
				}
				if(pf->m_part == STATEMENT_PART_WHERE_WHOLE)
				{
					if(!whereFields.IsEmpty())
					{
						if(pf->m_value.Compare(LINK_OP_AND,true)==0 || pf->m_value.Compare(LINK_OP_OR,true)==0)
							whereFields.AppendFormat(_T(" %s "),(LPCTSTR)pf->m_value);
						else
							whereFields.AppendFormat(_T(" %s "),LINK_OP_AND);
					}
					whereFields.AppendFormat(_T("(%s)"),(LPCTSTR)pf->m_name);
					continue;
				}
			}
			return whereFields;
		}
		String ISqlStatement::get_order_part()
		{
			FieldValue* pf = NULL;
			String orderFields,actualFileName;
			FieldColletion::iterator it = _fieldValues.begin();
			for (;it!=_fieldValues.end();it++)
			{
				pf = *it;
				if(pf->m_part == STATEMENT_PART_SELECT_ORDER)
				{
					if(!orderFields.IsEmpty()) orderFields.Append(SEPARATOR_STR);

					actualFileName = convert_filedName(pf->m_name,pf->m_part);
					orderFields.AppendFormat(_T("%s %s"),(LPCTSTR)actualFileName,(LPCTSTR)pf->m_value);
					continue;
				}
				if(pf->m_part == STATEMENT_PART_SELECT_ORDER_WHOLE)
				{
					if(!orderFields.IsEmpty()) orderFields.Append(SEPARATOR_STR);
					orderFields.Append(pf->m_name);
					continue;
				}
			}
			return orderFields;
		}
		String ISqlStatement::remove_keyword(IN LPCTSTR text,IN LPCTSTR keyword)
		{
			assert(keyword!=NULL && *keyword!=0);
			if(text==NULL||*text==0) return String::Empty;

			String textTemp = text;
			String keywordTemp = keyword;
			textTemp.Trim();

			//去除关键字			
			int index = textTemp.IndexOf(keywordTemp,0,true);
			if(index>=0) textTemp = textTemp.Substring(index+keywordTemp.GetLength());
			return textTemp;
		}
		String ISqlStatement::get_actual_value(IN String& name,IN LPCTSTR value,IN bool isFieldCheck)
		{
			if(_pStructure==NULL||!_pStructure->IsValid()) return value;

			String actualVal;
			TableField* pFiled = _pStructure->FindField(name);
			if(pFiled==NULL && isFieldCheck)
			{
				throw runtime_error("invalid field name");
				return String::Empty;
			}

			if(value!=NULL)
			{
				if(pFiled!=NULL && (pFiled->Is(FDT_TEXT)||pFiled->Is(FDT_DATETIME)))
				{
					actualVal.Append(_T('\''),1);
					if(*value!=0)
					{
						String valStrTemp = value;
						valStrTemp.Replace(_T("'"),_T("&quot;"));
						actualVal.Append(valStrTemp);
					}
					actualVal.Append(_T('\''),1);
				}
				else
					actualVal = value;
			}
			else if(pFiled!=NULL && !pFiled->m_isNotNull)
			{
				actualVal = _T("NULL");
			}
			return actualVal;
		}
		bool ISqlStatement::insert_field_value(IN String name,IN LPCTSTR value,IN StatementPart part,IN DWORD flags)
		{
			if(_type == ST_UNKNOWN)
			{
				assert(false);
				return false;
			}
			if(name.IsEmptyOrWhiteSpace())
			{
				assert("invalid parameter 'name'" && false);
				return false;
			}

			bool isFieldCheck = true;
			TableField* pFiled = NULL;
			String actualVal,nameAcutal = String::Trim(name,NULL);
			
			if(part == STATEMENT_PART_SELECT_BACK && nameAcutal==_T("*"))
				isFieldCheck = false;

			if(_type == ST_CREATE_TABLE)
				actualVal = value;
			else if(part == STATEMENT_PART_SELECT_LIMIT||part == STATEMENT_PART_SELECT_ORDER||part == STATEMENT_PART_SELECT_ORDER_WHOLE)
				actualVal = value;
			else if(_type == ST_INSERT && part == STATEMENT_PART_INSERT_MULTI)
				actualVal = value;
			else 
				actualVal = get_actual_value(nameAcutal,value,isFieldCheck);

			if(part == STATEMENT_PART_AUTO)
			{
				switch(_type)
				{
				case ST_SELECT:
					if(value==NULL) part = STATEMENT_PART_SELECT_BACK;
					else if(pFiled!=NULL && !pFiled->Is(FDT_TEXT) && *value==0)
						part = STATEMENT_PART_SELECT_BACK;
					else
						part = *value==0 ? STATEMENT_PART_SELECT_BACK:STATEMENT_PART_WHERE_AND;
					break;
					break;
				case ST_UPDATE:part = STATEMENT_PART_UPDATE_SET;break;
				case ST_INSERT:part=STATEMENT_PART_INSERT_SINGLE;break;
				case ST_DELETE:part=STATEMENT_PART_WHERE_AND;break;
				case ST_CREATE_TABLE:part=STATEMENT_PART_CREATE_TABLE_FIELD;break;
				}
			}
			FieldValue* fv = new FieldValue(nameAcutal,actualVal,part);
			if(fv!=NULL)
			{
				fv->m_flags = flags;
				_fieldValues.push_back(fv);
			}
			return true;
		}

}}//end namespace
#include "stdafx.h"
#include "TableStructure.hpp"

#define DT_TEXT_INT			_T("INT")
#define DT_TEXT_INTEGER		_T("INTEGER")
#define DT_TEXT_BIGINT		_T("BIGINT")

#define DT_TEXT_REAL		_T("REAL")
#define DT_TEXT_BLOB		_T("BLOB")

#define DT_TEXT_CHAR		_T("CHAR")
#define DT_TEXT_VARCHAR		_T("VARCHAR")
#define DT_TEXT_TEXT		_T("TEXT")
#define DT_TEXT_MEDIUMTEXT	_T("MEDIUMTEXT")
#define DT_TEXT_LONGTEXT	_T("LONGTEXT")
#define DT_TEXT_NCHAR		_T("NCHAR")
#define DT_TEXT_NVARCHAR	_T("NVARCHAR")
#define DT_TEXT_NTEXT		_T("NTEXT")
#define DT_DATETIME			_T("datetime")

namespace System{
	namespace DB{

	TableField::TableField()
	{
		Clear();
	}
	void TableField::Clear()
	{
		m_index = -1;
		m_name.Clear();
		m_dataType = FDT_UNKNOWN;
		m_dataLength = 0;
		m_dataDecimals = 0;
		m_sDataType.Clear();
		m_isNotNull = false;
		m_isPrimaryKey = false;
		m_isAutoIncrement = false;
	}
	void TableField::DataType(IN LPCTSTR type)
	{
		assert(type!=NULL && *type!=0);
		
		m_dataType = FDT_UNKNOWN;
		m_dataLength = 0;
		m_dataDecimals = 0;
		m_sDataType = type;
		Convert(m_sDataType,&m_dataType,&m_dataLength,&m_dataDecimals);
	}	
	bool TableField::Convert(IN PCTSTR src,OUT FieldDataType* dest,OUT int* len,OUT int* decimal)
	{
		static map<String,FieldDataType> types;
		if(types.size()==0)
		{
			types.insert(make_pair(DT_TEXT_INT,FDT_INTEGER));
			types.insert(make_pair(DT_TEXT_INTEGER,FDT_INTEGER));
			types.insert(make_pair(DT_TEXT_BIGINT,FDT_INTEGER));

			types.insert(make_pair(DT_TEXT_REAL,FDT_REAL));
			types.insert(make_pair(DT_TEXT_BLOB,FDT_BLOB));

			types.insert(make_pair(DT_TEXT_CHAR,FDT_TEXT));
			types.insert(make_pair(DT_TEXT_VARCHAR,FDT_TEXT));
			types.insert(make_pair(DT_TEXT_TEXT,FDT_TEXT));
			types.insert(make_pair(DT_TEXT_MEDIUMTEXT,FDT_TEXT));
			types.insert(make_pair(DT_TEXT_LONGTEXT,FDT_TEXT));
			types.insert(make_pair(DT_TEXT_NCHAR,FDT_TEXT));
			types.insert(make_pair(DT_TEXT_NVARCHAR,FDT_TEXT));
			types.insert(make_pair(DT_TEXT_NTEXT,FDT_TEXT));

			types.insert(make_pair(DT_DATETIME,FDT_DATETIME));
		}
		int lenTemp=0,decimalTemp =0;
		map<String,FieldDataType>::iterator it = types.begin();
		for (;it!=types.end();it++)
		{
			if(!do_analyze(src,it->first,&lenTemp,&decimalTemp)) continue;
			if(dest!=NULL) *dest = it->second;

			if(it->first == DT_TEXT_BIGINT && lenTemp<=0)
				lenTemp = 8;
			
			if(len!=NULL) *len = lenTemp;
			if(decimal!=NULL) *decimal = decimalTemp;
			return true;
		}
		return false;
	}
	String TableField::Convert(IN FieldDataType src,IN int len,IN int decimal)
	{
		assert(src!=FDT_UNKNOWN);
		
		String text;
		switch(src)
		{
		case FDT_INTEGER:text = DT_TEXT_INTEGER;break;
		case FDT_BLOB:text = DT_TEXT_BLOB;break;
		case FDT_DATETIME:text = DT_DATETIME;break;
		case FDT_TEXT:
			text = DT_TEXT_TEXT;
			if(len>0) text.AppendFormat(_T("(%d)"),len);
			break;
		case FDT_REAL:
			text = DT_TEXT_REAL;
			if(len>0||decimal>0)
			{
				text.Append(_T("("));
				if(len>0) text.AppendFormat(_T("%d"),len);
				if(decimal>0)
				{
					if(len>0) text.Append(_T(","));
					text.AppendFormat(_T("%d"),decimal);
				}
				text.Append(_T(")"));
			}
			break;
		}
		return text;
	}
	bool TableField::Is(IN FieldDataType val)
	{
		return m_dataType == val;
	}
	bool TableField::do_analyze(IN LPCTSTR srcText,IN LPCTSTR matchType,OUT int* len,OUT int* decimal)
	{
		if(len!=NULL) *len = 0;
		if(decimal!=NULL) *decimal = 0;

		if(srcText==NULL||*srcText==0) return false;
		if(matchType==NULL||*matchType==0) return false;

		String srcTxtTmp = srcText,typeTxtTmp = matchType;
		if(srcTxtTmp.IndexOf(typeTxtTmp,true)<0) return false;

		int lenType = typeTxtTmp.GetLength();
		if(srcTxtTmp.GetLength()<=lenType) return true;

		int lenTemp=0,decimalTemp=0;
		String infoTxt = srcTxtTmp.Substring(lenType);
		infoTxt.Trim(_T("() "));
		if(!infoTxt.IsEmptyOrWhiteSpace())
		{
			TCHAR ch = infoTxt[0];
			if(!Char::IsDigit(ch)) return false;//第一个字符不是数字那说不是该类型的

			vector<String> nums = infoTxt.Split(_T(','),SSO_RemoveEmptyEntries);
			if(nums.size()>0) lenTemp = System::Convert::ToIntByDef(nums[0],0);
			if(nums.size()>1) decimalTemp =System::Convert::ToIntByDef(nums[1],0);
		}
		if(len!=NULL) *len = lenTemp;
		if(decimal!=NULL) *decimal = decimalTemp;
		return true;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	TableStructure::TableStructure(IN LPCTSTR name)
	{
		if(name!=NULL && *name!=0)
			m_name = name;
	}
	TableStructure::~TableStructure()
	{
		m_name.Clear();
		Clear();
	}
	void TableStructure::Clear()
	{
		CLEAR_MEMORY_IN_VECTOR(TableField*,m_fields);
		m_createTableSql.Clear();
	}
	bool TableStructure::IsValid()
	{ 
		return !m_name.IsEmptyOrWhiteSpace() && m_fields.size()>0; 
	}
	bool TableStructure::HasField(IN LPCTSTR name)
	{
		assert(name!=NULL && *name!=0);
		return FindField(name)!=NULL;
	}

	TableField* TableStructure::FindField(IN LPCTSTR name)
	{
		assert(name!=NULL && *name!=0);
		vector<TableField*>::iterator it = m_fields.begin();
		for (;it!=m_fields.end();it++)
		{
			if((*it)->m_name.Compare(name,false)!=0) continue;
			return *it;
		}
		return NULL;
	}
	TableField* TableStructure::FindField(IN int index)
	{
		assert(index>=0);
		vector<TableField*>::iterator it = m_fields.begin();
		for (;it!=m_fields.end();it++)
		{
			if((*it)->m_index!=index) continue;
			return *it;
		}
		return NULL;
	}

}}//end namespace
#include "stdafx.h"
#include "inc/sqlite3.h"
#include "SqliteDataReader.hpp"

#undef PSTMT
#define PSTMT ((sqlite3_stmt*)_pStmt)

namespace System{
	namespace DB{

	SqliteDataReader::SqliteDataReader(IN void* pStmt,IN bool isBufferName):IDataReader(pStmt)
	{
		if(isBufferName)
		{
			std::string colName;
			for (int i=0;i<ColumnCount();i++)
			{
				colName = GetNameA(i);
				m_columns.insert(make_pair(colName,i));
			}
		}		
	}
	SqliteDataReader::~SqliteDataReader(){}  

	// 读取一行数据   
	bool SqliteDataReader::Read()
	{  
		if(_pStmt == NULL) return false;

		int result = sqlite3_step(PSTMT);
		return result == SQLITE_ROW;
	}  

	// 关闭Reader，读取结束后调用   
	void SqliteDataReader::Close()  
	{
		if(_pStmt==NULL) return;

		int result = sqlite3_finalize(PSTMT);
		_pStmt = NULL;  
	}  

	// 总的列数   
	int SqliteDataReader::ColumnCount()  
	{  
		return sqlite3_column_count(PSTMT);  
	}  

	// 获取某列的名称    
	LPCTSTR SqliteDataReader::GetName(IN int nCol)  
	{  
	#ifdef  _UNICODE    
		return (LPCTSTR)sqlite3_column_name16(PSTMT, nCol);  
	#else   
		return (LPCTSTR)sqlite3_column_name(PSTMT, nCol);  
	#endif   
	}
	LPCSTR SqliteDataReader::GetNameA(IN int nCol){
		return (LPCSTR)sqlite3_column_name(PSTMT, nCol);  
	}

	// 获取某列的数据类型   
	FieldDataType SqliteDataReader::GetDataType(IN int nCol)  
	{  
		return (FieldDataType)sqlite3_column_type(PSTMT, nCol);  
	}  

	// 获取某列的值(字符串)   
	String SqliteDataReader::GetString(IN int nCol)  
	{  
	#ifdef _UNICODE    
		return (LPCTSTR)sqlite3_column_text16(PSTMT, nCol);  
	#else
		return (LPCTSTR)sqlite3_column_text(PSTMT, nCol);  
	#endif   
	}
	String SqliteDataReader::GetString(IN LPCSTR name,IN LPCTSTR def)  
	{
		assert(name!=NULL && *name!=0 && m_columns.size()>0);
		NameIndexMap::iterator it = m_columns.find(name);
		if(it==m_columns.end()) return def;
		return GetString(it->second);
	}
#ifdef _UNICODE  
	string SqliteDataReader::GetStringA(IN LPCSTR name,IN LPCSTR def)  
	{
		assert(name!=NULL && *name!=0 && m_columns.size()>0);
		NameIndexMap::iterator it = m_columns.find(name);
		if(it==m_columns.end()) return def;

		LPCTSTR text = GetString(it->second);
		return Convert::ToStringA(text,IO::EC_ANSI);
	}
#endif
	
	// 获取某列的值(整形)   
	int SqliteDataReader::GetInt(IN int nCol)  
	{  
		return sqlite3_column_int(PSTMT, nCol);  
	}  
	int SqliteDataReader::GetInt(IN LPCSTR name,IN int def)  
	{
		assert(name!=NULL && *name!=0 && m_columns.size()>0);
		NameIndexMap::iterator it = m_columns.find(name);
		if(it==m_columns.end()) return def;
		return GetInt(it->second);
	}
	bool SqliteDataReader::GetBool(IN int nCol)  
	{  
		return sqlite3_column_int(PSTMT, nCol)==1;  
	}  
	bool SqliteDataReader::GetBool(IN LPCSTR name,IN bool def)  
	{
		assert(name!=NULL && *name!=0 && m_columns.size()>0);
		NameIndexMap::iterator it = m_columns.find(name);
		if(it==m_columns.end()) return def;
		return GetInt(it->second)==1;
	}
	// 获取某列的值(长整形)   
	INT64 SqliteDataReader::GetInt64(IN int nCol)  
	{  
		return (INT64)sqlite3_column_int64(PSTMT, nCol);  
	}  
	INT64 SqliteDataReader::GetInt64(IN LPCSTR name,IN INT64 def)  
	{
		assert(name!=NULL && *name!=0 && m_columns.size()>0);
		NameIndexMap::iterator it = m_columns.find(name);
		if(it==m_columns.end()) return def;
		return GetInt64(it->second);
	}
	// 获取某列的值(浮点形)   
	double SqliteDataReader::GetFloat(IN int nCol)  
	{  
		return sqlite3_column_double(PSTMT, nCol);  
	}  
	double SqliteDataReader::GetFloat(IN LPCSTR name,IN double def)  
	{
		assert(name!=NULL && *name!=0 && m_columns.size()>0);
		NameIndexMap::iterator it = m_columns.find(name);
		if(it==m_columns.end()) return def;
		return GetFloat(it->second);
	}
	// 获取某列的值(二进制数据)   
	const BYTE* SqliteDataReader::GetBlob(IN int nCol,IN int &nLen)  
	{  
		nLen = sqlite3_column_bytes(PSTMT, nCol);  
		return (const BYTE*)sqlite3_column_blob(PSTMT, nCol);  
	}
	const BYTE* SqliteDataReader::GetBlob(IN LPCSTR name,IN int &nLen)  
	{  
		NameIndexMap::iterator it = m_columns.find(name);
		if(it==m_columns.end()) return NULL;

		nLen = sqlite3_column_bytes(PSTMT, it->second);  
		return (const BYTE*)sqlite3_column_blob(PSTMT, it->second);  
	}
}}//end namespace System.DB
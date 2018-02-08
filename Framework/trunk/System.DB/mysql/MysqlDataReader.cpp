#include "stdafx.h"
#include <WinSock.h>
#include "inc/mysql.h"
#include "MysqlDataReader.hpp"
#include "MysqlDll.hpp"
#include "MysqlDB.hpp"

#undef PSTMT
#define PSTMT	((MYSQL_RES*)_pStmt)
#define PROW	((MYSQL_ROW)_row)
#undef PDLL
#define PDLL	((MysqlDll*)_pDB->m_pDll)

namespace System{
	namespace DB{

	MysqlDataReader::MysqlDataReader(IN void* pStmt,IN MysqlDB* pDB,IN bool isBufferName):IDataReader(pStmt)
	{
		_row = NULL;
		_pDB = pDB;
		if(isBufferName)
		{
			int index = 0;
			MYSQL_FIELD* fd = NULL;
			while(fd = PDLL->fetch_field(PSTMT))
			{
				m_columns.insert(make_pair(fd->name,index));
				index++;
			}
		}
	}
	MysqlDataReader::~MysqlDataReader(){}  

	// 读取一行数据   
	bool MysqlDataReader::Read()
	{  
		if(_pStmt == NULL) return false;

		_row = PDLL->fetch_row(PSTMT);
		return _row != NULL;
	}  

	// 关闭Reader，读取结束后调用
	void MysqlDataReader::Close()  
	{
		if(_pStmt==NULL) return;

		PDLL->free_result(PSTMT);
		_pStmt = NULL;  
	}  

	// 总的列数   
	int MysqlDataReader::ColumnCount()  
	{  
		return (int)PDLL->num_rows(PSTMT) ;
	}  
	// 获取某列的名称
	LPCSTR MysqlDataReader::GetNameA(IN int nCol)
	{
		NameIndexMap::iterator it = m_columns.begin();
		for (;it!=m_columns.end();it++)
		{
			if(it->second != nCol) continue;
			return it->first.c_str();
		}
		return NULL;
	}

	// 获取某列的值(字符串)   
	String MysqlDataReader::GetString(IN int nCol)  
	{
		char* valStr = _row[nCol];
		if(valStr==NULL) return String::Empty;
		return Convert::ToString(valStr,IO::EC_UTF8/*IO::EC_ANSI*/);
	}
	String MysqlDataReader::GetString(IN LPCSTR name,IN LPCTSTR def)  
	{
		assert(name!=NULL && *name!=0 && m_columns.size()>0);
		NameIndexMap::iterator it = m_columns.find(name);
		if(it==m_columns.end()) return def;
		return GetString(it->second);
	}
#ifdef _UNICODE  
	string MysqlDataReader::GetStringA(IN LPCSTR name,IN LPCSTR def)  
	{
		assert(name!=NULL && *name!=0 && m_columns.size()>0);
		NameIndexMap::iterator it = m_columns.find(name);
		if(it==m_columns.end()) return def;

		return _row[it->second];
	}
#endif
	
	// 获取某列的值(整形)   
	int MysqlDataReader::GetInt(IN int nCol)  
	{  
		String valStr = GetString(nCol);
		return valStr.ToInt(0);
	}  
	int MysqlDataReader::GetInt(IN LPCSTR name,IN int def)  
	{
		String valStr = GetString(name,NULL);
		return valStr.ToInt(def);
	}
	bool MysqlDataReader::GetBool(IN int nCol)  
	{  
		String valStr = GetString(nCol);
		return valStr.ToInt(0)!=0;
	}  
	bool MysqlDataReader::GetBool(IN LPCSTR name,IN bool def)
	{
		String valStr = GetString(name,NULL);
		return valStr.ToInt((int)def)!=0;
	}
	// 获取某列的值(长整形)   
	INT64 MysqlDataReader::GetInt64(IN int nCol)  
	{  
		String valStr = GetString(nCol);
		return Convert::ToInt64ByDef(valStr,0);
	}  
	INT64 MysqlDataReader::GetInt64(IN LPCSTR name,IN INT64 def)  
	{
		String valStr = GetString(name,NULL);
		return Convert::ToInt64ByDef(valStr,def);
	}
	// 获取某列的值(浮点形)   
	double MysqlDataReader::GetFloat(IN int nCol)  
	{  
		String valStr = GetString(nCol);
		return Convert::ToDouble(valStr,0.0);
	}  
	double MysqlDataReader::GetFloat(IN LPCSTR name,IN double def)  
	{
		String valStr = GetString(name,NULL);
		return Convert::ToDouble(valStr,def);
	}
	// 获取某列的值(二进制数据)   
	const BYTE* MysqlDataReader::GetBlob(IN int nCol,IN int &nLen)  
	{  
		unsigned   long   *FieldLength = PDLL->fetch_lengths(PSTMT);
		nLen = (int)FieldLength[nCol];
		return (const BYTE*)_row[nCol];
	}
	const BYTE* MysqlDataReader::GetBlob(IN LPCSTR name,IN int &nLen)  
	{
		NameIndexMap::iterator it = m_columns.find(name);
		if(it==m_columns.end()) return NULL;

		unsigned   long   *FieldLength = PDLL->fetch_lengths(PSTMT);
		nLen = (int)FieldLength[it->second];
		return (const BYTE*)_row[it->second];
	}
}}//end namespace System.DB
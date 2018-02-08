#pragma once
#include "..\core\IDataReader.hpp"
#include "MysqlDB.hpp"

namespace System{
	namespace DB{

	class MysqlDataReader:public IDataReader
	{
	private:
		char**		_row;
		MysqlDB*	_pDB;

	public:
		MysqlDataReader(IN void* pStmt,IN MysqlDB* pDB,IN bool isBufferName);
		~MysqlDataReader();

	public:  
		// 读取一行数据   
		bool Read();  
		// 关闭Reader，读取结束后调用   
		void Close();  
		// 总的列数   
		int ColumnCount();  
		// 获取某列的名称    
		LPCSTR GetNameA(IN int nCol);

		// 获取某列的值(字符串)   
		String GetString(IN int nCol);
		String GetString(IN LPCSTR name,IN LPCTSTR def=NULL);
#ifdef _UNICODE   
		string GetStringA(IN LPCSTR name,IN LPCSTR def=NULL);
#endif

		// 获取某列的值(整形)   
		int GetInt(IN int nCol);
		int GetInt(IN LPCSTR name,IN int def=0);

		bool GetBool(IN int nCol);
		bool GetBool(IN LPCSTR name,IN bool def=false);

		// 获取某列的值(长整形)   
		INT64 GetInt64(IN int nCol);
		INT64 GetInt64(IN LPCSTR name,IN INT64 def=0);

		// 获取某列的值(浮点形)   
		double GetFloat(IN int nCol);
		double GetFloat(IN LPCSTR name,IN double def=0.0);

		// 获取某列的值(二进制数据)   
		const BYTE* GetBlob(IN int nCol,IN int &nLen);
		const BYTE* GetBlob(IN LPCSTR name,IN int &nLen);
	};

}}//end namespace System.DB
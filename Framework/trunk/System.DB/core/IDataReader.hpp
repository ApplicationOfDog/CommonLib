#pragma once
#include <map>

namespace System{
	namespace DB{

	typedef std::map<std::string,int> NameIndexMap;

	class API_DB IDataReader
	{
	public:
		NameIndexMap	m_columns;

	protected:  
		void* _pStmt;

	public:
		IDataReader(IN void* pStmt);

	public:  
		// 读取一行数据   
		virtual bool Read()=0;
		// 关闭Reader，读取结束后调用   
		virtual void Close()=0;
		// 总的列数   
		virtual int ColumnCount()=0;  
		// 获取某列的名称    
		virtual LPCSTR GetNameA(IN int nCol)=0;

		// 获取某列的值(字符串)   
		virtual String GetString(IN int nCol)=0;
		virtual String GetString(IN LPCSTR name,IN LPCTSTR def=NULL)=0;
#ifdef _UNICODE
		virtual string GetStringA(IN LPCSTR name,IN LPCSTR def=NULL)=0;
#endif

		// 获取某列的值(整形)   
		virtual int GetInt(IN int nCol)=0;
		virtual int GetInt(IN LPCSTR name,IN int def=0)=0;

		virtual bool GetBool(IN int nCol)=0;
		virtual bool GetBool(IN LPCSTR name,IN bool def=false)=0;

		// 获取某列的值(长整形)   
		virtual INT64 GetInt64(IN int nCol)=0;
		virtual INT64 GetInt64(IN LPCSTR name,IN INT64 def=0)=0;

		// 获取某列的值(浮点形)   
		virtual double GetFloat(IN int nCol)=0;
		virtual double GetFloat(IN LPCSTR name,IN double def=0.0)=0;

		// 获取某列的值(二进制数据)
		virtual const BYTE* GetBlob(IN int nCol,IN int &nLen)=0;
		virtual const BYTE* GetBlob(IN LPCSTR name,IN int &nLen)=0;
	};

}}//end namespace System.DB
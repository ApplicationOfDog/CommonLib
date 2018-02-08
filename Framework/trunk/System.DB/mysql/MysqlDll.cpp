#include "stdafx.h"
#include <WinSock.h>
#include "inc/mysql.h"
#include "MysqlDll.hpp"

namespace System{
	namespace DB{

	MysqlDll::MysqlDll(IN LPCTSTR filePath)
	{
		_hDll = NULL;
		if(filePath!=NULL && *filePath!=0)
			Load(filePath);
	}
	MysqlDll::~MysqlDll()
	{
		Free();
	}  

	bool MysqlDll::Load(IN LPCTSTR filePath)
	{
		IError::Clear();
		if(filePath==NULL||*filePath==0)
		{
			IError::InvalidParameter(_T("filePath"));
			return false;
		}

		Free();
		_hDll = ::LoadLibrary(filePath);
		if (_hDll == NULL)
		{
			IError::Assign();
			return false;
		}
		_filePath = filePath;
		return true;
	}
	void MysqlDll::Free()
	{
		IError::Clear();
		if(_hDll != NULL)
		{
			::FreeLibrary(_hDll);
			_hDll = NULL;
		}
		_filePath.Clear();
		_functions.clear();
	}
	
	/////////////////////////////////////////////////////////////////////////////
	MYSQL* MysqlDll::init(MYSQL *mysql)
	{
		typedef MYSQL *	(STDCALL* mysql_init)(MYSQL *mysql);
		FARPROC pFunc = getProcessAddress("mysql_init");
		return ((mysql_init)pFunc)(mysql);
	}
	MYSQL* MysqlDll::real_connect(MYSQL *mysql, const char *host,
		const char *user,
		const char *passwd,
		const char *db,
		unsigned int port,
		const char *unix_socket,
		unsigned long clientflag)
	{
		typedef MYSQL* (STDCALL* mysql_real_connect)(MYSQL *mysql, const char *host,
			const char *user,
			const char *passwd,
			const char *db,
			unsigned int port,
			const char *unix_socket,
			unsigned long clientflag);
		FARPROC pFunc = getProcessAddress("mysql_real_connect");
		return ((mysql_real_connect)pFunc)(mysql,host,user,passwd,db,port,unix_socket,clientflag);
	}
	void MysqlDll::close(MYSQL *sock)
	{
		typedef void (STDCALL* mysql_close)(MYSQL *sock);
		FARPROC pFunc = getProcessAddress("mysql_close");
		((mysql_close)pFunc)(sock);
	}
	int MysqlDll::real_query(MYSQL *mysql, const char *q,unsigned long length)
	{
		typedef int	(STDCALL* mysql_real_query)(MYSQL *mysql, const char *q,unsigned long length);
		FARPROC pFunc = getProcessAddress("mysql_real_query");
		return ((mysql_real_query)pFunc)(mysql,q,length);
	}
	int MysqlDll::affected_rows(MYSQL *mysql)
	{
		typedef my_ulonglong (STDCALL* mysql_affected_rows)(MYSQL *mysql);
		FARPROC pFunc = getProcessAddress("mysql_affected_rows");
		return (int)((mysql_affected_rows)pFunc)(mysql);
	}
	UINT64 MysqlDll::insert_id(MYSQL *mysql)
	{
		typedef my_ulonglong (STDCALL* mysql_insert_id)(MYSQL *mysql);
		FARPROC pFunc = getProcessAddress("mysql_insert_id");
		return (UINT64)((mysql_insert_id)pFunc)(mysql);
	}
	unsigned int MysqlDll::get_errno(MYSQL *mysql)
	{
		typedef unsigned int (STDCALL* mysql_errno)(MYSQL *mysql);
		FARPROC pFunc = getProcessAddress("mysql_errno");
		return ((mysql_errno)pFunc)(mysql);
	}
	LPCSTR MysqlDll::get_errorText(MYSQL *mysql)
	{
		typedef const char * (STDCALL* mysql_error)(MYSQL *mysql);
		FARPROC pFunc = getProcessAddress("mysql_error");
		return ((mysql_error)pFunc)(mysql);
	}
	MYSQL_RES* MysqlDll::store_result(MYSQL *mysql)
	{
		typedef MYSQL_RES * (STDCALL* mysql_store_result)(MYSQL *mysql);
		FARPROC pFunc = getProcessAddress("mysql_store_result");
		return ((mysql_store_result)pFunc)(mysql);
	}
	MYSQL_FIELD* MysqlDll::fetch_field(MYSQL_RES *result)
	{
		typedef MYSQL_FIELD* (STDCALL* mysql_fetch_field)(MYSQL_RES *result);
		FARPROC pFunc = getProcessAddress("mysql_fetch_field");
		return ((mysql_fetch_field)pFunc)(result);
	}
	MYSQL_ROW MysqlDll::fetch_row(MYSQL_RES *result)
	{
		typedef MYSQL_ROW (STDCALL* mysql_fetch_row)(MYSQL_RES *result);
		FARPROC pFunc = getProcessAddress("mysql_fetch_row");
		return ((mysql_fetch_row)pFunc)(result);
	}
	unsigned long* MysqlDll::fetch_lengths(MYSQL_RES *result)
	{
		typedef unsigned long * (STDCALL* mysql_fetch_lengths)(MYSQL_RES *result);
		FARPROC pFunc = getProcessAddress("mysql_fetch_lengths");
		return ((mysql_fetch_lengths)pFunc)(result);
	}
	void MysqlDll::free_result(MYSQL_RES *result)
	{
		typedef void (STDCALL* mysql_free_result)(MYSQL_RES *result);
		FARPROC pFunc = getProcessAddress("mysql_free_result");
		return ((mysql_free_result)pFunc)(result);
	}	
	UINT64 MysqlDll::num_rows(MYSQL_RES *res)
	{
		typedef my_ulonglong (STDCALL* mysql_num_rows)(MYSQL_RES *res);
		FARPROC pFunc = getProcessAddress("mysql_num_rows");
		return (UINT64)((mysql_num_rows)pFunc)(res);
	}
	int MysqlDll::set_character_set(MYSQL *mysql, const char *csname)
	{
		typedef int (STDCALL* mysql_set_character_set)(MYSQL *mysql, const char *csname);
		FARPROC pFunc = getProcessAddress("mysql_set_character_set");
		return ((mysql_set_character_set)pFunc)(mysql,csname);
	}
	void MysqlDll::get_character_set_info(MYSQL *mysql, string* pInfo)
	{
		MY_CHARSET_INFO charset = {0};
		typedef int (STDCALL* mysql_get_character_set_info)(MYSQL *mysql,MY_CHARSET_INFO *charset);

		FARPROC pFunc = getProcessAddress("mysql_get_character_set_info");
		((mysql_get_character_set_info)pFunc)(mysql,&charset);
		if(pInfo!=NULL)
		{
#			undef _GET_STRING_
#			define _GET_STRING_(s) (s!=NULL && *s!=0 ? s :"")
			*pInfo = String::FormatA(
				"{\"csname\":\"%s\",\"name\":\"%s\",\"comment\":\"%s\",\"dir\":\"%s\",\"number\":%d,\"state\":%d,\"mbminlen\":%d,\"mbmaxlen\":%d}",
				_GET_STRING_(charset.csname),
				_GET_STRING_(charset.name),
				_GET_STRING_(charset.comment),
				_GET_STRING_(charset.dir),
				charset.number,
				charset.state,
				charset.mbminlen,
				charset.mbmaxlen
				);
#			undef _GET_STRING_
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	FARPROC MysqlDll::getProcessAddress(IN LPCSTR name)
	{
		if(name==NULL||*name==0) return NULL;
		
		map<string,FARPROC>::iterator it = _functions.find(name);
		if(it!=_functions.end())
			return it->second;
		
		FARPROC pFunc = ::GetProcAddress(_hDll,name);
		assert("get process address failed" && pFunc!=NULL);

		_functions.insert(make_pair(name,pFunc));
		return pFunc;
	}

}}//end namespace System.DB
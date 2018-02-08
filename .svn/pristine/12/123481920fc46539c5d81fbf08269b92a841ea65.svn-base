#include "StdAfx.h"
#include "inc/sqlite3.h"
#include "SqliteDB.hpp"
#include "SqliteStatement.hpp"
#include "SqliteDataReader.hpp"
using namespace System::IO;

#define PDB ((sqlite3*)_pDB)
#define DB_ASSERT_START_SELECT 0x1

namespace System{
	namespace DB{

	SqliteDB::SqliteDB(IN LPCTSTR filePath,IN LPCSTR encryptKey,IN bool isConnect)
	{
		_pDB = NULL;
		_dbType = DBT_SQLITE;
		_encryptKey = NULL;
		_nWaitTimesInBusy = 100;
		_nSleepTimeInBusy = 20;//ms
		_pStatement = new SqliteStatement();
		_multi_max = 500;//Sqlite只支持一次性最多插入500条数据
		_isUnicodeCharset = true;

		map<string,string> params;
		params["filePath"] = Convert::ToStringA(filePath,IO::EC_ANSI);
		if(encryptKey!=NULL && *encryptKey!=0)
			params["encryptKey"] = encryptKey;
		this->Initialize(params);

		if(isConnect) connect();
	}
	SqliteDB::~SqliteDB()
	{
		if(_pDB!=NULL) Close();
		DELETE_STRING(_encryptKey);
		if(_connectionCount!=0||_transactionCount!=0)
		{//还有数据库连接或事务处理没有被关闭
			assert("there are connection or transation alive" && false);
		}		
	}
	bool SqliteDB::LoadDll(IN LPCTSTR filePath)
	{
		assert("NOT IMPLETEMENT!!" && false);
		return false;
	}
	bool SqliteDB::IsValid()
	{ 
		return _pDB!=NULL; 
	}
	String SqliteDB::FilePath()
	{ 
		return _filePath; 
	}
	LPCSTR SqliteDB::EncryptKey()
	{ 
		return _encryptKey; 
	}
	bool SqliteDB::Initialize(IN map<string,string>& params)
	{		
		map<string,string>::iterator it = params.find("filePath");
		if(it == params.end())
		{
			IError::InvalidParameter("filePath");
			return false;
		}
		_filePath = Convert::ToString(it->second);

		it = params.find("encryptKey");
		if(it!=params.end() && !it->second.empty())
		{
			DELETE_STRING(_encryptKey);
			int len = (int)it->second.length()+1;
			_encryptKey = new char[len];
			memcpy(_encryptKey,it->second.c_str(),len);
		}

		it = params.find("waitTimesInBusy");
		if(it!=params.end() && !it->second.empty())
			_nWaitTimesInBusy = Convert::ToIntByDef(it->second.c_str(),100);

		it = params.find("sleepTimeInBusy");
		if(it!=params.end() && !it->second.empty())
			_nSleepTimeInBusy = Convert::ToIntByDef(it->second.c_str(),20);

		it = params.find("isUnicodeCharset");
		if(it!=params.end() && !it->second.empty())
			_isUnicodeCharset = Convert::ToIntByDef(it->second.c_str(),1)==1;

		return true;
	}
	LPCTSTR SqliteDB::GetInforamtion()
	{
		static String text;
		if(text.IsEmpty())
		{
			text.FormatStr(_T("SQLite={ filePath:%s,waitTimesInBusy:%d,sleepTimeInBusy:%d }"),
				(LPCTSTR)_filePath,_nWaitTimesInBusy,_nSleepTimeInBusy);
		}
		return text;
	}
	bool SqliteDB::Open(bool isTransaction)
	{
		IError::Clear();
		String error;
		if(!connect())
		{
			error = getLastError();
			String dbFileName = Path::GetFileName(_filePath);
			m_nLastError = ERROR_CONNECTION_ABORTED;
			m_sLastError.FormatStr(_T("connect db failed.(db:%s,err:%s)"),(LPCTSTR)dbFileName,(LPCTSTR)error);//连接全局数据库失败.(db:%s,err:%s)
			return false;
		}
		if(isTransaction && !BeginTransaction())
		{
			error = getLastError();
			m_nLastError = ERROR_DATABASE_FAILURE;
			m_sLastError.FormatStr(_T("begin db transaction failed.(err:%s)"),(LPCTSTR)error);//启动全局数据库事务失败
			Close();
			return false;
		}
		return true;
	}
	bool SqliteDB::Close()
	{
		Lock(__LINE__);
		bool result = true;
		int status = SQLITE_OK;
		if(_connectionCount<=0)
		{
			assert("Close function should be match with Open" && _connectionCount==0);//小于0表示关闭太多次
			goto _BACK_;//链接已经关闭或open失败
		}

		_connectionCount--;
		if(_connectionCount!=0) goto _BACK_;//多次Connect之后要匹配相同次数的Close

		status = sqlite3_close(PDB);
		if (status!=SQLITE_OK)
		{
			String error = getLastError();
			m_nLastError = ERROR_DATABASE_FAILURE;//关闭数据库失败
			m_sLastError.FormatStr(_T("close db connection failed,%d-%s"),status,(LPCTSTR)error);
			result = false;
			goto _BACK_;
		}
		_pDB=NULL;
		assert("no all transactions have been closed" && _transactionCount==0);//在关闭连接之前应该要确保事务处理完毕
		assert("EndSelect not have been called" && !FLAG_ON(_asserts,DB_ASSERT_START_SELECT));
	_BACK_:		
		Unlock(__LINE__);
		return result;
	}

	bool SqliteDB::BeginTransaction()
	{
		Lock(__LINE__);
		bool result = true;	
		if(_transactionCount<0)
		{
			_transactionCount = 0;//Commit次数过多导致_transactionCount为负数
			assert("Commit too much times" && false);
		}

		_transactionCount++;
		if (_transactionCount > 1) goto _BACK_;//BeginTransaction被重复调用多次
		result = ExecuteQuery(_T("begin;"),(sqlite3_callback)NULL);
_BACK_:
		Unlock(__LINE__);
		return result;
	}
	bool SqliteDB::CommitTransaction()
	{
		Lock(__LINE__);
		bool result = true;
		_transactionCount--;
		if (_transactionCount != 0) goto _BACK_;//多次Begin之后要匹配相同次数的Commit
		result = ExecuteQuery(_T("commit;"),(sqlite3_callback)NULL);
_BACK_:
		Unlock(__LINE__);
		return result;
	}
	bool SqliteDB::RollbackTransaction()
	{
		Lock(__LINE__);
		_transactionCount = 0;
		bool result = ExecuteQuery(_T("rollback;"),(sqlite3_callback)NULL);
		Unlock(__LINE__);
		return result;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool SqliteDB::GetTableStructureInfo(IN LPCTSTR table,OUT TableStructure* pStructure)
	{
		assert(table!=NULL && *table!=0);
		assert(pStructure!=NULL);

		bool isSuc = false;
		bool isGetDetail = true;
		if(!connect()) return false;

		pStructure->Clear();
		IDataReader* pReader=NULL;
		String sql = String::Format(_T("pragma table_info ('%s') "),table);
		isSuc = ExecuteQuery(sql,&pReader);
		if(!isSuc) goto _BACK_;

		while(pReader->Read())
		{
			TableField* pField = new TableField();
			pField->m_index = pReader->GetInt("cid");
			pField->m_name = pReader->GetString("name");
			pField->DataType(pReader->GetString("type"));
			pField->m_isNotNull = pReader->GetBool("notnull");
			pField->m_isPrimaryKey = pReader->GetBool("pk");
			pStructure->m_fields.push_back(pField);
		}
		pStructure->m_name = table;
		pReader->Close();
		DELETE_OBJECT(pReader);

		if(isGetDetail)
		{
			sql.FormatStr(_T("select * from sqlite_master where type='table' and tbl_name='%s' order by name;"),table);
			isSuc = ExecuteQuery(sql,&pReader);
			if(!isSuc) goto _BACK_;
			if(pReader->Read())
			{
				pStructure->m_createTableSql = pReader->GetString("sql");
				pStructure->m_createTableSql.Trim();
				if(!pStructure->m_createTableSql.IsEmptyOrWhiteSpace())
				{
					int startIdx = pStructure->m_createTableSql.IndexOf(_T('('),0);
					int endIdx = pStructure->m_createTableSql.LastIndexOf(_T(')'));
					if(startIdx>0 && endIdx>0 && endIdx>startIdx)
					{
						String temp = pStructure->m_createTableSql.Substring(startIdx+1,endIdx-startIdx-1);
						vector<String> allFieldStr = temp.Split(_T(','),SSO_RemoveEmptyEntries);
						vector<String>::iterator it = allFieldStr.begin();
						for (;it!=allFieldStr.end();it++)
						{
							String fieldStr = String::Trim(*it);
							if(fieldStr.IndexOf(_T("AUTOINCREMENT"),0,true)<0) continue;

							endIdx = fieldStr.IndexOf(_T(' '),0);
							if(endIdx<=0) continue;
							String fieldName = fieldStr.Substring(0,endIdx);
							fieldName.Trim(_T("\" "));
							TableField* pField = pStructure->FindField(fieldName);
							if(pField!=NULL) pField->m_isAutoIncrement = true;
						}
					}
				}
			}
			pReader->Close();
			DELETE_OBJECT(pReader);
		}
_BACK_:
		Close();
		return isSuc;
	}

	bool SqliteDB::GetAllTableName(OUT vector<String>& tables)
	{
		bool isSuc = false;
		if(!connect()) return false;

		tables.clear();
		IDataReader* pReader=NULL;
		LPCTSTR sql = _T("select name from sqlite_master where type='table'");
		isSuc = ExecuteQuery(sql,&pReader);
		if(!isSuc) goto _BACK_;

		while(pReader->Read())
		{
			String item = pReader->GetString("name");
			tables.push_back(item);
		}
		pReader->Close();
		DELETE_OBJECT(pReader);
_BACK_:
		Close();
		return isSuc;
	}
	bool SqliteDB::IsTableExist(IN LPCTSTR name)
	{
		IError::Clear();
		if(!connect()) return false;

		String sql;
		sql.FormatStr(_T("SELECT COUNT(*) FROM sqlite_master where type='table' and name='%s'"),name);
		int count = GetSingle_Int32(sql,-1);

		Close();
		return count>0;
	}
	bool SqliteDB::GetInformation(IN LPCSTR command,OUT string* pInfo)
	{
		IError::Clear();
		if(command==NULL||*command==0){
			IError::InvalidParameter("command");
			return false;
		}
		if(pInfo==NULL){
			IError::InvalidParameter("pInfo");
			return false;
		}
		string commandTemp = command;
		assert("Not Implemented" && false);
		return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool SqliteDB::ExecuteQuery(IN LPCTSTR sql,IN sqlite3_callback fnCallBack)
	{
		Lock(__LINE__);
		IError::Clear();
		bool result = false;
		char* error = NULL;
		std::string utf8_sql;

		if(sql==NULL||*sql==0)
		{
			m_nLastError = ERROR_INVALID_PARAMETER;
			m_sLastError = _T("invalid parameters:empty sql statement");
			goto _BACK_;
		}
		if (_pDB==NULL)
		{
			m_nLastError = ERROR_DATABASE_FAILURE;
			m_sLastError = _T("invalid parameters:no connection active");
			goto _BACK_;
		}

		utf8_sql = Convert::ToStringA(sql,IO::EC_UTF8);
		m_nLastError = (DWORD)sqlite3_exec(PDB, utf8_sql.c_str(),NULL,fnCallBack,&error);
		if (m_nLastError!=SQLITE_OK)
		{
			m_sLastError = Convert::ToString(error,IO::EC_UTF8);	
			goto _FREE_;
		}
		m_nLastError = ERROR_SUCCESS;
		result = true;
_FREE_:
		if(error!=NULL)
		{
			sqlite3_free(error);
			error = NULL;
		}
_BACK_:
		Unlock(__LINE__);
		return result;
	}
	bool SqliteDB::ExecuteQuery(IN LPCTSTR sql,OUT IDataReader** pReader)  
	{  
		assert(pReader!=NULL);
		Lock(__LINE__);
		IError::Clear();
		sqlite3_stmt* stmt=NULL;
		bool result = false;

		if(sql==NULL||*sql==0)
		{
			m_nLastError = ERROR_INVALID_PARAMETER;
			m_sLastError = _T("invalid parameters:empty sql statement");
			goto _BACK_;
		}
		if (_pDB==NULL)
		{
			m_nLastError = ERROR_DATABASE_FAILURE;
			m_sLastError = _T("invalid parameters:no connection active");
			goto _BACK_;
		}

		*pReader=NULL;
		if(_isUnicodeCharset)
		{
			wstring sqlw = Convert::ToStringW(sql,IO::EC_ANSI);
			m_nLastError = (DWORD)sqlite3_prepare16_v2(PDB, sqlw.c_str(), -1, &stmt, NULL);
		}
		else
		{
			string sqlA = Convert::ToStringA(sql,IO::EC_ANSI);
			m_nLastError = (DWORD)sqlite3_prepare_v2(PDB, sqlA.c_str(), -1, &stmt, NULL);
		}
		if( m_nLastError!= SQLITE_OK)
		{
			this->getLastError();
			goto _BACK_;
		}
		m_nLastError = ERROR_SUCCESS;
		*pReader = new SqliteDataReader(stmt,true);
		result = true;
_BACK_:
		Unlock(__LINE__);
		return result;
	}  
	INT64 SqliteDB::ExecuteNonQuery(IN LPCTSTR commandText,IN bool isBackRowID)  
	{
		Lock(__LINE__);
		IError::Clear();
		INT64 result = 0,nRowModified = 0;
		sqlite3_stmt* stmt=NULL;

		if(commandText==NULL||*commandText==0)
		{
			m_nLastError = ERROR_INVALID_PARAMETER;
			m_sLastError = _T("invalid parameters:empty sql statement");
			result = -1;
			goto _BACK_;
		}
		if (_pDB==NULL)
		{
			m_nLastError = ERROR_DATABASE_FAILURE;
			m_sLastError = _T("invalid parameters:no connection active");
			result = -1;
			goto _BACK_;
		}
		if(_isUnicodeCharset)
		{
			wstring sqlw = Convert::ToStringW(commandText,IO::EC_ANSI);
			m_nLastError = (DWORD)sqlite3_prepare16_v2(PDB, sqlw.c_str(), -1, &stmt, NULL);
		}
		else
		{
			string sqlA = Convert::ToStringA(commandText,IO::EC_ANSI);
			m_nLastError = (DWORD)sqlite3_prepare_v2(PDB, sqlA.c_str(), -1, &stmt, NULL);
		}
		if(result!= SQLITE_OK)
		{
			m_nLastError = (DWORD)result;
			this->getLastError();
			result = -2;
			goto _BACK_;
		}

		sqlite3_step(stmt);
		if((result  = sqlite3_finalize(stmt)) != SQLITE_OK)
		{
			m_nLastError = (DWORD)result;
			this->getLastError();
			result = -3;
			goto _BACK_;
		}
		nRowModified = sqlite3_changes(PDB);
		if(nRowModified<=0||!isBackRowID)
		{
			result = nRowModified;
			goto _BACK_;
		}
		result = sqlite3_last_insert_rowid(PDB);//返回最后插入的RowID(即自增长ID值)
_BACK_:
		Unlock(__LINE__);
		return result;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	////// internal
	bool SqliteDB::connect()
	{
		Lock(__LINE__);
		IError::Clear();
		bool result = true;
		int nBack = SQLITE_OK;
		if(_filePath.IsEmptyOrWhiteSpace())
		{
			m_nLastError = ERROR_FILE_NOT_FOUND;
			m_sLastError = _T("invalid db path,empty file path");//数据库文件路径为空
			result =  false;
			goto _BACK_;
		}

		if(_connectionCount<0)
		{
			assert(false);//Close次数过多导致_connectionCount为负数
			_connectionCount = 0;
		}

		_connectionCount++;
		if(_connectionCount>1) goto _BACK_;//连接过了

		//此时:_connectionCount = 1
		if(_isUnicodeCharset)
		{
			wstring filePathW = Convert::ToStringW(_filePath,IO::EC_ANSI);
			nBack = sqlite3_open16(filePathW.c_str(),(sqlite3**)&_pDB);
		}
		else
		{
			string filePathA = Convert::ToStringA(_filePath,IO::EC_ANSI);
			nBack = sqlite3_open(filePathA.c_str(),(sqlite3**)&_pDB);
		}
		if(nBack !=SQLITE_OK)
		{
			String lastError = getLastError();
			m_nLastError = nBack;//打开数据库失败
			m_sLastError.FormatStr(_T("open database failed,%s"),(LPCTSTR)lastError);
			_connectionCount = 0;
			result =  false;
			goto _BACK_;
		}

#ifdef SQLITE_HAS_CODEC
		if(_encryptKey!=NULL && *_encryptKey!=0)
		{
			nBack = sqlite3_key(PDB, _encryptKey,(int)strlen(_encryptKey));
			if(nBack != SQLITE_OK) 
			{
				String lastError = getLastError();
				m_nLastError = nBack;//设置加密密钥失败
				m_sLastError.FormatStr(_T("set db password failed,%s"),(LPCTSTR)lastError);
				_connectionCount = 0;
				result =  false;
				goto _BACK_;
			}
		}
#endif
		sqlite3_busy_handler(PDB,do_db_busy_handler,this);
_BACK_:
		Unlock(__LINE__);
		return result;
	}
	String SqliteDB::getLastError()
	{
#ifdef _UNICODE    
		m_sLastError =(LPCTSTR)sqlite3_errmsg16(PDB);  
#else   
		m_sLastError = sqlite3_errmsg(PDB);  
#endif
		return m_sLastError;
	}
	int SqliteDB::do_db_busy_handler(void *ptr,int count)
	{
		SqliteDB* _this = (SqliteDB*)ptr;

		String text;
		if(count>=_this->_nWaitTimesInBusy)
		{
			text.FormatStr(_T("System.DB.SqliteDB:database is lock too long,return SQLITE_IOERR_BLOCKED \n"),count);
			::OutputDebugString(text);
			return 0;//超过次数还无法获取数据库锁就直接返回SQLITE_BUSY或者SQLITE_IOERR_BLOCKED
		}

		text.FormatStr(_T("System.DB.SqliteDB:database is lock now,can not write/read,count:%d \n"),count);
		::OutputDebugString(text);
		::Sleep(_this->_nSleepTimeInBusy);//每次休眠时间
		return 1;//继续尝试操作数据库
	}
}}//end namespace
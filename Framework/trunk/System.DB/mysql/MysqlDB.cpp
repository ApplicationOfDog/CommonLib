#include "StdAfx.h"
#include <WinSock.h>
#include "inc/mysql.h"
#include "MysqlDB.hpp"
#include "MysqlDll.hpp"
#include "MysqlStatement.hpp"
#include "MysqlDataReader.hpp"
using namespace System::IO;
#pragma comment( lib, "ws2_32.lib")

#undef PDB
#undef PDLL
#define PDB (MYSQL*)_pDB
#define PDLL ((MysqlDll*)m_pDll)

#define DB_ASSERT_START_SELECT 0x1

namespace System{
	namespace DB{

	MysqlDB::MysqlDB()
	{
		_pDB = NULL;
		_dbType = DBT_MYSQL;
		_pStatement = new MysqlStatement();
		_charset = "GBK";
		_multi_max = 0;
	}
	MysqlDB::~MysqlDB()
	{
		if(_pDB!=NULL) Close();
		if(_connectionCount!=0||_transactionCount!=0)
		{//还有数据库连接或事务处理没有被关闭
			assert("there are connection or transation alive" && false);
		}
		if(m_pDll!=NULL)
		{
			PDLL->Free();
			delete m_pDll;
		}
	}
	bool MysqlDB::LoadDll(IN LPCTSTR filePath)
	{
		if(filePath==NULL||*filePath==0) return false;
		if(m_pDll!=NULL) return true;//已经加载过了,不用重复加载

		m_pDll = new MysqlDll();
		if(m_pDll==NULL)
		{
			Assign();
			return false;
		}
		if(!PDLL->Load(filePath))
		{
			delete m_pDll;
			m_pDll = NULL;
		}
		return m_pDll!=NULL;
	}
	bool MysqlDB::IsValid()
	{ 
		return _pDB!=NULL; 
	}
	bool MysqlDB::Initialize(IN map<string,string>& params)
	{
		string s_port,s_multi_max;
		map<string,string*> values;
		values.insert(make_pair("host",&_host));
		values.insert(make_pair("port",&s_port));
		values.insert(make_pair("userName",&_userName));
		values.insert(make_pair("password",&_password));
		values.insert(make_pair("database",&_database));
		values.insert(make_pair("charset",&_charset));
		values.insert(make_pair("multi_max",&s_multi_max));

		map<string,string>::iterator pit;
		map<string,string*>::iterator vit = values.begin();
		for (;vit!=values.end();vit++)
		{
			if(vit->second == NULL) continue;

			pit = params.find(vit->first);
			if(pit == params.end()||pit->second.empty())
			{
				if(vit->first == "multi_max") continue;//非必须参数
				IError::InvalidParameter(vit->first.c_str());
				return false;
			}
			vit->second->assign(pit->second.c_str());
		}
		_port = Convert::ToIntByDef(s_port.c_str(),0);
		_multi_max = Convert::ToIntByDef(s_multi_max.c_str(),0);
		return true;
	}
	LPCTSTR MysqlDB::GetInforamtion()
	{
		static String text;
		if(text.IsEmpty())
			text.FormatStrA("MySQL={ host:%s,port:%d,charset:%s }",_host.c_str(),_port,_charset.c_str());		
		return text;
	}
	bool MysqlDB::Open(bool isTransaction)
	{
		IError::Clear();
		String error;
		if(!connect())
		{
			error = getLastError();
			m_nLastError = ERROR_CONNECTION_ABORTED;
			m_sLastError.FormatStr(_T("connect db failed.(db:%s,err:%s)"),(LPCTSTR)error);//连接全局数据库失败.(db:%s,err:%s)
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
	bool MysqlDB::Close()
	{
		Lock(__LINE__);
		bool result = true;
		if(_connectionCount<=0)
		{
			assert("Close function should be match with Open" && false);//小于0表示关闭太多次
			goto _BACK_;//链接已经关闭或open失败
		}

		_connectionCount--;
		if(_connectionCount!=0) goto _BACK_;//多次Connect之后要匹配相同次数的Close

		PDLL->close(PDB);
		DELETE_OBJECT(_pDB);
		assert("no all transactions have been closed" && _transactionCount==0);//在关闭连接之前应该要确保事务处理完毕
		assert("EndSelect not have been called" && !FLAG_ON(_asserts,DB_ASSERT_START_SELECT));
	_BACK_:
		Unlock(__LINE__);
		return true;
	}

	bool MysqlDB::BeginTransaction()
	{
		assert(_pDB!=NULL && "call Open function first");
		Lock(__LINE__);
		bool result = true;	
		string sql = "START TRANSACTION";
		if(_transactionCount<0)
		{
			_transactionCount = 0;//Commit次数过多导致_transactionCount为负数
			assert("Commit too much times" && false);
		}

		_transactionCount++;
		if (_transactionCount > 1) goto _BACK_;//BeginTransaction被重复调用多次

		if(PDLL->real_query(PDB,sql.c_str() ,(unsigned long)sql.length())!=0)
		{
			result = false;
			getLastError(true);
			goto _BACK_;
		}
_BACK_:
		Unlock(__LINE__);
		return result;
	}
	bool MysqlDB::CommitTransaction()
	{
		Lock(__LINE__);
		bool result = true;
		string sql = "COMMIT";
		_transactionCount--;
		if (_transactionCount != 0) goto _BACK_;//多次Begin之后要匹配相同次数的Commit

		if(PDLL->real_query(PDB,sql.c_str() ,(unsigned long)sql.length())!=0)
		{
			result = false;
			getLastError(true);
			goto _BACK_;
		}
_BACK_:
		Unlock(__LINE__);
		return result;
	}
	bool MysqlDB::RollbackTransaction()
	{
		Lock(__LINE__);
		_transactionCount = 0;
		string sql = "ROLLBACK";
		bool result =true;

		if(PDLL->real_query(PDB,sql.c_str() ,(unsigned long)sql.length())!=0)
		{
			result = false;
			getLastError(true);
		}
		Unlock(__LINE__);
		return result;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool MysqlDB::GetTableStructureInfo(IN LPCTSTR table,OUT TableStructure* pStructure)
	{
		assert(table!=NULL && *table!=0);
		assert(pStructure!=NULL);

		bool isSuc = false;
		if(!connect()) return false;

		int index = 0;
		String tempTxt,sql;		
		IDataReader* pReader=NULL;
		sql.FormatStr(_T("show columns from %s"),table);
		isSuc = ExecuteQuery(sql,&pReader);
		if(!isSuc) goto _BACK_;

		pStructure->Clear();
		while(pReader->Read())
		{
			TableField* pField = new TableField();
			pField->m_index = index++;
			pField->m_name = pReader->GetString("Field");
			pField->DataType(pReader->GetString("Type"));
			tempTxt = pReader->GetString("Null");
			pField->m_isNotNull = tempTxt.Compare(_T("YES"),true)==0;
			tempTxt = pReader->GetString("Key");
			pField->m_isPrimaryKey = tempTxt.Compare(_T("PRI"),true)==0;

			tempTxt = pReader->GetString("Extra");
			pField->m_isAutoIncrement = tempTxt.Compare(_T("auto_increment"),true)==0;
			pStructure->m_fields.push_back(pField);
		}
		pStructure->m_name = table;
		pReader->Close();
		DELETE_OBJECT(pReader);
_BACK_:
		Close();
		return isSuc;
	}
	bool MysqlDB::GetAllTableName(OUT vector<String>& tables)
	{
		bool isSuc = false;
		if(!connect()) return false;

		tables.clear();
		IDataReader* pReader=NULL;
		String commandText = String::Format("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = '%s'",_database.c_str());
		isSuc = ExecuteQuery(commandText,&pReader);
		if(!isSuc) goto _BACK_;

		while(pReader->Read())
		{
			String item = pReader->GetString(0);
			tables.push_back(item);
		}
		pReader->Close();
		DELETE_OBJECT(pReader);
_BACK_:
		Close();
		return isSuc;
	}
	bool MysqlDB::IsTableExist(IN LPCTSTR name)
	{
		IError::Clear();
		if(!connect()) return false;

		String commandText;
		String databaseW = Convert::ToString(_database);
		commandText.FormatStr(_T("SELECT COUNT(*) FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = '%s' AND TABLE_NAME='%s'"),
			(LPCTSTR)databaseW,name);
		int count = GetSingle_Int32(commandText,-1);

		Close();
		return count>0;
	}
	bool MysqlDB::GetInformation(IN LPCSTR command,OUT string* pInfo)
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
		if(commandTemp == "GetCharset")
		{
			PDLL->get_character_set_info(PDB,pInfo);
		}
		else
		{
			IError::Error(_T("invalid command"));
			return false;
		}
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool MysqlDB::ExecuteQuery(IN LPCTSTR sql,OUT IDataReader** pReader)  
	{  
		assert(pReader!=NULL);
		Lock(__LINE__);
		IError::Clear();
		MYSQL_RES* stmt=NULL;
		bool result = false;
		string commandTextA;

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
		//这里commandText只能用UTF-8编码,否则对于一些特殊字符会出现类似"Incorrect string value: '\xF0\x90\x8D\x83\xF0\x90...' "这样的错误
		commandTextA = Convert::ToStringA(sql,IO::EC_UTF8/*IO::EC_ANSI*/);
		m_nLastError = (DWORD)PDLL->real_query(PDB,commandTextA.c_str(),commandTextA.length());
		if( m_nLastError!= 0)
		{
			getLastError();
			goto _BACK_;
		}
		m_nLastError = ERROR_SUCCESS;
		//保存查询结果
		stmt = PDLL->store_result(PDB);
		if(stmt==NULL)
		{
			getLastError(true);
			goto _BACK_;
		}
		*pReader = new MysqlDataReader(stmt,this,true);
		result = true;
_BACK_:
			Unlock(__LINE__);
			return result;
	}  
	INT64 MysqlDB::ExecuteNonQuery(IN LPCTSTR commandText,IN bool isBackRowID)  
	{
		Lock(__LINE__);
		IError::Clear();
		INT64 result = 0,nRowModified = 0;
		string commandTextA;

		if(commandText==NULL||*commandText==0)
		{
			m_nLastError = ERROR_INVALID_PARAMETER;
			m_sLastError = _T("invalid parameters:empty sql statement");
			result = -1;
			goto _BACK_;
		}
		if (_pDB==NULL)
		{
			m_nLastError = ERROR_ACTIVE_CONNECTIONS;
			m_sLastError = _T("invalid parameters:no connection active");
			result = -1;
			goto _BACK_;
		}
		//这里commandText只能用UTF-8编码,否则对于一些特殊字符会出现类似"Incorrect string value: '\xF0\x90\x8D\x83\xF0\x90...' "这样的错误
		commandTextA = Convert::ToStringA(commandText,IO::EC_UTF8/*IO::EC_ANSI*/);
		m_nLastError = (DWORD)PDLL->real_query(PDB,commandTextA.c_str(),commandTextA.length());
		if(m_nLastError!= 0)
		{
			m_nLastError = ERROR_DATABASE_FAILURE;
			getLastError();
			result = -2;
			goto _BACK_;
		}
		nRowModified = (INT64)PDLL->affected_rows(PDB);
		if(nRowModified<=0||!isBackRowID)
		{
			result = nRowModified;
			goto _BACK_;
		}
		result = (INT64)PDLL->insert_id(PDB);//返回最后插入的RowID(即自增长ID值)
_BACK_:
		Unlock(__LINE__);
		return result;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	////// internal
	bool MysqlDB::connect()
	{
		Lock(__LINE__);
		IError::Clear();
		bool result = false;

		if(_connectionCount<0)
		{
			assert("close too much times" && false);//Close次数过多导致_connectionCount为负数
			_connectionCount = 0;
		}

		_connectionCount++;
		if(_connectionCount>1)
		{
			result = true;
			goto _BACK_;//连接过了
		}

		if(_pDB == NULL) _pDB = new MYSQL();
		if(_pDB==NULL)
		{
			Assign();
			goto _BACK_;
		}

		//此时:_connectionCount = 1
		if(PDLL->init(PDB) == NULL)
		{
			getLastError(true);
			goto _BACK_;
		}
		if(PDLL->real_connect(PDB,_host.c_str(),_userName.c_str(),_password.c_str(),_database.c_str(),_port,NULL,0) == NULL)
		{
			getLastError(true);
			goto _BACK_;
		}
		if(PDLL->set_character_set(PDB,_charset.c_str()) != 0)/*"GBK"  "utf8"*/
		{
			getLastError(true);
			goto _BACK_;
		}
		result = true;
_BACK_:
		Unlock(__LINE__);
		return result;
	}
	String MysqlDB::getLastError()
	{
		string errorA = PDLL->get_errorText(PDB);
#ifdef _UNICODE		
		m_sLastError = Convert::ToString(errorA.c_str(),IO::EC_ANSI);  
#else   
		m_sLastError = errorA;  
#endif
		return m_sLastError;
	}
	String MysqlDB::getLastError(IN bool isGetCode)
	{
		if(isGetCode) m_nLastError = PDLL->get_errno(PDB);
		return getLastError();
	}

}}//end namespace

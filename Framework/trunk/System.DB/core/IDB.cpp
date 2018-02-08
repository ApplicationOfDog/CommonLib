#include "StdAfx.h"
#include "IDB.hpp"

#include "..\sqlite\SqliteStatement.hpp"
#include "..\sqlite\SqliteDB.hpp"
#include "..\mysql\MysqlStatement.hpp"
#include "..\mysql\MysqlDB.hpp"

using namespace System::IO;

#define DB_ASSERT_START_SELECT 0x1

namespace System{
	namespace DB{

		IDB* IDB::CreateObject(IN DatabaseType dt)
		{
			switch(dt)
			{
			case DBT_SQLITE:
				return new SqliteDB();

			case DBT_MYSQL:
				return new MysqlDB();
			}
			return NULL;
		}
		void IDB::DeleteObject(IN void* p)
		{
			DELETE_OBJECT(p);
		}

		IDB::IDB()
		{
			_connectionCount = 0;
			_transactionCount = 0;
			_asserts = 0x0;
			_lockCounter = 0;
			_pStatement = NULL;
			m_pDll = NULL;
			_multi_max = 0;
		}
		IDB::~IDB()
		{
			DELETE_OBJECT(_pStatement);
		}
		ISqlStatement* IDB::Statement(IN TableStructure* structure,IN StatementType statType,IN bool isNew)
		{			
			if(isNew)
			{
				switch(_dbType)
				{
				case DBT_SQLITE:
					return new SqliteStatement(structure,statType,_multi_max);
				case DBT_MYSQL:
					return new MysqlStatement(structure,statType,_multi_max);
				}
				return NULL;
			}
			if(_pStatement!=NULL)
			{
				_pStatement->Clear();
				_pStatement->Structure(structure);
				_pStatement->Type(statType);
				_pStatement->MultiMax(_multi_max);
			}
			return _pStatement;
		}
		IDataReader* IDB::StartSelect(IN LPCTSTR table,IN LPCTSTR fields,IN LPCTSTR format,...)
		{
			Lock(__LINE__);
			if(!connect())
			{
				Unlock(__LINE__);//执行失败直接解锁,因为不会调用EndSelect了
				return NULL;
			}

			String condition;
			if(format!=NULL && *format!=0)
			{
				va_list ap;
				va_start(ap,format);
				condition = String::Format(format,ap);
				va_end(ap);
			}
			IDataReader* reader=NULL;
			String sql = _pStatement->Select(fields).Where(condition).ToString(table);
			if(!ExecuteQuery(sql,&reader))
			{
				Close();
				Unlock(__LINE__);//执行失败直接解锁,因为不会调用EndSelect了
				return NULL;
			}
			//执行成功时直到EndSelect才会解锁
			SET_VAR_FLAG(_asserts,DB_ASSERT_START_SELECT);
			return reader;
		}
		IDataReader* IDB::StartSelect(IN ISqlStatement* sql)
		{
			assert("error sql type" && sql->Is(ST_SELECT));
			Lock(__LINE__);
			if(!connect())
			{
				Unlock(__LINE__);//执行失败直接解锁,因为不会调用EndSelect了
				return NULL;
			}

			IDataReader* reader=NULL;
			if(!ExecuteQuery(sql->ToString(),&reader))
			{
				Close();
				Unlock(__LINE__);//执行失败直接解锁,因为不会调用EndSelect了
				return NULL;
			}
			//执行成功时直到EndSelect才会解锁
			SET_VAR_FLAG(_asserts,DB_ASSERT_START_SELECT);
			return reader;
		}
		IDataReader* IDB::StartSelectBy0(IN LPCTSTR table,IN LPCTSTR fields,IN LPCTSTR where,IN LPCTSTR orders,IN int limit_count,IN int limit_skip)
		{
			Lock(__LINE__);
			if(!connect())
			{
				Unlock(__LINE__);//执行失败直接解锁,因为不会调用EndSelect了
				return NULL;
			}

			_pStatement->Select(fields).Where(where).OrderBy(orders).Limit(limit_skip,limit_count);
			String commandText = _pStatement->ToString(table);

			IDataReader* reader=NULL;
			if(!ExecuteQuery(commandText,&reader))
			{
				Close();
				Unlock(__LINE__);//执行失败直接解锁,因为不会调用EndSelect了
				return NULL;
			}
			//执行成功时直到EndSelect才会解锁
			SET_VAR_FLAG(_asserts,DB_ASSERT_START_SELECT);
			return reader;
		}
		IDataReader* IDB::StartSelectBy1(IN LPCTSTR format,...)
		{
			assert(format!=NULL && *format!=0);
			Lock(__LINE__);
			if(!connect())
			{
				Unlock(__LINE__);//执行失败直接解锁,因为不会调用EndSelect了
				return NULL;
			}

			String sql;
			if(format!=NULL && *format!=0)
			{
				va_list ap;
				va_start(ap,format);
				sql = String::Format(format,ap);
				va_end(ap);
			}
			IDataReader* reader=NULL;
			if(!ExecuteQuery(sql,&reader))
			{
				Close();
				Unlock(__LINE__);//执行失败直接解锁,因为不会调用EndSelect了
				return NULL;
			}
			//执行成功时直到EndSelect才会解锁
			SET_VAR_FLAG(_asserts,DB_ASSERT_START_SELECT);
			return reader;
		}
		void IDB::EndSelect(IN IDataReader* pReader)
		{
			if(pReader==NULL)
			{
				assert("invalid parameter 'pReader'" && false);
				return;
			}
			pReader->Close();
			DELETE_OBJECT(pReader);		

			if(FLAG_ON(_asserts,DB_ASSERT_START_SELECT))
			{
				CLEAR_VAR_FLAG(_asserts,DB_ASSERT_START_SELECT);
				Unlock(__LINE__);
			}
			Close();
		}
		bool IDB::Update(IN LPCTSTR table,IN LPCTSTR set,IN LPCTSTR whereFormat,...)
		{
			if(!connect()) return false;

			String sql_where;
			if(whereFormat!=NULL && *whereFormat!=0)
			{
				va_list ap;
				va_start(ap,whereFormat);
				sql_where = String::Format(whereFormat,ap);
				va_end(ap);
			}
			String sql = _pStatement->Update(set).Where(sql_where).ToString(table);
			bool isSuc = ExecuteNonQuery(sql,false)>=0;
			Close();
			return isSuc;
		}
		bool IDB::UpdateByID(IN LPCTSTR table,IN LPCTSTR set,IN int id,IN LPCTSTR field)
		{
			return Update(table,set,_T("%s=%d"),field,id);
		}
		INT64 IDB::Insert(IN LPCTSTR table,IN LPCTSTR fields,IN LPCTSTR values,IN bool isBackID)
		{
			INT64 result = FALSE;
			if(!connect()) return false;

			String sql = _pStatement->Insert(isBackID,fields,values).ToString(table);
			result = ExecuteNonQuery(sql,isBackID);
			if(!isBackID) result = result >0 ? TRUE : FALSE;
			Close();
			return result;
		}

		int IDB::Delete(IN LPCTSTR table,IN LPCTSTR format,...)
		{
			if(!connect()) return -10;

			String part_where;
			if(format!=NULL && *format!=0)
			{
				va_list ap;
				va_start(ap,format);
				part_where=String::Format(format,ap);
				va_end(ap);
			}
			String sql = _pStatement->Delete(part_where).ToString(table);
			int result = (int)ExecuteNonQuery(sql,false);
			Close();
			return result;
		}
		int IDB::DeleteByID(IN LPCTSTR table,IN int id,IN LPCTSTR field)
		{
			return Delete(table,_T("%s=%d"),field,id);
		}
		int IDB::Count(IN LPCTSTR table,IN LPCTSTR format,...)
		{
			String sql_where;
			if(format!=NULL && *format!=0)
			{
				va_list ap;
				va_start(ap,format);
				sql_where=String::Format(format,ap);
				va_end(ap);
			}	

			String sql = _pStatement->Count().Where(sql_where).ToString(table);
			return GetSingle_Int32(sql,-1);
		}
		INT64 IDB::Execute(IN LPCTSTR sql,IN bool isBackRowID,IN bool isErrorWithoutRowEffect)
		{
			assert(sql!=NULL && *sql!=0);

			IError::Clear();
			if(!connect()) return -100;

			INT64 result = ExecuteNonQuery(sql,isBackRowID);
			if(!isBackRowID && result==0 && isErrorWithoutRowEffect)
			{
				m_nLastError = ERROR_DATABASE_FAILURE;
				m_sLastError = getLastError();
			}
			Close();
			return result;
		}
		INT64 IDB::Execute(IN ISqlStatement* sql,IN LPCTSTR tableName)
		{
			assert(!sql->IsEmpty());

			IError::Clear();
			if(!connect()) return -100;

			INT64 result = 0x0;
			String commandText;
			if(sql->Is(ST_INSERT) && sql->IsProperty(STATEMENT_PROPERTY_INSERT_IS_MULTI))
			{
				int multi_left = 0;
				if(!BeginTransaction())
				{
					result = -101;
					goto _BACK_;
				}

				do 
				{
					commandText = sql->ToString(tableName,&multi_left);
					int nRows = (int)ExecuteNonQuery(commandText,false);
					if(nRows<0)
					{
						RollbackTransaction();
						result = -102;
						goto _BACK_;
					}
					result += nRows;
				} while (multi_left>0);
				CommitTransaction();
			}
			else
			{
				bool isBackID = sql->Is(ST_INSERT) && sql->IsProperty(STATEMENT_PROPERTY_INSERT_IS_BACK_AUTO_ID);
				commandText = sql->ToString(tableName);
				result = ExecuteNonQuery(commandText,isBackID);
				if(sql->Is(ST_CREATE_TABLE))//Create Table语句成功时返回的值是0,这里将其转成返回TRUE
					result =  result == 0 && this->IsSuccess() ? TRUE:FALSE;
			}
_BACK_:
			Close();
			return result;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////



		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IDB::GetSingle(IN LPCTSTR sql,OUT std::string& result)
		{
			IError::Clear();
			if(!connect()) return false;

			result.clear();
			IDataReader* pReader=NULL;
			if(!ExecuteQuery(sql,&pReader))
			{
				Close();
				return false;
			}

			if(pReader->Read())
			{
				String temp = pReader->GetString(0);//返回第一行第一列的值
				result = Convert::ToStringA(temp,IO::EC_ANSI);
			}

			pReader->Close();
			DELETE_OBJECT(pReader);
			Close();
			return true;
		}
		INT32 IDB::GetSingle_Int32(IN LPCTSTR sql,IN INT32 def)
		{
			IError::Clear();
			if(!connect()) return def;

			INT32 result = 0x0;
			IDataReader* pReader=NULL;
			if(!ExecuteQuery(sql,&pReader))
			{
				Close();
				return def;
			}

			if(pReader->Read())
				result = pReader->GetInt(0);//返回第一行第一列的值

			pReader->Close();
			DELETE_OBJECT(pReader);
			Close();
			return result;
		}
		INT64 IDB::GetSingle_Int64(IN LPCTSTR sql,IN INT64 def)
		{
			IError::Clear();
			if(!connect()) return def;

			INT64 result = 0x0;
			IDataReader* pReader=NULL;
			if(!ExecuteQuery(sql,&pReader))
			{
				Close();
				return def;
			}

			if(pReader->Read())
				result = pReader->GetInt64(0);//返回第一行第一列的值

			pReader->Close();
			DELETE_OBJECT(pReader);
			Close();
			return result;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		////// internal
		void IDB::Lock(IN int line)
		{ 
			ILock::Lock();
			_lockCounter++;
			//DEBUG_STRING("DBH:Lock(times:%d,line:%d)\n",_lockCounter,line);
		}
		void IDB::Unlock(IN int line)
		{ 
			_lockCounter--;
			//DEBUG_STRING("DBH:Unlock(times:%d,line:%d)\n",_lockCounter,line);
			//if(_lockCounter<0)
			//{
			//	int exception = 0;
			//}
			ILock::Unlock();
		}

	}}//end namespace
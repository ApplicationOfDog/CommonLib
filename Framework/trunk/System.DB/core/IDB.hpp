#pragma once
using namespace System::Pattern;

namespace System{
	namespace DB{

		/// <summary>
		/// 数据库操作基类,PS.多线程安全
		/// </summary>
		class API_DB IDB:public IError,public ILock
		{
		protected:
			DatabaseType	_dbType;			//数据库类型
			ISqlStatement*	_pStatement;		//sql语句生成器
			int				_connectionCount;	//当前链接数
			int				_transactionCount;	//打开事务处理的次数
			DWORD			_asserts;			//断言标识
			int				_lockCounter;
			int				_multi_max;			//insert批量操作时允许的最多记录数量

		public:
			void*			m_pDll;				//针对动态加载dll的情况

		public:
			static IDB* CreateObject(IN DatabaseType dt);
			static void DeleteObject(IN void* p);

		public:	
			///<summary>对象构造</summary>
			IDB();
			virtual ~IDB();

			///<summary>
			/// 当前类是动态加载dll(数据库对应的API操作dll,例如mysql的libmysql.dll)
			/// 所以该函数要在所有对象进行数据库操作之前调用
			///</summary>
			virtual bool LoadDll(IN LPCTSTR filePath)=0;

			///<summary>获取SQL语句构造对象</summary>
			///<param name="structure">表结构</param>
			///<param name="statType">语句类型</param>
			///<param name="isNew">true:创建一个新对象,false:返回DB实例中的成员对象</param>
			///<returns>SQL语句构造对象</returns>
			virtual ISqlStatement* Statement(IN TableStructure* structure = NULL,IN StatementType statType = ST_SELECT,IN bool isNew=false);

			///<summary>初始化:于使用该对象之前调用</summary>
			///<param name="params">数据库初始化参数
			///SqliteDB:
			/// filePath:"数据库物理文件路径(必须是文件全路径)",		例如:"c:\...\xx.db"
			/// encryptKey:"如果目标数据库是加密数据库,则传入密钥。否则不要设置该参数或为空",		例如:"123456"
			/// waitTimesInBusy:当数据库处于lock状态时等待次数(每次20ms),默认值:"100"
			/// sleepTimeInBusy:当数据库处于lock状态时每次休眠时间(默认20ms)
			///
			///Mysql:
			/// host:"主机地址",		例如:"localhost"
			/// port:主机端口,			例如:3306
			/// userName:"帐号用户名",	例如:"root"
			/// password:"帐号密码",	例如:"xxx"
			/// database:"数据库名称",	例如:"xxx"
			/// multi_max:"最大允许多少记录同时插入",例如:"2000" ,通过show VARIABLES like '%max_allowed_packet%'可以看到数据库接受的最大容量
			///</param>
			///<returns>是否成功</returns>
			virtual bool Initialize(IN map<string,string>& params) = 0;

			///<summary>获取数据库的相关信息,用于简单区分显示</summary>
			virtual LPCTSTR GetInforamtion() = 0;

			///<summary>判断是否合法对象</summary>
			///<returns>true:合法,false:非法对象(例如未初始化)</returns>
			virtual bool IsValid() = 0;

			///<summary>连接数据库
			///注意:Open与Close必须要成对出现
			///</summary>
			///<param name="isTransaction">连接成功后是否开始一个事务处理</param>
			///<returns>执行结果,true:连接成功,false:连接失败</returns>
			virtual bool Open(bool isTransaction)=0;

			///<summary>关闭数据库</summary>
			virtual bool Close() = 0;

			///<summary>
			/// 在BeginTransaction和CommitTransaction之间批量操作可以提交
			///</summary>
			///开始事务处理
			virtual bool BeginTransaction() = 0;
			///提交并结束事务处理
			virtual bool CommitTransaction() = 0;
			///事务回滚
			virtual bool RollbackTransaction() = 0;

			///<remark>StartSelect和EndSelect配合使用完成select语句的功能</remark>
			///<summary>
			/// 执行select语句
			/// SELECT 列名称0,列名称1,... FROM 表名称 WHERE 列名称 = 某值
			/// 备注:StartSelect成功之后IDB进入lock状态,调用EndSelect之后才会Unlock,所以StartSelect和EndSelect一定要配对使用
			///      StartSelect失败时不会进入lock状态,所以也不需要调用EndSelect
			///</summary>
			///<returns>NULL表示查询失败,否则表示查询成功</returns>
			///<param name="fields">返回的字段列表</param>
			///<param name="format">条件部分即where语句的格式化字符串</param>
			virtual IDataReader* StartSelect(IN LPCTSTR table,IN LPCTSTR fields=_T("*"),IN LPCTSTR format=NULL,...);
			virtual IDataReader* StartSelect(IN ISqlStatement* sql);
			virtual IDataReader* StartSelectBy0(IN LPCTSTR table,IN LPCTSTR fields=_T("*"),IN LPCTSTR where=NULL,IN LPCTSTR orders=NULL,IN int limit_count=-1,IN int limit_skip=-1);
			///<param name="format">完整sql语句(不是单是where部分)的格式化字符串</param>
			virtual IDataReader* StartSelectBy1(IN LPCTSTR format,...);
			///<summary>用于释放StartSelect分配的所有资源和关闭链接</summary>
			virtual void EndSelect(IN IDataReader* pReader);

			///<summary>
			///执行update命令,
			///UPDATE 表名称 SET 列名称0 = 新值,列名称1 = 新值 WHERE 列名称 = 某值
			///</summary>
			///<param name="wherFormat">where条件的格式字符串</param>
			virtual bool Update(IN LPCTSTR table,IN LPCTSTR set,IN LPCTSTR whereFormat,...);
			virtual bool UpdateByID(IN LPCTSTR table,IN LPCTSTR set,IN int id,IN LPCTSTR field=_T("ID"));

			///<summary>
			/// 执行insert命令
			///INSERT INTO 表名称 (列1, 列2,...) VALUES (值1, 值2,....)
			///</summary>
			///<param name="isBackID">是否返回自增长ID</param>
			///<returns>
			/// 如果isBackID为false,则返回0或1表示执行成功与否。
			/// 如果isBackID为true,则返回自增长ID值(-1表示执行失败)
			///</returns>
			virtual INT64 Insert(IN LPCTSTR table,IN LPCTSTR fields,IN LPCTSTR values,IN bool isBackID);
			///<returns>返回自增长ID值(-1表示执行失败)</returns>
			virtual INT64 InsertByBackID(IN LPCTSTR table,IN LPCTSTR fields,IN LPCTSTR values){ return Insert(table,fields,values,true); }

			///<summary>
			/// 执行delete命令
			///DELETE FROM 表名称 WHERE 列名称 = 值
			///</summary>
			///<param name="format">where条件的格式字符串</param>
			///<returns>返回受影响的行数</returns>
			virtual int Delete(IN LPCTSTR table,IN LPCTSTR format,...);
			virtual int DeleteByID(IN LPCTSTR table,IN int id,IN LPCTSTR field=_T("ID"));

			///<summary>
			/// 是否存在记录
			/// select count(*) from xxx where yyy 
			///</summary>
			///<param name="format">where条件的格式字符串</param>
			///<returns>小于0表示失败,其他表示返回的记录数</returns>
			virtual int Count(IN LPCTSTR table,IN LPCTSTR format,...);

			///<summary>
			/// 执行一条计算查询结果语句，返回查询结果（result）。
			///</summary>
			///<param name="sql">计算查询结果语句</param>
			///<param name="result">查询结果</param>
			///<returns>执行结果,true:执行成功,false:失败</returns>
			virtual bool GetSingle(IN LPCTSTR sql,OUT std::string& result);
			///<param name="def">查询失败时返回的值</param>
			///<returns>执行成功返回查询结果值,否则返回def</returns>
			virtual INT32 GetSingle_Int32(IN LPCTSTR sql,IN INT32 def);
			virtual INT64 GetSingle_Int64(IN LPCTSTR sql,IN INT64 def);

			///<summary>
			/// 执行非查询操作(更新或删除或插入),返回受影响的行数 
			///</summary>
			///<param name="sql">sql语句</param>
			///<param name="isBackRowID">是否返回自增长id</param>
			///<param name="isErrorWithoutRowEffect">isBackRowID=true时有效,当没有任何行数受影响是否认定为错误</param>
			///<returns>返回值根据isBackRowID值不同而不同
			/// isBackRowID为false情况,0表示没有记录受影响,大于0表示受影响的行数,小于0表示错误码
			/// isBackRowID为true情况,大于0表示返回ID,小于等于0表示错误码
			///</returns>
			virtual INT64 Execute(IN LPCTSTR sql,IN bool isBackRowID,IN bool isErrorWithoutRowEffect);
			///<returns>
			/// 1.如果sql是ST_INSERT语句且要求返回自增长ID,大于0表示返回ID,小于等于0表示错误码
			/// 2.如果sql是ST_CREATE_TABLE,返回TRUE(1)表示成功,FALSE(0)表示失败
			/// 3.其他情况:0表示没有记录受影响,大于0表示受影响的行数,小于0表示错误码 
			///</returns>
			virtual INT64 Execute(IN ISqlStatement* sql,IN LPCTSTR tableName=NULL);

		public:
			///<summary>
			/// 获取数据表结构信息(所有字段结构)
			///</summary>
			///<param name="table">表名称</param>
			///<param name="pStructure">返回的表结构对象</param>
			///<returns>执行结果,true:执行成功,false:失败</returns>
			virtual bool GetTableStructureInfo(IN LPCTSTR table,OUT TableStructure* pStructure) = 0;

			///<summary>
			/// 获取当前数据库中所有的表名称
			///</summary>
			virtual bool GetAllTableName(OUT vector<String>& tables)=0;

			///<summary>
			/// 判断数据表是否存在
			///</summary>
			virtual bool IsTableExist(IN LPCTSTR name) = 0;

			///<summary>获取数据库相关信息</summary>
			///<param name="command">查询命令</param>
			///<param name="pInfo">返回的信息字符串(可能是普通字符串,也可能是json字符串)</param>
			///<returns>是否成功</returns>
			virtual bool GetInformation(IN LPCSTR command,OUT string* pInfo) = 0;

		protected://使用如下函数时，需要自己Open和Close	
			///<summary>
			/// 执行SQL语句，返回执行结果集合
			///</summary>
			///<param name="sql">sql命令</param>
			///<param name="pReader">返回数据集读取器</param>
			///<returns>执行结果,true:执行成功,false:失败</returns>
			virtual bool ExecuteQuery(IN LPCTSTR sql,OUT IDataReader** pReader)=0;

			///<summary>
			/// 执行非查询操作（更新或删除） ,返回受影响的行数 
			///</summary>
			///<param name="commandText">sql命令</param>
			///<param name="isBackRowID">是否返回自增长id</param>
			///<returns>返回值根据isBackRowID值不同而不同
			/// isBackRowID为false情况,0表示没有记录受影响,大于0表示受影响的行数,小于0表示错误码
			/// isBackRowID为true情况,大于0表示返回ID,小于等于0表示错误码
			///</returns>
			virtual INT64 ExecuteNonQuery(IN LPCTSTR commandText,IN bool isBackRowID)=0;

		protected:
			virtual bool connect() = 0;
			virtual String getLastError()=0;
			virtual void Lock(IN int line);
			virtual void Unlock(IN int line);			
		};

	}}//end namespace
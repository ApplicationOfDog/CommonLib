#pragma once
using namespace System::Pattern;

namespace System{
	namespace DB{

	#ifndef _NO_SQLITE3_CALLBACK_ //不定义sqlite3_callback函数指针
	typedef int (*sqlite3_callback)(void*,int,char**, char**);
	#endif

	/// <summary>
	/// Sqlite数据库,PS.多线程安全,支持数据库加密存储
	/// </summary>
	class API_DB SqliteDB:public IDB
	{
	private:
		void*			_pDB;		//数据对象指针
		String			_filePath;	//数据库文件全路径
		char*			_encryptKey;//加密密钥,NULL表示不设置密钥即明文存储
		int				_nWaitTimesInBusy;//当数据库处于lock状态时等待次数(每次休眠_nSleepTimeInBusy)
		int				_nSleepTimeInBusy;//当数据库处于lock状态时每次休眠时间(默认20ms)
		bool			_isUnicodeCharset;

	public:	
		///<summary>对象构造</summary>
		///<param name="filePath">数据库物理文件路径(必须是文件全路径)</param>
		///<param name="encryptKey">如果目标数据库是加密数据库,则传入密钥。否则传入NULL即可</param>
		///<param name="isConnect">是否立即连接数据库</param>
		SqliteDB(IN LPCTSTR filePath=NULL,IN LPCSTR encryptKey=NULL,IN bool isConnect=false);
		virtual ~SqliteDB();

		///<summary>当前类是动态加载libmysql.dll,
		/// 所以该函数要在所有对象进行数据库操作之前调用
		///</summary>
		bool LoadDll(IN LPCTSTR filePath);
		///<summary>判断是否合法对象</summary>
		///<returns>true:合法,false:非法对象(例如未初始化)</returns>
		bool IsValid();

		String FilePath();
		LPCSTR EncryptKey();

		///<summary>初始化:于使用该对象之前调用</summary>
		///<param name="params">
		/// filePath:"数据库物理文件路径(必须是文件全路径)",		例如:"c:\...\xx.db"
		/// encryptKey:"如果目标数据库是加密数据库,则传入密钥。否则不要设置该参数或为空",		例如:"123456"
		/// waitTimesInBusy:当数据库处于lock状态时等待次数(每次20ms),默认值:"100"
		/// sleepTimeInBusy:当数据库处于lock状态时每次休眠时间(默认20ms)
		/// isUnicodeCharset:1表示使用unicode api读写数据库,0表示使用ansi编码
		///</param>
		bool Initialize(IN map<string,string>& params);

		///<summary>获取数据库的相关信息,用于简单区分显示</summary>
		LPCTSTR GetInforamtion();
		
		///<summary>连接数据库
		///注意:Open与Close必须要成对出现
		///</summary>
		///<param name="isTransaction">连接成功后是否开始一个事务处理</param>
		///<returns>执行结果,true:连接成功,false:连接失败</returns>
		bool Open(bool isTransaction);
		///<summary>关闭数据库</summary>
		bool Close();

		///<summary>
		/// 在BeginTransaction和CommitTransaction之间批量操作可以提交
		///</summary>
		///开始事务处理
		bool BeginTransaction();
		///提交并结束事务处理
		bool CommitTransaction();
		///事务回滚
		bool RollbackTransaction();

	public:
		///<summary>
		/// 获取数据表结构信息(所有字段结构)
		///</summary>
		///<param name="isGetDetail">是否获取除基本信息之外的信息
		/// 1.创建表的sql语句
		/// 2.字段是否自增长
		///</param>
		///<returns>执行结果,true:执行成功,false:失败</returns>
		bool GetTableStructureInfo(IN LPCTSTR table,OUT TableStructure* pStructure);
		///<summary>
		/// 获取当前数据库中所有的表名称
		///</summary>
		bool GetAllTableName(OUT vector<String>& tables);
		///<summary>
		/// 判断数据表是否存在
		///</summary>
		bool IsTableExist(IN LPCTSTR name);

	protected://使用如下函数时，需要自己Open和Close	
		///<summary>
		/// 执行SQL语句，返回执行结果集合
		///</summary>
		///<param name="sql">sql命令</param>
		///<param name="fnCallBack">获取到命令执行结果时的回调函数,
		/// select语句返回几行就会被调用几次
		///</param>
		///<returns>执行结果,true:执行成功,false:失败</returns>
		bool ExecuteQuery(IN LPCTSTR sql,IN sqlite3_callback fnCallBack);
		///<param name="pReader">结果集合的阅读器</param>
		bool ExecuteQuery(IN LPCTSTR sql,OUT IDataReader** pReader);

		///<summary>
		/// 执行非查询操作（更新或删除） ,返回受影响的行数 
		///</summary>
		///<param name="commandText">sql命令</param>
		///<param name="isBackRowID">是否返回自增长id</param>
		///<returns>返回值根据isBackRowID值不同而不同
		/// isBackRowID为false情况,0表示没有记录受影响,大于0表示受影响的行数,小于0表示错误码
		/// isBackRowID为true情况,大于0表示返回ID,小于等于0表示错误码
		///</returns>
		INT64 ExecuteNonQuery(IN LPCTSTR commandText,IN bool isBackRowID);

		///<summary>获取数据库相关信息</summary>
		///<param name="command">查询命令</param>
		///<param name="pInfo">
		/// 返回的信息字符串(可能是普通字符串,也可能是json字符串)
		///</param>
		///<returns>是否成功</returns>
		bool GetInformation(IN LPCSTR command,OUT string* pInfo);

	protected:
		bool connect();
		String getLastError();
		static int do_db_busy_handler(void *ptr,int count);
	};

}}//end namespace
#pragma once
using namespace System::Pattern;

namespace System{
	namespace DB{

	/// <summary>
	/// Mysql数据库,PS.多线程安全
	/// </summary>
	class API_DB MysqlDB:public IDB
	{
	private:
		string			_host;
		UINT32			_port;
		string			_userName;
		string			_password;
		string			_database;
		string			_charset;
		void*			_pDB;//数据对象指针

	public:
		///<summary>对象构造</summary>
		MysqlDB();
		virtual ~MysqlDB();

		///<summary>当前类是动态加载libmysql.dll,
		/// 所以该函数要在所有对象进行数据库操作之前调用
		///</summary>
		bool LoadDll(IN LPCTSTR filePath);
		///<summary>判断是否合法对象</summary>
		///<returns>true:合法,false:非法对象(例如未初始化)</returns>
		bool IsValid();

		///<summary>初始化:于使用该对象之前调用</summary>
		///<param name="params">
		/// host:"主机地址",		例如:"localhost"
		/// port:主机端口,			例如:3306
		/// userName:"帐号用户名",	例如:"root"
		/// password:"帐号密码",	例如:"xxx"
		/// database:"数据库名称",	例如:"xxx"
		/// charset :"字符集名称",	例如:"utf8","GBK"(默认值),"utf8mb4"
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
		///<param name="table">表名称</param>
		///<param name="pStructure">返回的表结构对象</param>
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

		///<summary>获取数据库相关信息</summary>
		///<param name="command">查询命令
		/// GetCharset:获取当前连接的数据库使用的字符集
		///</param>
		///<param name="pInfo">
		/// 返回的信息字符串(可能是普通字符串,也可能是json字符串)
		/// 实例:
		/// GetCharset => {"csname":"utf8mb4","name":"utf8mb4_general_ci","comment":"UTF-8 Unicode","dir":"","number":45,"state":17377,"mbminlen":1,"mbmaxlen":4}
		///</param>
		///<returns>是否成功</returns>
		bool GetInformation(IN LPCSTR command,OUT string* pInfo); 

	protected://使用如下函数时，需要自己Open和Close	
		///<summary>
		/// 执行SQL语句，返回执行结果集合
		///</summary>
		///<param name="sql">sql命令</param>
		///<param name="fnCallBack">获取到命令执行结果时的回调函数,
		/// select语句返回几行就会被调用几次
		///</param>
		///<returns>执行结果,true:执行成功,false:失败</returns>
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

	private:
		bool connect();
		String getLastError();
		String getLastError(IN bool isGetCode);
	};

}}//end namespace


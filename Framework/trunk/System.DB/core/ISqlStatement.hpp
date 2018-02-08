#pragma once
#include <list>
#include <queue>

namespace System{
	namespace DB{

	///<summary>数据库类型</summary>
	enum DatabaseType
	{
		DBT_UNKNOWN=-1,
		DBT_SQLITE=0,
		DBT_MYSQL,
	};
	/// <summary>
	/// SQL语句类型
	/// </summary>
	enum StatementType
	{
		ST_UNKNOWN=0,
		ST_COMMAND_TEXT,//纯文本的SQL语句
		ST_SELECT,		//select语句
		ST_UPDATE,		//update语句
		ST_INSERT,		//insert语句
		ST_DELETE,		//delete语句
		ST_COUNT,		//select count(*)语句
		ST_CREATE_TABLE,//create table语句
	};
	/// <summary>
	/// SQL语句中具体某一部分
	/// </summary>
	enum StatementPart
	{
		STATEMENT_PART_AUTO = 0,			//自动设置
		STATEMENT_PART_WHERE_AND,			//where条件部分(字段-值对,并且使用and与之前的条件连接)
		STATEMENT_PART_WHERE_OR,			//where条件部分(字段-值对,并且使用or与之前的条件连接)
		STATEMENT_PART_WHERE_WHOLE,			//where条件部分(多字段组成的where完整块)
		STATEMENT_PART_SELECT_BACK,			//select语句的返回字段
		STATEMENT_PART_SELECT_ORDER,		//select语句的排序字段(order by),单字段
		STATEMENT_PART_SELECT_ORDER_WHOLE,	//select语句的排序字段(order by),一个整体块
		STATEMENT_PART_SELECT_LIMIT,		//select语句的换页字段(limit skip,count)
		STATEMENT_PART_UPDATE_SET,			//update语句的set部分(字段-值对)
		STATEMENT_PART_UPDATE_SET_WHOLE,	//update语句的set部分(完整的set部分)
		STATEMENT_PART_UPDATE_INCREAMENT,	//update table set A = A + X,字段自增或自减语句

		STATEMENT_PART_INSERT_SINGLE,		//insert语句(单条插入)
		STATEMENT_PART_INSERT_MULTI,		//insert语句(批量插入)
		STATEMENT_PART_CREATE_TABLE,		//CREATE TABLE语句的表名称部分
		STATEMENT_PART_CREATE_TABLE_FIELD,	//CREATE TABLE语句的字段定义部分
	};
	///<summary>语句属性特征</summary>
	enum StatementProperty
	{
		STATEMENT_PROPERTY_INSERT_IS_BACK_AUTO_ID = 0x1, //inert操作是否返回自增长ID
		STATEMENT_PROPERTY_INSERT_IS_MULTI = 0x2,		 //inert操作是否为批量插入
	};
	///<summary>字段标志</summary>
	enum StatementFieldFlags
	{
		STATEMENT_FIELD_USEABLE = 0x1,//是否可以用状态
		STATEMENT_FIELD_INC_OPERATION = 0x2,//值中已经包含了操作符
	};

	class API_DB FieldValue
	{
	public:
		String			m_name;	//域名称
		String			m_value;//域值
		StatementPart	m_part;	//操作类型
		DWORD			m_flags;//标志位,详细值请看StatementFieldFlags

	public:
		FieldValue(IN StatementPart part);
		FieldValue(IN LPCTSTR name=NULL,IN LPCTSTR value=NULL,IN StatementPart part=STATEMENT_PART_AUTO);
		FieldValue& operator=(IN const FieldValue& v);
		
		void Clear();
		void Copy(IN const FieldValue& v);
	};
	
	
	/// <summary>
	/// 表数据结构
	/// </summary>
	class API_DB ISqlStatement
	{
	protected:
		typedef std::vector<FieldValue*> FieldColletion;

		TableStructure*			_pStructure;
		StatementType			_type;
		DWORD					_properties;
		///<summary>一开始使用vector当数据量上万时,CombineField函数变得异常的慢</summary>
		FieldColletion			_fieldValues;
		String					_commandText;
		DatabaseType			_databaseType;
		int						_multi_max;		//批量操作时允许的最大记录数量

	public:
		ISqlStatement(IN TableStructure* structure=NULL,IN StatementType type = ST_UNKNOWN);
		ISqlStatement(IN LPCTSTR commandText);
		~ISqlStatement();

		///<summary>设置表结构</summary>
		void Structure(IN TableStructure* val);
		///<summary>设置语句类型</summary>
		void Type(IN StatementType val);

		///<summary>批量操作时允许的最大记录数量</summary>
		void MultiMax(IN int val);
		int MultiMax();

		///<summary>清空对象</summary>
		virtual void Clear();

		///<summary>是否为空对象</summary>
		virtual bool IsEmpty();

		///<summary>是否为指定类型的语句</summary>
		virtual bool Is(IN StatementType type);

		///<summary>属性特征判断</summary>
		///<param name="val">StatementProperty组合值</param>
		bool IsProperty(IN DWORD val);
		DWORD Properties();		

		///<summary>对象拷贝</summary>
		///<param name="sql">目标对象</param>
		///<param name="isStructure">是否连表结构一起复制</param>
		void Copy(IN ISqlStatement& sql,IN bool isStructure=false);

		///<summary>纯文本的SQL语句
		/// 情况1.ST_COMMAND_TEXT类型可以在SQLStatement构造函数之后直接调用
		/// 情况2.除ST_COMMAND_TEXT外其他类型的,必须在调用ToString函数之后才能得到正确值,否则会直接返回空字符串.
		///</summary>
		LPCTSTR CommandText();

		///<summary>
		/// 编写select部分,即select语句的返回字段列表
		/// SELECT 列名称0,列名称1,... FROM 表名称 WHERE 列名称 = 某值 ORDER BY 列名称x,列名称y LIMIT n,m
		///</summary>
		///<param name="fields">字段列表,不同字段用','隔开,例如:A,B,C,...</param>
		ISqlStatement& Select(IN LPCTSTR fields=NULL);
		/// SELECT count(*) FROM 表名称 WHERE 列名称 = 某值
		ISqlStatement& Count(IN LPCTSTR whereFormat=NULL,...);
		///<summary>
		///update命令,
		///UPDATE 表名称 SET 列名称0 = 新值,列名称1 = 新值 WHERE 列名称 = 某值
		///</summary>
		ISqlStatement& Update(IN LPCTSTR setFormat=NULL,...);
		///<summary>
		/// insert命令
		///INSERT INTO 表名称 (列1, 列2,...) VALUES (值1, 值2,....)
		///</summary>
		ISqlStatement& Insert(IN bool isBackID=false,IN LPCTSTR fields=NULL,IN LPCTSTR valuesFormat=NULL,...);
		///<summary>
		/// delete命令
		///DELETE FROM 表名称 WHERE 列名称 = 值
		///</summary>
		ISqlStatement& Delete(IN LPCTSTR whereFormat=NULL,...);
		///<summary>
		/// create table命令
		///CREATE TABLE IF NOT EXISTS "表名称"(
		///"列名称0" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
		///"列名称0" TEXT,
		///)
		///</summary>
		ISqlStatement& CreateTable(IN LPCTSTR tableName);

		///<summary>
		/// 编写where部分条件
		///</summary>
		ISqlStatement& Where(IN LPCTSTR format,...);//默认使用and与以前的条件链接
		///<param name="text">条件字段名称</param>
		///<param name="isAndWithFront">与前面的条件的连接方式,true表示and,false表示or</param>
		ISqlStatement& WhereBy(IN LPCTSTR text,IN bool isAndWithFront);	
		///<param name="value">条件字段值</param>
		ISqlStatement& WhereItem(IN LPCTSTR name,IN LPCTSTR value,IN bool isAndWithFront=true);
		ISqlStatement& WhereItem(IN LPCTSTR name,IN int value,IN bool isAndWithFront=true);		
		ISqlStatement& WhereItem(IN LPCTSTR name,IN UINT32 value,IN bool isAndWithFront=true);
		ISqlStatement& WhereItem(IN LPCTSTR name,IN INT64 value,IN bool isAndWithFront=true);
		ISqlStatement& WhereItem(IN LPCTSTR name,IN UINT64 value,IN bool isAndWithFront=true);
		///<summary>in条件:AA in (x0,x1,x2...)</summary>
		///<param name="isNot">false:表示in,true:表示not in</param>
		ISqlStatement& WhereIn(IN LPCTSTR name,IN LPCTSTR values,IN bool isAndWithFront=true,IN bool isNot = false);
		
		///<summary>
		/// 编写order by部分,用于结果排序
		///</summary>
		///<param name="field">排序字段名称</param>
		///<param name="isAsc">true:升序排列即ASC,false:降序排列即DESC</param>
		ISqlStatement& OrderBy(IN LPCTSTR field,IN bool isAsc);
		///<param name="fields">排序字段名称集合,例如 Field0 ASC,Field1 DESC</param>
		ISqlStatement& OrderBy(IN LPCTSTR fields);
		///<summary>
		/// select * from account limit 10,9
		/// 编写limit部分,一般用于结果分页查询
		///</summary>
		///<param name="skip">跳过的行数</param>
		///<param name="count">取回的行数</param>
		ISqlStatement& Limit(IN int skip,IN int count);

		///<summary>
		/// 生成sql语句
		///</summary>
		String ToString(IN LPCTSTR tableName=NULL,OUT int* multi_left = NULL);

	public:
		///<summary>
		/// 字段映射:字段名称-字段值映射
		///</summary>
		///<param name="name">字段名称,使用String类型最大的好处就是支持同时"xx"和_T("xx")</param>
		///<param name="value">字段值</param>
		///<param name="part">所属语句部分类型,STATEMENT_PART_XXX</param>
		bool Field(IN String name,IN LPCTSTR value,IN StatementPart part = STATEMENT_PART_AUTO);
		bool Field(IN String name,IN int value,IN StatementPart part = STATEMENT_PART_AUTO);
		bool Field(IN String name,IN UINT32 value,IN StatementPart part = STATEMENT_PART_AUTO);
		bool Field(IN String name,IN INT64 value,IN StatementPart part = STATEMENT_PART_AUTO);
		bool Field(IN String name,IN UINT64 value,IN StatementPart part = STATEMENT_PART_AUTO);
		bool Field(IN String name,IN double value,IN StatementPart part = STATEMENT_PART_AUTO);
		bool Field(IN String name,IN bool value,IN StatementPart part = STATEMENT_PART_AUTO);
		///<summary>针对BLOB类型字段</summary>
		bool Field(IN String name,IN unsigned char* value,IN int len,IN StatementPart part = STATEMENT_PART_AUTO);
		bool Field(IN String name,IN FieldDataType type,
			IN bool isNotNull=false,IN bool isPrimaryKey=false,IN bool isAutoIncrement=false,
			IN int length=0,IN int decimals=0);

		///<summary>将之前的Field合并成一条Values(),针对insert批量插入</summary>
		///insert into tableName (A,B,C,D) values (a0,'b0' , 'c0' , d0),(a1,'b1' , 'c1' , d1),(a2,'b3' , 'c2' , d2);
		///update tableName set A = case id WHEN 1 THEN a0 WHEN 2 THEN a1 WHEN 3 THEN a2 END,
		///						B = case id WHEN 1 THEN 'b0' WHEN 2 THEN 'b1' WHEN 3 THEN 'b2' END,
		///where id in (1,2,3)
		void CombineField();

	private:
		String get_select(IN LPCTSTR tableName,IN bool isCount);
		String get_update(IN LPCTSTR tableName);
		///<summary>生成insert语句</summary>
		///<param name="multi_max">如果当前是批量插入,那么该值表示允许最大的插入记录数量</param>
		///<param name="multi_left">如果当前是批量插入,本次取走的记录之后剩下的记录数量</param>
		///<returns>返回完整的插入语句(如果批量插入超过multi_max,那么会预留一部分记录)</returns>
		String get_insert(IN LPCTSTR tableName,IN int multi_max,IN int* multi_left);
		String get_delete(IN LPCTSTR tableName);
		String get_create_table(IN LPCTSTR tableName);
		
		String get_table_name(IN LPCTSTR tableName);
		String get_where_part();
		String get_order_part();
		String remove_keyword(IN LPCTSTR text,IN LPCTSTR keyword);
		bool insert_field_value(IN String name,IN LPCTSTR value,IN StatementPart part,IN DWORD flags =0x0);
		String get_actual_value(IN String& name,IN LPCTSTR value,IN bool isFieldCheck);

	///<remark>虚函数定义</remark>
	protected:
		///<summary>子类重载函数:将目标字段转换成sql语句中使用的名称</summary>
		///<param name="filedName">字段名称</param>
		///<returns>返回实际使用的名称</returns>
		///<example>
		/// SQLite:convert_filedName("OrderID") => "[OrderID]"
		/// MySQL: convert_filedName("OrderID") => "OrderID"
		///</example>
		virtual String convert_filedName(IN LPCTSTR fieldName,IN StatementPart statPart)=0;
	};

}}//end namespace System.DB
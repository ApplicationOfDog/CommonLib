#pragma once

namespace System{
	namespace DB{

	/// <summary>
	/// 表字段数据类型
	/// </summary>
	enum FieldDataType
	{
		FDT_UNKNOWN=0,
		FDT_INTEGER,
		FDT_REAL,
		FDT_TEXT,	
		FDT_BLOB,
		FDT_DATETIME,
		FDT_NULL
	};
	/// <summary>
	/// 表字段数据结构
	/// </summary>
	class API_DB TableField
	{
	public:
		int				m_index;
		String			m_name;
		FieldDataType	m_dataType;		//字段类型
		int				m_dataLength;	//数据长度
		int				m_dataDecimals;	//小数位数
		String			m_sDataType;	//类型全字符串,例如TEXT(150)
		bool			m_isNotNull;	//true:不允许为NULL值,false:允许为NULL值
		bool			m_isPrimaryKey;
		
	public:///<remark>详细信息</remark>
		bool			m_isAutoIncrement;//是否自增长	

	public:
		TableField();
		void Clear();
		void DataType(IN LPCTSTR type);
		bool Is(IN FieldDataType val);

	public:
		static bool Convert(IN PCTSTR src,OUT FieldDataType* dest,OUT int* len,OUT int* decimal);
		static String Convert(IN FieldDataType src,IN int len,IN int decimal);

	private:
		static bool do_analyze(IN LPCTSTR srcText,IN LPCTSTR matchType,OUT int* len,OUT int* decimal);
	};

	/// <summary>
	/// 表数据结构
	/// </summary>
	class API_DB TableStructure
	{
	public:
		String				m_name;
		vector<TableField*> m_fields;
		String				m_createTableSql;//创建表的sql语句

	public:
		TableStructure(IN LPCTSTR name=NULL);
		~TableStructure();

		void Clear();
		bool IsValid();

		/// <summary>是否包含指定的字段名称</summary>
		bool HasField(IN LPCTSTR name);
		TableField* FindField(IN LPCTSTR name);
		TableField* FindField(IN int index);
	};

}}//end namespace
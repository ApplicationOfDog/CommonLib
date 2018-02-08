#pragma once

namespace System{
	namespace DB{

	class MysqlDll:public IError
	{
	private:
		String				_filePath;
		HMODULE				_hDll;
		map<string,FARPROC> _functions;

	public:
		MysqlDll(IN LPCTSTR filePath=NULL);
		~MysqlDll();

		bool Load(IN LPCTSTR filePath);
		void Free();

	public:
		MYSQL* init(MYSQL *mysql);
		MYSQL* real_connect(MYSQL *mysql, const char *host,
			const char *user,const char *passwd,const char *db,unsigned int port,const char *unix_socket,
			unsigned long clientflag);

		void close(MYSQL *sock);
		int real_query(MYSQL *mysql, const char *q,unsigned long length);
		int affected_rows(MYSQL *mysql);
		UINT64 insert_id(MYSQL *mysql);
		unsigned int get_errno(MYSQL *mysql);
		LPCSTR get_errorText(MYSQL *mysql);
		MYSQL_RES* store_result(MYSQL *mysql);
		MYSQL_FIELD* fetch_field(MYSQL_RES *result);
		MYSQL_ROW fetch_row(MYSQL_RES *result);
		unsigned long * fetch_lengths(MYSQL_RES *result);
		void free_result(MYSQL_RES *result);
		UINT64 num_rows(MYSQL_RES *res);
		int set_character_set(MYSQL *mysql, const char *csname);

		///<summary>获取当前连接的数据库使用的字符集</summary>
		///<param name="mysql">链接对象</param>
		///<param name="pInfo">
		/// 返回MY_CHARSET_INFO对象对应的json字符串,NULL表示不返回
		/// 例如:{"csname":"utf8mb4","name":"utf8mb4_general_ci","comment":"UTF-8 Unicode","dir":"","number":45,"state":17377,"mbminlen":1,"mbmaxlen":4}
		///</param>
		void get_character_set_info(IN MYSQL *mysql,OUT string* pInfo);

	private:
		FARPROC getProcessAddress(IN LPCSTR name);
	};

}}//end namespace System.DB
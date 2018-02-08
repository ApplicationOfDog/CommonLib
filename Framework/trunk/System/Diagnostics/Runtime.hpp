#pragma once
#include<time.h>
#include <string>
#include <map>
using namespace std;

namespace System{
	namespace Diagnostics{

		/// <summary>
		/// 运行时间结构
		/// </summary>
		class RTDuration
		{
		public:
			int			m_id;
			string		m_name;
			clock_t		m_begin;//开始(单位毫秒)
			clock_t		m_end;	//结束(单位毫秒)
			string		m_description;//描述

		public:
			RTDuration(IN LPCSTR name,IN bool isBegin,IN LPCSTR description=NULL);
			void Clear();
			void Name(IN LPCSTR text);
			void Description(IN LPCSTR text);

			void Start(IN LPCSTR name=NULL);
			void End();
			long Duration();
		};
		typedef map<string,RTDuration*> RTDurations;

		///<summary>
		/// 程序运行时
		///</summary>
		class API_SYSTEM Runtime
		{
		private:
			RTDurations	_durationMr;			//记录列表
			bool		_isEnabled;				//使能标识,true表示enabled,false表示disabled
			bool		_isOutputDebugView;	//true输出到DebugView,false输出到控制台

		public:
			Runtime(IN bool enabled = true,IN bool isOutputDebugView = true);
			~Runtime();

			void Set(IN bool enabled,IN bool isOutputToDebugView);
			void Clear();
			
			///<summary>开始记录模块执行时间,插入要测试代码开始位置</summary>
			///<param name="name">模块名称,NULL表示匿名即内部自动命名</param>
			///<param name="description">模块描述</param>
			///<param name="isOutput">是否输出开始结束信息到DebugView</param>
			///<returns>返回该模块在记录表中的ID,小于0表示错误码</returns>
			int Start(IN LPCSTR name=NULL,IN LPCSTR description=NULL,OUT clock_t* pBegin=NULL);
			
			///<summary>结束本次记录,插入要测试代码结束位置</summary>
			///<param name="name">模块名称,NULL表示结束第一条记录</param>
			///<returns>返回执行时间,小于0表示错误码</returns>
			long End(IN LPCSTR name=NULL);
			///<param name="id">模块ID,小于等于0表示结束第一条记录</param>
			long End(IN int id);

			///<summary>输出测试结果到DebugView或控制台中</summary>
			///<param name="containter">如果不为空则表示返回字符串不打印</param>
			void OutputDebugText(IN int nTab=0,IN String* containter = NULL);			

			///<summary>获取指定记录所花费的时间</summary>
			///<param name="name">模块名称,NULL表示结束第一条记录</param>
			///<param name="pBegin">返回起始时间(单位毫秒)</param>
			///<returns>返回执行时间(单位毫秒),小于0表示错误码</returns>
			long Duration(IN LPCSTR name=NULL,OUT clock_t* pBegin=NULL);
			///<param name="id">模块ID,小于等于0表示结束第一条记录</param>
			long Duration(IN int id,OUT clock_t* pBegin=NULL);

		private:
			RTDuration* find(IN int id);
			RTDuration* find(IN LPCSTR name);
			
		};

}}
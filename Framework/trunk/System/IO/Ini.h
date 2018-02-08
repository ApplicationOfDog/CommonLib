#pragma once

namespace System{
	namespace IO{

		#define INI_MAX_BUFFER 1024
		// 摘要:
		//     Ini文件读取器。
		class API_SYSTEM Ini
		{
		private:
			String _path;

		public:
			Ini(IN String path);
			/// <summary>
			/// 从文件中读取对应的字符串值
			/// </summary>
			/// <param name="secName">Section名称</param>
			/// <param name="keyName">Key名称</param>
			/// <param name="def">默认值，找不到Section.Key时返回</param>
			/// <returns>返回字符串值</returns>
			String ReadString(IN String secName,IN String keyName,IN String def=_T(""));
			int ReadInt(IN String secName,IN String keyName,IN int def=0);

			/// <summary>
			/// 使用XPath从文件中读取对应的字符串值，xpath:Section/Key
			/// </summary>
			String ReadString(IN String xpath,IN String def=_T(""));			
			int ReadInt(IN String xpath,IN int def=0);
		};
}}


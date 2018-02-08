#pragma once

namespace System
{
	class StringHelper
	{
	public:
		/// <summary>
		/// 在lpszSour中查找字符串lpszFind，lpszFind中可以包含通配字符‘?’
		/// </summary>
		/// <param name="nStart">在lpszSour中的起始查找位置</param>
		/// <returns>成功返回匹配位置，否则返回-1</returns>
		static int FindingString(LPCTSTR lpszSour,LPCTSTR lpszFind, int nStart = 0);
		
		/// <summary>
		/// 带通配符的字符串匹配
		/// 通配符意义: 
		///		‘*’	代表任意字符串，包括空字符串；
		///		‘?’	代表任意一个字符，不能为空；
		/// </summary>
		/// <param name="lpszSour">原字符串</param>
		/// <param name="lpszMatch">lpszMatch是一可以包含通配符的字符串</param>
		/// <param name="bMatchCase">为FALSE，不区分大小写，否则区分大小写</param>
		/// <returns>匹配，返回1；否则返回0。</returns>
		static bool Match(LPCTSTR lpszSour,LPCTSTR lpszMatch, bool bMatchCase);

		/// <summary>
		/// 多重匹配，不同匹配字符串之间用‘|’隔开
		///		如: “*.h|*.cpp”将依次匹配“*.h”和“*.cpp”
		/// </summary>		
		/// <param name="lpszSour">原字符串</param>
		/// <param name="lpszMatch">lpszMatch是一可以包含通配符的字符串</param>
		/// <param name="bMatchCase">是否大小敏感</param>
		/// <param name="nMatchLogic">等于0:不同匹配求或(即一个匹配即命中)，否则求与(全部匹配才算命中)</param>
		/// <returns>匹配,返回1.否则返回0</returns>
		static bool MultiMatch(LPCTSTR lpszSour,LPCTSTR lpszMatch, int nMatchLogic, bool bRetReversed, bool bMatchCase);
	};
}
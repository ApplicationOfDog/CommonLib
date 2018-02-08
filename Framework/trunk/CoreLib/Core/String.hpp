#pragma once
#include <vector>
#include <string>
using namespace std;

//#define _STRING_NO_CONVERT_		//屏蔽转换函数,即减少Convert.hpp依赖
//#define _STRING_NO_STRINGHELPER_  //屏蔽模糊匹配函数,即减少StringHelper.hpp依赖

namespace System
{
	///<summary>
	/// 字符串类
	/// 该字符串类是可排序的,即可以被std::map作为键使用
	///</summary>
	class API_CORE String :public stdString
	{
	public:
		static String Empty;

	public:
		///<summary>构造函数,初始化为""空字符串</summary>
		String():stdString() {}

#ifndef _STRING_NO_CONVERT_
		///<summary>将数值转为字符串类型</summary>
		explicit String(IN int num);//声明为explicit的构造函数不能在隐式转换中使用
		/// <param name="base">数值格式，2:2进制，8：8进制...</param>
		explicit String(IN LONG64 num,IN int base);
		String(IN LPCSTR s);//String sf(__FILE__); or String sf2 = __FILE__;
#endif		
		~String(){}
		
		///<summary>Duplicate the STL string copy constructor</summary>
		///<param name="s">The string to copy</param>
		///<param name="pos">The starting position in the string to copy from</param>
		///<param name="n">The number of characters to copy</param>
		String(IN const stdString &s,IN size_type pos = 0,IN size_type n = npos) : stdString(s, pos, npos) { }

		///<summary>以一个以'\0'结束的字符串创建String对象,NULL将当做空字符串处理</summary>
		///<param name="s">param[in] s The character array to copy into the new string</param>
		String(IN LPCTSTR s) : stdString(s==NULL?_T(""):s) { }		

		///<summary>Construct an ext_string from a character array and a length</summary>
		///<param name="s">The character array to copy into the new string</param>
		///<param name="n">The number of characters to copy</param>
		String(IN LPCTSTR s,IN size_type n) : stdString(s, n) { }

		///<summary>构造n个c字符的字符串</summary>
		///<param name="n">字符个数</param>
		///<param name="c">重复的字符</param>
		String(IN size_type n,IN value_type c) : stdString(n, c) { }

		///<summary>Create a string from a range</summary>
		///<param name="first>The first element to copy in</param>
		///<param name="last">last  The last element to copy in</param>
		template <class InputIterator> 
		String(InputIterator first, InputIterator last) : stdString(first, last) {}

		///<summary>
		/// 获取或设置当前实例中位于指定位置的字符
		/// 实例:String ss=_T("aa  ssss ");TCHAR c=ss[0];ss[0]=_T('b');// ss="ba  ssss "
		///</summary>
		///<param name="i">当前的字符串中的位置</param>
		TCHAR operator[](int i);

		///#region 类型强制转换
		///<summary>
		/// 将String类型转换成LPCTSTR
		/// 实例:String ss=_T("aa  ssss ");LPCTSTR ps=(LPCTSTR)ss;
		///</summary>
		operator LPCTSTR() const;
		operator LPTSTR() const;
#ifndef _STRING_NO_CONVERT_
		///<summary>将LONG64转为String类型</summary>
		String& operator=(IN LONG64 num);
#endif
		///<summary>
		/// 将TCAHR转为String类型
		/// 实例: String ss; ss=_T('a');
		///</summary>
		String& operator=(IN TCHAR c);
		///#endregion 类型强制转换
#ifndef _STRING_NO_CONVERT_
		String& operator+=(IN LONG64 num);
#endif
		String& operator+=(IN TCHAR c);
		String& operator+=(IN String& val);
		String& operator+=(IN LPCTSTR val);
#ifndef _STRING_NO_CONVERT_
		String operator+(IN LONG64 num);
#endif
		String operator+(IN TCHAR c);
#if IS_WINDOWS_OS
		String& operator+=(const _Myt& val);
		//对于不区分大小写的字符串比较直接用==操作即可,例如 String a=_T("a*"); bool test = a==_T("a*0");
#endif
#ifdef _UNICODE
		///<summary>与ANSI字符串比较</summary>
		///<param name="astr">ANSI编码字符串</param>
		///<returns>true:字符串内容相同,false:字符串内容不同</returns>
		bool operator==(IN LPCSTR astr);
#endif
	public:
		///<summary>判断当前实例是否为空还是仅由空白字符组成</summary>
		bool IsEmptyOrWhiteSpace();
		static bool IsEmptyOrWhiteSpace(IN String& value);

		///<summary>判断当前实例是否为空字符串</summary>
		bool IsEmpty();		
		static bool IsEmpty(IN String& value);

		///<summary>判断给定的字符串指针是否为null、空还是仅由空白字符组成</summary>
		static bool IsNullOrWhiteSpace(IN LPCTSTR value);

		///<summary>将当前字符串中的大写字符全部转成小写字符</summary>
		void ToLower();
		///<summary>将当前字符串中的小写字符全部转成大写字符</summary>
		void ToUpper();

		///<summary>根据分隔符分割字符串</summary>
		///<param name="separator">分隔符</param>
		///<param name="options">分割选项</param>
		///<param name="limit">返回集合的最大数量</param>
		///<returns>返回分割后的字符串集合</returns>
		vector<String> Split(IN TCHAR separator,IN StringSplitOptions options=SSO_None,IN size_type limit = npos) const;
		///<summary>根据字符串分隔符分割字符串</summary>
		vector<String> Split(IN LPCTSTR separator,IN StringSplitOptions options=SSO_None,IN size_type limit = npos) const;
		///<summary>根据长度分割字符串</summary>
		vector<String> Split(IN size_type chunklen) const;

		///<summary>比较两个指定的 System.String 对象（其中忽略或考虑其大小写）,并返回一个整数，指示二者在排序顺序中的相对位置</summary>
		///<param name="strA">要比较的第一个字符串,const String&amp;也是可以通过LPCTSTR构造对象的</param>
		///<param name="strB">要比较的第二个字符串</param>
		///<param name="ignoreCase">要在比较过程中忽略大小写，则为 true;否则为 false</param>
		///<returns>
		/// 一个 32 位带符号整数，指示两个比较数之间的词法关系. 值 Condition 小于零 strA 小于 strB。 零 strA 等于 strB。
		/// 大于零 strA 大于 strB
		///</returns>
		static int Compare(IN const String& strA,IN String strB,IN bool ignoreCase);
		static int Compare(IN const_iterator ap,IN size_type len,IN String& strB,IN bool ignoreCase);
		static int Compare(IN reverse_iterator ap,IN size_type len,IN String& strB,IN bool ignoreCase);
		int Compare(IN String str,IN bool ignoreCase);

		///<summary>判断当前字符串中的所有字符是否都为大写</summary>
		///<returns>true:所有字符都是大写,false:空串或包含小写字符</returns>
		bool IsUpper() const;

		///<summary>判断当前字符串中的所有字符是否都为小写</summary>
		///<returns>true:所有字符都是小写,false:空串或包含大写字符</returns>
		bool IsLower() const;

		///<summary>判断当前字符串中是否只包含英文字母字符和数字字符(a-z||A-Z||0-9)</summary>
		bool IsAlnum() const;

		///<summary>判断当前字符串中是否只包含英文字母字符(a-z||A-Z)</summary>
		///<returns>true:所有字符都是英文字母字符,false:含有非法字符</returns>
		bool IsAlpha() const;

		///<summary>判断当前字符串中是否只包含数字字符</summary>
		///<returns>true:所有字符都是0-9,false:包含有除0-9的其他字符</returns>
		bool IsNumeric() const;

		///<summary>将一个连续的字符串集合连接成一个新的字符串
		/// 实例:std::vector<std::String> v;
		/// v.push_back("This");v.push_back("is");v.push_back("a");v.push_back("test.");
		/// std::cout << std::String::join("|", v.begin(), v.end()) << std::endl;
		/// => This|is|a|test.
		///</summary>
		///<param name="glue">The glue to join strings with,即分隔字符串或分隔符</param>
		///<param name="first">The beginning of the range to join</param>
		///<param name="last">The end of the range to join</param>
		///<returns>return A string constructed of each element of the range connected together with @p glue,返回新字符串</returns>
		template <class InputIterator>
		static String Join(IN String& glue,IN InputIterator first,IN InputIterator last)
		{
			String retval;
			for (; first != last; first++)
			{
				retval.append(*first);
				retval.append(glue);
			}
			retval.erase(retval.length() - glue.length());

			return retval;
		}
		template <class InputIterator>
		static String Join(IN TCHAR glue,IN InputIterator first,IN InputIterator last)
		{
			String retval;
			for (; first != last; first++)
			{
				retval.append(*first);
				retval.append(1, glue);
			}
			retval.erase(retval.length() - 1);

			return retval;
		}


		///<summary>
		/// 在源字符串上进行字符串替换,将旧字符串替换新字符串.
		///</summary>
		///<param name="oldValue">旧字符串</param>
		///<param name="newValue">新字符串</param>
		///<returns>返回源字符串替换后的字符串(即*this)</returns>
		String& Replace(IN String oldValue, IN String newValue);
		String& Replace(IN String oldValue, IN String newValue,IN bool ignoreCase);
		String& Replace(IN TCHAR oldChar,IN TCHAR newChar);

		///<summary>
		/// 将指定字符串中的一个或多个格式项替换为指定对象的字符串表示形式
		/// PS、因为'%'字符是特殊字符,如果要输出该字符要使用'%%'进行转义
		///</summary>
		///<param name="format">格式化字符串</param>
		///<param name="...">可变参数,注意如果有参数是字符串也必须是TCHAR编码</param>
		void FormatStr(IN LPCTSTR format, ...);
		void FormatStr(IN LPCTSTR format,IN va_list args);
		void FormatStrA(IN LPCSTR format, ...);
		void FormatStrA(IN LPCSTR format,IN va_list args);

		static String Format(IN LPCTSTR format, ...);
		static String Format(IN LPCTSTR format,IN va_list args);

#ifdef _UNICODE
		///<summary>ansi编码的格式化函数</summary>
		///<param name="format">ansi编码的格式化字符串</param>
		///<param name="...">可变参数,注意如果有参数是字符串也必须是ANSI编码</param>
		static String Format(IN LPCSTR format, ...);		
		static String Format(IN LPCSTR format,va_list args);
		static std::string FormatA(IN LPCSTR format, ...);
		static std::string FormatA(IN LPCSTR format,va_list args);
#endif

		///<summary>获取指定字符在此字符串中的第一个匹配项的从零开始的索引,该搜索从指定字符位置开始</summary>
		///<param name="value" type="IN TCHAR">要查找的字符</param>
		///<param name="startIndex" type="IN int">搜索起始位置</param>
		///<returns>如果找到该字符,则为value的从零开始的索引位置;如果未找到，则为 -1</returns>
		int IndexOf(IN TCHAR value,IN int startIndex);
		///<param name="value">要搜寻的字符串</param>
		///<param name="ignoreCase">是否忽略大小写</param>
		int IndexOf(IN LPCTSTR value,IN bool ignoreCase=false);			
		///<param name="value">要搜寻的字符串</param>
		///<param name="startIndex">搜索起始位置</param>
		///<param name="ignoreCase">是否忽略大小写</param>
		int IndexOf(IN LPCTSTR value,IN int startIndex,IN bool ignoreCase);
		
		///<summary>报告指定字符串在当前String对象中最后一个匹配项的从零开始的索引。 一个参数指定要用于指定字符串的搜索类型</summary>
		///<param name="value">要查找的字符</param>
		///<param name="ignoreCase">是否忽略大小写</param>
		///<returns>
		/// 如果找到该字符串，则为 value 参数的索引位置；如果未找到该字符串，则为 -1。 
		/// 如果 value 为 String.Empty，则返回值为此实例中的最后一个索引位置。
		/// 返回的是value中第一个字符的下标.
		/// 例如:原字符串：0123456，value="45" ，则返回值为4
		///</returns>
		int LastIndexOf(IN LPCTSTR value,IN bool ignoreCase=false);
		int LastIndexOf(IN TCHAR value);

		///<summary>移除所有前导空白字符和尾部空白字符</summary>
		///<returns>从当前字符串的开头和结尾删除所有空白字符后剩余的字符串</returns>
		String& Trim();
		static String Trim(IN String& src);
		
		///<summary>删除字符串中所有的不可见字符</summary>
		///<returns>返回剩余字符组成的字符串</returns>
		String& RemoveUnprintableChars();

		///<summary>
		/// 对象移除数组中指定的一组字符的所有前导匹配项和尾部匹配项
		///</summary>
		///<param name="trimChars">
		/// 要删除的字符的数组或NULL,PS:参数被当成一个字符数组而不是一个字符串
		///</param>
		///<returns>
		/// 从当前字符串的开头和结尾删除所出现的所有trimChars参数中的字符后剩余的字符串。 
		/// 如果trimChars为null，则改为移除空白字符
		/// 例如:Trim(_T("000123xxx"),_T("0x"))=>"123"
		///</returns>
		String& Trim(IN LPCTSTR trimChars);//在源字符串上操作,并返回源字符串实例
		static String Trim(IN String& src,IN LPCTSTR trimChars);//返回操作之后的副本,不影响源字符串
#ifdef _UNICODE
		static string TrimA(IN string& src,IN LPCSTR trimChars);
		static string TrimA(IN LPCSTR src,IN LPCSTR trimChars);
#endif

		///<summary>
		/// 对象移除数组中指定的一组字符的所有尾部匹配项
		///</summary>
		///<param name="trimChars">
		/// 要删除的字符的数组或NULL,PS:参数被当成一个字符数组而不是一个字符串
		/// null表示移除空白字符
		///</param>
		///<returns>
		/// 从当前字符串的结尾删除所出现的所有trimChars参数中的字符后剩余的字符串。 
		/// 例如:TrimEnd(_T("000123xxx"),_T("0x"))=>"000123"
		///</returns>
		String& TrimEnd(IN LPCTSTR trimChars);//在源字符串上操作,并返回源字符串实例
		String& TrimEnd(IN TCHAR c);//在源字符串上操作,并返回源字符串实例
		static String TrimEnd(IN String& src,IN LPCTSTR trimChars);//返回操作之后的副本,不影响源字符串
		static String TrimEnd(IN String& src,IN TCHAR c);//返回操作之后的副本,不影响源字符串

		///<summary>
		/// 对象移除数组中指定的一组字符的所有前导匹配项
		///</summary>
		///<param name="trimChars">
		/// 要删除的字符的数组或NULL,PS:参数被当成一个字符数组而不是一个字符串
		///</param>
		///<returns>
		/// 从当前字符串的开头删除所出现的所有trimChars参数中的字符后剩余的字符串。 
		/// 如果trimChars为null，则改为移除空白字符
		/// 例如:TrimStart(_T("000123xxx"),_T("0x"))=>"123xxx"
		///</returns>
		String& TrimStart(IN LPCTSTR trimChars);
		String& TrimStart(IN TCHAR c);
		static String TrimStart(IN String& src,IN LPCTSTR trimChars);
		static String TrimStart(IN String& src,IN TCHAR c);

		///<summary>
		/// 获取当前字符串的子字符串,并返回子字符串副本且原始字符串保持不变
		/// PS:_T("0000234xxx").Substring(4,4) => _T("234x")
		///</summary>
		///<param name="startIndex">此实例中子字符串的起始字符位置(从零开始)</param>
		///<param name="length">子字符串中的字符数</param>
		///<returns>返回子字符串</returns>
		String Substring(IN int startIndex,IN int length);

		///<summary>从此实例检索子字符串。 子字符串从指定的字符位置开始</summary>
		///<returns>与此实例中在 startIndex 处开头的子字符串等效的一个字符串；如果 startIndex 等于此实例的长度，则为 String.Empty</returns>
		String Substring(IN int startIndex);
		
		///<summary>
		/// 返回限定长度的字符串(并在截断的字符串后面加上特定后缀)
		/// 例如:"123456789".Maxstring(0,3,"...") => "123..."
		///		 "123456789".Maxstring(-1,3,"...") => "...789"
		///		 "123456789".Maxstring(-2,4,"...") => "12...89"
		///</summary>
		///<param name="startIndex">此实例中子字符串的起始字符位置(从零开始)
		/// 如果值为-1表示截断前面的字符,并在字符串之前加上前缀
		/// 其他小于-1的值表示截断中间的字符,并在字符串中间加上前缀
		///</param>
		///<param name="length">子字符串中的字符数,小于0表示到结束位置或开头位置</param>
		///<param name="suffix">如果字符串长度过长,截断后的字符串会加入的后缀.NULL表示不要后缀</param>
		///<returns>截断后的字符串</returns>
		String Maxstring(IN int startIndex,IN int length,IN LPCTSTR suffix=NULL);

#ifndef _STRING_NO_STRINGHELPER_
		/// <summary>
		/// 带通配符的字符串匹配
		/// 通配符意义: 
		///		‘*’	代表任意字符串，包括空字符串；
		///		‘?’	代表任意一个字符，不能为空；
		/// </summary>
		/// <param name="source">要匹配的源字符串</param>
		/// <param name="pattern">匹配模板,可以包含通配符的字符串</param>
		/// <param name="ignoreCase">为TRUE，不区分大小写，否则区分大小写</param>
		/// <returns>匹配，返回1；否则返回0。</returns>
		static bool Match(LPCTSTR source,LPCTSTR pattern, bool ignoreCase);
		//
		/// <summary>
		/// 多重匹配，不同匹配字符串之间用‘|’隔开
		///		如: “*.h|*.cpp”将依次匹配“*.h”和“*.cpp”
		/// </summary>		
		/// <param name="source">要匹配的源字符串</param>
		/// <param name="pattern">匹配模板,可以包含通配符的字符串</param>
		/// <param name="nMatchLogic">等于0:不同匹配求或(即一个匹配即命中)，否则求与(全部匹配才算命中)</param>
		/// <param name="ignoreCase">是否忽略大小写</param>
		/// <returns>匹配,返回1.否则返回</returns>
		static bool MultiMatch(LPCTSTR source,LPCTSTR pattern, int nMatchLogic, bool ignoreCase);
#endif

		///<summary>
		/// 向此实例追加通过处理组合格式字符串（包含零个或更多格式项）而返回的字符串。 
		/// 每个格式项都由参数数组中相应参数的字符串表示形式替换
		///</summary>
		///<param name="format">符合格式字符串</param>
		///<param name="...">要设置其格式的对象的可变参数</param>
		void AppendFormat(IN LPCTSTR format,...);
#ifdef _UNICODE
		void AppendFormatA(IN LPCSTR format,...);
#endif

#ifndef _STRING_NO_CONVERT_
		void Append(IN long value);
		void Append(IN double value,IN int count);
#endif
		void Append(IN LPCTSTR text);
		void Append(IN TCHAR c,IN int count);
		void Append(IN LPCTSTR format,IN va_list args);
		void Append(IN LPCSTR text,IO::Encoding code = IO::EC_ANSI);
		void Assign(IN TCHAR c,IN int count);

		///<summary>比较此字符串实例的开头是否与指定的字符串匹配</summary>
		///<param name="value">要比较的字符串</param>
		///<param name="ignoreCase">要在比较过程中忽略大小写,则为 true;否则为 false</param>
		///<returns>如果 value 参数与此字符串的开头匹配,则为 true;否则为 false</returns>
		bool StartsWith(IN String value,IN bool ignoreCase=false);
		bool StartsWith(IN TCHAR value,IN bool ignoreCase=false);

		///<summary>比较此字符串实例的结尾是否与指定的字符串匹配</summary>
		///<param name="value">要与此实例末尾的子字符串进行比较的字符串</param>
		///<param name="ignoreCase">要在比较过程中忽略大小写，则为 true；否则为 false</param>
		///<returns>如果此字符串的末尾与 value 参数匹配，则为 true；否则为 false</returns>
		bool EndsWith(IN String value,IN bool ignoreCase=false);
		bool EndsWith(IN TCHAR value,IN bool ignoreCase=false);

		///<summary>获取字符串所占用的字节数(包含末尾的'\0'字符)</summary>
		int GetBytes();
		///<summary>获取字符串的字符数(不包含末尾的'\0'字符)</summary>
		int GetLength();
		//
		// 摘要:
		//     获取字符串缓冲区地址
		LPCTSTR GetBuffer();
#ifndef _STRING_NO_CONVERT_
		///<summary>将字符串转成数值</summary>
		///<param name="def">转换失败时返回的默认值</param>
		///<param name="base">数值进制
		/// 目前支持2-9进制,和16进制
		/// 小于等于0表示自动识别:根据特定的开头字符串表示不同的进制
		/// 0x(16进制),0n(10进制),0t(8进制),0y(2进制),其他(默认使用10进制解析)
		///</param>
		///<returns>返回转换后的数值,转换失败返回def值</returns>
		int ToInt(IN int def=0,IN int base=10);
#endif
		//清空字符串对象
		void Clear();
	};
}
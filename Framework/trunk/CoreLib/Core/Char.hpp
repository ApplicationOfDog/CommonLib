#pragma once

namespace System
{
	/// <summary>
	/// 字符类
	/// </summary>
	class API_CORE Char
	{
	private:
		TCHAR _char;

	public:
		 operator TCHAR();
		 Char& operator = (const Char& c);
		 Char& operator = (TCHAR c);		 
		 Char& operator + (const Char& c);
		 Char& operator + (TCHAR c);
		 Char& operator - (const Char& c);
		 Char& operator - (TCHAR c);
		 bool  operator == (const Char& c);
		 bool  operator == (TCHAR c);
		 bool  operator != (const Char& c);
		 bool  operator != (TCHAR c);

		 Char();
		 Char(IN Char& c);
		 Char(IN TCHAR c);

	public:
		///<summary>指示指定的字符是否属于控制字符类别。</summary>
		///<param name="c">要计算的字符</param>
		///<returns>如果 c 是控制字符，则为 true；否则，为 false。</returns>
		static bool IsControl(IN TCHAR c);
		bool IsControl();

		///<summary>指示指定的字符是否属于十进制数字类别,即0-9</summary>
		///<param name="c">要计算的字符</param>
		///<returns>如果c是十进制数字,则为true;否则为false</returns>
		static bool IsDigit(IN TCHAR c);
		bool IsDigit();

		///<summary>指示指定的字符是否属于字母类别</summary>
		///<param name="c">要计算的字符</param>
		///<returns>如果c是字母,则为true;否则为false</returns>
		static bool IsLetter(IN TCHAR c);
		bool IsLetter();

		///<summary>指示指定的字符是否属于字母或十进制数字类别</summary>
		///<param name="c">要计算的字符</param>
		///<returns>如果c是字母或十进制数字,则为true;否则为false</returns>
		static bool IsLetterOrDigit(IN TCHAR c);
		bool IsLetterOrDigit();

		///<summary>指示指定的字符是否属于小写字母类别,即a–z</summary>
		///<param name="c">要计算的字符</param>
		///<returns>如果c是小写字母,则为true;否则为false</returns>
		static bool IsLower(IN TCHAR c);
		bool IsLower();

		///<summary>指示指定的字符是否属于标点符号类别</summary>
		///<param name="c">要计算的字符</param>
		///<returns>如果 c 是标点符号，则为 true；否则，为 false</returns>
		static bool IsPunctuation(IN TCHAR c);
		bool IsPunctuation();

		///<summary>指示指定的字符是否属于大写字母类别,即A–Z</summary>
		///<param name="c">要计算的字符</param>
		///<returns>如果c是大写字母,则为true;否则为false</returns>
		static bool IsUpper(TCHAR c);
		bool IsUpper();

		///<summary>指示指定的字符是否属于16进制字符,0-9a-fA-F</summary>
		///<param name="c">要计算的字符</param>
		///<returns>如果 c 是16进制字符，则为 true；否则，为 false。</returns>
		static bool IsHex(TCHAR c);
		static bool IsHex(char c);
		bool IsHex();

		///<summary>指示指定的字符是否属于空白类别</summary>
		///<param name="c">要计算的字符</param>
		///<returns>如果 c 是空白，则为 true；否则，为 false</returns>
		static bool IsWhiteSpace(IN TCHAR c);
		static bool IsWhiteSpace(IN char c);
		bool IsWhiteSpace();

		///<summary>将字符的值转换为它的小写等效项</summary>
		///<param name="c">要转换的字符</param>
		///<returns>c 的小写等效项；如果 c 已经是小写形式或不是字母，则为 c 的未更改值</returns>
		static TCHAR ToLower(IN TCHAR c);
		void ToLower();

		///<summary>将 Unicode 字符的值转换为它的大写等效项</summary>
		///<param name="c">要转换的 Unicode 字符</param>
		///<returns>c 的大写等效项；如果 c 已经是大写形式而没有大写等效项或不是字母，则为 c 的未更改值</returns>
		static TCHAR ToUpper(IN TCHAR c);
		void ToUpper();

		///<summary>将此实例的值转换为其等效的字符串表示形式</summary>
		///<returns>此实例的值的字符串表示形式</returns>
		static String ToString(IN TCHAR c);
		String ToString();

		///<summary>字符比较</summary>
		///<param name="a">比较字符</param>
		///<param name="b">比较字符</param>
		///<param name="ignoreCase">是否忽略大小写</param>
		///<returns>返回结果:TRUE:相同字符，FALSE：不同字符</returns>
		bool static Compare(IN TCHAR a,IN TCHAR b,IN bool ignoreCase);
		bool Compare(IN Char b,IN bool ignoreCase);
		
		///<summary>是否为可见的ASCII字符</summary>
		///<param name="c">目标字符</param>
		static bool IsPrintable(IN TCHAR c);
#ifdef _UNICODE //unicode才需要两个版本
		static bool IsPrintable(IN char c);
#endif
		
		///<summary>TEST0</summary>
		///<returns>TEST0</returns>
		static int TEST0(IN LPCSTR p0,OUT LPCTSTR p1,BOOL p2,bool p3);
	};
}
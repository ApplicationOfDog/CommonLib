#include "stdafx.h"
#include "Char.hpp"

namespace System
{
	Char::Char(){
		_char=_T('\0');
	}
	Char::Char(Char& c)
	{
		_char=c._char;
	}
	Char::Char(TCHAR c)
	{
		_char=c;
	}
	Char::operator TCHAR()
	{
		return _char;
	}
	Char& Char::operator = (const Char& c)
	{
		this->_char=c._char;
		return *this;
	}
	Char& Char::operator = (TCHAR c)
	{
		this->_char=c;
		return *this;
	}	
	Char& Char::operator + (const Char& c)
	{
		this->_char+=c._char;
		return *this;
	}
	Char& Char::operator + (TCHAR c)
	{
		this->_char+=c;
		return *this;
	}
	Char& Char::operator - (const Char& c)
	{
		this->_char-=c._char;
		return *this;
	}
	Char& Char::operator - (TCHAR c)
	{
		this->_char-=c;
		return *this;
	}
	bool Char::operator == (const Char& c)
	{
		return _char==c._char;
	}
	bool Char::operator == (TCHAR c)
	{
		return _char==c;
	}
	bool Char::operator != (const Char& c)
	{
		return _char!=c._char;
	}
	bool Char::operator != (TCHAR c)
	{
		return _char!=c;
	}

	/// <summary>
	/// 在ASCⅡ码中，第0～31号及第127号(共33个)是控制字符或通讯专用字符
	/// </summary>
	bool Char::IsControl(TCHAR c)
	{
		return _istcntrl(c)!=0;
	}
	bool Char::IsControl()
	{
		return _istcntrl(_char)!=0;
	}
	/// <summary>
	/// 指示指定的字符是否属于十进制数字类别
	/// </summary>
	bool Char::IsDigit(TCHAR c)
	{		
		return c>=_T('0') && c<=_T('9');
	}
	bool Char::IsDigit()
	{		
		return IsDigit(_char);
	}
	/// <summary>
	/// Each of these routines returns nonzero if c is a particular representation of an alphabetic character. 
	/// istalpha returns a nonzero value if c is within the ranges A – Z or a – z
	/// </summary>
	bool Char::IsLetter(TCHAR c)
	{
		return ( c>=_T('a') && c<=_T('z') ) || ( c>=_T('A') && c<=_T('Z') );
	}
	bool Char::IsLetter()
	{		
		return IsLetter(_char);
	}
	/// <summary>
	/// Each of these routines returns nonzero if c is a particular representation of an alphanumeric character. 
	/// isalnum returns a nonzero value if either isalpha or isdigit is nonzero for c, that is, if c is within the ranges A – Z, a – z, or 0 – 9. 
	/// </summary>
	bool Char::IsLetterOrDigit(TCHAR c)
	{
		return ( c>=_T('a') && c<=_T('z') ) || ( c>=_T('A') && c<=_T('Z') ) || (c>=_T('0') && c<=_T('9'));
	}
	bool Char::IsLetterOrDigit()
	{		
		return IsLetterOrDigit(_char);
	}
	/// <summary>
	/// Each of these routines returns nonzero if c is a particular representation of a lowercase character.
	/// islower returns a nonzero value if c is a lowercase character (a – z). 
	/// </summary>
	bool Char::IsLower(TCHAR c)
	{		
		return c>=_T('a') && c<=_T('z');
	}
	bool Char::IsLower()
	{		
		return IsLower(_char);
	}
	/// <summary>
	/// Each of these routines returns nonzero if c is a particular representation of a punctuation character. 
	/// ispunct returns a nonzero value for any printable character that is not a space character or a character for which isalnum is nonzero. 
	/// </summary>
	bool Char::IsPunctuation(TCHAR c)
	{
		return _istpunct(c)!=0;
	}
	bool Char::IsPunctuation()
	{		
		return _istpunct(_char)!=0;
	}
	/// <summary>
	/// Each of these routines returns nonzero if c is a particular representation of a lowercase character.
	/// _istupper returns a nonzero value if c is a lowercase character (A – Z). 
	/// </summary>
	bool Char::IsUpper(TCHAR c)
	{
		return c>=_T('A') && c<=_T('Z');
	}
	bool Char::IsUpper()
	{		
		return IsUpper(_char);
	}
	/// <summary>
	/// Each of these routines returns nonzero if c is a particular representation of a space character. 
	/// isspace returns a nonzero value if c is a white-space character (0x09 – 0x0D or 0x20).
	/// </summary>
	bool Char::IsWhiteSpace(TCHAR c)
	{		
		return c==_T(' ')||c==_T('\t')||c==_T('\n')||c==_T('\r')||c==_T('\v');
	}
	bool Char::IsWhiteSpace(char c)
	{		
		return c==' '||c=='\t'||c=='\n'||c=='\r'||c=='\v';
	}
	bool Char::IsWhiteSpace()
	{
		return IsWhiteSpace(_char);
	}
	bool Char::IsHex(TCHAR c)
	{
		return ISHEX_T(c);
	}
	bool Char::IsHex(char c)
	{
		return ISHEX(c);
	}
	bool Char::IsHex()
	{
		return ISHEX_T(_char);
	}
	/// <summary>
	/// Converts a character to lowercase. 
	/// Each of these routines converts a given uppercase letter to a lowercase letter if it is possible and relevant. 
	/// The case conversion of towlower is locale-specific. Only the characters relevant to the current locale are changed in case. 
	/// </summary>
	TCHAR Char::ToLower(TCHAR c)
	{
		return tolower(c);
	}
	void Char::ToLower()
	{
		_char=tolower(_char);
	}
	/// <summary>
	/// Convert character to uppercase.
	/// </summary>
	TCHAR Char::ToUpper(TCHAR c)
	{
		return toupper(c);
	}
	void Char::ToUpper()
	{
		_char=toupper(_char);
	}
	String Char::ToString(TCHAR c)
	{
		return String(1,c);
	}
	String Char::ToString()
	{
		return String(1,_char);
	}

	bool Char::Compare(TCHAR a,TCHAR b,bool ignoreCase)
	{
		if(!ignoreCase) return a==b;
		return ToLower(a)==ToLower(b);
	}
	bool Char::Compare(Char b,bool ignoreCase)
	{
		if(!ignoreCase) return _char==b._char;
		return ToLower(_char)==ToLower(b._char);
	}	
	bool Char::IsPrintable(IN IN TCHAR c)
	{
		return _istprint(c)!=0;//c>=32 && c<=128;
	}


#ifdef _UNICODE
	bool Char::IsPrintable(IN IN char c)
	{
		return isprint(c)!=0;//c>=32 && c<=128;
	}
#endif
}
#pragma once

//#define _CONVERT_NO_IO_

/// <summary>
/// Unicode means Unicode16
/// </summary>
namespace System
{
	class API_CORE Convert
	{
	public:
		/// <summary>
		/// 对所有数制都通用（2进制、8进制、10进制、16进制）的字符串转数字的函数
		/// ps.从字符串开始位置开始转换直到一个非法字符（包括'\0'）结束。如果全部都是非法字符，返回0
		/// </summary>
		/// <param name="value">带转换的字符串,16进制可以'0x开头'</param>
		/// <param name="base">
		/// 数值格式，2:2进制，8：8进制...,目前支持2-9进制，和16进制。
		/// </param>
		/// <param name="pEnd">转换结束位置的字符下标,如果*pEnd==0表示第一个字符就是非法字符</param>
		static LONG64 ToInt64(IN LPCTSTR s,IN int base=10,OUT int* pEnd=NULL);//针对TCHAR字符串
		static LONG64 ToInt64(IN LPCSTR s,IN int base=10,OUT int* pEnd=NULL);//针对char字符串
		static int ToInt(IN LPCTSTR s,IN int base=10,OUT int* pEnd=NULL);
		static int ToInt(IN LPCSTR s,IN int base=10,OUT int* pEnd=NULL);
		///<param name="def">转换失败时返回的默认值</param>
		static int ToIntByDef(IN LPCTSTR s,OUT int def=0);
		static int ToIntByDef(IN LPCSTR s,OUT int def=0);
		static LONG64 ToInt64ByDef(IN LPCTSTR s,OUT LONG64 def=0);
		static LONG64 ToInt64ByDef(IN LPCSTR s,OUT LONG64 def=0);

		///<summary>将数字的指定字符串表示形式转换为等效的双精度浮点数</summary>
		///<param name="value">包含要转换的数字的字符串</param>
		///<param name="def">转换失败时返回的默认值</param>
		///<returns>返回转换后的数值,空字符串返回0</returns>
		static double ToDouble(IN LPCTSTR val,IN double def=0.0);

		///<summary>将指定的 32 位带符号整数的值转换为其等效的字符串表示形式</summary>
		///<param name="value">要转换的 32 位带符号整数</param>
		///<returns>value 的字符串表示形式</returns>
		static String ToString(IN int val);

		/// <summary>
		/// 对所有数制都通用（2进制、8进制、10进制、16进制）的数字转字符串的函数
		/// </summary>
		/// <param name="base">数值格式，2:2进制，8：8进制...</param>
		static String ToString(IN LONG64 val,IN int base);//返回TCHAR字符串
		static string ToStringA(IN LONG64 val,IN int base);//返回char字符串
		
		/// <summary>
		/// 将浮点数转为字符串，最后一位会进行四舍五入。
		/// PS.ToString(1.123456,4)=>"1.1235",ToString(1.123426,4)=>"1.1234"
		/// </summary>
		/// <param name="count">保留的小数位数</param>
		static String ToString(IN double val,IN int count=2);

		/// <summary>
		/// 将char*转为String
		/// </summary>
		/// <param name="str">源字符串即待转字符串</param>
		/// <param name="code">源字符串编码</param>
		static String ToString(IN LPCSTR str,IN IO::Encoding code=IO::EC_ANSI);
		static String ToString(IN std::string& str);

		/// <summary>
		/// 将TCHAR*转为string或wstring
		/// </summary>
		/// <param name="str">源字符串即待转字符串(Unicode字符串)</param>
		/// <param name="code">目标字符串的编码,即返回值编码</param>
		static string ToStringA(IN LPCTSTR str,IN IO::Encoding code=IO::EC_ANSI);
		static wstring ToStringW(IN LPCTSTR str,IN IO::Encoding code=IO::EC_ANSI);

		/// <summary>
		/// 将一个字符串的内容转换成为对应的BYTE数组,当含有非法字符时会直接跳过并且继续翻译下一个字符
		/// 例如:"10 20 0a 0b" => BYTE arr[]={0x10,0x20,0xa,0xb}
		/// </summary>
		/// <param name="text">待转换的字符串的数据缓冲区</param>
		/// <param name="len">待转换的text的字符个数(不包括结尾'\0',-1表示内部自行计算长度)</param>
		/// <param name="output">转换之后的BYTE数组,空间由外部申请传入</param>
		/// <param name="szOutput">output缓冲区大小,小于等于0表示没有设置大小(即默认缓冲区足够大)</param>
		/// <returns>true:转换成功。false:表示有字符转换失败,含有非法字符</returns>
		static bool ToByteArray(IN LPCSTR text,IN int len,OUT BYTE* output,IN int szOutput);
		static bool ToByteArray(IN LPCTSTR text,OUT BYTE* output,IN int szOutput);
		/// <summary>
		/// 将一个BYTE数组对应的值转成字符串
		/// 例如:BYTE arr[]={0x10,0x20,0xa,0xb} => "10 20 0a 0b"
		/// </summary>
		/// <param name="arr">待转换的数据缓冲区</param>
		/// <param name="len">数据缓冲区长度</param>
		/// <param name="separator">字节间的分隔符,0表示无分隔符</param>
		/// <returns>BYTE数组对应的字符串</returns>
		static string ToTextFromArray(IN BYTE* arr,IN int len,IN char separator=0);

	public:
		/// <summary>
		/// 将UTF-8编码的字符串转为Unicode编码字符串
		/// </summary>
		/// <returns>转换后目标字符串的字节数,包括'\0'字符。-1表示转换失败</returns>
		static int Utf8ToWideChar(IN const string& src,OUT wstring& dest);
		static int Utf8ToWideChar(IN LPCSTR src,OUT wchar_t** dest);
		static int WideCharToUtf8(IN const wstring& src,OUT string& dest);
		static int WideCharToUtf8(IN LPCWSTR src,OUT char** dest);

#if IS_WINDOWS_OS //window系统才有下面的接口
		/// <summary>
		/// 将UTF-8编码的字符串转为Unicode编码字符串
		/// </summary>
		/// <returns>转换后目标字符串的字节数,包括'\0'字符。-1表示转换失败</returns>
		static void Utf8ToWideChar(IN byte* pBuffer,IN int szBuf,OUT std::wstring& dest);
		static void Utf8ToAnsi(IN const string &src,OUT string &dest);
		static void AnsiToWideChar(IN byte* buffer,IN int szBuf,OUT wstring& dest);
		static void AnsiToWideChar(IN const string& src,OUT wstring& dest);
		static void AnsiToUtf8(IN const string &src,OUT string &dest);
		static int WideCharToAnsi(IN const wstring& src,OUT string& dest);

#ifndef _CONVERT_NO_IO_
		/// <summary>
		/// 将windows路径(应用层形式)转成设备路径路径(内核层形式)
		///  例如:"c:\\aaa\\bb\\cc.txt" => "\Device\HarddiskVolume1\aaa\bb\cc.txt"
		///  PS、由外部确定传入参数是合法路径
		/// </summary>
		/// <param name="path">windows路径(应用层形式)</param>
		/// <returns>返回d设备路径路径(内核层形式),如果转换失败则返回String::Empty</returns>
		static String ToVolumeDevicePath(IN String path);
		/// <summary>
		/// 将设备路径路径(内核层形式)转成windows路径(应用层形式)
		///  例如:"\Device\HarddiskVolume1\aaa\bb\cc.txt" => "c:\\aaa\\bb\\cc.txt" 
		///  PS、由外部确定传入参数是合法路径
		/// </summary>
		/// <param name="path">内核路径(内核层形式)</param>
		/// <returns>返回app路径(应用层形式),如果转换失败则返回String::Empty</returns>
		static String ToNtPath(IN String path);
		/// <summary>
		/// 获取文件内容头部的BOM编码格式
		/// </summary>
		/// <param name="buffer">文件内容</param>
		static IO::Encoding BomToEncode(IN byte* buffer);
		/// <summary>
		/// 获取编码格式对应的BOM数据，返回BOM的字节数
		/// </summary>
		/// <param name="encode">编码</param>
		/// <param name="buffer">返回BOM数据,可以为null</param>
		/// <returns>BOM的字节数</returns>
		static int EncodeToBom(IN IO::Encoding encode,OUT byte* buffer);
#endif //_CONVERT_NO_IO_
#endif //IS_WINDOWS_OS
	};
}
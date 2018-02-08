#pragma once

namespace System
{
	#define MAX_CONSOLE_BUFFER 1024
	/// <summary>
	/// 字符类
	/// </summary>
	class API_CORE Console
	{
	public:
		enum TextColor
		{
			TCNone=0,	//初始颜色,即白色(不增强)			
			TCRed,		//红色(增强)
			TCGreen,	//绿色(增强)
			TCBlue,		//蓝色(增强)
			TCYellow,	//黄色(增强)
			TCWhite,	//白色(增强)
		};
		enum MessageType{
			MTNormal=0,		//普通消息
			MTWarning,		//警告消息
			MTError,		//错误消息
		};
	public:
		///<summary>tab字符,可以通过修改该值修改前导字符</summary>
		static TCHAR	m_tabChar;	//针对void WriteLine(IN int nTab,...)和void Write(IN int nTab,...)
		static int		m_isEnabled;//是否启动

	private:
		map<string,UINT64> _recordXY;//记录位置的XY坐标

	public:
		Console();
		///<summary>将当前位置做为开始位置记录</summary>
		///<returns>是否成功</returns>
		bool StartRecord(IN LPCSTR name=NULL,IN bool isClearBefore = false);
		///<summary>指定范围清屏,从StartRecord记录的位置开始到当前位置进行清屏</summary>
		///<param name="isSetCursor">清空成功之后是否设置光标到起始位置</param>
		///<returns>是否成功</returns>
		bool ClearRecord(LPCSTR name=NULL,IN bool isSetCursor = true);

	public:
		#pragma region 从标准输入流读取
		///<summary>从标准输入流读取下一行字符</summary>
		///<returns>输入流中的下一行字符；如果没有更多的可用行，则为 null</returns>
		static String ReadLine();

		///<summary>从标准输入流读取一个数值,限定只能输入10进制数值</summary>
		///<param name="bLoopWhenError">输入错误时是否循环输入</param>
		///<param name="def">输入错误时返回的默认值</param>
		static int ReadInt(IN bool bLoopWhenError=true,IN int def=-1,IN int nTab=-1);

		///<summary>从标准输入流读取一个数值,支持所有进制类型
		/// 根据输入自动判断输入的数值进制
		/// 0x(16进制),0n(10进制),0t(8进制),0y(2进制),其他(默认使用10进制解析)
		/// 例如:输入0x11,返回17
		///		 输入0n11,返回11
		///		 输入0t11,返回9
		///		 输入0y11,返回3
		///</summary>
		///<param name="bLoopWhenError">当输入的字符串不是一个合法数值时是否打印错误信息并等待输入</param>
		///<param name="def">bLoopWhenError为false时有效,如果输入错误则需要默认返回数值</param>
		///<param name="nTab">在输出信息之前需要输出的前导字符个数,小等于0表示不输出</param>
		///<returns>返回用户输入的数值,如果输入错误则返回def</returns>
		static int ReadIntBySmart(IN bool bLoopWhenError=true,IN int def=-1,IN int nTab=-1);

		///<summary>从标准输入流读取枚举字符集合中的字符</summary>
		///<param name="enums">合法的字符集合</param>
		///<param name="len">集合字符个数</param>
		///<param name="ignoreCase">true:忽略大小写,false:大小写敏感</param>
		///<param name="bLoopWhenError">当输入的字符串不是一个合法数值时是否打印错误信息并等待输入</param>		
		///<param name="nTab">在输出信息之前需要输出的前导字符个数,小等于0表示不输出</param>
		///<returns>返回用户输入的字符下标,即enums中的下标.小于0表示错误码</returns>
		static char ReadEnum(IN char enums[],IN int len,IN bool ignoreCase,IN bool bLoopWhenError=true,IN int nTab=-1);
		#pragma endregion


		#pragma region 格式化参数为Unicode字符串
		///<summary>使用指定的格式信息，将指定的对象数组（后跟当前行终止符）的文本表示形式写入标准输出流</summary>
		///<param name="format">符合格式字符串</param>
		///<param name="...">要使用format写入的可变参数</param>
		///<returns>返回输出到界面的字符数量</returns>
		static int WriteLine(IN LPCTSTR format,...);
		static int Write(IN LPCTSTR format,...);
		static int WriteLine(IN int nTab,IN LPCTSTR format,...);
		static int Write(IN int nTab,IN LPCTSTR format,...);
		static void Write(IN TCHAR c);
#ifdef _UNICODE
		static void Write(IN char c);
#endif
		static int WriteLine(IN MessageType type,IN int nTab,IN LPCTSTR format,...);
		static int Write(IN MessageType type,IN int nTab,IN LPCTSTR format,...);	
		#pragma endregion


		#pragma region 格式化参数为Ansi字符串
		static std::string ReadLineA();
		///注意:格式化参数为ansi编码字符串,那么如果可变参数有字符串也必须为ansi字符串
		static int WriteLine(IN LPCSTR format,...);
		static int Write(IN LPCSTR format,...);
		///<param name="nTab">在输出真实信息之前需要输出的前导字符个数,小等于0表示不输出</param>
		static int WriteLine(IN int nTab,IN LPCSTR format,...);
		static int Write(IN int nTab,IN LPCSTR format,...);

		static int WriteLine(IN MessageType type,IN int nTab,IN LPCSTR format,...);
		static int Write(IN MessageType type,IN int nTab,IN LPCSTR format,...);		
		#pragma endregion

		static void WriteLine();
		static void Pause();

		///<summary>设置控制台字体颜色</summary>
		///<param name="isIntensity">是否颜色增强(其实就是亮度更高)</param>
		///<returns>是否成功</returns>
		static bool SetTextColor(IN TextColor value,IN bool isIntensity=true);

	private:
		static HANDLE getOutputHandle();
		static HANDLE getInputHandle();
		static bool setTextAttribute(IN WORD value);
		static string getTabChars(IN int nTab);
		static bool getTextAttribute(OUT WORD* pAttributes);
		static bool switchType(IN MessageType type,OUT WORD* pOldAttr);
	};
}
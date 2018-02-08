#include "stdafx.h"
#include "Console.hpp"
#include "Convert.hpp"
#include <iostream>
#include <WinCon.h>

namespace System
{
	TCHAR Console::m_tabChar = _T('\t');
	int Console::m_isEnabled = 0xffff; 

	Console::Console(){}

	String Console::ReadLine()
	{
		std::string line;
		cin.clear();//清除错误状态
		cin.sync();//清空流缓存
		if(!getline(cin,line)) return String::Empty;

		return Convert::ToString(line);
	}
	std::string Console::ReadLineA()
	{
		std::string line;

		cin.clear();//清除错误状态
		cin.sync();//清空流缓存
		if(!getline(cin,line)) return "";
		return line;
	}
	int Console::ReadInt(IN bool bLoopWhenError,IN int def,IN int nTab)
	{
		int inval = -1;
		std::string tabStrA = getTabChars(nTab);
		
		while(true)
		{
			cin >> inval;
			if(cin.fail())//输入格式有误,即非数值
			{
				cin.clear();//清除错误状态
				cin.sync();//清空流缓存
				if(!bLoopWhenError) return def;

				cout << tabStrA <<endl;
				continue;
			}
			return inval;
		}
	}
	int Console::ReadIntBySmart(IN bool bLoopWhenError,IN int def,IN int nTab)
	{
		int inval = def;
		std::string tabStrA = getTabChars(nTab);

		while(true)
		{
			inval = ReadLine().ToInt(def,-1);
			if(!bLoopWhenError) return inval;
			if(inval == def)
			{
				cout << tabStrA <<endl;
				continue;
			}
			return inval;
		}
	}
	char Console::ReadEnum(IN char enums[],IN int len,IN bool ignoreCase,IN bool bLoopWhenError,IN int nTab)
	{
		if(enums==NULL||len<=0) return -1;

		char inval;
		std::string tabStrA = getTabChars(nTab);

		while(true)
		{
			cin >> inval;
			if(cin.fail())//输入格式有误,即非数值
			{
				cin.clear();//清除错误状态
				cin.sync();//清空流缓存
				if(!bLoopWhenError) return -2;

				cout << tabStrA <<endl;
				continue;
			}
			int index = 0;
			for (;index<len;index++)
			{
				if(!ignoreCase)
				{
					if(enums[index]==inval) return enums[index];
					continue;
				}
				if(tolower(enums[index])==tolower(inval)) return enums[index];
			}

			if(!bLoopWhenError) return -3;
			cout << tabStrA <<endl;
			continue;
		}
		return -4;
	}

	int Console::WriteLine(IN LPCTSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,MTNormal)) return 0;

		va_list ap;		
		va_start(ap,format);		
		String item = String::Format(format,ap);
		va_end(ap);
		item.Append(_T("\n"));

		DWORD numOfCharsWritten = 0;
		BOOL isSuc = ::WriteConsole(getOutputHandle(),item.c_str(),item.GetLength(),&numOfCharsWritten,NULL);
		return (int)numOfCharsWritten;
	}
	int Console::Write(IN LPCTSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,MTNormal)) return 0;

		va_list ap;
		va_start(ap,format);
		String item = String::Format(format,ap);		
		DWORD numOfCharsWritten = 0;
		::WriteConsole(getOutputHandle(),item.c_str(),item.GetLength(),&numOfCharsWritten,NULL);
		va_end(ap);
		return (int)numOfCharsWritten;
	}
	int Console::WriteLine(IN int nTab,IN LPCTSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,MTNormal)) return 0;

		String line;		
		if(nTab>0) line.Append(m_tabChar,nTab);
		if(format!=NULL && *format!=0)
		{
			va_list ap;
			va_start(ap,format);
			line.Append(format,ap);
			va_end(ap);
		}
		if(line.IsEmpty()) return 0;

		line.Append(_T("\n"));
		DWORD numOfCharsWritten = 0;
		::WriteConsole(getOutputHandle(),line.c_str(),line.GetLength(),&numOfCharsWritten,NULL);
		return (int)numOfCharsWritten;
	}
	int Console::Write(IN int nTab,IN LPCTSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,MTNormal)) return 0;

		String line;		
		if(nTab>0) line.Append(m_tabChar,nTab);
		if(format!=NULL && *format!=0)
		{
			va_list ap;
			va_start(ap,format);
			line.Append(format,ap);
			va_end(ap);
		}
		if(line.IsEmpty()) return 0;
		
		DWORD numOfCharsWritten = 0;
		::WriteConsole(getOutputHandle(),line.c_str(),line.GetLength(),&numOfCharsWritten,NULL);
		return (int)numOfCharsWritten;
	}
	int Console::WriteLine(IN MessageType type,IN int nTab,IN LPCTSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,type)) return 0;

		String line;	
		if(nTab>0) line.Append(m_tabChar,nTab);
		if(format!=NULL && *format!=0)
		{
			va_list ap;
			va_start(ap,format);
			line.Append(format,ap);
			va_end(ap);
		}
		if(line.IsEmpty()) return 0;

		WORD wAttributes = 0x0;
		bool isSuc = switchType(type,&wAttributes);//设置为红色字体

		line.Append(_T("\n"));
		DWORD numOfCharsWritten = 0;
		::WriteConsole(getOutputHandle(),line.c_str(),line.GetLength(),&numOfCharsWritten,NULL);		

		if(isSuc) setTextAttribute(wAttributes);//恢复为原来的字体颜色
		return (int)numOfCharsWritten;
	}
	int Console::Write(IN MessageType type,IN int nTab,IN LPCTSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,type)) return 0;

		String line;		
		if(nTab>0) line.Append(m_tabChar,nTab);
		if(format!=NULL && *format!=0)
		{
			va_list ap;
			va_start(ap,format);
			line.Append(format,ap);
			va_end(ap);
		}
		if(line.IsEmpty()) return 0;

		WORD wAttributes = 0x0;
		bool isSuc = switchType(type,&wAttributes);//设置为红色字体

		DWORD numOfCharsWritten = 0;
		::WriteConsole(getOutputHandle(),line.c_str(),line.GetLength(),&numOfCharsWritten,NULL);

		if(isSuc) setTextAttribute(wAttributes);//恢复为原来的字体颜色
		return (int)numOfCharsWritten;
	}
	
	////ansi
	/////////////////////////////////////////////////////////////////////////////
	int Console::WriteLine(IN LPCSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,MTNormal)) return 0;
		if(format==NULL || *format==0) return 0;

		va_list ap;
		va_start(ap,format);
		std::string a_text = String::FormatA(format,ap);		
		cout << a_text << endl;
		cout.flush();
		va_end(ap);
		return (int)a_text.size()+1;
	}
	int Console::Write(IN LPCSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,MTNormal)) return 0;
		if(format==NULL || *format==0) return 0;

		va_list ap;
		va_start(ap,format);
		std::string a_text = String::FormatA(format,ap);		
		cout<< a_text;
		cout.flush();
		va_end(ap);
		return (int)a_text.size()+1;
	}
	int Console::WriteLine(IN int nTab,IN LPCSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,MTNormal)) return 0;

		String line;
		if(nTab>0) line.Append(m_tabChar,nTab);
		if(format!=NULL && *format!=0)
		{
			va_list ap;
			va_start(ap,format);
			String text = String::Format(format,ap);
			line.Append(text);
			va_end(ap);
		}
		if(line.IsEmpty()) return 0;

		string lineA = Convert::ToStringA(line);
		cout<< lineA << endl;
		cout.flush();
		return (int)line.size()+1;
	}
	int Console::Write(IN int nTab,IN LPCSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,MTNormal)) return 0;

		String line;		
		if(nTab>0) line.Append(m_tabChar,nTab);
		if(format!=NULL && *format!=0)
		{
			va_list ap;
			va_start(ap,format);
			String text = String::Format(format,ap);
			line.Append(text);
			va_end(ap);
		}
		if(line.IsEmpty()) return 0;

		string lineA = Convert::ToStringA(line);
		cout<< lineA;
		cout.flush();		
		return (int)line.size()+1;
	}
	int Console::WriteLine(IN MessageType type,IN int nTab,IN LPCSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,type)) return 0;

		String line;
		if(nTab>0) line.Append(m_tabChar,nTab);
		if(format!=NULL && *format!=0)
		{
			va_list ap;
			va_start(ap,format);
			String text = String::Format(format,ap);
			line.Append(text);
			va_end(ap);
		}
		if(line.IsEmpty()) return 0;

		WORD wAttributes = 0x0;
		bool isSuc = switchType(type,&wAttributes);//设置为红色字体

		string lineA = Convert::ToStringA(line);
		cout<< lineA << endl;
		cout.flush();
		
		if(isSuc) setTextAttribute(wAttributes);//恢复为原来的字体颜色
		return (int)line.size()+1;
	}
	int Console::Write(IN MessageType type,IN int nTab,IN LPCSTR format,...)
	{
		if(IS_BIT_0(m_isEnabled,type)) return 0;

		String line;
		if(nTab>0) line.Append(m_tabChar,nTab);
		if(format!=NULL && *format!=0)
		{
			va_list ap;
			va_start(ap,format);
			String text = String::Format(format,ap);
			line.Append(text);
			va_end(ap);
		}
		if(line.IsEmpty()) return 0;

		WORD wAttributes = 0x0;
		bool isSuc = switchType(type,&wAttributes);//设置为红色字体

		string lineA = Convert::ToStringA(line);
		cout<< lineA;
		cout.flush();

		if(isSuc) setTextAttribute(wAttributes);//恢复为原来的字体颜色
		return (int)line.size()+1;
	}
	void Console::Write(IN TCHAR c)
	{
		if(IS_BIT_0(m_isEnabled,MTNormal)) return;
		wcout<<c;
	}
#ifdef _UNICODE
	void Console::Write(IN char c)
	{
		if(!m_isEnabled) return;
		cout<<c;
	}
#endif
	void Console::WriteLine()
	{
		if(IS_BIT_0(m_isEnabled,MTNormal)) return;
		cout<< endl;
	}
	void Console::Pause()
	{
		::system("pause");
	}
	bool Console::SetTextColor(IN TextColor value,IN bool isIntensity)
	{
		WORD wAttributes = 0x0;
		if(!getTextAttribute(&wAttributes)) return false;
		
		CLEAR_VAR_FLAG(wAttributes,FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
		switch(value)
		{
		case TCRed:
			SET_VAR_FLAG(wAttributes,FOREGROUND_RED);
			break;
		case TCGreen:
			SET_VAR_FLAG(wAttributes,FOREGROUND_GREEN);
			break;
		case TCBlue:
			SET_VAR_FLAG(wAttributes,FOREGROUND_BLUE);
			break;
		case TCYellow:
			SET_VAR_FLAG(wAttributes,FOREGROUND_RED|FOREGROUND_GREEN);
			break;
		case TCWhite:
		default:
			SET_VAR_FLAG(wAttributes,FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
			break;
		}
		if(isIntensity) SET_VAR_FLAG(wAttributes,FOREGROUND_INTENSITY);
		return setTextAttribute(wAttributes);
	}
	///////////////////////////////////////////////////////////////////////////
	bool Console::StartRecord(IN LPCSTR name,IN bool isClearBefore)
	{
		string nameStr;
		if(name != NULL && *name != 0) nameStr = name;

		HANDLE  hConsole = getOutputHandle(); 
		if(hConsole==INVALID_HANDLE_VALUE)
		{
			String error = Environment::LastErrorText();
			return false;
		}
		CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo={0};
		if(!::GetConsoleScreenBufferInfo(hConsole,&consoleScreenBufferInfo))
		{
			String error = Environment::LastErrorText();
			return false;
		}
		int recordX = consoleScreenBufferInfo.dwCursorPosition.X;
		int recordY = consoleScreenBufferInfo.dwCursorPosition.Y;
		_recordXY[nameStr] = ((UINT64)recordX<<32)|recordY;
		return true;
	}
	bool Console::ClearRecord(LPCSTR name,IN bool isSetCursor)
	{
		string nameStr;
		if(name != NULL && *name != 0) nameStr = name;
		map<string,UINT64>::iterator it = _recordXY.find(nameStr);
		if(it == _recordXY.end()) return false;

		HANDLE  hConsole = getOutputHandle(); 
		if(hConsole==INVALID_HANDLE_VALUE)
		{
			String error = Environment::LastErrorText();
			return false;
		}

		CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo={0};
		if(!::GetConsoleScreenBufferInfo(hConsole,&consoleScreenBufferInfo))
		{
			String error = Environment::LastErrorText();
			return false;
		}
		int startX = it->second>>32;
		int startY = it->second & 0xffffffff;
		int endX = consoleScreenBufferInfo.dwCursorPosition.X;
		int endY = consoleScreenBufferInfo.dwCursorPosition.Y;
		int recordLen = 0x0;
		if(endY<startX)
		{
			Environment::LastError(ERROR_ACCESS_DENIED);
			return false;
		}
		if(endY==startY)
			recordLen = endX - startX;
		else
			recordLen = (endY - startY)*consoleScreenBufferInfo.dwSize.X+endX;

		COORD recCoord = {(SHORT)startX,(SHORT)startY};
		DWORD numberOfCharsWritten = 0x0;
		if(!::FillConsoleOutputCharacter(hConsole,' ',recordLen, recCoord,&numberOfCharsWritten))
		{
			String error = Environment::LastErrorText();
			return false;
		}
		if(isSetCursor)
		{
			if(!::SetConsoleCursorPosition(hConsole,recCoord))
			{
				String error = Environment::LastErrorText();
				return false;
			}
		}
		return true;
	}
	///////////////////////////////////////////////////////////////////////////
	HANDLE Console::getOutputHandle()
	{
		static HANDLE  hConsole = NULL;
		if(hConsole==NULL)
			hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);

		return hConsole;
	}
	HANDLE Console::getInputHandle()
	{
		static HANDLE  hConsole = NULL;
		if(hConsole==NULL)
			hConsole = ::GetStdHandle(STD_INPUT_HANDLE);

		return hConsole;
	}
	bool Console::setTextAttribute(IN WORD value)
	{
		HANDLE  hConsole = getOutputHandle(); 
		if(hConsole==INVALID_HANDLE_VALUE)
		{
			String error = Environment::LastErrorText();
			return false;
		}
		BOOL isSuc =::SetConsoleTextAttribute(hConsole,value);
		return isSuc == TRUE;
	}
	string Console::getTabChars(IN int nTab)
	{
		if(nTab<=0) return "";

		String error;
		error.Append(m_tabChar,nTab);
		return Convert::ToStringA(error);
	}
	bool Console::getTextAttribute(OUT WORD* pAttributes)
	{
		assert("invalid parameter 'pAttributes'" && pAttributes!=NULL);
		HANDLE  hConsole = getOutputHandle(); 
		if(hConsole==INVALID_HANDLE_VALUE)
		{
			String error = Environment::LastErrorText();
			return false;
		}
		CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo={0};
		if(!::GetConsoleScreenBufferInfo(hConsole,&consoleScreenBufferInfo))
		{
			String error = Environment::LastErrorText();
			return false;
		}

		if(pAttributes!=NULL)
			*pAttributes = consoleScreenBufferInfo.wAttributes;

		return true;
	}
	bool Console::switchType(IN MessageType type,OUT WORD* pOldAttr)
	{
		WORD wAttributes = 0x0;
		bool isSuc = getTextAttribute(&wAttributes);
		if(isSuc)
		{
			TextColor color = TCNone;
			switch(type)
			{
			case MTNormal:color = TCWhite;break;
			case MTWarning:color = TCYellow;break;
			case MTError:color = TCRed;break;
			}
			SetTextColor(color,true);//设置为红色字体
		}

		if(pOldAttr!=NULL) *pOldAttr = wAttributes;
		return isSuc;
	}
}
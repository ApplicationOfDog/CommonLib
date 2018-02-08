#pragma once
#include "Context.h"
#include "..\ui\MainWin.h"

class Program:public Singleton_ptr<Program>
{
	friend Singleton_ptr<Program>;
public:
	Context		m_context;
	MainWin		m_mainWin;

public:
	///<summary>
	/// 解析命令行参数,命令参数格式如下:
	/// -xx:aa,-yy:bb,-zz:cc //说明:-参数名称:参数值(如果是数值,均支持16进制表示,例如:0x3)
	/// 内置参数说明如下:
	/// -dmsgbox:0|1	//是否弹出MessageBox等待调试
	/// -dlevel:x		//设置GDebuger的输出级别,具体值请看System::IO::DebugLevel
	///</summary>
	bool ParseCommand();

	///<summary>执行程序业务</summary>
	void Run();
};

#define GProgram Program::Instance()
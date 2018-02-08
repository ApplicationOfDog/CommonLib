#pragma once

///<summary>程序数据上下文</summary>
class Context
{
public:
	String	m_programPath;//当前应用程序全路径,即可执行文件全路径
	String	m_programDir;//当前应用程序目录,即可执行文件所在的文件夹目录

public:
	Context();
};
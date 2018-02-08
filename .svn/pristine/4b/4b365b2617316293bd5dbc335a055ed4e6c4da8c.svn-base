#include "stdafx.h"
#include "Program.h"

bool Program::ParseCommand()
{
	map<String,String> params = Environment::AnayzeProgramCommandLine(NULL);
	map<String,String>::iterator it,endIt = params.end();
	if((it = params.find(_T("dmsgbox")))!=endIt)
	{
		if(it->second.ToInt(0,0)==1)
			::MessageBoxA(NULL,"Waiting for test...","Test",MB_OK);
	}
	if((it = params.find(_T("dlevel")))!=endIt)
	{
		int val = it->second.ToInt(-1,0);
		if(val>DEBUG_LEVEL_NULL && GDebuger!=NULL) 
			GDebuger->RecordLevel((DWORD)val);
	}
	return true;
}

void Program::Run()
{
	m_mainWin.CreateAndShow(_T("DuilibUI"));
}
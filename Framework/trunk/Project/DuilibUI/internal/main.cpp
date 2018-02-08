#include "stdafx.h"

Context* GContext = NULL;
PDebuger g_pDebuger = NULL;

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	GDebuger = new Debuger(_T("XXX:"));
	GContext = &GProgram->m_context;

	if(!GProgram->ParseCommand()) goto _BACK_;
	CPaintManagerUI::SetInstance(hInstance);
	GProgram->Run();
	CPaintManagerUI::MessageLoop();
_BACK_:
	DEBUG_STRING("Program Exit");
	return 0;
}	
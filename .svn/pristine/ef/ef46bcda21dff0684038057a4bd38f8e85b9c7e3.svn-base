#include "stdafx.h"
#include "Helper/LogHelper.hpp"
#include "TimeLog.hpp"

#define PHELPER ((LogHelper*)_pHelper)

namespace System
{
	namespace IO
	{
		LARGE_INTEGER		g_perfFreq={0};
		LARGE_INTEGER		g_perfBegin={0};
		LARGE_INTEGER		g_perfLast={0};
		LARGE_INTEGER		g_perfNext={0};

		TimeLog::TimeLog()
		{
			_pHelper = new LogHelper();
			Init();
		}
		TimeLog::TimeLog(TLogSave type,String path,CalcMethod cm)
		{
			_pHelper = new LogHelper();
			Init();
			PHELPER->_saveType=type;
			PHELPER->_path=path;
			_method=cm;
		}
		TimeLog::~TimeLog()
		{
			DELETE_OBJECT(_pHelper);
		}
		void TimeLog::Start()
		{
			if(_method==CM_CLOCK){
				_tBegin=::clock();
				_tLast=_tBegin;
				return;
			}
			QueryPerformanceFrequency(&_perfFreq);
			QueryPerformanceCounter(&_perfBegin);
			_perfLast=_perfBegin;
		}
		void TimeLog::Record(LPCTSTR format,...)
		{
			String text=STRING_EMPTY;
			if(format!=NULL){
				va_list ap;
				va_start(ap,format);
				text=String::Format(format,ap);
				va_end(ap);
			}
			InnerRecord(text,TRUE);
		}
		void TimeLog::End(LPCTSTR text,bool bOutDuration)
		{
			InnerRecord(text,bOutDuration);

			String _text;

			double duration =0;
			if(_method==CM_CLOCK)
				duration=(double)(_tLast-_tBegin)/CLOCKS_PER_SEC;//s
			else //获得对应的时间值，单位为秒
				duration=(double)(_perfLast.QuadPart-_perfBegin.QuadPart)/_perfFreq.QuadPart;

			if(duration>1.0)
				_text=String::Format(_T("%s total running times:%.2lf(s)\n"),text==NULL?_T(""):text,duration);
			else
				_text=String::Format(_T("%s total running times:%.2lf(ms)\n"),text==NULL?_T(""):text,duration*1000);

			PHELPER->InnerOutput(_text);
			PHELPER->InnerOutputToFile();
		}

		/////////////////////////////////////
		void TimeLog::Init()
		{
			PHELPER->_saveType=LS_File_LastOut;
			PHELPER->_isClearFirst=TRUE;
			PHELPER->_isShowTime=FALSE;
			PHELPER->_headText.clear();
			PHELPER->_isStartupCounter=FALSE;
			PHELPER->_defaultExt=_T(".t");

			_tBegin=0;
			_tLast=0;
			_tNext=0;

			_method=CM_COUNTER;
			_perfFreq.QuadPart=0;
			_perfBegin.QuadPart=0;
			_perfLast.QuadPart=0;
			_perfNext.QuadPart=0;
		}
		void TimeLog::InnerRecord(LPCTSTR text,bool bOutText)
		{
			if(_method==CM_CLOCK)
				_tNext=::clock();
			else
				QueryPerformanceCounter(&_perfNext);

			if(bOutText)
			{
				double duration =0;
				if(_method==CM_CLOCK)
					duration=(double)(_tNext-_tLast)/CLOCKS_PER_SEC;//s
				else
					duration=(double)(_perfNext.QuadPart-_perfLast.QuadPart)/_perfFreq.QuadPart;//获得对应的时间值，单位为秒

				String _text;
				if(duration>=1.0){
					_text=String::Format(_T("%s-duration:%.2lf(s)\n"),text==NULL?_T(""):text,duration);
				}
				else{
					_text=String::Format(_T("%s-duration:%.2lf(ms)\n"),text==NULL?_T(""):text,duration*1000);
				}
				PHELPER->InnerOutput(_text);
			}

			if(_method==CM_CLOCK){
				_tLast=_tNext;
				return;
			}
			_perfLast=_perfNext;
		}		
		
	}
}
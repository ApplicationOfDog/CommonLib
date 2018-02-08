#include "stdafx.h"
#include "DIOC.h"

namespace System{
	namespace Communication{

	DIOC::DIOC(IN HANDLE hDevice,IN bool isCloseHandleInDestructor)
	{
		_hDevice = hDevice;
		_isCloseHandleInDestructor = isCloseHandleInDestructor;
	}
	DIOC::~DIOC()
	{
		if(_hDevice!=NULL && _hDevice!=INVALID_HANDLE_VALUE && _isCloseHandleInDestructor){
			if(::CloseHandle(_hDevice)) _hDevice = INVALID_HANDLE_VALUE;
		}
	}
	int DIOC::Input(IN PBYTE addr,IN int size)
	{
		return _input.Push(addr,size);
	}
	int DIOC::Input(IN LPCTSTR str,IN bool isAddEndChar)
	{
		return _input.PushText(str,isAddEndChar);
	}
	int DIOC::Output(IN PBYTE addr,IN int size)
	{
		return _output.Push(addr,size);
	}
	int DIOC::Output(IN LPCTSTR str,IN bool isAddEndChar)
	{
		return _output.PushText(str,isAddEndChar);
	}
	bool DIOC::Send(IN DWORD dwIoControlCode)
	{
		if(_hDevice==NULL||_hDevice==INVALID_HANDLE_VALUE) return false;

		PBYTE pInBuffer = _input.GetBuffer();//该内存由MemBuffer内部管控,不需要手动释放
		PBYTE pOutBuffer = _output.GetBuffer();
		int szInBuffer = _input.Size();	
		int szOutBuffer = _output.Size();
		DWORD bytesReturned = 0;
		BOOL isSuc = ::DeviceIoControl(_hDevice,dwIoControlCode,pInBuffer,szInBuffer,pOutBuffer,szOutBuffer,&bytesReturned, NULL);
		if(isSuc) _output.ParseBufferToVar();
		
		return isSuc == TRUE;
	}

}}
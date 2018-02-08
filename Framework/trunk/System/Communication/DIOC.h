#pragma once

namespace System{
	namespace Communication{

	///<summary>
	/// DeviceIoControl抽象类
	///</summary>
	class API_SYSTEM DIOC
	{
	protected:
		HANDLE	_hDevice;
		bool	_isCloseHandleInDestructor;
		System::Data::MemBuffer _input;
		System::Data::MemBuffer _output;

	public:
		///<param name="isCloseInDestructor">是否在析构函数中关闭内核句柄</param>
		DIOC(IN HANDLE hDevice,IN bool isCloseHandleInDestructor=false);
		~DIOC();

		int Input(IN PBYTE addr,IN int size);
		int Input(IN LPCTSTR str,IN bool isAddEndChar);

		int Output(IN PBYTE addr,IN int size);
		int Output(IN LPCTSTR str,IN bool isAddEndChar);

		bool Send(IN DWORD dwIoControlCode);
	};

	///<summary>添加输入缓冲区参数</summary>
	#define DIOC_INPUT_VAR(inst,v) inst.Input((PBYTE)&v,sizeof(v))
	#define DIOC_INPUT_VAR_P(inst,p) inst.Input((PBYTE)p,sizeof(*p))
	#define DIOC_INPUT_STR(inst,s,isEndChar) inst.Input(s,isEndChar)
	#define DIOC_INPUT_MEM(inst,addr,len) inst.Input((PBYTE)addr,len)

	///<summary>添加输出缓冲区参数</summary>
	#define DIOC_OUTPUT_VAR(inst,v) inst.Output((PBYTE)&v,sizeof(v))
	#define DIOC_OUTPUT_VAR_P(inst,p) inst.Output((PBYTE)p,sizeof(*p))
	#define DIOC_OUTPUT_STR(inst,s,isEndChar) inst.Output(s,isEndChar)
	#define DIOC_OUTPUT_MEM(inst,addr,len) inst.Output((PBYTE)addr,len)


}}

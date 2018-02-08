#pragma once

#ifndef _NO_CUSTOM_TYPE_
	/////////////////////////////////////////////
	//定义基础类型
	/////////////////////////////////////////////
	typedef char CHAR,*PCHAR;
	typedef wchar_t WCHAR,*PWCHAR;
	typedef unsigned char BYTE,*PBYTE;
	typedef unsigned char BOOLEAN,*PBOOLEAN;
	typedef short INT16;
	typedef unsigned short UINT16;
	typedef int INT32;
	typedef unsigned int UINT32;
	typedef __int64 INT64;
	typedef unsigned __int64 UINT64;
	typedef UINT64 QWORD,*PQWORD;
	typedef void * PVOID;
#endif

//delete memory block
#define DELETE_ARRAY(p) if((p)!=NULL){ delete[] (p); (p)=NULL; }
//delete a string memory
#define DELETE_STRING(s) DELETE_ARRAY(s)
//delete a memory
#define DELETE_MEM(p) DELETE_ARRAY(p)

//delete single object
#define DELETE_OBJECT(p) if((p)!=NULL){ delete (p); (p)=NULL; }
//close the handle
#define CLOSE_HANDLE(h) if((h)!=NULL && (h)!=INVALID_HANDLE_VALUE){ ::CloseHandle(h); (h)=NULL; }
//is it a invalid handle ? true:invalid handle,false:valid handle
#define IS_INVALID_HANDLE(h) (h==NULL || h==INVALID_HANDLE_VALUE)

//v:value,b:bitX  
#define IS_BIT_1(v,b) (((v) & (1<<(b)))>0) //判断v中某一个bit是否为1,返回TRUE或FALSE
#define IS_BIT_0(v,b) (((v) & (1<<(b)))==0) //判断v中某一个bit是否为0,返回TRUE或FALSE
#define BIT(v,b) (((v) & (1<<(b)))>>(b)) //获取v中某一个bit的值,返回1或0
#define SET_BIT_1(v,b) ((v)|=1<<(b)) //设置某一个bit为1
#define SET_BIT_0(v,b) ((v)&=~(1<<(b))) //设置某一个bit为0

#ifndef IS_FLAG_ON
#	define IS_FLAG_ON(v,b) (((v) & (b))!=0) //是否设置标志位
#endif
#ifndef FLAG_ON
#	define FLAG_ON(_F,_SF)        ((_F) & (_SF))//是否设置标志位
#endif
#ifndef SET_VAR_FLAG
#	define SET_VAR_FLAG(_F,_SF)       ((_F) |= (_SF))
#endif
#ifndef CLEAR_VAR_FLAG
#	define CLEAR_VAR_FLAG(_F,_SF)     ((_F) &= ~(_SF))
#endif

///<summary>缓冲区与变量之间的转换宏-COPY_XXX_TO/FROM_BUFFER</summary>
//将变量值复制到缓冲区  v:变量,b:缓冲区首地址,p:位置
//eg. DWORD position=0;
//eg. COPY_VAR_TO_BUFFER(value,pBuffer,position);
#define COPY_VAR_TO_BUFFER(v,b,p) { memcpy((BYTE*)(b)+(p),&(v),sizeof(v));(p)+=sizeof(v); }

//将一块内存复制到缓冲区  m:内存指针,b:缓冲区首地址,p:位置,l:内存字节长度
#define COPY_MEM_TO_BUFFER(m,b,p,l) { memcpy((BYTE*)(b)+(p),(m),(l));(p)+=(l); }

//将缓冲区中某一位置值复制到变量  v:变量,b:缓冲区首地址,p:位置
#define COPY_VAR_FROM_BUFFER(v,b,p)  { memcpy(&(v),(BYTE*)(b)+(p),sizeof(v));(p)+=sizeof(v); }

//将缓冲区中某一位置值复制到用户内存块中  m:内存首地址,b:缓冲区首地址,p:位置,l:字符串字节长度
#define COPY_MEM_FROM_BUFFER(m,b,p,l) { memcpy((BYTE*)(m),(BYTE*)(b)+(p),(l));(p)+=(l); }

	//将缓冲区中某一位置值复制到字符串(缓冲区中的字符串以\0结束,且为ansi编码)  v:变量,b:缓冲区首地址,p:位置
#define COPY_ASTR_FROM_BUFFER(s,b,p)  \
	do{\
		int l = (int)strlen((char*)(b)+(p))+sizeof(char);\
		memcpy((BYTE*)(s),(BYTE*)(b)+(p),l);\
		(p)+=l;\
	}while(0)

	//将缓冲区中某一位置值复制到字符串(缓冲区中的字符串以\0结束,且为unicode编码)  v:变量,b:缓冲区首地址,p:位置
#define COPY_WSTR_FROM_BUFFER(s,b,p)  \
	do{\
		int l =  ((int)wcslen((wchar_t*)((BYTE*)(b)+(p)))+1)*sizeof(wchar_t);\
		memcpy((BYTE*)(s),(BYTE*)(b)+(p),l);\
		(p)+=l;\
	}while(0)

	//将缓冲区中某一位置值复制到字符串(缓冲区中的字符串以\0结束,且为TCHAR编码)  v:变量,b:缓冲区首地址,p:位置
#define COPY_TSTR_FROM_BUFFER(s,b,p)  \
	do{\
		int l = ((int)_tcslen((TCHAR*)((BYTE*)(b)+(p)))+1)*sizeof(TCHAR);\
		memcpy((BYTE*)(s),(BYTE*)(b)+(p),l);\
		(p)+=l;\
	}while(0)

//计算变量大小。v:变量,p:位置
#define CALC_VAR_SIZE(v,p) (p)+=sizeof(v)
#define CALC_VAR_SIZE2(x,y) (sizeof(x)+sizeof(y))
#define CALC_VAR_SIZE3(x,y,z) (sizeof(x)+sizeof(y)+sizeof(z))
#define CALC_VAR_SIZE4(x,y,z0,z1) (sizeof(x)+sizeof(y)+sizeof(z0)+sizeof(z1))

#define ISHEX(c) ((c>='0' && c<='9')||(c>='a'&& c<='f')||(c>='A'&& c<='F'))
#define ISHEX_T(c) ((c>=_T('0') && c<=_T('9'))||(c>=_T('a')&& c<=_T('f'))||(c>=_T('A')&& c<=_T('F')))
#define ISDIGIT(c) ((c>='0' && c<='9'))

#ifndef CLEAR_MEMORY_IN_VECTOR
///<summary>释放和清空指针数组
/// 例如数组声明:vector<int*> arr，则调用方法：ClearMemoryInVector(int*,arr);
///</summary>
#define CLEAR_MEMORY_IN_VECTOR(T,Collection) \
	do{\
		vector<T>::iterator it= Collection.begin();\
		for(;it!=Collection.end();it++){\
			if(*it==NULL) continue;\
			delete *it;\
		}\
		Collection.clear();\
	} while (0)
#endif

#include <string>
#ifdef _UNICODE
	typedef std::wstring  stdString;
#else 
	typedef std::string	  stdString;
#endif



//////////////////////////////////////////////////////////////////////////////////
/// constant define
#define SPACE_CHARS_A " \t\v\r\n\f\a\b" //空白符
#define SPACE_CHARS _T(SPACE_CHARS_A) //空白符

#define SPACE_PATH_CHARS _T(" \t\v\r\n\f\a\b\\/")  //空白字符和路径分隔符
#define STRING_EMPTY _T("") //空字符串

#define KB 1024
#define MB (1024*KB)
#define GB (1024*MB)

#define BUFFERSIZE 1024	//缓冲区大小
#define NEWLINE _T("\r\n") ///换行符


#pragma once

namespace System{
	namespace Data{


class API_CORE MemoryBlock
{
public:
	PBYTE	m_address;
	int		m_size;
public:
	MemoryBlock();
	MemoryBlock(IN PBYTE addr,IN int size);
};
typedef vector<MemoryBlock> BlockCollection;

/// <summary>
/// 内存缓冲区,即将变量输出到缓冲区中
/// </summary>
class API_CORE MemBuffer
{
private:
	BlockCollection	_blocks;
	int				_szBuffer;
	PBYTE			_pBuffer;

public:
	MemBuffer();
	~MemBuffer();

	///<summary>当前所需内存大小</summary>
	int Size();

	///<summary>将内存块压入</summary>
	///<returns>返回当前内存字节数</returns>
	int Push(IN PBYTE addr,IN int size);
	int PushText(IN LPCTSTR str,IN bool isAddEndChar);

	///<summary>压入一块占位内存(即空内存)</summary>
	int PushPlaceHolder(IN int size);

	///<summary>将所有内存块拷贝到缓冲区</summary>
	///<param name="pBuffer">目标缓冲区,PS.调用者自己确保缓冲大小足够</param>
	///<returns>返回拷贝的字节数</returns>
	int CopyTo(IN PBYTE pBuffer);

	///<summary>将缓冲区中的数据拷贝到所有内存块</summary>
	///<param name="pBuffer">源缓冲区,PS.调用者自己确保缓冲大小足够</param>
	///<returns>返回拷贝的字节数</returns>
	int CopyFrom(IN PBYTE pBuffer);

	///<summary>清除所有内存块</summary>
	void Clear();
	
	///<summary>
	/// 申请一块缓冲区并将所有数据块复制到缓冲区中
	/// PS.调用者可以不需要手动释放该缓冲区,因为在实例析构函数中将会释放此缓冲区
	///</summary>
	///<returns>返回缓冲区首地址</returns>
	PBYTE GetBuffer();
	///<summary>手动释放缓冲区</summary>
	void FreeBuffer();
	///<summary>将完整的缓冲区解析到各个变量中</summary>
	///<returns>返回解析的字节数</returns>
	int ParseBufferToVar();
};


///<summary>将一个变量压入到内存缓冲区</summary>
#define MEMORY_PUSH_VAR(inst,v) inst.Push((PBYTE)&v,sizeof(v))

///<summary>将一个字符串压入到内存缓冲区</summary>
#define MEMORY_PUSH_STR(inst,s,isEndChar) inst.PushText(s,isEndChar)

///<summary>将一块内存压入到内存缓冲区</summary>
#define MEMORY_PUSH_MEM(inst,addr,len) inst.Push((PBYTE)addr,len)

///<summary>在内存缓冲区中设置一个空占位</summary>
#define MEMORY_PUSH_PLACEHOLDER(inst,len) inst.PushPlaceHolder(len)

}}
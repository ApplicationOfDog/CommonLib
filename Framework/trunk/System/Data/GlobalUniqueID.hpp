#pragma once

namespace System{
	namespace Data{

/// <summary>
/// GUID操作类
/// </summary>
class API_SYSTEM GlobalUniqueID
{
public:
	static GUID	Null;

private:
	GUID	_data;

public:
	GlobalUniqueID(IN bool isCreate=true);
	GlobalUniqueID(IN GUID& value);

	operator GUID() const;//GUID guid=(GUID)GlobalUniqueID;
	GlobalUniqueID& operator = (IN GUID& value);//GlobalUniqueID val = GUID
	bool operator == (IN GUID& value);
	bool operator == (IN GlobalUniqueID& value);

	///<summary>创建一个新的GUID值</summary>
	bool Create();
	
	///<summary>将GUID转成字符串</summary>
	String ToString();
	///<summary>将字符串转成GUID</summary>
	bool FromString(IN LPCTSTR value);
	
	bool IsNull();

public:
	///<summary>将GUID转成字符串</summary>
	///<param name="source">待转换的GUID</param>
	///<param name="isSeparator">是否加入分隔符</param>
	static String ConvertTo(IN GUID& source,IN bool isSeparator);

	///<summary>将字符串转成GUID</summary>
	///<param name="source">待转换的字符串</param>
	///<param name="dest">目标GUID值</param>
	static bool ConvertTo(IN LPCTSTR source,OUT GUID& dest);

	///<summary>是否为空的GUID(即全0)</summary>
	///<result>true:是空GUID,false:非空</result>
	static bool IsNull(IN GUID& source);
};


}}
#include "stdafx.h"
#include <System.Json.hpp>

///<summary>
/// System.Json.hpp库使用使用实例
///</summary>

///<summary>Json类型测试:
/*
	Json::Value js;
	js["aa"] = 102.5;
	js["bb"] = 115;
	js["cc"] = (UINT64)-1;

	Json_TypeTest(js,"aa");
	Json_TypeTest(js,"bb");
	Json_TypeTest(js,"cc");
*/
///</summary>
void Json_TypeTest(IN Json::Value& js,IN LPCSTR member)
{
	Json::Value temp = js[member];
	String valW = temp.ToString();
	valW.Trim();

	string valA = Convert::ToStringA(valW,IO::EC_ANSI);
	Console::WriteLine("{%s=%s",member,valA.c_str());
	Console::WriteLine(1,"isNumeric:%d,",temp.isNumeric());
	Console::WriteLine(1,"isDouble:%d,",temp.isDouble());
	Console::WriteLine(1,"isInt:%d,",temp.isInt());
	Console::WriteLine(1,"isUInt:%d,",temp.isUInt());
	Console::WriteLine(1,"isInt64:%d,",temp.isInt64());
	Console::WriteLine(1,"isUInt64:%d,",temp.isUInt64());
	Console::WriteLine(1,"isIntegral:%d,",temp.isIntegral());
	Console::WriteLine("}");
}

void Json_Test()
{
	Json::Value jv0;
	JSONCPP_STRING error;
	bool isSuc = jv0.FromString("{\"AA\":150,\"BB\":\"string\",\"CC\":{CC0:121,CC1:'text'}}",false,false,&error);
	//Json::Value jv0("{\"A0\":145,B0:\"text\"}}",true);

	int aa = jv0.GetInt32("AA");
	string bb = jv0.GetStringA("BB");
	int cc0 = jv0.GetInt32("CC.CC0");
	string cc1 = jv0.GetStringA("CC.CC1");

	Console::WriteLine("aa=%d,bb='%s',cc0=%d,cc1='%s'",aa,bb.c_str(),cc0,cc1.c_str());

	Json::Value js;
	js["aa"] = 102.5;
	js["bb"] = 115;
	js["cc"] = (UINT64)-1;

	Json_TypeTest(js,"aa");
	Json_TypeTest(js,"bb");
	Json_TypeTest(js,"cc");

	Json::Value jsResponse;
	string jsError;
	String text = _T("{\"Flag\":1,\"Status\":-1,\"Message\":\"成功同步0条非法资源\",\"Data\":\"\"}");
	isSuc = jsResponse.FromString(text,true,true,&jsError);
	
	//text.Trim(_T("\""));
	Json::Value js2;
	isSuc = js2.FromString(text,true,true);
	int flags = js2.GetInt32("Flag",-1);
	String msg = js2.GetString("Message");
}

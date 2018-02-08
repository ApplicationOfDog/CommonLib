#include "stdafx.h"
#include "JsonValue.hpp"
/*
#define XPATH_CHAR '.'

namespace System{
	namespace Json{
		
		JsonValue::JsonValue()
		{ 
			__super::clear();
		}
		JsonValue::JsonValue(IN const Json::Value& v)
		{
			__super::operator = (v);
		}
		JsonValue::JsonValue(IN LPCTSTR text,IN bool isParse)
		{
			__super::clear();
			if(!isParse)
			{
				if(text==NULL||*text==0)
				{
					__super::operator = ("");
					return;
				}
				std::string astr = Convert::ToStringA(text,IO::EC_ANSI);
				__super::operator =(astr.c_str());
				return;
			}
			
			bool isSuc = FromString(text);
			assert("invalid json text" && isSuc);
		}
		JsonValue::JsonValue(IN LPCSTR text,IN bool isParse)
		{
			__super::clear();
			if(isParse)
			{
				if(text==NULL||*text==0) return;
				__super::operator =(text);
				return;
			}
			
			bool isSuc = FromString(text);
			assert("invalid json text" && isSuc);
		}
		bool JsonValue::FromString(IN LPCTSTR text)
		{
			if(text==NULL||*text==0) return false;

			std::string a_text = Convert::ToStringA(text);
			return FromString(a_text.c_str());
		}
		bool JsonValue::FromString(IN LPCSTR text)
		{
			if(text==NULL||*text==0) return false;			
			
			__super::clear();
			Json::Reader reader;
			return reader.parse(text,*this);
		}
		String JsonValue::ToString(IN bool isFormat)
		{
			if(isFormat)
			{
				std::string outerJson = __super::toStyledString();
				return Convert::ToString(outerJson.c_str(),IO::EC_ANSI);
			}
			Json::FastWriter writer;
			std::string outerJson = writer.write(*this); 
			return Convert::ToString(outerJson.c_str(),IO::EC_ANSI); 
		}
		//////////////////////////////////////////////////////////////////////////
		JsonValue JsonValue::GetMember(IN UINT index)
		{
			Json::Value::const_iterator it = __super::begin();
			while(index-->0 && it!=__super::end()) it++;
			if(it==__super::end()) return JsonValue();

			return *it;
		}
		JsonValue JsonValue::GetMemberByXPath(IN LPCSTR xpath)
		{
			Json::Value* p = getMemberByXPath(xpath);
			if(p==NULL) return Json::Value::null;
			return *p;
		}
		bool JsonValue::HasMember(IN LPCSTR name)
		{
			if(name==NULL||*name==0) return false;
			return __super::isMember(name);
		}

		//////////////////////////////////////////////////////////////////////////
		bool JsonValue::IsNull(){ return __super::isNull();}
		bool JsonValue::IsArray(){ return __super::isArray(); }
		bool JsonValue::IsObject(){ return __super::isObject(); }
		UINT JsonValue::Size(){ return __super::size();}

		string JsonValue::GetStringA(IN LPCSTR key,IN LPCSTR def)
		{
			if(def==NULL) def = "";
			Json::Value* pAttr = getMemberBy(key);
			if(pAttr==NULL || !pAttr->isString()) return def;

			return pAttr->asString();
		}
		
		String JsonValue::GetString(IN LPCSTR key,IN LPCTSTR def,IN IO::Encoding code)
		{			
			Json::Value* pAttr = getMemberBy(key);
			if(pAttr==NULL || !pAttr->isString()) return def;

			std::string text = pAttr->asString();
			return Convert::ToString(text.c_str(),code);
		}
		bool JsonValue::GetBool(IN LPCSTR key,IN bool def)
		{
			Json::Value* pAttr = getMemberBy(key);
			if(pAttr==NULL || !pAttr->isBool()) return def;

			return pAttr->asBool();
		}
		INT32 JsonValue::GetInt32(IN LPCSTR key,IN INT32 def)
		{
			Json::Value* pAttr = getMemberBy(key);
			if(pAttr==NULL || !pAttr->isInt()) return def;

			return pAttr->asInt();
		}
		UINT32 JsonValue::GetUInt32(IN LPCSTR key,IN UINT32 def)
		{
			Json::Value* pAttr = getMemberBy(key);
			if(pAttr==NULL || !pAttr->isUInt()) return def;

			return pAttr->asUInt();
		}
		
		INT64 JsonValue::GetInt64(IN LPCSTR key,IN INT64 def)
		{
			Json::Value* pAttr = getMemberBy(key);
			if(pAttr==NULL || !pAttr->isInt64()) return def;

			return pAttr->asInt64();
		}
		UINT64 JsonValue::GetUInt64(IN LPCSTR key,IN UINT64 def)
		{
			Json::Value* pAttr = getMemberBy(key);
			if(pAttr==NULL || !pAttr->isUInt64()) return def;

			return pAttr->asUInt64();
		}
		double JsonValue::GetDouble(IN LPCSTR key,IN double def)
		{
			Json::Value* pAttr = getMemberBy(key);
			if(pAttr==NULL || !pAttr->isDouble()) return def;

			return pAttr->asDouble();
		}
		bool JsonValue::Set(IN LPCSTR name,IN JsonValue val)
		{
			Json::Value* pAttr = getMemberBy(name);
			if(pAttr==NULL) return false;

			pAttr->swap(val);
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////
		Json::Value* JsonValue::getMemberBy(IN LPCSTR name)
		{
			if(name==NULL) return this;

			assert("invalid parameter 'name'" && *name!=0);
			LPCSTR pCh = strchr(name,XPATH_CHAR);
			if(pCh!=NULL)//包含xpath表达式分隔符,则认为是xpath表达式
				return getMemberByXPath(name);
			
			if(!__super::isObject()||!__super::isMember(name)) return NULL;

			return &(*this)[name];
		}
		Json::Value* JsonValue::getMemberByXPath(IN LPCSTR xpath)
		{
			if(xpath==NULL||*xpath==0)
			{
				assert("invalid parameter" && false);
				return this;
			}
			Json::Value* pJsTemp = this;
			char name[_MAX_FNAME]={0};
			const char* p = xpath,*pItem_s = xpath,*pItem_e = xpath;
			for (;*p!=0;p++)
			{
				if(*p!=XPATH_CHAR)
				{
					pItem_e = p;
					continue;
				}
				if(pItem_e>pItem_s)//123.aa
				{
					ZeroMemory(name,sizeof(name));
					memcpy(name,pItem_s,pItem_e-pItem_s+1);
					pItem_s = p+1;
					if(!pJsTemp->isMember(name)) return NULL;

					pJsTemp = &(*pJsTemp)[name];
				}
			}
			if(pItem_e<=pItem_s) return NULL;

			ZeroMemory(name,sizeof(name));
			memcpy(name,pItem_s,pItem_e-pItem_s+1);
			if(!pJsTemp->isMember(name)) return NULL;

			return &(*pJsTemp)[name];

		}
}}
*/
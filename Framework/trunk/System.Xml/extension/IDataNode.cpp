#include "stdafx.h"
#include "IDataNode.hpp"

namespace System{
	namespace Xml{
		namespace Extension{

			String& IDataNode::operator[](IN LPCSTR key)
			{
				assert(key!=NULL);
				Properties::iterator it = _properties.find(key);
				assert(it!=_properties.end());
				return it->second;
			}
			
			void IDataNode::Load(IN XmlElement element)
			{
				assert(element.type()==node_element);
				_name=element.name();
				_properties.clear();

				std::string key;
				String attr,value;	
				xml_node::attribute_iterator it = element.attributes_begin();
				for (;it!=element.attributes_end();it++){
					attr = it->name();
					key = Convert::ToStringA(attr);
					value = it->value();
					value.Trim();
					_properties.insert(std::make_pair(key,value));
				}
			}
			xml_node IDataNode::SaveTo(XmlElement element)
			{
				assert(element.type()==node_element);
				assert(!_name.IsEmptyOrWhiteSpace());

				String name;
				xml_attribute attr;
				xml_node node = element.append_child(_name.c_str());
				Properties::iterator it = _properties.begin();

				std::string key;
				for (;it!=_properties.end();it++){
					key = it->first;
					name = Convert::ToString(key);
					attr = node.append_attribute(name.c_str());
					attr.set_value((const char_t*)it->second);
				}
				return node;
			}


			///////////////////////////////////////////////////////////////////////////////
			///inner
			String IDataNode::Get(IN LPCSTR key,IN String def)
			{
				assert(key!=NULL && *key!=0);
				Properties::iterator it = _properties.find(key);
				if(it==_properties.end()) return def;
				return it->second;
			}
			void IDataNode::Set(IN LPCSTR key,IN String val)
			{
				//对于不存在的key,通过[]引用这个key,默认会创建一个这个key的对应的value的初始值
				assert(key!=NULL && *key!=0);
				_properties[key]=val;
			}
			int IDataNode::Get(IN LPCSTR key,IN int def)
			{
				assert(key!=NULL && *key!=0);
				Properties::iterator it = _properties.find(key);
				if(it==_properties.end()) return def;
				int endIdx=0;
				int result = Convert::ToInt(it->second,10,&endIdx);
				return endIdx==0?def:result;
			}
			bool IDataNode::Get_bool(IN LPCSTR key,IN bool def)
			{
				assert(key!=NULL && *key!=0);
				Properties::iterator it = _properties.find(key);
				if(it==_properties.end()) return def;
				String value = it->second;
				if(value.Compare(_T("true"),true)==0||value.Compare(_T("yes"),true)==0) return true;
				if(value.Compare(_T("0"),false)==0) return false;
				return value.IsNumeric();
			}
			double IDataNode::Get_double(IN LPCSTR key,IN double def)
			{
				assert(key!=NULL && *key!=0);
				Properties::iterator it = _properties.find(key);
				if(it==_properties.end()) return def;
				return Convert::ToDouble(it->second);
			}

			void IDataNode::Set(IN LPCSTR key,IN int val)
			{
				//对于不存在的key,通过[]引用这个key,默认会创建一个这个key的对应的value的初始值
				assert(key!=NULL && *key!=0);
				_properties[key]= Convert::ToString(val);
			}
			void IDataNode::Clear(IN LPCSTR key)
			{
				Properties::iterator it;
				//清除特定属性值
				if(key!=NULL && *key!=0){
					it= _properties.find(key);
					if(it==_properties.end()) return;
					_properties.erase(it);
					return;
				}

				//参数为空表示清除所有属性的值
				_name.clear();
				_properties.clear();
			}
}}}
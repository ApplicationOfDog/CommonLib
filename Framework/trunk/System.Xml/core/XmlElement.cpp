#include "stdafx.h"
#include "XmlElement.hpp"

namespace System{
	namespace Xml{
		
		XmlElement::XmlElement():XmlNode(){}
		XmlElement::XmlElement(xml_node node)
		{
			if(node.type()!=node_element) return;
			_root=node.internal_object();
		}
		XmlElement::XmlElement(XmlNode node)
		{
			if(node.type()!=node_element) return;
			_root=node.internal_object();
		}

		XmlElement& XmlElement::operator =(xml_node node)
		{
			if(node.type()==node_element)
				XmlNode::operator =(node);
			return *this;
		}
		XmlElement& XmlElement::operator =(XmlNode node)
		{
			if(node.type()==node_element)
				XmlNode::operator =(node);
			return *this;
		}
		String XmlElement::operator[](LPCTSTR name)
		{
			return GetAttribute(name);
		}

		String XmlElement::GetAttribute(IN LPCTSTR name)
		{
			xml_attribute xa = attribute(name);
			if(!xa) return STRING_EMPTY;
			return String(xa.value());
		}
		String XmlElement::GetAttribute(IN LPCTSTR name,IN LPCTSTR def)
		{
			xml_attribute xa = attribute(name);
			if(!xa) return def==NULL?String::Empty:def;
			return String(xa.value());
		}
		int XmlElement::GetAttribute(IN LPCTSTR name,IN int def)
		{
			return GetAttribute_int(name,def);
		}
		int XmlElement::GetAttribute_int(IN LPCTSTR name,IN int def)
		{
			xml_attribute xa = attribute(name);
			if(!xa) return def;
			return xa.as_int(def);
		}
		UINT XmlElement::GetAttribute_uint(IN LPCTSTR name,IN UINT def)
		{
			xml_attribute xa = attribute(name);
			if(!xa) return def;
			return xa.as_uint(def);
		}
		LONGLONG XmlElement::GetAttribute_ll(IN LPCTSTR name,IN LONGLONG def)
		{
			xml_attribute xa = attribute(name);
			if(!xa) return def;
			return xa.as_llong(def);
		}
		ULONGLONG XmlElement::GetAttribute_ull(IN LPCTSTR name,IN ULONGLONG def)
		{
			xml_attribute xa = attribute(name);
			if(!xa) return def;
			return xa.as_ullong(def);
		}
		double XmlElement::GetAttribute_double(IN LPCTSTR name,IN double def)
		{
			xml_attribute xa = attribute(name);
			if(!xa) return def;
			return xa.as_double(def);
		}
		float XmlElement::GetAttribute_float(IN LPCTSTR name,IN float def)
		{
			xml_attribute xa = attribute(name);
			if(!xa) return def;
			return xa.as_float(def);
		}
		bool XmlElement::GetAttribute(IN LPCTSTR name,IN bool def)
		{
			return GetAttribute_bool(name,def);
		}
		bool XmlElement::GetAttribute_bool(IN LPCTSTR name,IN bool def)
		{
			xml_attribute xa = attribute(name);
			if(!xa) return def;
			String val = xa.as_string();
			if(val.Compare(_T("true"),true)==0|| val.Compare(_T("yes"),true)==0) return true;
			if(val.Compare(_T("0"),false)==0) return false;
			return val.IsNumeric();
		}

		XmlAttribute XmlElement::GetAttributeNode(IN LPCTSTR name)
		{
			return attribute(name);
		}
		bool XmlElement::SetAttribute(IN LPCTSTR name,IN LPCTSTR value)
		{
			xml_attribute xa = attribute(name);
			if(!xa)
			{
				xa = append_attribute(name);
				if(!xa) return false;
			}
			return xa.set_value(value);
		}
		bool XmlElement::SetAttribute(IN LPCTSTR name,IN int value)
		{
			xml_attribute xa = attribute(name);
			if(!xa)
			{
				xa = append_attribute(name);
				if(!xa) return false;
			}
			return xa.set_value(value);
		}
		bool XmlElement::SetAttribute(IN LPCTSTR name,IN UINT64 val)
		{
			xml_attribute xa = attribute(name);
			if(!xa)
			{
				xa = append_attribute(name);
				if(!xa) return false;
			}
			return xa.set_value((unsigned long long)val);
		}
		bool XmlElement::RemoveAttribute(IN LPCTSTR name){
			return remove_attribute(name);
		}
		bool XmlElement::HasAttributes(){
			return !first_attribute().empty();
		}
		bool XmlElement::HasAttributes(LPCTSTR name){
			xml_attribute attr = attribute(name);
			return !attr.empty();
		}

		XmlAttribute XmlElement::SelectSingleNodeAttribute(LPCTSTR xpath,LPCTSTR attribute)
		{
			xpath_node node = select_single_node(xpath);
			if(!node) return XmlAttribute();
			xml_node xn = node.node();
			if(xn.type()!=node_element) return XmlAttribute();
			return xn.attribute(attribute);
		}
		String XmlElement::SelectSingleNodeAttrValue(LPCTSTR xpath,LPCTSTR attribute,LPCTSTR def)
		{
			XmlAttribute xa = SelectSingleNodeAttribute(xpath,attribute);
			LPCTSTR value = xa.Get(def);
			if(value==NULL) return String::Empty;
			return String(value);
		}
		int XmlElement::SelectSingleNodeAttrValue_i(LPCTSTR xpath,LPCTSTR attribute,int def)
		{
			XmlAttribute xa = SelectSingleNodeAttribute(xpath,attribute);
			return xa.Get(def);
		}
		LONGLONG XmlElement::SelectSingleNodeAttrValue_ll(LPCTSTR xpath,LPCTSTR attribute,LONGLONG def)
		{
			XmlAttribute xa = SelectSingleNodeAttribute(xpath,attribute);
			return xa.GetLlong(def);
		}
		ULONGLONG XmlElement::SelectSingleNodeAttrValue_ull(LPCTSTR xpath,LPCTSTR attribute,ULONGLONG def)
		{
			XmlAttribute xa = SelectSingleNodeAttribute(xpath,attribute);
			return xa.GetUllong(def);
		}
		bool XmlElement::SelectNodesAttrValue(IN LPCTSTR xpath,IN LPCTSTR attribute,OUT vector<String>& collection)
		{
			collection.clear();
			xpath_node_set nodes = select_nodes(xpath);
			if(nodes.empty()) return false;

			XmlElement element ;
			xpath_node_set::const_iterator it = nodes.begin();
			for (;it!=nodes.end();it++)
			{
				element = it->node();
				if(!element||!element.HasAttributes(attribute)) continue;				
				collection.push_back(element.GetAttribute(attribute));
			}
			return true;
		}
		XmlElement XmlElement::AppendChild(IN LPCTSTR name)
		{
			assert(name!=NULL && *name!=0);
			return XmlElement(append_child(name));
		}
		bool XmlElement::AppendText(IN LPCTSTR val)
		{
			if(val==NULL||*val==0) return false;
			xml_node::text().set(val);
			return true;
		}
		void XmlElement::AppendText(IN int val)
		{
			xml_node::text().set(val);
		}
}}
#include "stdafx.h"
#include "XmlNode.hpp"
#include "XmlElement.hpp"
#include "XmlNodeList.hpp"

namespace System{
	namespace Xml{
		
		XmlNode::XmlNode():xml_node(){}
		XmlNode::XmlNode(xml_node xn):xml_node(xn.internal_object()){}
		XmlNode::XmlNode(const xpath_node xpn){
			if(!xpn.node()) return;
			_root=xpn.node().internal_object();
		}
		XmlNode::XmlNode(xpath_node_set::const_iterator it){
			if(it==NULL||!it->node()) return;
			_root=it->node().internal_object();
		}
		
		XmlNode& XmlNode::operator=(xml_node value){
			_root=value.internal_object();
			return *this;
		}
		BOOL XmlNode::IsNull(){
			return empty();
		}
		XmlNode XmlNode::SelectSingleNode(String xpath){
			xpath_node xn = select_single_node(xpath.c_str());
			return xn.node();
		}
		XmlElement XmlNode::SelectSingleElement(IN String xpath){
			if(type()!=node_element) return XmlElement();
			xpath_node xn = select_single_node(xpath);
			return xn.node();
		}
		
		XmlElement XmlNode::FindElement(IN LPCTSTR name){
			if(_root==NULL||name==NULL) return XmlElement();
			for (xml_node node=first_child();node;node=node.next_sibling()){
				if(node.type()== node_element && _tcscmp(name,node.name())==0) return node; 
			}
			return XmlElement();
		}
		XmlNodeList XmlNode::SelectNodes(IN LPCTSTR xpath){
			return select_nodes(xpath);
		}

		BOOL XmlNode::HasChildNodes(){
			return !first_child().empty();
		}
		
		//////////////////////////////////////////////////////////////////////////////////////////
		///private
		XmlNode XmlNode::Convert(xml_node xn)
		{
			XmlNode result = xn;
			return result;
		}
}}
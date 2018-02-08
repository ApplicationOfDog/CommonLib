#include "stdafx.h"
#include "XmlNodeList.hpp"

namespace System{
	namespace Xml{
		
		XmlNodeList::XmlNodeList(){}
		XmlNodeList::XmlNodeList(xpath_node_set nodes):xpath_node_set(nodes){}
		
		XmlNode XmlNodeList::operator[](int i){
			return xpath_node_set::operator[](i);
		}

		XmlNode XmlNodeList::Item(int index){
			return xpath_node_set::operator[](index);
		}
}}
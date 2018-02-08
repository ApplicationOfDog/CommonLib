#pragma once
#include "XmlNode.hpp"

namespace System
{
	namespace Xml
	{
		class API_Xml XmlNodeList:public xpath_node_set
		{
		public:
			typedef const xpath_node* const_iterator;

		public:
			XmlNodeList();
			XmlNodeList(xpath_node_set nodes);

			///<summary>获取给定索引处的节点</summary>
			///<param name="i">节点列表中从零开始的索引</param>
			///<returns>System.Xml.XmlNode 与集合中的指定索引. 如果 index 大于或等于列表中的节点数，则这返回 null</returns>
			XmlNode operator[](int i);

			///<summary>获取 XmlNodeList 中的节点数</summary>
			int Count(){ return (int)size(); }
			
			///<summary>检索给定索引处的节点</summary>
			///<param name="index">节点列表中从零开始的索引</param>
			///<returns>System.Xml.XmlNode 与集合中的指定索引. 如果 index 大于或等于列表中的节点数，则这返回 null</returns>
			XmlNode Item(int index);

			XmlNodeList::const_iterator Begin(){ return begin(); };
			XmlNodeList::const_iterator End(){ return end(); }
		};
	}
}
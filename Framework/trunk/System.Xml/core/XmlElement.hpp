#pragma once
#include "XmlNode.hpp"
#include "XmlAttribute.hpp"

namespace System
{
	namespace Xml
	{
		class API_Xml XmlElement:public XmlNode
		{
		public:
			XmlElement();
			XmlElement(xml_node node);
			XmlElement(XmlNode node);
			XmlElement& operator=(xml_node node);
			XmlElement& operator=(XmlNode node);

			///<summary>获取具有指定名称的属性值</summary>
			///<param name="name">属性名称</param>
			///<returns>属性值字符串，如果不存在属性则返回空串</returns>
			String operator[](LPCTSTR name);

		public:
			///<summary>返回具有指定名称的特性的值</summary>
			///<param name="name">属性名称</param>
			///<returns>属性值</returns>
			String GetAttribute(IN LPCTSTR name);
			String GetAttribute(IN LPCTSTR name,IN LPCTSTR def);

			///<summary>获取布尔值类型</summary>
			///<returns>"true"或"yes"返回true,如果是不为0的数值返回true,其他返回false</returns>
			bool GetAttribute(IN LPCTSTR name,IN bool def);
			int GetAttribute(IN LPCTSTR name,IN int def);
			bool GetAttribute_bool(IN LPCTSTR name,IN bool def=0);
			int GetAttribute_int(IN LPCTSTR name,IN int def=0);
			UINT GetAttribute_uint(IN LPCTSTR name,IN UINT def=0);
			LONGLONG GetAttribute_ll(IN LPCTSTR name,IN LONGLONG def=0);
			ULONGLONG GetAttribute_ull(IN LPCTSTR name,IN ULONGLONG def=0);
			double GetAttribute_double(IN LPCTSTR name,IN double def=0.0);
			float GetAttribute_float(IN LPCTSTR name,IN float def=0.0);				

			///<summary>返回具有指定名称的 XmlAttribute</summary>
			///<param name="name">属性名称</param>
			///<returns>属性对象</returns>
			XmlAttribute GetAttributeNode(IN LPCTSTR name);

			///<summary>设置具有指定名称的特性的值</summary>
			///<param name="name">属性名称</param>
			///<param name="value">属性值</param>
			///<returns>是否成功</returns>
			bool SetAttribute(IN LPCTSTR name,IN LPCTSTR value);
			bool SetAttribute(IN LPCTSTR name,IN int value);
			bool SetAttribute(IN LPCTSTR name,IN UINT64 val);

			//
			// 摘要:
			//     按名称移除特性。
			//
			// 参数:
			//   name:
			//     要移除的特性的名称。这是限定名。 它针对匹配节点的 Name 属性进行匹配。
			bool RemoveAttribute(IN LPCTSTR name);
			//
			// 摘要:
			//     获取一个 boolean 值，该值指示当前节点是否有任何特性。
			//
			// 返回结果:
			//     如果当前节点具有属性，则为 true；否则为 false。
			bool HasAttributes();
			bool HasAttributes(LPCTSTR name);

			///<summary>选择匹配xpath表达式的第一个 XmlElement的属性attribute的值</summary>
			///<param name="attribute">节点的属性名称</param>
			///<param name="def">如果节点不存在的情况下返回的默认值</param>
			///<returns>属性值,如果节点不存在返回def值</returns>			
			String SelectSingleNodeAttrValue(LPCTSTR xpath,LPCTSTR attribute,LPCTSTR def=NULL);
			int SelectSingleNodeAttrValue_i(LPCTSTR xpath,LPCTSTR attribute,int def=0);
			LONGLONG SelectSingleNodeAttrValue_ll(LPCTSTR xpath,LPCTSTR attribute,LONGLONG def=0);
			ULONGLONG SelectSingleNodeAttrValue_ull(LPCTSTR xpath,LPCTSTR attribute,ULONGLONG def=0);
			XmlAttribute SelectSingleNodeAttribute(LPCTSTR xpath,LPCTSTR attribute);

			///<summary>选择匹配xpath表达式的XmlElement集合的属性attribute的值</summary>
			///<param name="attribute">节点的属性名称</param>
			///<param name="collection">所有节点对应的属性值</param>
			///<returns>TRUE:获取成功,获取值在collection中，false:获取失败</returns>
			bool SelectNodesAttrValue(IN LPCTSTR xpath,IN LPCTSTR attribute,OUT vector<String>& collection);

			///<summary>
			/// 将指定的节点添加到该节点的子节点列表的末尾
			/// 也就是说:只能通过先添加节点再为返回的节点添加属性列表
			///</summary>
			///<param name="name">要添加的节点名称</param>
			///<returns>返回新增的节点对象</returns>
			XmlElement AppendChild(IN LPCTSTR name);

			///<summary>向当前元素节点中添加文本节点</summary>
			///<param name="text">文本字符串</param>
			///<returns>是否成功</returns>
			bool AppendText(IN LPCTSTR val);
			void AppendText(IN int val);
		};
	}
}
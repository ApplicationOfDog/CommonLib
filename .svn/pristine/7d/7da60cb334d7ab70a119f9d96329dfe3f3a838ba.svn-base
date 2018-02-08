#pragma once

namespace System
{
	namespace Xml
	{
		typedef xml_node_type XmlNodeType;
		class XmlElement;
		class XmlNodeList;
		class API_Xml XmlNode:public xml_node
		{
		public:
			XmlNode();
			XmlNode(xml_node xn);
			XmlNode(const xpath_node xpn);
			XmlNode(xpath_node_set::const_iterator it);
			LPCTSTR Name(){ return name(); }
			LPCTSTR Value(){ return value(); }
			XmlNodeType NodeType(){ return type(); }		
			XmlNode& operator =(xml_node value);
			// 摘要:
			//	   未初始化的文档对象
			BOOL IsNull();

		public:
			//
			// 摘要:
			//     获取该节点（对于可以具有父级的节点）的父级。
			//
			// 返回结果:
			//     为当前节点父级的 XmlNode。 如果节点刚刚创建还未添加到树中，或如果已从树中移除了节点，则父级为 null。 对于所有其他节点，返回的值取决于节点的
			//     System.Xml.XmlNode.NodeType。 下表描述 ParentNode 属性可能的返回值。 NodeType ParentNode
			//     的返回值 Attribute、Document、DocumentFragment、Entity、Notation 返回 null；这些节点不具有父级。
			//     CDATA 返回包含 CDATA 节的元素或实体引用。 注释 返回包含注释的元素、实体引用、文档类型或文档。 DocumentType 返回文档节点。
			//     元素 返回该元素的父节点。 如果该元素是树中的根节点，则父级是文档节点。 EntityReference 返回包含该实体引用的元素、特性或实体引用。
			//     ProcessingInstruction 返回包含该处理指令的文档、元素、文档类型或实体引用。 Text 返回包含该文本节点的父元素、特性或实体引用。
			XmlNode ParentNode(){ return Convert(parent()); }
			//
			// 摘要:
			//     获取紧接在该节点之前的节点。
			//
			// 返回结果:
			//     前一个 XmlNode。 如果前面没有节点，则返回 null。
			XmlNode PreviousSibling(){ return Convert(previous_sibling()); }
			//
			// 摘要:
			//     获取节点的第一个子级。
			//
			// 返回结果:
			//     节点的第一个子级。 如果没有这样的节点，则返回 null。
			XmlNode FirstChild(){ return Convert(first_child()); }
			//
			// 摘要:
			//     获取节点的最后一个子级。
			//
			// 返回结果:
			//     节点的最后一个子级。 如果没有这样的节点，则返回 null。
			XmlNode LastChild(){ return Convert(last_child()); }

			///<summary>选择匹配 XPath 表达式的第一个 XmlNode</summary>
			///<param name="xpath">XPath 表达式</param>
			///<returns>与 XPath 查询匹配的第一个 XmlNode；如果未找到任何匹配节点，则为 null。 不应该要求将 XmlNode“实时”连接到 XML 文档。
			//     也就是说，XML 文档中的更改不会出现在 XmlNode 中，反之亦然
			///</returns>
			XmlNode SelectSingleNode(IN String xpath);
			XmlElement SelectSingleElement(IN String xpath);

			///<summary>获取类型为node_element的第一个名称为name的子节点</summary>
			///<param name="name">要匹配的子节点名称</param>
			///<returns>返回XmlElement类型的子节点,如果没匹配项则返回的XmlElement为空节点</returns>
			XmlElement FindElement(IN LPCTSTR name);

			///<summary>选择匹配 XPath 表达式的节点列表</summary>
			///<param name="xpath">XPath 表达式</param>
			///<returns>一个 System.Xml.XmlNodeList，包含匹配 XPath 查询的节点集合</returns>
			XmlNodeList SelectNodes(IN LPCTSTR xpath);

			///<summary>获取一个值，该值指示节点是否有任何子节点</summary>
			///<returns>如果节点具有子节点，则为 true；否则为 false</returns>
			BOOL HasChildNodes();

		private:
			XmlNode Convert(xml_node xn);
		};
	}
}
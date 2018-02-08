#pragma once
#include "XmlElement.hpp"
#include "XmlNodeList.hpp"

namespace System
{
	namespace Xml
	{
		///<summary>Xml文档类</summary>
		class API_Xml XmlDocument:public xml_document
		{
		private:
			xml_parse_result _lastError;

		public:
			XmlElement Document();

			///<summary>返回根节点</summary>
			XmlNode Root();

		public:
			xml_parse_status GetLastError(OUT INT64* pOffset = NULL);
			String GetLastErrorText();
			LPCSTR GetLastErrorTextA();

			///<summary>未初始化的文档对象</summary>
			bool IsNull();

			///<summary>从指定的 URL 加载 XML 文档</summary>
			///<param name="filename">文件的 URL，该文件包含要加载的 XML 文档。 URL 既可以是本地文件，也可以是 HTTP URL（Web 地址）</param>
			bool Load(LPCTSTR filename,xml_encoding encoding = encoding_auto);

			///<summary>从指定的字符串加载 XML 文档</summary>
			///<param name="xml">包含要加载的 XML 文档的字符串</param>
			bool LoadXml(LPCTSTR xml);
			bool LoadXml(void* buffer,size_t size,xml_encoding encoding = encoding_auto);

			///<summary>将XML文档保存到指定的文件</summary>
			///<param name="filename"> 要将文档保存到其中的文件的位置</param>
			bool SaveFile(LPCTSTR filename,xml_encoding encoding = encoding_auto);
			///<summary>将XML文档保存到字符串缓冲区中</summary>
			///<param name="buffer">字符串缓冲区</param>
			void SaveBuffer(OUT string& buffer,xml_encoding encoding = encoding_auto);
			void SaveBuffer(OUT wstring& buffer);

			///<summary>
			///获取文档缓冲区.
			///请切记不要修改缓冲区的值,否则可能导致严重错误.
			///该接口用于方便文件加解密的
			///备注:此接口只对从文件中load的文档有效,而针对新XmlDocument(通过代码添加节点等)此接口返回NULL(可通过SaveBuffer获取新文档的缓冲区)
			///</summary>
			///<param name="length">返回缓冲区字符串长度，即字符个数</param>
			///<param name="szElement">字符元素所在的字节数，如果是宽字符值为2，否则为1</param>
			///<returns>返回文档缓冲区</returns>
			LPCTSTR GetBuffer(OUT int* length=NULL,OUT int* szElement=NULL);

			///<summary>复制缓冲区</summary>
			///<param name="buffer">目标缓冲区</param>
			///<param name="size">目标缓冲区大小</param>
			void CopyBuffer(OUT PBYTE buffer,OUT int size);
			///<param name="buffer">目标缓冲区,函数内部为缓冲区申请空间并返回到*buffer中</param>
			///<param name="size">返回申请的目标缓冲区大小</param>
			///<returns>true:拷贝成功,false:发生错误</returns>
			bool CopyBuffer(OUT PBYTE* buffer,OUT int* size);

			///<summary>选择匹配 XPath 表达式的第一个 XmlNode</summary>
			///<param name="xpath">XPath 表达式
			/// 例如:"/USER/ITEM[@LogonName='AW']"表示获取USER节点下LogonName为'AW'的ITEM节点
			///</param>
			///<returns> 与 XPath 查询匹配的第一个 XmlNode；如果未找到任何匹配节点，则为 null。 
			// 不应该要求将 XmlNode“实时”连接到 XML 文档。也就是说，XML 文档中的更改不会出现在 XmlNode 中，反之亦然。
			///</returns>
			XmlNode SelectSingleNode(IN LPCTSTR xpath);
			///<param name="xpath">XPath 表达式
			/// 例如:"/USER/ITEM[@LogonName='AW']"表示获取USER节点下LogonName为'AW'的ITEM节点
			///</param>
			XmlElement SelectSingleElement(IN LPCTSTR xpath);

			///<summary>选择匹配xpath表达式的第一个 XmlNode的属性attribute的值</summary>
			///<param name="xpath">表示文档中的节点路径,如果值为NULL则表示获取根节点的属性值,例如:"/Root/Book"</param>
			///<param name="attribute">节点的属性名称</param>
			///<param name="def">如果节点不存在的情况下返回的默认值</param>
			///<returns>属性值,如果节点不存在返回def值</returns>
			String SelectSingleNodeAttrValue(IN LPCTSTR xpath,IN LPCTSTR attribute,IN LPCTSTR def=NULL);
			int SelectSingleNodeAttrValue_i(IN LPCTSTR xpath,IN LPCTSTR attribute,IN int def=0);
			LONGLONG SelectSingleNodeAttrValue_ll(IN LPCTSTR xpath,IN LPCTSTR attribute,IN LONGLONG def=0);
			ULONGLONG SelectSingleNodeAttrValue_ull(IN LPCTSTR xpath,IN LPCTSTR attribute,IN ULONGLONG def=0);
			XmlAttribute SelectSingleNodeAttribute(IN LPCTSTR xpath,IN LPCTSTR attribute);

			///<summary>选择匹配xpath表达式的XmlElement集合的属性attribute的值</summary>
			///<param name="attribute">节点的属性名称</param>
			///<param name="collection">所有节点对应的属性值</param>
			///<returns>TRUE:获取成功,获取值在collection中，false:获取失败</returns>
			bool SelectNodesAttrValue(IN LPCTSTR xpath,IN LPCTSTR attribute,OUT vector<String>& collection);

			///<summary>选择匹配XPath表达式的节点列表</summary>
			///<param name="xpath">XPath 表达式
			/// 例如:"/USER/ITEM[@LogonName='AW']"表示获取USER节点下LogonName为'AW'的ITEM节点列表
			///</param>
			///<returns>匹配的节点列表<</returns>
			XmlNodeList SelectNodes(IN LPCTSTR xpath);

			///<summary>获取类型为node_element的第一个名称为name的子节点</summary>
			///<param name="name">要匹配的子节点名称</param>
			///<returns>返回XmlElement类型的子节点,如果没匹配项则返回的XmlElement为空节点</returns>
			XmlElement FindElement(IN LPCTSTR name);

			///<summary>获取一个值，该值指示节点是否有任何子节点</summary>
			///<returns> 如果节点具有子节点，则为 true；否则为 false</returns>
			bool HasChildNodes();
			///<summary>重置文档,删除所有子节点剩下空文档</summary>
			void Reset(){ reset(); }

			///<summary>向Xml文档添加根节点</summary>
			///<param name="name">要添加的节点名称</param>
			///<returns>添加的节点,节点是node_element类型的</returns>
			XmlElement AppendRoot(IN LPCTSTR name);
			
		protected:
			void ClearLastError();
		};
	}
}
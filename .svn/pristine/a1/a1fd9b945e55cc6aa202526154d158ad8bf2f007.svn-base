#include "stdafx.h"
#include "XmlDocument.hpp"
#include <sstream>

namespace System{
	namespace Xml{
		
		XmlElement XmlDocument::Document(){
			return XmlElement(document_element());
		}
		XmlNode XmlDocument::Root(){
			return XmlNode(root());
		}
		xml_parse_status XmlDocument::GetLastError(OUT INT64* pOffset)
		{
			if(pOffset!=NULL) *pOffset = _lastError.offset;
			return _lastError.status;
		}
		String XmlDocument::GetLastErrorText(){
			std::string text = _lastError.description();
			return Convert::ToString(text);
		}
		LPCSTR XmlDocument::GetLastErrorTextA(){
			return _lastError.description();
		}

		bool XmlDocument::IsNull(){
			return empty();
		}

		bool XmlDocument::Load(LPCTSTR filename,xml_encoding encoding){
			_lastError = load_file(filename,parse_default,encoding);
			return _lastError.status==status_ok;
		}

		bool XmlDocument::LoadXml(LPCTSTR xml){
			_lastError =  load(xml,parse_default);
			return _lastError.status==status_ok;
		}

		bool XmlDocument::LoadXml(void* buffer,size_t size,xml_encoding encoding){
			_lastError =  load_buffer(buffer,size,parse_default,encoding);
			return _lastError.status==status_ok;
		}

		bool XmlDocument::SaveFile(LPCTSTR filename,xml_encoding encoding)
		{
			return save_file(filename,PUGIXML_TEXT("\t"),format_default,encoding);
		}
		void XmlDocument::SaveBuffer(OUT string& buffer,xml_encoding encoding)
		{
			ostringstream output;
			save(output,PUGIXML_TEXT("\t"),format_default,encoding);
			buffer = output.str();
		}
		void XmlDocument::SaveBuffer(OUT wstring& buffer)
		{
			wostringstream output;
			save(output,PUGIXML_TEXT("\t"),format_default);
			buffer = output.str();
		}
		
		LPCTSTR XmlDocument::GetBuffer(OUT int* length,OUT int* szElement)
		{
			LPCTSTR buffer = document_buffer();
#ifdef PUGIXML_WCHAR_MODE
			if(length!=NULL)
				*length = buffer==NULL?-1:(int)wcslen(buffer);
#else
			if(length!=NULL)
				*length = buffer==NULL?-1:(int)strlen(buffer);
#endif
			if(szElement!=NULL)
				*szElement=sizeof(char_t);
			return buffer;
		}
		void XmlDocument::CopyBuffer(OUT PBYTE buffer,OUT int size)
		{
			assert(buffer!=NULL && size>0);

			int lenBuffer=0,szBuffer=0;
			LPCTSTR docBuffer = GetBuffer(&lenBuffer,&szBuffer);
			int nCopy=min(size,lenBuffer*szBuffer);
			memcpy(buffer,docBuffer,nCopy);
		}
		bool XmlDocument::CopyBuffer(OUT PBYTE* buffer,OUT int* size)
		{
			assert(buffer!=NULL);
			ClearLastError();

			*buffer=NULL;
			int lenBuffer=0,szElement=0,szNew=0;
			LPCTSTR docBuffer = GetBuffer(&lenBuffer,&szElement);
			if(docBuffer==NULL){
				_lastError.status = status_io_error;
				return false;
			}

			szNew = (lenBuffer+1)*szElement;
			if(size!=NULL) *size=szNew;
			PBYTE newBuffer=new BYTE[szNew];
			if(newBuffer==NULL) {
				if(size!=NULL) *size=0;
				_lastError.status=status_out_of_memory;
				return false;
			}
			memcpy(newBuffer,docBuffer,szNew);//复制缓冲区
			ZeroMemory(newBuffer+(szNew-szElement),szElement);//最后一个字符赋值为'\0'
			*buffer = newBuffer;
			return true;
		}
		XmlNode XmlDocument::SelectSingleNode(IN LPCTSTR xpath)
		{
			assert(xpath!=NULL && *xpath!=0);
			xpath_node xn = select_single_node(xpath);
			return xn.node();
		}
		XmlElement XmlDocument::SelectSingleElement(IN LPCTSTR xpath)
		{
			assert(xpath!=NULL && *xpath!=0);
			xpath_node xn = select_single_node(xpath);
			return xn.node();
		}
		XmlElement XmlDocument::FindElement(IN LPCTSTR name)
		{
			if(_root==NULL||name==NULL) return XmlElement();
			for (xml_node node=first_child();node;node=node.next_sibling()){
				if(node.type()== node_element && _tcscmp(name,node.name())==0) return node; 
			}
			return XmlElement();
		}
		String XmlDocument::SelectSingleNodeAttrValue(LPCTSTR xpath,LPCTSTR attribute,LPCTSTR def)
		{
			XmlElement root = Document();
			if(root.IsNull()) return String::Empty;
			if(xpath==NULL) return root.GetAttribute(attribute,def);
			return root.SelectSingleNodeAttrValue(xpath,attribute,def);
		}
		int XmlDocument::SelectSingleNodeAttrValue_i(LPCTSTR xpath,LPCTSTR attribute,int def)
		{
			XmlElement root = Document();
			if(root.IsNull()) return def;
			if(xpath==NULL) return root.GetAttribute_int(attribute,def);
			return root.SelectSingleNodeAttrValue_i(xpath,attribute,def);
		}
		LONGLONG XmlDocument::SelectSingleNodeAttrValue_ll(LPCTSTR xpath,LPCTSTR attribute,LONGLONG def)
		{
			XmlElement root = Document();
			if(root.IsNull()) return def;
			if(xpath==NULL) return root.GetAttribute_ll(attribute,def);
			return root.SelectSingleNodeAttrValue_ll(xpath,attribute,def);
		}
		ULONGLONG XmlDocument::SelectSingleNodeAttrValue_ull(LPCTSTR xpath,LPCTSTR attribute,ULONGLONG def)
		{
			XmlElement root = Document();
			if(root.IsNull()) return def;
			if(xpath==NULL) return root.GetAttribute_ull(attribute,def);
			return root.SelectSingleNodeAttrValue_ull(xpath,attribute,def);
		}
		XmlAttribute XmlDocument::SelectSingleNodeAttribute(LPCTSTR xpath,LPCTSTR attribute)
		{
			XmlElement root = Document();
			if(root.IsNull()) return XmlAttribute();
			return root.SelectSingleNodeAttribute(xpath,attribute);
		}
		bool XmlDocument::SelectNodesAttrValue(IN LPCTSTR xpath,IN LPCTSTR attribute,OUT vector<String>& collection)
		{
			XmlElement root = Document();
			if(root.IsNull()) return false;
			return root.SelectNodesAttrValue(xpath,attribute,collection);
		}
		XmlNodeList XmlDocument::SelectNodes(IN LPCTSTR xpath){
			return Document().select_nodes(xpath);
		}

		bool XmlDocument::HasChildNodes()
		{
			return !first_child().empty();
		}
		XmlElement XmlDocument::AppendRoot(IN LPCTSTR name)
		{
			assert(name!=NULL && *name!=0);
			return XmlElement(append_child(name));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		////private
		void XmlDocument::ClearLastError()
		{
			_lastError.status=status_ok;
			_lastError.offset=0x0;
			_lastError.encoding=encoding_auto;
		}
}}
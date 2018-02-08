#pragma once
#include <map>
#include "../Core/XmlElement.hpp"

namespace System{
	namespace Xml{
		namespace Extension{

		typedef std::map<std::string,String> Properties;
		class API_Xml IDataNode
		{
		protected:
			String _name;
			Properties _properties;

		public:
			String Name(){ return _name; }
			void Name(IN String val){ _name=val; }
			String& operator[](IN LPCSTR key);

			virtual void Load(IN XmlElement element);
			virtual xml_node SaveTo(IN XmlElement element);

		protected:
			String Get(IN LPCSTR key,IN String def=STRING_EMPTY);
			void Set(IN LPCSTR key,IN String val);
			int Get(IN LPCSTR key,IN int def);
			///<summary>获取布尔值类型,"true"或"yes"返回true,如果是不为0的数值返回true,其他返回false</summary>
			bool Get_bool(IN LPCSTR key,IN bool def=false);
			double Get_double(IN LPCSTR key,IN double def=0.0);
			void Set(IN LPCSTR key,IN int val);
			virtual void Clear(IN LPCSTR key=NULL);		
		};
}}}
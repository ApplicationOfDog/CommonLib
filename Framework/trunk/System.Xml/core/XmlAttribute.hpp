#pragma once

namespace System
{
	namespace Xml
	{
		class API_Xml XmlAttribute:public xml_attribute
		{
		public:
			XmlAttribute();
			XmlAttribute(xml_attribute attr);

			// Get attribute name/value, or "" if attribute is empty
			LPCTSTR Name(){ return name(); }
			LPCTSTR Value(){ return value(); }
			// Set attribute name/value (returns false if attribute is empty or there is not enough memory)
			bool Name(IN LPCTSTR val){ return set_name(val); }
			bool Value(IN LPCTSTR val){ return set_value(val); }
			// Set attribute value with type conversion (numbers are converted to strings, boolean is converted to "true"/"false")
			bool Value(IN int val){ return set_value(val); }
			bool Value(IN UINT val){ return set_value(val); }
			bool Value(IN double val){ return set_value(val); }
			bool Value(IN bool val){ return set_value(val); }
			

		public:
			// Get attribute value, or the default value if attribute is empty
			LPCTSTR Get(IN LPCTSTR def=NULL){ return as_string(def); }
			// Get attribute value as a number, or the default value if conversion did not succeed or attribute is empty
			int Get(IN int def)						{ return as_int(def); }
			UINT Get(IN UINT def)					{ return as_uint(def); }
			LONGLONG Get(IN LONGLONG def)			{ return as_llong(def); }
			LONGLONG GetLlong(IN LONGLONG def)		{ return as_llong(def); }
			ULONGLONG Get(IN ULONGLONG def)			{ return as_ullong(def); }
			ULONGLONG GetUllong(IN ULONGLONG def)   { return as_ullong(def); }
			bool Get(IN bool def)					{ return as_bool(def); }
			double Get(IN double def)				{ return as_double(def); }
			double GetDouble(IN double def)			{ return as_double(def); }
			float Get(IN float def)					{ return as_float(def); }
			float GetFloat(IN float def)			{ return as_float(def); }
		};
	}
}
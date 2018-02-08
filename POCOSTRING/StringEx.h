#pragma once 

#include "Poco/Foundation.h"
#include <string>
#include <vector>

namespace Poco {

class StringList;

/**
 * std::string wrapper/helper.
 *
 * Inspired from the class QString from the Qt library.
 *
 * @see QString
 * @see std::string
 * @see java.lang.String
 * @author Tanguy Krotoff
 */
class Foundation_API String //: public std::string
{
private:
	std::string str_;

public:
	/**/
	const char* c_str () const { return str_.c_str(); }
	int			size  () const { return str_.size(); }
	int			length() { return str_.length(); }

	int find(const char*		 other) { return str_.find(other); }
	int find(const std::string&  other) { return str_.find( other.c_str() ); }
	int find(const Poco::String& other) const { return str_.find( other.c_str() ); }

	int rfind(const char*		  other) { return str_.rfind(other); }
	int rfind(const std::string&  other) { return str_.rfind( other.c_str() ); }
	int rfind(const Poco::String& other) const { return str_.rfind( other.c_str() ); }	

	std::string& insert(int insert_at, const std::string & str) { return str_.insert(insert_at, str); }

	bool operator==(const char*			other) { return str_ == other; }
	bool operator==(const std::string&	other) { return str_ == other; }
	bool operator==(const Poco::String& other) { return str_ == other.c_str(); }
	
	bool operator!=(const char*			other) { return str_ == other; }
	bool operator!=(const std::string&	other) { return str_ == other; }
	bool operator!=(const Poco::String& other) { return str_ == other.c_str(); }
	
	std::string& operator+=(const char*			other) { return str_ += other; }
	std::string& operator+=(const std::string&	other) { return str_ += other; }
	std::string& operator+=(const Poco::String& other) { return str_ += other.c_str(); }
	//*/

	 static const char * null;

	 /**/
	 String() { }
	 String(const char * str) { str_ = str; }
	 String(const std::string & str) { str_ = str; }
	 operator const std::string&() { return str_; }
	 //*/
	 /*/
	 String() : std::string() { }
	 String(const char * str) : std::string(str) { }
	 String(const std::string & str) : std::string(str) { }
	 operator const std::string&() { return *this; }
	 //*/

	/**
	 * Converts this string to an int.
	 *
	 * @return the string converted to an int or 0 if failed to convert
	 */
	 int toInteger() const;

	 /**
	 * Converts this string to an int,this string is hex format number as "0x1a2b3c4e"
	 */
	 int toIntegerHex() const;

	/**
	 * Converts this string to a boolean.
	 *
	 * Detects strings: "true", "TRUE", "yes", "YES", "1"
	 *                  "false", "FALSE", "no", "NO", "0"
	 * Be very carefull when using this method, maybe it should throw an Exception.
	 *
	 * @return the string converted to a boolean (return false if failed to convert)
	 */
	 bool toBoolean() const;

	/**
	 * Converts all of the characters in this string to lower case.
	 *
	 * Example:
	 * <pre>
	 * String myString("QuteCom");
	 * str = myString.toLowerCase();	//str == "qutecom"
	 * </pre>
	 *
	 * @return the string converted to lowercase
	 */
	 std::string toLowerCase() const;

	/**
	 * Converts all of the characters in this string to upper case.
	 *
	 * Example:
	 * <pre>
	 * String myString("QuteCom");
	 * str = myString.toUpperCase();	//str == "QUTECOM"
	 * </pre>
	 *
	 * @return the string converted to uppercase
	 */
	 std::string toUpperCase() const;

	/**
	 * @param str the string to test
	 * @return true if String begins with str
	 */
	 bool beginsWith(const String & str) const;

	/**
	 * @param str the string to test
	 * @return true if String ends with str
	 */
	 bool endsWith(const String & str) const;

	/**
	 * Gets the number of occurences of the string str inside this string.
	 *
	 * @param str string to find
	 * @param caseSensitive the search is case sensitive; otherwise the search is case insensitive
	 * @return true if this list contains the specified element
	 */
	 bool contains(const std::string & str, bool caseSensitive = true) const;

	/**
	 * @see contains()
	 */
	 bool contains(char ch, bool caseSensitive = true) const;

	/**
	 * Replaces every occurence of the string 'before' with the string 'after'.
	 *
	 * @param before occurence to find
	 * @param after the string that will replace the string before
	 * @param caseSensitive the search is case sensitive; otherwise the search is case insensitive
	 */
	 void replace(const std::string & before, const std::string & after, bool caseSensitive = true);

	/**
	 * Replaces first occurence of the string 'before' with the string 'after'.
     *
	 * This version will only look for 'before' into the given range of the string. 
	 *
	 * @param index index of the first character of the substring to find in
	 * @param size size of the substring to find in
	 * @param before occurence to find
	 * @param after the string that will replace the string before
	 * @param caseSensitive the search is case sensitive; otherwise the search is case insensitive
	 */
	 void replaceInRange(unsigned index, unsigned size,	const std::string & before, const std::string & after, bool caseSensitive = true);

	/**
	 * Appends a string onto the end of this string.
	 *
	 * @param str string to append
	 * @return the new string
	 */
	 std::string & append(const std::string & str);

	/**
	 * Removes every occurrence of str in the string.
	 *
	 * @param str to remove in the string
	 */
	 void remove(const std::string & str);

	/**
	 * Gets a string from a number.
	 *
	 * @param number number to convert into a string
 	 * @param minLength minimal length of the string (i.e. fromNumber(15, 3) => "015")
	 * @return number converted to a string
	 */
	 static std::string fromNumber(int number, int minLength = 0);

	/**
	 * Gets a string from a boolean.
	 *
	 * @param boolean boolean to convert into a string
	 * @return boolean converted to a string
	 */
	 static std::string fromBoolean(bool boolean);

	/**
	 * Gets a string from an unsigned int.
	 *
	 * @param number unsigned int to convert into a string
	 * @return unsigned int converted to a string
	 */
	static std::string fromUnsignedInt(unsigned int number);

	/**
	 * Gets a string from an double.
	 *
	 * @param number double to convert into a string
	 * @return double converted to a string
	 */
	 static std::string fromDouble(double number);

	/**
	 * Gets a string from a long.
	 *
	 * @param number long to convert into a string
	 * @return long converted to a string
	 */
	 static std::string fromLong(long number);

	/**
	 * Gets a string from a long long.
	 *
	 * @param number long long to convert into a string
	 * @return long long converted to a string
	 */
	 static std::string fromLongLong(long long number);

	/**
	 * Gets a string from a unsigned long long.
	 *
	 * @param number unsigned long long to convert into a string
	 * @return unsigned long long converted to a string
	 */
	 static std::string fromUnsignedLongLong(unsigned long long number);

	/**
	 * URL-encodes a string.
	 *
	 * @param url the string to encode
	 * @return a string with all non-alphanumeric characters replaced by their
	 *         URL-encoded equivalent.
	 */
	 static std::string encodeUrl(const std::string & url);

	/**
	 * URL-decodes a string.
	 *
	 * @param url the URL-encoded string to decode
	 * @return a string with all URL-encoded sequences replaced by their
	 *         ASCII equivalent
	 */
	 static std::string decodeUrl(const std::string & url);


	 std::wstring toStdWString();
	 static std::string fromStdWString(const std::wstring& wstr);
	/**
	 * Tokenizes the string with a delimiter of your choice.
	 *
	 * Example:
	 * <pre>
	 * String str("four");
	 * StringList tokens = str.split("");	//tokens = "four"
	 * String str("four roses");
	 * StringList tokens = str.split(" ");	//tokens = "four", "roses"
	 * </pre>
	 *
	 * @param separator string delimiter
	 * @return tokens, strings created by splitting the input string
	 */
	 StringList split(const std::string & separator) const;

	 std::vector<std::string> split_array(const std::string& separator) const;
	/**
	 * Removes spaces at the beginning and the end of a string.
	 *
	 * @return a cleaned string
	 */
	 std::string trim();
};
}


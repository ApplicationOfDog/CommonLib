// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#pragma once

/** \brief JSON (JavaScript Object Notation).
 */
namespace System{
namespace Json {

	/** \brief Type of the value held by a Value object.
	 */
	enum ValueType {
	  nullValue = 0, ///< 'null' value
	  intValue,      ///< signed integer value
	  uintValue,     ///< unsigned integer value
	  realValue,     ///< double value
	  stringValue,   ///< UTF-8 string value
	  booleanValue,  ///< bool value
	  arrayValue,    ///< array value (ordered list)
	  objectValue    ///< object value (collection of name/value pairs).
	};

	enum CommentPlacement {
	  commentBefore = 0,      ///< a comment placed on the line before a value
	  commentAfterOnSameLine, ///< a comment just after a value on the same line
	  commentAfter, ///< a comment on the line after a value (only make sense for
	  /// root value)
	  numberOfCommentPlacement
	};

	//# ifdef JSON_USE_CPPTL
	//   typedef CppTL::AnyEnumerator<const char *> EnumMemberNames;
	//   typedef CppTL::AnyEnumerator<const Value &> EnumValues;
	//# endif

	/** \brief Lightweight wrapper to tag static string.
	 *
	 * Value constructor and objectValue member assignement takes advantage of the
	 * StaticString and avoid the cost of string duplication when storing the
	 * string or the member name.
	 *
	 * Example of usage:
	 * \code
	 * Json::Value aValue( StaticString("some text") );
	 * Json::Value object;
	 * static const StaticString code("code");
	 * object[code] = 1234;
	 * \endcode
	 */
	class JSON_API StaticString {
	public:
	  explicit StaticString(const char* czstring) : c_str_(czstring) {}

	  operator const char*() const { return c_str_; }

	  const char* c_str() const { return c_str_; }

	private:
	  const char* c_str_;
	};

	/** \brief Represents a <a HREF="http://www.json.org">JSON</a> value.
	 *
	 * This class is a discriminated union wrapper that can represents a:
	 * - signed integer [range: Value::minInt - Value::maxInt]
	 * - unsigned integer (range: 0 - Value::maxUInt)
	 * - double
	 * - UTF-8 string
	 * - boolean
	 * - 'null'
	 * - an ordered list of Value
	 * - collection of name/value pairs (javascript object)
	 *
	 * The type of the held value is represented by a #ValueType and
	 * can be obtained using type().
	 *
	 * Values of an #objectValue or #arrayValue can be accessed using operator[]()
	 * methods.
	 * Non-const methods will automatically create the a #nullValue element
	 * if it does not exist.
	 * The sequence of an #arrayValue will be automatically resized and initialized
	 * with #nullValue. resize() can be used to enlarge or truncate an #arrayValue.
	 *
	 * The get() methods can be used to obtain default value in the case the
	 * required element does not exist.
	 *
	 * It is possible to iterate over the list of a #objectValue values using
	 * the getMemberNames() method.
	 *
	 * \note #Value string-length fit in size_t, but keys must be < 2^30.
	 * (The reason is an implementation detail.) A #CharReader will raise an
	 * exception if a bound is exceeded to avoid security holes in your app,
	 * but the Value API does *not* check bounds. That is the responsibility
	 * of the caller.
	 */
	
	class ValueIteratorBase;
	class ValueIterator;
	class ValueConstIterator;

	class JSON_API Value 
	{
	  friend class ValueIteratorBase;
	public:
	  typedef std::vector<JSONCPP_STRING> Members;
	  typedef ValueIterator iterator;
	  typedef ValueConstIterator const_iterator;
	  typedef Json::UInt UInt;
	  typedef Json::Int Int;
	#if defined(JSON_HAS_INT64)
	  typedef Json::UInt64 UInt64;
	  typedef Json::Int64 Int64;
	#endif // defined(JSON_HAS_INT64)
	  typedef Json::LargestInt LargestInt;
	  typedef Json::LargestUInt LargestUInt;
	  typedef Json::ArrayIndex ArrayIndex;

	  static const Value& null;  ///< We regret this reference to a global instance; prefer the simpler Value().
	  static const Value& nullRef;  ///< just a kludge for binary-compatibility; same as null
	  static Value const& nullSingleton(); ///< Prefer this to null or nullRef.

	  /// Minimum signed integer value that can be stored in a Json::Value.
	  static const LargestInt minLargestInt;
	  /// Maximum signed integer value that can be stored in a Json::Value.
	  static const LargestInt maxLargestInt;
	  /// Maximum unsigned integer value that can be stored in a Json::Value.
	  static const LargestUInt maxLargestUInt;

	  /// Minimum signed int value that can be stored in a Json::Value.
	  static const Int minInt;
	  /// Maximum signed int value that can be stored in a Json::Value.
	  static const Int maxInt;
	  /// Maximum unsigned int value that can be stored in a Json::Value.
	  static const UInt maxUInt;

	#if defined(JSON_HAS_INT64)
	  /// Minimum signed 64 bits int value that can be stored in a Json::Value.
	  static const Int64 minInt64;
	  /// Maximum signed 64 bits int value that can be stored in a Json::Value.
	  static const Int64 maxInt64;
	  /// Maximum unsigned 64 bits int value that can be stored in a Json::Value.
	  static const UInt64 maxUInt64;
	#endif // defined(JSON_HAS_INT64)

	private:
	#ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION
	  class CZString {
	  public:
		enum DuplicationPolicy {
		  noDuplication = 0,
		  duplicate,
		  duplicateOnCopy
		};
		CZString(ArrayIndex index);
		CZString(char const* str, unsigned length, DuplicationPolicy allocate);
		CZString(CZString const& other);
	#if JSON_HAS_RVALUE_REFERENCES
		CZString(CZString&& other);
	#endif
		~CZString();
		CZString& operator=(CZString other);
		bool operator<(CZString const& other) const;
		bool operator==(CZString const& other) const;
		ArrayIndex index() const;
		//const char* c_str() const; ///< \deprecated
		char const* data() const;
		unsigned length() const;
		bool isStaticString() const;

	  private:
		void swap(CZString& other);

		struct StringStorage {
		  unsigned policy_: 2;
		  unsigned length_: 30; // 1GB max
		};

		char const* cstr_;  // actually, a prefixed string, unless policy is noDup
		union {
		  ArrayIndex index_;
		  StringStorage storage_;
		};
	  };

	public:
	#ifndef JSON_USE_CPPTL_SMALLMAP
	  typedef std::map<CZString, Value> ObjectValues;
	#else
	  typedef CppTL::SmallMap<CZString, Value> ObjectValues;
	#endif // ifndef JSON_USE_CPPTL_SMALLMAP
	#endif // ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION

	public:
	  /** \brief Create a default Value of the given type.

		This is a very useful constructor.
		To create an empty array, pass arrayValue.
		To create an empty object, pass objectValue.
		Another Value can then be set to this one by assignment.
	This is useful since clear() and resize() will not alter types.

		Examples:
	\code
	Json::Value null_value; // null
	Json::Value arr_value(Json::arrayValue); // []
	Json::Value obj_value(Json::objectValue); // {}
	\endcode
	  */
	  Value(ValueType type = nullValue);
	  Value(Int value);
	  Value(UInt value);
	#if defined(JSON_HAS_INT64)
	  Value(Int64 value);
	  Value(UInt64 value);
	#endif // if defined(JSON_HAS_INT64)
	  Value(double value);
	  Value(const char* value,IN bool isParse = false); ///< Copy til first 0. (NULL causes to seg-fault.)
	  Value(const char* begin, const char* end); ///< Copy all, incl zeroes.
	  /** \brief Constructs a value from a static string.

	   * Like other value string constructor but do not duplicate the string for
	   * internal storage. The given string must remain alive after the call to this
	   * constructor.
	   * \note This works only for null-terminated strings. (We cannot change the
	   *   size of this class, so we have nowhere to store the length,
	   *   which might be computed later for various operations.)
	   *
	   * Example of usage:
	   * \code
	   * static StaticString foo("some text");
	   * Json::Value aValue(foo);
	   * \endcode
	   */
	  Value(const StaticString& value);

#if !defined(IS_DLL_EXPORT) && !defined(IS_DLL_IMPORT)
	  ///<summary>将字符串转为json对象</summary>
	  ///<param name="value">json字符串
	  /// PS.中文等多字节字符会被转成成UTF-8字符存储于json对象中
	  ///</param>
	  ///<param name="isParse">true表示字符串是json字符串要进行解析,false:表示普通字符串</param>
	  Value(const JSONCPP_STRING& value,IN bool isParse = false); ///< Copy data() til size(). Embedded zeroes too.
#endif
	  ///<summary>将字符串转为json对象</summary>
	  ///<param name="value">json字符串
	  /// PS.中文等多字节字符会被转成成UTF-8字符存储于json对象中
	  ///</param>
	  ///<param name="isParse">true表示字符串是json字符串要进行解析,false:表示普通字符串</param>
	  Value(IN LPCTSTR text,IN bool isParse = false);

	#ifdef JSON_USE_CPPTL
	  Value(const CppTL::ConstString& value);
	#endif
	  Value(bool value);
	  /// Deep copy.
	  Value(const Value& other);
	#if JSON_HAS_RVALUE_REFERENCES
	  /// Move constructor
	  Value(Value&& other);
	#endif
	  ~Value();

	  /// Deep copy, then swap(other).
	  /// \note Over-write existing comments. To preserve comments, use #swapPayload().
	  Value& operator=(Value other);
	  /// Swap everything.
	  void swap(Value& other);
	  /// Swap values but leave comments and source offsets in place.
	  void swapPayload(Value& other);

	  ValueType type() const;

	  /// Compare payload only, not comments etc.
	  bool operator<(const Value& other) const;
	  bool operator<=(const Value& other) const;
	  bool operator>=(const Value& other) const;
	  bool operator>(const Value& other) const;
	  bool operator==(const Value& other) const;
	  bool operator!=(const Value& other) const;
	  int compare(const Value& other) const;

	  const char* asCString() const; ///< Embedded zeroes could cause you trouble!
	#if JSONCPP_USING_SECURE_MEMORY
	  unsigned getCStringLength() const; //Allows you to understand the length of the CString
	#endif
	  JSONCPP_STRING asString() const; ///< Embedded zeroes are possible.
	  /** Get raw char* of string-value.
	   *  \return false if !string. (Seg-fault if str or end are NULL.)
	   */
	  bool getString(
		  char const** begin, char const** end) const;
	#ifdef JSON_USE_CPPTL
	  CppTL::ConstString asConstString() const;
	#endif
	  Int asInt() const;
	  UInt asUInt() const;
	#if defined(JSON_HAS_INT64)
	  Int64 asInt64() const;
	  UInt64 asUInt64() const;
	#endif // if defined(JSON_HAS_INT64)
	  LargestInt asLargestInt() const;
	  LargestUInt asLargestUInt() const;
	  float asFloat() const;
	  double asDouble() const;
	  bool asBool() const;

	  bool isNull() const;
	  bool isBool() const;
	  bool isInt() const;
	  bool isInt64() const;
	  bool isUInt() const;
	  bool isUInt64() const;
	  ///<summary>是否为整数类型</summary>
	  ///<returns>true表示是,false表示非当前类型</returns>
	  bool isIntegral() const;
	  ///<summary>是否为浮点数类型</summary>
	  ///<returns>true表示是,false表示非当前类型</returns>
	  bool isDouble() const;
	  ///<summary>是否为数值类型,整数、浮点数都是数值类型</summary>
	  ///<returns>true表示是,false表示非当前类型</returns>
	  bool isNumeric() const;
	  bool isString() const;
	  bool isArray() const;
	  bool isObject() const;

	  bool isConvertibleTo(ValueType other) const;

	  /// Number of values in array or object
	  ArrayIndex size() const;

	  /// \brief Return true if empty array, empty object, or null;
	  /// otherwise, false.
	  bool empty() const;

	  /// Return isNull()
	  bool operator!() const;

	  /// Remove all object members and array elements.
	  /// \pre type() is arrayValue, objectValue, or nullValue
	  /// \post type() is unchanged
	  void clear();

	  /// Resize the array to size elements.
	  /// New elements are initialized to null.
	  /// May only be called on nullValue or arrayValue.
	  /// \pre type() is arrayValue or nullValue
	  /// \post type() is arrayValue
	  void resize(ArrayIndex size);

	  /// Access an array element (zero based index ).
	  /// If the array contains less than index element, then null value are
	  /// inserted
	  /// in the array so that its size is index+1.
	  /// (You may need to say 'value[0u]' to get your compiler to distinguish
	  ///  this from the operator[] which takes a string.)
	  Value& operator[](ArrayIndex index);

	  /// Access an array element (zero based index ).
	  /// If the array contains less than index element, then null value are
	  /// inserted
	  /// in the array so that its size is index+1.
	  /// (You may need to say 'value[0u]' to get your compiler to distinguish
	  ///  this from the operator[] which takes a string.)
	  Value& operator[](int index);

	  /// Access an array element (zero based index )
	  /// (You may need to say 'value[0u]' to get your compiler to distinguish
	  ///  this from the operator[] which takes a string.)
	  const Value& operator[](ArrayIndex index) const;

	  /// Access an array element (zero based index )
	  /// (You may need to say 'value[0u]' to get your compiler to distinguish
	  ///  this from the operator[] which takes a string.)
	  const Value& operator[](int index) const;

	  /// If the array contains at least index+1 elements, returns the element
	  /// value,
	  /// otherwise returns defaultValue.
	  Value get(ArrayIndex index, const Value& defaultValue) const;
	  /// Return true if index < size().
	  bool isValidIndex(ArrayIndex index) const;
	  /// \brief Append value to array at the end.
	  ///
	  /// Equivalent to jsonvalue[jsonvalue.size()] = value;
	  Value& append(const Value& value);

	  /// Access an object value by name, create a null member if it does not exist.
	  /// \note Because of our implementation, keys are limited to 2^30 -1 chars.
	  ///  Exceeding that will cause an exception.
	  Value& operator[](const char* key);
	  /// Access an object value by name, returns null if there is no member with
	  /// that name.
	  const Value& operator[](const char* key) const;
	  /// Access an object value by name, create a null member if it does not exist.
	  /// \param key may contain embedded nulls.
	  Value& operator[](const JSONCPP_STRING& key);
	  /// Access an object value by name, returns null if there is no member with
	  /// that name.
	  /// \param key may contain embedded nulls.
	  const Value& operator[](const JSONCPP_STRING& key) const;
	  /** \brief Access an object value by name, create a null member if it does not
	   exist.

	   * If the object has no entry for that name, then the member name used to store
	   * the new entry is not duplicated.
	   * Example of use:
	   * \code
	   * Json::Value object;
	   * static const StaticString code("code");
	   * object[code] = 1234;
	   * \endcode
	   */
	  Value& operator[](const StaticString& key);
	#ifdef JSON_USE_CPPTL
	  /// Access an object value by name, create a null member if it does not exist.
	  Value& operator[](const CppTL::ConstString& key);
	  /// Access an object value by name, returns null if there is no member with
	  /// that name.
	  const Value& operator[](const CppTL::ConstString& key) const;
	#endif
	  /// Return the member named key if it exist, defaultValue otherwise.
	  /// \note deep copy
	  Value get(const char* key, const Value& defaultValue) const;
	  /// Return the member named key if it exist, defaultValue otherwise.
	  /// \note deep copy
	  /// \note key may contain embedded nulls.
	  Value get(const char* begin, const char* end, const Value& defaultValue) const;
	  /// Return the member named key if it exist, defaultValue otherwise.
	  /// \note deep copy
	  /// \param key may contain embedded nulls.
	  Value get(const JSONCPP_STRING& key, const Value& defaultValue) const;
	#ifdef JSON_USE_CPPTL
	  /// Return the member named key if it exist, defaultValue otherwise.
	  /// \note deep copy
	  Value get(const CppTL::ConstString& key, const Value& defaultValue) const;
	#endif
	  /// Most general and efficient version of isMember()const, get()const,
	  /// and operator[]const
	  /// \note As stated elsewhere, behavior is undefined if (end-begin) >= 2^30
	  Value const* find(char const* begin, char const* end) const;
	  /// Most general and efficient version of object-mutators.
	  /// \note As stated elsewhere, behavior is undefined if (end-begin) >= 2^30
	  /// \return non-zero, but JSON_ASSERT if this is neither object nor nullValue.
	  Value const* demand(char const* begin, char const* end);
	  /// \brief Remove and return the named member.
	  ///
	  /// Do nothing if it did not exist.
	  /// \return the removed Value, or null.
	  /// \pre type() is objectValue or nullValue
	  /// \post type() is unchanged
	  /// \deprecated
	  Value removeMember(const char* key);
	  /// Same as removeMember(const char*)
	  /// \param key may contain embedded nulls.
	  /// \deprecated
	  Value removeMember(const JSONCPP_STRING& key);
	  /// Same as removeMember(const char* begin, const char* end, Value* removed),
	  /// but 'key' is null-terminated.
	  bool removeMember(const char* key, Value* removed);
	  /** \brief Remove the named map member.

		  Update 'removed' iff removed.
		  \param key may contain embedded nulls.
		  \return true iff removed (no exceptions)
	  */
	  bool removeMember(JSONCPP_STRING const& key, Value* removed);
	  /// Same as removeMember(JSONCPP_STRING const& key, Value* removed)
	  bool removeMember(const char* begin, const char* end, Value* removed);
	  /** \brief Remove the indexed array element.

		  O(n) expensive operations.
		  Update 'removed' iff removed.
		  \return true iff removed (no exceptions)
	  */
	  bool removeIndex(ArrayIndex i, Value* removed);

	  /// Return true if the object has a member named key.
	  /// \note 'key' must be null-terminated.
	  bool isMember(const char* key) const;
	  /// Return true if the object has a member named key.
	  /// \param key may contain embedded nulls.
	  bool isMember(const JSONCPP_STRING& key) const;
	  /// Same as isMember(JSONCPP_STRING const& key)const
	  bool isMember(const char* begin, const char* end) const;
	#ifdef JSON_USE_CPPTL
	  /// Return true if the object has a member named key.
	  bool isMember(const CppTL::ConstString& key) const;
	#endif

	  /// \brief Return a list of the member names.
	  ///
	  /// If null, return an empty list.
	  /// \pre type() is objectValue or nullValue
	  /// \post if type() was nullValue, it remains nullValue
	  Members getMemberNames() const;

	  //# ifdef JSON_USE_CPPTL
	  //      EnumMemberNames enumMemberNames() const;
	  //      EnumValues enumValues() const;
	  //# endif

	  /// \deprecated Always pass len.
	  JSONCPP_DEPRECATED("Use setComment(JSONCPP_STRING const&) instead.")
	  void setComment(const char* comment, CommentPlacement placement);
	  /// Comments must be //... or /* ... */
	  void setComment(const char* comment, size_t len, CommentPlacement placement);
	  /// Comments must be //... or /* ... */
	  void setComment(const JSONCPP_STRING& comment, CommentPlacement placement);
	  bool hasComment(CommentPlacement placement) const;
	  /// Include delimiters and embedded newlines.
	  JSONCPP_STRING getComment(CommentPlacement placement) const;

	  //JSONCPP_STRING toStyledString() const;

	  const_iterator begin() const;
	  const_iterator end() const;

	  iterator begin();
	  iterator end();

	  // Accessors for the [start, limit) range of bytes within the JSON text from
	  // which this value was parsed, if any.
	  void setOffsetStart(ptrdiff_t start);
	  void setOffsetLimit(ptrdiff_t limit);
	  ptrdiff_t getOffsetStart() const;
	  ptrdiff_t getOffsetLimit() const;


	  ///#region aweigh interface

	  ///<summary>将json字符串转为json对象</summary>
	  ///<param name="text">json字符串
	  /// PS.中文等多字节字符会被转成成UTF-8字符存储于json对象中,而对于anscii码字符则不会转换(保存原始值)
	  ///</param>
	  ///<param name="allowSingleQuote">
	  /// 是否允许单引号作为字符串或key标识
	  /// 例如:{ 'xx': 100,'yy':"string" } => 名称需要用单引号引用
	  ///</param>
	  ///<param name="allowNoneQuoteKey">
	  /// 是否允许无引号作为key标识
	  /// 例如:{ xx: 100,yy:"string" } => 名称不需要其他符号
	  ///</param>
	  ///<param name="pError">如果转换失败,则可通过该值返回错误信息</param>
	  ///<returns>转换是否成功</returns>
	  bool FromString(IN LPCSTR text,IN bool isSupportSingleQuote = false,IN bool isSupportNoneQuoteKey = false,OUT JSONCPP_STRING* pError=NULL);
#ifdef _UNICODE
	  bool FromString(IN LPCTSTR text,IN bool isSupportSingleQuote = false,IN bool isSupportNoneQuoteKey = false,OUT JSONCPP_STRING* pError=NULL);
#endif
	  
	  ///<summary>将json对象转为json字符串</summary>
	   ///<param name="encode">返回字符串的编码,最好保持与json源编码相同</param>
	  String ToString(IN bool isFormat = false,IN IO::Encoding encode = IO::EC_ANSI);
	  ///<returns>返回字符串是UTF-8编码的字符串</returns>
	  JSONCPP_STRING ToStringA(IN bool isFormat = false);

	  ///<summary>获取字符串值或成员的字符串值</summary>
	  ///<param name="key">
	  /// 1.NULL表示当前对象是字符串的JSON对象;
	  /// 2.成员名称字符串,例如GetStringA("DD")表示获取当前对象的DD成员值
	  /// 3.XPATH表达式则表示要获取的对象链
	  /// 例如:"{ 
	  ///		  AA:{ BB:{ CC:[1,2,3] } } 
	  ///         DD:"test"
	  ///  }"
	  /// 表达式".AA.BB.CC[1]"表示获取子节点CC数组中的第一个元素
	  ///  XPATH Syntax:
	  ///  "." => root node
	  ///  ".[n]" => elements at index 'n' of root node (an array value)
	  ///  ".name" => member named 'name' of root node (an object value)
	  ///  ".name1.name2.name3"
	  ///  ".[0][1][2].name1[3]"
	  ///  ".%" => member name is provided as parameter
	  ///  ".[%]" => index is provied as parameter
	  ///</param>
	  ///<param name="def">获取失败时返回的默认值</param>	  
	  ///<returns>返回ansi编码字符串</returns>
	  string GetStringA(IN LPCSTR key,IN LPCSTR def=NULL);
	  ///<param name="code">json中字符串的编码</param>
	  ///<param name="isConvertOtherType">如果对象不是当前类型是否进行强制转换
	  /// true:表示要强制转换.数值类型和布尔值可以进行转换,其他类型返回def
	  ///</param>
	  ///<returns>返回unicode编码字符串</returns>
	  String GetString(IN LPCSTR key,IN LPCTSTR def=NULL,IN IO::Encoding code = IO::EC_ANSI,IN bool isConvertOtherType=false);

	  ///<summary>获取布尔值或成员的布尔值</summary>
	  ///<param name="key">要获取的属性名称或对象链表达式,详细看GetStringA的Key参数</param>
	  ///<param name="isConvertOtherType">如果对象不是当前类型是否进行强制转换
	  /// true:表示要强制转换.字符串类型和数值类型可以进行转换,其他类型返回def
	  ///</param>
	  ///<returns>返回布尔值</returns>
	  bool GetBool(IN LPCSTR key,IN bool def=false,IN bool isConvertOtherType=false);

	  ///<summary>获取整数值或成员的整数值</summary>
	  ///<param name="key">要获取的属性名称或对象链表达式,详细看GetStringA的Key参数</param>
	  ///<param name="isConvertOtherType">如果对象不是当前类型是否进行强制转换
	  /// true:表示要强制转换.字符串类型和数值类型可以进行转换,其他类型返回def
	  ///</param>
	  ///<returns>返回节点值</returns>
	  INT32 GetInt32(IN LPCSTR key,IN INT32 def=0,IN bool isConvertOtherType=false);
	  UINT32 GetUInt32(IN LPCSTR key,IN UINT32 def=0,IN bool isConvertOtherType=false);

	  ///<param name="key">要获取的属性名称或对象链表达式,详细看GetStringA的Key参数</param>
	  INT64 GetInt64(IN LPCSTR key,IN INT64 def=0,IN bool isConvertOtherType=false);
	  UINT64 GetUInt64(IN LPCSTR key,IN UINT64 def=0,IN bool isConvertOtherType=false);

	  ///<param name="key">要获取的属性名称或对象链表达式,详细看GetStringA的Key参数</param>
	  double GetDouble(IN LPCSTR key,IN double def=0.0,IN bool isConvertOtherType=false);

	  ///<param name="key">要获取的属性名称或对象链表达式,详细看GetStringA的Key参数</param>
	  bool GetMember(IN LPCSTR key,OUT Value& member);

	  ///<summary>判断是否包含成员</summary>
	  bool HasMember(IN LPCSTR key){ return isMember(key); }
	  ///#endregion

	private:
	  void initBasic(ValueType type, bool allocated = false);

	  Value& resolveReference(const char* key);
	  Value& resolveReference(const char* key, const char* end);

	  Value* getMemberByXPath(IN LPCSTR xpath);

	  struct CommentInfo {
		CommentInfo();
		~CommentInfo();

		void setComment(const char* text, size_t len);

		char* comment_;
	  };

	  // struct MemberNamesTransform
	  //{
	  //   typedef const char *result_type;
	  //   const char *operator()( const CZString &name ) const
	  //   {
	  //      return name.c_str();
	  //   }
	  //};

	  union ValueHolder {
		LargestInt int_;
		LargestUInt uint_;
		double real_;
		bool bool_;
		char* string_;  // actually ptr to unsigned, followed by str, unless !allocated_
		ObjectValues* map_;
	  } value_;
	  ValueType type_ : 8;
	  unsigned int allocated_ : 1; // Notes: if declared as bool, bitfield is useless.
								   // If not allocated_, string_ must be null-terminated.
	  CommentInfo* comments_;

	  // [start, limit) byte offsets in the source JSON text from which this Value
	  // was extracted.
	  ptrdiff_t start_;
	  ptrdiff_t limit_;
	};

}} // namespace Json


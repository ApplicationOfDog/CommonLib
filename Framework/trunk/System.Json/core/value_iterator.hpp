#pragma once

namespace System{
	namespace Json{

	/** \brief base class for Value iterators.
	 *
	 */
	class JSON_API ValueIteratorBase {
	public:
	  typedef std::bidirectional_iterator_tag iterator_category;
	  typedef unsigned int size_t;
	  typedef int difference_type;
	  typedef ValueIteratorBase SelfType;

	  bool operator==(const SelfType& other) const { return isEqual(other); }

	  bool operator!=(const SelfType& other) const { return !isEqual(other); }

	  difference_type operator-(const SelfType& other) const {
		return other.computeDistance(*this);
	  }

	  /// Return either the index or the member name of the referenced value as a
	  /// Value.
	  Value key() const;

	  /// Return the index of the referenced Value, or -1 if it is not an arrayValue.
	  UInt index() const;

	  /// Return the member name of the referenced Value, or "" if it is not an
	  /// objectValue.
	  /// \note Avoid `c_str()` on result, as embedded zeroes are possible.
	  JSONCPP_STRING name() const;

	  /// Return the member name of the referenced Value. "" if it is not an
	  /// objectValue.
	  /// \deprecated This cannot be used for UTF-8 strings, since there can be embedded nulls.
	  JSONCPP_DEPRECATED("Use `key = name();` instead.")
	  char const* memberName() const;
	  /// Return the member name of the referenced Value, or NULL if it is not an
	  /// objectValue.
	  /// \note Better version than memberName(). Allows embedded nulls.
	  char const* memberName(char const** end) const;

	protected:
	  Value& deref() const;

	  void increment();

	  void decrement();

	  difference_type computeDistance(const SelfType& other) const;

	  bool isEqual(const SelfType& other) const;

	  void copy(const SelfType& other);

	private:
	  Value::ObjectValues::iterator current_;
	  // Indicates that iterator is for a null value.
	  bool isNull_;

	public:
	  // For some reason, BORLAND needs these at the end, rather
	  // than earlier. No idea why.
	  ValueIteratorBase();
	  explicit ValueIteratorBase(const Value::ObjectValues::iterator& current);
	};

	/** \brief const iterator for object and array value.
	 *
	 */
	class JSON_API ValueConstIterator : public ValueIteratorBase 
	{
	  friend class Value;

	public:
	  typedef const Value value_type;
	  //typedef unsigned int size_t;
	  //typedef int difference_type;
	  typedef const Value& reference;
	  typedef const Value* pointer;
	  typedef ValueConstIterator SelfType;

	  ValueConstIterator();
	  ValueConstIterator(ValueIterator const& other);

	private:
	/*! \internal Use by Value to create an iterator.
	 */
	  explicit ValueConstIterator(const Value::ObjectValues::iterator& current);
	public:
	  SelfType& operator=(const ValueIteratorBase& other);

	  SelfType operator++(int) {
		SelfType temp(*this);
		++*this;
		return temp;
	  }

	  SelfType operator--(int) {
		SelfType temp(*this);
		--*this;
		return temp;
	  }

	  SelfType& operator--() {
		decrement();
		return *this;
	  }

	  SelfType& operator++() {
		increment();
		return *this;
	  }

	  reference operator*() const { return deref(); }

	  pointer operator->() const { return &deref(); }
	};

	/** \brief Iterator for object and array value.
	 */
	class JSON_API ValueIterator : public ValueIteratorBase 
	{
	  friend class Value;

	public:
	  typedef Value value_type;
	  typedef unsigned int size_t;
	  typedef int difference_type;
	  typedef Value& reference;
	  typedef Value* pointer;
	  typedef ValueIterator SelfType;

	  ValueIterator();
	  explicit ValueIterator(const ValueConstIterator& other);
	  ValueIterator(const ValueIterator& other);

	private:
	/*! \internal Use by Value to create an iterator.
	 */
	  explicit ValueIterator(const Value::ObjectValues::iterator& current);
	public:
	  SelfType& operator=(const SelfType& other);

	  SelfType operator++(int) {
		SelfType temp(*this);
		++*this;
		return temp;
	  }

	  SelfType operator--(int) {
		SelfType temp(*this);
		--*this;
		return temp;
	  }

	  SelfType& operator--() {
		decrement();
		return *this;
	  }

	  SelfType& operator++() {
		increment();
		return *this;
	  }

	  reference operator*() const { return deref(); }

	  pointer operator->() const { return &deref(); }
	};

}}
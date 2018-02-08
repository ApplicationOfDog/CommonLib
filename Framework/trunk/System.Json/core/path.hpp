#pragma once

namespace System{
	namespace Json{

	/** \brief Experimental and untested: represents an element of the "path" to
	 * access a node.
	 */
	class JSON_API PathArgument {
	public:
	  friend class Path;

	  PathArgument();
	  PathArgument(ArrayIndex index);
	  PathArgument(const char* key);
	  PathArgument(const JSONCPP_STRING& key);

	private:
	  enum Kind {
		kindNone = 0,
		kindIndex,
		kindKey
	  };
	  JSONCPP_STRING key_;
	  ArrayIndex index_;
	  Kind kind_;
	};

	
	
	/** \brief Experimental and untested: represents a "path" to access a node.
	 *
	 * Syntax:
	 * - "." => root node
	 * - ".[n]" => elements at index 'n' of root node (an array value)
	 * - ".name" => member named 'name' of root node (an object value)
	 * - ".name1.name2.name3"
	 * - ".[0][1][2].name1[3]"
	 * - ".%" => member name is provided as parameter
	 * - ".[%]" => index is provied as parameter
	 */
	class JSON_API Path {
	public:
	  Path(const JSONCPP_STRING& path,
		   const PathArgument& a1 = PathArgument(),
		   const PathArgument& a2 = PathArgument(),
		   const PathArgument& a3 = PathArgument(),
		   const PathArgument& a4 = PathArgument(),
		   const PathArgument& a5 = PathArgument());

	  const Value& resolve(const Value& root) const;
	  Value resolve(const Value& root, const Value& defaultValue) const;
	  /// Creates the "path" to access the specified node and returns a reference on
	  /// the node.
	  //Value& make(Value& root) const;
	  Value* make(IN Value& root,OUT int* error=NULL);

	private:
	  typedef std::vector<const PathArgument*> InArgs;
	  typedef std::vector<PathArgument> Args;

	  void makePath(const JSONCPP_STRING& path, const InArgs& in);
	  void addPathInArg(const JSONCPP_STRING& path,
						const InArgs& in,
						InArgs::const_iterator& itInArg,
						PathArgument::Kind kind);
	  void invalidPath(const JSONCPP_STRING& path, int location);

	  Args args_;
	};


}}
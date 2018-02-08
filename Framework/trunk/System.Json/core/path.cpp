#include "stdafx.h"
#include "path.hpp"

namespace System{
	namespace Json{

		// class PathArgument
		// //////////////////////////////////////////////////////////////////

		PathArgument::PathArgument() : key_(), index_(), kind_(kindNone) {}

		PathArgument::PathArgument(ArrayIndex index)
			: key_(), index_(index), kind_(kindIndex) {}

		PathArgument::PathArgument(const char* key)
			: key_(key), index_(), kind_(kindKey) {}

		PathArgument::PathArgument(const JSONCPP_STRING& key)
			: key_(key.c_str()), index_(), kind_(kindKey) {}


		// class Path
		// //////////////////////////////////////////////////////////////////

		Path::Path(const JSONCPP_STRING& path,
			const PathArgument& a1,
			const PathArgument& a2,
			const PathArgument& a3,
			const PathArgument& a4,
			const PathArgument& a5) {
				InArgs in;
				in.push_back(&a1);
				in.push_back(&a2);
				in.push_back(&a3);
				in.push_back(&a4);
				in.push_back(&a5);
				makePath(path, in);
		}

		void Path::makePath(const JSONCPP_STRING& path, const InArgs& in) {
			const char* current = path.c_str();
			const char* end = current + path.length();
			InArgs::const_iterator itInArg = in.begin();
			while (current != end) {
				if (*current == '[') {
					++current;
					if (*current == '%')
						addPathInArg(path, in, itInArg, PathArgument::kindIndex);
					else {
						ArrayIndex index = 0;
						for (; current != end && *current >= '0' && *current <= '9'; ++current)
							index = index * 10 + ArrayIndex(*current - '0');
						args_.push_back(index);
					}
					if (current == end || *current++ != ']')
						invalidPath(path, int(current - path.c_str()));
				} else if (*current == '%') {
					addPathInArg(path, in, itInArg, PathArgument::kindKey);
					++current;
				} else if (*current == '.') {
					++current;
				} else {
					const char* beginName = current;
					while (current != end && !strchr("[.", *current))
						++current;
					args_.push_back(JSONCPP_STRING(beginName, current));
				}
			}
		}

		void Path::addPathInArg(const JSONCPP_STRING& /*path*/,
			const InArgs& in,
			InArgs::const_iterator& itInArg,
			PathArgument::Kind kind) {
				if (itInArg == in.end()) {
					// Error: missing argument %d
				} else if ((*itInArg)->kind_ != kind) {
					// Error: bad argument type
				} else {
					args_.push_back(**itInArg);
				}
		}

		void Path::invalidPath(const JSONCPP_STRING& /*path*/, int /*location*/) {
			// Error: invalid path.
		}

		const Value& Path::resolve(const Value& root) const {
			const Value* node = &root;
			for (Args::const_iterator it = args_.begin(); it != args_.end(); ++it) {
				const PathArgument& arg = *it;
				if (arg.kind_ == PathArgument::kindIndex) {
					if (!node->isArray() || !node->isValidIndex(arg.index_)) {
						// Error: unable to resolve path (array value expected at position...
					}
					node = &((*node)[arg.index_]);
				} else if (arg.kind_ == PathArgument::kindKey) {
					if (!node->isObject()) {
						// Error: unable to resolve path (object value expected at position...)
					}
					node = &((*node)[arg.key_]);
					if (node == &Value::nullSingleton()) {
						// Error: unable to resolve path (object has no member named '' at
						// position...)
					}
				}
			}
			return *node;
		}

		Value Path::resolve(const Value& root, const Value& defaultValue) const {
			const Value* node = &root;
			for (Args::const_iterator it = args_.begin(); it != args_.end(); ++it) {
				const PathArgument& arg = *it;
				if (arg.kind_ == PathArgument::kindIndex) {
					if (!node->isArray() || !node->isValidIndex(arg.index_))
						return defaultValue;
					node = &((*node)[arg.index_]);
				} else if (arg.kind_ == PathArgument::kindKey) {
					if (!node->isObject())
						return defaultValue;
					node = &((*node)[arg.key_]);
					if (node == &Value::nullSingleton())
						return defaultValue;
				}
			}
			return *node;
		}

		//Value& Path::make(Value& root) const {
		//	Value* node = &root;
		//	for (Args::const_iterator it = args_.begin(); it != args_.end(); ++it) {
		//		const PathArgument& arg = *it;
		//		if (arg.kind_ == PathArgument::kindIndex) {
		//			if (!node->IsArray()) {
		//				// Error: node is not an array at position ...
		//			}
		//			node = &((*node)[arg.index_]);
		//		} else if (arg.kind_ == PathArgument::kindKey) {
		//			if (!node->IsObject()) {
		//				// Error: node is not an object at position...
		//			}
		//			node = &((*node)[arg.key_]);
		//		}
		//	}
		//	return *node;
		//}
		Value* Path::make(IN Value& root,OUT int* error)
		{
			int errorCode = 0;
			Value* node = &root;
			for (Args::const_iterator it = args_.begin(); it != args_.end(); ++it) 
			{
				const PathArgument& arg = *it;
				if (arg.kind_ == PathArgument::kindIndex) 
				{
					if (!node->isArray()) {
						// Error: node is not an array at position ...
						errorCode = -1;
						goto _BACK_;
					}
					node = &((*node)[arg.index_]);
				} 
				else if (arg.kind_ == PathArgument::kindKey) 
				{
					if (!node->isObject()) {
						// Error: node is not an object at position...
						errorCode = -2;
						goto _BACK_;
					}
					node = &((*node)[arg.key_]);
				}
			}
_BACK_:
			if(error!=NULL) *error = errorCode;
			return errorCode==0 ? node :NULL;
		}


}}
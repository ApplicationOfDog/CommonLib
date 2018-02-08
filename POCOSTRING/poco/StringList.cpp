#include "Poco/StringList.h"
#include "Poco/StringEx.h"

using namespace std;

namespace Poco {

template class std::list<std::string>;


StringList StringList::null;

StringList::StringList(const std::list<std::string> & strList) {
	for (std::list<string>::const_iterator it = strList.begin();
		it != strList.end(); ++it) {

		push_back(*it);
	}
}

StringList::StringList() {
}

StringList::~StringList() {
}

StringList::operator std::list<std::string>() {
	std::list<std::string> strList;
	for (unsigned i = 0; i < size(); i++) {
		strList.push_back((*this)[i]);
	}
	return strList;
}

std::string StringList::operator[](unsigned i) const {
	if (i >= size()) {
		return String::null;
	}

	return List<std::string>::operator[](i);
}

std::string& StringList::operator[]( unsigned i )
{
	return List<std::string>::operator [](i);
}


void StringList::operator+=(const std::string & str) {
	List<std::string>::operator+=(str);
}

void StringList::operator+=(const StringList & strList) {
	for (unsigned i = 0; i < strList.size(); i++) {
		(*this) += strList[i];
	}
}

unsigned StringList::contains(const std::string & str, bool caseSensitive) const {
	unsigned result = 0;
	for (unsigned i = 0; i < size(); i++) {
		String tmp1 = str;
		String tmp2 = (*this)[i];
		if (!caseSensitive) {
			tmp1 = tmp1.toLowerCase();
			tmp2 = tmp2.toLowerCase();
		}

		if (tmp1 == tmp2) {
			++result;
		}
	}

	return result;
}

void StringList::sort(SortingOrder order) {
	switch(order) 
	{
	case Ascendant:
		std::sort(begin(), end());
		break;

	case Descendant:
		std::sort(begin(), end(), StringCompareDescendant());
		break;

	default:
		//LOG_FATAL("unknown sorting order=" + String::fromNumber(order));
		break;
	}
}

std::string StringList::join(const std::string & separator) const {
	std::string joinedString;
	for (unsigned i = 0; i < size(); i++) {
		//Last token
		if (i == (size() - 1)) {
			joinedString += (*this)[i];
		}
		else {
			joinedString += (*this)[i] + separator;
		}
	}
	return joinedString;
}

void StringList::removeDuplicatedStrings() {
	for (unsigned i = 0; i < size(); i++) {
		std::string tmp = (*this)[i];
		for (unsigned j = 0; j < size(); j++) {
			if (tmp == (*this)[j] && i != j) {
				remove(tmp);

				//Restarts the entire loop
				i = 0;
				break;
			}
		}
	}
}

std::string StringList::toString(const std::string & separator) const {
	std::string result;

	for (const_iterator it = begin(); it != end(); ++it) {
		if (it != begin()) {
			result += separator;
		}

		result += *it;
	}

	return result;
}
}


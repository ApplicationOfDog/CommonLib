#pragma once
#include "poco/NonCopyable.h"
namespace Poco
{
class Interface : NonCopyable {
public:

	virtual ~Interface() { }
};
}

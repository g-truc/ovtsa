#include "object_factory.hpp"

object* object_factory::create()
{
	object* Object = new object;
	this->Objects.push_back(Object);
	return Object;
}

#include "object_factory.hpp"

object * objectFactory::create()
{
	object * Object = new object;
	this->Objects.push_back(Object);
	return Object;
}

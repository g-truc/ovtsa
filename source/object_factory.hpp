#ifndef OBJECT_FACTORY_INCLUDED
#define OBJECT_FACTORY_INCLUDED

#include "object.hpp"
#include "singleton.hpp"
#include "factory.hpp"

class objectFactory : 
	public singleton<objectFactory>,
	public factory<object>
{
	friend class singleton<objectFactory>;

public:
	object * create();
};

#endif //OBJECT_FACTORY_INCLUDED

#pragma once

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


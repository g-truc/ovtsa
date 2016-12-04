#pragma once

#include "object.hpp"
#include "singleton.hpp"
#include "factory.hpp"

class object_factory : public singleton<object_factory>, public factory<object>
{
	friend class singleton<object_factory>;

public:
	object* create();
};


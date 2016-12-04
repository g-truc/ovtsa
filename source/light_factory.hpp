#pragma once

#include "singleton.hpp"
#include "factory.hpp"
#include "light.hpp"

class light_factory : public singleton<light_factory>, public factory<light>
{
public:
	light* create(light::type const& Type);
};

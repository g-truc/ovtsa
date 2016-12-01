#pragma once

#include "singleton.hpp"
#include "factory.hpp"
#include "light.hpp"

class lightFactory :
	public singleton<lightFactory>,
	public factory<light>
{
public:
	light * create(light::type const & Type);
};

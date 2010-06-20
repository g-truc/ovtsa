#ifndef LIGHT_FACTORY_INCLUDED
#define LIGHT_FACTORY_INCLUDED

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

#endif //LIGHT_FACTORY_INCLUDED

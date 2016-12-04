#include "light_factory.hpp"
#include "light_spot.hpp"
#include "light_point.hpp"
#include "light_directional.hpp"

light* light_factory::create(light::type const& Type)
{
	light* Light = 0;
	switch(Type)
	{
	default:
		break;
	case light::POINT:
		Light = new point;
		break;
	case light::SPOT:
		Light = new spot;
		break;
	case light::DIRECTIONAL:
		Light = new directional;
		break;
	}

	if(!Light)
		return 0;

	this->Objects.push_back(Light);
	return Light;
}

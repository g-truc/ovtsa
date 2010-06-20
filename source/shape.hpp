#ifndef SHAPE_INCLUDED
#define SHAPE_INCLUDED

#include "ray.hpp"
#include "intersection.hpp"

class shape
{
public:
	enum type
	{
		CONE,         // Not available
		CUBE,         // Not available
		PLANE,
		TRIANGLE,
		SPHERE,
		CYLINDER,     // Not available
		TORUS         // Not available
	};
	
	virtual glm::vec3 computeNormal(
		glm::vec3 const & Position, 
		glm::vec3 const & RayDirection) const = 0;

    virtual bool intersect(
		ray const & Ray, 
		intersection & Intersection) const = 0;
};

#endif //SHAPE_INCLUDED

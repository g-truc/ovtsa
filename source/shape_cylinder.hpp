#ifndef SHAPE_CYLINDER_INCLUDED
#define SHAPE_CYLINDER_INCLUDED

#include "shape.hpp"

class cylinder : public shape
{
public:
	virtual glm::vec3 computeNormal(
		glm::vec3 const & Position, 
		glm::vec3 const & RayDirection) const;

	virtual bool intersect(
		ray const & Ray, 
		intersection & Intersection) const;
};

#endif //SHAPE_CYLINDER_INCLUDED

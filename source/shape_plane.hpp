#ifndef SHAPE_PLANE_INCLUDED
#define SHAPE_PLANE_INCLUDED

#include "shape.hpp"

class plane : public shape
{
public:
    virtual glm::vec3 computeNormal(
		glm::vec3 const & Position, 
		glm::vec3 const & RayDirection) const;

    virtual bool intersect(
		ray const & Ray, 
		intersection & Intersection) const;
};

#endif //SHAPE_PLANE_INCLUDED

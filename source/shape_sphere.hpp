#ifndef SHAPE_SPHERE_INCLUDED
#define SHAPE_SPHERE_INCLUDED

#include "shape.hpp"

class sphere : public shape
{
public:
    virtual glm::vec3 computeNormal(
		glm::vec3 const & Position, 
		glm::vec3 const & RayDirection) const;

    virtual bool intersect(
		ray const & Ray, 
		intersection & Intersection) const;
};

#endif //SHAPE_SPHERE_INCLUDED

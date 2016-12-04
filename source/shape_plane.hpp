#pragma once

#include "shape.hpp"

class plane : public shape
{
public:
	virtual glm::vec3 compute_normal(glm::vec3 const& Position, glm::vec3 const& RayDirection) const;
	virtual bool intersect(ray const& Ray, intersection& Intersection) const;
};


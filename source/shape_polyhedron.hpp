#pragma once

#include "shape.hpp"
#include "util.hpp"
#include <array>

class triangle : public shape
{
private:
	glm::vec3 A, B, C;
	glm::vec3 m_Normal;
	int DominantAxis;

public:
	triangle() : 
		A(glm::vec3( 0, 0, 1)), 
		B(glm::vec3(-1, 0, 0)),
		C(glm::vec3( 1, 0, 0))
	{
		m_Normal = -glm::cross(glm::normalize(B - A), glm::normalize(C - A));
	}

	void set_positions(std::array<glm::vec3, 3> const& Data);

	virtual glm::vec3 compute_normal(glm::vec3 const& Position, glm::vec3 const& RayDirection) const;
	virtual bool intersect(ray const& Ray, intersection& Intersection) const;
};


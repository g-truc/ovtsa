#ifndef SHAPE_POLYHEDRON_INCLUDED
#define SHAPE_POLYHEDRON_INCLUDED

#include "shape.hpp"
#include "util.hpp"

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

	void setPositions(
		std::vector<glm::vec3> const & Data);

	virtual glm::vec3 computeNormal(
		glm::vec3 const & Position, 
		glm::vec3 const & RayDirection) const;

	virtual bool intersect(
		ray const & Ray, 
		intersection & Intersection) const;
};

#endif //SHAPE_POLYHEDRON_INCLUDED

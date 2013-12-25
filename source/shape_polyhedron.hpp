#ifndef SHAPE_POLYHEDRON_INCLUDED
#define SHAPE_POLYHEDRON_INCLUDED

#include "shape.hpp"
#include <glm/vec3.hpp>

class triangle : public shape
{
private:
	glm::vec3 A, B, C;
	glm::vec3 m_Normal;
	int DominantAxis;

public:
	triangle();

	void setPositions(
		glm::vec3 const & A,
		glm::vec3 const & B,
		glm::vec3 const & C);

	virtual glm::vec3 computeNormal(
		glm::vec3 const & Position, 
		glm::vec3 const & RayDirection) const;

	virtual bool intersect(
		ray const & Ray, 
		intersection & Intersection) const;
};

#endif //SHAPE_POLYHEDRON_INCLUDED

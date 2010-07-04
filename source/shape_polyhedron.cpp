#include "shape_polyhedron.hpp"
#include "main.hpp"
#include "transform.hpp"
#include "material.hpp"

glm::vec3 triangle::computeNormal
(
	glm::vec3 const & Position, 
	glm::vec3 const & RayDirection
) const
{
	if (glm::dot(Position, RayDirection) < 0)
        return glm::vec3(Position.x, Position.y, Position.z);
    else
        return glm::vec3(-Position.x, -Position.y, -Position.z);
}

bool triangle::intersect
(
	ray const & Ray, 
	intersection & Intersection
) const
{
    glm::vec3 w = Ray.getPosition() - A;
    glm::vec3 u = -(B - A);
    glm::vec3 v = -(C - A);
    float D = glm::dot(glm::cross(u, v), Ray.getDirection());
    float a = -glm::dot(glm::cross(w, v), Ray.getDirection()) / D;
    float b = -glm::dot(glm::cross(u, w), Ray.getDirection()) / D;
    float t = glm::dot(glm::cross(u, v), w) / D;

    if(a > 0.0f && b > 0.0f && a + b < 1.0)
        return true;
    return false;
}

void triangle::setPositions
(
	std::vector<glm::vec3> const & Data
)
{
    A = Data[0];
    B = Data[1];
    C = Data[2];
}

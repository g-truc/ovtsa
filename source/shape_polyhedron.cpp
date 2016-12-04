#include "shape_polyhedron.hpp"
#include "main.hpp"
#include "transform.hpp"
#include "material.hpp"

glm::vec3 triangle::compute_normal(glm::vec3 const& Position, glm::vec3 const& RayDirection) const
{
	if (glm::dot(Position, RayDirection) < 0)
		return glm::vec3(Position.x, Position.y, Position.z);
	else
		return glm::vec3(-Position.x, -Position.y, -Position.z);
}

bool triangle::intersect(ray const& Ray, intersection& Intersection) const
{
	glm::vec3 w = Ray.get_position() - A;
	glm::vec3 u = -(B - A);
	glm::vec3 v = -(C - A);
	float D = glm::dot(glm::cross(u, v), Ray.get_direction());
	float a = -glm::dot(glm::cross(w, v), Ray.get_direction()) / D;
	float b = -glm::dot(glm::cross(u, w), Ray.get_direction()) / D;
	float t = glm::dot(glm::cross(u, v), w) / D;

	if(a > 0.0f && b > 0.0f && a + b < 1.0)
		return true;
	return false;
}

void triangle::set_positions(std::array<glm::vec3, 3> const& Data)
{
	A = Data[0];
	B = Data[1];
	C = Data[2];
}

#include "shape_sphere.hpp"
#include "main.hpp"
#include "transform.hpp"
#include "material.hpp"
#include "util.hpp"

glm::vec3 sphere::compute_normal(const glm::vec3& Position, const glm::vec3& RayDirection) const
{
	return glm::mix(-Position, Position, glm::dot(Position, RayDirection) < 0);
}

bool sphere::intersect(ray const& Ray, intersection& Intersection) const
{
	bool bHit = false;

	float b = glm::dot(Ray.get_position(), Ray.get_direction());
	float c = glm::dot(Ray.get_position(), Ray.get_position()) - 1.0f; // 1.0f => Radius
	float d = b * b - c;

	if(d > glm::epsilon<float>())
	{
		float e = glm::sqrt(d);
		float x1 = -b - e;
		float x2 = -b + e;
		if(x1 > glm::epsilon<float>())
		{
			Intersection.setLocalPosition(Ray.get_position() + Ray.get_direction() * x1);
			bHit = true;
		}
		else if(x2 > glm::epsilon<float>())
		{
			Intersection.setLocalPosition(Ray.get_position() + Ray.get_direction() * x2);
			bHit = true;
		}
	}

	return bHit;
}

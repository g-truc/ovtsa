#include "shape_plane.hpp"
#include "util.hpp"

glm::vec3 plane::compute_normal(glm::vec3 const& Position, glm::vec3 const& RayDirection) const
{
	return glm::mix(glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, 0.0f, 1.0f), RayDirection.z < 0.0f);
}

bool plane::intersect(ray const& Ray, intersection& Intersection) const
{
	bool hit = false;

	if(glm::abs(Ray.get_direction().z) > 0.0f)
	{
		float t = -Ray.get_position().z / Ray.get_direction().z;
		if(t > glm::epsilon<float>() * 100.f)
		{
			Intersection.setLocalPosition(Ray.get_position() + Ray.get_direction() * t);
			hit = true;
		}
	}
	return hit;
}


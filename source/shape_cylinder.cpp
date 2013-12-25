#include "shape_cylinder.hpp"
#include "main.hpp"
#include "transform.hpp"
#include <glm/geometric.hpp>

glm::vec3 cylinder::computeNormal
(
	glm::vec3 const & Position, 
	glm::vec3 const & RayDirection
) const
{
	if(glm::dot(Position, RayDirection) < 0)
		return glm::vec3(0, Position.y, Position.z);
	else
		return glm::vec3(0, -Position.y, -Position.z);
}

bool cylinder::intersect
(
	ray const & Ray, 
	intersection & Intersection
) const
{
	bool bHit = false;

//    glm::vec3 L = Ray.Origin() - ;
/*
	float z2 = Ray.Direction().z * Ray.Direction().z;
	float d = glm::vec3(Ray.Direction().y,-Ray.Direction().x, 0.0f) * Ray.Origin().z;
	float delta = 1.0f * 1.0f * Ray.Direction().z * Ray.Direction().z - d * d;
	if(delta < 0)
		return bHit;
	float l = Ray.Direction().z * Ray.Origin().z;
	float a = glm::dot(Ray.Origin(), Ray.Origin());
	if(l > 0 && (a * a > 1.0f * 1.0f))
	{
		float t1 = (-l - delta) / z2;
		float t2 = (-l + delta) / z2;
		if(t1 > EPSILON)
		{
			Intersection.LocalPosition() = Ray.Origin() + Ray.Direction() * t1;
			bHit = true;
		}
		else if(t2 > EPSILON)
		{
			Intersection.LocalPosition() = Ray.Origin() + Ray.Direction() * t2;
			bHit = true;
		}
	}
*/
/*
	float a = Ray.Direction().y * Ray.Direction().y + Ray.Direction().z * Ray.Direction().z;
	float b = Ray.Direction().y * Ray.Origin().y + Ray.Direction().x * Ray.Origin().x;
	float c = Ray.Origin().y * Ray.Origin().y + Ray.Origin().z * Ray.Origin().z - 1.0f;
	float delta = b * b - 4 * a * c;

	if(delta > EPSILON)
	{
		float e = sqrt(delta);
		float x1 = (-b - e) / (2.0f * a);
		float x2 = (-b + e) / (2.0f * a);
		if(x1 > EPSILON)
		{
			Intersection.LocalPosition() = Ray.Origin() + Ray.Direction() * x1;
			bHit = true;
		}
		else if(x2 > EPSILON)
		{
			Intersection.LocalPosition() = Ray.Origin() + Ray.Direction() * x2;
			bHit = true;
		}
	}
*/
	return bHit;
}

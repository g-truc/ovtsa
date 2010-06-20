#include "shape_plane.hpp"
#include "util.hpp"

glm::vec3 plane::computeNormal
(
	glm::vec3 const & Position, 
	glm::vec3 const & RayDirection
) const
{
    if (RayDirection.z < 0.0f)
        return glm::vec3(0.0f, 0.0f, 1.0f);
    else
        return glm::vec3(0.0f, 0.0f,-1.0f);
}

bool plane::intersect
(
	ray const & Ray, 
	intersection & Intersection
) const
{
    bool hit = false;

	if(glm::abs(Ray.getDirection().z) > 0.0f)
    {
        float t = -Ray.getPosition().z / Ray.getDirection().z;
        if (t > EPSILON)
        {
            Intersection.setLocalPosition(Ray.getPosition() + Ray.getDirection() * t);
            hit = true;
        }
    }
    return hit;
}


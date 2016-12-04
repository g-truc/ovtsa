#include "light.hpp"
#include "ray.hpp"
#include "object_factory.hpp"

light::light()
	: Inaccuracy(0.0f)
	, RayNumber(1)
{}

bool light::shadow
(
	glm::vec3 const& GlobalIntersection,
	glm::vec3 const& GlobalLightPosition,
	glm::vec3 const& GlobalLightDirection
) const
{
	float Distance = glm::distance(GlobalIntersection, GlobalLightPosition);
	ray LocalLightRay;
	intersection LocalIntersection;

	bool Shadow = false;
	object_factory& ObjectFactory = object_factory::instance();
	for(object_factory::size_type i = 0, n = ObjectFactory.size(); i < n; i++)
	{
		glm::vec3 Origin(ObjectFactory[i]->getTransform()->computeInverse(glm::vec4(GlobalIntersection, 1.0f)));
		glm::vec3 Direction = glm::normalize(glm::vec3(ObjectFactory[i]->getTransform()->computeInverse(glm::vec4(GlobalLightDirection, 0.0f))));
		LocalLightRay.set_position(Origin);
		LocalLightRay.set_direction(Direction);
		if(ObjectFactory[i]->getShape()->intersect(LocalLightRay, LocalIntersection))
		{
			glm::vec3 Position(ObjectFactory[i]->getTransform()->computeMatrix(glm::vec4(LocalIntersection.getLocalPosition(), 1.0f)));
			LocalIntersection.setGlobalPosition(Position);
			if(glm::distance(GlobalIntersection, LocalIntersection.getGlobalPosition()) < Distance)
				Shadow = true;
		}
	}

	return Shadow;
}

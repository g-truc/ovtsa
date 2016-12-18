#include "light.hpp"
#include "ray.hpp"
#include "object_factory.hpp"

light::light()
	: Inaccuracy(0.0f)
	, RayCount(1)
{}

bool light::shadow(glm::vec3 const& GlobalIntersection, glm::vec3 const& GlobalLightPosition, glm::vec3 const& GlobalLightDirection) const
{
	float Distance = glm::distance(GlobalIntersection, GlobalLightPosition);
	ray LocalLightRay;
	intersection LocalIntersection;

	bool Shadow = false;
	object_factory& ObjectFactory = object_factory::instance();
	for(object_factory::size_type i = 0, n = ObjectFactory.size(); i < n; i++)
	{
		glm::vec3 Origin(ObjectFactory[i]->get_transform()->compute_inverse(glm::vec4(GlobalIntersection, 1.0f)));
		glm::vec3 Direction = glm::normalize(glm::vec3(ObjectFactory[i]->get_transform()->compute_inverse(glm::vec4(GlobalLightDirection, 0.0f))));
		LocalLightRay.set_position(Origin);
		LocalLightRay.set_direction(Direction);
		if(ObjectFactory[i]->get_shape()->intersect(LocalLightRay, LocalIntersection))
		{
			glm::vec3 const& Position = ObjectFactory[i]->get_transform()->compute_transform(glm::vec4(LocalIntersection.get_local_position(), 1.0f));
			LocalIntersection.set_global_position(Position);
			if(glm::distance(GlobalIntersection, LocalIntersection.get_global_position()) < Distance)
				Shadow = true;
		}
	}

	return Shadow;
}

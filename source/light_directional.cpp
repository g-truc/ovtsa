#include "light_directional.hpp"
#include "light_factory.hpp"
#include "main.hpp"
#include "util.hpp"

directional * directional::create()
{
	return static_cast<directional*>(light_factory::instance().create(DIRECTIONAL));
}

directional::directional()
{}

directional::~directional()
{}

glm::vec3 directional::shade(intersection const& Intersection, material const& Material, glm::vec3 const& View) const
{
	glm::vec3 Color(0.0f);

	glm::vec3 Position = -m_Direction * 100000.f;
	glm::vec3 LightVector = glm::normalize(Position - Intersection.get_global_position()/* + glm::sphericalRand(this->Inaccuracy)*/);

	if(!this->shadow(Intersection.get_global_position(), Position, LightVector))
	{
		float Diffuse = glm::dot(glm::normalize(Intersection.get_normal()), LightVector);
		if(Diffuse > 0.0f)
		{
			if(Material.is_diffuse())
				Color += m_Color * Material.diffuse() * Diffuse;

			if(Material.is_specular())
			{
				glm::vec3 Reflect = glm::reflect(
					glm::normalize(-LightVector),
					glm::normalize(glm::normalize(Intersection.get_normal())));
				float Dot = glm::dot(Reflect, View);
				float Specular = glm::pow(Dot > 0.0f ? Dot : 0.0f, Material.get_specular_exponent());
				Color += Material.specular() * Specular;
			}
		}
	}

	return Color;
}

#include "light_spot.hpp"
#include "light_factory.hpp"
#include "util.hpp"
#include "main.hpp"

spot* spot::create()
{
	return static_cast<spot*>(light_factory::instance().create(SPOT));
}

spot::spot()
{}

spot::~spot()
{}

glm::vec3 spot::shade(intersection const& Intersection, material const& Material, glm::vec3 const& View) const
{
	glm::vec3 Color(0.0f);

	glm::vec3 LightVector = glm::normalize(this->get_position() - Intersection.get_global_position() + glm::sphericalRand(this->Inaccuracy));

	if(!this->shadow(Intersection.get_global_position(), this->get_position(), LightVector))
	if(glm::dot(this->Direction, -LightVector) > this->CutOff)
	{
		float fDiffuse = glm::dot(Intersection.get_normal(), LightVector);
		if(fDiffuse > 0.0f)
		{
			if(Material.is_diffuse())
				Color += this->Color * Material.diffuse() * fDiffuse;

			if(Material.is_specular())
			{
				glm::vec3 Reflect = glm::reflect(glm::normalize(-LightVector), glm::normalize( Intersection.get_normal()));
				float fDot = glm::dot(Reflect, View);
				float fSpecular = glm::pow(fDot > 0.0f ? fDot : 0.0f, Material.get_specular_exponent());
				Color += Material.specular() * fSpecular;
			}
		}
	}

	return Color;
}

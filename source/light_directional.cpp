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
	glm::vec3 LightVector = glm::normalize(Position - Intersection.getGlobalPosition()/* + glm::sphericalRand(this->Inaccuracy)*/);

	if(!this->shadow(Intersection.getGlobalPosition(), Position, LightVector))
	{
		float fDiffuse = glm::dot(glm::normalize(Intersection.getNormal()), LightVector);
		if(fDiffuse > 0.0f)
		{
			if(Material.isDiffuse())
				Color += m_Color * Material.diffuse() * fDiffuse;

			if(Material.isSpecular())
			{
				glm::vec3 Reflect = glm::reflect(
					glm::normalize(-LightVector), 
					glm::normalize(glm::normalize(Intersection.getNormal())));
				float fDot = glm::dot(Reflect, View);
				float fSpecular = glm::pow(fDot > 0.0f ? fDot : 0.0f, Material.getSpecularExponent());
				Color += Material.specular() * fSpecular;
			}
		}
	}

	//if(this->shadow(Intersection.getGlobalPosition(), Position, LightVector))
		//Color *= 1.0f - Material.getOpacity();
		//Color = glm::vec3(0);

	return Color;
}

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

	glm::vec3 LightVector = glm::normalize(this->getPosition() - Intersection.getGlobalPosition()/* + glm::sphericalRand(this->Inaccuracy)*/);

	if(!this->shadow(Intersection.getGlobalPosition(), this->getPosition(), LightVector))
	if(glm::dot(this->Direction, -LightVector) > this->CutOff)
	{
		float fDiffuse = glm::dot(Intersection.getNormal(), LightVector);
		if(fDiffuse > 0.0f)
		{
			if(Material.isDiffuse())
				Color += this->Color * Material.diffuse() * fDiffuse;

			if(Material.isSpecular())
			{
				glm::vec3 Reflect = glm::reflect(
					glm::normalize(-LightVector),
					glm::normalize( Intersection.getNormal()));
				float fDot = glm::dot(Reflect, View);
				float fSpecular = glm::pow(fDot > 0.0f ? fDot : 0.0f, Material.getSpecularExponent());
				Color += Material.specular() * fSpecular;
			}
		}
	}

	//if(this->shadow(Intersection.getGlobalPosition(), this->getPosition(), LightVector))
		//Color *= 1.0f - Material.getOpacity();
		//Color = glm::vec3(0);

	return Color;
}

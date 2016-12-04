#include "light_point.hpp"
#include "light_factory.hpp"
#include "util.hpp"
#include "main.hpp"

point* point::create()
{
	return static_cast<point*>(light_factory::instance().create(POINT));
}

point::point() :
	Color(1.0f),
	Position(0.0f)
{}

point::~point()
{}

glm::vec3 point::shade(intersection const& Intersection, material const& Material, glm::vec3 const& View) const
{
	glm::vec3 Color(0.0f);
	glm::vec3 LightVector = glm::normalize(this->getPosition() - Intersection.getGlobalPosition()/* + glm::sphericalRand(this->Inaccuracy)*/);

	if(!this->shadow(Intersection.getGlobalPosition(), this->getPosition(), LightVector))
	{
		float Diffuse = glm::dot(Intersection.getNormal(), LightVector);
		if(Diffuse > 0.0f)
		{
			if(Material.isDiffuse()) 
				Color += this->getColor() * Material.diffuse() * Diffuse;

			if(Material.isSpecular())
			{
				glm::vec3 Reflect = glm::reflect(
					glm::normalize(-LightVector), 
					glm::normalize( Intersection.getNormal()));
				float Specular = glm::pow(glm::dot(Reflect, View) > 0 ? glm::dot(Reflect, View) : 0, Material.getSpecularExponent());
				Color += Material.specular() * Specular;
			}
		}
	}

	//if(shadow(Intersection.getGlobalPosition(), this->getPosition(), LightVector))
		//Color *= 1.0f - Material.getOpacity();
		//Color = glm::vec3(0);

	return Color;
}

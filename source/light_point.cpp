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
	glm::vec3 LightVector = glm::normalize(this->get_position() - Intersection.get_global_position()/* + glm::sphericalRand(this->Inaccuracy)*/);

	if(!this->shadow(Intersection.get_global_position(), this->get_position(), LightVector))
	{
		float Diffuse = glm::dot(Intersection.get_normal(), LightVector);
		if(Diffuse > 0.0f)
		{
			if(Material.is_diffuse()) 
				Color += this->get_color() * Material.diffuse() * Diffuse;

			if(Material.is_specular())
			{
				glm::vec3 Reflect = glm::reflect(
					glm::normalize(-LightVector), 
					glm::normalize( Intersection.get_normal()));
				float Specular = glm::pow(glm::dot(Reflect, View) > 0 ? glm::dot(Reflect, View) : 0, Material.get_specular_exponent());
				Color += Material.specular() * Specular;
			}
		}
	}

	return Color;
}

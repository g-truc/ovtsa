#pragma once

#include "intersection.hpp"
#include "material.hpp"

class light
{
public:
	enum type
	{
		POINT,
		SPOT,
		DIRECTIONAL
	};
	
	light();

	virtual glm::vec3 shade(intersection const& Intersection, material const& Material, glm::vec3 const& View) const = 0;

	float get_inaccuracy() const {return this->Inaccuracy;}
	int get_ray_count() const {return this->RayCount;}

	void set_inaccuracy(float Inaccuracy) {this->Inaccuracy = Inaccuracy;}
	void set_ray_count(int RayCount) {this->RayCount = RayCount;}

protected:
	bool shadow(glm::vec3 const& GlobalIntersection, glm::vec3 const& GlobalLightPosition, glm::vec3 const& GlobalLightDirection) const;

	float Inaccuracy;
	int RayCount;
};


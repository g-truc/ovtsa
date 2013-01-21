#ifndef LIGHT_INCLUDED
#define LIGHT_INCLUDED

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

	virtual glm::vec3 shade(
		intersection const & Intersection, 
		material const & Material, 
		glm::vec3 const & View) const = 0;

	float getInaccuracy() const {return this->Inaccuracy;}
	int getRayNumber() const {return this->RayNumber;}

	void setInaccuracy(float Inaccuracy) {this->Inaccuracy = Inaccuracy;}
	void setRayNumber(int RayNumber) {this->RayNumber = RayNumber;}

protected:
	bool shadow(
		glm::vec3 const & GlobalIntersection, 
		glm::vec3 const & GlobalLightPosition, 
		glm::vec3 const & GlobalLightDirection) const;

	float Inaccuracy;
	int RayNumber;
};

#endif //LIGHT_INCLUDED

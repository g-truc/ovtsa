#ifndef LIGHT_POINT_INCLUDED
#define LIGHT_POINT_INCLUDED

#include "light.hpp"

class point : public light
{
	friend class lightFactory;

public:
	static point* create();

	virtual glm::vec3 shade(
		intersection const & Intersection, 
		material const & Material, 
		glm::vec3 const & View) const;

	glm::vec3 const & getPosition() const {return this->Position;}
	glm::vec3 const & getColor() const {return this->Color;}

	void setPosition(glm::vec3 const & Position) {this->Position = Position;}
	void setColor(glm::vec3 const & Color) {this->Color = Color;}

private:
	point();
	~point();

	glm::vec3 Position;
	glm::vec3 Color;
};

#endif //LIGHT_POINT_INCLUDED


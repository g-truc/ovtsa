#ifndef LIGHT_DIRECTIONAL_INCLUDED
#define LIGHT_DIRECTIONAL_INCLUDED

#include "light.hpp"

class directional : public light
{
	friend class lightFactory;

public:
	static directional* create();

	virtual glm::vec3 shade(
		intersection const & Intersection, 
		material const & Material, 
		glm::vec3 const & View) const;

	glm::vec3 const & getDirection() const {return this->m_Direction;}
	glm::vec3 const & getColor() const {return this->m_Color;}
    
	void setDirection(glm::vec3 const & Direction) {this->m_Direction = Direction;}
	void setColor(glm::vec3 const & Color) {this->m_Color = Color;}

private:
	directional();
	~directional();

	glm::vec3 m_Direction;
	glm::vec3 m_Color;
};

#endif //LIGHT_DIRECTIONAL_INCLUDED

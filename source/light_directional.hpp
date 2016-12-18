#pragma once

#include "light.hpp"

class directional : public light
{
	friend class light_factory;

public:
	static directional* create();

	virtual glm::vec3 shade(intersection const& Intersection, material const& Material, glm::vec3 const& View) const;

	glm::vec3 const& get_direction() const {return this->m_Direction;}
	glm::vec3 const& get_color() const {return this->m_Color;}

	void set_direction(glm::vec3 const& Direction) {this->m_Direction = Direction;}
	void set_color(glm::vec3 const& Color) {this->m_Color = Color;}

private:
	directional();
	~directional();

	glm::vec3 m_Direction;
	glm::vec3 m_Color;
};


#pragma once

#include "light.hpp"

class point : public light
{
	friend class light_factory;

public:
	static point* create();

	virtual glm::vec3 shade(intersection const& Intersection, material const& Material, glm::vec3 const& View) const;

	glm::vec3 const& get_position() const {return this->Position;}
	glm::vec3 const& get_color() const {return this->Color;}

	void set_position(glm::vec3 const& Position) {this->Position = Position;}
	void set_color(glm::vec3 const& Color) {this->Color = Color;}

private:
	point();
	~point();

	glm::vec3 Position;
	glm::vec3 Color;
};

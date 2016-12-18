#pragma once

#include "light.hpp"

class spot : public light
{
	friend class light_factory;

public:
	static spot * create();

	virtual glm::vec3 shade(intersection const& Intersection, material const& Material, glm::vec3 const& View) const;

	glm::vec3 const& get_position() const {return this->Position;}
	glm::vec3 const& get_direction() const {return this->Direction;}
	float get_cutoff() const {return this->CutOff;}
	glm::vec3 const& get_color() const {return this->Color;}

	void set_position(glm::vec3 const& Position) {this->Position = Position;}
	void set_direction(glm::vec3 const& Direction) {this->Direction = Direction;}
	void set_cutoff(float CutOff) {this->CutOff = CutOff;}
	void set_color(glm::vec3 const& Color) {this->Color = Color;}

private:
	spot();
	~spot();

	glm::vec3 Position;
	glm::vec3 Direction;
	float CutOff;
	glm::vec3 Color;
};

#pragma once

#include "util.hpp"

class ray
{
public:
	glm::vec3 const& get_position() const {return this->Position;}
	glm::vec3 const& get_direction() const {return this->Direction;}
	float get_environment_index() const {return this->EnvironmentIndex;}

	void set_position(glm::vec3 const& Position) {this->Position = Position;}
	void set_direction(glm::vec3 const& Direction) {this->Direction = Direction;}
	void set_environment_index(float EnvironmentIndex) {this->EnvironmentIndex = EnvironmentIndex;}

private:
	glm::vec3 Position;
	glm::vec3 Direction;
	float EnvironmentIndex;
};


#pragma once

#include "util.hpp"

class intersection
{
public:
	glm::vec3 const& get_local_position() const {return LocalPosition;}
	glm::vec3 const& get_global_position() const {return GlobalPosition;}
	glm::vec3 const& get_normal() const {return Normal;}

	void set_local_position(glm::vec3 const& LocalPosition) {this->LocalPosition = LocalPosition;}
	void set_global_position(glm::vec3 const& GlobalPosition) {this->GlobalPosition = GlobalPosition;}
	void set_normal(glm::vec3 const& Normal) {this->Normal = Normal;}

private:
	glm::vec3 LocalPosition;
	glm::vec3 GlobalPosition;
	glm::vec3 Normal;
};


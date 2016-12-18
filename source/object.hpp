#pragma once

#include "shape.hpp"
#include "transform.hpp"
#include "material.hpp"

class object
{
public:
	static object* create(shape::type const& Shape, material const& Material, glm::mat4 const& Transform);

	object();
	~object();

	object(shape* const Shape, material const& Material, glm::mat4 const& Transform);

	shape* get_shape() {return this->Shape;}
	shape* const get_shape() const {return this->Shape;}
	material const* get_material() const {return &this->Material;}
	transform const* get_transform() const {return &this->Transform;}

private:
	shape* Shape;
	material Material;
	transform Transform;
};

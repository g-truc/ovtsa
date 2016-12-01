#pragma once

#include "shape.hpp"
#include "transform.hpp"
#include "material.hpp"

class object
{
public:
	static object* create(
		shape::type const & Shape,
		material const & Material,
		glm::mat4 const & Transform);

	object();
	~object();

	object(
		shape * const Shape, 
		material const & Material, 
		glm::mat4 const & Transform);

	shape* getShape() {return this->Shape;}
	shape* const getShape() const {return this->Shape;}
	material const* getMaterial() const {return &this->Material;}
	transform const* getTransform() const {return &this->Transform;}

private:
	shape* Shape;
	material Material;
	transform Transform;
};

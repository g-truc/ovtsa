#pragma once

#include "util.hpp"

class transform
{
public:
	transform(){}
	transform(glm::mat4 const& Matrix)
		: TransformMatrix(glm::transpose(Matrix))
		, InverseMatrix(glm::inverse(TransformMatrix))
		, InverseTransposeMatrix(glm::transpose(InverseMatrix))
	{}

	glm::vec4 computeMatrix(glm::vec4 const& v) const
	{
		return v * this->TransformMatrix;
	}

	glm::vec4 computeInverse(glm::vec4 const& v) const
	{
		return v * this->InverseMatrix;
	}

	glm::vec4 computeNormal(glm::vec4 const& Normal) const
	{
		return Normal * this->InverseTransposeMatrix;
	}

private:
	glm::mat4 TransformMatrix;
	glm::mat4 InverseMatrix;
	glm::mat4 InverseTransposeMatrix;
};


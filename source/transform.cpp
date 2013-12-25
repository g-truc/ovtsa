#include "transform.hpp"
#include <glm/matrix.hpp>

transform::transform()
{}

transform::transform
(
	glm::mat4 const & Matrix
)
{
	this->TransformMatrix = glm::transpose(Matrix);
	this->InverseMatrix = glm::inverse(TransformMatrix);
	this->InverseTransposeMatrix = glm::transpose(InverseMatrix);
}

glm::vec4 transform::computeMatrix
(
	glm::vec4 const & v
) const
{
	return v * this->TransformMatrix;
}

glm::vec4 transform::computeInverse
(
	glm::vec4 const & v
) const
{
	return v * this->InverseMatrix;
}

glm::vec4 transform::computeNormal
(
	glm::vec4 const & Normal
) const
{
	return Normal * this->InverseTransposeMatrix;
}


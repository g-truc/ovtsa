#ifndef TRANSFORM_INCLUDED
#define TRANSFORM_INCLUDED

#include "util.hpp"

class transform
{
public:
	transform();
	transform(glm::mat4 const & Matrix);

	glm::vec4 computeMatrix(glm::vec4 const & v) const;
	glm::vec4 computeInverse(glm::vec4 const & v) const;
	glm::vec4 computeNormal(glm::vec4 const & Normal) const;

private:
	glm::mat4 TransformMatrix;
	glm::mat4 InverseMatrix;
	glm::mat4 InverseTransposeMatrix;
};

#endif //TRANSFORM_INCLUDED

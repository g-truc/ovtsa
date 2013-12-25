#include "adaptator.hpp"
#include <glm/geometric.hpp>
#include <cassert>

adaptator::adaptator(glm::uvec2 const & Size)
{
	this->init(Size);
}

adaptator::~adaptator()
{}

bool adaptator::init(glm::uvec2 const & Size)
{
	this->Size = Size;
	this->Data.resize(this->Size.x * this->Size.y, 0);

	return true;
}

void adaptator::add(glm::uvec2 const & Position)
{
	this->Data[Position.x + Position.y * this->Size.x]++;
}

float adaptator::getFactor(glm::uvec2 const & Position) const
{
	glm::uvec2 PositionClamped = glm::clamp(Position, glm::uvec2(0), this->Size - glm::uvec2(1));
	assert(glm::all(glm::lessThan(PositionClamped, this->Size)));
	return this->Data[PositionClamped.x + PositionClamped.y * this->Size.x];
}

#include "surface.hpp"
#include <gli/core/save_dds.hpp>
#include <gli/core/fetch.hpp>

surface::surface(glm::uvec2 const & Size)
{
	this->init(Size);
}

surface::~surface()
{}

void surface::init(glm::uvec2 const & Size)
{
	this->Size = Size;
	this->Data.resize(glm::compMul(this->Size));
}

glm::vec3 const & surface::getTexel(glm::uvec2 const & Position) const
{
	glm::uvec2 PositionClamped = glm::clamp(Position, glm::uvec2(0), this->Size - glm::uvec2(1));
	assert(glm::all(glm::lessThan(PositionClamped, this->Size)));
	return this->Data[PositionClamped.x + PositionClamped.y * this->Size.x];
}

void surface::add
(
	glm::uvec2 const & Position, 
	glm::vec3 const & Color
)
{
	assert(glm::all(glm::lessThan(Position, this->Size)));
	this->Data[Position.x + Position.y * this->Size.x] += Color;
}

void surface::div
(
	glm::uvec2 const & Position, 
	float const & Value
)
{
	assert(glm::all(glm::lessThan(Position, this->Size)));
	this->Data[Position.x + Position.y * this->Size.x] /= Value;
}

void surface::save(char const * Filename)
{
	gli::texture2D Texture(1, gli::RGB8U, this->Size);

	for(glm::uint y = 0; y < Texture.dimensions().y; y++)
	for(glm::uint x = 0; x < Texture.dimensions().x; x++)
	{
		glm::u8vec3 Color(glm::clamp(this->Data[x + y * this->Size.x] * 256.f, 0.0f, 255.f));

		gli::texelWrite<glm::u8vec3>(Texture, gli::texture2D::dimensions_type(x, y), 0, Color);
	}

	gli::save_dds(Texture, Filename);
}


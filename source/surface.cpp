#include "surface.hpp"

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

void surface::SaveAs(std::string const & Filename)
{
	gli::image Image(1);
	Image[0] = gli::image::mipmap(glm::uvec3(this->Size, glm::uint(1)), gli::RGB8U);

	for(glm::uint y = 0; y < Image[0].dimensions().y; y++)
    for(glm::uint x = 0; x < Image[0].dimensions().x; x++)
    {
        glm::vec3 Color = this->Data[x + y * this->Size.x];
		Color = glm::clamp(Color * 256.f, 0.0f, 255.f);
		glm::u8vec3 ColorLDR(Color);

		gli::texelWrite<glm::u8vec3>(
			Image,
			glm::uvec2(x, y),
			0,
			ColorLDR);
    }

	gli::export_as(Image, 0, Filename);
}

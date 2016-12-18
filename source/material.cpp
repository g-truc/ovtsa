#include "material.hpp"

material::material()
	: Repeat(1.f)
	, Type(MATERIAL_FLAT)
	, Ambient(0.1f, 0.1f, 0.1f)
	, AmbientSecondary(0.0f, 0.0f, 0.0f)
	, Diffuse(0.1f, 0.1f, 0.1f)
	, Specular(0.1f, 0.1f, 0.1f)
	, EnvironmentIndex(0)
{}

glm::vec3 material::ambient(glm::vec3 const& Position) const
{
	switch(Type)
	{
	default:
	case MATERIAL_NULL:
		return glm::vec3(0, 0, 0);
	case MATERIAL_FLAT:
		return this->Ambient;
	case MATERIAL_GRID:
		return this->color_grid(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_LINE_X:
		return this->color_line_x(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_LINE_Y:
		return this->color_line_y(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_LINE_Z:
		return this->color_line_z(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_PERLIN:
		return this->color_perlin(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_MARBLE:
		return this->color_marble(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_WOOD:
		return this->color_wood(Position, this->Ambient, this->AmbientSecondary);
	}
}

void material::set_ambient(glm::vec3 const& Color, float fFactor)
{
	this->Ambient = Color * fFactor;
}

void material::set_ambient_secondary(glm::vec3 const& Color, float Factor)
{
	this->AmbientSecondary = Color * Factor;
}

void material::set_diffuse(glm::vec3 const& Color, float Factor)
{
	this->Diffuse = Color * Factor;
	this->DiffuseEnabled = Factor > 0.0f;
}
void material::set_specular(glm::vec3 const& Color, float Factor)
{
	this->Specular = Color * Factor;
	this->SpecularEnabled = Factor > 0.0f;
}

bool material::is_diffuse() const
{
	return this->DiffuseEnabled;
}

bool material::is_specular() const
{
	return this->SpecularEnabled;
}

glm::vec3 material::color_grid(glm::vec3 const& Position, glm::vec3 const& ColorPrimary, glm::vec3 const& ColorSecondary) const
{
	glm::bvec3 Grid = glm::equal(glm::ivec3((Position - 1000.f) * this->Repeat) % 2, glm::ivec3(0));

	if(Grid.z)
	{
		if((Grid.x && Grid.y) || (!Grid.x && !Grid.y))
			return ColorPrimary;
		else
			return ColorSecondary;
	}
	else
	{
		if((Grid.x && Grid.y) || (!Grid.x && !Grid.y))
			return ColorSecondary;
		else
			return ColorPrimary;
	}
}

glm::vec3 material::color_line_x(glm::vec3 const& Position, glm::vec3 const& ColorPrimary, glm::vec3 const& ColorSecondary) const
{
	bool y = (int((Position.y - 1000) * Repeat) % 2) == 0;
	bool z = (int((Position.z - 1000) * Repeat) % 2) == 0;

	if(z)
	{
		if(y)
			return ColorPrimary;
		else
			return ColorSecondary;
	}
	else
	{
		if(y)
			return ColorSecondary;
		else
			return ColorPrimary;
	}
}

glm::vec3 material::color_line_y(glm::vec3 const& Position, glm::vec3 const& ColorPrimary, glm::vec3 const& ColorSecondary) const
{
	bool x = (int((Position.x - 1000) * this->Repeat) % 2) == 0;
	bool z = (int((Position.z - 1000) * this->Repeat) % 2) == 0;

	if(z)
	{
		if(x)
			return ColorPrimary;
		else
			return ColorSecondary;
	}
	else
	{
		if(x)
			return ColorSecondary;
		else
			return ColorPrimary;
	}
}

glm::vec3 material::color_line_z(glm::vec3 const& Position, glm::vec3 const& ColorPrimary, glm::vec3 const& ColorSecondary) const
{
	bool x = (int((Position.x - 1000) * this->Repeat) % 2) == 0;
	bool y = (int((Position.y - 1000) * this->Repeat) % 2) == 0;

	if(y)
	{
		if(x)
			return ColorPrimary;
		else
			return ColorSecondary;
	}
	else
	{
		if(x)
			return ColorSecondary;
		else
			return ColorPrimary;
	}
}

glm::vec3 material::color_perlin(glm::vec3 const& Position, glm::vec3 const& ColorPrimary, glm::vec3 const& ColorSecondary) const
{
	float noise = glm::simplex(Position * this->Repeat);
	return glm::mix(ColorPrimary, ColorSecondary, glm::clamp(noise, 0.0f, 1.0f));
}

glm::vec3 material::color_marble(glm::vec3 const& Position, glm::vec3 const& ColorPrimary, glm::vec3 const& ColorSecondary) const
{
	float noise = glm::abs(glm::simplex(Position * this->Repeat));
	float marble = glm::cos(Position.x + noise);

	if(marble < -0.5f)
		return ColorSecondary;
	else if((marble < 0.5f) && (marble > -0.5f))
		return ColorPrimary * (marble + 0.5f) + ColorSecondary * (0.5f - marble);
	else
		return ColorPrimary;
}

glm::vec3 material::color_wood(glm::vec3 const& Position, glm::vec3 const& ColorPrimary, glm::vec3 const& ColorSecondary) const
{
	float noise = 100.f * glm::abs(glm::simplex(Position * this->Repeat));
	float wood = noise - glm::abs(noise);

	if(wood < -0.5f)
		return ColorSecondary;
	else if((wood < 0.5f) && (wood > -0.5f))
		return ColorPrimary * (wood + 0.5f) + ColorSecondary * (0.5f - wood);
	else
		return ColorPrimary;
}

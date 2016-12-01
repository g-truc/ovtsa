#include "material.hpp"

perlin g_Perlin;

material::material() : 
	Repeat(1.f),
	Type(MATERIAL_FLAT),
	Ambient(0.1f, 0.1f, 0.1f),
	AmbientSecondary(0.0f, 0.0f, 0.0f),
	Diffuse(0.1f, 0.1f, 0.1f),
	Specular(0.1f, 0.1f, 0.1f),
	EnvironmentIndex(0)
{
	static bool Init = false;
	if(!Init)
	{
		g_Perlin.Init(0);
		Init = true;
	}
}

glm::vec3 material::ambient
(
	glm::vec3 const & Position
) const
{
	switch(Type)
	{
	default:
	case MATERIAL_NULL:
		return glm::vec3(0, 0, 0);
	case MATERIAL_FLAT:
		return this->Ambient;
	case MATERIAL_GRID:
		return colorGrid(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_LINE_X:
		return colorLineX(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_LINE_Y:
		return colorLineY(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_LINE_Z:
		return colorLineZ(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_NOISE:
		return colorNoise(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_PERLIN:
		return colorPerlin(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_MARBLE:
		return colorMarble(Position, this->Ambient, this->AmbientSecondary);
	case MATERIAL_WOOD:
		return colorWood(Position, this->Ambient, this->AmbientSecondary);
	}
}

void material::setAmbient
(
	glm::vec3 const & Color, 
	float fFactor
)
{
	this->Ambient = Color * fFactor;
}

void material::setAmbientSecondary
(
	glm::vec3 const & Color, 
	float fFactor
)
{
	this->AmbientSecondary = Color * fFactor;
}

void material::setDiffuse
(
	glm::vec3 const & Color, 
	float fFactor
)
{
	this->Diffuse = Color * fFactor;
	this->DiffuseEnabled = fFactor > 0.0f;
}
void material::setSpecular
(
	glm::vec3 const & Color, 
	float Factor
)
{
	this->Specular = Color * Factor;
	this->SpecularEnabled = Factor > 0.0f;
}

bool material::isDiffuse() const
{
	return this->DiffuseEnabled;
}

bool material::isSpecular() const
{
	return this->SpecularEnabled;
}

glm::vec3 material::colorGrid
(
	glm::vec3 const & Position, 
	glm::vec3 const & ColorPrimary, 
	glm::vec3 const & ColorSecondary
) const
{
	glm::bvec3 Grid = glm::equal(glm::ivec3((Position - 1000.f) * this->Repeat) % 2, glm::ivec3(0));
	//bool x = (int((Position.x - 1000) * this->Repeat) % 2) == 0;
	//bool y = (int((Position.y - 1000) * this->Repeat) % 2) == 0;
	//bool z = (int((Position.z - 1000) * this->Repeat) % 2) == 0;

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

glm::vec3 material::colorLineX
(
	glm::vec3 const & Position, 
	glm::vec3 const & ColorPrimary, 
	glm::vec3 const & ColorSecondary
) const
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

glm::vec3 material::colorLineY
(
	glm::vec3 const & Position,
	glm::vec3 const & ColorPrimary, 
	glm::vec3 const & ColorSecondary
) const
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

glm::vec3 material::colorLineZ
(
	glm::vec3 const & Position, 
	glm::vec3 const & ColorPrimary, 
	glm::vec3 const & ColorSecondary
) const
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

glm::vec3 material::colorNoise
(
	glm::vec3 const & Position, 
	glm::vec3 const & ColorPrimary, 
	glm::vec3 const & ColorSecondary
) const
{
	float noise = float(std::rand()) / RAND_MAX;
	return ColorPrimary * noise + ColorSecondary * (1.0f - noise);  
}

glm::vec3 material::colorPerlin
(
	glm::vec3 const & Position,
	glm::vec3 const & ColorPrimary,
	glm::vec3 const & ColorSecondary
) const
{
	//float noise = glm::simplex(Position * this->Repeat);
	float noise = g_Perlin.Noise3(Position.x * this->Repeat, Position.y * this->Repeat, Position.z * this->Repeat);
	if(noise < -0.5f)
		return ColorSecondary;
	else if((noise > -0.5f) && (noise < 0.5f))
		return ColorPrimary * (noise - 1.0f) + ColorSecondary * (1.0f - noise);
	else
		return ColorPrimary;
}

glm::vec3 material::colorMarble
(
	glm::vec3 const & Position, 
	glm::vec3 const & ColorPrimary, 
	glm::vec3 const & ColorSecondary
) const
{
	float noise = g_Perlin.Noise3(Position.x * this->Repeat, Position.y * this->Repeat, Position.z * this->Repeat);
	//float noise = glm::simplex(Position * this->Repeat);
	float marble = glm::cos(Position.x + noise);

	if(marble < -0.5f)
		return ColorSecondary;
	else if((marble < 0.5f) && (marble > -0.5f))
		return ColorPrimary * (marble + 0.5f) + ColorSecondary * (0.5f - marble);
	else
		return ColorPrimary;
}

glm::vec3 material::colorWood
(
	glm::vec3 const & Position,
	glm::vec3 const & ColorPrimary,
	glm::vec3 const & ColorSecondary
) const
{
	float noise = 100.f * g_Perlin.Noise3(Position.x * this->Repeat, Position.y * this->Repeat, Position.z * this->Repeat);
	//float noise = 100.f * glm::simplex(Position * this->Repeat);
	float wood = noise - glm::abs(noise);

	if(wood < -0.5f)
		return ColorSecondary;
	else if((wood < 0.5f) && (wood > -0.5f))
		return ColorPrimary * (wood + 0.5f) + ColorSecondary * (0.5f - wood);
	else
		return ColorPrimary;
}

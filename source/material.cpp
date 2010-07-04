#include "material.hpp"

perlin g_Perlin;

material::material() : 
    m_Repeat(1.f),
    m_Type(MATERIAL_FLAT),
    m_Ambient(0.1f, 0.1f, 0.1f),
    m_AmbientSecondary(0.0f, 0.0f, 0.0f),
    m_Diffuse(0.1f, 0.1f, 0.1f),
    m_Specular(0.1f, 0.1f, 0.1f),
    m_EnvironmentIndex(0)
{
	static bool Init = false;
	if(!Init)
	{
		g_Perlin.Init(0);
		Init = true;
	}
}

glm::vec3 material::ambient(const glm::vec3 & Position) const
{
    switch(m_Type)
    {
    default:
    case MATERIAL_NULL:
        return glm::vec3(0, 0, 0);
    case MATERIAL_FLAT:
        return m_Ambient;
    case MATERIAL_GRID:
        return colorGrid(Position, m_Ambient, m_AmbientSecondary);
    case MATERIAL_LINE_X:
        return colorLineX(Position, m_Ambient, m_AmbientSecondary);
    case MATERIAL_LINE_Y:
        return colorLineY(Position, m_Ambient, m_AmbientSecondary);
    case MATERIAL_LINE_Z:
        return colorLineZ(Position, m_Ambient, m_AmbientSecondary);
    case MATERIAL_NOISE:
        return colorNoise(Position, m_Ambient, m_AmbientSecondary);
    case MATERIAL_PERLIN:
        return colorPerlin(Position, m_Ambient, m_AmbientSecondary);
    case MATERIAL_MARBLE:
        return colorMarble(Position, m_Ambient, m_AmbientSecondary);
    case MATERIAL_WOOD:
        return colorWood(Position, m_Ambient, m_AmbientSecondary);
    }
}

void material::setAmbient(const glm::vec3 & Color, float fFactor)
{
    m_Ambient = Color * fFactor;
}

void material::setAmbientSecondary(const glm::vec3 & Color, float fFactor)
{
    m_AmbientSecondary = Color * fFactor;
}

void material::setDiffuse(const glm::vec3 & Color, float fFactor)
{
    m_Diffuse = Color * fFactor;
    m_bDiffuse = fFactor > 0.0f;
}
void material::setSpecular(const glm::vec3 & Color, float fFactor)
{
    m_Specular = Color * fFactor;
    m_bSpecular = fFactor > 0.0f;
}

bool material::isDiffuse() const
{
    return m_bDiffuse;
}

bool material::isSpecular() const
{
    return m_bSpecular;
}

glm::vec3 material::colorGrid(const glm::vec3 & Position, const glm::vec3 & ColorPrimary, const glm::vec3 & ColorSecondary) const
{
    bool x = (int((Position.x - 1000) * m_Repeat) % 2) == 0;
    bool y = (int((Position.y - 1000) * m_Repeat) % 2) == 0;
    bool z = (int((Position.z - 1000) * m_Repeat) % 2) == 0;

    if(z)
    {
        if((x && y) || (!x && !y))
            return ColorPrimary;
        else
            return ColorSecondary;
    }
    else
    {
        if((x && y) || (!x && !y))
            return ColorSecondary;
        else
            return ColorPrimary;
    }
}

glm::vec3 material::colorLineX(const glm::vec3 & Position, const glm::vec3 & ColorPrimary, const glm::vec3 & ColorSecondary) const
{
    bool y = (int((Position.y - 1000) * m_Repeat) % 2) == 0;
    bool z = (int((Position.z - 1000) * m_Repeat) % 2) == 0;

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

glm::vec3 material::colorLineY(const glm::vec3 & Position, const glm::vec3 & ColorPrimary, const glm::vec3 & ColorSecondary) const
{
    bool x = (int((Position.x - 1000) * m_Repeat) % 2) == 0;
    bool z = (int((Position.z - 1000) * m_Repeat) % 2) == 0;

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

glm::vec3 material::colorLineZ(const glm::vec3 & Position, const glm::vec3 & ColorPrimary, const glm::vec3 & ColorSecondary) const
{
    bool x = (int((Position.x - 1000) * m_Repeat) % 2) == 0;
    bool y = (int((Position.y - 1000) * m_Repeat) % 2) == 0;

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

glm::vec3 material::colorNoise(const glm::vec3 & Position, const glm::vec3 & ColorPrimary, const glm::vec3 & ColorSecondary) const
{
    float noise = float(std::rand()) / RAND_MAX;
    return ColorPrimary * noise + ColorSecondary * (1.0f - noise);  
}

glm::vec3 material::colorPerlin(const glm::vec3 & Position, const glm::vec3 & ColorPrimary, const glm::vec3 & ColorSecondary) const
{
    float noise = g_Perlin.Noise3(Position.x * m_Repeat, Position.y * m_Repeat, Position.z * m_Repeat);
    if(noise < -0.5f)
        return ColorSecondary;
    else if((noise > -0.5f) && (noise < 0.5f))
        return ColorPrimary * (noise - 1.0f) + ColorSecondary * (1.0f - noise);
    else
        return ColorPrimary;
}

glm::vec3 material::colorMarble(const glm::vec3 & Position, const glm::vec3 & ColorPrimary, const glm::vec3 & ColorSecondary) const
{
    float noise = g_Perlin.Noise3(Position.x * m_Repeat, Position.y * m_Repeat, Position.z * m_Repeat);
    float marble = glm::cos(Position.x + noise);

    if(marble < -0.5f)
        return ColorSecondary;
    else if((marble < 0.5f) && (marble > -0.5f))
        return ColorPrimary * (marble + 0.5f) + ColorSecondary * (0.5f - marble);
    else
        return ColorPrimary;
}

glm::vec3 material::colorWood(const glm::vec3 & Position, const glm::vec3 & ColorPrimary, const glm::vec3 & ColorSecondary) const
{
    float noise = 100.f * g_Perlin.Noise3(Position.x * m_Repeat, Position.y * m_Repeat, Position.z * m_Repeat);
    float wood = noise - glm::abs(noise);

    if(wood < -0.5f)
        return ColorSecondary;
    else if((wood < 0.5f) && (wood > -0.5f))
        return ColorPrimary * (wood + 0.5f) + ColorSecondary * (0.5f - wood);
    else
        return ColorPrimary;
}

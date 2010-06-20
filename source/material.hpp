#ifndef MATERIAL_INCLUDED
#define MATERIAL_INCLUDED

#include "util.hpp"
#include "perlin.hpp"

extern perlin g_Perlin;

class material
{
public:
	enum type
	{
		MATERIAL_NULL,
		MATERIAL_FLAT,
		MATERIAL_GRID,
		MATERIAL_LINE_X,
		MATERIAL_LINE_Y,
		MATERIAL_LINE_Z,
		MATERIAL_NOISE,
		MATERIAL_PERLIN,
		MATERIAL_MARBLE,
		MATERIAL_WOOD
	};
	
	material();

    glm::vec3 Ambient(const glm::vec3 & Position) const;
    glm::vec3 AmbientSecondary(const glm::vec3 & Position) const;
    glm::vec3 Diffuse() const {return m_Diffuse;}
    glm::vec3 Specular() const {return m_Specular;}
	float getOpacity() const {return this->Opacity;}
	float SpecularExponent() const {return m_SpecularExponent;}
    float EnvironmentIndex() const {return m_EnvironmentIndex;}
    float ReflectionFactor() const {return m_ReflectionFactor;}
    float RefractionFactor() const {return m_RefractionFactor;}
    float Repeat() const {return m_Repeat;}
    type Type() const {return m_Type;}

    void Ambient(const glm::vec3 & Color, float fFactor);
    void AmbientSecondary(const glm::vec3 & Color, float fFactor);
    void Diffuse(const glm::vec3 & Color, float fFactor);
    void Specular(const glm::vec3 & Color, float fFactor);
	void setOpacity(float const & Opacity){this->Opacity = Opacity;}
    void SpecularExponent(float SpecularExponent){m_SpecularExponent = SpecularExponent;}
    void EnvironmentIndex(float EnvironmentIndex){m_EnvironmentIndex = EnvironmentIndex;}
    void ReflectionFactor(float ReflectionFactor){m_ReflectionFactor = ReflectionFactor;}
    void RefractionFactor(float RefractionFactor){m_RefractionFactor = RefractionFactor;}
    void Repeat(float Repeat){m_Repeat = Repeat;}
    void Type(const type & Material){m_Type = Material;}

    bool IsDiffuse() const;
    bool IsSpecular() const;

private:
    glm::vec3 m_Ambient;
    glm::vec3 m_AmbientSecondary;
    glm::vec3 m_Diffuse;
    glm::vec3 m_Specular;

	float Opacity;
    float m_SpecularExponent;
    float m_EnvironmentIndex;
    float m_ReflectionFactor;
    float m_RefractionFactor;
    float m_Repeat;

    bool m_bDiffuse;
    bool m_bSpecular;

    type m_Type;

    glm::vec3 ColorGrid(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 ColorLineX(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 ColorLineY(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 ColorLineZ(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 ColorNoise(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 ColorPerlin(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 ColorMarble(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 ColorWood(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
};

#endif //MATERIAL_INCLUDED


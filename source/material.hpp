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

    glm::vec3 ambient(const glm::vec3 & Position) const;
    glm::vec3 ambientSecondary(const glm::vec3 & Position) const;
    glm::vec3 diffuse() const {return m_Diffuse;}
    glm::vec3 specular() const {return m_Specular;}
	float getOpacity() const {return this->Opacity;}
	float getSpecularExponent() const {return m_SpecularExponent;}
    float getEnvironmentIndex() const {return m_EnvironmentIndex;}
    float getReflectionFactor() const {return m_ReflectionFactor;}
    float getRefractionFactor() const {return m_RefractionFactor;}
    float getRepeat() const {return m_Repeat;}
    type getType() const {return m_Type;}

    void setAmbient(const glm::vec3 & Color, float fFactor);
    void setAmbientSecondary(const glm::vec3 & Color, float fFactor);
    void setDiffuse(const glm::vec3 & Color, float fFactor);
    void setSpecular(const glm::vec3 & Color, float fFactor);
	void setOpacity(float const & Opacity){this->Opacity = Opacity;}
    void setSpecularExponent(float SpecularExponent){m_SpecularExponent = SpecularExponent;}
    void setEnvironmentIndex(float EnvironmentIndex){m_EnvironmentIndex = EnvironmentIndex;}
    void setReflectionFactor(float ReflectionFactor){m_ReflectionFactor = ReflectionFactor;}
    void setRefractionFactor(float RefractionFactor){m_RefractionFactor = RefractionFactor;}
    void setRepeat(float Repeat){m_Repeat = Repeat;}
    void setType(const type & Material){m_Type = Material;}

    bool isDiffuse() const;
    bool isSpecular() const;

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

    glm::vec3 colorGrid(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 colorLineX(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 colorLineY(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 colorLineZ(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 colorNoise(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 colorPerlin(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 colorMarble(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
    glm::vec3 colorWood(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
};

#endif //MATERIAL_INCLUDED


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
    glm::vec3 diffuse() const {return this->Diffuse;}
    glm::vec3 specular() const {return this->Specular;}
	float getOpacity() const {return this->Opacity;}
	float getSpecularExponent() const {return this->SpecularExponent;}
    float getEnvironmentIndex() const {return this->EnvironmentIndex;}
    float getReflectionFactor() const {return this->ReflectionFactor;}
    float getRefractionFactor() const {return this->RefractionFactor;}
    float getRepeat() const {return this->Repeat;}
    type getType() const {return this->Type;}

    void setAmbient(glm::vec3 const & Color, float Factor);
    void setAmbientSecondary(glm::vec3 const & Color, float Factor);
    void setDiffuse(glm::vec3 const & Color, float Factor);
    void setSpecular(glm::vec3 const & Color, float Factor);
	void setOpacity(float const & Opacity){this->Opacity = Opacity;}
    void setSpecularExponent(float SpecularExponent){this->SpecularExponent = SpecularExponent;}
    void setEnvironmentIndex(float EnvironmentIndex){this->EnvironmentIndex = EnvironmentIndex;}
    void setReflectionFactor(float ReflectionFactor){this->ReflectionFactor = ReflectionFactor;}
    void setRefractionFactor(float RefractionFactor){this->RefractionFactor = RefractionFactor;}
    void setRepeat(float Repeat){this->Repeat = Repeat;}
    void setType(type const & Type){this->Type = Type;}

    bool isDiffuse() const;
    bool isSpecular() const;

private:
    glm::vec3 Ambient;
    glm::vec3 AmbientSecondary;
    glm::vec3 Diffuse;
    glm::vec3 Specular;

	float Opacity;
    float SpecularExponent;
    float EnvironmentIndex;
    float ReflectionFactor;
    float RefractionFactor;
    float Repeat;

    bool DiffuseEnabled;
    bool SpecularEnabled;

    type Type;

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


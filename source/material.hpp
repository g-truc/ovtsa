#pragma once

#include "util.hpp"

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
		MATERIAL_PERLIN,
		MATERIAL_MARBLE,
		MATERIAL_WOOD
	};
	
	material();

	glm::vec3 ambient(const glm::vec3 & Position) const;
	glm::vec3 diffuse() const {return this->Diffuse;}
	glm::vec3 specular() const {return this->Specular;}
	float get_opacity() const {return this->Opacity;}
	float get_specular_exponent() const {return this->SpecularExponent;}
	float get_environment_index() const {return this->EnvironmentIndex;}
	float get_reflection_factor() const {return this->ReflectionFactor;}
	float get_refraction_factor() const {return this->RefractionFactor;}
	float get_repeat() const {return this->Repeat;}
	type get_type() const {return this->Type;}

	void set_ambient(glm::vec3 const& Color, float Factor);
	void set_ambient_secondary(glm::vec3 const& Color, float Factor);
	void set_diffuse(glm::vec3 const& Color, float Factor);
	void set_specular(glm::vec3 const& Color, float Factor);
	void set_opacity(float Opacity){this->Opacity = Opacity;}
	void set_specular_exponent(float SpecularExponent){this->SpecularExponent = SpecularExponent;}
	void set_environment_index(float EnvironmentIndex){this->EnvironmentIndex = EnvironmentIndex;}
	void set_reflection_factor(float ReflectionFactor){this->ReflectionFactor = ReflectionFactor;}
	void set_refraction_factor(float RefractionFactor){this->RefractionFactor = RefractionFactor;}
	void set_repeat(float Repeat){this->Repeat = Repeat;}
	void set_type(type const& Type){this->Type = Type;}

	bool is_diffuse() const;
	bool is_specular() const;

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

	glm::vec3 color_grid(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
	glm::vec3 color_line_x(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
	glm::vec3 color_line_y(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
	glm::vec3 color_line_z(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
	glm::vec3 color_perlin(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
	glm::vec3 color_marble(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
	glm::vec3 color_wood(glm::vec3 const & Position, glm::vec3 const & ColorPrimary, glm::vec3 const & ColorSecondary) const;
};


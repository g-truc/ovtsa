#pragma once

#include "util.hpp"

class surface
{
public:
	surface(glm::uvec2 const& Size);
	~surface();

	glm::vec3* get_data() {return &this->Data[0];}
	glm::vec3 const& get_texel(glm::uvec2 const& Position) const;
	void save_as(std::string const& Filename);

	void add(glm::uvec2 const& Position, glm::vec3 const& Color);
	void div(glm::uvec2 const& Position, float Value);

private:
	surface(surface const& Surface);
	surface& operator=(surface const& Surface);

	void init(glm::uvec2 const& Size);

	glm::uvec2 Size;
	std::vector<glm::vec3> Data;
};

#pragma once

#include "util.hpp"

class adaptator
{
public:
	adaptator(glm::uvec2 const& Size);
	~adaptator();

	void add(glm::uvec2 const& Position);
	float get_factor(glm::uvec2 const& Position) const;

private:
	adaptator(adaptator const&);
	adaptator& operator=(adaptator const&);

	bool init(glm::uvec2 const & Size);

	std::vector<float> Data;
	glm::uvec2 Size;
};

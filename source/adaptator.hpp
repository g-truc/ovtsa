#ifndef ADAPTATOR_INCLUDED
#define ADAPTATOR_INCLUDED

#include <glm/vec2.hpp>
#include <vector>

class adaptator// : boost::noncopyable
{
public:
	adaptator(glm::uvec2 const & Size);
	~adaptator();

	void add(glm::uvec2 const & Position);
	float getFactor(glm::uvec2 const & Position) const;

private:
	bool init(glm::uvec2 const & Size);

	std::vector<float> Data;
	glm::uvec2 Size;
};

#endif //ADAPTATOR_INCLUDED

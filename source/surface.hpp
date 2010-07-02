#ifndef SURFACE_INCLUDED
#define SURFACE_INCLUDED

#include "util.hpp"

class surface// : boost::noncopyable
{
public:
    surface(glm::uvec2 const & Size);
    ~surface();

    glm::vec3 * getData() {return &this->Data[0];}
    glm::vec3 const & getTexel(glm::uvec2 const & Position) const;
	void SaveAs(std::string const & Filename);

    void add(
		glm::uvec2 const & Position, 
		glm::vec3 const & Color);
    void div(
		glm::uvec2 const & Position, 
		float const & Value);

private:
    void init(glm::uvec2 const & Size);

	glm::uvec2 Size;
	std::vector<glm::vec3> Data;
};

#endif //SURFACE_INCLUDED


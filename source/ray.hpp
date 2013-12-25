#ifndef RAY_INCLUDED
#define RAY_INCLUDED

#include <glm/vec3.hpp>

class ray
{
public:
	glm::vec3 const & getPosition() const {return this->Position;}
	glm::vec3 const & getDirection() const {return this->Direction;}
	float getEnvironmentIndex() const {return this->EnvironmentIndex;}

	void setPosition(glm::vec3 const & Position) {this->Position = Position;}
	void setDirection(glm::vec3 const & Direction) {this->Direction = Direction;}
	void setEnvironmentIndex(float EnvironmentIndex) {this->EnvironmentIndex = EnvironmentIndex;}

private:
	glm::vec3 Position;
	glm::vec3 Direction;
	float EnvironmentIndex;
};

#endif //RAY_INCLUDED

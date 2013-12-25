#ifndef INTERSECTION_INCLUDED
#define INTERSECTION_INCLUDED

#include <glm/vec3.hpp>

class intersection
{
public:
	glm::vec3 const & getLocalPosition() const {return LocalPosition;}
	glm::vec3 const & getGlobalPosition() const {return GlobalPosition;}
	glm::vec3 const & getNormal() const {return Normal;}

	void setLocalPosition(glm::vec3 const & LocalPosition) {this->LocalPosition = LocalPosition;}
	void setGlobalPosition(glm::vec3 const & GlobalPosition) {this->GlobalPosition = GlobalPosition;}
	void setNormal(glm::vec3 const & Normal) {this->Normal = Normal;}

private:
	glm::vec3 LocalPosition;
	glm::vec3 GlobalPosition;
	glm::vec3 Normal;
};

#endif //INTERSECTION_INCLUDED

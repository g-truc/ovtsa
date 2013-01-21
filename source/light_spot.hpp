#ifndef LIGHT_SPOT_INCLUDED
#define LIGHT_SPOT_INCLUDED

#include "light.hpp"

class spot : public light
{
	friend class lightFactory;

public:
	static spot * create();

	virtual glm::vec3 shade(
		intersection const & Intersection, 
		material const & Material, 
		glm::vec3 const & View) const;

	glm::vec3 const & getPosition() const {return this->Position;}
	glm::vec3 const & getDirection() const {return this->Direction;}
	float getCutOff() const {return this->CutOff;}
	glm::vec3 const & getColor() const {return this->Color;}

	void setPosition(glm::vec3 const & Position) {this->Position = Position;}
	void setDirection(glm::vec3 const & Direction) {this->Direction = Direction;}
	void setCutOff(float const & CutOff) {this->CutOff = CutOff;}
	void setColor(glm::vec3 const & Color) {this->Color = Color;}

private:
	spot();
	~spot();

	glm::vec3 Position;
	glm::vec3 Direction;
	float CutOff;
	glm::vec3 Color;
};

#endif //LIGHT_SPOT_INCLUDED

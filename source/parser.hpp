#pragma once

#include "tinyxml.h"
#include "util.hpp"
#include "material.hpp"
#include <array>

class parser
{
public:
	parser(std::string const& Filename);
	~parser();

private:
	void parseRaytracer(TiXmlElement* pElement);
	void parseObjects(TiXmlElement* pElement);
	void parseTriangle(TiXmlElement* pElement);
	void parsePlane(TiXmlElement* pElement);
	void parseSphere(TiXmlElement* pElement);
	void parseCylinder(TiXmlElement* pElement);
	std::array<glm::vec3, 3> getPositions(TiXmlElement* pElement);
	material getMaterial(TiXmlElement* pElement);
	glm::vec4 getColorMaterial(TiXmlElement* pElement);
	glm::mat4 getTransforms(TiXmlElement* pElement);
	glm::mat4 getTransform(TiXmlElement* pElement);
	void parseLights(TiXmlElement* pElement);
	void parseLightSpots(TiXmlElement* pElement);
	void parseLightSpot(TiXmlElement* pElement);
	void parseLightPoints(TiXmlElement* pElement);
	void parseLightPoint(TiXmlElement* pElement);
	void parseLightDirections(TiXmlElement* pElement);
	void parseLightDirection(TiXmlElement* pElement);
	glm::vec3 get_position(TiXmlElement* pElement);
	glm::vec3 get_direction(TiXmlElement* pElement);
	glm::vec3 get_color(TiXmlElement* pElement);
};


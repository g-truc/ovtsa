#ifndef PARSER_INCLUDED
#define PARSER_INCLUDED

#include "tinyxml.h"
#include "util.hpp"
#include "material.hpp"

class parser
{
public:
	parser(std::string const & Filename);
    ~parser();

private:
    void RayTracer(TiXmlElement* pElement);
    void Objects(TiXmlElement* pElement);
    void Triangle(TiXmlElement* pElement);
    void Plane(TiXmlElement* pElement);
    void Sphere(TiXmlElement* pElement);
    void Cylinder(TiXmlElement* pElement);
    std::vector<glm::vec3> Positions(TiXmlElement* pElement);
    material Material(TiXmlElement* pElement);
    glm::vec4 ColorMaterial(TiXmlElement* pElement);
    glm::mat4 Transforms(TiXmlElement* pElement);
    glm::mat4 Transform(TiXmlElement* pElement);
    void Lights(TiXmlElement* pElement);
    void LightSpots(TiXmlElement* pElement);
    void LightSpot(TiXmlElement* pElement);
    void LightPoints(TiXmlElement* pElement);
    void LightPoint(TiXmlElement* pElement);
    void LightDirections(TiXmlElement* pElement);
    void LightDirection(TiXmlElement* pElement);
    glm::vec3 Position(TiXmlElement* pElement);
    glm::vec3 Direction(TiXmlElement* pElement);
    glm::vec3 Color(TiXmlElement* pElement);
};

#endif //PARSER_INCLUDED

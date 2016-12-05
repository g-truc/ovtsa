#include "parser.hpp"
#include "config.hpp"
#include "object.hpp"
#include "object_factory.hpp"
#include "shape_polyhedron.hpp"
#include "light_point.hpp"
#include "light_spot.hpp"
#include "light_directional.hpp"
#include "light_factory.hpp"
#include "config.hpp"

parser::parser(std::string const & Filename)
{
	TiXmlDocument Document(Filename.c_str());
	if(!Document.LoadFile())
	{
		assert(0);
		return;
	}
	TiXmlElement* pRoot = Document.FirstChild("raytrace")->ToElement();
	parseRaytracer(pRoot);
}

parser::~parser()
{}

void parser::parseRaytracer(TiXmlElement * pElement)
{
	config * pConfig = & config::instance();
	std::string AAType;
	glm::ivec2 WindowSize(0);

	TiXmlAttribute* pAttribute = pElement->FirstAttribute();
	do
	{
		if(!strcmp("file", pAttribute->Name()))
			pConfig->SetFile(pAttribute->Value());
		else if(!strcmp("width", pAttribute->Name()))
			WindowSize.x = atoi(pAttribute->Value());
		else if(!strcmp("height", pAttribute->Name()))
			WindowSize.y = atoi(pAttribute->Value());
		else if(!strcmp("depth", pAttribute->Name()))
			pConfig->SetDepth(atoi(pAttribute->Value()));
		else if(!strcmp("antialiasing", pAttribute->Name()))
			pConfig->SetAntiAliasingLevel(atoi(pAttribute->Value()));
		else if(!strcmp("aa-accuracy", pAttribute->Name()))
			pConfig->SetAntiAliasingAccuracy(float(atof(pAttribute->Value())));
		else if(!strcmp("aa-type", pAttribute->Name()))
			AAType = pAttribute->Value();
		else if(!strcmp("reflection-rays", pAttribute->Name()))
			pConfig->SetReflectionRays(atoi(pAttribute->Value()));
		else if(!strcmp("refraction-rays", pAttribute->Name()))
			pConfig->SetRefractionRays(atoi(pAttribute->Value()));
		else if(!strcmp("reflection-accuracy", pAttribute->Name()))
			pConfig->SetReflectionAccuracy(float(atof(pAttribute->Value())));
		else if(!strcmp("refraction-accuracy", pAttribute->Name()))
			pConfig->SetReflactionAccuracy(float(atof(pAttribute->Value())));
	}    
	while (pAttribute = pAttribute->Next());

	if(!strcmp("none", AAType.c_str()))
		pConfig->SetAntiAliasingType(AA_NONE);
	else if(!strcmp("adapt", AAType.c_str()))
		pConfig->SetAntiAliasingType(AA_ADAPT);
	else if(!strcmp("force", AAType.c_str()))
		pConfig->SetAntiAliasingType(AA_FORCE);


	pConfig->SetWindowSize(WindowSize);

	parseObjects(pElement->FirstChildElement("objects"));
	parseLights(pElement->FirstChildElement("lights"));
}

void parser::parseObjects(TiXmlElement* pElement)
{
	TiXmlElement* pChild = 0;

	pChild = pElement->FirstChildElement("plane");
	if(pChild)
	{
		do
		{
			parsePlane(pChild);
		}
		while(pChild = pChild->NextSiblingElement("plane"));
	}

	pChild = pElement->FirstChildElement("sphere");
	if(pChild)
	{
		do
		{
			parseSphere(pChild);
		}
		while(pChild = pChild->NextSiblingElement("sphere"));
	}

	pChild = pElement->FirstChildElement("cylinder");
	if(pChild)
	{
		do
		{
			parseCylinder(pChild);
		}
		while(pChild = pChild->NextSiblingElement("cylinder"));
	}

	pChild = pElement->FirstChildElement("triangle");
	if(pChild)
	{
		do
		{
			parseTriangle(pChild);
		}
		while(pChild = pChild->NextSiblingElement("triangle"));
	}
}

void parser::parsePlane(TiXmlElement* pElement)
{
	material Material;
	glm::mat4 Transform;

	TiXmlElement* pChildMaterial = pElement->FirstChildElement("material");
	if(pChildMaterial)
		Material = this->getMaterial(pChildMaterial);

	TiXmlElement* pChildTransform = pElement->FirstChildElement("transforms");
	if(pChildTransform)
		Transform = this->getTransforms(pChildTransform);

	object::create(shape::PLANE, Material, Transform);
}

void parser::parseSphere(TiXmlElement* pElement)
{
	material Material;
	glm::mat4 Transform;

	TiXmlElement* pChildMaterial = pElement->FirstChildElement("material");
	if(pChildMaterial)
		Material = this->getMaterial(pChildMaterial);

	TiXmlElement* pChildTransform = pElement->FirstChildElement("transforms");
	if(pChildTransform)
		Transform = this->getTransforms(pChildTransform);

	object::create(shape::SPHERE, Material, Transform);
}

void parser::parseTriangle(TiXmlElement* pElement)
{
	std::vector<glm::vec3> Positions;
	material Material;
	glm::mat4 Transform;

	TiXmlElement* pChildMaterial = pElement->FirstChildElement("material");
	if(pChildMaterial)
		Material = this->getMaterial(pChildMaterial);

	TiXmlElement* pChildTransform = pElement->FirstChildElement("transforms");
	if(pChildTransform)
		Transform = this->getTransforms(pChildTransform);

	object * pObject = object::create(shape::TRIANGLE, Material, Transform);

	TiXmlElement* pChildPositions = pElement->FirstChildElement("positions");
	if(pChildPositions)
		static_cast<triangle*>(pObject->getShape())->set_positions(this->getPositions(pChildPositions));
}

void parser::parseCylinder(TiXmlElement* pElement)
{
	material Material;
	glm::mat4 Transform;

	TiXmlElement* pChildMaterial = pElement->FirstChildElement("material");
	if(pChildMaterial)
		Material = this->getMaterial(pChildMaterial);

	TiXmlElement* pChildTransform = pElement->FirstChildElement("transforms");
	if(pChildTransform)
		Transform = this->getTransforms(pChildTransform);

	object::create(shape::CYLINDER, Material, Transform);
}

std::array<glm::vec3, 3> parser::getPositions(TiXmlElement* pElement)
{
	std::array<glm::vec3, 3> Positions;

	TiXmlElement* pChild = pElement->FirstChildElement("position");
	Positions[0] = getPosition(pChild);
	Positions[1] = getPosition(pChild);
	Positions[2] = getPosition(pChild);

	return Positions;
}

material parser::getMaterial(TiXmlElement* pElement)
{
	material Material;
	std::string type;

	TiXmlAttribute* pAttribute = pElement->FirstAttribute();
	do
	{
		if(!strcmp("opacity", pAttribute->Name()))
			Material.setOpacity(float(atof(pAttribute->Value())));
		else if(!strcmp("reflection", pAttribute->Name()))
			Material.setReflectionFactor(float(atof(pAttribute->Value())));
		else if(!strcmp("refraction", pAttribute->Name()))
			Material.setRefractionFactor(float(atof(pAttribute->Value())));
		else if(!strcmp("environment-index", pAttribute->Name()))
			Material.setEnvironmentIndex(float(atof(pAttribute->Value())));
		else if(!strcmp("specular-exponent", pAttribute->Name()))
			Material.setSpecularExponent(float(atof(pAttribute->Value())));
		else if(!strcmp("repeat", pAttribute->Name()))
			Material.setRepeat(float(atof(pAttribute->Value())));
		else if(!strcmp("type", pAttribute->Name()))
			type = pAttribute->Value();
	}
		while (pAttribute = pAttribute->Next());

	if(!strcmp("flat", type.c_str()))
		Material.setType(material::MATERIAL_FLAT);
	else if(!strcmp("grid", type.c_str()))
		Material.setType(material::MATERIAL_GRID);
	else if(!strcmp("line-x", type.c_str()))
		Material.setType(material::MATERIAL_LINE_X);
	else if(!strcmp("line-y", type.c_str()))
		Material.setType(material::MATERIAL_LINE_Y);
	else if(!strcmp("line-z", type.c_str()))
		Material.setType(material::MATERIAL_LINE_Z);
	else if(!strcmp("perlin", type.c_str()))
		Material.setType(material::MATERIAL_PERLIN);
	else if(!strcmp("marble", type.c_str()))
		Material.setType(material::MATERIAL_MARBLE);
	else if(!strcmp("wood", type.c_str()))
		Material.setType(material::MATERIAL_WOOD);
	else
		Material.setType(material::MATERIAL_FLAT);

	glm::vec4 Ambient = getColorMaterial(pElement->FirstChildElement("ambient"));
	Material.setAmbient(glm::vec3(Ambient), Ambient.a);

	TiXmlElement* pAmbientSecondary = pElement->FirstChildElement("ambient-secondary");
	if(pAmbientSecondary)
	{
		glm::vec4 AmbientSecondary = getColorMaterial(pAmbientSecondary);
		Material.setAmbientSecondary(glm::vec3(AmbientSecondary), AmbientSecondary.a);
	}

	glm::vec4 Diffuse = getColorMaterial(pElement->FirstChildElement("diffuse"));
	Material.setDiffuse(glm::vec3(Diffuse), Diffuse.a);

	glm::vec4 Specular = getColorMaterial(pElement->FirstChildElement("specular"));
	Material.setSpecular(glm::vec3(Specular), Specular.a);

	return Material;
}

glm::vec4 parser::getColorMaterial(TiXmlElement* pElement)
{
	glm::vec4 Color;

	TiXmlAttribute* pAttribute = pElement->FirstAttribute();
	do
	{
		if(!strcmp("r", pAttribute->Name()))
			Color.r = float(atof(pAttribute->Value()));
		else if(!strcmp("g", pAttribute->Name()))
			Color.g = float(atof(pAttribute->Value()));
		else if(!strcmp("b", pAttribute->Name()))
			Color.b = float(atof(pAttribute->Value()));
		else if(!strcmp("a", pAttribute->Name()))
			Color.a = float(atof(pAttribute->Value()));
	}    
	while (pAttribute = pAttribute->Next());

	return Color;
}

glm::mat4 parser::getTransforms(TiXmlElement* pElement)
{
	glm::mat4 Transform = glm::mat4(1.0f);

	TiXmlElement* pChild = pElement->FirstChildElement();
	do
	{
		Transform *= this->getTransform(pChild);
	}
	while(pChild = pChild->NextSiblingElement());

	return Transform;
}

glm::mat4 parser::getTransform(TiXmlElement* pElement)
{
	float value = 0.0f;
	std::string type;

	TiXmlAttribute* pAttribute = pElement->FirstAttribute();
	do
	{
		if(!strcmp("value", pAttribute->Name()))
			value = float(atof(pAttribute->Value()));
		if(!strcmp("type", pAttribute->Name()))
			type = pAttribute->Value();
	}    
	while(pAttribute = pAttribute->Next());

	if(!strcmp("identity", type.c_str()))
		return glm::mat4(1.0f);
	else if(!strcmp("translate-x", type.c_str()))
		return glm::translate(glm::mat4(1.0f), glm::vec3(value, 0.0f, 0.0f));
	else if(!strcmp("translate-y", type.c_str()))
		return glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, value, 0.0f));
	else if(!strcmp("translate-z", type.c_str()))
		return glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, value));
	else if(!strcmp("scale-x", type.c_str()))
		return glm::scale(glm::mat4(1.0f), glm::vec3(value, 1.0f, 1.0f));
	else if(!strcmp("scale-y", type.c_str()))
		return glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, value, 1.0f));
	else if(!strcmp("scale-z", type.c_str()))
		return glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, value));
	else if(!strcmp("rotate-x", type.c_str()))
		return glm::rotate(glm::mat4(1.0f), glm::radians(value), glm::vec3(1.0f, 0.0f, 0.0f));
	else if(!strcmp("rotate-y", type.c_str()))
		return glm::rotate(glm::mat4(1.0f), glm::radians(value), glm::vec3(0.0f, 1.0f, 0.0f));
	else if(!strcmp("rotate-z", type.c_str()))
		return glm::rotate(glm::mat4(1.0f), glm::radians(value), glm::vec3(0.0f, 0.0f, 1.0f));
	return glm::mat4(1.0f);
}

void parser::parseLights(TiXmlElement* pElement)
{
	TiXmlElement* pChildSpots = pElement->FirstChildElement("light-spots");
	if(pChildSpots)
		parseLightSpots(pChildSpots);

	TiXmlElement* pChildDirections = pElement->FirstChildElement("light-directions");
	if(pChildDirections)
		parseLightDirections(pChildDirections);

	TiXmlElement* pChildPoints = pElement->FirstChildElement("light-points");
	if(pChildPoints)
		parseLightPoints(pChildPoints);
}

void parser::parseLightSpots(TiXmlElement* pElement)
{
	TiXmlElement* pChild = pElement->FirstChildElement("light-spot");
	do
	{
		parseLightSpot(pChild);
	}
	while(pChild = pChild->NextSiblingElement());
}

void parser::parseLightSpot(TiXmlElement* pElement)
{
	spot * pLight = spot::create();

	TiXmlAttribute* pAttribute = pElement->FirstAttribute();
	do
	{
		if(!strcmp("rays-number", pAttribute->Name()))
			pLight->setRayNumber(atoi(pAttribute->Value()));
		else if(!strcmp("inaccuracy", pAttribute->Name()))
			pLight->setInaccuracy(float(atof(pAttribute->Value())));
		else if(!strcmp("cutoff", pAttribute->Name()))
			pLight->setCutOff(float(atof(pAttribute->Value())));
	}
	while(pAttribute = pAttribute->Next());

	pLight->setPosition(getPosition(pElement->FirstChildElement("position")));
	pLight->setDirection(getDirection(pElement->FirstChildElement("direction")));
	pLight->setColor(getColor(pElement->FirstChildElement("color")));
}

void parser::parseLightPoints(TiXmlElement* pElement)
{
	TiXmlElement* pChild = pElement->FirstChildElement("light-point");
	do
	{
		parseLightPoint(pChild);
	}
	while(pChild = pChild->NextSiblingElement());
}

void parser::parseLightPoint(TiXmlElement* pElement)
{
	point* pLight = point::create();

	TiXmlAttribute* pAttribute = pElement->FirstAttribute();
	do
	{
		if(!strcmp("rays-number", pAttribute->Name()))
			pLight->setRayNumber(atoi(pAttribute->Value()));
		else if(!strcmp("inaccuracy", pAttribute->Name()))
			pLight->setInaccuracy(float(atof(pAttribute->Value())));
	}
	while(pAttribute = pAttribute->Next());

	pLight->setPosition(getPosition(pElement->FirstChildElement("position")));
	pLight->setColor(getColor(pElement->FirstChildElement("color")));
}

void parser::parseLightDirections(TiXmlElement* pElement)
{
	TiXmlElement* pChild = pElement->FirstChildElement("light-direction");
	do
	{
		parseLightDirection(pChild);
	}
	while(pChild = pChild->NextSiblingElement());
}

void parser::parseLightDirection(TiXmlElement* pElement)
{
	directional* pLight = directional::create();

	TiXmlAttribute* pAttribute = pElement->FirstAttribute();
	do
	{
		if(!strcmp("rays-number", pAttribute->Name()))
			pLight->setRayNumber(atoi(pAttribute->Value()));
		else if(!strcmp("inaccuracy", pAttribute->Name()))
			pLight->setInaccuracy(float(atof(pAttribute->Value())));
	}
	while(pAttribute = pAttribute->Next());

	pLight->setDirection(getDirection(pElement->FirstChildElement("direction")));
	pLight->setColor(getColor(pElement->FirstChildElement("color")));
}

glm::vec3 parser::getPosition(TiXmlElement* pElement)
{
	glm::vec3 Position;
	TiXmlAttribute* pAttribute = pElement->FirstAttribute();
	do
	{
		if(!strcmp("x", pAttribute->Name()))
			Position.x = float(atof(pAttribute->Value()));
		else if(!strcmp("y", pAttribute->Name()))
			Position.y = float(atof(pAttribute->Value()));
		else if(!strcmp("z", pAttribute->Name()))
			Position.z = float(atof(pAttribute->Value()));
	}    
	while(pAttribute = pAttribute->Next());
	return Position;
}

glm::vec3 parser::getDirection(TiXmlElement* pElement)
{
	glm::vec3 Direction;
	TiXmlAttribute* pAttribute = pElement->FirstAttribute();
	do
	{
		if(!strcmp("x", pAttribute->Name()))
			Direction.x = float(atof(pAttribute->Value()));
		else if(!strcmp("y", pAttribute->Name()))
			Direction.y = float(atof(pAttribute->Value()));
		else if(!strcmp("z", pAttribute->Name()))
			Direction.z = float(atof(pAttribute->Value()));
	}
	while(pAttribute = pAttribute->Next());
	return Direction;
}

glm::vec3 parser::getColor(TiXmlElement* pElement)
{
	glm::vec3 Color;
	float Alpha;
	TiXmlAttribute* pAttribute = pElement->FirstAttribute();
	do
	{
		if(!strcmp("r", pAttribute->Name()))
			Color.r = float(atof(pAttribute->Value()));
		else if(!strcmp("g", pAttribute->Name()))
			Color.g = float(atof(pAttribute->Value()));
		else if(!strcmp("b", pAttribute->Name()))
			Color.b = float(atof(pAttribute->Value()));
		else if(!strcmp("a", pAttribute->Name()))
			Alpha = float(atof(pAttribute->Value()));
	}
	while(pAttribute = pAttribute->Next());
	return Color * Alpha;
}

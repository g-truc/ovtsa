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
		return;
    TiXmlElement* pRoot = Document.FirstChild("raytrace")->ToElement();
    RayTracer(pRoot);
}

parser::~parser()
{}

void parser::RayTracer(TiXmlElement * pElement)
{
    config * pConfig = &config::instance();
    std::string AAType;

    TiXmlAttribute* pAttribute = pElement->FirstAttribute();
    do
    {
        if(!strcmp("file", pAttribute->Name()))
            pConfig->File() = std::string(pAttribute->Value());
        else if(!strcmp("width", pAttribute->Name()))
            pConfig->WindowWidth() = atoi(pAttribute->Value());
        else if(!strcmp("height", pAttribute->Name()))
            pConfig->WindowHeight() = atoi(pAttribute->Value());
        else if(!strcmp("depth", pAttribute->Name()))
            pConfig->Depth() = atoi(pAttribute->Value());
        else if(!strcmp("antialiasing", pAttribute->Name()))
            pConfig->AntiAliasingLevel() = atoi(pAttribute->Value());
        else if(!strcmp("aa-accuracy", pAttribute->Name()))
            pConfig->AntiAliasingAccuracy() = float(atof(pAttribute->Value()));
        else if(!strcmp("aa-type", pAttribute->Name()))
            AAType = pAttribute->Value();
        else if(!strcmp("reflection-rays", pAttribute->Name()))
            pConfig->ReflectionRays() = atoi(pAttribute->Value());
        else if(!strcmp("refraction-rays", pAttribute->Name()))
            pConfig->RefractionRays() = atoi(pAttribute->Value());
        else if(!strcmp("reflection-accuracy", pAttribute->Name()))
            pConfig->ReflectionAccuracy() = float(atof(pAttribute->Value()));
        else if(!strcmp("refraction-accuracy", pAttribute->Name()))
            pConfig->ReflactionAccuracy() = float(atof(pAttribute->Value()));
    }    
    while (pAttribute = pAttribute->Next());

    if(!strcmp("none", AAType.c_str()))
        pConfig->AntiAliasingType() = AA_NONE;
    else if(!strcmp("adapt", AAType.c_str()))
        pConfig->AntiAliasingType() = AA_ADAPT;
    else if(!strcmp("force", AAType.c_str()))
        pConfig->AntiAliasingType() = AA_FORCE;

    Objects(pElement->FirstChildElement("objects"));
    Lights(pElement->FirstChildElement("lights"));
}

void parser::Objects(TiXmlElement* pElement)
{
    TiXmlElement* pChild = 0;

    pChild = pElement->FirstChildElement("plane");
    if(pChild)
    {
        do
        {
            Plane(pChild);
        }
        while(pChild = pChild->NextSiblingElement("plane"));
    }

    pChild = pElement->FirstChildElement("sphere");
    if(pChild)
    {
        do
        {
            Sphere(pChild);
        }
        while(pChild = pChild->NextSiblingElement("sphere"));
    }

    pChild = pElement->FirstChildElement("cylinder");
    if(pChild)
    {
        do
        {
            Cylinder(pChild);
        }
        while(pChild = pChild->NextSiblingElement("cylinder"));
    }

    pChild = pElement->FirstChildElement("triangle");
    if(pChild)
    {
        do
        {
            Triangle(pChild);
        }
        while(pChild = pChild->NextSiblingElement("triangle"));
    }
}

void parser::Plane(TiXmlElement* pElement)
{
    material Material;
    glm::mat4 Transform;

    TiXmlElement* pChildMaterial = pElement->FirstChildElement("material");
    if(pChildMaterial)
        Material = this->Material(pChildMaterial);

    TiXmlElement* pChildTransform = pElement->FirstChildElement("transforms");
    if(pChildTransform)
        Transform = this->Transforms(pChildTransform);

    object::create(shape::PLANE, Material, Transform);
}

void parser::Sphere(TiXmlElement* pElement)
{
    material Material;
    glm::mat4 Transform;

    TiXmlElement* pChildMaterial = pElement->FirstChildElement("material");
    if(pChildMaterial)
        Material = this->Material(pChildMaterial);

    TiXmlElement* pChildTransform = pElement->FirstChildElement("transforms");
    if(pChildTransform)
        Transform = this->Transforms(pChildTransform);

    object::create(shape::SPHERE, Material, Transform);
}

void parser::Triangle(TiXmlElement* pElement)
{
    std::vector<glm::vec3> Positions;
    material Material;
    glm::mat4 Transform;

    TiXmlElement* pChildMaterial = pElement->FirstChildElement("material");
    if(pChildMaterial)
        Material = this->Material(pChildMaterial);

    TiXmlElement* pChildTransform = pElement->FirstChildElement("transforms");
    if(pChildTransform)
        Transform = this->Transforms(pChildTransform);

    object * pObject = object::create(shape::TRIANGLE, Material, Transform);

    TiXmlElement* pChildPositions = pElement->FirstChildElement("positions");
    if(pChildPositions)
        static_cast<triangle*>(pObject->getShape())->Positions(this->Positions(pChildPositions));
}

void parser::Cylinder(TiXmlElement* pElement)
{
    material Material;
    glm::mat4 Transform;

    TiXmlElement* pChildMaterial = pElement->FirstChildElement("material");
    if(pChildMaterial)
        Material = this->Material(pChildMaterial);

    TiXmlElement* pChildTransform = pElement->FirstChildElement("transforms");
    if(pChildTransform)
        Transform = this->Transforms(pChildTransform);

    object::create(shape::CYLINDER, Material, Transform);
}

std::vector<glm::vec3> parser::Positions(TiXmlElement* pElement)
{
    std::vector<glm::vec3> Positions;

    TiXmlElement* pChild = pElement->FirstChildElement("position");
    do
    {
        Positions.push_back(Position(pChild));
    }
    while(pChild = pChild->NextSiblingElement());
   
    return Positions;
}

material parser::Material(TiXmlElement* pElement)
{
    material Material;
    std::string type;

    TiXmlAttribute* pAttribute = pElement->FirstAttribute();
    do
    {
 		if(!strcmp("opacity", pAttribute->Name()))
			Material.setOpacity(float(atof(pAttribute->Value())));
        else if(!strcmp("reflection", pAttribute->Name()))
            Material.ReflectionFactor(float(atof(pAttribute->Value())));
        else if(!strcmp("refraction", pAttribute->Name()))
            Material.RefractionFactor(float(atof(pAttribute->Value())));
        else if(!strcmp("environment-index", pAttribute->Name()))
            Material.EnvironmentIndex(float(atof(pAttribute->Value())));
        else if(!strcmp("specular-exponent", pAttribute->Name()))
            Material.SpecularExponent(float(atof(pAttribute->Value())));
        else if(!strcmp("repeat", pAttribute->Name()))
            Material.Repeat(float(atof(pAttribute->Value())));
        else if(!strcmp("type", pAttribute->Name()))
            type = pAttribute->Value();
    }    
    while (pAttribute = pAttribute->Next());

    if(!strcmp("flat", type.c_str()))
		Material.Type(material::MATERIAL_FLAT);
    else if(!strcmp("grid", type.c_str()))
        Material.Type(material::MATERIAL_GRID);
    else if(!strcmp("line-x", type.c_str()))
        Material.Type(material::MATERIAL_LINE_X);
    else if(!strcmp("line-y", type.c_str()))
        Material.Type(material::MATERIAL_LINE_Y);
    else if(!strcmp("line-z", type.c_str()))
        Material.Type(material::MATERIAL_LINE_Z);
    else if(!strcmp("noise", type.c_str()))
        Material.Type(material::MATERIAL_NOISE);
    else if(!strcmp("perlin", type.c_str()))
        Material.Type(material::MATERIAL_PERLIN);
    else if(!strcmp("marble", type.c_str()))
        Material.Type(material::MATERIAL_MARBLE);
    else if(!strcmp("wood", type.c_str()))
        Material.Type(material::MATERIAL_WOOD);
    else
        Material.Type(material::MATERIAL_FLAT);

    glm::vec4 Ambient = ColorMaterial(pElement->FirstChildElement("ambient"));
    Material.Ambient(glm::vec3(Ambient), Ambient.a);

    TiXmlElement* pAmbientSecondary = pElement->FirstChildElement("ambient-secondary");
    if(pAmbientSecondary)
    {
        glm::vec4 AmbientSecondary = ColorMaterial(pAmbientSecondary);
        Material.AmbientSecondary(glm::vec3(AmbientSecondary), AmbientSecondary.a);
    }

    glm::vec4 Diffuse = ColorMaterial(pElement->FirstChildElement("diffuse"));
    Material.Diffuse(glm::vec3(Diffuse), Diffuse.a);

    glm::vec4 Specular = ColorMaterial(pElement->FirstChildElement("specular"));
    Material.Specular(glm::vec3(Specular), Specular.a);

    return Material;
}

glm::vec4 parser::ColorMaterial(TiXmlElement* pElement)
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

glm::mat4 parser::Transforms(TiXmlElement* pElement)
{
    glm::mat4 Transform = glm::mat4(1.0f);

    TiXmlElement* pChild = pElement->FirstChildElement();
    do
    {
        Transform *= this->Transform(pChild);
    }
    while(pChild = pChild->NextSiblingElement());

    return Transform;
}

glm::mat4 parser::Transform(TiXmlElement* pElement)
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
        return glm::translate(glm::mat4(1.0f), value, 0.0f, 0.0f);
    else if(!strcmp("translate-y", type.c_str()))
        return glm::translate(glm::mat4(1.0f), 0.0f, value, 0.0f);
    else if(!strcmp("translate-z", type.c_str()))
        return glm::translate(glm::mat4(1.0f), 0.0f, 0.0f, value);
    else if(!strcmp("scale-x", type.c_str()))
        return glm::scale(glm::mat4(1.0f), value, 1.0f, 1.0f);
    else if(!strcmp("scale-y", type.c_str()))
        return glm::scale(glm::mat4(1.0f), 1.0f, value, 1.0f);
    else if(!strcmp("scale-z", type.c_str()))
        return glm::scale(glm::mat4(1.0f), 1.0f, 1.0f, value);
    else if(!strcmp("rotate-x", type.c_str()))
        return glm::rotate(glm::mat4(1.0f), value, 1.0f, 0.0f, 0.0f);
    else if(!strcmp("rotate-y", type.c_str()))
        return glm::rotate(glm::mat4(1.0f), value, 0.0f, 1.0f, 0.0f);
    else if(!strcmp("rotate-z", type.c_str()))
        return glm::rotate(glm::mat4(1.0f), value, 0.0f, 0.0f, 1.0f);
    return glm::mat4(1.0f);
}

void parser::Lights(TiXmlElement* pElement)
{
    TiXmlElement* pChildSpots = pElement->FirstChildElement("light-spots");
    if(pChildSpots)
        LightSpots(pChildSpots);

    TiXmlElement* pChildDirections = pElement->FirstChildElement("light-directions");
    if(pChildDirections)
        LightDirections(pChildDirections);

    TiXmlElement* pChildPoints = pElement->FirstChildElement("light-points");
    if(pChildPoints)
        LightPoints(pChildPoints);
}

void parser::LightSpots(TiXmlElement* pElement)
{
    TiXmlElement* pChild = pElement->FirstChildElement("light-spot");
    do
    {
        LightSpot(pChild);
    }
    while(pChild = pChild->NextSiblingElement());
}

void parser::LightSpot(TiXmlElement* pElement)
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

    pLight->setPosition(Position(pElement->FirstChildElement("position")));
    pLight->setDirection(Direction(pElement->FirstChildElement("direction")));
    pLight->setColor(Color(pElement->FirstChildElement("color")));
}

void parser::LightPoints(TiXmlElement* pElement)
{
    TiXmlElement* pChild = pElement->FirstChildElement("light-point");
    do
    {
        LightPoint(pChild);
    }
    while(pChild = pChild->NextSiblingElement());
}

void parser::LightPoint(TiXmlElement* pElement)
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

    pLight->setPosition(Position(pElement->FirstChildElement("position")));
    pLight->setColor(Color(pElement->FirstChildElement("color")));
}

void parser::LightDirections(TiXmlElement* pElement)
{
    TiXmlElement* pChild = pElement->FirstChildElement("light-direction");
    do
    {
        LightDirection(pChild);
    }
    while(pChild = pChild->NextSiblingElement());
}

void parser::LightDirection(TiXmlElement* pElement)
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

    pLight->setDirection(Direction(pElement->FirstChildElement("direction")));
    pLight->setColor(Color(pElement->FirstChildElement("color")));
}

glm::vec3 parser::Position(TiXmlElement* pElement)
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

glm::vec3 parser::Direction(TiXmlElement* pElement)
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

glm::vec3 parser::Color(TiXmlElement* pElement)
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

#include "object.hpp"
#include "object_factory.hpp"
#include "shape_plane.hpp"
#include "shape_sphere.hpp"
#include "shape_cylinder.hpp"
#include "shape_polyhedron.hpp"

object* object::create
(
	shape::type const & Shape, 
	material const & Material, 
	glm::mat4 const & Transform
)
{
    object * Object = objectFactory::instance().create();
    switch(Shape)
    {
    case shape::CONE:
        break;
    case shape::CUBE:
        break;
    case shape::PLANE:
        Object->Shape = new plane;
        break;
    case shape::TRIANGLE:
        Object->Shape = new triangle;
        break;
    case shape::SPHERE:
        Object->Shape = new sphere;
        break;
    case shape::CYLINDER:
        Object->Shape = new cylinder;
        break;
    case shape::TORUS:
        break;
    }

    Object->Material = Material;
    Object->Transform = Transform;
    return Object;
}

object::object() :
	Shape(0)
{}

object::~object()
{
    if(!Shape)
	{
        delete Shape;
		Shape = 0;
	}
}

object::object
(
	shape * const Shape, 
	material const & Material, 
	glm::mat4 const & Transform
)
{
    this->Shape = Shape;
    this->Transform = transform(Transform);
    this->Material = Material;
}

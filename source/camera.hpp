#ifndef CAMERA_INCLUDED
#define CAMERA_INCLUDED

#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "timer.hpp"
#include "surface.hpp"
#include "adaptator.hpp"

class camera
{
public:
	enum action
	{
		PLUS,
		MINUS
	};

public:
	camera();
	~camera();

	void shoot(
		int Depth, 
		int Antialising, 
		glm::uvec2 const & WindowSize);

	void rotateX(action const & Action);
	void rotateZ(action const & Action);
	void move(action const & Action);

private:
	bool checkAliasing(
		surface const & Surface, 
		adaptator const & Adaptator, 
		int x, int y);
	void shootAntiAliasingAdaptative(
		glm::mat4 const & ModelView,
		int Depth, 
		int Antialising);
	void shootAntiAliasing(
		glm::mat4 const & ModelView,
		int Depth, 
		int Antialising);
	void shootAliasing(
		glm::mat4 const & ModelView,
		int Depth);

	glm::vec3 shade(
		intersection const & Intersection, 
		material const & Material, 
		glm::vec3 const & View);
	glm::vec3 trace(
		ray const & Ray, 
		int iDepth);
    
	//glm::mat4 ModelView;
	glm::uvec2 WindowSize;
	float MoveForward;
	float MoveUp;
	glm::vec3 Rotate;
};

#endif //CAMERA_INCLUDED



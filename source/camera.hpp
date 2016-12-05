#pragma once

#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "timer.hpp"
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

	void shoot(int Depth, int Antialising, glm::uvec2 const& WindowSize);

	void rotate_x(action Action);
	void rotate_z(action Action);
	void move(action Action);

private:
	bool check_aliasing(gli::texture2d const& Surface, adaptator const& Adaptator, int x, int y);

	void shoot_adaptative(glm::mat4 const& ModelView, int Depth, int Antialising);
	void shoot(glm::mat4 const& ModelView, int Depth, int Antialising);

	glm::vec3 shade(intersection const& Intersection, material const& Material, glm::vec3 const& View);
	glm::vec3 trace(ray const& Ray, int iDepth);

	glm::uvec2 WindowSize;
	float MoveForward;
	float MoveUp;
	glm::vec3 Rotate;
};




#pragma once

#include "util.hpp"

#define MAX_DISTANCE    10000.0f

class raytrace
{
public:
	raytrace(std::string const & Filename);
	~raytrace();

private:
	void render();
	void begin();
	void end();
};


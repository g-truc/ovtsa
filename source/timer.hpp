#pragma once

#include "util.hpp"

class timer 
{
public:
	timer();
	
	void init();
	float time() const;

private:
	float StartTime;
};


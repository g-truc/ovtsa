#include "timer.hpp"

timer::timer()
{

}

void timer::init()
{
	this->StartTime = clock() / 1000.f;
}

float timer::time() const
{
	return clock() / 1000.f - this->StartTime;
}

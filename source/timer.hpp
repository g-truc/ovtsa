#ifndef TIMER_INCLUDED
#define TIMER_INCLUDED

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

#endif //TIMER_INCLUDED

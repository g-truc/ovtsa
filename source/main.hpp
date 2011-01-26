#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED

#include "util.hpp"

#define MAX_DISTANCE    10000.0f

class raytrace
{
public:
	raytrace(std::string const & Filename);
	raytrace(std::string const & Filename, std::string const & Output);
    ~raytrace();

private:
    void render();
    void begin();
    void end();
};

#endif //MAIN_INCLUDED

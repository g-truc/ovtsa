#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED

#define MAX_DISTANCE    10000.0f

class raytrace
{
public:
	raytrace(char const * Filename);
	raytrace(char const * Filename, char const * Output);
	~raytrace();

private:
	void render();
	void begin();
	void end();
};

#endif //MAIN_INCLUDED

#include "main.hpp"
#include "camera.hpp"
#include "config.hpp"
#include "object_factory.hpp"
#include "light_factory.hpp"

raytrace::raytrace
(
	std::string const & Filename
)
{
	config::instance().load(Filename);

	this->begin();
	this->render();
	this->end();
}

raytrace::~raytrace()
{

}

void raytrace::begin()
{

}

void raytrace::render()
{
	config & Config = config::instance();
	timer Time;
	Time.init();

	printf("===========================\n");
	printf("processing...\n");

	clock_t TimeStart = clock();

	camera().shoot(
		Config.get_depth(), 
		Config.get_anti_aliasing_level(), 
		glm::uvec2(Config.get_window_size()));

	printf("Time: %d\n", clock() - TimeStart);

	printf("===========================\n");
	printf("Objects : %d\n", static_cast<int>(object_factory::instance().size()));
	printf("Lights : %d\n", static_cast<int>(light_factory::instance().size()));
	printf("Bounding count %d\n", Config.get_depth());
	printf("Antialiasing %dX\n", Config.get_anti_aliasing_level());
	printf("Reflection %dX\n", Config.get_reflection_rays());
	printf("Refraction %dX\n", Config.get_refraction_rays());
	printf("Image size: (%d, %d) pixels\n", Config.get_window_size().x, Config.get_window_size().y);

	system("pause");
}

void raytrace::end()
{
	light_factory::destroy();
	object_factory::destroy();
	config::destroy();
}

int main(int argc, char *argv[])
{
	printf("===========================\n");
	printf("Ovt'sa\n");

	if(argc > 1)
	{
		printf("Filename: %s\n", argv[1]);
		raytrace Raytrace(argv[1]);
	}
	else
	{
		raytrace Raytrace("../data/full.xml");
	}

	return 0;
}


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
		Config.getDepth(), 
		Config.getAntiAliasingLevel(), 
		glm::uvec2(Config.getWindowSize()));

	printf("Time: %d\n", clock() - TimeStart);

    printf("===========================\n");
    printf("Objects : %d\n", objectFactory::instance().size());
    printf("Lights : %d\n", lightFactory::instance().size());
    printf("Bounding count %d\n", Config.getDepth());
    printf("Antialiasing %dX\n", Config.getAntiAliasingLevel());
    printf("Reflection %dX\n", Config.getReflectionRays());
    printf("Refraction %dX\n", Config.getRefractionRays());
    printf("Image size: (%d, %d) pixels\n", Config.getWindowSize().x, Config.getWindowSize().y);

	system("pause");
}

void raytrace::end()
{
    lightFactory::destroy();
    objectFactory::destroy();
    config::destroy();
}

int main(int argc, char *argv[])
{
    printf("===========================\n");
	printf("G-Truc Creation's Raytracer\n");

    if(argc > 1)
    {
        printf("Filename: %s\n", argv[1]);
        raytrace Raytrace(argv[1]);
    }
    else
    {
        raytrace Raytrace("../../data/full.xml");
    }

    return 0;
}


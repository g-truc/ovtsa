#include "config.hpp"
#include "parser.hpp"

config::config()
	: AAAccuracy(0.1f)
	, AntiAliasing(1)
	, AAType(AA_NONE)
	, WindowSize(320, 240)
	, Depth(0)
	, ReflectionRays(1)
	, RefractionRays(1)
	, ReflectionAccuracy(0.0f)
	, ReflactionAccuracy(0.0f)
{}

config::~config()
{}

void config::load(std::string const & Filename)
{
	parser(Filename.c_str());
}

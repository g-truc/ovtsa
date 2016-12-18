#pragma once

#include "util.hpp"
#include "singleton.hpp"

enum anti_alias_mode
{
	AA_NONE,
	AA_ADAPT,
	AA_FORCE
};

class config : public singleton<config>
{
	friend class singleton<config>;
	config();
	~config();

public:
	void load(std::string const& Filename);

	glm::ivec2 get_window_size() const {return this->WindowSize;}
	int get_depth() const {return this->Depth;}
	int get_anti_aliasing_level() const {return this->AntiAliasing;}
	float get_anti_aliasing_accuracy() const {return this->AAAccuracy;}
	anti_alias_mode get_anti_aliasing_type() const {return this->AAType;}
	int get_reflection_rays() const {return this->ReflectionRays;}
	int get_refraction_rays() const {return this->RefractionRays;}
	float get_reflection_accuracy() const {return this->ReflectionAccuracy;}
	float get_reflaction_accuracy() const {return this->ReflactionAccuracy;}
	std::string get_file() const {return this->File;}
	std::string get_config() const {return this->Config;}

	void set_window_size(glm::ivec2 const & WindowSize) {this->WindowSize = WindowSize;}
	void set_depth(int Depth) {this->Depth = Depth;}
	void set_anti_aliasing_level(int AntiAliasing) {this->AntiAliasing = AntiAliasing;}
	void set_anti_aliasing_accuracy(float AAAccuracy) {this->AAAccuracy = AAAccuracy;}
	void set_anti_aliasing_type(anti_alias_mode AAType) {this->AAType = AAType;}
	void set_reflection_rays(int ReflectionRays) {this->ReflectionRays = ReflectionRays;}
	void set_refraction_rays(int RefractionRays) {this->RefractionRays = RefractionRays;}
	void set_reflection_accuracy(float ReflectionAccuracy) {this->ReflectionAccuracy = ReflectionAccuracy;}
	void set_reflaction_accuracy(float ReflactionAccuracy) {this->ReflactionAccuracy = ReflactionAccuracy;}
	void set_file(std::string const& File) {this->File = File;}
	void set_config(std::string const& Config) {this->Config = Config;}

private:
	glm::ivec2 WindowSize;
	int Depth;
	int AntiAliasing;
	float AAAccuracy;
	anti_alias_mode AAType;
	int ReflectionRays;
	int RefractionRays;
	float ReflectionAccuracy;
	float ReflactionAccuracy;
	std::string File;
	std::string Config;
};


#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

#include "singleton.hpp"
#include <glm/vec2.hpp>
#include <string>

enum EAntiAlias
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
	void load(std::string const & Filename);
	void load(std::string const & Filename, std::string const & output);

	glm::ivec2 getWindowSize() const {return this->WindowSize;}
	int getDepth() const {return this->Depth;}
	int getAntiAliasingLevel() const {return this->AntiAliasing;}
	float getAntiAliasingAccuracy() const {return this->AAAccuracy;}
	EAntiAlias getAntiAliasingType() const {return this->AAType;}
	int getReflectionRays() const {return this->ReflectionRays;}
	int getRefractionRays() const {return this->RefractionRays;}
	float getReflectionAccuracy() const {return this->ReflectionAccuracy;}
	float getReflactionAccuracy() const {return this->ReflactionAccuracy;}
	std::string getFile() const {return this->File;}
	std::string getConfig() const {return this->Config;}

	void SetWindowSize(glm::ivec2 const & WindowSize) {this->WindowSize = WindowSize;}
	void SetDepth(int Depth) {this->Depth = Depth;}
	void SetAntiAliasingLevel(int AntiAliasing) {this->AntiAliasing = AntiAliasing;}
	void SetAntiAliasingAccuracy(float AAAccuracy) {this->AAAccuracy = AAAccuracy;}
	void SetAntiAliasingType(EAntiAlias AAType) {this->AAType = AAType;}
	void SetReflectionRays(int ReflectionRays) {this->ReflectionRays = ReflectionRays;}
	void SetRefractionRays(int RefractionRays) {this->RefractionRays = RefractionRays;}
	void SetReflectionAccuracy(float ReflectionAccuracy) {this->ReflectionAccuracy = ReflectionAccuracy;}
	void SetReflactionAccuracy(float ReflactionAccuracy) {this->ReflactionAccuracy = ReflactionAccuracy;}
	void SetFile(std::string const & File) {this->File = File;}
	void SetConfig(std::string const & Config) {this->Config = Config;}

private:
	glm::ivec2 WindowSize;
	int Depth;
	int AntiAliasing;
	float AAAccuracy;
	EAntiAlias AAType;
	int ReflectionRays;
	int RefractionRays;
	float ReflectionAccuracy;
	float ReflactionAccuracy;
	std::string File;
	std::string Config;
};

#endif //CONFIG_INCLUDED

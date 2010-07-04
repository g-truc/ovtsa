#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

#include "util.hpp"
#include "singleton.hpp"

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

	int getWindowWidth() const {return m_iWindowWidth;}
    int getWindowHeight() const {return m_iWindowHeight;}
    int getDepth() const {return m_iDepth;}
    int getAntiAliasingLevel() const {return m_iAntiAliasing;}
    float getAntiAliasingAccuracy() const {return m_fAAAccuracy;}
    EAntiAlias getAntiAliasingType() const {return m_AAType;}
    int getReflectionRays() const {return m_iReflectionRays;}
    int getRefractionRays() const {return m_iRefractionRays;}
    float getReflectionAccuracy() const {return m_fReflectionAccuracy;}
    float getReflactionAccuracy() const {return m_fReflactionAccuracy;}
    std::string getFile() const {return m_File;}
    std::string getConfig() const {return m_Config;}

    int& WindowWidth() {return m_iWindowWidth;}
    int& WindowHeight() {return m_iWindowHeight;}
    int& Depth() {return m_iDepth;}
    int& AntiAliasingLevel() {return m_iAntiAliasing;}
    float& AntiAliasingAccuracy() {return m_fAAAccuracy;}
    EAntiAlias& AntiAliasingType() {return m_AAType;}
    int& ReflectionRays() {return m_iReflectionRays;}
    int& RefractionRays() {return m_iRefractionRays;}
    float& ReflectionAccuracy() {return m_fReflectionAccuracy;}
    float& ReflactionAccuracy() {return m_fReflactionAccuracy;}
    std::string& File() {return m_File;}
    std::string& Config() {return m_Config;}

private:
    int m_iWindowWidth;
    int m_iWindowHeight;
    int m_iDepth;
    int m_iAntiAliasing;
    float m_fAAAccuracy;
    EAntiAlias m_AAType;
    int m_iReflectionRays;
    int m_iRefractionRays;
    float m_fReflectionAccuracy;
    float m_fReflactionAccuracy;
    std::string m_File;
    std::string m_Config;
};

#endif //CONFIG_INCLUDED

#include "config.hpp"
#include "parser.hpp"

config::config() :
    m_fAAAccuracy(0.1f),
    m_iAntiAliasing(1),
    m_AAType(AA_NONE),
    m_iWindowWidth(320),
    m_iWindowHeight(240),
    m_iDepth(0),
    m_iReflectionRays(1),
    m_iRefractionRays(1),
    m_fReflectionAccuracy(0.0f),
    m_fReflactionAccuracy(0.0f)
{}

config::~config()
{}

void config::load(std::string const & Filename)
{
    parser(Filename.c_str());
}

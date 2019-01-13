#include "ApplicationSettings.h"

using namespace RED_LILIUM_NAMESPACE;

ApplicationSettings::ApplicationSettings(int argc, char** argv)
	: m_args(argv)
	, m_resourcesPath()
{

}

const std::string& ApplicationSettings::GetResourcesPath() const
{
	return m_resourcesPath;
}

const argh::parser& ApplicationSettings::GetArguments() const
{
	return m_args;
}

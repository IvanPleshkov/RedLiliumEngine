#pragma once

#include <Core/Common.h>
#include <argh.h>

namespace RED_LILIUM_NAMESPACE
{

class ApplicationSettings : public RedLiliumObject
{
public:
	ApplicationSettings(int argc, char** argv);
	~ApplicationSettings() override = default;

	const std::string& GetResourcesPath() const;
	const argh::parser& GetArguments() const;

private:
	argh::parser m_args;
	std::string m_resourcesPath;
};

}

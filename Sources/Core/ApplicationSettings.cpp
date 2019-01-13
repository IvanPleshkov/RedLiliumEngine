#include "pch.h"
#include "ApplicationSettings.h"

using namespace RED_LILIUM_NAMESPACE;

ApplicationSettings::ApplicationSettings(int argc, char** argv)
	: m_args(argv)
	, m_resourcesPath()
{
	RED_LILIUM_LOG_INFO("Init ApplicationSettings");
	RED_LILIUM_LOG_INFO(std::string("Current path: ") + std::filesystem::current_path().string());

	std::ifstream pathJsonFileStream("paths.json");
	std::string pathJsonData(
		(std::istreambuf_iterator<char>(pathJsonFileStream)),
		std::istreambuf_iterator<char>());

	if (!pathJsonData.empty())
	{
		json pathsJson = json::parse(pathJsonData);
		if (pathsJson.find("resourcesPath") != pathsJson.end())
		{
			m_resourcesPath = pathsJson["resourcesPath"].get<std::string>();
		}
		else
		{
			RED_LILIUM_LOG_ERROR("No resources path in paths.json");
		}
	}
	else
	{
		RED_LILIUM_LOG_ERROR("Not found paths.json");
	}
}

const std::string& ApplicationSettings::GetResourcesPath() const
{
	return m_resourcesPath;
}

const argh::parser& ApplicationSettings::GetArguments() const
{
	return m_args;
}

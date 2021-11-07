#include "resources_manager.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>

ResourcesManager::ResourcesManager()
{
    spdlog::info("Working path: " + std::filesystem::current_path().string());

	std::ifstream pathJsonFileStream("paths.json");
	std::string pathJsonData(
		(std::istreambuf_iterator<char>(pathJsonFileStream)),
		std::istreambuf_iterator<char>());

	if (!pathJsonData.empty())
	{
        nlohmann::json pathsJson = nlohmann::json::parse(pathJsonData);
		if (pathsJson.find("resourcesPath") != pathsJson.end())
		{
			_path = pathsJson["resourcesPath"].get<std::string>();
		}
		else
		{
			throw std::runtime_error("No resources path in paths.json");
		}
	}
	else
	{
        throw std::runtime_error("Not found paths.json");
	}
}

ResourcesManager::~ResourcesManager() { }

std::string ResourcesManager::readResourceData(std::string_view resource)
{
	std::stringstream ss;
	ss << _path << "/" << resource;
    std::ifstream fileStream(ss.str());
	if (fileStream.is_open())
	{
		std::string fileData(
			(std::istreambuf_iterator<char>(fileStream)),
			std::istreambuf_iterator<char>());
		return std::move(fileData);
	}
	else
	{
		return std::string();
	}
}

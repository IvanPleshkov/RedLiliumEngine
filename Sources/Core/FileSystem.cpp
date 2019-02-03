#include "pch.h"
#include "FileSystem.h"
#include "ApplicationSettings.h"

using namespace RED_LILIUM_NAMESPACE;

FileSystem::FileSystem(ptr<ApplicationSettings> applicationSettings)
	: RedLiliumObject()
	, m_applicationSettings(applicationSettings)
	, m_workingPath(applicationSettings->GetResourcesPath())
{
}

std::string FileSystem::ReadFile(const std::string& filename)
{
	const std::string fullFilename = m_workingPath + "\\" + filename;
	std::ifstream fileStream(fullFilename);
	std::string fileData(
		(std::istreambuf_iterator<char>(fileStream)),
		std::istreambuf_iterator<char>());
	return std::move(fileData);
}

json FileSystem::ReadJson(const std::string& filename)
{
	std::string fileData = ReadFile(filename);
	json j = json::parse(fileData);
	return std::move(j);
}

cv::Mat FileSystem::ReadTexture(const std::string& filename, int cvFlags)
{
	const std::string fullFilename = m_workingPath + "\\" + filename;
	auto m = cv::imread(fullFilename.c_str(), cv::IMREAD_UNCHANGED);
	return m;
}

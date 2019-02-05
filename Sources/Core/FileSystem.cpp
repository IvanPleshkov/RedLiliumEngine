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

std::string FileSystem::ToAbsolute(std::string_view filename)
{
	return m_workingPath + "\\" + std::string(filename);
}

std::string FileSystem::ReadFile(std::string_view filename)
{
	const std::string fullFilename = ToAbsolute(filename);
	std::ifstream fileStream(fullFilename);
	std::string fileData(
		(std::istreambuf_iterator<char>(fileStream)),
		std::istreambuf_iterator<char>());
	return std::move(fileData);
}

std::string FileSystem::ReadShaderFile(std::string_view filename)
{
	std::set<std::string> includedFiles;
	return ReadShaderFileRecursive(filename, includedFiles);
}

std::string FileSystem::ReadShaderFileRecursive(std::string_view filename, std::set<std::string>& includedFiles)
{
	const std::string fullFilename = ToAbsolute(filename);
	RED_LILIUM_ASSERT(includedFiles.find(fullFilename) == includedFiles.end());
	includedFiles.insert(fullFilename);

	std::string shaderData = ReadFile(filename);
	std::string resultShaderData = "";

	// todo: use regex
	std::istringstream stream(shaderData);
	std::string line;
	while (std::getline(stream, line))
	{
		size_t pos = line.find("#include", 0);
		if (pos != std::string::npos)
		{
			size_t startPos = line.find('"', 0);
			RED_LILIUM_ASSERT(startPos != std::string::npos);
			size_t endPos = line.find('"', startPos + 1);
			RED_LILIUM_ASSERT(endPos != std::string::npos);
			std::string includeFilename(line.c_str() + startPos + 1, endPos - startPos - 1);
			std::string includedData = ReadShaderFileRecursive(includeFilename, includedFiles);
			resultShaderData += includedData + "\n";
		}
		else
		{
			resultShaderData += line + "\n";
		}
	}

	return std::move(resultShaderData);
}

json FileSystem::ReadJson(std::string_view filename)
{
	const std::string fileData = ReadFile(filename);
	json j = json::parse(fileData);
	return std::move(j);
}

cv::Mat FileSystem::ReadTexture(std::string_view filename, i32 cvFlags)
{
	const std::string fullFilename = ToAbsolute(filename);
	auto m = cv::imread(fullFilename.c_str(), static_cast<int>(cvFlags));
	return m;
}

void FileSystem::Dump(std::string_view filename, std::string data)
{
#if RED_LILIUM_USE_DUMPS
#endif
}

#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

class ApplicationSettings;
class TextureBase;

class FileSystem : public RedLiliumObject
{
public:
	FileSystem(ptr<ApplicationSettings> applicationSettings);
	~FileSystem() override = default;

	std::string ToAbsolute(std::string_view filename);
	std::string ReadFile(std::string_view filename);
	std::string ReadShaderFile(std::string_view filename);
	json ReadJson(std::string_view filename);
	cv::Mat ReadTexture(std::string_view filename, i32 cvFlags = cv::IMREAD_UNCHANGED);

	void Dump(std::string_view filename, std::string data);

private:
	std::string ReadShaderFileRecursive(std::string_view filename, std::set<std::string>& includedFiles);

	ptr<ApplicationSettings> m_applicationSettings;
	std::string m_workingPath;
};

} // namespace RED_LILIUM_NAMESPACE

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

	std::string ReadFile(std::string_view filename);
	json ReadJson(std::string_view filename);
	cv::Mat ReadTexture(std::string_view filename, int cvFlags = cv::IMREAD_UNCHANGED);

private:
	ptr<ApplicationSettings> m_applicationSettings;
	std::string m_workingPath;
};

} // namespace RED_LILIUM_NAMESPACE

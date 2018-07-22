#pragma once

#include <GuiFramework/GuiFramework.h>

namespace RED_LILIUM_NAMESPACE
{
class BgfxWindow;
class BgfxEnvironment;

class BgfxGuiApplication : public RedLiliumObject
{
public:
	BgfxGuiApplication(
		const std::string& name,
		const std::string& description,
		const std::vector<std::string>& args,
		vec2 windowSize);
	~BgfxGuiApplication() override;

	void Run(GuiRecordingMode recordingMode = GuiRecordingMode::NoRecording);
	void TakeScreenshot(const std::string& outputFile);

	virtual void OnBeginApplication() {}
	virtual void OnEndApplication() {}
	virtual void Update() {}
	
	ptr<GuiManager> GetGuiManager();
	ptr<const GuiManager> GetGuiManager() const;
	ptr<BgfxWindow> GetNativeWindow();
	ptr<const BgfxWindow> GetNativeWindow() const;
	ptr<BgfxEnvironment> GetNativeEnvironment();
	ptr<const BgfxEnvironment> GetNativeEnvironment() const;
	const std::string& GetDataPath() const;

private:
	class Impl;
	friend class Impl;

	void InitDataPath();

private:
	uptr<Impl> m_impl;
	uptr<GuiManager> m_guiManager;
	uptr<BgfxWindow> m_nativeWindow;
	uptr<BgfxEnvironment> m_nativeEnvironment;

	std::vector<std::string> m_args;
	std::string m_dataPath;
	std::string m_name;
	std::string m_description;

	std::string m_screenshotOutputFilename;
};

}  // namespace RED_LILIUM_NAMESPACE

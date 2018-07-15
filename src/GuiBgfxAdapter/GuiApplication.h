#pragma once

#include <GuiFramework/GuiFramework.h>

namespace RED_LILIUM_NAMESPACE
{
class BgfxWindow;
class BgfxEnvironment;

class GuiApplication : public RedLiliumObject
{
public:
	GuiApplication(
		const std::string& name,
		const std::string& description,
		const std::vector<std::string>& args,
		vec2 windowSize);
	~GuiApplication() override;

	void Run();

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

	MouseState m_mouseState;
	KeyState m_keyState;
};

}  // namespace RED_LILIUM_NAMESPACE

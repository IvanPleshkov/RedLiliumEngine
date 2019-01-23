#pragma once

#include <Core/Common.h>
#include <imgui/application.h>
#include <NodesEditor/NodesEditorSystem.h>

namespace RED_LILIUM_NAMESPACE
{

class EditorImguiWindow : public RedLiliumObject
{
public:
	EditorImguiWindow();
	~EditorImguiWindow() override = default;
	const std::string& GetCaption();
	void SetCaption(const std::string& caption);
	virtual void Tick();

private:
	std::string m_caption;
};

class EditorImguiApplication : public ImguiApplication
{
public:
	void Init() override;
	void Tick(f32 dTime) override;
	void PollEvent(ptr<SDL_Event> event) override;

	ptr<const NodesEditorSystem> GetEditorSystem() const;
	ptr<NodesEditorSystem> GetEditorSystem();

private:
	void WindowTick(ImGuiID dockspaceId, ptr<EditorImguiWindow> window);

	uptr<NodesEditorSystem> m_editorSystem;
	ImGuiWindowClass m_guiWindowClass;
	std::vector<uptr<EditorImguiWindow>> m_windows;
};

} // namespace RED_LILIUM_NAMESPACE

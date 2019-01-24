#pragma once

#include "EditorImguiApplication.h"
#include <Editor/Event.h>

namespace RED_LILIUM_NAMESPACE
{

class NodesEditorWindow : public EditorImguiWindow
{
public:
	NodesEditorWindow(ptr<EditorImguiApplication> application);
	~NodesEditorWindow() override = default;
	void Tick() override;

private:
	vec2 GetScreenPosition(vec2 globalPosition);
	vec2 GetGlobalPosition(vec2 screenPosition);

	void DrawNode(ptr<ImDrawList> imguiDrawList, ptr<const Entity> nodeEntity);

	ptr<EditorImguiApplication> m_application;
	ptr<NodesEditorSystem> m_editorSystem;
	vec2 m_canvasPos;
	vec2 m_canvasSize;
	vec2 m_mousePos;

	float m_scale;
	vec2 m_translate;
};

} // namespace RED_LILIUM_NAMESPACE

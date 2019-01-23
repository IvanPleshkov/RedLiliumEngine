#pragma once

#include "EditorImguiApplication.h"

namespace RED_LILIUM_NAMESPACE
{

class PropertyGridWindow : public EditorImguiWindow
{
public:
	PropertyGridWindow(ptr<EditorImguiApplication> application);
	~PropertyGridWindow() override = default;
	void Tick() override;

private:
	ptr<EditorImguiApplication> m_application;
	ptr<NodesEditorSystem> m_editorSystem;
};

} // namespace RED_LILIUM_NAMESPACE

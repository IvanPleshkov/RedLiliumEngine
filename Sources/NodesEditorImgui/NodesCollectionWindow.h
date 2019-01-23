#pragma once

#include "EditorImguiApplication.h"

namespace RED_LILIUM_NAMESPACE
{

class NodesCollectionWindow : public EditorImguiWindow
{
public:
	NodesCollectionWindow(ptr<EditorImguiApplication> application);
	~NodesCollectionWindow() override = default;
	void Tick() override;

private:
	ptr<EditorImguiApplication> m_application;
	ptr<NodesEditorSystem> m_editorSystem;
};

} // namespace RED_LILIUM_NAMESPACE

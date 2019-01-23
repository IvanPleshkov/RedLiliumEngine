#include "pch.h"
#include "NodesCollectionWindow.h"

using namespace RED_LILIUM_NAMESPACE;

NodesCollectionWindow::NodesCollectionWindow(ptr<EditorImguiApplication> application)
	: EditorImguiWindow()
	, m_application(application)
	, m_editorSystem(application->GetEditorSystem())
{
	SetCaption("Nodes Collection");
}

void NodesCollectionWindow::Tick()
{}

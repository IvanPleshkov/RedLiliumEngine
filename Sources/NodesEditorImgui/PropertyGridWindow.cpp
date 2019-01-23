#include "pch.h"
#include "PropertyGridWindow.h"

using namespace RED_LILIUM_NAMESPACE;

PropertyGridWindow::PropertyGridWindow(ptr<EditorImguiApplication> application)
	: EditorImguiWindow()
	, m_application(application)
	, m_editorSystem(application->GetEditorSystem())
{
	SetCaption("Properties");
}

void PropertyGridWindow::Tick()
{}

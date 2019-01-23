#include "pch.h"

#include "Commands.h"
#include <NodesEditorImgui/EditorImguiApplication.h>

REGISTER_COMMAND("editor", StartEditor)

using namespace RED_LILIUM_NAMESPACE;

int StartEditor(ptr<ApplicationSettings> settings)
{
	uptr<EditorImguiApplication> application = umake<EditorImguiApplication>();
	return application->Start(settings);
}

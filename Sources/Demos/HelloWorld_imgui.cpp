#include "pch.h"

#include "Commands.h"
#include <imgui/application.h>

REGISTER_COMMAND("HelloWorld_imgui", HelloWorld_imgui)

using namespace RED_LILIUM_NAMESPACE;

int HelloWorld_imgui(ptr<ApplicationSettings> settings)
{
	uptr<ImguiDemoApplication> application = umake<ImguiDemoApplication>();
	return application->Start(settings);
}

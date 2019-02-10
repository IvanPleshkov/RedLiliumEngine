#include "pch.h"

#include "Commands.h"
#include <imgui/application.h>
#include <Render/RenderDevice.h>
#include <Render/GpuTexture.h>
#include <Render/GpuTextureManager.h>

REGISTER_COMMAND("LoadSceneDemo", LoadSceneDemo)

using namespace RED_LILIUM_NAMESPACE;

namespace
{

class LoadSceneApplication : public ImguiApplication
{
public:
	void Init() override
	{
		TextureSettings loadSettings;
		m_texture = GetRenderDevice()->GetGpuTextureManager()->Get("Textures\\wood.png", loadSettings);
	}

	void Tick(f32 dTime) override
	{
		ptr<ImDrawList> imguiDrawList = ImGui::GetWindowDrawList();
		auto canvasPos = ImGui::GetCursorScreenPos();

		imguiDrawList->AddRect(
			ImVec2(canvasPos.x + 100, canvasPos.y + 100),
			ImVec2(canvasPos.x + 200, canvasPos.y + 200),
			IM_COL32(100, 0, 0, 255));

		imguiDrawList->AddImage(
			(void*)(intptr_t)m_texture->GetNative(),
			ImVec2(canvasPos.x + 100, canvasPos.y + 100),
			ImVec2(canvasPos.x + 200, canvasPos.y + 200));

		ImGui::Image((void*)(intptr_t)m_texture->GetNative(), ImVec2(512, 512));
	}

	void PollEvent(ptr<SDL_Event> event) override
	{
		if (event->type == SDL_QUIT)
		{
			Stop();
		}

		if (event->type == SDL_WINDOWEVENT &&
			event->window.event == SDL_WINDOWEVENT_CLOSE &&
			event->window.windowID == SDL_GetWindowID(GetMainWindow()))
		{
			Stop();
		}
	}

private:
	sptr<GpuTexture> m_texture;
};

}

int LoadSceneDemo(ptr<ApplicationSettings> settings)
{
	uptr<LoadSceneApplication> application = umake<LoadSceneApplication>();
	return application->Start(settings);
}

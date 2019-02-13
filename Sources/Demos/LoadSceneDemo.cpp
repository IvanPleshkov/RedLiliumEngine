#include "pch.h"

#include "Commands.h"
#include <imgui/application.h>
#include <Render/RenderDevice.h>
#include <Render/RenderTarget.h>
#include <Render/GpuTexture.h>
#include <Render/GpuTextureManager.h>
#include <Render/Material.h>
#include <Pipeline/RenderPipeline.h>
#include <Pipeline/Assimp/AssimpLoader.h>
#include <Pipeline/Components/CameraComponent.h>
#include <Pipeline/Components/CameraControllerComponent.h>

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
		loadSettings.format = TextureFormat::RGB8;
		m_texture = GetRenderDevice()->GetGpuTextureManager()->Get("Textures\\wood.png", loadSettings);
		m_texture2 = GetRenderDevice()->GetGpuTextureManager()->Get("Textures\\alphatest.png", loadSettings);

		m_pipeline = RenderPipeline::CreateSimpleOpaquePipeline(GetRenderDevice());
		m_pipeline->SetTargetSize({ 512, 512 });

		CreateDemoScene();
	}

	void Tick(f32 dTime) override
	{
		m_pipeline->Render({ m_scene->GetRoot() });

		ptr<ImDrawList> imguiDrawList = ImGui::GetWindowDrawList();
		auto canvasPos = ImGui::GetCursorScreenPos();

		GLuint id = m_pipeline->GetRenderTarget()->GetColor()->GetNative();

		imguiDrawList->AddRect(
			ImVec2(canvasPos.x + 100, canvasPos.y + 100),
			ImVec2(canvasPos.x + 612, canvasPos.y + 612),
			IM_COL32(100, 0, 0, 255));

		imguiDrawList->AddImage(
			(void*)(intptr_t)id,
			ImVec2(canvasPos.x + 100, canvasPos.y + 100),
			ImVec2(canvasPos.x + 612, canvasPos.y + 612));
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
	void CreateDemoScene()
	{
		m_scene = umake<Scene>();
		AssimpImportOptions importOptions;
		importOptions.materialFabric = [renderDevice = GetRenderDevice()](auto)
		{
			return Material::Create(renderDevice, "Shaders\\ColoredTriangle\\material.json");
		};
		LoadSceneByAssimp(GetRenderDevice(), "Models\\nanosuit\\nanosuit.obj", m_scene->GetRoot()->AddChild("Loaded Scene"), importOptions);

		Camera camera;
		camera.LookAt({ 25.0f, 25.0f, 25.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
		camera.SetPerspective(45.0f, 1.0f, 0.1f, 100.0f);

		mat4 camView = camera.GetView();
		camera.SetView(mat4(1.0f));

		ptr<Entity> cameraEntity = m_scene->GetRoot()->AddChild("Camera");
		ptr<CameraComponent> cameraComponent = cameraEntity->AddComponent<CameraComponent>();
		cameraComponent->SetCamera(camera);
		cameraEntity->AddComponent<CameraControllerComponent>();

		cameraEntity->SetLocalTransform(camView);
	}

	uptr<Scene> m_scene;
	uptr<RenderPipeline> m_pipeline;
	sptr<GpuTexture> m_texture;
	sptr<GpuTexture> m_texture2;
};

}

int LoadSceneDemo(ptr<ApplicationSettings> settings)
{
	uptr<LoadSceneApplication> application = umake<LoadSceneApplication>();
	return application->Start(settings);
}

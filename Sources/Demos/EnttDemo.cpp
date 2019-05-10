#include "pch.h"

#include "Commands.h"
#include <imgui/application.h>
#include <Render/RenderDevice.h>
#include <Render/RenderContext.h>
#include <Render/RenderTarget.h>
#include <Render/GpuTexture.h>
#include <Render/GpuTextureManager.h>
#include <Render/Material.h>
#include <Render/GpuMesh.h>
#include <Core/InputEvent.h>
#include <Core/Camera.h>

#include <Ecs/Scene.h>

REGISTER_COMMAND("EnttDemo", EnttDemo)

using namespace RED_LILIUM_NAMESPACE;

struct HierarchyComponent
{
	Entity m_parent;
};

struct TransformComponent
{
	mat3 m_localTransform;
	mat3 m_worldTransform;
};

struct CameraComponent
{
	Camera m_camera;
	sptr<RenderTarget> m_renderTarget;
};

struct CameraControllerComponent
{
	vec3 m_up;
	vec3 m_lookAt;
};

struct MeshFilterComponent
{
	sptr<Mesh> m_mesh;
};

struct MeshRendererComponent
{
	sptr<Mesh> m_mesh;
	sptr<GpuMesh> m_gpuMesh;
	sptr<Material> m_material;
};

class Pipeline;

struct IPipelineStage : public NonCopyable
{
	IPipelineStage(ptr<Pipeline> pipeline)
		: m_pipeline(pipeline)
	{}

	virtual void Init() {}
	virtual bool HandleEvent(MouseEvent& mouseEvent, Scene& scene) { return false; }
	virtual void Tick(Scene& scene) { }
	virtual void Draw(
		Scene& scene,
		ptr<RenderContext> renderContext,
		CameraComponent& cameraComponent) { }

protected:
	ptr<Pipeline> m_pipeline;
};

class Pipeline final : public NonCopyable
{
public:
	Pipeline(ptr<RenderDevice> renderDevice)
		: m_renderDevice(renderDevice)
	{}
	~Pipeline() = default;

	void AddStage(uptr<IPipelineStage>&& stage)
	{
		m_stages.push_back(std::move(stage));
	}

	template<class T>
	ptr<T> GetStage()
	{
		for (auto& stage : m_stages)
		{
			ptr<T> casted = Cast<T>(stage.get());
			if (casted != nullptr)
			{
				return casted;
			}
		}
		return nullptr;
	}

	bool HandleEvent(MouseEvent& mouseEvent, Scene& scene)
	{
		for (auto& stage : m_stages)
		{
			if (stage->HandleEvent(mouseEvent, scene))
			{
				return true;
			}
		}

		return false;
	}

	void Tick(Scene& scene)
	{
		for (auto& stage : m_stages)
		{
			stage->Tick(scene);
		}
	}

	void Draw(Scene& scene)
	{
		uptr<RenderContext> renderContext = m_renderDevice->CreateRenderContext();

		auto& view = scene.GetView<CameraComponent>();
		for (auto[entity, cameraComponent] : view)
		{
			Draw(scene, renderContext.get(), *cameraComponent);
		}
	}

	ptr<RenderDevice> GetRenderDevice() { return m_renderDevice; }

private:
	void Draw(Scene& scene, ptr<RenderContext> renderContext, CameraComponent& cameraComponent)
	{
		for (auto& stage : m_stages)
		{
			stage->Draw(scene, renderContext, cameraComponent);
		}
	}

	ptr<RenderDevice> m_renderDevice;
	std::vector<uptr<IPipelineStage>> m_stages;
};


class RenderOpaqueStage : public IPipelineStage
{
public:
	RenderOpaqueStage(ptr<Pipeline> pipeline)
		: IPipelineStage(pipeline)
	{}

	void Draw(
		Scene& scene,
		ptr<RenderContext> renderContext,
		CameraComponent& cameraComponent) override
	{
		if (cameraComponent.m_renderTarget == nullptr)
		{
			return;
		}

		// uptr<RenderContext> renderContext = GetRenderDevice()->CreateRenderContext();
		renderContext->SetRenderTarget(cameraComponent.m_renderTarget);

		glViewport(
			0, 0,
			static_cast<GLsizei>(512),
			static_cast<GLsizei>(512));
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderContext->CurrentCamera(cameraComponent.m_camera);
		renderContext->SetRenderTarget(cameraComponent.m_renderTarget);

		DrawScene(scene, renderContext, cameraComponent);
	}

private:
	void DrawScene(
		Scene& scene,
		ptr<RenderContext> renderContext,
		CameraComponent& cameraComponent)
	{
		auto& view = scene.GetView<MeshFilterComponent, MeshRendererComponent>();
		for (auto [entity, filter, renderer] : view)
		{
			if (!(*renderer).m_material)
			{
				continue;
			}

			if ((*filter).m_mesh == nullptr)
			{
				(*renderer).m_mesh = nullptr;
				(*renderer).m_gpuMesh = nullptr;
				continue;
			}

			if ((*filter).m_mesh != (*renderer).m_mesh)
			{
				(*renderer).m_mesh = (*filter).m_mesh;
				(*renderer).m_gpuMesh = smake<GpuMesh>(renderContext->GetRenderDevice());
				(*renderer).m_gpuMesh->Update((*filter).m_mesh.get());
			}

			mat4 worldTransform(1.0f);
			auto transformComponent = scene.GetComponent<TransformComponent>(entity);
			if (transformComponent)
			{
				worldTransform = transformComponent->m_worldTransform;
			}
			renderContext->Set("g_model", worldTransform);
			renderContext->Draw((*renderer).m_gpuMesh, (*renderer).m_material);
		}
	}
};

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

		CreateDemoScene();
	}

	void Tick(f32 dTime) override
	{
		m_pipeline->Draw(m_scene);

		ptr<ImDrawList> imguiDrawList = ImGui::GetWindowDrawList();
		auto canvasPos = ImGui::GetCursorScreenPos();

		for (auto[entity, cameraComponent] : m_scene.GetView<CameraComponent>())
		{
			GLuint id = cameraComponent->m_renderTarget->GetColor()->GetNative();

			imguiDrawList->AddRect(
				ImVec2(canvasPos.x + 100, canvasPos.y + 100),
				ImVec2(canvasPos.x + 612, canvasPos.y + 612),
				IM_COL32(100, 0, 0, 255));

			imguiDrawList->AddImage(
				(void*)(intptr_t)id,
				ImVec2(canvasPos.x + 100, canvasPos.y + 100),
				ImVec2(canvasPos.x + 612, canvasPos.y + 612));
		}
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
		m_pipeline = umake<Pipeline>(GetRenderDevice());
		uptr<RenderOpaqueStage> renderOpaqueStage = umake<RenderOpaqueStage>(m_pipeline.get());
		m_pipeline->AddStage(std::move(renderOpaqueStage));
		
		auto entity = m_scene.Add();
		auto& cameraComponent = *m_scene.AddComponent<CameraComponent>(entity);

		Camera camera;
		camera.LookAt({ 5.0f, 5.0f, 5.0f }, { 0, 0, 0 }, { 0, 0, 1 });
		camera.SetPerspective(45.0f, 1.0f, 0.1f, 100.0f);
		cameraComponent.m_camera = camera;

		cameraComponent.m_renderTarget = smake<RenderTarget>(GetRenderDevice());
		cameraComponent.m_renderTarget->Create(u32(512), u32(512));

		entity = m_scene.Add();
		m_scene.AddComponent<MeshRendererComponent>(entity);
		m_scene.AddComponent<MeshFilterComponent>(entity);
		m_scene.AddComponent<TransformComponent>(entity);

		auto meshRendererComponent = m_scene.GetComponent<MeshRendererComponent>(entity);
		auto meshFilterComponent = m_scene.GetComponent<MeshFilterComponent>(entity);
		auto transformComponent = m_scene.GetComponent<TransformComponent>(entity);

		float angle = 0.0f;
		transformComponent->m_worldTransform = glm::rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));
		transformComponent->m_localTransform = glm::inverse(transformComponent->m_worldTransform);

		sptr<Mesh> mesh = Mesh::GenerateRectangle();
		sptr<GpuMesh> gpuMesh = smake<GpuMesh>(GetRenderDevice());
		gpuMesh->Update(mesh.get());
		meshFilterComponent->m_mesh = mesh;

		sptr<Material> material = Material::Create(GetRenderDevice(), "Shaders\\ColoredTriangle\\material.json");
		material->Set("g_diffuseColor1", vec4(1.0f, 1.0f, 0.0f, 0.0f));
		material->Set("g_albedo2", m_texture);
		material->Set("g_albedo1", m_texture2);

		meshRendererComponent->m_gpuMesh = gpuMesh;
		meshRendererComponent->m_mesh = mesh;
		meshRendererComponent->m_material = material;
	}

	Scene m_scene;
	uptr<Pipeline> m_pipeline;
	sptr<GpuTexture> m_texture;
	sptr<GpuTexture> m_texture2;
};

}

int EnttDemo(ptr<ApplicationSettings> settings)
{
	uptr<LoadSceneApplication> application = umake<LoadSceneApplication>();
	return application->Start(settings);
}

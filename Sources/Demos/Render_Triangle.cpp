#include "pch.h"

#include <Render/RenderDevice.h>
#include <Render/GpuTextureManager.h>
#include <Render/Shader.h>
#include <Render/Material.h>
#include <Render/GpuMesh.h>
#include <Render/RenderContext.h>

#include "Commands.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

REGISTER_COMMAND("Render_Triangle", Render_Triangle)

using namespace RED_LILIUM_NAMESPACE;

namespace RenderTriangleNamespace
{
	static bool quitting = false;
	static SDL_Window *window = NULL;
	static SDL_GLContext gl_context;

	int SDLCALL watch(void *userdata, SDL_Event* event) {

		if (event->type == SDL_APP_WILLENTERBACKGROUND) {
			quitting = true;
		}

		return 1;
	}

	int run(ptr<ApplicationSettings> settings)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
			return 1;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

		window = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, SDL_WINDOW_OPENGL);
		gl_context = SDL_GL_CreateContext(window);
		auto err = glewInit();
		if (err != GLEW_OK)
		{
			fprintf(stderr, "Failed to initialize OpenGL loader!\n");
			return 1;
		}

		SDL_AddEventWatch(watch, NULL);

		uptr<FileSystem> fileSystem = umake<FileSystem>(settings);
		uptr<RenderDevice> renderDevice = umake<RenderDevice>(settings, fileSystem.get());

		TextureSettings textureSettings;
		textureSettings.format = TextureFormat::RGB8;
		sptr<GpuTexture> texture1 = renderDevice->GetGpuTextureManager()->Get("Textures\\wood.png", textureSettings);
		textureSettings.format = TextureFormat::RGBA8;
		sptr<GpuTexture> texture2 = renderDevice->GetGpuTextureManager()->Get("Textures\\alphatest.png", textureSettings);

		sptr<Material> material = Material::Create(renderDevice.get(), "Shaders\\ColoredTriangle\\material.json");
		material->Set("g_diffuseColor1", vec4(1.0f, 1.0f, 0.0f, 0.0f));
		material->Set("g_albedo2", texture2);
		material->Set("g_albedo1", texture1);

		sptr<Mesh> mesh = Mesh::GenerateRectangle();
		sptr<GpuMesh> gpuMesh = smake<GpuMesh>(renderDevice.get());
		gpuMesh->Update(mesh.get());

		mat4 view;
		view = glm::translate(view, vec3(0.0f, 0.0f, -3.0f));
		Camera camera;
		camera.LookAt({ 5.0f, 5.0f, 5.0f }, { 0, 0, 0 }, { 0, 0, 1 });
		camera.SetPerspective(45.0f, 1.0f, 0.1f, 100.0f);

		quitting = false;
		float angle = 0.0f;
		while (!quitting)
		{

			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					quitting = true;
				}
			}

			SDL_GL_MakeCurrent(window, gl_context);

			glViewport(0, 0, 512, 512);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			mat4 model(1.0f);
			angle += 0.03f;
			model = glm::rotate(model, angle, vec3(0.0f, 0.0f, 1.0f));
			material->Set("g_model", model);

			auto context = renderDevice->CreateRenderContext();
			context->CurrentCamera(camera);

			context->Set("g_testColor", vec4(0.0f, 1.0f, 1.0f, 1.0f));
			// context->Set("g_albedo1", texture2);
			// context->Set("g_albedo2", texture1);
			// context->Set("g_diffuseColor1", vec4(1.0f, 0.0f, 1.0f, 0.0f));
			context->Draw(gpuMesh, material);

			SDL_GL_SwapWindow(window);
			SDL_Delay(2);
		}

		SDL_DelEventWatch(watch, NULL);
		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		SDL_Quit();

		return 0;
	}
}

int Render_Triangle(ptr<ApplicationSettings> settings)
{
	return RenderTriangleNamespace::run(settings);
}

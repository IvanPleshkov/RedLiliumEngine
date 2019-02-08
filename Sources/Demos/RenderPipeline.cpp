#include "pch.h"

#include <Render/RenderDevice.h>
#include <Render/MaterialManager.h>
#include <Render/GpuTextureManager.h>
#include <Render/Shader.h>
#include <Render/Material.h>
#include <Render/GpuMesh.h>
#include <Render/RenderContext.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Component.h>
#include <Pipeline/RenderPipeline.h>

#include "Commands.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

REGISTER_COMMAND("RenderPipeline", RenderPipelineDemo)

using namespace RED_LILIUM_NAMESPACE;

namespace RenderPipelineDemoNamespace
{
	static bool quitting = false;
	static SDL_Window *window = NULL;
	static SDL_GLContext gl_context;

	uptr<Scene> CreateDemoScene()
	{
		return nullptr;
	}

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

		uptr<RenderDevice> renderDevice = umake<RenderDevice>(settings);
		uptr<FileSystem> fileSystem = umake<FileSystem>(settings);
		uptr<MaterialManager> materialManager = umake<MaterialManager>(renderDevice.get(), fileSystem.get());
		uptr<GpuTextureManager> gpuTextureManager = umake<GpuTextureManager>(renderDevice.get(), fileSystem.get());
		renderDevice->Init(materialManager.get(), gpuTextureManager.get());

		uptr<Scene> scene = CreateDemoScene();
		uptr<RenderPipeline> pipeline = RenderPipeline::CreateSimpleOpaquePipeline(renderDevice.get());

		quitting = false;
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

			pipeline->Render({ scene->GetRoot() });

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

int RenderPipelineDemo(ptr<ApplicationSettings> settings)
{
	return RenderPipelineDemoNamespace::run(settings);
}

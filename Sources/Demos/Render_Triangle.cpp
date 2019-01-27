#include "pch.h"

#include <Render/RenderDevice.h>
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
	static uptr<RenderDevice> renderDevice;

	static std::string vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";
	static std::string fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	int SDLCALL watch(void *userdata, SDL_Event* event) {

		if (event->type == SDL_APP_WILLENTERBACKGROUND) {
			quitting = true;
		}

		return 1;
	}

	int run()
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

		renderDevice = umake<RenderDevice>();
		sptr<Shader> vertexShader = smake<Shader>(renderDevice.get());
		vertexShader->CompileFromString(ShaderType::Vertex, vertexShaderSource);
		sptr<Shader> fragmentShader = smake<Shader>(renderDevice.get());
		fragmentShader->CompileFromString(ShaderType::Fragment, fragmentShaderSource);
		sptr<ShaderProgram> shaderProgram = smake<ShaderProgram>(renderDevice.get());
		shaderProgram->Link(vertexShader, fragmentShader);
		sptr<Material> material = smake<Material>();
		material->SetShaderProgram(shaderProgram);
		material->SetVertexDeclaration(renderDevice->GetVertexDeclarationP());

		uptr<Mesh> mesh = Mesh::GenerateTriangle();
		sptr<GpuMesh> gpuMesh = smake<GpuMesh>(renderDevice.get());
		gpuMesh->Update(mesh.get());

		quitting = false;
		while (!quitting) {

			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					quitting = true;
				}
			}

			SDL_GL_MakeCurrent(window, gl_context);

			glViewport(0, 0, 512, 512);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			auto context = renderDevice->CreateRenderContext();
			context->Draw(gpuMesh, material);

			renderDevice->CheckErrors();

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
	return RenderTriangleNamespace::run();
}

#include <SDL.h>
#include <spdlog/spdlog.h>

#include "render/render_instance.h"
#include "render/render_device.h"
#include "render/render_target.h"
#include "render/render_target_builder.h"
#include "render/render_pipeline.h"
#include "render/render_pipeline_builder.h"
#include "render/render_step.h"
#include "render/gpu_buffer.h"
#include "render/gpu_mesh.h"

#include "mesh.h"
#include "resources_manager.h"

#include <memory>

void triangleSample(SDL_Window* window)
{
    ResourcesManager resourcesManager;

    auto renderInstance = std::make_shared<RenderInstance>(window, true);
    auto renderDevice = std::make_shared<RenderDevice>(renderInstance);
    auto renderTargetBuilder = RenderTargetBuilder(renderDevice)
        .setVkFormat(renderDevice->getSwapChainVkImageFormat())
        .setSize(renderDevice->getSwapChainSize());
    for (auto& vkImageView : renderDevice->getSwapChainVkImageViews())
    {
        renderTargetBuilder.addImageView(vkImageView);
    }
    auto renderTarget = renderTargetBuilder.build();
    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget)
        .setVertexShader(resourcesManager.readResourceData("shaders/triangle.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/triangle.frag.spv"))
        .build();
    auto renderStep = std::make_shared<RenderStep>(renderDevice, renderTarget, renderPipeline);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT: quit = true; break;
            }
        }
        
        renderDevice->startFrame();
        renderTarget->setFramebufferIndex(renderDevice->getSwapChainCurrentImageIndex());
        renderStep->draw(nullptr, renderDevice->getSwapChainVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }
}

void meshSample(SDL_Window* window)
{
    ResourcesManager resourcesManager;

    auto renderInstance = std::make_shared<RenderInstance>(window, true);
    auto renderDevice = std::make_shared<RenderDevice>(renderInstance);
    auto renderTargetBuilder = RenderTargetBuilder(renderDevice)
        .setVkFormat(renderDevice->getSwapChainVkImageFormat())
        .setSize(renderDevice->getSwapChainSize());
    for (auto& vkImageView : renderDevice->getSwapChainVkImageViews())
    {
        renderTargetBuilder.addImageView(vkImageView);
    }
    auto renderTarget = renderTargetBuilder.build();
    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget)
        .setVertexShader(resourcesManager.readResourceData("shaders/colored_mesh.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/colored_mesh.frag.spv"))
        .addVertexAttribute(0, offsetof(Vertex, pos), VK_FORMAT_R32G32_SFLOAT)
        .addVertexAttribute(1, offsetof(Vertex, color), VK_FORMAT_R32G32B32_SFLOAT)
        .build();
    
    Mesh mesh;
    mesh._vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
    auto gpuMesh = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh->update(mesh);
    auto renderStep = std::make_shared<RenderStep>(renderDevice, renderTarget, renderPipeline);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT: quit = true; break;
            }
        }
        
        renderDevice->startFrame();
        renderTarget->setFramebufferIndex(renderDevice->getSwapChainCurrentImageIndex());
        renderStep->draw(gpuMesh, renderDevice->getSwapChainVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }
}

void indexedMeshSample(SDL_Window* window)
{
    ResourcesManager resourcesManager;

    auto renderInstance = std::make_shared<RenderInstance>(window, true);
    auto renderDevice = std::make_shared<RenderDevice>(renderInstance);
    auto renderTargetBuilder = RenderTargetBuilder(renderDevice)
        .setVkFormat(renderDevice->getSwapChainVkImageFormat())
        .setSize(renderDevice->getSwapChainSize());
    for (auto& vkImageView : renderDevice->getSwapChainVkImageViews())
    {
        renderTargetBuilder.addImageView(vkImageView);
    }
    auto renderTarget = renderTargetBuilder.build();
    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget)
        .setVertexShader(resourcesManager.readResourceData("shaders/colored_mesh.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/colored_mesh.frag.spv"))
        .addVertexAttribute(0, offsetof(Vertex, pos), VK_FORMAT_R32G32_SFLOAT)
        .addVertexAttribute(1, offsetof(Vertex, color), VK_FORMAT_R32G32B32_SFLOAT)
        .build();

    Mesh mesh;
    mesh._vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };
    mesh._indices = {
        0, 1, 2, 2, 3, 0
    };
    auto gpuMesh = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh->update(mesh);
    auto renderStep = std::make_shared<RenderStep>(renderDevice, renderTarget, renderPipeline);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT: quit = true; break;
            }
        }
        
        renderDevice->startFrame();
        renderTarget->setFramebufferIndex(renderDevice->getSwapChainCurrentImageIndex());
        renderStep->draw(gpuMesh, renderDevice->getSwapChainVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }
}

int main(int argc, char *args[])
{
    spdlog::info("Start application");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Window* window = SDL_CreateWindow("vulkan demo", -1, -1, 640, 480, SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

    indexedMeshSample(window);
    // triangleSample(window);

    SDL_DestroyWindow(window);
    SDL_Quit();

    spdlog::info("End application");
    return 0;
}

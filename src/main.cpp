#include <SDL.h>
#include <spdlog/spdlog.h>

#include "render/render_instance.h"
#include "render/render_device.h"
#include "render/render_target.h"
#include "render/render_target_builder.h"
#include "render/render_pipeline.h"
#include "render/render_pipeline_builder.h"
#include "render/render_step.h"

#include "resources_manager.h"

#include <memory>

int main(int argc, char *args[])
{
    spdlog::info("Start application");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Window *window = SDL_CreateWindow("vulkan demo", -1, -1, 640, 480, SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

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
        renderStep->draw(renderDevice->getSwapChainVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    spdlog::info("End application");
    return 0;
}

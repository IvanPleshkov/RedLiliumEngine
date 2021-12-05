#include <SDL.h>
#include <spdlog/spdlog.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "render/render_instance.h"
#include "render/render_device.h"
#include "render/render_target.h"
#include "render/render_target_builder.h"
#include "render/render_pipeline.h"
#include "render/render_pipeline_builder.h"
#include "render/render_step.h"
#include "render/gpu_buffer.h"
#include "render/gpu_mesh.h"
#include "render/render_descriptor.h"
#include "render/render_descriptor_builder.h"
#include "render/gpu_texture.h"

#include "mesh.h"
#include "resources_manager.h"

#include <chrono>
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
    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget, nullptr)
        .setFrontFace(VK_FRONT_FACE_CLOCKWISE)
        .setVertexShader(resourcesManager.readResourceData("shaders/triangle.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/triangle.frag.spv"))
        .build();
    auto renderStep = std::make_shared<RenderStep>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

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
        renderStep->draw(renderTarget, renderPipeline, nullptr);
        renderStep->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
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
    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget, nullptr)
        .setFrontFace(VK_FRONT_FACE_CLOCKWISE)
        .setVertexShader(resourcesManager.readResourceData("shaders/colored_mesh.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/colored_mesh.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT)
        .addVertexAttribute(1, 2 * sizeof(float), VK_FORMAT_R32G32B32_SFLOAT)
        .build();
    
    Mesh mesh;
    mesh._vertices = {
        0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };
    auto gpuMesh = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh->update(mesh);
    auto renderStep = std::make_shared<RenderStep>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

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
        renderStep->draw(renderTarget, renderPipeline, gpuMesh);
        renderStep->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
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
    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget, nullptr)
        .setFrontFace(VK_FRONT_FACE_CLOCKWISE)
        .setVertexShader(resourcesManager.readResourceData("shaders/colored_mesh.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/colored_mesh.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT)
        .addVertexAttribute(1, 2 * sizeof(float), VK_FORMAT_R32G32B32_SFLOAT)
        .build();

    Mesh mesh;
    mesh._vertices = {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 1.0f, 1.0f, 1.0f
    };
    mesh._indices = {
        0, 1, 2, 2, 3, 0
    };
    auto gpuMesh = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh->update(mesh);
    auto renderStep = std::make_shared<RenderStep>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

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
        renderStep->draw(renderTarget, renderPipeline, gpuMesh);
        renderStep->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }
}

void uniformBufferSample(SDL_Window* window)
{
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    } ubo;

    auto startTime = std::chrono::high_resolution_clock::now();
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

    auto uniformBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform, false);
    std::vector<char> bufferData(sizeof(UniformBufferObject), 0);
    uniformBuffer->update(bufferData.data(), sizeof(UniformBufferObject));
    auto renderDescriptor = RenderDescriptorBuilder(renderDevice)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
        .build();
    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget, renderDescriptor)
        .setVertexShader(resourcesManager.readResourceData("shaders/uniform_buffer.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/uniform_buffer.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT)
        .addVertexAttribute(1, 2 * sizeof(float), VK_FORMAT_R32G32B32_SFLOAT)
        .build();

    Mesh mesh;
    mesh._vertices = {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 1.0f, 1.0f, 1.0f
    };
    mesh._indices = {
        0, 1, 2, 2, 3, 0
    };
    auto gpuMesh = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh->update(mesh);
    auto renderStep = std::make_shared<RenderStep>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT: quit = true; break;
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        auto screenSize = renderTarget->getSize();
        float aspectRatio = static_cast<float>(screenSize.y) / static_cast<float>(screenSize.x);
        ubo.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;
        uniformBuffer->update(reinterpret_cast<const char*>(&ubo), sizeof(UniformBufferObject));

        renderDevice->startFrame();
        renderTarget->setFramebufferIndex(renderDevice->getSwapChainCurrentImageIndex());
        renderStep->draw(renderTarget, renderPipeline, gpuMesh);
        renderStep->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }
}

void textureSample(SDL_Window* window)
{
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    } ubo;

    auto startTime = std::chrono::high_resolution_clock::now();
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

    auto gpuTexture = std::make_shared<GpuTexture>(renderDevice);
    gpuTexture->upload(resourcesManager.readResourceData("textures/texture.jpeg"));

    auto uniformBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform, false);
    std::vector<char> bufferData(sizeof(UniformBufferObject), 0);
    uniformBuffer->update(bufferData.data(), sizeof(UniformBufferObject));
    auto renderDescriptor = RenderDescriptorBuilder(renderDevice)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
        .addCombinedImageSampler(gpuTexture, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
        .build();
    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget, renderDescriptor)
        .setVertexShader(resourcesManager.readResourceData("shaders/texture.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/texture.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT)
        .addVertexAttribute(1, 2 * sizeof(float), VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(2, 5 * sizeof(float), VK_FORMAT_R32G32_SFLOAT)
        .build();

    Mesh mesh;
    mesh._vertices = {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };
    mesh._indices = {
        0, 1, 2, 2, 3, 0
    };
    auto gpuMesh = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh->update(mesh);
    auto renderStep = std::make_shared<RenderStep>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT: quit = true; break;
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        auto screenSize = renderTarget->getSize();
        float aspectRatio = static_cast<float>(screenSize.y) / static_cast<float>(screenSize.x);
        ubo.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;
        uniformBuffer->update(reinterpret_cast<const char*>(&ubo), sizeof(UniformBufferObject));

        renderDevice->startFrame();
        renderTarget->setFramebufferIndex(renderDevice->getSwapChainCurrentImageIndex());
        renderStep->draw(renderTarget, renderPipeline, gpuMesh);
        renderStep->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }
}

void depthBufferSample(SDL_Window* window)
{
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    } ubo;

    auto startTime = std::chrono::high_resolution_clock::now();
    ResourcesManager resourcesManager;

    auto renderInstance = std::make_shared<RenderInstance>(window, true);
    auto renderDevice = std::make_shared<RenderDevice>(renderInstance);
    auto renderTargetBuilder = RenderTargetBuilder(renderDevice)
        .setVkFormat(renderDevice->getSwapChainVkImageFormat())
        .setSize(renderDevice->getSwapChainSize())
        .enableDepth(true);
    for (auto& vkImageView : renderDevice->getSwapChainVkImageViews())
    {
        renderTargetBuilder.addImageView(vkImageView);
    }
    auto renderTarget = renderTargetBuilder.build();

    auto gpuTexture = std::make_shared<GpuTexture>(renderDevice);
    gpuTexture->upload(resourcesManager.readResourceData("textures/texture.jpeg"));

    auto uniformBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform, false);
    std::vector<char> bufferData(sizeof(UniformBufferObject), 0);
    uniformBuffer->update(bufferData.data(), sizeof(UniformBufferObject));
    auto renderDescriptor = RenderDescriptorBuilder(renderDevice)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
        .addCombinedImageSampler(gpuTexture, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
        .build();
    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget, renderDescriptor)
        .setVertexShader(resourcesManager.readResourceData("shaders/depth_buffer.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/depth_buffer.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(1, 3 * sizeof(float), VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(2, 6 * sizeof(float), VK_FORMAT_R32G32_SFLOAT)
        .build();

    Mesh mesh;
    mesh._vertices = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };
    mesh._indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };
    auto gpuMesh = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh->update(mesh);
    auto renderStep = std::make_shared<RenderStep>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT: quit = true; break;
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        auto screenSize = renderTarget->getSize();
        float aspectRatio = static_cast<float>(screenSize.y) / static_cast<float>(screenSize.x);
        ubo.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;
        uniformBuffer->update(reinterpret_cast<const char*>(&ubo), sizeof(UniformBufferObject));

        renderDevice->startFrame();
        renderTarget->setFramebufferIndex(renderDevice->getSwapChainCurrentImageIndex());
        renderStep->draw(renderTarget, renderPipeline, gpuMesh);
        renderStep->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }
}

void objMeshSample(SDL_Window* window)
{
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    } ubo;

    auto startTime = std::chrono::high_resolution_clock::now();
    ResourcesManager resourcesManager;

    auto renderInstance = std::make_shared<RenderInstance>(window, true);
    auto renderDevice = std::make_shared<RenderDevice>(renderInstance);
    auto renderTargetBuilder = RenderTargetBuilder(renderDevice)
        .setVkFormat(renderDevice->getSwapChainVkImageFormat())
        .setSize(renderDevice->getSwapChainSize())
        .addMultisampling(renderDevice->getMaxSampleCount())
        .enableDepth(true);
    for (auto& vkImageView : renderDevice->getSwapChainVkImageViews())
    {
        renderTargetBuilder.addImageView(vkImageView);
    }
    auto renderTarget = renderTargetBuilder.build();

    auto gpuTexture = std::make_shared<GpuTexture>(renderDevice, true);
    gpuTexture->upload(resourcesManager.readResourceData("textures/viking_room.png"));

    auto uniformBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform);
    std::vector<char> bufferData(sizeof(UniformBufferObject), 0);
    uniformBuffer->update(bufferData.data(), sizeof(UniformBufferObject));
    auto renderDescriptor = RenderDescriptorBuilder(renderDevice)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
        .addCombinedImageSampler(gpuTexture, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
        .build();
    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget, renderDescriptor)
        .setVertexShader(resourcesManager.readResourceData("shaders/obj_mesh.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/obj_mesh.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(1, 3 * sizeof(float), VK_FORMAT_R32G32_SFLOAT)
        .build();

    auto mesh = resourcesManager.loadObj("models/viking_room.obj");
    auto gpuMesh = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh->update(*mesh);
    auto renderStep = std::make_shared<RenderStep>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT: quit = true; break;
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        auto screenSize = renderTarget->getSize();
        float aspectRatio = static_cast<float>(screenSize.y) / static_cast<float>(screenSize.x);
        ubo.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;
        uniformBuffer->update(renderStep, reinterpret_cast<const char*>(&ubo), sizeof(UniformBufferObject));

        renderDevice->startFrame();
        renderTarget->setFramebufferIndex(renderDevice->getSwapChainCurrentImageIndex());
        renderStep->draw(renderTarget, renderPipeline, gpuMesh);
        renderStep->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }
}

int main(int argc, char *args[])
{
    spdlog::info("Start application");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Window* window = SDL_CreateWindow("vulkan demo", -1, -1, 640, 480, SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

    objMeshSample(window);
    // depthBufferSample(window);
    // textureSample(window);
    // uniformBufferSample(window);
    // indexedMeshSample(window);
    // meshSample(window);
    // triangleSample(window);

    SDL_DestroyWindow(window);
    SDL_Quit();

    spdlog::info("End application");
    return 0;
}

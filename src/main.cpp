#include <SDL.h>
#include <spdlog/spdlog.h>
#include <stb_image.h>
#include <stb_image_write.h>

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
#include "render/render_context.h"
#include "render/gpu_buffer.h"
#include "render/gpu_mesh.h"
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
    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget)
        .setFrontFace(VK_FRONT_FACE_CLOCKWISE)
        .setVertexShader(resourcesManager.readResourceData("shaders/triangle.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/triangle.frag.spv"))
        .build();
    auto renderContext = std::make_shared<RenderContext>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

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
        renderContext->draw(renderTarget, renderPipeline, nullptr);
        renderContext->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
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
    auto renderContext = std::make_shared<RenderContext>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

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
        renderContext->draw(renderTarget, renderPipeline, gpuMesh);
        renderContext->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
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
    auto renderContext = std::make_shared<RenderContext>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

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
        renderContext->draw(renderTarget, renderPipeline, gpuMesh);
        renderContext->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
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

    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget)
        .setVertexShader(resourcesManager.readResourceData("shaders/uniform_buffer.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/uniform_buffer.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT)
        .addVertexAttribute(1, 2 * sizeof(float), VK_FORMAT_R32G32B32_SFLOAT)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
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
    auto renderContext = std::make_shared<RenderContext>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

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
        renderContext->draw(renderTarget, renderPipeline, gpuMesh);
        renderContext->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
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
    gpuTexture->uploadStbImage(resourcesManager.readResourceData("textures/texture.jpeg"));

    auto uniformBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform, false);
    std::vector<char> bufferData(sizeof(UniformBufferObject), 0);
    uniformBuffer->update(bufferData.data(), sizeof(UniformBufferObject));

    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget)
        .setVertexShader(resourcesManager.readResourceData("shaders/texture.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/texture.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT)
        .addVertexAttribute(1, 2 * sizeof(float), VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(2, 5 * sizeof(float), VK_FORMAT_R32G32_SFLOAT)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
        .addCombinedImageSampler(gpuTexture, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
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
    auto renderContext = std::make_shared<RenderContext>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

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
        renderContext->draw(renderTarget, renderPipeline, gpuMesh);
        renderContext->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
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
    gpuTexture->uploadStbImage(resourcesManager.readResourceData("textures/texture.jpeg"));

    auto uniformBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform, false);
    std::vector<char> bufferData(sizeof(UniformBufferObject), 0);
    uniformBuffer->update(bufferData.data(), sizeof(UniformBufferObject));

    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget)
        .setVertexShader(resourcesManager.readResourceData("shaders/depth_buffer.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/depth_buffer.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(1, 3 * sizeof(float), VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(2, 6 * sizeof(float), VK_FORMAT_R32G32_SFLOAT)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
        .addCombinedImageSampler(gpuTexture, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
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
    auto renderContext = std::make_shared<RenderContext>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

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
        renderContext->draw(renderTarget, renderPipeline, gpuMesh);
        renderContext->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
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
    gpuTexture->uploadStbImage(resourcesManager.readResourceData("textures/viking_room.png"));

    auto uniformBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform);
    std::vector<char> bufferData(sizeof(UniformBufferObject), 0);
    uniformBuffer->update(bufferData.data(), sizeof(UniformBufferObject));

    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget)
        .setVertexShader(resourcesManager.readResourceData("shaders/obj_mesh.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/obj_mesh.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(1, 3 * sizeof(float), VK_FORMAT_R32G32_SFLOAT)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
        .addCombinedImageSampler(gpuTexture, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
        .build();

    auto mesh = resourcesManager.loadObj("models/viking_room.obj");
    auto gpuMesh = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh->update(*mesh);
    auto renderContext = std::make_shared<RenderContext>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

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
        uniformBuffer->update(renderContext, reinterpret_cast<const char*>(&ubo), sizeof(UniformBufferObject));

        renderDevice->startFrame();
        renderTarget->setFramebufferIndex(renderDevice->getSwapChainCurrentImageIndex());
        renderContext->draw(renderTarget, renderPipeline, gpuMesh);
        renderContext->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }
}

void renderToTextureSample(SDL_Window* window)
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

    auto renderTarget2 = RenderTargetBuilder(renderDevice)
        .setSize({ 512, 512 })
        .setClearColor({ 0.5f, 0.5f, 0.5f, 1.0f })
        .setVkFormat(VK_FORMAT_R8G8B8A8_SNORM)
        .setFinalColorLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        .addMultisampling(VK_SAMPLE_COUNT_4_BIT)
        .enableDepth(true)
        .build();

    auto gpuTexture = std::make_shared<GpuTexture>(renderDevice, true);
    gpuTexture->uploadStbImage(resourcesManager.readResourceData("textures/viking_room.png"));

    auto uniformBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform);
    std::vector<char> bufferData(sizeof(UniformBufferObject), 0);
    uniformBuffer->update(bufferData.data(), sizeof(UniformBufferObject));
    
    auto renderPipeline2 = RenderPipelineBuilder(renderDevice, renderTarget2)
        .setVertexShader(resourcesManager.readResourceData("shaders/obj_mesh.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/obj_mesh.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(1, 3 * sizeof(float), VK_FORMAT_R32G32_SFLOAT)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
        .addCombinedImageSampler(gpuTexture, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
        .build();

    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget)
        .setVertexShader(resourcesManager.readResourceData("shaders/obj_mesh.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/obj_mesh.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(1, 3 * sizeof(float), VK_FORMAT_R32G32_SFLOAT)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
        .addCombinedImageSampler(renderTarget2->getColorTexture(), VK_SHADER_STAGE_FRAGMENT_BIT, 1)
        .build();

    auto mesh2 = resourcesManager.loadObj("models/viking_room.obj");
    auto gpuMesh2 = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh2->update(*mesh2);

    Mesh mesh;
    mesh._vertices = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    };
    mesh._indices = {
        0, 1, 2, 2, 3, 0,
    };
    auto gpuMesh = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh->update(mesh);

    auto renderContext = std::make_shared<RenderContext>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT: quit = true; break;
            }
        }

        renderDevice->startFrame();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        auto screenSize = renderTarget->getSize();
        float aspectRatio = static_cast<float>(screenSize.y) / static_cast<float>(screenSize.x);
        ubo.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;
        uniformBuffer->update(renderContext, reinterpret_cast<const char*>(&ubo), sizeof(UniformBufferObject));

        renderContext->draw(renderTarget2, renderPipeline2, gpuMesh2);
        
        renderTarget->setFramebufferIndex(renderDevice->getSwapChainCurrentImageIndex());
        renderContext->draw(renderTarget, renderPipeline, gpuMesh);
        renderContext->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }
}

void downloadRendedSample(SDL_Window* window)
{
    glm::ivec2 textureSize = { 512, 512 };
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    } ubo;
    ubo.model = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    float aspectRatio = 1.0f;
    ubo.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    ResourcesManager resourcesManager;

    auto renderInstance = std::make_shared<RenderInstance>(window, true);
    auto renderDevice = std::make_shared<RenderDevice>(renderInstance);
    auto renderTarget = RenderTargetBuilder(renderDevice)
        .setVkFormat(renderDevice->getSwapChainVkImageFormat())
        .setSize(textureSize)
        .addMultisampling(renderDevice->getMaxSampleCount())
        .enableDepth(true)
        .setFinalColorLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
        .build();

    auto gpuTexture = std::make_shared<GpuTexture>(renderDevice, true);
    gpuTexture->uploadStbImage(resourcesManager.readResourceData("textures/viking_room.png"));

    auto uniformBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform, false);
    uniformBuffer->update(reinterpret_cast<const char*>(&ubo), sizeof(UniformBufferObject));

    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget)
        .setVertexShader(resourcesManager.readResourceData("shaders/obj_mesh.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/obj_mesh.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(1, 3 * sizeof(float), VK_FORMAT_R32G32_SFLOAT)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
        .addCombinedImageSampler(gpuTexture, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
        .build();

    auto mesh = resourcesManager.loadObj("models/viking_room.obj");
    auto gpuMesh = std::make_shared<GpuMesh>(renderDevice);
    gpuMesh->update(*mesh);
    
    auto renderingResultBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform, false);
    std::vector<char> imageData(4 * textureSize.x * textureSize.y, 0);
    renderingResultBuffer->update(imageData.data(), imageData.size());

    auto renderContext = std::make_shared<RenderContext>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);
    renderDevice->startFrame();
    renderContext->draw(renderTarget, renderPipeline, gpuMesh);
    renderContext->copyImageToBuffer(renderTarget->getColorTexture(), renderingResultBuffer);
    renderContext->run(VK_NULL_HANDLE, VK_NULL_HANDLE);
    
    renderingResultBuffer->download(imageData.data(), imageData.size());
    stbi_write_png("render_target.png", textureSize.x, textureSize.y, 4, imageData.data(), 4 * textureSize.x);
}

void dynamicTextureSample(SDL_Window* window)
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
        .setClearColor({0.5, 0.5, 0.5, 1.0f})
        .setSize(renderDevice->getSwapChainSize());
    for (auto& vkImageView : renderDevice->getSwapChainVkImageViews())
    {
        renderTargetBuilder.addImageView(vkImageView);
    }
    auto renderTarget = renderTargetBuilder.build();

    int texWidth, texHeight, texChannels;
    auto textureData = resourcesManager.readResourceData("textures/texture.jpeg");
    stbi_uc* pixels = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(textureData.data()), static_cast<int>(textureData.size()), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    glm::ivec2 size = { texWidth, texHeight };
    auto gpuTexture = std::make_shared<GpuTexture>(renderDevice, VK_FORMAT_R8G8B8A8_SNORM, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, size, true);
    auto textureDataBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform, false);
    textureDataBuffer->update(reinterpret_cast<const char*>(pixels), 4 * texWidth * texHeight);

    auto uniformBuffer = std::make_shared<GpuBuffer>(renderDevice, GpuBuffer::Uniform, false);
    std::vector<char> bufferData(sizeof(UniformBufferObject), 0);
    uniformBuffer->update(bufferData.data(), sizeof(UniformBufferObject));

    auto renderPipeline = RenderPipelineBuilder(renderDevice, renderTarget)
        .setVertexShader(resourcesManager.readResourceData("shaders/texture.vert.spv"))
        .setFragmentShader(resourcesManager.readResourceData("shaders/texture.frag.spv"))
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT)
        .addVertexAttribute(1, 2 * sizeof(float), VK_FORMAT_R32G32B32_SFLOAT)
        .addVertexAttribute(2, 5 * sizeof(float), VK_FORMAT_R32G32_SFLOAT)
        .addUniformBuffer(uniformBuffer, VK_SHADER_STAGE_VERTEX_BIT, 0)
        .addCombinedImageSampler(gpuTexture, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
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
    auto renderContext = std::make_shared<RenderContext>(renderDevice, renderDevice->getGraphicsVkQueue().first, renderDevice->getGraphicsVkQueue().second);

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
        renderContext->transitionImageLayout(gpuTexture, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        renderContext->copyBufferToImage(textureDataBuffer, gpuTexture, 0);
        renderContext->transitionImageLayout(gpuTexture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        renderTarget->setFramebufferIndex(renderDevice->getSwapChainCurrentImageIndex());
        renderContext->draw(renderTarget, renderPipeline, gpuMesh);
        renderContext->run(renderDevice->getSwapChainVkSemaphore(), renderTarget->getVkSemaphore());
        renderDevice->endFrame(renderTarget->getVkSemaphore());
    }
}

int main(int argc, char *args[])
{
    spdlog::info("Start application");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Window* window = SDL_CreateWindow("vulkan demo", -1, -1, 640, 480, SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

    // dynamicTextureSample(window);
    // downloadRendedSample(window);
    renderToTextureSample(window);
    // objMeshSample(window);
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

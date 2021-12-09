#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class RenderDevice;
class RenderTarget;
class RenderPipeline;
class RenderDescriptor;
class GpuMesh;
class GpuBuffer;
class GpuTexture;

class RenderStep
{
public:
    RenderStep(const std::shared_ptr<RenderDevice>& renderDevice, VkQueue vkQueue, uint32_t vkFamilyIndex);

    ~RenderStep();

    void draw(
              const std::shared_ptr<RenderTarget>& renderTarget,
              const std::shared_ptr<RenderPipeline>& renderPipeline,
              const std::shared_ptr<GpuMesh>& gpuMesh);

    void copyBufferToImage(const std::shared_ptr<GpuBuffer>& gpuBuffer, const std::shared_ptr<GpuTexture>& gpuTexture, uint32_t mipLevel = 0);

    void copyImageToBuffer(const std::shared_ptr<GpuTexture>& gpuTexture, const std::shared_ptr<GpuBuffer>& gpuBuffer, uint32_t mipLevel = 0);
    
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void transitionImageLayout(const std::shared_ptr<GpuTexture>& gpuTexture, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout, uint32_t mipsLevel = 1);

    void generateMipmaps(const std::shared_ptr<GpuTexture>& gpuTexture);

    void run(VkSemaphore waitVkSemaphore, VkSemaphore signalVkSemaphore);

public:
    void init();
    
    void initCommandBuffer();

    void destroy();
    
    void destroyCommandBuffer();

    std::shared_ptr<RenderDevice> _renderDevice;
    VkQueue _vkQueue = VK_NULL_HANDLE;
    uint32_t _vkFamilyIndex = 0;
    VkCommandPool _vkCommandPool = VK_NULL_HANDLE;
    VkCommandBuffer _vkCommandBuffer = VK_NULL_HANDLE;
};

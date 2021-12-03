#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class RenderDevice;
class GpuBuffer;
class GpuTexture;
class RenderDescriptor;

class RenderDescriptorBuilder
{
public:
    struct UniformBuffer
    {
        std::shared_ptr<GpuBuffer> _gpuBuffer;
        VkShaderStageFlags _vkShaderStageFlags;
        uint32_t _binding;
    };

    struct CombinedImageSampler
    {
        std::shared_ptr<GpuTexture> _gpuTexture;
        VkShaderStageFlags _vkShaderStageFlags;
        uint32_t _binding;
    };
    
    RenderDescriptorBuilder(const std::shared_ptr<RenderDevice>& renderDevice);

    ~RenderDescriptorBuilder();

    RenderDescriptorBuilder& addUniformBuffer(const std::shared_ptr<GpuBuffer>& gpuBuffer, VkShaderStageFlags vkShaderStageFlags, uint32_t binding);

    RenderDescriptorBuilder& addCombinedImageSampler(const std::shared_ptr<GpuTexture>& gpuTexture, VkShaderStageFlags vkShaderStageFlags, uint32_t binding);

    std::shared_ptr<RenderDescriptor> build();

public:
    std::vector<UniformBuffer> _uniformBuffers;
    std::vector<CombinedImageSampler> _combinedImageSamplers;
    
private:
    std::shared_ptr<RenderDevice> _renderDevice;
};

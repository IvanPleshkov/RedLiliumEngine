#include "render_descriptor_builder.h"
#include "render_descriptor.h"
#include "render_device.h"
#include "gpu_buffer.h"
#include "gpu_texture.h"

RenderDescriptorBuilder::RenderDescriptorBuilder(const std::shared_ptr<RenderDevice>& renderDevice)
    : _renderDevice(renderDevice)
{
    
}

RenderDescriptorBuilder::~RenderDescriptorBuilder()
{ }

RenderDescriptorBuilder& RenderDescriptorBuilder::addUniformBuffer(const std::shared_ptr<GpuBuffer>& gpuBuffer, VkShaderStageFlags vkShaderStageFlags, uint32_t binding)
{
    UniformBuffer uniformBuffer;
    uniformBuffer._gpuBuffer = gpuBuffer;
    uniformBuffer._vkShaderStageFlags = vkShaderStageFlags;
    uniformBuffer._binding = binding;
    _uniformBuffers.push_back(uniformBuffer);
    return *this;
}

RenderDescriptorBuilder& RenderDescriptorBuilder::addCombinedImageSampler(const std::shared_ptr<GpuTexture>& gpuTexture, VkShaderStageFlags vkShaderStageFlags, uint32_t binding)
{
    CombinedImageSampler combinedImageSampler;
    combinedImageSampler._gpuTexture = gpuTexture;
    combinedImageSampler._binding = binding;
    combinedImageSampler._vkShaderStageFlags = vkShaderStageFlags;
    _combinedImageSamplers.push_back(combinedImageSampler);
    return *this;
}

std::shared_ptr<RenderDescriptor> RenderDescriptorBuilder::build()
{
    return std::make_shared<RenderDescriptor>(_renderDevice, *this);
}

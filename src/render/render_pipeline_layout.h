#pragma once

#include "render_pipeline_builder.h"
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class RenderDevice;
class GpuBuffer;
class RenderPipeline;

class RenderPipelineLayout
{
public:
    RenderPipelineLayout(const std::shared_ptr<RenderDevice>& renderDevice, const std::vector<UniformBufferDescription>& uniformBufferDescriptions, const std::vector<CombinedImageSamplerDescription>& combinedImageSamplerDescriptions);

    ~RenderPipelineLayout();

    void bind(VkCommandBuffer vkCommandBuffer) const;
    
    VkPipelineLayout getVkPipelineLayout() const;

private:
    void init(const std::vector<UniformBufferDescription>& uniformBufferDescriptions, const std::vector<CombinedImageSamplerDescription>& combinedImageSamplerDescriptions);
    
    void initDescriptorSetLayout(const std::vector<UniformBufferDescription>& uniformBufferDescriptions, const std::vector<CombinedImageSamplerDescription>& combinedImageSamplerDescriptions);
    
    void initDescriptorPool(const std::vector<UniformBufferDescription>& uniformBufferDescriptions, const std::vector<CombinedImageSamplerDescription>& combinedImageSamplerDescriptions);
    
    void initDescriptorSets(const std::vector<UniformBufferDescription>& uniformBufferDescriptions, const std::vector<CombinedImageSamplerDescription>& combinedImageSamplerDescriptions);

    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    VkDescriptorPool _vkDescriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSetLayout> _vkDescriptorSetLayouts;
    VkDescriptorSet _vkDescriptorSet = VK_NULL_HANDLE;
    VkPipelineLayout _vkPipelineLayout = VK_NULL_HANDLE;
};

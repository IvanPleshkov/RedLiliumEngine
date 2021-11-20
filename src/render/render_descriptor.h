#pragma once

#include <vulkan/vulkan.h>
#include <memory>

class RenderDevice;
class GpuBuffer;
class RenderPipeline;

class RenderDescriptor
{
public:
    RenderDescriptor(const std::shared_ptr<RenderDevice>& renderDevice, VkShaderStageFlags vkStage, uint32_t binding, size_t size);

    ~RenderDescriptor();
    
    void update(const char* data);
    
    uint32_t binding() const;

    void bind(VkCommandBuffer vkCommandBuffer, VkPipelineLayout vkPipelineLayout) const;
    
    VkDescriptorSetLayout getVkDescriptorSetLayout() const;

private:
    void init();
    
    void initDescriptorSetLayout();
    
    void initDescriptorPool();
    
    void initDescriptorSets();

    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    std::shared_ptr<GpuBuffer> _gpuBuffer;
    VkShaderStageFlags _vkStage;
    uint32_t _binding;
    size_t _size;
    VkDescriptorPool _vkDescriptorPool = VK_NULL_HANDLE;
    VkDescriptorSetLayout _vkDescriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorSet _vkDescriptorSet = VK_NULL_HANDLE;
};

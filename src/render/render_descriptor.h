#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class RenderDevice;
class GpuBuffer;
class RenderPipeline;
class RenderDescriptorBuilder;

class RenderDescriptor
{
public:
    RenderDescriptor(const std::shared_ptr<RenderDevice>& renderDevice, const RenderDescriptorBuilder& renderDescriptorBuilder);

    ~RenderDescriptor();

    void bind(VkCommandBuffer vkCommandBuffer) const;
    
    VkPipelineLayout getVkPipelineLayout() const;

private:
    void init(const RenderDescriptorBuilder& renderDescriptorBuilder);
    
    void initDescriptorSetLayout(const RenderDescriptorBuilder& renderDescriptorBuilder);
    
    void initDescriptorPool(const RenderDescriptorBuilder& renderDescriptorBuilder);
    
    void initDescriptorSets(const RenderDescriptorBuilder& renderDescriptorBuilder);

    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    VkDescriptorPool _vkDescriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSetLayout> _vkDescriptorSetLayouts;
    VkDescriptorSet _vkDescriptorSet = VK_NULL_HANDLE;
    VkPipelineLayout _vkPipelineLayout = VK_NULL_HANDLE;
};

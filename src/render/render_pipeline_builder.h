#pragma once

#include "render_pipeline.h"
#include <vulkan/vulkan.h>
#include <string_view>
#include <memory>
#include <vector>

class RenderDevice;
class RenderTarget;

class RenderPipelineBuilder final
{
public:
    RenderPipelineBuilder(const std::shared_ptr<RenderDevice>& renderDevice, const std::shared_ptr<RenderTarget>& renderTarget);

    RenderPipelineBuilder& setVertexShader(std::string_view spirvShader);

    RenderPipelineBuilder& setFragmentShader(std::string_view spirvShader);

    RenderPipelineBuilder& addVertexAttribute(uint32_t location, uint32_t offset, VkFormat format);

    RenderPipelineBuilder& addUniformBuffer(VkShaderStageFlags vkStage, uint32_t binding);

    std::shared_ptr<RenderPipeline> build();

private:
    VkShaderModule createShaderModule(std::string_view spirvShader);

    std::shared_ptr<RenderDevice> _renderDevice;
    std::shared_ptr<RenderTarget> _renderTarget;
    std::vector<VkVertexInputAttributeDescription> _vkVertexInputAttributeDescriptions;
    uint32_t _vertexInputBindingSize = 0;
    std::vector<VkDescriptorSetLayoutBinding> _vkDescriptorSetLayoutBindings;
    std::string_view _vertexShader;
    std::string_view _fragmentShader;
};

#pragma once

#include <vulkan/vulkan.h>
#include <string_view>
#include <memory>
#include <vector>

class RenderDevice;
class RenderTarget;
class RenderPipeline;
class RenderPipelineLayout;
class GpuBuffer;
class GpuTexture;

struct UniformBufferDescription
{
    std::shared_ptr<GpuBuffer> _gpuBuffer;
    VkShaderStageFlags _vkShaderStageFlags;
    uint32_t _binding;
};

struct CombinedImageSamplerDescription
{
    std::shared_ptr<GpuTexture> _gpuTexture;
    VkShaderStageFlags _vkShaderStageFlags;
    uint32_t _binding;
};

class RenderPipelineBuilder final
{
public:
    RenderPipelineBuilder(const std::shared_ptr<RenderDevice>& renderDevice, const std::shared_ptr<RenderTarget>& renderTarget);

    ~RenderPipelineBuilder();

    RenderPipelineBuilder& setVertexShader(std::string_view spirvShader);

    RenderPipelineBuilder& setFragmentShader(std::string_view spirvShader);

    RenderPipelineBuilder& addVertexAttribute(uint32_t location, uint32_t offset, VkFormat format);

    RenderPipelineBuilder& addUniformBuffer(const std::shared_ptr<GpuBuffer>& gpuBuffer, VkShaderStageFlags vkShaderStageFlags, uint32_t binding);

    RenderPipelineBuilder& addCombinedImageSampler(const std::shared_ptr<GpuTexture>& gpuTexture, VkShaderStageFlags vkShaderStageFlags, uint32_t binding);

    RenderPipelineBuilder& setFrontFace(VkFrontFace vkFrontFace);

    std::shared_ptr<RenderPipeline> build();

public:
    std::vector<UniformBufferDescription> _uniformBufferDescriptions;
    std::vector<CombinedImageSamplerDescription> _combinedImageSamplerDescriptions;
    std::vector<VkVertexInputAttributeDescription> _vkVertexInputAttributeDescriptions;
    std::vector<VkPipelineShaderStageCreateInfo> _vkPipelineShaderStageCreateInfos;
    std::vector<VkVertexInputBindingDescription> _vkVertexInputBindingDescriptions;
    VkPipelineVertexInputStateCreateInfo _vkPipelineVertexInputStateCreateInfo{};
    VkPipelineInputAssemblyStateCreateInfo _vkPipelineInputAssemblyStateCreateInfo{};
    VkViewport _vkViewport{};
    VkRect2D _vkScissor{};
    VkPipelineViewportStateCreateInfo _vkPipelineViewportStateCreateInfo{};
    VkPipelineDepthStencilStateCreateInfo _vkPipelineDepthStencilStateCreateInfo{};
    VkPipelineRasterizationStateCreateInfo _vkPipelineRasterizationStateCreateInfo{};
    VkPipelineMultisampleStateCreateInfo _vkPipelineMultisampleStateCreateInfo{};
    std::vector<VkPipelineColorBlendAttachmentState> _vkPipelineColorBlendAttachmentStates;
    VkPipelineColorBlendStateCreateInfo _vkPipelineColorBlendStateCreateInfo{};
    VkGraphicsPipelineCreateInfo _vkGraphicsPipelineCreateInfo{};

private:
    VkShaderModule createShaderModule(std::string_view spirvShader);

    std::shared_ptr<RenderDevice> _renderDevice;
    std::shared_ptr<RenderTarget> _renderTarget;
};

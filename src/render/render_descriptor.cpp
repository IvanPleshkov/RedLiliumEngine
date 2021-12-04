#include "render_descriptor.h"
#include "render_descriptor_builder.h"
#include "gpu_buffer.h"
#include "gpu_texture.h"
#include "render_device.h"
#include "render_pipeline.h"

RenderDescriptor::RenderDescriptor(const std::shared_ptr<RenderDevice>& renderDevice, const RenderDescriptorBuilder& renderDescriptorBuilder)
    : _renderDevice(renderDevice)
{
    init(renderDescriptorBuilder);
}

RenderDescriptor::~RenderDescriptor()
{
    destroy();
}

void RenderDescriptor::bind(VkCommandBuffer vkCommandBuffer) const
{
    if (_vkDescriptorSet != VK_NULL_HANDLE)
    {
        vkCmdBindDescriptorSets(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _vkPipelineLayout, 0, 1, &_vkDescriptorSet, 0, nullptr);
    }
}

VkPipelineLayout RenderDescriptor::getVkPipelineLayout() const
{
    return _vkPipelineLayout;
}

void RenderDescriptor::init(const RenderDescriptorBuilder& renderDescriptorBuilder)
{
    if (!renderDescriptorBuilder._uniformBuffers.empty() || !renderDescriptorBuilder._combinedImageSamplers.empty())
    {
        initDescriptorSetLayout(renderDescriptorBuilder);
        initDescriptorPool(renderDescriptorBuilder);
        initDescriptorSets(renderDescriptorBuilder);
    }

    VkPipelineLayoutCreateInfo _vkPipelineLayoutCreateInfo{};
    _vkPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    if (!_vkDescriptorSetLayouts.empty())
    {
        _vkPipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(_vkDescriptorSetLayouts.size());
        _vkPipelineLayoutCreateInfo.pSetLayouts = _vkDescriptorSetLayouts.data();
    }
    else
    {
        _vkPipelineLayoutCreateInfo.setLayoutCount = 0;
        _vkPipelineLayoutCreateInfo.pSetLayouts = nullptr;
    }
    _vkPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    _vkPipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(_renderDevice->getVkDevice(), &_vkPipelineLayoutCreateInfo, _renderDevice->allocator(), &_vkPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout!");
    }
}

void RenderDescriptor::initDescriptorSetLayout(const RenderDescriptorBuilder& renderDescriptorBuilder)
{
    std::vector<VkDescriptorSetLayoutBinding> vkDescriptorSetLayoutBindings;
    for (auto& uniformBuffer : renderDescriptorBuilder._uniformBuffers)
    {
        VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding{};
        vkDescriptorSetLayoutBinding.binding = uniformBuffer._binding;
        vkDescriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        vkDescriptorSetLayoutBinding.descriptorCount = 1;
        vkDescriptorSetLayoutBinding.stageFlags = uniformBuffer._vkShaderStageFlags;
        vkDescriptorSetLayoutBinding.pImmutableSamplers = nullptr;
        vkDescriptorSetLayoutBindings.push_back(vkDescriptorSetLayoutBinding);
    }

    for (auto& combinedImageSampler : renderDescriptorBuilder._combinedImageSamplers)
    {
        VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding{};
        vkDescriptorSetLayoutBinding.binding = combinedImageSampler._binding;
        vkDescriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        vkDescriptorSetLayoutBinding.descriptorCount = 1;
        vkDescriptorSetLayoutBinding.stageFlags = combinedImageSampler._vkShaderStageFlags;
        vkDescriptorSetLayoutBinding.pImmutableSamplers = nullptr;
        vkDescriptorSetLayoutBindings.push_back(vkDescriptorSetLayoutBinding);
    }
    
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(vkDescriptorSetLayoutBindings.size());
    layoutInfo.pBindings = vkDescriptorSetLayoutBindings.data();

    VkDescriptorSetLayout vkDescriptorSetLayout;
    if (vkCreateDescriptorSetLayout(_renderDevice->getVkDevice(), &layoutInfo, _renderDevice->allocator(), &vkDescriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
    _vkDescriptorSetLayouts.push_back(vkDescriptorSetLayout);
}

void RenderDescriptor::initDescriptorPool(const RenderDescriptorBuilder& renderDescriptorBuilder)
{
    std::vector<VkDescriptorPoolSize> vkDescriptorPoolSizes;
    if (!renderDescriptorBuilder._uniformBuffers.empty())
    {
        VkDescriptorPoolSize vkDescriptorPoolSize{};
        vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        vkDescriptorPoolSize.descriptorCount = static_cast<uint32_t>(renderDescriptorBuilder._uniformBuffers.size());
        vkDescriptorPoolSizes.push_back(vkDescriptorPoolSize);
    }
    if (!renderDescriptorBuilder._combinedImageSamplers.empty())
    {
        VkDescriptorPoolSize vkDescriptorPoolSize{};
        vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        vkDescriptorPoolSize.descriptorCount = static_cast<uint32_t>(renderDescriptorBuilder._combinedImageSamplers.size());
        vkDescriptorPoolSizes.push_back(vkDescriptorPoolSize);
    }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = vkDescriptorPoolSizes.size();
    poolInfo.pPoolSizes = vkDescriptorPoolSizes.data();
    poolInfo.maxSets = 1;

    if (vkCreateDescriptorPool(_renderDevice->getVkDevice(), &poolInfo, _renderDevice->allocator(), &_vkDescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void RenderDescriptor::initDescriptorSets(const RenderDescriptorBuilder& renderDescriptorBuilder)
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _vkDescriptorPool;
    if (!_vkDescriptorSetLayouts.empty())
    {
        allocInfo.descriptorSetCount = static_cast<uint32_t>(_vkDescriptorSetLayouts.size());
        allocInfo.pSetLayouts = _vkDescriptorSetLayouts.data();
    }
    else
    {
        allocInfo.descriptorSetCount = 0;
        allocInfo.pSetLayouts = nullptr;
    }
    
    if (vkAllocateDescriptorSets(_renderDevice->getVkDevice(), &allocInfo, &_vkDescriptorSet) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    std::vector<VkWriteDescriptorSet> vkWriteDescriptorSets;
    std::vector<VkDescriptorBufferInfo> vkDescriptorBufferInfos;
    for (auto& uniformBuffer : renderDescriptorBuilder._uniformBuffers)
    {
        VkDescriptorBufferInfo vkDescriptorBufferInfo{};
        vkDescriptorBufferInfo.buffer = uniformBuffer._gpuBuffer->getVkBuffer();
        vkDescriptorBufferInfo.offset = 0;
        vkDescriptorBufferInfo.range = static_cast<uint32_t>(uniformBuffer._gpuBuffer->getSize());

        VkWriteDescriptorSet vkWriteDescriptorSet{};
        vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        vkWriteDescriptorSet.dstSet = _vkDescriptorSet;
        vkWriteDescriptorSet.dstBinding = uniformBuffer._binding;
        vkWriteDescriptorSet.dstArrayElement = 0;
        vkWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        vkWriteDescriptorSet.descriptorCount = 1;
        vkWriteDescriptorSet.pBufferInfo = &vkDescriptorBufferInfo;
        vkWriteDescriptorSet.pImageInfo = nullptr;
        vkWriteDescriptorSet.pTexelBufferView = nullptr;

        vkWriteDescriptorSets.push_back(vkWriteDescriptorSet);
    }

    for (auto& combinedImageSampler : renderDescriptorBuilder._combinedImageSamplers)
    {
        VkDescriptorImageInfo vkDescriptorImageInfo{};
        vkDescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        vkDescriptorImageInfo.imageView = combinedImageSampler._gpuTexture->getVkImageView();
        vkDescriptorImageInfo.sampler = combinedImageSampler._gpuTexture->getVkSampler();

        VkWriteDescriptorSet vkWriteDescriptorSet{};
        vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        vkWriteDescriptorSet.dstSet = _vkDescriptorSet;
        vkWriteDescriptorSet.dstBinding = combinedImageSampler._binding;
        vkWriteDescriptorSet.dstArrayElement = 0;
        vkWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        vkWriteDescriptorSet.descriptorCount = 1;
        vkWriteDescriptorSet.pImageInfo = &vkDescriptorImageInfo;
    
        vkWriteDescriptorSets.push_back(vkWriteDescriptorSet);
    }
    vkUpdateDescriptorSets(_renderDevice->getVkDevice(), static_cast<uint32_t>(vkWriteDescriptorSets.size()), vkWriteDescriptorSets.data(), 0, nullptr);
}

void RenderDescriptor::destroy()
{
    if (_vkPipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(_renderDevice->getVkDevice(), _vkPipelineLayout, _renderDevice->allocator());
        _vkPipelineLayout = VK_NULL_HANDLE;
    }
    if (_vkDescriptorPool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(_renderDevice->getVkDevice(), _vkDescriptorPool, _renderDevice->allocator());
        _vkDescriptorPool = VK_NULL_HANDLE;
    }
    for (const auto& vkDescriptorSetLayout : _vkDescriptorSetLayouts)
    {
        vkDestroyDescriptorSetLayout(_renderDevice->getVkDevice(), vkDescriptorSetLayout, _renderDevice->allocator());
    }
    _vkDescriptorSetLayouts.clear();
}

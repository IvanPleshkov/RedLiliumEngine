#include "render_descriptor.h"
#include "gpu_buffer.h"
#include "render_device.h"
#include "render_pipeline.h"

RenderDescriptor::RenderDescriptor(const std::shared_ptr<RenderDevice>& renderDevice, VkShaderStageFlags vkStage, uint32_t binding, size_t size)
    : _renderDevice(renderDevice)
    , _vkStage(vkStage)
    , _binding(binding)
    , _size(size)
{
    init();
}

RenderDescriptor::~RenderDescriptor()
{
    destroy();
}

void RenderDescriptor::update(const char* data)
{
    _gpuBuffer->update(data, _size);
}

uint32_t RenderDescriptor::binding() const
{
    return _binding;
}

void RenderDescriptor::bind(VkCommandBuffer vkCommandBuffer, VkPipelineLayout vkPipelineLayout) const
{
    vkCmdBindDescriptorSets(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &_vkDescriptorSet, 0, nullptr);
}

const VkDescriptorSetLayout* RenderDescriptor::getVkDescriptorSetLayout() const
{
    return &_vkDescriptorSetLayout;
}

void RenderDescriptor::init()
{
    _gpuBuffer = std::make_shared<GpuBuffer>(_renderDevice, GpuBuffer::Uniform, false);
    std::vector<char> bufferData(_size, 0);
    _gpuBuffer->update(bufferData.data(), _size);

    initDescriptorSetLayout();
    initDescriptorPool();
    initDescriptorSets();
}

void RenderDescriptor::initDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(_renderDevice->getVkDevice(), &layoutInfo, _renderDevice->allocator(), &_vkDescriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void RenderDescriptor::initDescriptorPool()
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 1;
    
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 1;

    if (vkCreateDescriptorPool(_renderDevice->getVkDevice(), &poolInfo, _renderDevice->allocator(), &_vkDescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void RenderDescriptor::initDescriptorSets()
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _vkDescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &_vkDescriptorSetLayout;
    
    if (vkAllocateDescriptorSets(_renderDevice->getVkDevice(), &allocInfo, &_vkDescriptorSet) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
    
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = _gpuBuffer->getVkBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = static_cast<uint32_t>(_size);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = _vkDescriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr; // Optional
    descriptorWrite.pTexelBufferView = nullptr; // Optional
    vkUpdateDescriptorSets(_renderDevice->getVkDevice(), 1, &descriptorWrite, 0, nullptr);
}

void RenderDescriptor::destroy()
{
    if (_vkDescriptorPool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(_renderDevice->getVkDevice(), _vkDescriptorPool, _renderDevice->allocator());
        _vkDescriptorPool = VK_NULL_HANDLE;
    }
    if (_vkDescriptorSetLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(_renderDevice->getVkDevice(), _vkDescriptorSetLayout, _renderDevice->allocator());
        _vkDescriptorSetLayout = VK_NULL_HANDLE;
    }
    _gpuBuffer = nullptr;
}

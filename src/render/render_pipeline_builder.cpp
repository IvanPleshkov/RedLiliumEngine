#include "render_pipeline_builder.h"
#include "render_pipeline.h"
#include "render_device.h"
#include "render_instance.h"
#include "render_target.h"
#include "render_descriptor.h"
#include "render_descriptor_builder.h"
#include <stdexcept>

namespace
{
const char* g_shaderMainFunctionName = "main";
}

RenderPipelineBuilder::RenderPipelineBuilder(const std::shared_ptr<RenderDevice>& renderDevice, const std::shared_ptr<RenderTarget>& renderTarget, const std::shared_ptr<RenderDescriptor>& renderDescriptor)
    : _renderDevice(renderDevice)
    , _renderTarget(renderTarget)
    , _renderDescriptor(renderDescriptor)
    , _vkGraphicsPipelineCreateInfo{}
    , _vkPipelineColorBlendStateCreateInfo{}
    , _vkPipelineMultisampleStateCreateInfo{}
    , _vkPipelineRasterizationStateCreateInfo{}
    , _vkPipelineDepthStencilStateCreateInfo{}
    , _vkPipelineViewportStateCreateInfo{}
    , _vkScissor{}
    , _vkViewport{}
    , _vkPipelineInputAssemblyStateCreateInfo{}
    , _vkPipelineVertexInputStateCreateInfo{}
{
    if (_renderDescriptor == nullptr)
    {
        _renderDescriptor = RenderDescriptorBuilder(_renderDevice).build();
    }

    _vkPipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    _vkPipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
    _vkPipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = nullptr;
    _vkPipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;
    _vkPipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = nullptr;

    _vkPipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    _vkPipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
    _vkPipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    _vkPipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    _vkPipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
    _vkPipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    _vkPipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    _vkPipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    _vkPipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f; // Optional
    _vkPipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f; // Optional
    _vkPipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f; // Optional

    _vkPipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    _vkPipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    _vkPipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

    _vkViewport.x = 0.0f;
    _vkViewport.y = 0.0f;
    _vkViewport.width = static_cast<float>(_renderTarget->getSize().x);
    _vkViewport.height = static_cast<float>(_renderTarget->getSize().y);
    _vkViewport.minDepth = 0.0f;
    _vkViewport.maxDepth = 1.0f;
    
    _vkScissor.offset = {0, 0};
    _vkScissor.extent.width = static_cast<uint32_t>(_renderTarget->getSize().x);
    _vkScissor.extent.height = static_cast<uint32_t>(_renderTarget->getSize().y);

    _vkPipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    _vkPipelineViewportStateCreateInfo.viewportCount = 1;
    _vkPipelineViewportStateCreateInfo.pViewports = &_vkViewport;
    _vkPipelineViewportStateCreateInfo.scissorCount = 1;
    _vkPipelineViewportStateCreateInfo.pScissors = &_vkScissor;

    _vkPipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    _vkPipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    _vkPipelineMultisampleStateCreateInfo.rasterizationSamples = _renderTarget->getVkSampleCount();
    _vkPipelineMultisampleStateCreateInfo.minSampleShading = 1.0f; // Optional
    _vkPipelineMultisampleStateCreateInfo.pSampleMask = nullptr; // Optional
    _vkPipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
    _vkPipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE; // Optional
    
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    _vkPipelineColorBlendAttachmentStates.push_back(colorBlendAttachment);

    _vkPipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    _vkPipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
    _vkPipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
    if (!_vkPipelineColorBlendAttachmentStates.empty())
    {
        _vkPipelineColorBlendStateCreateInfo.attachmentCount = static_cast<uint32_t>(_vkPipelineColorBlendAttachmentStates.size());
        _vkPipelineColorBlendStateCreateInfo.pAttachments = _vkPipelineColorBlendAttachmentStates.data();
    }
    else
    {
        _vkPipelineColorBlendStateCreateInfo.attachmentCount = 0;
        _vkPipelineColorBlendStateCreateInfo.pAttachments = nullptr;
    }
    _vkPipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f;
    _vkPipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f;
    _vkPipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f;
    _vkPipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f;
    
    _vkGraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    _vkGraphicsPipelineCreateInfo.pVertexInputState = &_vkPipelineVertexInputStateCreateInfo;
    _vkGraphicsPipelineCreateInfo.pInputAssemblyState = &_vkPipelineInputAssemblyStateCreateInfo;
    _vkGraphicsPipelineCreateInfo.pViewportState = &_vkPipelineViewportStateCreateInfo;
    _vkGraphicsPipelineCreateInfo.pRasterizationState = &_vkPipelineRasterizationStateCreateInfo;
    _vkGraphicsPipelineCreateInfo.pMultisampleState = &_vkPipelineMultisampleStateCreateInfo;
    _vkGraphicsPipelineCreateInfo.pDepthStencilState = nullptr; // Optional
    _vkGraphicsPipelineCreateInfo.pColorBlendState = &_vkPipelineColorBlendStateCreateInfo;
    _vkGraphicsPipelineCreateInfo.pDynamicState = nullptr; // Optional
    _vkGraphicsPipelineCreateInfo.layout = _renderDescriptor->getVkPipelineLayout();
    _vkGraphicsPipelineCreateInfo.renderPass = _renderTarget->getVkRenderPass();
    _vkGraphicsPipelineCreateInfo.subpass = 0;
    _vkGraphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    _vkGraphicsPipelineCreateInfo.basePipelineIndex = -1; // Optional

    if (_renderTarget->hasDepth())
    {
        _vkPipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        _vkPipelineDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
        _vkPipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
        _vkPipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        _vkPipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
        _vkPipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0f;
        _vkPipelineDepthStencilStateCreateInfo.maxDepthBounds = 1.0f;
        _vkPipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
        _vkPipelineDepthStencilStateCreateInfo.front = {};
        _vkPipelineDepthStencilStateCreateInfo.back = {};
        _vkGraphicsPipelineCreateInfo.pDepthStencilState = &_vkPipelineDepthStencilStateCreateInfo;
    }
}

RenderPipelineBuilder::~RenderPipelineBuilder()
{
    for (auto& vkPipelineShaderStageCreateInfo : _vkPipelineShaderStageCreateInfos)
    {
        vkDestroyShaderModule(_renderDevice->getVkDevice(), vkPipelineShaderStageCreateInfo.module, _renderDevice->allocator());
        vkPipelineShaderStageCreateInfo.module = VK_NULL_HANDLE;
    }
}

RenderPipelineBuilder& RenderPipelineBuilder::setVertexShader(std::string_view spirvShader)
{
    VkPipelineShaderStageCreateInfo vkPipelineShaderStageCreateInfo{};
    vkPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vkPipelineShaderStageCreateInfo.module = createShaderModule(spirvShader);
    vkPipelineShaderStageCreateInfo.pName = g_shaderMainFunctionName;
    _vkPipelineShaderStageCreateInfos.push_back(vkPipelineShaderStageCreateInfo);

    _vkGraphicsPipelineCreateInfo.stageCount = static_cast<uint32_t>(_vkPipelineShaderStageCreateInfos.size());
    _vkGraphicsPipelineCreateInfo.pStages = _vkPipelineShaderStageCreateInfos.data();
    return *this;
}

RenderPipelineBuilder& RenderPipelineBuilder::setFragmentShader(std::string_view spirvShader)
{
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = createShaderModule(spirvShader);
    fragShaderStageInfo.pName = g_shaderMainFunctionName;
    _vkPipelineShaderStageCreateInfos.push_back(fragShaderStageInfo);

    _vkGraphicsPipelineCreateInfo.stageCount = static_cast<uint32_t>(_vkPipelineShaderStageCreateInfos.size());
    _vkGraphicsPipelineCreateInfo.pStages = _vkPipelineShaderStageCreateInfos.data();
    return *this;
}

RenderPipelineBuilder& RenderPipelineBuilder::addVertexAttribute(uint32_t location, uint32_t offset, VkFormat format)
{
    uint32_t size = sizeof(float);
    switch (format)
    {
        case VK_FORMAT_R32G32_SFLOAT:
            size = 2 * sizeof(float);
            break;
        case VK_FORMAT_R32G32B32_SFLOAT:
            size = 3 * sizeof(float);
            break;
        default:
            throw std::runtime_error("add please format size");
    }

    VkVertexInputAttributeDescription vkVertexInputAttributeDescription{};
    vkVertexInputAttributeDescription.binding = 0;
    vkVertexInputAttributeDescription.location = location;
    vkVertexInputAttributeDescription.format = format;
    vkVertexInputAttributeDescription.offset = offset;
    _vkVertexInputAttributeDescriptions.push_back(vkVertexInputAttributeDescription);

    if (_vkVertexInputBindingDescriptions.empty())
    {
        VkVertexInputBindingDescription vkVertexInputBindingDescription{};
        vkVertexInputBindingDescription.binding = 0;
        vkVertexInputBindingDescription.stride = 0;
        vkVertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        _vkVertexInputBindingDescriptions.push_back(vkVertexInputBindingDescription);
        
        _vkPipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(_vkVertexInputBindingDescriptions.size());
        _vkPipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = _vkVertexInputBindingDescriptions.data();
    }

    _vkVertexInputBindingDescriptions.front().stride = std::max(_vkVertexInputBindingDescriptions.front().stride, offset + size);

    _vkPipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(_vkVertexInputAttributeDescriptions.size());
    _vkPipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = _vkVertexInputAttributeDescriptions.data();

    return *this;
}

RenderPipelineBuilder& RenderPipelineBuilder::setFrontFace(VkFrontFace vkFrontFace)
{
    _vkPipelineRasterizationStateCreateInfo.frontFace = vkFrontFace;
    return *this;
}

std::shared_ptr<RenderPipeline> RenderPipelineBuilder::build()
{
    return std::make_shared<RenderPipeline>(_renderDevice, _renderDescriptor, *this);
}

VkShaderModule RenderPipelineBuilder::createShaderModule(std::string_view spirvShader)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = spirvShader.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(spirvShader.data());
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(_renderDevice->getVkDevice(), &createInfo, _renderDevice->allocator(), &shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader module!");
    }
    return shaderModule;
}

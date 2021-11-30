#include "render_pipeline_builder.h"
#include "render_pipeline.h"
#include "render_device.h"
#include "render_instance.h"
#include "render_target.h"
#include "render_descriptor.h"
#include <stdexcept>

RenderPipelineBuilder::RenderPipelineBuilder(const std::shared_ptr<RenderDevice>& renderDevice, const std::shared_ptr<RenderTarget>& renderTarget)
    : _renderDevice(renderDevice)
    , _renderTarget(renderTarget)
{
}

RenderPipelineBuilder& RenderPipelineBuilder::setVertexShader(std::string_view spirvShader)
{
    _vertexShader = spirvShader;
    return *this;
}

RenderPipelineBuilder& RenderPipelineBuilder::setFragmentShader(std::string_view spirvShader)
{
    _fragmentShader = spirvShader;
    return *this;
}

RenderPipelineBuilder& RenderPipelineBuilder::addVertexAttribute(uint32_t location, uint32_t offset, VkFormat format)
{
    VkVertexInputAttributeDescription vkVertexInputAttributeDescription{};
    vkVertexInputAttributeDescription.binding = 0;
    vkVertexInputAttributeDescription.location = location;
    vkVertexInputAttributeDescription.format = format;
    vkVertexInputAttributeDescription.offset = offset;
    _vkVertexInputAttributeDescriptions.push_back(vkVertexInputAttributeDescription);
    
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
    _vertexInputBindingSize = std::max(_vertexInputBindingSize, offset + size);
    return *this;
}

RenderPipelineBuilder& RenderPipelineBuilder::addUniformBuffer(VkShaderStageFlags vkStage, uint32_t binding, uint32_t size)
{
    _renderDescriptors.push_back(std::make_shared<RenderDescriptor>(_renderDevice, vkStage, binding, size));
    return *this;
}

std::shared_ptr<RenderPipeline> RenderPipelineBuilder::build()
{
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = createShaderModule(_vertexShader);
    vertShaderStageInfo.pName = "main";
    
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = createShaderModule(_fragmentShader);
    fragShaderStageInfo.pName = "main";
    
    VkPipelineShaderStageCreateInfo shaderStages[] = {
        vertShaderStageInfo,
        fragShaderStageInfo
    };
    
    VkVertexInputBindingDescription vkVertexInputBindingDescription{};
    vkVertexInputBindingDescription.binding = 0;
    vkVertexInputBindingDescription.stride = _vertexInputBindingSize;
    vkVertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    if (_vertexInputBindingSize > 0)
    {
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &vkVertexInputBindingDescription;
    }
    else
    {
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
    }
    if (!_vkVertexInputAttributeDescriptions.empty())
    {
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(_vkVertexInputAttributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = _vkVertexInputAttributeDescriptions.data();
    }
    else
    {
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    }
    
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(_renderTarget->getSize().x);
    viewport.height = static_cast<float>(_renderTarget->getSize().y);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent.width = static_cast<uint32_t>(_renderTarget->getSize().x);
    scissor.extent.height = static_cast<uint32_t>(_renderTarget->getSize().y);
    
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
    
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    // rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional
    
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;
    
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts;
    if (!_renderDescriptors.empty())
    {
        for (auto& renderDescriptor : _renderDescriptors)
        {
            vkDescriptorSetLayouts.push_back(renderDescriptor->getVkDescriptorSetLayout());
        }
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(vkDescriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = vkDescriptorSetLayouts.data();
    }
    else
    {
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
    }
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    VkPipelineLayout vkPipelineLayout;
    if (vkCreatePipelineLayout(_renderDevice->getVkDevice(), &pipelineLayoutInfo, _renderDevice->allocator(), &vkPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr; // Optional
    pipelineInfo.layout = vkPipelineLayout;
    pipelineInfo.renderPass = _renderTarget->getVkRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    VkPipeline vkPipeline;
    if (vkCreateGraphicsPipelines(_renderDevice->getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, _renderDevice->allocator(), &vkPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create graphics pipeline!");
    }
    
    auto pipeline = std::make_shared<RenderPipeline>(_renderDevice, std::move(_renderDescriptors), vkPipelineLayout, vkPipeline);

    vkDestroyShaderModule(_renderDevice->getVkDevice(), vertShaderStageInfo.module, nullptr);
    vkDestroyShaderModule(_renderDevice->getVkDevice(), fragShaderStageInfo.module, nullptr);
    return pipeline;
}

VkShaderModule RenderPipelineBuilder::createShaderModule(std::string_view spirvShader)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = spirvShader.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(spirvShader.data());
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(_renderDevice->getVkDevice(), &createInfo, _renderDevice->allocator(), &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module!");
    }
    return shaderModule;
}

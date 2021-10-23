#pragma once

#include "render_pipeline.h"
#include <vulkan/vulkan.h>
#include <string_view>

class RenderDevice;
class RenderPass;

class RenderPipelineBuilder
{
public:
    RenderPipelineBuilder(RenderDevice& renderDevice, const RenderPass& renderPass);
    
    RenderPipelineBuilder& setVertexShader(std::string_view spirvShader);
    
    RenderPipelineBuilder& setFragmentShader(std::string_view spirvShader);
    
    RenderPipeline build();

private:
    VkShaderModule createShaderModule(std::string_view spirvShader);

    RenderDevice& _renderDevice;
    const RenderPass& _renderPass;
    std::string_view _vertexShader;
    std::string_view _fragmentShader;
};

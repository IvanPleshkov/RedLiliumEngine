#pragma once

#include "render_pipeline.h"
#include <vulkan/vulkan.h>
#include <string_view>
#include <memory>

class RenderDevice;
class RenderTarget;

class RenderPipelineBuilder
{
public:
    RenderPipelineBuilder(const std::shared_ptr<RenderDevice>& renderDevice, const std::shared_ptr<RenderTarget>& renderTarget);
    
    RenderPipelineBuilder& setVertexShader(std::string_view spirvShader);
    
    RenderPipelineBuilder& setFragmentShader(std::string_view spirvShader);
    
    std::shared_ptr<RenderPipeline> build();

private:
    VkShaderModule createShaderModule(std::string_view spirvShader);

    std::shared_ptr<RenderDevice> _renderDevice;
    std::shared_ptr<RenderTarget> _renderTarget;
    std::string_view _vertexShader;
    std::string_view _fragmentShader;
};

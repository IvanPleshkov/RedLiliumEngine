#pragma once

#include <vulkan/vulkan.h>

class RenderDevice;

class RenderPipeline
{
public:
    RenderPipeline(RenderDevice& renderDevice);
    
    virtual ~RenderPipeline();

private:
    void init();

    void destroy();

    RenderDevice& _renderDevice;
};

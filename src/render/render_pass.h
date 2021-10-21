#pragma once

#include <vulkan/vulkan.h>

class RenderDevice;

class RenderPass
{
public:
    RenderPass(RenderDevice& renderDevice);
    
    ~RenderPass();

private:
    void init();

    void destroy();

    RenderDevice& _renderDevice;
};

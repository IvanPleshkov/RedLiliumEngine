#include "render_target_builder.h"
#include "render_target.h"
#include "render_device.h"

RenderTargetBuilder::RenderTargetBuilder(const std::shared_ptr<RenderDevice>& renderDevice)
    : _renderDevice(renderDevice)
{}

RenderTargetBuilder& RenderTargetBuilder::setVkFormat(VkFormat vkFormat)
{
    _vkFormat = vkFormat;
    return *this;
}

RenderTargetBuilder& RenderTargetBuilder::setSize(glm::ivec2 size)
{
    _size = size;
    return *this;
}

RenderTargetBuilder& RenderTargetBuilder::addImageView(VkImageView vkImageView)
{
    _vkImageViews.push_back(vkImageView);
    return *this;
}

RenderTargetBuilder& RenderTargetBuilder::enableDepth(bool enable)
{
    _enableDepth = enable;
    return *this;
}

RenderTargetBuilder& RenderTargetBuilder::addMultisampling(VkSampleCountFlagBits vkSampleCountFlagBits)
{
    _vkSampleCountFlagBits = vkSampleCountFlagBits;
    return *this;
}

RenderTargetBuilder& RenderTargetBuilder::setFinalColorLayout(VkImageLayout finalColorLayout)
{
    _finalColorLayout = finalColorLayout;
    return *this;
}

RenderTargetBuilder& RenderTargetBuilder::setClearColor(const glm::vec4& clearColor)
{
    _clearColor = clearColor;
    return *this;
}

std::shared_ptr<RenderTarget> RenderTargetBuilder::build()
{
    return std::make_shared<RenderTarget>(_renderDevice, _vkImageViews, _vkFormat, _size, _vkSampleCountFlagBits, _enableDepth, _finalColorLayout, _clearColor);
}

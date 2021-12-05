#include "gpu_texture.h"
#include "gpu_buffer.h"
#include "render_device.h"
#include "render_step.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cmath>

GpuTexture::GpuTexture(const std::shared_ptr<RenderDevice>& renderDevice, bool generateMips)
    : _renderDevice(renderDevice)
    , _generateMips(generateMips)
{ }

GpuTexture::~GpuTexture()
{
    destroy();
}

glm::ivec2 GpuTexture::getSize() const
{
    return _size;
}

uint32_t GpuTexture::getMipsCount() const
{
    return _mipLevels;
}

VkImage GpuTexture::getVkImage() const
{
    return _vkImage;
}

VkImageView GpuTexture::getVkImageView() const
{
    return _vkImageView;
}

VkSampler GpuTexture::getVkSampler() const
{
    return _vkSampler;
}

void GpuTexture::upload(std::string_view textureData)
{
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(textureData.data()), static_cast<int>(textureData.size()), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    _size.x = texWidth;
    _size.y = texHeight;
    VkDeviceSize imageSize = texWidth * texHeight * 4;
    if (!pixels)
    {
        throw std::runtime_error("failed to load texture image!");
    }
    if (_generateMips)
    {
        _mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
    }
    else
    {
        _mipLevels = 1;
    }

    auto stagingBuffer = std::make_shared<GpuBuffer>(_renderDevice, GpuBuffer::Uniform, false);
    stagingBuffer->update(reinterpret_cast<const char*>(pixels), texWidth * texHeight * 4);
    stbi_image_free(pixels);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(texWidth);
    imageInfo.extent.height = static_cast<uint32_t>(texHeight);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = _mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    if (_generateMips)
    {
        imageInfo.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;
    if (vkCreateImage(_renderDevice->getVkDevice(), &imageInfo, _renderDevice->allocator(), &_vkImage) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_renderDevice->getVkDevice(), _vkImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = _renderDevice->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(_renderDevice->getVkDevice(), &allocInfo, _renderDevice->allocator(), &_vkImageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(_renderDevice->getVkDevice(), _vkImage, _vkImageMemory, 0);
    
    auto renderStep = std::make_shared<RenderStep>(_renderDevice, _renderDevice->getGraphicsVkQueue().first, _renderDevice->getGraphicsVkQueue().second);

    renderStep->transitionImageLayout(shared_from_this(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, _mipLevels);
    renderStep->copyBufferToImage(stagingBuffer, shared_from_this(), 0);

    if (_generateMips)
    {
        renderStep->generateMipmaps(shared_from_this());
    }
    else
    {
        renderStep->transitionImageLayout(shared_from_this(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, _mipLevels);
    }
    
    renderStep->run(VK_NULL_HANDLE, VK_NULL_HANDLE);
    renderStep = nullptr;
    
    createImageView();
    createSampler();
}

void GpuTexture::destroy()
{
    if (_vkSampler != VK_NULL_HANDLE)
    {
        vkDestroySampler(_renderDevice->getVkDevice(), _vkSampler, _renderDevice->allocator());
        _vkSampler = VK_NULL_HANDLE;
    }
    if (_vkImageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(_renderDevice->getVkDevice(), _vkImageView, _renderDevice->allocator());
        _vkImageView = VK_NULL_HANDLE;
    }
    if (_vkImage != VK_NULL_HANDLE)
    {
        vkDestroyImage(_renderDevice->getVkDevice(), _vkImage, _renderDevice->allocator());
        _vkImage = VK_NULL_HANDLE;
    }
    if (_vkImageMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(_renderDevice->getVkDevice(), _vkImageMemory, _renderDevice->allocator());
        _vkImageMemory = VK_NULL_HANDLE;
    }
}

void GpuTexture::createImageView()
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = _vkImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = _mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    if (vkCreateImageView(_renderDevice->getVkDevice(), &viewInfo, _renderDevice->allocator(), &_vkImageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }
}

void GpuTexture::createSampler()
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = _renderDevice->getMaxAnisotropy();
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    
    if (_mipLevels > 1)
    {
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = static_cast<float>(_mipLevels);
        samplerInfo.mipLodBias = 0.0f;
    }
    
    if (vkCreateSampler(_renderDevice->getVkDevice(), &samplerInfo, _renderDevice->allocator(), &_vkSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

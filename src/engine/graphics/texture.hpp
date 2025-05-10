
#pragma once

#include "command_pool.hpp"
#include "context.hpp"


class TextureImage
{
public:
    TextureImage(GraphicsContext &ctx, CommandPool &commandPool, std::string texturePath);
    ~TextureImage();

    const VkImageView getTextureImageView() const { return textureImageView; }
    const VkSampler getTextureSampler() const { return textureSampler; }

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void createTextureImageView();
    void createTextureSampler();

    //const VkImage &getTextureImage() const { return textureImage; };

private:
    GraphicsContext &ctx;
    CommandPool &commandPool;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;
};


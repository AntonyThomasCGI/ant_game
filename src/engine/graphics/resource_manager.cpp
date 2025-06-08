
#include "resource_manager.hpp"


std::map<std::string, Shader> ResourceManager::Shaders;

std::map<std::string, TextureImage*> ResourceManager::Textures;



TextureImage* ResourceManager::createTexture(GraphicsContext &ctx, CommandPool &commandPool, std::string texturePath)
{
    Textures[texturePath] = new TextureImage(ctx, commandPool, texturePath);

    // TODO, idk how we delete these in a singleton.

    return Textures[texturePath];
}


TextureImage* ResourceManager::getTexture(std::string texturePath)
{
    return Textures[texturePath];
}


Shader ResourceManager::createShader()
{

}


Shader ResourceManager::getShader()
{

}

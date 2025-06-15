
#pragma once

#include <map>
#include <string>

#include "shader.hpp"
#include "texture.hpp"


class ResourceManager
{
    public:
        static std::map<std::string, Shader> Shaders;
        static std::map<std::string, TextureImage*> Textures;

        static TextureImage* createTexture(GraphicsContext &ctx, CommandPool &commandPool, std::string texturePath);

        static TextureImage* getTexture(std::string texturePath);

        //static Shader createShader();

        //static Shader getShader();

    private:
        // Singleton
        ResourceManager() {};
};


#pragma once

#include <filesystem>
#include <map>
#include <string>

#include "graphics/texture.hpp"
#include "graphics/shader.hpp"


class ResourceManager
{
public:
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, TextureImage> Textures;

    static void setRootPath(std::filesystem::path newPath);

    static std::filesystem::path rootPath;

private:
    // Do not allow class to be instantiated, all methods should be static
    ResourceManager() { };
};

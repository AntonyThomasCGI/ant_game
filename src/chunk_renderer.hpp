#ifndef CHUNK_RENDERER_HPP
#define CHUNK_RENDERER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"
#include "shader.hpp"


class ChunkRenderer
{
public:
    // Constructor (inits shaders/shapes)
    ChunkRenderer(Shader &shader, Texture2D &mapTex);
    // Destructor
    ~ChunkRenderer();
    // Renders a defined quad textured with given sprite
    void DrawChunk(Texture2D &texture1, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    // Render state
    Shader       shader;
    Texture2D    mapTex;
    unsigned int quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif

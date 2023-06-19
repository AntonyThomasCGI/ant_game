
#include "chunk_renderer.hpp"

#include <iostream>


ChunkRenderer::ChunkRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}


ChunkRenderer::~ChunkRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}


void ChunkRenderer::DrawChunk(Texture2D &texture1, Texture2D &texture2, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    this->shader.Use();
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    transform = glm::translate(transform, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    transform = glm::rotate(transform, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    transform = glm::translate(transform, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    transform = glm::scale(transform, glm::vec3(size, 1.0f)); // last scale

    this->shader.SetMatrix4("transform", transform);

    // render textured quad
    //this->shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0 + 0);
    texture1.Bind();

    glActiveTexture(GL_TEXTURE0 + 1);
    texture2.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


void ChunkRenderer::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;

    //float x1, x2, y1, y2;
    //float tileX = 5.0;
    //float tileY = 1.0;

    //x1 = (32.0f * (tileX - 1.0f)) / (32.0f * 5.0f);
    //x2 = (32.0f * tileX) / (32.0f * 5.0f);
    //y1 = (32.0f * (tileY - 1.0f)) / (32.0f * 3.0f);
    //y2 = (32.0f * tileY) / (32.0f * 3.0f);

    //std::cout << "x: " << x1 << " <---> " << x2 << std::endl;
    //std::cout << "y: " << y1 << " <---> " << y2 << std::endl;

    // Use vert pos as texture coord.
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, //0.0f, 1.0f,// x1, y2,
        1.0f, 0.0f, //1.0f, 0.0f,// x2, y1,
        0.0f, 0.0f, //0.0f, 0.0f,// x1, y1,

        0.0f, 1.0f, //0.0f, 1.0f,// x1, y2,
        1.0f, 1.0f, //1.0f, 1.0f,// x2, y2,
        1.0f, 0.0f, //1.0f, 0.0f,// x2, y1,
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
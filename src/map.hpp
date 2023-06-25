#ifndef MAP_HPP
#define MAP_HPP

#include <glm/glm.hpp>

#include "chunk_renderer.hpp"


class Map
{
public:
    int **grid;
    unsigned int Width;
    unsigned int Height;
    ChunkRenderer *chunkRenderer;

    Map(unsigned int width, unsigned int height, Shader &shader, int noiseDensity=50, int iterations=1);
    virtual ~Map();

    void drawChunks();

private:
    void generate(int noiseDensity=50, int iterations=0);
    void calculateTileIDs();
    void copyGrid(int **grid, int **newGrid);
    int **randomNoiseGrid(int noiseDensity);
    int **allocateGrid();
};

#endif

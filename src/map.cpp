
#include "map.hpp"
#include "texture.hpp"
#include "resource_manager.hpp"

#include <stdio.h>
#include <iostream>
#include <random>
#include <exception>


Map::Map(unsigned int width, unsigned int height, Shader &shader, int noiseDensity, int iterations)
{
    if (width % CHUNKSIZE != 0 || height % CHUNKSIZE != 0) {
        std::cout << "Error: Width, Height for map must be divisible by CHUNKSIZE " << CHUNKSIZE << std::endl;
        exit(EXIT_FAILURE);
    }

    int chunksX = width / CHUNKSIZE;
    int chunksY = height / CHUNKSIZE;

    this->Width = width;
    this->Height = height;
    this->generate(noiseDensity, iterations);

    shader.SetInteger("mapTex", 1);

    for (int y = 0; y < chunksY; y++) {
        for (int x = 0; x < chunksX; x++) {
            int **chunkData = create2DArray<int>(CHUNKSIZE, CHUNKSIZE);
            for (int i = 0; i < CHUNKSIZE; i++) {
                for (int j = 0; j < CHUNKSIZE; j++) {
                    int indexIntoGridY = CHUNKSIZE * y + i;
                    int indexIntoGridX = CHUNKSIZE * x + j;

                    chunkData[i][j] = grid[indexIntoGridY][indexIntoGridX];
                }
            }
            Texture2D mapTexture;
            mapTexture.Filter_Min = GL_NEAREST;
            mapTexture.Filter_Max = GL_NEAREST;
            mapTexture.Internal_Format = GL_R32I;
            mapTexture.Image_Format = GL_RED_INTEGER;
            mapTexture.Generate(CHUNKSIZE, CHUNKSIZE, chunkData);

            ChunkRenderer *chunkRenderer = new ChunkRenderer(shader, mapTexture);
            this->chunkRenderers.push_back(chunkRenderer);
        }
    }
}


Map::~Map()
{
    delete2DArray(grid);

    for (ChunkRenderer* cr : this->chunkRenderers) {
        delete cr;
    }
    this->chunkRenderers.clear();
}


void Map::drawChunks(glm::vec2 cameraTransform)
{
    float chunksX = this->Width / CHUNKSIZE;
    float chunksY = this->Height / CHUNKSIZE;

    unsigned int i = 0;
    for (ChunkRenderer* chunkRenderer : this->chunkRenderers) {
        // Get coordinates for current chunks from index.
        int xChunkOffset = i % int(chunksX);
        int yChunkOffset = int(i - xChunkOffset) / chunksX;

        // Get coordinates in pixels and center chunks by subtracting half of total chunks.
        int xOffset = (xChunkOffset - chunksX / 2.0f) * CHUNKWIDTH;
        int yOffset = (yChunkOffset - chunksY / 2.0f) * CHUNKWIDTH;

        chunkRenderer->DrawChunk(
            ResourceManager::GetTexture("dirt_tiles"),
            glm::vec2(xOffset, yOffset) + cameraTransform,
            glm::vec2(CHUNKWIDTH)
        );

        i += 1;
    }
}


/**
 * @brief Generate grid of random noise.
 *
 * @param width
 * @param height
 * @param noiseDensity Density of initial noise, provided as percentage from 0-100.
 * @return int**
 */
int** Map::randomNoiseGrid(unsigned int width, unsigned int height, int noiseDensity)
{
    //int **randomGrid = allocateGrid(width, height);
    int **randomGrid = create2DArray<int>(width, height);

    std::random_device rand_dev;
    std::mt19937 gen(rand_dev());
    std::uniform_int_distribution<int> distr(0, 99);

    for (int i = 0; i < height; i++ ) {
        for (int j = 0; j < width; j++ ) {
            distr(gen) >= noiseDensity ? randomGrid[i][j] = 1 : randomGrid[i][j] = 0;
        }
    }

    return randomGrid;
}


void Map::copyGrid(int **oldGrid, int **newGrid) {
    // Copy
    for (int i = 0; i < this->Height; i++ ) {
        for (int j = 0; j < this->Width; j++ ) {
            newGrid[i][j] = oldGrid[i][j];
        }
    }
}


void printGrid(int **grid, unsigned int width, unsigned int height, int pointAtX = -1, int pointAtY = -1) {
    //std::cout << "==grid==" << std::endl;
    for (int i = 0; i < height; i++ ) {
        for (int j = 0; j < width; j++ ) {
            std::string c;
            if (i == pointAtY && j == pointAtX) {
                c = "X";
            } else {
                c = grid[i][j] == 0 ? "#" : " ";
            }
            std::cout << c;
            //std::cout << "[" << grid[i][j] << "]";
        }
        std::cout << std::endl;
    }
}


void Map::calculateTileIDs() {
    int ** tmpGrid = create2DArray<int>(this->Width, this->Height);

    // Represent each possible neighboring wall as a bit flag.
    const unsigned char N = 0x01;
    const unsigned char E = 0x02;
    const unsigned char S = 0x04;
    const unsigned char W = 0x08;

    std::random_device rand_dev;
    std::mt19937 gen(rand_dev());
    std::uniform_int_distribution<int> distr(1, 200);

    copyGrid(grid, tmpGrid);

    for (int row = 0; row < this->Height; row++ ) {
        for (int col = 0; col < this->Width; col++ ) {
            if (tmpGrid[row][col]) {
                // Tile is wall, randomize the wall texture ID.
                if (distr(gen) == 1) {
                    grid[row][col] = 8;
                } else {
                    grid[row][col] = 0;
                }
                continue;
            }

            // Initialize our 8-bit char to 00000000.
            unsigned char walls = 0x00;

            // North
            if (row < this->Width - 1) {
                if (tmpGrid[row + 1][col]) {
                    walls |= S;  // Open GL flips iamge so reverse N/S
                }
            }
            // South
            if (row > 0) {
                if (tmpGrid[row - 1][col]) {
                    walls |= N;  // Open GL flips iamge so reverse N/S
                }
            }
            // West
            if (col > 0) {
                if (tmpGrid[row][col - 1]) {
                    walls |= W;
                }
            }
            // East
            if (col < this->Width - 1) {
                if (tmpGrid[row][col + 1]) {
                    walls |= E;
                }
            }

            // Check for every possible combination of NESW and set corresponding texture ID.
            switch (walls) {
                case 0x00:
                    grid[row][col] = 18;
                    break;
                case N:
                    grid[row][col] = 3;
                    break;
                case (N | E):
                    grid[row][col] = 4;
                    break;
                case E:
                    grid[row][col] = 9;
                    break;
                case (S | E):
                    grid[row][col] = 14;
                    break;
                case S:
                    grid[row][col] = 13;
                    break;
                case (S | W):
                    grid[row][col] = 12;
                    break;
                case W:
                    grid[row][col] = 7;
                    break;
                case (N | W):
                    grid[row][col] = 2;
                    break;
                case (N | S):
                    grid[row][col] = 15;
                    break;
                case (E | W):
                    grid[row][col] = 16;
                    break;
                case (N | W | E):
                    grid[row][col] = 1;
                    break;
                case (S | W | E):
                    grid[row][col] = 6;
                    break;
                case (N | W | S):
                    grid[row][col] = 10;
                    break;
                case (N | E | S):
                    grid[row][col] = 11;
                    break;
                case (N | E | S | W):
                    grid[row][col] = 5;
                    break;
            }
        }
    }

    // Clean up.
    delete2DArray(tmpGrid);
}


/**
 * @brief Generate a random map using cellular automaton.
 * Calling code is responsible for freeing the array after it is no longer needed.
 *
 * @param noiseDensity Density of initial noise, provided as percentage from 0-100.
 * @param iterations Number cellular automaton iterations.
 */
void Map::generate(int noiseDensity, int iterations)
{
    grid = randomNoiseGrid(this->Width, this->Height, noiseDensity);
    int **tmpGrid = create2DArray<int>(this->Width, this->Height);

    for (int i = 0; i < iterations; i++ ) {
        copyGrid(grid, tmpGrid);
        for (int row = 0; row < this->Height; row++ ) {
            for (int col = 0; col < this->Width; col++ ) {
                // Sum number of adjacent walls in 3x3 area at point (col, row).
                int numNeighbours = 0;
                for (int y = row - 1; y <= row + 1; y++ ) {
                    for (int x = col - 1; x <= col + 1; x++ ) {
                        if (x >= 0 && x < this->Width && y >= 0 && y < this->Height) {
                            if (x != col || y != row) { // Skip checking (col, row) itself.
                                // Wall is 1, non-wall is 0, so we can just add current adjacent wall.
                                numNeighbours = numNeighbours + tmpGrid[y][x];
                            }
                        } else {
                            numNeighbours++;
                            continue;
                        }
                    }
                }
                // If sum of adjacent walls is > 4, we set current col, row to wall.
                grid[row][col] = numNeighbours > 4 ? 1 : 0;
            }
        }
    }
    delete2DArray(tmpGrid);

#if 0
    // Generate square border map.
    grid = create2DArray<int>(this->Width, this->Height);
    for (int row = 0; row < this->Height; row++ ) {
        for (int col = 0; col < this->Width; col++ ) {
            if (row < 2 || row > this->Height - 3 || col < 2 || col > this->Width - 3) {
                grid[row][col] = 1;
            } else {
                grid[row][col] = 0;
            }
        }
    }
#endif

    calculateTileIDs();
}


int Map::tileAt(glm::vec2 coords)
{
    // Get number of tiles in grid on x/y.
    float tilesX = this->Width / TILESIZE;
    float tilesY = this->Height / TILESIZE;

    // Convert transform unit coordinates to tile index coordinates.
    float tileCoordX = coords.x / TILESIZE;
    float tileCoordY = coords.y / TILESIZE;

    // Offset coordinates to index grid (ant [0, 0] is middle of grid).
    int gridCoordX = tileCoordX + this->Width / 2;
    int gridCoordY = tileCoordY + this->Height / 2;

    if (gridCoordX < 0 || gridCoordX >= this->Width || gridCoordY < 0 || gridCoordY >= this->Height) {
        return 18;  // Out of map bounds is considered air (id=18).
    }

    return grid[gridCoordY][gridCoordX];
}

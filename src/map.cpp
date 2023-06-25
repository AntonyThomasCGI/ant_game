
#include "map.hpp"
#include "texture.hpp"
#include "resource_manager.hpp"

#include <stdio.h>
#include <iostream>
#include <random>


Map::Map(unsigned int width, unsigned int height, Shader &shader, int noiseDensity, int iterations)
{
    this->Width = width;
    this->Height = height;
    this->generate(noiseDensity, iterations);

    Texture2D mapTexture;
    mapTexture.Filter_Min = GL_NEAREST;
    mapTexture.Filter_Max = GL_NEAREST;
    mapTexture.Internal_Format = GL_R32I;
    mapTexture.Image_Format = GL_RED_INTEGER;
    mapTexture.Generate(width, height, grid);

    // Register map tex in resource manager.
    ResourceManager::Textures["map"] = mapTexture;

    this->chunkRenderer = new ChunkRenderer(shader, mapTexture);
}


Map::~Map()
{
    free(grid);
    delete chunkRenderer;
}


void Map::drawChunks()
{
    this->chunkRenderer->DrawChunk(ResourceManager::GetTexture("dirt_tiles"), glm::vec2(100.0f), glm::vec2(700.0f, 700.0f));
}

int** Map::allocateGrid() {
    int **newGrid;

    newGrid = (int **)calloc((size_t)(this->Height), sizeof(int *));
    if (newGrid == NULL) {
        std::cout << "Could not initialize array" << std::endl;
        exit(EXIT_FAILURE);
    }
    newGrid[0] = (int *)calloc((size_t)(this->Height * this->Width), sizeof(int));
    if (newGrid[0] == NULL) {
        std::cout << "Could not initialize array" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < this->Height; i++ ) {
        newGrid[i] = newGrid[i - 1] + this->Width;
    }

    return newGrid;
}

/**
 * @brief Generate grid of random noise.
 *
 * @param width
 * @param height
 * @param noiseDensity Density of initial noise, provided as percentage from 0-100.
 * @return int**
 */
int** Map::randomNoiseGrid(int noiseDensity)
{
    int **randomGrid = allocateGrid();

    std::random_device rand_dev;
    std::mt19937 gen(rand_dev());
    std::uniform_int_distribution<int> distr(0, 99);

    for (int i = 0; i < this->Height; i++ ) {
        for (int j = 0; j < this->Width; j++ ) {
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


void printGrid(int **grid, unsigned int width, unsigned int height) {
    std::cout << "==grid==" << std::endl;
    for (int i = 0; i < height; i++ ) {
        for (int j = 0; j < width; j++ ) {
            std::string c = grid[i][j] ? "#" : " ";
            std::cout << c;
            //std::cout << "[" << grid[i][j] << "]";
        }
        std::cout << std::endl;
    }
}


void Map::calculateTileIDs() {

    int **tmpGrid = allocateGrid();

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
            if (row < (this->Width - 1)) {
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
            // East
            if (col > 0) {
                if (tmpGrid[row][col - 1]) {
                    walls |= W;
                }
            }
            // West
            if (col < (this->Width - 1)) {
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
    grid = randomNoiseGrid(noiseDensity);
    int **tmpGrid = allocateGrid();

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
    free(tmpGrid);

    calculateTileIDs();
}

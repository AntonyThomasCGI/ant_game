
#include "map.hpp"

#include <stdio.h>
#include <iostream>
#include <random>


int **allocateGrid(unsigned int width, unsigned int height) {
    int **grid;

    grid = (int **)calloc((size_t)(height), sizeof(int *));
    if (grid == NULL) {
        std::cout << "Could not initialize array" << std::endl;
        exit(EXIT_FAILURE);
    }
    grid[0] = (int *)calloc((size_t)(height * width), sizeof(int));
    if (grid[0] == NULL) {
        std::cout << "Could not initialize array" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < height; i++ ) {
        grid[i] = grid[i - 1] + width;
    }

    return grid;
}

/**
 * @brief Generate grid of random noise.
 *
 * @param width
 * @param height
 * @param noiseDensity Density of initial noise, provided as percentage from 0-100.
 * @return int**
 */
int **randomNoiseGrid(unsigned int width, unsigned int height, int noiseDensity)
{
    int **grid = allocateGrid(width, height);

    std::random_device rand_dev;
    std::mt19937 gen(rand_dev());
    std::uniform_int_distribution<int> distr(0, 99);

    for (int i = 0; i < height; i++ ) {
        for (int j = 0; j < width; j++ ) {
            distr(gen) >= noiseDensity ? grid[i][j] = 1 : grid[i][j] = 0;
        }
    }

    return grid;
}


void copyGrid(int **grid, int **newGrid, unsigned int width, unsigned int height) {
    // Copy
    for (int i = 0; i < height; i++ ) {
        for (int j = 0; j < width; j++ ) {
            newGrid[i][j] = grid[i][j];
        }
    }
}


void printGrid(int **grid, unsigned int width, unsigned int height) {
    std::cout << "==grid==" << std::endl;
    for (int i = 0; i < height; i++ ) {
        for (int j = 0; j < width; j++ ) {
            std::string c = grid[i][j] ? "#" : " ";
            std::cout << c;
        }
        std::cout << std::endl;
    }
}


/**
 * @brief Generate a random map of given `width` `height`.
 * Calling code is responsible for freeing the array after it is no longer needed.
 *
 * @param width Map width.
 * @param height Map height.
 * @param noiseDensity Density of initial noise, provided as percentage from 0-100.
 * @param iterations Number iterations for cellular automaton algoritm.
 * @return int** Map represented by 2D array of integers, walls are represented as 1's non-walls are 0's.
 */
int **Map::Generate(unsigned int width, unsigned int height, int noiseDensity, int iterations)
{
    int **grid = randomNoiseGrid(width, height, noiseDensity);
    int **tmpGrid = allocateGrid(width, height);

    for (int i = 0; i < iterations; i++ ) {
        copyGrid(grid, tmpGrid, width, height);
        for (int row = 0; row < height; row++ ) {
            for (int col = 0; col < width; col++ ) {
                // Sum number of adjacent walls in 3x3 area at point (col, row).
                int numNeighbours = 0;
                for (int y = row - 1; y <= row + 1; y++ ) {
                    for (int x = col - 1; x <= col + 1; x++ ) {
                        if (x >= 0 && x < width && y >= 0 && y < height) {
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

    return grid;
}

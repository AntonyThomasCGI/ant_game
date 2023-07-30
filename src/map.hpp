#ifndef MAP_HPP
#define MAP_HPP

#include <vector>

#include <glm/glm.hpp>

#include "chunk_renderer.hpp"

#define CHUNKSIZE 16  // Number of tiles in chunk.
#define CHUNKWIDTH 800  // Size of chunk in units.
#define TILESIZE 50  // Size of tiles in units (CHUNKWIDTH / CHUNKSIZE).


// https://stackoverflow.com/a/21944048
template <typename T>
T** create2DArray(unsigned nrows, unsigned ncols, const T& val = T())
{
   if (nrows == 0)
        throw std::invalid_argument("number of rows is 0");
   if (ncols == 0)
        throw std::invalid_argument("number of columns is 0");
   T** ptr = nullptr;
   T* pool = nullptr;
   try
   {
       ptr = new T*[nrows];  // allocate pointers (can throw here)
       pool = new T[nrows*ncols]{val};  // allocate pool (can throw here)

       // now point the row pointers to the appropriate positions in
       // the memory pool
       for (unsigned i = 0; i < nrows; ++i, pool += ncols )
           ptr[i] = pool;

       // Done.
       return ptr;
   }
   catch (std::bad_alloc& ex)
   {
       delete [] ptr; // either this is nullptr or it was allocated
       throw ex;  // memory allocation error
   }
}

template <typename T>
void delete2DArray(T** arr)
{
   delete [] arr[0];  // remove the pool
   delete [] arr;     // remove the pointers
}


class Map
{
public:
    int **grid;
    unsigned int Width;
    unsigned int Height;
    std::vector<ChunkRenderer*> chunkRenderers;

    Map(unsigned int width, unsigned int height, Shader &shader, int noiseDensity=50, int iterations=1);
    virtual ~Map();

    void drawChunks(glm::vec2 cameraTransform=glm::vec2(0.0f));

    int tileAt(glm::vec2 coords);

private:
    void generate(int noiseDensity=50, int iterations=0);
    void calculateTileIDs();
    void copyGrid(int **grid, int **newGrid);
    int **randomNoiseGrid(unsigned int width, unsigned int height, int noiseDensity);
};

#endif

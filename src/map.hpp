#ifndef MAP_HPP
#define MAP_HPP

#define MAP(width, height, A) {int i,j,_ii,_jj;for(i=0,_ii=width;i<_ii;i++)for(j=0,_jj=height;j<_jj;j++){A};}


class Map
{
public:
    static int **Generate(unsigned int width, unsigned int height, int noiseDensity=50, int iterations=0);

private:
    Map() { }
};

#endif

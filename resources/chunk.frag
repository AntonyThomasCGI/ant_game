#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D tileTex;
uniform isampler2D mapTex;


void main()
{
    // Tiles are 32x32 pixels.
    const float tileWidth = 32.0;

    // Get dimensions of tile texture in number of tiles.
    vec2 tileDimensions = textureSize(tileTex, 0) / tileWidth;

    // Get tile index we pre-calculated on CPU.
    int tileIndex = texture(mapTex, TexCoords).r;

    // Convert index of tile to x, y coordinates in 'tile space'.
    int tileX = int(tileIndex) % int(tileDimensions.x);
    int tileY = (int(tileIndex) - tileX) / int(tileDimensions.x);
    vec2 tileXY = vec2(tileX, tileY);

    // Get number of tiles in current chunk by checking map x coordinate.
    float nTiles = float(textureSize(mapTex, 0).x);

    // Map 0->1 UV space into n chunks of 0->1 so that 0->1/n, 1/n->2/n ... n-1/n->n
    // all become 0-->1 where n = number of tiles on U or V.
    vec2 chunkedUV = (TexCoords - (1.0 / nTiles) * floor(TexCoords * nTiles)) * nTiles;

    // Now can do final calculation to render correct tile for corresponding mapID.
    vec2 tileUV = (chunkedUV / tileDimensions) + ((1.0 / tileDimensions) * tileXY);

    color = texture(tileTex, tileUV);
}

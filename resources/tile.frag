#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D tileTex;
uniform isampler2D mapTex;

uniform vec2 textureDimensions;
//uniform vec3 spriteColor;

void main()
{
    //float tileX = 4.0;
    //float tileY = 0.0;

    // TODO should be determined by mapTex and pre calculated on cpu
    vec2 tiles[256] = vec2[](
        vec2(1.0, 1.0), vec2(0.0, 0.0), vec2(4.0, 2.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(3.0, 2.0), vec2(2.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(3.0, 1.0), vec2(4.0, 0.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(3.0, 0.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(0.0, 1.0), vec2(2.0, 0.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(5.0, 5.0), vec2(2.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(0.0, 0.0), vec2(5.0, 5.0), vec2(5.0, 5.0), vec2(5.0, 5.0), vec2(5.0, 5.0), vec2(4.0, 2.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(0.0, 2.0), vec2(5.0, 5.0), vec2(5.0, 5.0), vec2(5.0, 5.0), vec2(2.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(0.0, 1.0), vec2(5.0, 5.0), vec2(5.0, 5.0), vec2(2.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(0.0, 2.0), vec2(1.0, 2.0), vec2(1.0, 2.0), vec2(2.0, 2.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 1.0)
    );

    float nTiles = sqrt(tiles.length());

    // Get width/height of texture in number of tiles.
    float tileWidth = textureDimensions.x / 32.0;
    float tileHeight = textureDimensions.y / 32.0;

    // Map 0->1 UV space into n x n chunks of 0->1 so that
    // 0->1/n, 1/n->2/n ... n-1/n->n all become 0-->1 where n = number of tiles.
    float chunkedU = (TexCoords.x - (1.0 / nTiles) * floor(TexCoords.x * nTiles)) * nTiles;
    //float chunkedV = (TexCoords.y - (1.0 / nTiles) * floor(TexCoords.y * nTiles - 0.5) / nTiles) * nTiles;
    float chunkedV = (TexCoords.y - (1.0 / nTiles) * floor(TexCoords.y * nTiles)) * nTiles;

    // Get index of tile based on UV position.
    int tileIndex = int(TexCoords.x * nTiles) + int(TexCoords.y * nTiles) * int(nTiles);

    float tileCoordX = (chunkedU / tileWidth) + ((1.0 / tileWidth) * tiles[tileIndex].x);
    //float tileCoordX = (32.0 * (tileX - 1.0)) / (32.0 * 5.0) * TexCoords.x;
    float tileCoordY = (chunkedV / tileHeight) + ((1.0 / tileHeight) * tiles[tileIndex].y);

    if (TexCoords.x >= 0.0) {
        color = texture(tileTex, vec2(tileCoordX, tileCoordY));
    } else {
        int value = texture(mapTex, TexCoords).r;
        if (value > 0) {
            color = vec4(0.15f, 0.125f, 0.1f, 1.0f);
        } else {
            color = vec4(0.6f, 0.5f, 0.4f, 1.0f);

        }
    }
    //color = vec4(1.0, 1.0, 1.0, 1.0);
    //color = vec4(spriteColor, 1.0);
    //color = vec4(0.9, 0.6, 0.1, 1.0);
}

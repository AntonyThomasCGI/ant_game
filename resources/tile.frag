#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D tileTex;
uniform isampler2D mapTex;

uniform vec2 textureDimensions;

void main()
{
    int wallValue = texture(mapTex, TexCoords).r;

    vec2 tile;
    switch (wallValue) {
        case 0:  // Air
            tile = vec2(-1.0, -1.0);  // Use UV that does not exist to render "air".
            break;
        case 1:  // Wall
            tile = vec2(0.0, 0.0);
            break;
        case 4:  // N
            tile = vec2(3.0, 0.0);
            break;
        case 5:  // NE
            tile = vec2(4.0, 0.0);
            break;
        case 10:  // E
            tile = vec2(4.0, 1.0);
            break;
        case 15:  // SE
            tile = vec2(4.0, 2.0);
            break;
        case 14:  // S
            tile = vec2(3.0, 2.0);
            break;
        case 13:  // SW
            tile = vec2(2.0, 2.0);
            break;
        case 8:  // W
            tile = vec2(2.0, 1.0);
            break;
        case 3:  // NW
            tile = vec2(2.0, 0.0);
            break;
        case 16:  // NS
            tile = vec2(0.0, 3.0);
            break;
        case 17:  // EW
            tile = vec2(1.0, 3.0);
            break;
        case 2:  // NWE
            tile = vec2(1.0, 0.0);
            break;
        case 7:  // SWE
            tile = vec2(1.0, 1.0);
            break;
        case 11:  // NWS
            tile = vec2(0.0, 2.0);
            break;
        case 12:  // NES
            tile = vec2(1.0, 2.0);
            break;
        case 6:  // NESW
            tile = vec2(0.0, 1.0);
            break;
    }

    //float nTiles = sqrt(tiles.length());
    float nTiles = 16;

    // Get width/height of texture in number of tiles.
    float tileWidth = textureDimensions.x / 32.0;
    float tileHeight = textureDimensions.y / 32.0;

    // Map 0->1 UV space into n x n chunks of 0->1 so that
    // 0->1/n, 1/n->2/n ... n-1/n->n all become 0-->1 where n = number of tiles.
    float chunkedU = (TexCoords.x - (1.0 / nTiles) * floor(TexCoords.x * nTiles)) * nTiles;
    //float chunkedV = (TexCoords.y - (1.0 / nTiles) * floor(TexCoords.y * nTiles - 0.5) / nTiles) * nTiles;
    float chunkedV = (TexCoords.y - (1.0 / nTiles) * floor(TexCoords.y * nTiles)) * nTiles;

    // Get index of tile based on UV position.
    //int tileIndex = int(TexCoords.x * nTiles) + int(TexCoords.y * nTiles) * int(nTiles);

    float tileCoordX = (chunkedU / tileWidth) + ((1.0 / tileWidth) * tile.x);
    float tileCoordY = (chunkedV / tileHeight) + ((1.0 / tileHeight) * tile.y);

    color = texture(tileTex, vec2(tileCoordX, tileCoordY));
}

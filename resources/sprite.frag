#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
//uniform vec3 spriteColor;

void main()
{
    color = texture(sprite, TexCoords);
    //color = vec4(1.0, 1.0, 1.0, 1.0);
    //color = vec4(spriteColor, 1.0);
    //color = vec4(0.9, 0.6, 0.1, 1.0);
}

#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform isampler2D checker;
//uniform vec3 spriteColor;

void main()
{
    //int maxValue = 1;
    //int value = texture(checker, TexCoords).r;
    //color = texture(checker, TexCoords);
    //color = vec4(float(value), 1.0f, 1.0f, 1.0f);
    //color = vec4(0.8, 0.4, 0.7, 1.0);
    //float colorValue = float(value) / maxValue;
    //color = vec4(texture(checker, TexCoords), 1.0f);
    int value = texture(checker, TexCoords).r;
    if (value > 0) {
        color = vec4(0.15f, 0.125f, 0.1f, 1.0f);
    } else {
        color = vec4(0.6f, 0.5f, 0.4f, 1.0f);

    }
    //color = vec4(spriteColor, 1.0);
    //color = vec4(0.9, 0.6, 0.1, 1.0);
}

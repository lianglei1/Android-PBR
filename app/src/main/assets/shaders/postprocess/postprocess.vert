#version 300 es

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;


void main()
{
    TexCoords = texCoords;

    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
}

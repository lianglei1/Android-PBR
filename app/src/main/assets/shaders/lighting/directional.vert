#version 300 es
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;
out vec3 envMapCoords;

uniform mat4 inverseView;
uniform mat4 inverseProj;

void main()
{
    TexCoords = texCoords;
    vec4 unprojCoords = (inverseProj * vec4(position, vec2(1.0f)));
    envMapCoords = (inverseView * unprojCoords).xyz;

    gl_Position = vec4(position, 1.0f);
}

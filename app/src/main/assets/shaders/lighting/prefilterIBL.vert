#version 300 es

layout (location = 0) in vec3 position;

out vec3 cubeCoords;

uniform mat4 projection;
uniform mat4 view;


void main()
{
    cubeCoords = position;
    vec4 clipPosition = projection * mat4(mat3(view)) * vec4(cubeCoords, 1.0f);

    gl_Position = clipPosition.xyww;
}

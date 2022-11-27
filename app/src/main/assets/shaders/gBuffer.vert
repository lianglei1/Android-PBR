#version 300 es

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoords;

out vec3 viewPos;
out vec2 TexCoords;
out vec3 normal;
out vec4 fragPosition;
out vec4 fragPrevPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 projViewModel;
uniform mat4 prevProjViewModel;


void main()
{
    // View Space
    vec4 viewFragPos = view * model * vec4(position, 1.0f);
    viewPos = viewFragPos.xyz;

    TexCoords = texCoords;

    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    normal = normalMatrix * Normal;

    fragPosition = projViewModel * vec4(position, 1.0f);
    fragPrevPosition = prevProjViewModel * vec4(position, 1.0f);

    gl_Position = projection * viewFragPos;

}

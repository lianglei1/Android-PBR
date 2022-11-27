#version 300 es
out vec4 colorOutput;

uniform vec4 lightColor;


void main()
{
    colorOutput = vec4(lightColor);
}

#version 430 core

out float saoOutput;
in vec2 TexCoords;

int saoQ = 4;
float PI  = 3.14159265359f;
float saoEpsilon = 0.01f;

uniform sampler2D gPosition;
uniform sampler2D gNormal;

uniform int viewportWidth;
uniform int viewportHeight;
uniform int saoSamples;
uniform int saoTurns;
uniform float saoRadius;
uniform float saoBias;
uniform float saoScale;
uniform float saoContrast;


void main(void){
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);

    float saoOcclusion = 0.0f;

    // AlchemyAO XOR hash to randomize our sample offset rotation
    ivec2 saoOffset = ivec2(gl_FragCoord.xy);
    float saoPhi = (30 * saoOffset.x ^ saoOffset.y + 10 * saoOffset.x * saoOffset.y);

    const float saoScreenRadius = -saoRadius * 3500.0f / fragPos.z;   // Kinda hard to properly define the pixel-size of a 1m object at z = −1m, sooo...
    int saoMaxMipLevel = textureQueryLevels(gPosition) - 1;

    for (int i = 0; i < saoSamples; ++i)
    {
        float saoAlpha = 1.0f / saoSamples * (i + 0.5f);
        float saoH = saoScreenRadius * saoAlpha;
        float saoTetha = 2.0f * PI * saoAlpha * saoTurns + saoPhi;
        vec2 saoU = vec2(cos(saoTetha), sin(saoTetha));

        int saoM = clamp(findMSB(int(saoH)) - 4, 0, saoMaxMipLevel);
        vec3 saoSampleOffset = texelFetch(gPosition, ivec2(saoH * saoU + saoOffset) >> saoM, saoM).xyz;
        vec3 saoV = saoSampleOffset - fragPos;

        // AlchemyAO obscurance estimator
        saoOcclusion += max(0.0f, dot(saoV, normal) + (fragPos.z * saoBias)) / (dot(saoV, saoV) + saoEpsilon);
    }

    saoOcclusion = max(0, 1.0f - 2.0f * saoScale / saoSamples * saoOcclusion);
    saoOcclusion = pow(saoOcclusion, saoContrast);

    saoOutput = saoOcclusion;
}

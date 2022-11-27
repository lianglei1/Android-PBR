#version 300 es

in vec2 TexCoords;
out vec2 colorOutput;


const float PI = 3.14159265359f;
const uint numSamples = 1024u;

float saturate(float f);
float radicalInverse_VdC(uint bits);
vec3 computeImportanceSampleGGX(vec2 Xi, float roughness, vec3 N);
vec2 computeHammersley(uint i, uint N);
float computeDistributionGGX(vec3 N, vec3 H, float roughness);
float computeGeometryAttenuationGGXSmith(float NdotL, float NdotV, float roughness);
vec2 computeIntegrateBRDF(float NdotV, float roughness);


void main()
{
    vec2 integratedBRDF = computeIntegrateBRDF(TexCoords.x, TexCoords.y);

    colorOutput = integratedBRDF;
}



float saturate(float f)
{
    return clamp(f, 0.0f, 1.0f);
}


float radicalInverse_VdC(uint bits) // In place of bitfieldreverse()
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);

    return float(bits) * 2.3283064365386963e-10;
}


vec2 computeHammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), radicalInverse_VdC(i));
}


vec3 computeImportanceSampleGGX(vec2 Xi, float roughness, vec3 N)
{
    float alpha = roughness * roughness;

    float anglePhi = 2 * PI * Xi.x;
    float cosTheta = sqrt((1.0f - Xi.y) / (1.0f + (alpha * alpha - 1.0f) * Xi.y));
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

    vec3 H;
    H.x = sinTheta * cos(anglePhi);
    H.y = sinTheta * sin(anglePhi);
    H.z = cosTheta;

    vec3 upDir = abs(N.z) < 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
    vec3 tanX = normalize(cross(upDir, N));
    vec3 tanY = cross(N, tanX);

    return normalize(tanX * H.x + tanY * H.y + N * H.z);
}


float computeDistributionGGX(vec3 N, vec3 H, float roughness)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;

    float NdotH = saturate(dot(N, H));
    float NdotH2 = NdotH * NdotH;

    return (alpha2) / (PI * (NdotH2 * (alpha2 - 1.0f) + 1.0f) * (NdotH2 * (alpha2 - 1.0f) + 1.0f));
}


float computeGeometryAttenuationGGXSmith(float NdotL, float NdotV, float roughness) // Should use another G function here
{
    float NdotL2 = NdotL * NdotL;
    float NdotV2 = NdotV * NdotV;
    float kRough2 = roughness * roughness + 0.0001f;

    float ggxL = (2.0f * NdotL) / (NdotL + sqrt(NdotL2 + kRough2 * (1.0f - NdotL2)));
    float ggxV = (2.0f * NdotV) / (NdotV + sqrt(NdotV2 + kRough2 * (1.0f - NdotV2)));

    return ggxL * ggxV;
}


vec2 computeIntegrateBRDF(float NdotV, float roughness)
{
    vec3 V;
    V.x = sqrt(1.0f - NdotV * NdotV);
    V.y = 0.0f;
    V.z = NdotV;

    vec3 N = vec3(0.0f, 0.0f, 1.0f);
    vec2 brdfLUT = vec2(0.0f);

    for(uint i = 0; i < numSamples; i++)
    {
        vec2 Xi = computeHammersley(i, numSamples);
        vec3 H = computeImportanceSampleGGX(Xi, roughness, N);
        vec3 L = normalize(2.0f * dot(V, H) * H - V);

        float NdotL = saturate(L.z);
        float NdotH = saturate(H.z);
        float VdotH = saturate(dot(V, H));

        if(NdotL > 0.0f)
        {
            float G = computeGeometryAttenuationGGXSmith(NdotL, NdotV, roughness);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0f - VdotH, 5.0f);

            brdfLUT.x += (1.0f - Fc) * G_Vis;
            brdfLUT.y += Fc * G_Vis;
        }
    }

    return brdfLUT / numSamples;
}

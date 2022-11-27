#version 300 es
in vec2 TexCoords;
in vec3 envMapCoords;
out vec4 colorOutput;


float PI  = 3.14159265359f;

// G-Buffer
uniform sampler2D gPosition;
uniform sampler2D gAlbedo;
uniform sampler2D gNormal;
uniform sampler2D gEffects;

uniform sampler2D sao;
uniform sampler2D envMap;
uniform samplerCube envMapIrradiance;
uniform samplerCube envMapPrefilter;
uniform sampler2D envMapLUT;

uniform int gBufferView;
uniform int brdfMaxSamples;
uniform float materialRoughness;
uniform float materialMetallicity;
uniform float ambientIntensity;
uniform vec3 materialF0;
uniform mat4 view;

float Fd90(float NoL, float roughness);
float KDisneyTerm(float NoL, float NoV, float roughness);
vec3 FresnelSchlick(float NdotV, vec3 F0);
vec3 FresnelSchlick(float NdotV, vec3 F0, float roughness);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometryAttenuationGGXSmith(float NdotL, float NdotV, float roughness);
vec3 colorLinear(vec3 colorVector);
float saturate(float f);
vec2 getSphericalCoord(vec3 normalCoord);


void main()
{
    // Retrieve G-Buffer informations
    vec3 viewPos = texture(gPosition, TexCoords).rgb;
    vec3 albedo = colorLinear(texture(gAlbedo, TexCoords).rgb);
    vec3 normal = texture(gNormal, TexCoords).rgb;
    float roughness = texture(gAlbedo, TexCoords).a;
    float metalness = texture(gNormal, TexCoords).a;
    float ao = texture(gEffects, TexCoords).r;
    vec2 velocity = texture(gEffects, TexCoords).gb;
    float depth = texture(gPosition, TexCoords).a;

    float sao = texture(sao, TexCoords).r;
    vec3 envColor = texture(envMap, getSphericalCoord(normalize(envMapCoords))).rgb;

    vec3 color = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    if(depth == 1.0f)
    {
        color = envColor;
    }

    else
    {
        vec3 V = normalize(- viewPos);
        vec3 N = normalize(normal);
        vec3 R = reflect(-V, N);

        float NdotV = max(dot(N, V), 0.0001f);

        // Fresnel (Schlick) computation (F term)
        vec3 F0 = mix(materialF0, albedo, metalness);
        vec3 F = FresnelSchlick(NdotV, F0, roughness);

        // Energy conservation
        vec3 kS = F;
        vec3 kD = vec3(1.0f) - kS;
        kD *= 1.0f - metalness;

        // Diffuse irradiance computation
        vec3 irradiance = texture(envMapIrradiance, N * mat3(view)).rgb;
        diffuse = irradiance * (albedo / PI);

        // Specular radiance computation
        const float MAX_REFLECTION_LOD = 4.0;
        vec3 prefilteredColor = textureLod(envMapPrefilter, R * mat3(view), roughness * MAX_REFLECTION_LOD).rgb;
        vec2 brdf  = texture(envMapLUT, vec2(NdotV, roughness)).rg;
        vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

        color = (diffuse * kD) + specular;
        color *= ao;
    }


    // Switching between the different buffers
    // Final buffer
    if(gBufferView == 1)
        colorOutput = vec4(color, 1.0f);

    // Position buffer
    else if (gBufferView == 2)
        colorOutput = vec4(viewPos, 1.0f);

    // View Normal buffer
    else if (gBufferView == 3)
        colorOutput = vec4(normal, 1.0f);

    // Color buffer
    else if (gBufferView == 4)
        colorOutput = vec4(albedo, 1.0f);

    // Roughness buffer
    else if (gBufferView == 5)
        colorOutput = vec4(vec3(roughness), 1.0f);

    // Metalness buffer
    else if (gBufferView == 6)
        colorOutput = vec4(vec3(metalness), 1.0f);

    // Depth buffer
    else if (gBufferView == 7)
        colorOutput = vec4(vec3(depth/1000.0f), 1.0f);

    // SAO buffer
    else if (gBufferView == 8)
        colorOutput = vec4(vec3(sao), 1.0f);

    // Velocity buffer
    else if (gBufferView == 9)
        colorOutput = vec4(velocity, 0.0f, 1.0f);
}



float Fd90(float NoL, float roughness)
{
    return (2.0f * NoL * roughness) + 0.4f;
}


float KDisneyTerm(float NoL, float NoV, float roughness)
{
    return (1.0f + Fd90(NoL, roughness) * pow(1.0f - NoL, 5.0f)) * (1.0f + Fd90(NoV, roughness) * pow(1.0f - NoV, 5.0f));
}


vec3 FresnelSchlick(float NdotV, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - NdotV, 5.0f);
}


vec3 FresnelSchlick(float NdotV, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0f - NdotV, 5.0f);
}


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;

    float NdotH = saturate(dot(N, H));
    float NdotH2 = NdotH * NdotH;

    return (alpha2) / (PI * (NdotH2 * (alpha2 - 1.0f) + 1.0f) * (NdotH2 * (alpha2 - 1.0f) + 1.0f));
}


float GeometryAttenuationGGXSmith(float NdotL, float NdotV, float roughness)
{
    float NdotL2 = NdotL * NdotL;
    float NdotV2 = NdotV * NdotV;
    float kRough2 = roughness * roughness + 0.0001f;

    float ggxL = (2.0f * NdotL) / (NdotL + sqrt(NdotL2 + kRough2 * (1.0f - NdotL2)));
    float ggxV = (2.0f * NdotV) / (NdotV + sqrt(NdotV2 + kRough2 * (1.0f - NdotV2)));

    return ggxL * ggxV;
}


vec3 colorLinear(vec3 colorVector)
{
    vec3 linearColor = pow(colorVector.rgb, vec3(2.2f));

    return linearColor;
}


float saturate(float f)
{
    return clamp(f, 0.0f, 1.0f);
}


vec2 getSphericalCoord(vec3 normalCoord)
{
    float phi = acos(-normalCoord.y);
    float theta = atan(1.0f * normalCoord.x, -normalCoord.z) + PI;

    return vec2(theta / (2.0f * PI), phi / PI);
}

#version 330 core

#define PI 3.1415926535897932384626433832795

in vec3 Normal;
in vec2 TexCoords;
in vec3 TangentViewDir;
in vec3 TangentFragPos;
in vec3 TangentLightPos;
in mat3 iTBN;

out vec4 FragColor;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D brdfLUT;
uniform samplerCube irradianceMap;
uniform samplerCube environmentMap;
uniform samplerCube preFilterMap;
uniform float heightScale;
uniform bool lightEnabled;

// Trowbridge-Reitz GGX normal distribution function
float DistributionGGX(vec3 N, vec3 H, float roughness);

// Smith's Schlick-GGX geometry function
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);

//Fresnel-Schlick Fresnel equation
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

void main()
{
	vec2 texCoords = TexCoords;

    vec3 albedo = texture(albedoMap, texCoords).rgb;
    float metallic = texture(metallicMap, texCoords).r;
    float roughness = texture(roughnessMap, texCoords).r;
    float ao = texture(aoMap, texCoords).r;

	vec3 lightColor = vec3(2.0, 2.0, 2.0);

	vec3 N = texture(normalMap, texCoords).rgb;
	N = normalize(N * 2.0 - 1.0);
    vec3 V = normalize(TangentViewDir);
	vec3 R = reflect(-V, N);

	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, albedo, metallic);
	vec3 Lo = vec3(0.0);

	// PointLight
	if (lightEnabled) {
		vec3 L = normalize(TangentLightPos - TangentFragPos);
		vec3 H = normalize(V + L);

		float distance = length(TangentLightPos - TangentFragPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lightColor * attenuation;

		// Fresnel equation
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
		// Normal distribution function
		float D = DistributionGGX(N, H, clamp(roughness, 0.025, 1.0));
		// Geometry function
		float G = GeometrySmith(N, V, L, clamp(roughness, 0.025, 1.0));

		//Cook-Torrance BRDF
		vec3 numerator    = D * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular     = numerator / max(denominator, 0.001);

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

    // IBL
	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;
	vec3 worldSpaceNormal = iTBN * N;
	vec3 worldSpaceReflect = iTBN * R;
	vec3 irradiance = texture(irradianceMap, worldSpaceNormal).rgb;
	vec3 diffuse = irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilteredColor = textureLod(preFilterMap, worldSpaceReflect,  roughness * MAX_REFLECTION_LOD).rgb;   
	vec2 envBRDF  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
	vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

	vec3 ambient = (kD * diffuse + specular) * ao;
    
    vec3 color = ambient + Lo;

	FragColor = vec4(color, 1.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom; // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	roughness = clamp(roughness, 0.01, 1.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float a = roughness;
	float k = (a * a) / 2.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

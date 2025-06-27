#version 450 core

//const
const float PI = 3.14159265359;
const int LIGHT_COUNT = 2;

//struct
struct Material{
    sampler2D albedo;
    sampler2D metalrough;
    sampler2D normal;
    sampler2D occlusion;
    sampler2D emissive;
};
struct Light {
  vec3 position;
  vec3 color;
};

//input
layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec2 fragUV;
layout(location = 2) in mat3 fragTBN;

//output
layout(location = 0) out vec4 finalColor;


layout(location = 10) uniform Material material;
//total layout is qualified as [1,5]

layout(location = 15) uniform Light lights[LIGHT_COUNT];

layout(location = 9) uniform vec3 viewPos;

//D: Trowbridge-Reitz GGX
float DistributionGGX(vec3 N, vec3 H, float rough) {
    float a = rough * rough;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return a2 / denom;
}

//G: Schlick-GGX
float GeometrySchlickGGX(float NdotV, float rough) {
    float r = rough+1.0;
    float k = (r*r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float rough) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, rough);
    float ggx2 = GeometrySchlickGGX(NdotL, rough);
    return ggx1 * ggx2;
}

//F: Fresnel equation
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


void main()
{
    vec3 albedo = pow(texture(material.albedo, fragUV).rgb, vec3(2.2));
    float metallic = texture(material.metalrough, fragUV).b; //gltf metallic is blue
    float roughness = texture(material.metalrough, fragUV).g; //gltf metallic is green
    float occlusion = texture(material.occlusion, fragUV).r;
    vec3 emissive = texture(material.emissive, fragUV).rgb;
    vec3 fragNormal = texture(material.normal, fragUV).rgb * 2.0 - 1.0;
    fragNormal = fragTBN * fragNormal;

    vec3 viewDir = normalize(viewPos - fragPos);
    float dotNV = max(dot(fragNormal, viewDir), 0.0);


    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 outRadiance = vec3(0.0);
    for (int i = 0; i < LIGHT_COUNT; i++) {
        // calculate per-light radiance
        vec3 lightDir = normalize(lights[i].position - fragPos);
        vec3 halfDir = normalize(viewDir + lightDir);

        float dist = length(lights[i].position - fragPos);
        float attenuation = 1.0 / (dist * dist);
        vec3 radiance = lights[i].color * attenuation;

        // Cook-Torrance BRDF
        float ndf = DistributionGGX(fragNormal, halfDir, roughness);
        float geometry = GeometrySmith(fragNormal, viewDir, lightDir, roughness);
        vec3 fresnel = fresnelSchlick(max(dot(halfDir, viewDir), 0.0), F0);

        vec3 kS = fresnel;
        vec3 kD = 1.0 - kS;
        kD *= (1.0 - metallic);

        float dotNL = max(dot(fragNormal, lightDir), 0.0);
        vec3 numerator = ndf * geometry * fresnel;
        float denominator = 4.0 * dotNV * dotNL;
        vec3 specular = numerator / max(denominator, 0.001);

        // add to outgoing radiance Lo
        outRadiance += (kD * albedo / PI + specular) * radiance * dotNL;
    }

    vec3 ambient = vec3(0.03) * albedo * occlusion;
    vec3 color = emissive + ambient + outRadiance;

    // Reinhard tone mapping + gamma correction
    color = color / (color + 1.0);
    color = pow(color, vec3(1.0 / 2.2));

    finalColor = vec4(color, 1.0);
}
#version 450 core

layout(location = 0) in vec3 vtxPosition;
layout(location = 1) in vec2 vtxUV;
layout(location = 2) in vec3 vtxNormal;
layout(location = 3) in vec3 vtxTangent;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec2 fragUV;
layout(location = 2) out mat3 fragTBN;

//uniform buffer
layout(binding = 0) uniform Transform{
    mat4 Proj;
    mat4 View;
    mat4 World;
    mat4 Model;
}transform;

void main()
{
    mat4 MVP = transform.Proj * transform.View * transform.World * transform.Model;
    gl_Position = MVP * vec4(vtxPosition.x, vtxPosition.z, vtxPosition.y,1);
    fragPos = (transform.World*transform.Model*vec4(vtxPosition.x, vtxPosition.z, vtxPosition.y,1)).xyz;
    fragUV = vtxUV;

    mat4 invModel = transpose(inverse(transform.World*transform.Model));
    vec3 normal = normalize((invModel*vec4(vtxNormal,0.0)).xyz);
    vec3 tangent = normalize((invModel*vec4(vtxTangent,0.0)).xyz);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 binormal = cross(normal, tangent);
    fragTBN = mat3(tangent, binormal, normal);
}
#version 450 core

layout(binding = 0) uniform ub_block{
    mat4 MVP;
}blocks;

layout(location = 0) in vec3 vtxPosition;
layout(location = 1) in vec2 vtxUV;
layout(location = 2) in vec3 vtxNormal;
layout(location = 3) in vec3 vtxTangent;
layout(location = 0) out vec2 UV;

layout(binding = 2) uniform sampler2D TextureSampler;

void main()
{
    gl_Position = blocks.MVP * vec4(vtxPosition.x, (vtxPosition.z), (vtxPosition.y),1);
    UV = vtxUV;
}
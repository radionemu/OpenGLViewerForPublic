#version 450 core

layout(location = 0) in vec2 vtxPosition;
layout(location = 1) in vec2 vtxUV;
layout(location = 0) out vec2 UV;

layout(binding = 0) uniform sampler2D TextureSampler;

void main()
{
    gl_Position = vec4(vtxPosition.x, vtxPosition.y, 0, 1.0f);
    UV = vtxUV;
}
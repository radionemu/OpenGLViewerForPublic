#version 450 core

#define PI 3.14159265359

layout(binding = 0) uniform ub_block{
    mat4 MVP;
}blocks;

layout(location = 0) in vec3 vtxPosition;
    layout(location = 1) in vec2 vtxUV;
layout(location = 0) out vec2 UV;

layout(binding = 0) uniform sampler2D TextureSampler;

void main()
{
    gl_Position = blocks.MVP * vec4(vtxPosition.x, (vtxPosition.z), (vtxPosition.y),1);

    // float r = length(vtxPosition);//radius of sphere
    // float theta = asin(vtxPosition.y/r);
    // float phi = atan(vtxPosition.x/vtxPosition.z);

    // float u = (PI+phi)/(2*PI); //0<= u <=1
    // float v = ((2/PI)+theta)/PI; //0<= v <=1
    // UV = vec2(u,v);

    UV = vtxUV;
}
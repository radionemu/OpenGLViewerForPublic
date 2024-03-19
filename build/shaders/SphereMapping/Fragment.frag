#version 450 core

layout(binding = 0) uniform ub_block{
    mat4 MVP;
}blocks;

layout(binding = 1) uniform pbrRGB{
    vec4 PBRBaseRGB;
}prb;

layout(location = 0) in vec2 UV;
layout(location = 0) out vec4 color;

layout(binding = 2) uniform sampler2D TextureSampler;

void main()
{
		color = texture(TextureSampler, UV).rgba+prb.PBRBaseRGB.rgba;
		// color = vec4(1.0,1.0,1.0,1.0);
}
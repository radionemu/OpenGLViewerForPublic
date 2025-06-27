#version 450 core

//input
layout(location = 0) in vec2 UV;

//output
layout(location = 0) out vec4 color;

//uniform
layout(binding = 0) uniform ub_block{
    mat4 MVP;
}blocks;

layout(binding = 1) uniform pbrRGB{
    vec4 PBRBaseRGB;
}prb;

layout(binding = 2) uniform sampler2D TextureSampler;

void main()
{
		color = texture(TextureSampler, UV).rgba+prb.PBRBaseRGB.rgba;
        //+prb.PBRBaseRGB.rgba
        // color = blocks.PBRBaseRGB.rgba;

		// color = vec4(UV.x,UV.y,1,1);
}
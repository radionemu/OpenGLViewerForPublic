#version 450 core

layout(location = 0) in vec2 UV;
layout(location = 0) out vec4 color;

layout(binding = 0) uniform sampler2D TextureSampler;

void main()
{
		color = texture(TextureSampler, UV).rgba;
}
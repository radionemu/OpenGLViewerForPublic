#version 450 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

layout(binding = 0)uniform VIEW{
    mat4 view;
}vw;

layout(binding = 1)uniform PROJ{
    mat4 projection;
}pro;

layout(binding = 0) uniform MOD{
    mat4 model;
}model;

void main()
{
    TexCoords = aPos;
    gl_Position = pro.projection * vw.view * model.model * vec4(aPos, 1.0);
    // gl_Position = mvp.mvpmat * vec4(aPos,1.0);
}  
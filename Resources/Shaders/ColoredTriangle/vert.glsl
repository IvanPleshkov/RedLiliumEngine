#version 450 core

in vec3 position;
in vec4 color0;

out vec4 vertexColor;

uniform vec4 g_diffuseColor1;
uniform vec4 g_diffuseColor2;

layout (std140) uniform Transforms
{
    mat4 g_view;
    mat4 g_proj;
    mat4 g_viewProj;
    vec4 g_testColor;
};

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    vertexColor = color0 * g_testColor * g_diffuseColor1;
}

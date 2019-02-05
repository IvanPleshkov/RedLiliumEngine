#version 450 core

#include "Shaders/IncludeTest.glsl"

in vec3 position;
in vec4 color0;
in vec2 texcoord0;

out vec4 vertexColor;
out vec2 uv;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    vertexColor = color0 * g_testColor * g_diffuseColor1;
    uv = texcoord0;
}

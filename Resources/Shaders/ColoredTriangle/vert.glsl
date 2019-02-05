#version 450 core

#include "Shaders/Camera.glsl"

in vec3 position;
in vec4 color0;
in vec2 texcoord0;

out vec4 vertexColor;
out vec2 uv;

uniform mat4 g_model;

void main()
{
    gl_Position = g_proj * g_view * g_model * vec4(position, 1.0f);

    vertexColor = color0 * g_testColor * g_diffuseColor1;
    uv = texcoord0;
}

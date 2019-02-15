#version 450 core

#include "Shaders/Camera.glsl"

in vec3 position;
in vec2 texcoord0;

out vec2 uv;

void main()
{
    gl_Position = g_proj * g_view * g_model * vec4(position, 1.0f);
    uv = texcoord0;
}

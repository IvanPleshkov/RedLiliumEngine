#version 450 core

in vec2 uv;
out vec4 color;

uniform sampler2D g_diffuse;
// uniform sampler2D g_specular;

void main()
{
    color = texture( g_diffuse, uv );
}

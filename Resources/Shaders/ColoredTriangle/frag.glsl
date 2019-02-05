#version 450 core

in vec4 vertexColor;
in vec2 uv;

out vec4 color;

uniform vec4 g_diffuseColor;
uniform vec4 g_diffuseColor1;

uniform sampler2D g_albedo2;
uniform sampler2D g_albedo1;

void main()
{
    vec4 c1 = texture( g_albedo1, uv );
    vec4 c2 = texture( g_albedo2, uv );
    color = mix(c1, c2, c2.a);
}

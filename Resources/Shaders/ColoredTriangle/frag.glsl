#version 450 core

in vec4 vertexColor;

out vec4 color;

uniform vec4 g_diffuseColor;
uniform vec4 g_diffuseColor1;

void main()
{
    color = vertexColor * g_diffuseColor * g_diffuseColor1;
}

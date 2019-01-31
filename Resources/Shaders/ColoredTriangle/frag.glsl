#version 450 core

in vec4 vertexColor;

out vec4 color;

uniform vec4 g_diffuseColor3;
uniform vec4 g_diffuseColor4;

void main()
{
    color = vertexColor;
}

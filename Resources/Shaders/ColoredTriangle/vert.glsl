#version 450 core

layout (location = 0) in vec4 color0;
layout (location = 1) in vec3 position;

out vec4 vertexColor;

uniform vec4 g_diffuseColor;
uniform vec4 g_diffuseColor2;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    // vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
    vertexColor = color0;
}

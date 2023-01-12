#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_ViewProjection;
uniform mat4 model;

void main()
{
    gl_Position = u_ViewProjection * model * vec4(aPos, 1.0);
}
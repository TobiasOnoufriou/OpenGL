#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 tex_coords;

uniform mat4 u_ViewProjection;
uniform mat4 model;

void main()
{
    tex_coords = aTexCoords;    
    gl_Position = u_ViewProjection * model * vec4(aPos, 1.0);
}
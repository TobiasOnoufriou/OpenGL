#version 450 core
out vec4 pixel;

in vec2 tex_coords;

uniform sampler2D texture_diffuse1;
uniform vec4 col = vec4(1.0, 0.0, 0.0, 0.0);

void main() {
  pixel = vec4(0.0, 1.0, 0.0, 0.0);
}
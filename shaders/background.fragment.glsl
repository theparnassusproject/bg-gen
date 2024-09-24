#version 330 core

out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D texture;

void main()
{
  frag_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

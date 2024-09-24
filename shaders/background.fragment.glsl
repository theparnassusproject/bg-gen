#version 330 core

out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D texture_data;

void main()
{
  frag_color = texture(texture_data, tex_coord);
}
